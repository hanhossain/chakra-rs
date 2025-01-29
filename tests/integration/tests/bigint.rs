use crate::common;
use crate::common::Variant;
use rstest::rstest;


const DIRECTORY: &str = "chakracore-cxx/test/BigInt";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn comparison_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comparison.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn global_object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "global_object.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn increment_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "increment.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn decrement_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "decrement.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn assign_by_value_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "assign_by_value.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn add_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "add.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn subtract_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "subtract.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn mixed_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mixed.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictly_equal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictly_equal.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bitwise_not_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bitwise_not.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multiply_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiply.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
