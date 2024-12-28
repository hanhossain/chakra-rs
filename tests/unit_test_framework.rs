mod common;
const DIRECTORY: &str = "chakracore-cxx/test/UnitTestFramework";

#[test]
fn utf_tests_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UTFTests.js",
        baseline_path: Some("UTFTests.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
