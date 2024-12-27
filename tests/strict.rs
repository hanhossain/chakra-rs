mod common;
const DIRECTORY: &str = "chakracore-cxx/test/strict";

#[test]
fn global_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "GlobalEval.js",
        baseline_path: Some("GlobalEval.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn basics_function_in_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics_function_in_SM.js",
        baseline_path: Some("basics_function_in_SM.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn basics_function_in_sm_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics_function_in_SM.js",
        baseline_path: Some("basics_function_in_SM.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
    };
    common::run_test(&test);
}

#[test]
fn caller_or_args_no_access_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callerOrArgsNoAccess.js",
        baseline_path: Some("callerOrArgsNoAccess.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stricteval-deferred.js</files>
//     <baseline>stricteval-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stricteval2-deferred.js</files>
//     <baseline>stricteval2-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stricteval3-deferred.js</files>
//     <baseline>stricteval3-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictargs-deferred.js</files>
//     <baseline>strictargs-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictargs2-deferred.js</files>
//     <baseline>strictargs2-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictargs3-deferred.js</files>
//     <baseline>strictargs3-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>evalargs.js</files>
//     <baseline />
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>evalargs.js</files>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//     <baseline />
//   </default>
// </test>

#[test]
fn eval_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThis.js",
        baseline_path: Some("evalThis.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_this2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThis2.js",
        baseline_path: Some("evalThis2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_this_nested_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalThisNested.js",
        baseline_path: Some("evalThisNested.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formal_samename1.js</files>
//     <baseline>formal_samename1.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formal_samename1.js</files>
//     <baseline>formal_samename1.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formal_samename2.js</files>
//     <baseline>formal_samename2.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formal_samename2.js</files>
//     <baseline>formal_samename2.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>multiunit.js</files>
//     <baseline>multiunit.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>delete.js</files>
//     <baseline>delete.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>delete.js</files>
//     <baseline>delete.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn test01_octal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal.js",
        baseline_path: Some("01.octal.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test01_octal_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal.js",
        baseline_path: Some("01.octal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test01_octal_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "01.octal_sm.js",
        baseline_path: Some("01.octal_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test03_assign_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign.js",
        baseline_path: Some("03.assign.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test03_assign_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign.js",
        baseline_path: Some("03.assign_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
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

#[test]
fn test03_assign_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "03.assign_sm.js",
        baseline_path: Some("03.assign_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>04.eval.js</files>
//     <baseline>04.eval.baseline</baseline>
//     <!-- TODO 04.eval_sm.js -->
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>04.eval.js</files>
//     <baseline>04.eval.baseline</baseline>
//     <!-- TODO 04.eval_sm.js -->
//   </default>
// </test>

#[test]
fn test05_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments.js",
        baseline_path: Some("05.arguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test05_arguments_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Serialized -ForceStrictMode</compile-flags>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse -ForceStrictMode</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn test05_arguments_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "05.arguments_sm.js",
        baseline_path: Some("05.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments_sm.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Serialized</compile-flags>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>05.arguments_sm.js</files>
//     <baseline>05.arguments_sm.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn test06_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments.js",
        baseline_path: Some("06.arguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test06_arguments_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments.js",
        baseline_path: Some("06.arguments_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
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

#[test]
fn test06_arguments_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "06.arguments_sm.js",
        baseline_path: Some("06.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
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

#[test]
fn test07_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "07.arguments.js",
        baseline_path: Some("07.arguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test07_arguments_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "07.arguments_sm.js",
        baseline_path: Some("07.arguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test08_object_literal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral.js",
        baseline_path: Some("08.ObjectLiteral.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test08_object_literal_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral.js",
        baseline_path: Some("08.ObjectLiteral_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test08_object_literal_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "08.ObjectLiteral_sm.js",
        baseline_path: Some("08.ObjectLiteral_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test09_object_literal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral.js",
        baseline_path: Some("09.ObjectLiteral.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test09_object_literal_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral.js",
        baseline_path: Some("09.ObjectLiteral_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test09_object_literal_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "09.ObjectLiteral_sm.js",
        baseline_path: Some("09.ObjectLiteral_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test10_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "10.eval.js",
        baseline_path: Some("10.eval.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test10_eval_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "10.eval_sm.js",
        baseline_path: Some("10.eval_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test11_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this.js",
        baseline_path: Some("11.this.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test11_this_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this.js",
        baseline_path: Some("11.this_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test11_this_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "11.this_sm.js",
        baseline_path: Some("11.this_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>11.this_sm.js</files>
//     <baseline>11.this_sm.baseline</baseline>
//     <compile-flags>-maxInterpretCount:10</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn test12_delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete.js",
        baseline_path: Some("12.delete.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test12_delete_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete.js",
        baseline_path: Some("12.delete_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test12_delete_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "12.delete_sm.js",
        baseline_path: Some("12.delete_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test13_delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "13.delete.js",
        baseline_path: Some("13.delete.baseline"),
        compile_flags: vec!["-ES6RegExPrototypeProperties-"],
    };
    common::run_test(&test);
}

#[test]
fn test13_delete_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "13.delete_sm.js",
        baseline_path: Some("13.delete_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test14_var_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var.js",
        baseline_path: Some("14.var.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test14_var_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var.js",
        baseline_path: Some("14.var_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test14_var_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "14.var_sm.js",
        baseline_path: Some("14.var_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test15_with_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with.js",
        baseline_path: Some("15.with.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test15_with_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with.js",
        baseline_path: Some("15.with_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test15_with_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "15.with_sm.js",
        baseline_path: Some("15.with_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test16_catch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch.js",
        baseline_path: Some("16.catch.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test16_catch_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch.js",
        baseline_path: Some("16.catch_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test16_catch_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "16.catch_sm.js",
        baseline_path: Some("16.catch_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test17_formal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal.js",
        baseline_path: Some("17.formal.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test17_formal_sm_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal_sm.js",
        baseline_path: Some("17.formal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test17_formal_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "17.formal_sm.js",
        baseline_path: Some("17.formal_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test18_formal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal.js",
        baseline_path: Some("18.formal.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test18_formal_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal.js",
        baseline_path: Some("18.formal_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test18_formal_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "18.formal_sm.js",
        baseline_path: Some("18.formal_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test19_function_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "19.function.js",
        baseline_path: Some("19.function.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test19_function_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "19.function_sm.js",
        baseline_path: Some("19.function_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test20_function_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function.js",
        baseline_path: Some("20.function.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test20_function_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function.js",
        baseline_path: Some("20.function_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test20_function_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "20.function_sm.js",
        baseline_path: Some("20.function_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test21_function_declaration_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration.js",
        baseline_path: Some("21.functionDeclaration.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test21_function_declaration_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration.js",
        baseline_path: Some("21.functionDeclaration_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test21_function_declaration_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "21.functionDeclaration_sm.js",
        baseline_path: Some("21.functionDeclaration_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test22_caller_callee_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "22.callerCalleeArguments.js",
        baseline_path: Some("22.callerCalleeArguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test22_caller_callee_arguments_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "22.callerCalleeArguments_sm.js",
        baseline_path: Some("22.callerCalleeArguments_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test23_reserved_words_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "23.reservedWords.js",
        baseline_path: Some("23.reservedWords.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test23_reserved_words_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "23.reservedWords_sm.js",
        baseline_path: Some("23.reservedWords_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test24_properties_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties.js",
        baseline_path: Some("24.properties.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test24_properties_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties.js",
        baseline_path: Some("24.properties_sm.baseline"),
        compile_flags: vec!["-ForceStrictMode"],
    };
    common::run_test(&test);
}

#[test]
fn test24_properties_sm_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "24.properties_sm.js",
        baseline_path: Some("24.properties_sm.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictkwd.js</files>
//     <baseline>strictkwd.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictkwd.js</files>
//     <baseline>strictkwd.baseline</baseline>
//     <compile-flags>-Off:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>strictkwd-deferred.js</files>
//     <baseline>strictkwd-deferred.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn comma_bug219390_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comma_bug219390.js",
        baseline_path: Some("comma_bug219390.baseline"),
        compile_flags: vec!["-Off:Deferparse"],
    };
    common::run_test(&test);
}

#[test]
fn nestedfnnameargs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nestedfnnameargs.js",
        baseline_path: Some("nestedfnnameargs.baseline"),
        compile_flags: vec!["-Off:Deferparse"],
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug212755.js</files>
//     <baseline>bug212755.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug212755.js</files>
//     <baseline>bug212755.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn os_1362136_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_1362136.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nonSimpleParameterList.js</files>
//     <baseline>nonSimpleParameterList.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn class_computed_property_name_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classComputedPropertyName.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
