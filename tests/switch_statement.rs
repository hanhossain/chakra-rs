mod common;
const DIRECTORY: &str = "chakracore-cxx/test/switchStatement";

#[test]
fn all_iint_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "allIIntCases.js",
        baseline_path: Some("allIIntCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn empty_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emptyCases.js",
        baseline_path: Some("emptyCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn more_switches1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreSwitches1.js",
        baseline_path: Some("moreSwitches1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn more_switches2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreSwitches2.js",
        baseline_path: Some("moreSwitches2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn switch_math_exp_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchMathExp.js",
        baseline_path: Some("switchMathExp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn all_string_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "allStringCases.js",
        baseline_path: Some("allStringCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_and_non_strings_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringAndNonStrings.js",
        baseline_path: Some("stringAndNonStrings.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn repeat_int_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "repeatIntCases.js",
        baseline_path: Some("repeatIntCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn empty_string_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emptyStringCases.js",
        baseline_path: Some("emptyStringCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn repeat_string_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "repeatStringCases.js",
        baseline_path: Some("repeatStringCases.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn loop_and_retarget_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopAndRetarget.js",
        baseline_path: Some("loopAndRetarget.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn implicit_call_switch_expr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "implicitCallSwitchExpr.js",
        baseline_path: Some("implicitCallSwitchExpr.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-bgjit-",
            "-loopinterpretcount:0",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn simple_switch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simpleSwitch.js",
        baseline_path: Some("simpleSwitch.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_fix_regression_max_interpret_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BugFixRegression_MaxInterpret.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxinterpretcount:1",
            "-off:simpleJit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
    assert!(false);
}

#[test]
fn amd64jscript_number_regression_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "amd64JScriptNumberRegression.js",
        baseline_path: Some("amd64JScriptNumberRegression.baseline"),
        compile_flags: vec![
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:aggressiveinttypespec",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn substring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "substring.js",
        baseline_path: Some("substring.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_dictionary_bail_on_no_profile_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringDictionaryBailOnNoProfileBug.js",
        baseline_path: Some("stringDictionaryBailOnNoProfileBug.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simplejit",
            "-MaxLinearStringCaseCount:2",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn jmp_table_test1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jmpTableTest1.js",
        baseline_path: Some("jmpTableTest1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-MinSwitchJumpTableSize:2",
            "-MaxLinearIntCaseCount:1",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn min_max_case_values_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "minMaxCaseValues.js",
        compile_flags: vec!["-maxInterpretCount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn jmp_table_test2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jmpTableTest2.js",
        baseline_path: Some("jmpTableTest2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-MinSwitchJumpTableSize:1",
            "-MaxLinearIntCaseCount:1",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn duplicate_string_case_arm_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "duplicateStringCaseArmBug.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-loopinterpretcount:0",
            "-bgjit-",
            "-MaxLinearStringCaseCount:2",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn call_between_switch_expr_uses_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CallBetweenSwitchExprUses.js",
        compile_flags: vec![
            "-bgjit-",
            "-loopinterpretcount:1",
            "-off:aggressiveinttypespec",
            "-maxlinearintcasecount:1",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn switch_def_not_string_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchDefNotStringBug.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:2",
            "-maxinterpretcount:1",
            "-maxlinearstringcasecount:2",
            "-forcedeferparse",
            "-off:simplejit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn single_char_string_case_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "singleCharStringCase.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn aggressiveintoff_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggressiveintoff.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn aggressiveintoff_js_aggressive_int_type_spec_off() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggressiveintoff.js",
        compile_flags: vec!["-off:aggressiveinttypespec"],
        ..Default::default()
    };
    common::run_test(&test);
}
