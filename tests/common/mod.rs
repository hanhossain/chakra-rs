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
    ch.arg(source);

    let output = ch.output().unwrap();
    let out = String::from_utf8_lossy(&output.stdout);

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
    assert_eq!(output.stderr, b"");
    assert!(output.status.success());
}

fn trim_carriage_return(s: &str) -> &str {
    s.trim_end_matches('\r')
}
