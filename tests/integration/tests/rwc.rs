use crate::common;
use crate::common::Variant;
use rstest::rstest;


const DIRECTORY: &str = "chakracore-cxx/test/RWC";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn onenote_ribbon_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OneNote.ribbon.js",
        baseline_path: Some("OneNote.ribbon.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
