use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 1] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/PRE";

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn pre1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "pre1.js",
        baseline_path: Some("pre1.baseline"),
        compile_flags: vec!["-testtrace:fieldcopyprop", "-oopjit-"],
        tags: HashSet::from([
            "exclude_forceserialized",
            "exclude_dynapogo",
            "require_backend",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug0.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
