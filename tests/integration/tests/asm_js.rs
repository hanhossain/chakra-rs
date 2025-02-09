use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 3] = ["exclude_serialized", "require_backend", "require_asmjs"];
const DIRECTORY: &str = "chakracore-cxx/test/AsmJs";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arg_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argTest.js",
        compile_flags: vec!["-maic:0", "-maxInterpretCount:0"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn array_view_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayView.js",
        baseline_path: Some("ArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_branching_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranching.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_branching_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranchingLinkFail.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-forceAsmJsLinkFail"],
        tags: HashSet::from([
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_comparison_double_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonDouble.js",
        baseline_path: Some("basicComparisonDouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_comparison_int_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonInt.js",
        baseline_path: Some("basicComparisonInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_comparison_uint_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonUInt.js",
        baseline_path: Some("basicComparisonUInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_looping_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicLooping.js",
        baseline_path: Some("BasicLooping.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_math_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMath.js",
        baseline_path: Some("basicMath.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_math_int_specific_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathIntSpecific.js",
        baseline_path: Some("basicMathIntSpecific.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_math_unary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathUnary.js",
        baseline_path: Some("basicMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_switch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicSwitch.js",
        baseline_path: Some("BasicSwitch.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn composition_math_unary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CompositionMathUnary.js",
        baseline_path: Some("CompositionMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn function_calls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn function_calls_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-ForceStaticInterpreterThunk", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn functiontablecalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablecalls.js",
        baseline_path: Some("functiontablecalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn math_builtins_call_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn math_builtins_call_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1", "-sse:3"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn module_var_read_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarRead.js",
        baseline_path: Some("ModuleVarRead.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn module_var_write_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarWrite.js",
        baseline_path: Some("ModuleVarWrite.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn read_array_view_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadArrayView.js",
        baseline_path: Some("ReadArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn read_fix_offset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadFixOffset.js",
        baseline_path: Some("ReadFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn relink_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn relink_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn relink_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn relink_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn write_array_view_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteArrayView.js",
        baseline_path: Some("WriteArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn write_fix_offset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteFixOffset.js",
        baseline_path: Some("WriteFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn array_view_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayView.js",
        baseline_path: Some("ArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_branching_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranching.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_comparison_double_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonDouble.js",
        baseline_path: Some("basicComparisonDouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_comparison_int_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonInt.js",
        baseline_path: Some("basicComparisonInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_comparison_uint_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonUInt.js",
        baseline_path: Some("basicComparisonUInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_looping_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicLooping.js",
        baseline_path: Some("BasicLooping.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_math_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMath.js",
        baseline_path: Some("basicMath.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_math_int_specific_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathIntSpecific.js",
        baseline_path: Some("basicMathIntSpecific.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_math_unary_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathUnary.js",
        baseline_path: Some("basicMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_switch_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicSwitch.js",
        baseline_path: Some("BasicSwitch.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn composition_math_unary_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CompositionMathUnary.js",
        baseline_path: Some("CompositionMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn function_calls_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn functiontablecalls_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablecalls.js",
        baseline_path: Some("functiontablecalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn math_builtins_call_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_var_read_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarRead.js",
        baseline_path: Some("ModuleVarRead.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_var_write_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarWrite.js",
        baseline_path: Some("ModuleVarWrite.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn read_array_view_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadArrayView.js",
        baseline_path: Some("ReadArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn read_fix_offset_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadFixOffset.js",
        baseline_path: Some("ReadFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn write_array_view_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteArrayView.js",
        baseline_path: Some("WriteArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn write_fix_offset_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteFixOffset.js",
        baseline_path: Some("WriteFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn array_view_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayView.js",
        baseline_path: Some("ArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_branching_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranching.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_comparison_double_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonDouble.js",
        baseline_path: Some("basicComparisonDouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_comparison_int_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonInt.js",
        baseline_path: Some("basicComparisonInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_comparison_uint_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonUInt.js",
        baseline_path: Some("basicComparisonUInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_looping_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicLooping.js",
        baseline_path: Some("BasicLooping.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_math_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMath.js",
        baseline_path: Some("basicMath.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn basic_math_int_specific_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathIntSpecific.js",
        baseline_path: Some("basicMathIntSpecific.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_math_unary_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathUnary.js",
        baseline_path: Some("basicMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn basic_switch_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicSwitch.js",
        baseline_path: Some("BasicSwitch.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn composition_math_unary_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CompositionMathUnary.js",
        baseline_path: Some("CompositionMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn function_calls_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn functiontablecalls_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablecalls.js",
        baseline_path: Some("functiontablecalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn math_builtins_call_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn module_var_read_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarRead.js",
        baseline_path: Some("ModuleVarRead.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn module_var_write_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarWrite.js",
        baseline_path: Some("ModuleVarWrite.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn read_array_view_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadArrayView.js",
        baseline_path: Some("ReadArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn read_fix_offset_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadFixOffset.js",
        baseline_path: Some("ReadFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn write_array_view_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteArrayView.js",
        baseline_path: Some("WriteArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn write_fix_offset_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteFixOffset.js",
        baseline_path: Some("WriteFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from([
            "Slow",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn functiontablebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablebug.js",
        baseline_path: Some("functiontablebug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nanbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nanbug.js",
        baseline_path: Some("nanbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nanbug_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nanbug.js",
        baseline_path: Some("nanbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn switchbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchbug.js",
        baseline_path: Some("switchbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fgpeepsbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fgpeepsbug.js",
        baseline_path: Some("fgpeepsbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cse_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cseBug.js",
        baseline_path: Some("cseBug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-off:deferparse"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn evalbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalbug.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn sym_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symBug.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn brbool_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "brbool.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn const_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constTest.js",
        baseline_path: Some("constTest.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn const_test_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constTest.js",
        baseline_path: Some("constTest.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn const_test_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constTest.js",
        baseline_path: Some("constTest.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn ffibug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ffibug.js",
        baseline_path: Some("ffibug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn ternaryfloat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ternaryfloat.js",
        baseline_path: Some("ternaryfloat.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn minintbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "minintbug.js",
        baseline_path: Some("minintbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn floatmod_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "floatmod.js",
        baseline_path: Some("floatmod.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn floatmod_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "floatmod.js",
        baseline_path: Some("floatmod.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn invalid_int_literal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalidIntLiteral.js",
        baseline_path: Some("invalidIntLiteral.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-force:deferparse"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fstpbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fstpbug.js",
        baseline_path: Some("fstpbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn break2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break2.js",
        baseline_path: Some("break2.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn break3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break3.js",
        baseline_path: Some("break3.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn return1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return1.js",
        baseline_path: Some("return1.baseline"),
        compile_flags: vec!["-off:deferparse", "-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn return2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return2.js",
        baseline_path: Some("return2.baseline"),
        compile_flags: vec!["-off:deferparse", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn return3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return3.js",
        baseline_path: Some("return3.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn returndouble_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returndouble.js",
        baseline_path: Some("returndouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn break1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break1.js",
        baseline_path: Some("break1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jit_to_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JitToLoopBody.js",
        baseline_path: Some("JitToLoopBody.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn loop_body_to_jit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LoopBodyToJit.js",
        baseline_path: Some("LoopBodyToJit.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn breakfloat1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "breakfloat1.js",
        baseline_path: Some("breakfloat1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn return_float_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnFloat.js",
        baseline_path: Some("returnFloat.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn unitybug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unitybug.js",
        baseline_path: Some("unitybug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn unitybug_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unitybug.js",
        baseline_path: Some("unitybug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unitybug_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unitybug.js",
        baseline_path: Some("unitybug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argoutcapturebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argoutcapturebug.js",
        baseline_path: Some("argoutcapturebug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn read_av1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadAV1.js",
        baseline_path: Some("ReadAV1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-off:deferparse"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn clz32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn clz32_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
// <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn clz32_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-oopjit-", "-off:fulljit"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn neg_zero_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negZero.js",
        baseline_path: Some("negZero.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn shadowing_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadowingBug.js",
        baseline_path: Some("shadowingBug.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn block_label_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockLabelBug.js",
        baseline_path: Some("blockLabelBug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn switch_jump_table_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchJumpTable.js",
        baseline_path: Some("switchJumpTable.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn switch_binary_traverse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchBinaryTraverse.js",
        baseline_path: Some("switchBinaryTraverse.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lowererdivbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lowererdivbug.js",
        baseline_path: Some("lowererdivbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn qmarkbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "qmarkbug.js",
        baseline_path: Some("qmarkbug.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn uint_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uint.js",
        baseline_path: Some("uint.baseline"),
        compile_flags: vec!["-maic:1", "-off:deferparse", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn asmjscctx_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asmjscctx.js",
        baseline_path: Some("asmjscctx.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn constloads_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constloads.js",
        baseline_path: Some("constloads.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn vardeclnorhs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "vardeclnorhs.js",
        baseline_path: Some("vardeclnorhs.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug12239366_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug12239366.js",
        compile_flags: vec!["-lic:1", "-bgjit-"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug16253406_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug16253406.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bad_function_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_sanitize_address",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug_gh2270_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugGH2270.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn lots_of_locals_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lotsOfLocals.js",
        tags: HashSet::from(["exclude_debug", "exclude_razzle"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug9883547_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug9883547.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn const_fold_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constFoldTests.js",
        compile_flags: vec!["-asmjs", "-maic:0"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn lots_of_locals_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lotsOfLocals.js",
        tags: HashSet::from(["exclude_debug"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO: On unix platforms there is more stack available, so we need to find the right limit to test in order to not timeout
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn params_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "params.js",
        baseline_path: Some("params.baseline"),
        compile_flags: vec![
            "-testtrace:asmjs",
            "-args",
            " 14000",
            " -endargs",
            "-EnableFatalErrorOnOOM-",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_mac", "slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nested_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nested.js",
        baseline_path: Some("nested.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn not_binary_property_definition_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "notBinaryPropertyDefinition.js",
        baseline_path: Some("notBinaryPropertyDefinition.baseline"),
        compile_flags: vec!["-ES2018ObjectRestSpread", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn constbrbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constbrbug.js",
        baseline_path: Some("constbrbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:0"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn useasmbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "useasmbug.js",
        baseline_path: Some("useasmbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lambda_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lambda.js",
        baseline_path: Some("lambda.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bad_function_type_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_sanitize_address",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bad_function_type_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_sanitize_address",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn exports_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exports.js",
        baseline_path: Some("exports.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_serialized",
            "require_backend",
            "require_asmjs",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn trackdeferredonreparse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trackdeferredonreparse.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn regress_hascalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regress_hascalls.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argassignbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argassignbug.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn manyargs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "manyargs.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn maybecallbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "maybecallbug.js",
        baseline_path: Some("maybecallbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn div_by_constants_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants.js",
        baseline_path: Some("divByConstants.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn div_by_constants_unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants_Unsigned.js",
        baseline_path: Some("divByConstants.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug16252562_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug16252562.js",
        baseline_path: Some("bug16252562.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn emit_recursive_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emit_recursive.js",
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
