#[cfg(target_arch = "x86_64")]
use common::Variant;
#[cfg(target_arch = "x86_64")]
use rstest::rstest;
#[cfg(target_arch = "x86_64")]
use std::collections::HashSet;

#[cfg(target_arch = "x86_64")]
mod common;
#[cfg(target_arch = "x86_64")]
const DIRECTORY: &str = "chakracore-cxx/test/TaggedFloats";

#[cfg(target_arch = "x86_64")]
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
