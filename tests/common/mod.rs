use std::fs::read_to_string;
use std::path::PathBuf;
use std::process::Command;

#[derive(Debug, Default)]
pub struct Test {
    pub source_path: &'static str,
    pub baseline_path: Option<&'static str>,
    pub compile_flags: Vec<&'static str>,
}

pub fn run_test(test: &Test) {
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let source = manifest_dir.join(test.source_path);

    let out_dir = PathBuf::from(env!("OUT_DIR"));

    let mut ch = Command::new(out_dir.join("build/ch"));
    ch.arg(source)
        .arg("-ExtendedErrorStackForTestHost")
        .arg("-BaselineMode")
        .args(&test.compile_flags);

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
        let baseline = manifest_dir.join(baseline_path);
        let expected = read_to_string(baseline).unwrap();
        let expected = expected
            .lines()
            .map(|s| trim_carriage_return(s))
            .collect::<Vec<_>>();

        assert_eq!(actual, expected);
    } else {
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
