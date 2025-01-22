use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/UnitTestFramework";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn utf_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UTFTests.js",
        baseline_path: Some("UTFTests.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
