use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/switchStatement";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn all_iint_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "allIIntCases.js",
        baseline_path: Some("allIIntCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn empty_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emptyCases.js",
        baseline_path: Some("emptyCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn more_switches1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreSwitches1.js",
        baseline_path: Some("moreSwitches1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn more_switches2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreSwitches2.js",
        baseline_path: Some("moreSwitches2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn switch_math_exp_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switchMathExp.js",
        baseline_path: Some("switchMathExp.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn all_string_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "allStringCases.js",
        baseline_path: Some("allStringCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn string_and_non_strings_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringAndNonStrings.js",
        baseline_path: Some("stringAndNonStrings.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn repeat_int_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "repeatIntCases.js",
        baseline_path: Some("repeatIntCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn empty_string_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "emptyStringCases.js",
        baseline_path: Some("emptyStringCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn repeat_string_cases_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "repeatStringCases.js",
        baseline_path: Some("repeatStringCases.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loop_and_retarget_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopAndRetarget.js",
        baseline_path: Some("loopAndRetarget.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn implicit_call_switch_expr_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn simple_switch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simpleSwitch.js",
        baseline_path: Some("simpleSwitch.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn bug_fix_regression_max_interpret_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn amd64jscript_number_regression_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn substring_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "substring.js",
        baseline_path: Some("substring.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn string_dictionary_bail_on_no_profile_bug_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn jmp_table_test1_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn min_max_case_values_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "minMaxCaseValues.js",
        compile_flags: vec!["-maxInterpretCount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn jmp_table_test2_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn duplicate_string_case_arm_bug_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn call_between_switch_expr_uses_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn switch_def_not_string_bug_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn single_char_string_case_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "singleCharStringCase.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn aggressiveintoff_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggressiveintoff.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn aggressiveintoff_js_aggressive_int_type_spec_off(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggressiveintoff.js",
        compile_flags: vec!["-off:aggressiveinttypespec"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
