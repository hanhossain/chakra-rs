use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/strict";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn global_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "GlobalEval.js",
        baseline_path: Some("GlobalEval.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basics_function_in_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics_function_in_SM.js",
        baseline_path: Some("basics_function_in_SM.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basics_function_in_sm_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics_function_in_SM.js",
        baseline_path: Some("basics_function_in_SM.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn caller_or_args_no_access_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callerOrArgsNoAccess.js",
        baseline_path: Some("callerOrArgsNoAccess.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stricteval_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stricteval-deferred.js",
        baseline_path: Some("stricteval-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stricteval2_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stricteval2-deferred.js",
        baseline_path: Some("stricteval2-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stricteval3_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stricteval3-deferred.js",
        baseline_path: Some("stricteval3-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictargs_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictargs-deferred.js",
        baseline_path: Some("strictargs-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictargs2_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictargs2-deferred.js",
        baseline_path: Some("strictargs2-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictargs3_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictargs3-deferred.js",
        baseline_path: Some("strictargs3-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn evalargs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalargs.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>evalargs.js</files>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//     <baseline />
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_this_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThis.js",
        baseline_path: Some("evalThis.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_this2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThis2.js",
        baseline_path: Some("evalThis2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_this_nested_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThisNested.js",
        baseline_path: Some("evalThisNested.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn formal_samename1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_samename1.js",
        baseline_path: Some("formal_samename1.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn formal_samename1_js_force_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_samename1.js",
        baseline_path: Some("formal_samename1.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn formal_samename2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_samename2.js",
        baseline_path: Some("formal_samename2.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn formal_samename2_js_force_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_samename2.js",
        baseline_path: Some("formal_samename2.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multiunit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiunit.js",
        baseline_path: Some("multiunit.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete.js",
        baseline_path: Some("delete.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn delete_js_force_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete.js",
        baseline_path: Some("delete.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test01_octal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal.js",
        baseline_path: Some("01.octal.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test01_octal_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal.js",
        baseline_path: Some("01.octal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test01_octal_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal_sm.js",
        baseline_path: Some("01.octal_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test03_assign_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign.js",
        baseline_path: Some("03.assign.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test03_assign_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign.js",
        baseline_path: Some("03.assign_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>03.assign.js</files>
//     <baseline>03.assign_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse -ForceStrictMode</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test03_assign_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign_sm.js",
        baseline_path: Some("03.assign_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>03.assign_sm.js</files>
//     <baseline>03.assign_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test04_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "04.eval.js",
        baseline_path: Some("04.eval.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test05_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments.js",
        baseline_path: Some("05.arguments.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test05_arguments_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test05_arguments_js_serialized_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        compile_flags: vec!["-Serialized", "-ForceStrictMode"],
        tags: vec!["exclude_forceserialized"],
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse -ForceStrictMode</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test05_arguments_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments_sm.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test05_arguments_sm_js_serialized(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments_sm.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        compile_flags: vec!["-Serialized"],
        tags: vec!["exclude_forceserialized"],
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments_sm.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test06_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments.js",
        baseline_path: Some("06.arguments.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test06_arguments_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments.js",
        baseline_path: Some("06.arguments_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>06.arguments.js</files>
//     <baseline>06.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse -ForceStrictMode</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>06.arguments.js</files>
//     <baseline>06.arguments_sm.baseline</baseline>
//     <compile-flags>-force:cachedscope -ForceStrictMode</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test06_arguments_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments_sm.js",
        baseline_path: Some("06.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>06.arguments_sm.js</files>
//     <baseline>06.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>06.arguments_sm.js</files>
//     <baseline>06.arguments_sm.baseline</baseline>
//     <compile-flags>-force:cachedscope</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test07_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "07.arguments.js",
        baseline_path: Some("07.arguments.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test07_arguments_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "07.arguments_sm.js",
        baseline_path: Some("07.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test08_object_literal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral.js",
        baseline_path: Some("08.ObjectLiteral.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test08_object_literal_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral.js",
        baseline_path: Some("08.ObjectLiteral_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test08_object_literal_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral_sm.js",
        baseline_path: Some("08.ObjectLiteral_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test09_object_literal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral.js",
        baseline_path: Some("09.ObjectLiteral.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test09_object_literal_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral.js",
        baseline_path: Some("09.ObjectLiteral_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test09_object_literal_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral_sm.js",
        baseline_path: Some("09.ObjectLiteral_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test10_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "10.eval.js",
        baseline_path: Some("10.eval.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test10_eval_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "10.eval_sm.js",
        baseline_path: Some("10.eval_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test11_this_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this.js",
        baseline_path: Some("11.this.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test11_this_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this.js",
        baseline_path: Some("11.this_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test11_this_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this_sm.js",
        baseline_path: Some("11.this_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test11_this_sm_js_max_interpret_count10(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this_sm.js",
        baseline_path: Some("11.this_sm.baseline"),
        compile_flags: vec!["-maxInterpretCount:10"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test12_delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete.js",
        baseline_path: Some("12.delete.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test12_delete_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete.js",
        baseline_path: Some("12.delete_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test12_delete_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete_sm.js",
        baseline_path: Some("12.delete_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test13_delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "13.delete.js",
        baseline_path: Some("13.delete.baseline"),
        compile_flags: vec!["-ES6RegExPrototypeProperties-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test13_delete_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "13.delete_sm.js",
        baseline_path: Some("13.delete_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test14_var_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var.js",
        baseline_path: Some("14.var.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test14_var_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var.js",
        baseline_path: Some("14.var_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test14_var_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var_sm.js",
        baseline_path: Some("14.var_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test15_with_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with.js",
        baseline_path: Some("15.with.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test15_with_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with.js",
        baseline_path: Some("15.with_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test15_with_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with_sm.js",
        baseline_path: Some("15.with_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test16_catch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch.js",
        baseline_path: Some("16.catch.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test16_catch_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch.js",
        baseline_path: Some("16.catch_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test16_catch_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch_sm.js",
        baseline_path: Some("16.catch_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test17_formal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal.js",
        baseline_path: Some("17.formal.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test17_formal_sm_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal_sm.js",
        baseline_path: Some("17.formal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test17_formal_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal_sm.js",
        baseline_path: Some("17.formal_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test18_formal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal.js",
        baseline_path: Some("18.formal.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test18_formal_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal.js",
        baseline_path: Some("18.formal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test18_formal_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal_sm.js",
        baseline_path: Some("18.formal_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test19_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "19.function.js",
        baseline_path: Some("19.function.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test19_function_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "19.function_sm.js",
        baseline_path: Some("19.function_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test20_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function.js",
        baseline_path: Some("20.function.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test20_function_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function.js",
        baseline_path: Some("20.function_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test20_function_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function_sm.js",
        baseline_path: Some("20.function_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test21_function_declaration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration.js",
        baseline_path: Some("21.functionDeclaration.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test21_function_declaration_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration.js",
        baseline_path: Some("21.functionDeclaration_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test21_function_declaration_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration_sm.js",
        baseline_path: Some("21.functionDeclaration_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test22_caller_callee_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "22.callerCalleeArguments.js",
        baseline_path: Some("22.callerCalleeArguments.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test22_caller_callee_arguments_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "22.callerCalleeArguments_sm.js",
        baseline_path: Some("22.callerCalleeArguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test23_reserved_words_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "23.reservedWords.js",
        baseline_path: Some("23.reservedWords.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test23_reserved_words_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "23.reservedWords_sm.js",
        baseline_path: Some("23.reservedWords_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test24_properties_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties.js",
        baseline_path: Some("24.properties.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test24_properties_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties.js",
        baseline_path: Some("24.properties_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test24_properties_sm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties_sm.js",
        baseline_path: Some("24.properties_sm.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictkwd_js_force_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictkwd.js",
        baseline_path: Some("strictkwd.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictkwd_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictkwd.js",
        baseline_path: Some("strictkwd.baseline"),
        compile_flags: vec!["-Off:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn strictkwd_deferred_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictkwd-deferred.js",
        baseline_path: Some("strictkwd-deferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn comma_bug219390_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comma_bug219390.js",
        baseline_path: Some("comma_bug219390.baseline"),
        compile_flags: vec!["-Off:Deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn nestedfnnameargs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nestedfnnameargs.js",
        baseline_path: Some("nestedfnnameargs.baseline"),
        compile_flags: vec!["-Off:Deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug212755_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug212755.js",
        baseline_path: Some("bug212755.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug212755_js_force_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug212755.js",
        baseline_path: Some("bug212755.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn os_1362136_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_1362136.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn non_simple_parameter_list_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nonSimpleParameterList.js",
        baseline_path: Some("nonSimpleParameterList.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn class_computed_property_name_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classComputedPropertyName.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
