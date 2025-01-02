use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/TryCatch";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn try_catch_stack_overflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TryCatchStackOverflow.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
