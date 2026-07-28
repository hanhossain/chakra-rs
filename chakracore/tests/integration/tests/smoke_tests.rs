use crate::common::{init_tracing, run_test};
use chakracore_sys::config::CoreConfig;

#[test]
fn hello() {
    let source = "../chakracore-cxx/test/Basics/hello.js";
    let config = CoreConfig {
        filename: source.to_owned(),
        ..Default::default()
    };

    let _guard = init_tracing();
    let (exit_status, actual) = run_test(config, None);

    let expected = vec!["hello world", "PASS"];
    assert_eq!(actual, expected);
    assert!(exit_status.success());
}
