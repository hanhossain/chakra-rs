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

enum Variant {
    DisableJit,
}

struct VariantConfig {
    compile_flags: Vec<&'static str>,
    excluded_tags: Vec<&'static str>,
}

pub fn run_test(test: &Test) {
    run_test_variant(test, Variant::DisableJit);
}

fn run_test_variant(test: &Test, variant: Variant) {
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let source = manifest_dir.join(test.directory).join(test.source_path);
    println!("source_path: {:?}", source);

    assert!(source.exists());

    let out_dir = PathBuf::from(env!("OUT_DIR"));

    let variant_config = match variant {
        Variant::DisableJit => VariantConfig {
            compile_flags: vec!["-nonative"],
            excluded_tags: vec![
                "exclude_interpreted",
                "fails_interpreted",
                "require_backend",
            ],
        },
    };

    if variant_config
        .excluded_tags
        .iter()
        .any(|tag| test.tags.contains(tag))
    {
        println!("Skipping test because it is excluded for this variant");
        return;
    }

    let mut ch = Command::new(out_dir.join("build/ch"));
    ch.arg(source)
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
        let baseline = manifest_dir.join(test.directory).join(baseline_path);
        let expected = read_to_string(baseline).unwrap();
        let expected = expected
            .lines()
            .map(|s| trim_carriage_return(s))
            .collect::<Vec<_>>();

        assert_eq!(actual, expected);
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
        assert!(passed);
    }

    assert!(output.status.success());
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
