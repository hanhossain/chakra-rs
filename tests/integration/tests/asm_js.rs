use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_asmjs</tags>

const DIRECTORY: &str = "chakracore-cxx/test/AsmJs";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn argTest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argTest.js",
        compile_flags: vec!["-maic:0", "-maxInterpretCount:0"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayView.js",
        baseline_path: Some("ArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicBranching_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranching.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicBranching_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranchingLinkFail.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-forceAsmJsLinkFail"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonDouble_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonDouble.js",
        baseline_path: Some("basicComparisonDouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonInt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonInt.js",
        baseline_path: Some("basicComparisonInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonUInt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonUInt.js",
        baseline_path: Some("basicComparisonUInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicLooping_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicLooping.js",
        baseline_path: Some("BasicLooping.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMath_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMath.js",
        baseline_path: Some("basicMath.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMathIntSpecific_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathIntSpecific.js",
        baseline_path: Some("basicMathIntSpecific.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMathUnary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathUnary.js",
        baseline_path: Some("basicMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicSwitch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicSwitch.js",
        baseline_path: Some("BasicSwitch.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn CompositionMathUnary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CompositionMathUnary.js",
        baseline_path: Some("CompositionMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn FunctionCalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn FunctionCalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-ForceStaticInterpreterThunk", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn functiontablecalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablecalls.js",
        baseline_path: Some("functiontablecalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn MathBuiltinsCall_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn MathBuiltinsCall_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-maic:1 -sse:3")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ModuleVarRead_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarRead.js",
        baseline_path: Some("ModuleVarRead.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ModuleVarWrite_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarWrite.js",
        baseline_path: Some("ModuleVarWrite.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ReadArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadArrayView.js",
        baseline_path: Some("ReadArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ReadFixOffset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadFixOffset.js",
        baseline_path: Some("ReadFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn relink_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn relink_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn relink_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn relink_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relink.js",
        baseline_path: Some("relink.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn WriteArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteArrayView.js",
        baseline_path: Some("WriteArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn WriteFixOffset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteFixOffset.js",
        baseline_path: Some("WriteFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ArrayView.js</files>
//       <baseline>ArrayView.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>BasicBranching.js</files>
//       <baseline>BasicBranching.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicComparisonDouble.js</files>
//       <baseline>basicComparisonDouble.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicComparisonInt.js</files>
//       <baseline>basicComparisonInt.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicComparisonUInt.js</files>
//       <baseline>basicComparisonUInt.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>BasicLooping.js</files>
//       <baseline>BasicLooping.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicMath.js</files>
//       <baseline>basicMath.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicMathIntSpecific.js</files>
//       <baseline>basicMathIntSpecific.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>basicMathUnary.js</files>
//       <baseline>basicMathUnary.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>BasicSwitch.js</files>
//       <baseline>BasicSwitch.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>CompositionMathUnary.js</files>
//       <baseline>CompositionMathUnary.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>FunctionCalls.js</files>
//       <baseline>FunctionCalls.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>functiontablecalls.js</files>
//       <baseline>functiontablecalls.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>MathBuiltinsCall.js</files>
//       <baseline>MathBuiltinsCall.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ModuleVarRead.js</files>
//       <baseline>ModuleVarRead.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ModuleVarWrite.js</files>
//       <baseline>ModuleVarWrite.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ReadArrayView.js</files>
//       <baseline>ReadArrayView.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ReadFixOffset.js</files>
//       <baseline>ReadFixOffset.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>WriteArrayView.js</files>
//       <baseline>WriteArrayView.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>WriteFixOffset.js</files>
//       <baseline>WriteFixOffset.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayView.js",
        baseline_path: Some("ArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicBranching_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicBranching.js",
        baseline_path: Some("BasicBranching.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonDouble_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonDouble.js",
        baseline_path: Some("basicComparisonDouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonInt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonInt.js",
        baseline_path: Some("basicComparisonInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicComparisonUInt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicComparisonUInt.js",
        baseline_path: Some("basicComparisonUInt.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicLooping_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicLooping.js",
        baseline_path: Some("BasicLooping.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMath_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMath.js",
        baseline_path: Some("basicMath.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMathIntSpecific_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathIntSpecific.js",
        baseline_path: Some("basicMathIntSpecific.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basicMathUnary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basicMathUnary.js",
        baseline_path: Some("basicMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn BasicSwitch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BasicSwitch.js",
        baseline_path: Some("BasicSwitch.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn CompositionMathUnary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CompositionMathUnary.js",
        baseline_path: Some("CompositionMathUnary.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn FunctionCalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionCalls.js",
        baseline_path: Some("FunctionCalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn functiontablecalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablecalls.js",
        baseline_path: Some("functiontablecalls.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn MathBuiltinsCall_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MathBuiltinsCall.js",
        baseline_path: Some("MathBuiltinsCall.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ModuleVarRead_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarRead.js",
        baseline_path: Some("ModuleVarRead.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ModuleVarWrite_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ModuleVarWrite.js",
        baseline_path: Some("ModuleVarWrite.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ReadArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadArrayView.js",
        baseline_path: Some("ReadArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ReadFixOffset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadFixOffset.js",
        baseline_path: Some("ReadFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn WriteArrayView_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteArrayView.js",
        baseline_path: Some("WriteArrayView.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn WriteFixOffset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WriteFixOffset.js",
        baseline_path: Some("WriteFixOffset.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn functiontablebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functiontablebug.js",
        baseline_path: Some("functiontablebug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn nanbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nanbug.js",
        baseline_path: Some("nanbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn nanbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nanbug.js",
        baseline_path: Some("nanbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn switchbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchbug.js",
        baseline_path: Some("switchbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fgpeepsbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fgpeepsbug.js",
        baseline_path: Some("fgpeepsbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn cseBug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cseBug.js",
        baseline_path: Some("cseBug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-off:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn evalbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalbug.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn symBug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symBug.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn brbool_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "brbool.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constTest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constTest.js",
        baseline_path: Some("constTest.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constTest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constTest.js",
        baseline_path: Some("constTest.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>constTest.js</files>
//       <baseline>constTest.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ffibug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ffibug.js",
        baseline_path: Some("ffibug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ternaryfloat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ternaryfloat.js",
        baseline_path: Some("ternaryfloat.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn minintbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "minintbug.js",
        baseline_path: Some("minintbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn floatmod_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "floatmod.js",
        baseline_path: Some("floatmod.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn floatmod_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "floatmod.js",
        baseline_path: Some("floatmod.baseline"),
        compile_flags: vec!["-forceserialized", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalidIntLiteral_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalidIntLiteral.js",
        baseline_path: Some("invalidIntLiteral.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fstpbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fstpbug.js",
        baseline_path: Some("fstpbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn break2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break2.js",
        baseline_path: Some("break2.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn break3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break3.js",
        baseline_path: Some("break3.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn return1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return1.js",
        baseline_path: Some("return1.baseline"),
        compile_flags: vec!["-off:deferparse", todo!("-testtrace:asmjs -bgjit- -lic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn return2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return2.js",
        baseline_path: Some("return2.baseline"),
        compile_flags: vec!["-off:deferparse", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn return3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "return3.js",
        baseline_path: Some("return3.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn returndouble_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returndouble.js",
        baseline_path: Some("returndouble.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn break1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "break1.js",
        baseline_path: Some("break1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn JitToLoopBody_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JitToLoopBody.js",
        baseline_path: Some("JitToLoopBody.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1 -maic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn LoopBodyToJit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LoopBodyToJit.js",
        baseline_path: Some("LoopBodyToJit.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1 -maic:1")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn breakfloat1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "breakfloat1.js",
        baseline_path: Some("breakfloat1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1 ")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn returnFloat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnFloat.js",
        baseline_path: Some("returnFloat.baseline"),
        compile_flags: vec!["-testtrace:asmjs", todo!("-bgjit- -lic:1 ")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unitybug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unitybug.js",
        baseline_path: Some("unitybug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unitybug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unitybug.js",
        baseline_path: Some("unitybug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>unitybug.js</files>
//       <baseline>unitybug.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn argoutcapturebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argoutcapturebug.js",
        baseline_path: Some("argoutcapturebug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ReadAV1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadAV1.js",
        baseline_path: Some("ReadAV1.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-off:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn clz32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn clz32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-nonative"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>clz32.js</files>
//       <baseline>clz32.baseline</baseline>
//       <tags>exclude_x64</tags>
//       <!-- After removing -simdjs this test fail with -on:asmjsjittemplate. Investigate and enable
//       <compile-flags>-testtrace:asmjs -oopjit- -on:asmjsjittemplate -off:fulljit</compile-flags>
//       -->
//       <compile-flags>-testtrace:asmjs -oopjit- -off:fulljit</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn negZero_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negZero.js",
        baseline_path: Some("negZero.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn shadowingBug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadowingBug.js",
        baseline_path: Some("shadowingBug.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockLabelBug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockLabelBug.js",
        baseline_path: Some("blockLabelBug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn switchJumpTable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchJumpTable.js",
        baseline_path: Some("switchJumpTable.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn switchBinaryTraverse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchBinaryTraverse.js",
        baseline_path: Some("switchBinaryTraverse.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn lowererdivbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lowererdivbug.js",
        baseline_path: Some("lowererdivbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn qmarkbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "qmarkbug.js",
        baseline_path: Some("qmarkbug.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn uint_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uint.js",
        baseline_path: Some("uint.baseline"),
        compile_flags: vec!["-maic:1", todo!("-off:deferparse -testtrace:asmjs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn asmjscctx_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asmjscctx.js",
        baseline_path: Some("asmjscctx.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constloads_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constloads.js",
        baseline_path: Some("constloads.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn vardeclnorhs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "vardeclnorhs.js",
        baseline_path: Some("vardeclnorhs.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug12239366_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug12239366.js",
        compile_flags: vec!["-lic:1", "-bgjit-"],
        tags: HashSet::from(["exclude_drt"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug16253406_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug16253406.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn badFunctionType_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_sanitize_address"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bugGH2270_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugGH2270.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>lotsOfLocals.js</files>
//       <tags>exclude_debug,exclude_razzle</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug9883547_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug9883547.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constFoldTests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constFoldTests.js",
        compile_flags: vec!["-asmjs", "-maic:0"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>lotsOfLocals.js</files>
//       <tags>exclude_debug</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>params.js</files>
//       <baseline>params.baseline</baseline>
//       <compile-flags>-testtrace:asmjs -args 14000 -endargs -EnableFatalErrorOnOOM-</compile-flags>
//       <!-- todo:: On unix platforms there is more stack available,
//            so we need to find the right limit to test in order to not timeout -->
//       <tags>exclude_dynapogo,exclude_mac,slow</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn nested_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nested.js",
        baseline_path: Some("nested.baseline"),
        compile_flags: vec!["-forcedeferparse", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn notBinaryPropertyDefinition_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "notBinaryPropertyDefinition.js",
        baseline_path: Some("notBinaryPropertyDefinition.baseline"),
        compile_flags: vec!["-ES2018ObjectRestSpread", "-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constbrbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constbrbug.js",
        baseline_path: Some("constbrbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:0"],
        tags: HashSet::from(["exclude_drt"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn useasmbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "useasmbug.js",
        baseline_path: Some("useasmbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn lambda_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lambda.js",
        baseline_path: Some("lambda.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_drt"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn badFunctionType_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_drt", "exclude_sanitize_address"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn badFunctionType_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badFunctionType.js",
        baseline_path: Some("badFunctionType.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_drt", "exclude_sanitize_address"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn exports_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exports.js",
        baseline_path: Some("exports.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        tags: HashSet::from(["exclude_drt"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn trackdeferredonreparse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trackdeferredonreparse.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn regress_hascalls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regress_hascalls.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn argassignbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argassignbug.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn manyargs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "manyargs.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn maybecallbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "maybecallbug.js",
        baseline_path: Some("maybecallbug.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn divByConstants_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants.js",
        baseline_path: Some("divByConstants.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn divByConstants_Unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants_Unsigned.js",
        baseline_path: Some("divByConstants.baseline"),
        compile_flags: vec!["-testtrace:asmjs", "-maic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug16252562_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug16252562.js",
        baseline_path: Some("bug16252562.baseline"),
        compile_flags: vec!["-testtrace:asmjs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn emit_recursive_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emit_recursive.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
