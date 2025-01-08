use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/TryCatch";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn try_catch_stack_overflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TryCatchStackOverflow.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
