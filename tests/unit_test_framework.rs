mod common;

#[test]
fn utf_tests_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/UnitTestFramework/UTFTests.js",
        baseline_path: Some("chakracore-cxx/test/UnitTestFramework/UTFTests.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
