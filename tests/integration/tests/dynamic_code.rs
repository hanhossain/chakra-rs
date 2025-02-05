use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/DynamicCode";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn eval_nativecodedata_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval-nativecodedata.js",
        baseline_path: Some(""),
        compile_flags: vec!["-CollectGarbage"],
        tags: HashSet::from(["exclude_test", "Slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn eval_nativenumber_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval-nativenumber.js",
        baseline_path: Some(""),
        compile_flags: vec!["-CollectGarbage"],
        tags: HashSet::from(["exclude_test", "Slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_nativenumber_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval-nativenumber.js",
        baseline_path: Some(""),
        compile_flags: vec!["-CollectGarbage", "-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant);
}
