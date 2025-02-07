use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 3] = ["exclude_serialized", "require_backend", "require_asmjs"];
const DIRECTORY: &str = "chakracore-cxx/test/AsmJSFloat";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_math_op_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicMathOp.js",
        baseline_path: Some("BasicMathOp.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn float64_loadand_store_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Float64-LoadandStore.js",
        baseline_path: Some("Float64-LoadandStore.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn ld_undef_from_heap_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LdUndefFromHeap.js",
        baseline_path: Some("LdUndefFromHeap.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nested_math_lib_calls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NestedMathLibCalls.js",
        baseline_path: Some("NestedMathLibCalls.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn not_operator_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NotOperator.js",
        baseline_path: Some("NotOperator.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn store_double_to_float32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StoreDoubleToFloat32.js",
        baseline_path: Some("StoreDoubleToFloat32.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn store_float_to_float32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StoreFloatToFloat32.js",
        baseline_path: Some("StoreFloatToFloat32.baseline"),
        compile_flags: vec![
            "-on:asmjs",
            "-testtrace:asmjs",
            "-on:asmjs",
            "-testtrace:asmjsinterpreter",
        ],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_math_op_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicMathOp.js",
        baseline_path: Some("BasicMathOp.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn float64_loadand_store_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Float64-LoadandStore.js",
        baseline_path: Some("Float64-LoadandStore.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn ld_undef_from_heap_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LdUndefFromHeap.js",
        baseline_path: Some("LdUndefFromHeap.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nested_math_lib_calls_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NestedMathLibCalls.js",
        baseline_path: Some("NestedMathLibCalls.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn not_operator_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NotOperator.js",
        baseline_path: Some("NotOperator.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn store_double_to_float32_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StoreDoubleToFloat32.js",
        baseline_path: Some("StoreDoubleToFloat32.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn store_float_to_float32_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StoreFloatToFloat32.js",
        baseline_path: Some("StoreFloatToFloat32.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs", "-mtjrc:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}
