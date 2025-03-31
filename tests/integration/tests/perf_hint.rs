use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 3] = ["exclude_serialized", "exclude_snap", "require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/PerfHint";

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn try_with_eval_perfhint_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try_with_eval_perfhint.js",
        baseline_path: Some("try_with_eval_perfhint.baseline"),
        compile_flags: vec!["-oopjit-", "-trace:PerfHint", "-off:simplejit"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_backend",
            "exclude_dynapogo",
            "exclude_nonative",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn try_with_eval_perfhint_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try_with_eval_perfhint.js",
        baseline_path: Some("try_with_eval_perfhint_l2.baseline"),
        compile_flags: vec![
            "-oopjit-",
            "-trace:PerfHint",
            "-off:simplejit",
            "-perfhintlevel:2",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_backend",
            "exclude_dynapogo",
            "exclude_nonative",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(target_arch = "aarch64"), feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn arguments1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments1.js",
        baseline_path: Some("arguments1.baseline"),
        compile_flags: vec!["-oopjit-", "-trace:PerfHint", "-off:simplejit"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_backend",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(target_arch = "aarch64"), feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn polymorphictest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "polymorphictest.js",
        baseline_path: Some("polymorphictest.baseline"),
        compile_flags: vec!["-oopjit-", "-trace:PerfHint", "-off:simplejit"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_backend",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
