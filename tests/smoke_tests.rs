use std::path::PathBuf;

#[test]
fn hello() {
    let out_dir = env!("OUT_DIR");

    let ch = PathBuf::from(out_dir).join("build/ch");

    let source =
        PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("chakracore-cxx/test/Basics/hello.js");

    let mut ch_exe = std::process::Command::new(ch);
    ch_exe.arg(source);
    let output = ch_exe.output().unwrap();

    let out = String::from_utf8_lossy(&output.stdout);

    let actual = out.lines().collect::<Vec<_>>();
    let expected = vec!["hello world", "PASS"];
    assert_eq!(actual, expected);
    assert_eq!(output.stderr, b"");
    assert!(output.status.success());
}
