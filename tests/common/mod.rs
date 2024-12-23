use std::fs::read_to_string;
use std::path::PathBuf;
use std::process::Command;

#[derive(Debug)]
pub struct Test {
    pub source_path: &'static str,
    pub baseline_path: &'static str,
}

pub fn run_test(test: &Test) {
    let manifest_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let source = manifest_dir.join(test.source_path);
    let baseline = manifest_dir.join(test.baseline_path);
    let out_dir = PathBuf::from(env!("OUT_DIR"));

    let mut ch = Command::new(out_dir.join("build/ch"));
    ch.arg(source)
        .arg("-ExtendedErrorStackForTestHost")
        .arg("-BaselineMode");
    let output = ch.output().unwrap();

    let mut out = String::from_utf8(output.stdout).unwrap();
    let err = std::str::from_utf8(&output.stderr).unwrap();
    out.push_str(err);

    let actual = out
        .lines()
        .map(|s| trim_carriage_return(s))
        .collect::<Vec<_>>();
    let expected = read_to_string(baseline).unwrap();
    let expected = expected
        .lines()
        .map(|s| trim_carriage_return(s))
        .collect::<Vec<_>>();

    assert_eq!(actual, expected);
    assert!(output.status.success());
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
