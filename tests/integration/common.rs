use pretty_assertions::{assert_eq, assert_ne};
use std::collections::HashSet;
use std::fs::read_to_string;
use std::path::PathBuf;
use std::process::Command;
use std::time::Duration;

pub const SLOW_TEST_TIMEOUT: Duration = Duration::from_secs(180);
pub const DEFAULT_TAGS: [&str; 0] = [];

#[derive(Debug, Default)]
pub struct Test {
    pub directory: &'static str,
    pub source_path: &'static str,
    pub baseline_path: Option<&'static str>,
    pub compile_flags: Vec<&'static str>,
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

struct VariantConfig<'a> {
    compile_flags: Vec<&'a str>,
    excluded_tags: HashSet<&'static str>,
}

pub fn run_test_variant<const N: usize>(
    mut test: Test,
    variant: Variant,
    common_tags: [&'static str; N],
) {
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
        println!("Skipping {variant:?} as it's not supported with cfg!(disable_jit)");
        return;
    } else if !cfg!(disable_jit) && variant == Variant::DisableJit {
        println!("Skipping {variant:?} as it's not supported without cfg!(disable_jit)");
        return;
    }

    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));

    let test_dir = manifest_dir.join(test.directory);
    let source = test_dir.join(test.source_path);
    println!("source_path: {:?}", source);

    assert!(source.exists());

    let out_dir = PathBuf::from(env!("OUT_DIR"));

    if cfg!(feature = "compile-cpp") {
        let mut ch = Command::new(out_dir.join("build/ch"));
        ch.current_dir(test_dir)
            .arg(source)
            .arg("-ExtendedErrorStackForTestHost")
            .arg("-BaselineMode")
            .arg("-WERExceptionSupport")
            .args(&test.compile_flags)
            .args(&variant_config.compile_flags);

        if cfg!(unix) {
            ch.env("TZ", "America/Los_Angeles");
        }

        println!("Running command: {ch:#?}");
        let output = ch.output().unwrap();

        let mut out = String::from_utf8(output.stdout).unwrap();
        let err = std::str::from_utf8(&output.stderr).unwrap();
        out.push_str(err);

        let actual = out
            .lines()
            .map(|s| trim_carriage_return(s))
            .collect::<Vec<_>>();

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

        assert!(output.status.success());
    }
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
