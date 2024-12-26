mod common;

#[test]
fn string_at_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/string_at.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn char_at_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/charAt.js",
        baseline_path: Some("chakracore-cxx/test/Strings/charAt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn from_char_code_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/fromCharCode.js",
        baseline_path: Some("chakracore-cxx/test/Strings/fromCharCode.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn from_code_point_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/fromCodePoint.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn char_code_at_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/charCodeAt.js",
        baseline_path: Some("chakracore-cxx/test/Strings/charCodeAt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat1.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concat1.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn concat2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat2.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concat2.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn concat3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat3.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concat3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat4.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat5_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat5.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concat5.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn concat6_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat6.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat7_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat7.js",
        compile_flags: vec!["-off:bailonnoprofile", "-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat_empty_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concat_empty.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concat_empty.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn left_dead_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/LeftDead.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn split1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/split1.js",
        baseline_path: Some("chakracore-cxx/test/Strings/split1_v3.baseline"),
        compile_flags: vec!["-ES6ToLength"],
    };
    common::run_test(&test);
}

#[test]
fn string_builtin_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/stringBuiltin.js",
        baseline_path: Some("chakracore-cxx/test/Strings/stringBuiltin.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_case_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/toCase.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_replace_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/string_replace.js",
        baseline_path: Some("chakracore-cxx/test/Strings/string_replace.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn compare_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/compare.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn replace_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/replace.js",
        baseline_path: Some("chakracore-cxx/test/Strings/replace.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn replace_xsite_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/replace-xsite.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trim_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/trim.js",
        baseline_path: Some("chakracore-cxx/test/Strings/trim.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trimstart_trimend_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/trimStart_trimEnd.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lastindexof_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/lastindexof.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn indexof_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/indexof.js",
        baseline_path: Some("chakracore-cxx/test/Strings/indexof.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn neg_index_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/neg_index.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn substring_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/substring.js",
        baseline_path: Some("chakracore-cxx/test/Strings/substring.baseline"),
        compile_flags: vec!["-CollectGarbage", "-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn html_helpers_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/HTMLHelpers.js",
        baseline_path: Some("chakracore-cxx/test/Strings/HTMLHelpers.baseline"),
        compile_flags: vec!["-Intl-", "-ES6StringPrototypeFixes"],
    };
    common::run_test(&test);
}

#[test]
fn stringindex_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/stringindex.js",
        baseline_path: Some("chakracore-cxx/test/Strings/stringindex_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn length_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/length.js",
        baseline_path: Some("chakracore-cxx/test/Strings/length.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn stringtypespec_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/stringtypespec.js",
        baseline_path: Some("chakracore-cxx/test/Strings/stringtypespec.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>CompoundString.js</files>
//     <baseline>CompoundString.baseline</baseline>
//     <compile-flags>-minInterpretCount:1 -maxInterpretCount:1 -off:simpleJit -Intl-</compile-flags>
//     <tags>exclude_dynapogo,Slow</tags>
//   </default>
// </test>

#[test]
fn concatmulti_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concatmulti.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concatmulti.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_compoundstring_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concatmulti_compoundstring.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concatmulti_compoundstring.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_large_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concatmulti_large.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concatmulti_large.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_loop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/concatmulti_loop.js",
        baseline_path: Some("chakracore-cxx/test/Strings/concatmulti_loop.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn long_concatstr_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/long_concatstr.js",
        baseline_path: Some("chakracore-cxx/test/Strings/long_concatstr.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_tag_functions_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/StringTagFunctions.js",
        baseline_path: Some("chakracore-cxx/test/Strings/StringTagFunctions.baseline"),
        compile_flags: vec!["-ES6StringPrototypeFixes"],
    };
    common::run_test(&test);
}

#[test]
fn string_object_indices_589140_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/string_object_indices_589140.js",
        baseline_path: Some("chakracore-cxx/test/Strings/string_object_indices_589140.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn property_and_index_of_string_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/property_and_index_of_string.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_3080673_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/bug_OS_3080673.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_toUpperCase_toLowerCase.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//     <tags>exclude_win7,exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn null_embedded_string_to_double_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/null_embedded_string_toDouble.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constructor_contact_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Strings/constructorConcat.js",
        compile_flags: vec!["-lic:1", "-mic:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}
