use crate::common::CH_PATH;
use chakracore_sys::config::CoreConfig;

#[test]
fn hello() {
    let source = "../chakracore-cxx/test/Basics/hello.js";
    let config = CoreConfig {
        filename: source.to_owned(),
        ..Default::default()
    };
    let config = serde_json::to_string(&config).unwrap();

    let mut ch_exe = std::process::Command::new(CH_PATH);
    ch_exe.arg(config);
    let output = ch_exe.output().unwrap();

    let out = String::from_utf8_lossy(&output.stdout);

    let actual = out.lines().collect::<Vec<_>>();
    let expected = vec!["hello world", "PASS"];
    assert_eq!(actual, expected);
    assert_eq!(output.stderr, b"");
    assert!(output.status.success());
}
