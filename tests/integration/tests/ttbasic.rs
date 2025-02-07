use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 6] = [
    "sequential",
    "exclude_dynapogo",
    "exclude_snap",
    "exclude_serialized",
    "require_debugger",
    "exclude_sanitize_address",
];
const DIRECTORY: &str = "chakracore-cxx/test/TTBasic";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn accessor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "accessor.js",
        baseline_path: Some("accessorRecord.baseline"),
        compile_flags: vec!["-TTRecord=accessorTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("accessorReplay.baseline"),
        compile_flags: vec!["-TTReplay=accessorTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.js",
        baseline_path: Some("argumentsRecord.baseline"),
        compile_flags: vec!["-TTRecord=argumentsTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("argumentsReplay.baseline"),
        compile_flags: vec!["-TTReplay=argumentsTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array.js",
        baseline_path: Some("arrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=arrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("arrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=arrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bind_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bind.js",
        baseline_path: Some("bindRecord.baseline"),
        compile_flags: vec!["-TTRecord=bindTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("bindReplay.baseline"),
        compile_flags: vec!["-TTReplay=bindTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn boolean_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boolean.js",
        baseline_path: Some("booleanRecord.baseline"),
        compile_flags: vec!["-TTRecord=booleanTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("booleanReplay.baseline"),
        compile_flags: vec!["-TTReplay=booleanTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bound_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boundFunction.js",
        baseline_path: Some("boundFunctionRecord.baseline"),
        compile_flags: vec!["-TTRecord=boundFunctionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("boundFunctionReplay.baseline"),
        compile_flags: vec!["-TTReplay=boundFunctionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn boxed_object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boxedObject.js",
        baseline_path: Some("boxedObjectRecord.baseline"),
        compile_flags: vec!["-TTRecord=boxedObjectTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("boxedObjectReplay.baseline"),
        compile_flags: vec!["-TTReplay=boxedObjectTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn cross_site_main_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "crossSiteMain.js",
        baseline_path: Some("crossSiteRecord.baseline"),
        compile_flags: vec!["-TTRecord=crossSiteTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js8(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("crossSiteReplay1.baseline"),
        compile_flags: vec!["-TTReplay=crossSiteTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js9(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("crossSiteReplay2.baseline"),
        compile_flags: vec!["-TTReplay=crossSiteTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn constructor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor.js",
        baseline_path: Some("constructorRecord.baseline"),
        compile_flags: vec!["-TTRecord=constructorTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js10(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("constructorReplay.baseline"),
        compile_flags: vec!["-TTReplay=constructorTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn date_basic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dateBasic.js",
        baseline_path: Some("dateBasicRecord.baseline"),
        compile_flags: vec!["-TTRecord=dateBasicTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js11(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("dateBasicReplay.baseline"),
        compile_flags: vec!["-TTReplay=dateBasicTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js12(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("dateBasicReplay.baseline"),
        compile_flags: vec!["-TTReplay=dateBasicTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn delete_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deleteArray.js",
        baseline_path: Some("deleteArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=deleteArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js13(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("deleteArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=deleteArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn es5_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5Array.js",
        baseline_path: Some("es5ArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=es5ArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js14(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("es5ArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=es5ArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn es5_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5Arguments.js",
        baseline_path: Some("es5ArgumentsRecord.baseline"),
        compile_flags: vec!["-TTRecord=es5ArgumentsTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js15(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("es5ArgumentsReplay.baseline"),
        compile_flags: vec!["-TTReplay=es5ArgumentsTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval.js",
        baseline_path: Some("evalRecord.baseline"),
        compile_flags: vec!["-TTRecord=evalTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js16(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("evalReplay.baseline"),
        compile_flags: vec!["-TTReplay=evalTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn extensible_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "extensible.js",
        baseline_path: Some("extensibleRecord.baseline"),
        compile_flags: vec!["-TTRecord=extensibleTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js17(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("extensibleReplay.baseline"),
        compile_flags: vec!["-TTReplay=extensibleTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_shadowing_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInShadowing.js",
        baseline_path: Some("forInShadowingRecord.baseline"),
        compile_flags: vec!["-TTRecord=forInShadowingTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js18(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forInShadowingReplay.baseline"),
        compile_flags: vec!["-TTReplay=forInShadowingTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn freeze_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "freeze.js",
        baseline_path: Some("freezeRecord.baseline"),
        compile_flags: vec!["-TTRecord=freezeTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js19(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("freezeReplay.baseline"),
        compile_flags: vec!["-TTReplay=freezeTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "function.js",
        baseline_path: Some("functionRecord.baseline"),
        compile_flags: vec!["-TTRecord=functionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js20(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("functionReplay.baseline"),
        compile_flags: vec!["-TTReplay=functionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn large_aux_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "largeAuxArray.js",
        baseline_path: Some("largeAuxArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=largeAuxArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js21(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("largeAuxArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=largeAuxArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn load_re_entrant_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loadReEntrant.js",
        baseline_path: Some("loadReEntrantRecord.baseline"),
        compile_flags: vec!["-TTRecord=loadReEntrantTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js22(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("loadReEntrantReplay.baseline"),
        compile_flags: vec!["-TTReplay=loadReEntrantTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js23(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("loadReEntrantReplay.baseline"),
        compile_flags: vec!["-TTReplay=loadReEntrantTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn map_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "map.js",
        baseline_path: Some("mapRecord.baseline"),
        compile_flags: vec!["-TTRecord=mapTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js24(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("mapReplay.baseline"),
        compile_flags: vec!["-TTReplay=mapTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "missingArray.js",
        baseline_path: Some("missingArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=missingArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js25(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("missingArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=missingArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn native_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nativeArray.js",
        baseline_path: Some("nativeArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=nativeArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js26(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("nativeArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=nativeArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn new_from_args_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newFromArgs.js",
        baseline_path: Some("newFromArgsRecord.baseline"),
        compile_flags: vec!["-TTRecord=newFromArgsTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js27(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("newFromArgsReplay.baseline"),
        compile_flags: vec!["-TTReplay=newFromArgsTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn new_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newFunction.js",
        baseline_path: Some("newFunctionRecord.baseline"),
        compile_flags: vec!["-TTRecord=newFunctionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js28(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("newFunctionReplay.baseline"),
        compile_flags: vec!["-TTReplay=newFunctionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn number_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "number.js",
        baseline_path: Some("numberRecord.baseline"),
        compile_flags: vec!["-TTRecord=numberTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js29(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("numberReplay.baseline"),
        compile_flags: vec!["-TTReplay=numberTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn numeric_property_is_enumerable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "numericPropertyIsEnumerable.js",
        baseline_path: Some("numericPropertyIsEnumerableRecord.baseline"),
        compile_flags: vec![
            "-TTRecord=numericPropertyIsEnumerableTest",
            "-TTSnapInterval=0",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js30(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("numericPropertyIsEnumerableReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=numericPropertyIsEnumerableTest",
            "-TTDStartEvent=2",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "object.js",
        baseline_path: Some("objectRecord.baseline"),
        compile_flags: vec!["-TTRecord=objectTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js31(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("objectReplay.baseline"),
        compile_flags: vec!["-TTReplay=objectTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn pop_array_implicit_call_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "popArrayImplicitCall.js",
        baseline_path: Some("popArrayImplicitCallRecord.baseline"),
        compile_flags: vec!["-TTRecord=popArrayImplicitCallTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js32(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("popArrayImplicitCallReplay.baseline"),
        compile_flags: vec!["-TTReplay=popArrayImplicitCallTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn promise_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "promise.js",
        baseline_path: Some("promiseRecord.baseline"),
        compile_flags: vec!["-TTRecord=promiseTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js33(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promiseReplay.baseline"),
        compile_flags: vec!["-TTReplay=promiseTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js34(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promiseReplay.baseline"),
        compile_flags: vec!["-TTReplay=promiseTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js35(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promiseReplay.baseline"),
        compile_flags: vec!["-TTReplay=promiseTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js36(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promiseReplay.baseline"),
        compile_flags: vec!["-TTReplay=promiseTest", "-TTDStartEvent=6"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js37(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promiseReplay.baseline"),
        compile_flags: vec!["-TTReplay=promiseTest", "-TTDStartEvent=7"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn promise_multiple_then_calls_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "promise_MultipleThenCalls.js",
        baseline_path: Some("promise_MultipleThenCallsRecord.baseline"),
        compile_flags: vec![
            "-TTRecord=promise_MultipleThenCallsTest",
            "-TTSnapInterval=0",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js38(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=1",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js39(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=2",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js40(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=3",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js41(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=4",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js42(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=5",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js43(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=6",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js44(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("promise_MultipleThenCallsReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=promise_MultipleThenCallsTest",
            "-TTDStartEvent=7",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn proxy_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxy.js",
        baseline_path: Some("proxyRecord.baseline"),
        compile_flags: vec!["-TTRecord=proxyTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js45(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("proxyReplay.baseline"),
        compile_flags: vec!["-TTReplay=proxyTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn regex_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex.js",
        baseline_path: Some("regexRecord.baseline"),
        compile_flags: vec!["-TTRecord=regexTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js46(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("regexReplay.baseline"),
        compile_flags: vec!["-TTReplay=regexTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn seal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal.js",
        baseline_path: Some("sealRecord.baseline"),
        compile_flags: vec!["-TTRecord=sealTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js47(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("sealReplay.baseline"),
        compile_flags: vec!["-TTReplay=sealTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn scoped_accessors_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scopedAccessors.js",
        baseline_path: Some("scopedAccessorsRecord.baseline"),
        compile_flags: vec!["-TTRecord=scopedAccessorsTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js48(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("scopedAccessorsReplay.baseline"),
        compile_flags: vec!["-TTReplay=scopedAccessorsTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn scope_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scopeFunction.js",
        baseline_path: Some("scopeFunctionRecord.baseline"),
        compile_flags: vec!["-TTRecord=scopeFunctionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js49(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("scopeFunctionReplay.baseline"),
        compile_flags: vec!["-TTReplay=scopeFunctionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn set_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "set.js",
        baseline_path: Some("setRecord.baseline"),
        compile_flags: vec!["-TTRecord=setTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js50(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("setReplay.baseline"),
        compile_flags: vec!["-TTReplay=setTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn shadow_prototype_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadowPrototype.js",
        baseline_path: Some("shadowPrototypeRecord.baseline"),
        compile_flags: vec!["-TTRecord=shadowPrototypeTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js51(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("shadowPrototypeReplay.baseline"),
        compile_flags: vec!["-TTReplay=shadowPrototypeTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn sparse_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sparseArray.js",
        baseline_path: Some("sparseArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=sparseArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js52(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("sparseArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=sparseArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn string_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "string.js",
        baseline_path: Some("stringRecord.baseline"),
        compile_flags: vec!["-TTRecord=stringTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js53(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("stringReplay.baseline"),
        compile_flags: vec!["-TTReplay=stringTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn symbol_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symbol.js",
        baseline_path: Some("symbolRecord.baseline"),
        compile_flags: vec!["-TTRecord=symbolTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js54(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("symbolReplay.baseline"),
        compile_flags: vec!["-TTReplay=symbolTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js55(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("symbolReplay.baseline"),
        compile_flags: vec!["-TTReplay=symbolTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn type_conversions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typeConversions.js",
        baseline_path: Some("typeConversionsRecord.baseline"),
        compile_flags: vec!["-TTRecord=typeConversionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js56(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("typeConversionsReplay.baseline"),
        compile_flags: vec!["-TTReplay=typeConversionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn typed_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typedArray.js",
        baseline_path: Some("typedArrayRecord.baseline"),
        compile_flags: vec!["-TTRecord=typedArrayTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js57(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("typedArrayReplay.baseline"),
        compile_flags: vec!["-TTReplay=typedArrayTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn type_promotion_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typePromotion.js",
        baseline_path: Some("typePromotionRecord.baseline"),
        compile_flags: vec!["-TTRecord=typePromotionTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js58(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("typePromotionReplay.baseline"),
        compile_flags: vec!["-TTReplay=typePromotionTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_basic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorBasic.js",
        baseline_path: Some("generatorBasicRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorBasicTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js59(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorBasicReplay_1.baseline"),
        compile_flags: vec!["-TTReplay=generatorBasicTest", "-TTDStartEvent=1"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js60(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorBasicReplay_2.baseline"),
        compile_flags: vec!["-TTReplay=generatorBasicTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js61(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorBasicReplay_3.baseline"),
        compile_flags: vec!["-TTReplay=generatorBasicTest", "-TTDStartEvent=3"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js62(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorBasicReplay_4.baseline"),
        compile_flags: vec!["-TTReplay=generatorBasicTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js63(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorBasicReplay_5.baseline"),
        compile_flags: vec!["-TTReplay=generatorBasicTest", "-TTDStartEvent=5"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_return_yield_result_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorReturnYieldResult.js",
        baseline_path: Some("generatorReturnYieldResultRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorReturnYieldResult", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js64(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorReturnYieldResultReplay_1.baseline"),
        compile_flags: vec!["-TTReplay=generatorReturnYieldResult", "-TTDStartEvent=1"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js65(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorReturnYieldResultReplay_2.baseline"),
        compile_flags: vec!["-TTReplay=generatorReturnYieldResult", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js66(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorReturnYieldResultReplay_3.baseline"),
        compile_flags: vec!["-TTReplay=generatorReturnYieldResult", "-TTDStartEvent=3"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js67(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorReturnYieldResultReplay_4.baseline"),
        compile_flags: vec!["-TTReplay=generatorReturnYieldResult", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_int_param_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorIntParam.js",
        baseline_path: Some("generatorIntParamRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorIntParamTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js68(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorIntParamReplay.baseline"),
        compile_flags: vec!["-TTReplay=generatorIntParamTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_object_param_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorObjectParam.js",
        baseline_path: Some("generatorObjectParamRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorObjectParamTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js69(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorObjectParamReplay.baseline"),
        compile_flags: vec!["-TTReplay=generatorObjectParamTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_class_method_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorClassMethod.js",
        baseline_path: Some("generatorClassMethodRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorClassMethodTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js70(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorClassMethodReplay.baseline"),
        compile_flags: vec!["-TTReplay=generatorClassMethodTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_nested_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorNested.js",
        baseline_path: Some("generatorNestedRecord.baseline"),
        compile_flags: vec!["-TTRecord=generatorNestedTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js71(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorNestedReplay.baseline"),
        compile_flags: vec!["-TTReplay=generatorNestedTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_restore_completed_generator_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorRestoreCompletedGenerator.js",
        baseline_path: Some("generatorRestoreCompletedGeneratorRecord.baseline"),
        compile_flags: vec![
            "-TTRecord=generatorRestoreCompletedGeneratorTest",
            "-TTSnapInterval=0",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js72(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorRestoreCompletedGeneratorReplay.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorRestoreCompletedGeneratorTest",
            "-TTDStartEvent=6",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn generator_write_log_during_generator_execution_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generatorWriteLogDuringGeneratorExecution.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionRecord.baseline"),
        compile_flags: vec![
            "-TTRecord=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTSnapInterval=0",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js73(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_1.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=1",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js74(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_2.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=2",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js75(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_3.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=3",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js76(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_4.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=4",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js77(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_5.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=5",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js78(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("generatorWriteLogDuringGeneratorExecutionReplay_6.baseline"),
        compile_flags: vec![
            "-TTReplay=generatorWriteLogDuringGeneratorExecutionTest",
            "-TTDStartEvent=6",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn async_await_basic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncAwaitBasic.js",
        baseline_path: Some("asyncAwaitBasicRecord.baseline"),
        compile_flags: vec!["-TTRecord=asyncAwaitBasicTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js79(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwaitBasicReplay_1.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwaitBasicTest", "-TTDStartEvent=1"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js80(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwaitBasicReplay_2.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwaitBasicTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js81(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwaitBasicReplay_3.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwaitBasicTest", "-TTDStartEvent=3"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js82(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwaitBasicReplay_4.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwaitBasicTest", "-TTDStartEvent=4"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js83(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwaitBasicReplay_5.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwaitBasicTest", "-TTDStartEvent=5"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn async_await2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncAwait2.js",
        baseline_path: Some("asyncAwait2Record.baseline"),
        compile_flags: vec!["-TTRecord=asyncAwait2Test", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js84(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwait2Replay_1.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwait2Test", "-TTDStartEvent=1"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js85(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwait2Replay_2.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwait2Test", "-TTDStartEvent=5"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn async_await3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncAwait3.js",
        baseline_path: Some("asyncAwait3Record.baseline"),
        compile_flags: vec!["-TTRecord=asyncAwait3Test", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js86(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwait3Replay_1.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwait3Test", "-TTDStartEvent=1"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn ttd_sentinal_js87(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("asyncAwait3Replay_2.baseline"),
        compile_flags: vec!["-TTReplay=asyncAwait3Test", "-TTDStartEvent=5"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
