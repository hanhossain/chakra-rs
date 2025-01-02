use pretty_assertions::{assert_eq, assert_ne};
use std::fs::read_to_string;
use std::path::PathBuf;
use std::process::Command;

#[derive(Debug, Default)]
pub struct Test {
    pub directory: &'static str,
    pub source_path: &'static str,
    pub baseline_path: Option<&'static str>,
    pub compile_flags: Vec<&'static str>,
    pub tags: Vec<&'static str>,
}

#[derive(Debug)]
enum Variant {
    DisableJit,
    Interpreted,
}

struct VariantConfig<'a> {
    compile_flags: Vec<&'a str>,
    excluded_tags: Vec<&'static str>,
}

pub fn run_test(test: &Test) {
    if cfg!(disable_jit) {
        run_test_variant(test, Variant::DisableJit);
    } else {
        run_test_variant(test, Variant::Interpreted);
    }
}

fn run_test_variant(test: &Test, variant: Variant) {
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));

    assert_ne!(test.directory, "");

    let test_dir = manifest_dir.join(test.directory);
    let source = test_dir.join(test.source_path);
    println!("source_path: {:?}", source);

    assert!(source.exists());

    let out_dir = PathBuf::from(env!("OUT_DIR"));

    let variant_config = match variant {
        Variant::DisableJit => VariantConfig {
            compile_flags: vec!["-nonative"],
            excluded_tags: vec![
                "exclude_disable_jit",
                "exclude_interpreted",
                "fails_interpreted",
                "require_backend",
            ],
        },
        Variant::Interpreted => VariantConfig {
            compile_flags: vec!["-maxInterpretCount:1", "-maxSimpleJitRunCount:1", "-bgjit-"],
            excluded_tags: vec!["exclude_interpreted", "require_disable_jit"],
        },
    };

    if variant_config
        .excluded_tags
        .iter()
        .any(|tag| test.tags.contains(tag))
    {
        println!("Skipping test because it is excluded for the {variant:?} variant");
        return;
    }

    let mut ch = Command::new(out_dir.join("build/ch"));
    ch.current_dir(test_dir)
        .arg(source)
        .arg("-ExtendedErrorStackForTestHost")
        .arg("-BaselineMode")
        .arg("-WERExceptionSupport")
        .args(&test.compile_flags)
        .args(&variant_config.compile_flags);

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
