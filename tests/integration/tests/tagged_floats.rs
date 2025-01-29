use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/TaggedFloats";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test.js",
        baseline_path: Some("test.baseline"),
        tags: HashSet::from(["exclude_x86", "exclude_arm", "Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
