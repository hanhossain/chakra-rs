use crate::common;
use crate::common::Variant;
use rstest::rstest;


const DIRECTORY: &str = "chakracore-cxx/test/ControlFlow";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn do_loop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DoLoop.js",
        baseline_path: Some("DoLoop.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn do_loop2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DoLoop2.js",
        baseline_path: Some("DoLoop2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn do_loop3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DoLoop3.js",
        baseline_path: Some("DoLoop3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jump_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jump.js",
        baseline_path: Some("jump.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_loop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ForLoop.js",
        baseline_path: Some("ForLoop.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_loop2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ForLoop2.js",
        baseline_path: Some("ForLoop2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn while_loop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WhileLoop.js",
        baseline_path: Some("WhileLoop.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn while_loop2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "WhileLoop2.js",
        baseline_path: Some("WhileLoop2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_misc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInMisc.js",
        baseline_path: Some("forInMisc.baseline"),
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_object_delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectDelete.js",
        baseline_path: Some("forInObjectDelete.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_object_add_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectAdd.js",
        baseline_path: Some("forInObjectAddv3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_object_add_delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectAddDelete.js",
        baseline_path: Some("forInObjectAddDeletev3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_primitive_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInPrimitive.js",
        baseline_path: Some("forInPrimitive.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn enumeration_adddelete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enumeration_adddelete.js",
        baseline_path: Some("enumeration_adddelete.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_array_add_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInArrayAdd.js",
        baseline_path: Some("forinArrayAddv3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_object_with_prototype_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectWithPrototype.js",
        baseline_path: Some("forInObjectWithPrototype.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn do_while_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DoWhile.js",
        baseline_path: Some("DoWhileES5.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
