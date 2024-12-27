mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Basics";

#[test]
fn array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Array.js",
        baseline_path: Some("Array.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn script_function_to_strings_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ScriptFunctionToStrings.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate test
// <test>
//   <default>
//     <files>DomProperties.js</files>
//     <compile-flags>-Intl-</compile-flags>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn array_concat_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayConcat.js",
        baseline_path: Some("ArrayConcat.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arrayinit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrayinit.js",
        baseline_path: Some("arrayinit.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ids_with_escapes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IdsWithEscapes.js",
        baseline_path: Some("IdsWithEscapes.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_resize_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayResize.js",
        baseline_path: Some("ArrayResize.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn direct_call_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DirectCall.js",
        baseline_path: Some("DirectCall.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equal.js",
        baseline_path: Some("equal.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equal_object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equal_object.js",
        baseline_path: Some("equal_object.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn labels_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Labels.js",
        compile_flags: vec!["-MuteHostErrorMsg", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Length.js",
        baseline_path: Some("Length.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn logical_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Logical.js",
        baseline_path: Some("Logical.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn parameter_order_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ParameterOrder.js",
        baseline_path: Some("ParameterOrder.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn parameters_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Parameters.js",
        baseline_path: Some("Parameters.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_char_code_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StringCharCodeAt.js",
        baseline_path: Some("StringCharCodeAt.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_field_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StringField.js",
        baseline_path: Some("StringField.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_from_char_code_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StringFromCharCode.js",
        baseline_path: Some("StringFromCharCode.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_substring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StringSubstring.js",
        baseline_path: Some("StringSubstring.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn switch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "switch.js",
        baseline_path: Some("switch.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn switch2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Switch2.js",
        baseline_path: Some("switch2.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typeof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typeof.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typeofcombi_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typeofcombi.js",
        baseline_path: Some("typeofcombi.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn type_promotion_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypePromotion.js",
        baseline_path: Some("TypePromotion.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn undefined_vs_null_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UndefinedVsNull.js",
        baseline_path: Some("UndefinedVsNull.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "With.js",
        baseline_path: Some("With.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "With.js",
        baseline_path: Some("With.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_defer_block_scope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "With-defer-block-scope.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_bug940841_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "withBug940841.js",
        compile_flags: vec!["-MaxinterpretCount:1", "-MaxSimpleJITRunCount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_bug940841_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "withBug940841_2.js",
        compile_flags: vec!["-MaxinterpretCount:0", "-MaxSimpleJITRunCount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "With2.js",
        baseline_path: Some("With2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn witheval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "witheval.js",
        baseline_path: Some("witheval.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ternary_operator_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TernaryOperator.js",
        baseline_path: Some("TernaryOperator.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_property1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeleteProperty1.js",
        baseline_path: Some("DeleteProperty1.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_and_re_add_non_extensible_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeleteAndReAddNonExtensible.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn accessors_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Accessors.js",
        baseline_path: Some("Accessors.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defprop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DefProp.js",
        baseline_path: Some("DefProp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scopedaccessors_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scopedaccessors.js",
        baseline_path: Some("scopedaccessors.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn flags_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "flags.js",
        baseline_path: Some("flagsES5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn branching_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Branching.js",
        baseline_path: Some("Branching.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inlinecache_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlinecache.js",
        baseline_path: Some("inlinecache.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scan_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scan.js",
        baseline_path: Some("scan.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn enum_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enum.js",
        baseline_path: Some("enum.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with3.js",
        baseline_path: Some("with3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cross_site_accessor_main_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cross_site_accessor_main.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug650104_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug650104.js",
        baseline_path: Some("bug650104.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn special_symbol_capture_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SpecialSymbolCapture.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate test
// <test>
//   <default>
//     <files>VerifyParserState.js</files>
//     <baseline>VerifyParserState.baseline</baseline>
//     <compile-flags>-UseParserStateCache -ParserStateCache -Force:DeferParse -Trace:CreateParserState</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate test
// <test>
//   <default>
//     <files>VerifySkipNestedDeferred.js</files>
//     <baseline>VerifySkipNestedDeferred.baseline</baseline>
//     <compile-flags>-UseParserStateCache -ParserStateCache -Force:DeferParse -Trace:SkipNestedDeferred</compile-flags>
//     <tags>exclude_test,exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate test
// <test>
//   <default>
//     <files>bug_os17542375.js</files>
//     <compile-flags>-UseParserStateCache -ParserStateCache -Force:DeferParse -pageheap:2</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate test
// <test>
//   <default>
//     <files>bug_os16855035.js</files>
//     <compile-flags>-UseParserStateCache -ParserStateCache -Force:DeferParse -Force:Redeferral -CollectGarbage</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn float_comparison_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FloatComparison.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
