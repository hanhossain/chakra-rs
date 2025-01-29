use pretty_assertions::{assert_eq, assert_ne};
use std::collections::HashSet;
use std::fs::read_to_string;
use std::path::PathBuf;
use std::process::Command;
use std::time::Duration;

#[allow(dead_code)]
pub const SLOW_TEST_TIMEOUT: Duration = Duration::from_secs(180);

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

        let invalid_tags = self
            .tags
            .iter()
            .filter(|tag| tag.contains(','))
            .collect::<Vec<_>>();
        assert_eq!(empty_vec, invalid_tags, "no commas allowed in tags");
    }
}

#[derive(Debug, PartialEq, Eq)]
pub enum Variant {
    Interpreted,
    Dynapogo,
    DisableJit,
}

struct VariantConfig<'a> {
    compile_flags: Vec<&'a str>,
    excluded_tags: HashSet<&'static str>,
}

pub fn run_test_variant(test: &Test, variant: Variant) {
    if cfg!(disable_jit) && variant != Variant::DisableJit {
        println!("Skipping {variant:?} as it's not supported with cfg!(disable_jit)");
        return;
    } else if !cfg!(disable_jit) && variant == Variant::DisableJit {
        println!("Skipping {variant:?} as it's not supported without cfg!(disable_jit)");
        return;
    }

    test.validate();

    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));

    let test_dir = manifest_dir.join(test.directory);
    let source = test_dir.join(test.source_path);
    println!("source_path: {:?}", source);

    assert!(source.exists());

    let out_dir = PathBuf::from(env!("OUT_DIR"));

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

    let exclude_build_type = if cfg!(optimized) {
        "exclude_test"
    } else {
        "exclude_debug"
    };
    variant_config.excluded_tags.insert(exclude_build_type);
    variant_config
        .excluded_tags
        .insert("exclude_icu62AndAboveTestFailures");

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

    let mut ch = Command::new(out_dir.join("build/ch"));
    ch.current_dir(test_dir)
        .arg(source)
        .arg("-ExtendedErrorStackForTestHost")
        .arg("-BaselineMode")
        .arg("-WERExceptionSupport")
        .args(&test.compile_flags)
        .args(&variant_config.compile_flags);

    if cfg!(unix) {
        ch.env("TZ", "US/Pacific");
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

    if let Some(baseline_path) = test.baseline_path {
        if !baseline_path.is_empty() {
            let baseline = manifest_dir.join(test.directory).join(baseline_path);
            let expected = read_to_string(baseline).unwrap();
            let expected = expected
                .lines()
                .map(|s| trim_carriage_return(s))
                .collect::<Vec<_>>();

            assert_eq!(expected, actual);
        }
    } else {
        println!("Actual output: {:#?}", actual);
        let mut passed = false;
        for line in &actual {
            let lower = line.to_lowercase();
            if lower != "pass" && lower != "passed" && !lower.is_empty() {
                panic!(
                    "Test can only print `pass` or `passed`. Actual: {:?}",
                    actual
                );
            } else {
                passed = true;
            }
        }
        assert!(passed, "Test did not print 'pass' or 'passed'");
    }

    assert!(output.status.success());
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
