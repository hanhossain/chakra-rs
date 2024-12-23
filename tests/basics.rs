mod common;

#[test]
fn accessors_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Accessors.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Accessors.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With.js",
        baseline_path: Some("chakracore-cxx/test/Basics/With.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With2.js",
        baseline_path: Some("chakracore-cxx/test/Basics/With2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defprop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/DefProp.js",
        baseline_path: Some("chakracore-cxx/test/Basics/DefProp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scopedaccessors_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/scopedaccessors.js",
        baseline_path: Some("chakracore-cxx/test/Basics/scopedaccessors.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn flags_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/flags.js",
        baseline_path: Some("chakracore-cxx/test/Basics/flagsES5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inlinecache_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/inlinecache.js",
        baseline_path: Some("chakracore-cxx/test/Basics/inlinecache.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scan_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/scan.js",
        baseline_path: Some("chakracore-cxx/test/Basics/scan.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn enum_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/enum.js",
        baseline_path: Some("chakracore-cxx/test/Basics/enum.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/with3.js",
        baseline_path: Some("chakracore-cxx/test/Basics/with3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug650104_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/bug650104.js",
        baseline_path: Some("chakracore-cxx/test/Basics/bug650104.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_and_re_add_non_extensible_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/DeleteAndReAddNonExtensible.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cross_site_accessor_main_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/cross_site_accessor_main.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Array.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Array.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn script_function_to_strings_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/ScriptFunctionToStrings.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_concat_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/ArrayConcat.js",
        baseline_path: Some("chakracore-cxx/test/Basics/ArrayConcat.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn arrayinit_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/arrayinit.js",
        baseline_path: Some("chakracore-cxx/test/Basics/arrayinit.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn ids_with_escapes_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/IdsWithEscapes.js",
        baseline_path: Some("chakracore-cxx/test/Basics/IdsWithEscapes.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn array_resize_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/ArrayResize.js",
        baseline_path: Some("chakracore-cxx/test/Basics/ArrayResize.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn direct_call_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/DirectCall.js",
        baseline_path: Some("chakracore-cxx/test/Basics/DirectCall.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn equal_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/equal.js",
        baseline_path: Some("chakracore-cxx/test/Basics/equal.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn equal_object_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/equal_object.js",
        baseline_path: Some("chakracore-cxx/test/Basics/equal_object.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn labels_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Labels.js",
        compile_flags: vec!["-MuteHostErrorMsg", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn length_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Length.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Length.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn logical_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Logical.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Logical.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn parameter_order_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/ParameterOrder.js",
        baseline_path: Some("chakracore-cxx/test/Basics/ParameterOrder.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn parameters_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Parameters.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Parameters.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn string_char_code_at_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/StringCharCodeAt.js",
        baseline_path: Some("chakracore-cxx/test/Basics/StringCharCodeAt.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn string_field_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/StringField.js",
        baseline_path: Some("chakracore-cxx/test/Basics/StringField.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn string_from_char_code_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/StringFromCharCode.js",
        baseline_path: Some("chakracore-cxx/test/Basics/StringFromCharCode.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn string_substring_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/StringSubstring.js",
        baseline_path: Some("chakracore-cxx/test/Basics/StringSubstring.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn switch_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/switch.js",
        baseline_path: Some("chakracore-cxx/test/Basics/switch.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn switch2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Switch2.js",
        baseline_path: Some("chakracore-cxx/test/Basics/switch2.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn typeof_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/typeof.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typeofcombi_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/typeofcombi.js",
        baseline_path: Some("chakracore-cxx/test/Basics/typeofcombi.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn type_promotion_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/TypePromotion.js",
        baseline_path: Some("chakracore-cxx/test/Basics/TypePromotion.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn undefined_vs_null_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/UndefinedVsNull.js",
        baseline_path: Some("chakracore-cxx/test/Basics/UndefinedVsNull.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn with_js_defer_parse() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With.js",
        baseline_path: Some("chakracore-cxx/test/Basics/With.baseline"),
        compile_flags: vec!["-force:deferparse"],
    };
    common::run_test(&test);
}

#[test]
fn with_defer_block_scope_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With-defer-block-scope.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_bug940841_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/withBug940841.js",
        compile_flags: vec!["-MaxinterpretCount:1", "-MaxSimpleJITRunCount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_bug940841_2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/withBug940841_2.js",
        compile_flags: vec!["-MaxinterpretCount:0", "-MaxSimpleJITRunCount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn witheval_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/witheval.js",
        baseline_path: Some("chakracore-cxx/test/Basics/witheval.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn ternary_operator_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/TernaryOperator.js",
        baseline_path: Some("chakracore-cxx/test/Basics/TernaryOperator.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn delete_property1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/DeleteProperty1.js",
        baseline_path: Some("chakracore-cxx/test/Basics/DeleteProperty1.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn branching_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Branching.js",
        baseline_path: Some("chakracore-cxx/test/Basics/Branching.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn special_symbol_capture_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/SpecialSymbolCapture.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float_comparison_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/FloatComparison.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
