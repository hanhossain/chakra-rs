mod common;
const DIRECTORY: &str = "chakracore-cxx/test/GlobalFunctions";

#[test]
fn global_functions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "GlobalFunctions.js",
        baseline_path: Some("GlobalFunctions.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval1.js",
        baseline_path: Some("eval1_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_nulls_newlines_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalNullsNewlines.js",
        baseline_path: Some("evalNullsNewlines.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn evalreturns_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalreturns.js",
        baseline_path: Some("evalreturns3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_deferred_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalDeferred.js",
        baseline_path: Some("evalDeferred.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_strict_delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval-strict-delete.js",
        baseline_path: Some("eval-strict-delete.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn parse_float_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseFloat.js",
        baseline_path: Some("parseFloat.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn parse_int_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseInt.js",
        baseline_path: Some("parseInt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn parse_short_cut_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseShortCut.js",
        baseline_path: Some("parseShortCut.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>InternalToString.js</files>
//     <baseline>InternalToString.baseline</baseline>
//     <tags>slow</tags>
//   </default>
// </test>

#[test]
fn parse_int1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ParseInt1.js",
        baseline_path: Some("ParseInt1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferunicode.js</files>
//     <baseline>deferunicode.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString.js",
        baseline_path: Some("toString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn can_declare_global_function_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CanDeclareGlobalFunction.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn can_declare_global_function_non_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CanDeclareGlobalFunctionNonEval.js",
        baseline_path: Some("CanDeclareGlobalFunctionNonEval.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
