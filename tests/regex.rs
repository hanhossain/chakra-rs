mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Regex";

#[test]
fn captures_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "captures.js",
        baseline_path: Some("captures.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fast_regex_captures_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fastRegexCaptures.js",
        baseline_path: Some("fastRegexCaptures.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex1.js",
        baseline_path: Some("regex1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_split_optimization_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regexSplitOptimization.js",
        compile_flags: vec!["-skipsplitonnoresult-", "-off:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn match_global_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "match_global.js",
        baseline_path: Some("match_global.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn configurable_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "configurableTest.js",
        baseline_path: Some("configurableTest.baseline"),
        compile_flags: vec!["-ES6RegExPrototypeProperties-"],
    };
    common::run_test(&test);
}

#[test]
fn clamp_numeric_quantifier_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clampNumericQuantifier.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn rx1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rx1.js",
        baseline_path: Some("rx1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_replacefn_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex_replacefn.js",
        baseline_path: Some("regex_replacefn.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_replacefn_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex_replacefn_this.js",
        baseline_path: Some("regex_replacefn_this.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn class_case_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "class-case.js",
        baseline_path: Some("class-case.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prioritizedalternatives_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prioritizedalternatives.js",
        baseline_path: Some("prioritizedalternatives.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn multiline_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiline.js",
        baseline_path: Some("multiline.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_assertion_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex_assertion.js",
        baseline_path: Some("regex_assertion.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_deviations_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex_deviations.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn undefined_option_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "undefined_option.js",
        baseline_path: Some("undefined_option.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unicode_forbidden_escapes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_forbidden_escapes.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

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
fn trigram_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trigram.js",
        baseline_path: Some("trigram.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nul_character_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nul_character.js",
        baseline_path: Some("nul_character.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn replace_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "replace.js",
        baseline_path: Some("replace.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn dot_all_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dotAll.js",
        compile_flags: vec![
            "-ES6RegExPrototypeProperties",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bol_eol_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BolEol.js",
        baseline_path: Some("BolEol.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>crossContext.js</files>
//     <baseline>crossContext.baseline</baseline>
//     <tags>exclude_native</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>crossContext.js</files>
//     <baseline>crossContext.baseline</baseline>
//     <tags>exclude_native</tags>
//   </default>
// </test>

#[test]
fn properties_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "properties.js",
        baseline_path: Some("properties.baseline"),
        compile_flags: vec!["-ES6RegExPrototypeProperties-"],
    };
    common::run_test(&test);
}

#[test]
fn not_boiliteral2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NotBOILiteral2.js",
        baseline_path: Some("NotBOILiteral2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>BoiHardFail.js</files>
//     <baseline>BoiHardFail.baseline</baseline>
//     <compile-flags>-regexDebug</compile-flags>
//     <tags>exclude_test,exclude_serialized</tags>
//   </default>
// </test>

#[test]
fn leadtrail_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "leadtrail.js",
        baseline_path: Some("leadtrail.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug517864_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug517864.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stackregex_box_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackregex_box.js",
        baseline_path: Some("stackregex_box.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blue_102584_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blue_102584_1.js",
        baseline_path: Some("blue_102584_1.baseline"),
        compile_flags: vec!["-MaxinterpretCount:1", "-off:simplejit"],
    };
    common::run_test(&test);
}

#[test]
fn blue_102584_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blue_102584_2.js",
        baseline_path: Some("blue_102584_2.baseline"),
        compile_flags: vec!["-MaxinterpretCount:1", "-off:simplejit"],
    };
    common::run_test(&test);
}

#[test]
fn bug737451_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug737451.js",
        baseline_path: Some("Bug737451.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug1153694_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug1153694.js",
        baseline_path: Some("Bug1153694.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug14859460_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug14859460.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os14763260_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14763260.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug15992535_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug15992535.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn characterclass_with_range_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "characterclass_with_range.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn control_character_escapes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "control_character_escapes.js",
        compile_flags: vec!["-args", "summary", "endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_char_trie_stack_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regexCharTrieStack.js",
        ..Default::default()
    };
    common::run_test(&test);
}
