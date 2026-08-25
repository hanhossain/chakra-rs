use chakracore_sys::config::CoreConfig;
use pretty_assertions::{assert_eq, assert_ne};
use serde::Deserialize;
use std::assert_matches;
use std::collections::{HashMap, HashSet};
use std::fs::read_to_string;
use std::io::{BufRead, BufReader, Read};
use std::path::{Path, PathBuf};
use std::process::{Command, ExitStatus, Stdio};
use std::thread;
use std::time::Duration;
use tracing::dispatcher::DefaultGuard;
use tracing_subscriber::EnvFilter;

pub const CH_PATH: &'static str = env!("CARGO_BIN_EXE_chakracore");
pub const SLOW_TEST_TIMEOUT: Duration = Duration::from_secs(180);
pub const DEFAULT_TAGS: [&str; 0] = [];

#[derive(Debug, Default)]
pub struct Test {
    pub directory: &'static str,
    pub source_path: &'static str,
    pub baseline_path: Option<&'static str>,
    pub compile_flags: Vec<&'static str>,
    pub host_args: Vec<&'static str>,
    pub tags: HashSet<&'static str>,
}

impl Test {
    fn validate(&self) {
        assert_ne!(self.directory, "");
        assert_ne!(self.source_path, "");

        let empty_vec: Vec<&&str> = Vec::new();

        let invalid_flags = self
            .compile_flags
            .iter()
            .filter(|flag| flag.contains(','))
            .collect::<Vec<_>>();
        assert_eq!(
            empty_vec, invalid_flags,
            "no commas allowed in compile flags"
        );

        let trimmed_flags = self
            .compile_flags
            .iter()
            .map(|flag| flag.trim())
            .collect::<Vec<_>>();

        assert_eq!(
            self.compile_flags, trimmed_flags,
            "compile flags must not have leading or trailing whitespace"
        );

        let invalid_tags = self
            .tags
            .iter()
            .filter(|tag| tag.contains(','))
            .collect::<Vec<_>>();
        assert_eq!(empty_vec, invalid_tags, "no commas allowed in tags");

        // debug should only run exclude_test
        if !cfg!(feature = "optimized-tests") {
            assert!(
                self.tags.contains("exclude_test"),
                "Debug must contain exclude_test"
            );
        }

        if self
            .tags
            .iter()
            .map(|t| t.to_lowercase())
            .filter(|t| t == "slow")
            .next()
            .is_some()
            && std::env::var("CHAKRA_TEST_SLOW").is_err()
        {
            panic!("Test is marked as slow but CHAKRA_TEST_SLOW is not set");
        }
    }
}

#[derive(Debug, PartialEq, Eq, Copy, Clone)]
pub enum Variant {
    Interpreted,
    Dynapogo,
    DisableJit,
}

#[derive(Debug)]
struct VariantConfig<'a> {
    compile_flags: Vec<&'a str>,
    excluded_tags: HashSet<&'static str>,
}

pub fn init_tracing() -> DefaultGuard {
    let guard = tracing::subscriber::set_default(
        tracing_subscriber::fmt()
            .with_env_filter(
                EnvFilter::try_from_default_env().unwrap_or_else(|_| EnvFilter::new("trace")),
            )
            .with_test_writer()
            .finish(),
    );
    tracing::info!("starting test");
    guard
}

pub fn run_test_variant<const N: usize>(
    mut test: Test,
    variant: Variant,
    common_tags: [&'static str; N],
) {
    let _subscriber_guard = init_tracing();

    test.tags.extend(common_tags.iter());
    test.validate();

    let mut variant_config = match variant {
        Variant::Interpreted => VariantConfig {
            compile_flags: vec!["-maxInterpretCount:1", "-maxSimpleJitRunCount:1", "-bgjit-"],
            excluded_tags: HashSet::from(["exclude_interpreted", "require_disable_jit"]),
        },
        Variant::Dynapogo => VariantConfig {
            compile_flags: vec!["-forceNative", "-off:simpleJit", "-bgJitDelay:0"],
            excluded_tags: HashSet::from(["exclude_dynapogo", "require_disable_jit"]),
        },
        Variant::DisableJit => VariantConfig {
            compile_flags: vec!["-nonative"],
            excluded_tags: HashSet::from([
                "exclude_disable_jit",
                "exclude_interpreted",
                "fails_interpreted",
                "require_backend",
            ]),
        },
    };

    let exclude_build_type = if cfg!(feature = "optimized-tests") {
        "exclude_test"
    } else {
        "exclude_debug"
    };
    variant_config.excluded_tags.insert(exclude_build_type);
    variant_config
        .excluded_tags
        .insert("exclude_icu62AndAboveTestFailures");

    if cfg!(target_arch = "aarch64") {
        variant_config.excluded_tags.insert("exclude_arm");
        variant_config.excluded_tags.insert("exclude_arm64");
        variant_config.excluded_tags.insert("require_asmjs");
    } else if cfg!(target_arch = "x86_64") {
        variant_config.excluded_tags.insert("exclude_x64");
    }

    if cfg!(unix) {
        variant_config.excluded_tags.insert("exclude_xplat");
        variant_config.excluded_tags.insert("require_winglob");
        variant_config.excluded_tags.insert("require_simd");
    } else if cfg!(windows) {
        variant_config.excluded_tags.insert("exclude_windows");
    }

    let both: HashSet<_> = variant_config
        .excluded_tags
        .intersection(&test.tags)
        .collect();

    // TODO (hanhossain) remove this after removing the exclude_ tags
    assert!(
        both.is_empty(),
        "The following test tags were found in the variant's excluded tags: {:?}",
        both
    );

    if cfg!(disable_jit) && variant != Variant::DisableJit {
        tracing::info!(
            ?variant,
            "Skipping variant as it's not supported with `cfg!(disable_jit)`"
        );
        return;
    } else if !cfg!(disable_jit) && variant == Variant::DisableJit {
        tracing::info!(
            ?variant,
            "Skipping variant as it requires `cfg!(disable_jit)`"
        );
        return;
    }

    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));

    let test_dir = manifest_dir.join(test.directory);
    let source = test_dir.join(test.source_path);

    assert!(source.exists());
    let filename = source.to_str().unwrap().to_owned();

    let mut args = vec![
        "-ExtendedErrorStackForTestHost".to_owned(),
        "-BaselineMode".to_owned(),
        "-WERExceptionSupport".to_owned(),
    ];
    args.extend(test.compile_flags.into_iter().map(String::from));
    args.extend(variant_config.compile_flags.into_iter().map(String::from));

    let core_config = CoreConfig {
        filename,
        args,
        host_args: test.host_args.into_iter().map(String::from).collect(),
    };
    let (status, actual) = run_test(core_config, Some(test_dir.as_path()));

    match test.baseline_path {
        Some(baseline_path) => {
            if !baseline_path.is_empty() {
                let baseline = manifest_dir.join(test.directory).join(baseline_path);
                let expected = read_to_string(baseline).unwrap();
                let expected = expected
                    .lines()
                    .map(|s| trim_carriage_return(s))
                    .collect::<Vec<_>>();

                assert_eq!(expected, actual);
            }
        }
        _ => {
            println!("Actual output: {:#?}", actual);
            let mut passed = false;
            for (index, line) in actual.iter().enumerate() {
                let lower = line.to_lowercase();
                if lower != "pass" && lower != "passed" && !lower.is_empty() {
                    panic!(
                        "Test can only print 'pass' or 'passed'. Index: {index}, output: {line}"
                    );
                } else {
                    passed = true;
                }
            }
            assert!(passed, "Test did not print 'pass' or 'passed'");
        }
    }

    assert!(status.success());
}

#[tracing::instrument(skip_all)]
pub fn run_test(core_config: CoreConfig, test_dir: Option<&Path>) -> (ExitStatus, Vec<String>) {
    tracing::info!(?core_config);

    let serialized_config = serde_json::to_string(&core_config).unwrap();
    let mut ch = Command::new(CH_PATH);

    if let Some(test_dir) = test_dir {
        ch.current_dir(test_dir);
    }

    ch.arg(serialized_config)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .env("TZ", "America/Los_Angeles")
        .env("CHAKRA_TEST", "true");

    tracing::info!(?ch, "Running command");

    let mut child = ch.spawn().unwrap();
    let stdout = child.stdout.take().unwrap();
    let stderr = child.stderr.take().unwrap();

    let dispatcher = tracing::dispatcher::get_default(|dispatch| dispatch.clone());
    let dispatcher2 = dispatcher.clone();

    let stdout_span = tracing::info_span!("stdout_reader");
    let stdout_reader = thread::spawn(move || {
        let _span = stdout_span.entered();
        tracing::dispatcher::with_default(&dispatcher, || read_stdout(stdout))
    });

    let stderr_span = tracing::info_span!("stderr_reader");
    let stderr_reader = thread::spawn(move || {
        let _span = stderr_span.entered();
        tracing::dispatcher::with_default(&dispatcher2, || read_stderr(stderr))
    });

    let actual = stdout_reader.join().unwrap();
    let err_actual = stderr_reader.join().unwrap();
    assert_matches!(err_actual, None);

    let status = child.wait().unwrap();
    tracing::info!(?status, "Child process exited");
    (status, actual)
}

fn read_stdout<R: Read>(stream: R) -> Vec<String> {
    let mut actual = Vec::new();
    let reader = BufReader::new(stream);
    for mut message in reader.lines().map(|line| line.unwrap()) {
        tracing::info!(target: "chakracore stdout", message);
        // HACK: chakracore allows embedded nulls in its strings. This removes the nulls to allow
        // asserting with the test baseline.
        if message.contains('\0') {
            tracing::warn!(message, "message contains null byte");
            message = message.replace('\0', "");
        }
        actual.push(message);
    }
    actual
}

fn read_stderr<R: Read>(stream: R) -> Option<Vec<String>> {
    let mut actual = Vec::new();
    let reader = BufReader::new(stream);
    for message in reader.lines().map(|line| line.unwrap()) {
        match serde_json::from_str::<ChildEvent>(&message) {
            Ok(event) => propagate_child_event(event),
            Err(err) => {
                tracing::error!(%err, message, "Failed to parse message");
                actual.push(message);
            }
        }
    }
    if actual.is_empty() {
        None
    } else {
        Some(actual)
    }
}

fn propagate_child_event(event: ChildEvent) {
    let fields = &event.fields;
    match event.level.as_str() {
        "TRACE" => {
            tracing::trace!(
                target: "chakracore stderr",
                message = fields.message,
                fields.function_name,
                fields.file_name,
                fields.line,
                thread_name = event.thread_name,
                target = event.target,
                span = ?event.span);
        }
        "DEBUG" => {
            tracing::debug!(
                target: "chakracore stderr",
                message = fields.message,
                fields.function_name,
                fields.file_name,
                fields.line,
                thread_name = event.thread_name,
                target = event.target,
                span = ?event.span);
        }
        "INFO" => {
            tracing::info!(
                target: "chakracore stderr",
                message = fields.message,
                fields.function_name,
                fields.file_name,
                fields.line,
                thread_name = event.thread_name,
                target = event.target,
                span = ?event.span);
        }
        "WARN" => {
            tracing::warn!(
                target: "chakracore stderr",
                message = fields.message,
                fields.function_name,
                fields.file_name,
                fields.line,
                thread_name = event.thread_name,
                target = event.target,
                span = ?event.span);
        }
        "ERROR" => {
            tracing::error!(
                target: "chakracore stderr",
                message = fields.message,
                fields.function_name,
                fields.file_name,
                fields.line,
                thread_name = event.thread_name,
                target = event.target,
                span = ?event.span);
        }
        _ => panic!("invalid level: {}", event.level),
    }
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct ChildEvent {
    level: String,
    fields: ChildEventFields,
    target: String,
    #[serde(rename = "threadName")]
    thread_name: String,
    span: Option<HashMap<String, String>>,
    #[serde(rename = "spans")]
    _spans: Option<Vec<HashMap<String, String>>>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct ChildEventFields {
    message: String,
    function_name: Option<String>,
    file_name: Option<String>,
    line: Option<usize>,
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
