mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Strings";

#[test]
fn string_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "string_at.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn char_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "charAt.js",
        baseline_path: Some("charAt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn from_char_code_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fromCharCode.js",
        baseline_path: Some("fromCharCode.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn from_code_point_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fromCodePoint.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn char_code_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "charCodeAt.js",
        baseline_path: Some("charCodeAt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat1.js",
        baseline_path: Some("concat1.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat2.js",
        baseline_path: Some("concat2.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat3.js",
        baseline_path: Some("concat3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat4.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat5.js",
        baseline_path: Some("concat5.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat6_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat6.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat7_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat7.js",
        compile_flags: vec!["-off:bailonnoprofile", "-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat_empty_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concat_empty.js",
        baseline_path: Some("concat_empty.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn left_dead_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LeftDead.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn split1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "split1.js",
        baseline_path: Some("split1_v3.baseline"),
        compile_flags: vec!["-ES6ToLength"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_builtin_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringBuiltin.js",
        baseline_path: Some("stringBuiltin.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_case_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toCase.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_replace_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "string_replace.js",
        baseline_path: Some("string_replace.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn compare_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "compare.js",
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
fn replace_xsite_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "replace-xsite.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trim_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trim.js",
        baseline_path: Some("trim.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trimstart_trimend_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trimStart_trimEnd.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lastindexof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lastindexof.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn indexof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "indexof.js",
        baseline_path: Some("indexof.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn neg_index_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "neg_index.js",
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
        compile_flags: vec!["-CollectGarbage", "-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn html_helpers_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HTMLHelpers.js",
        baseline_path: Some("HTMLHelpers.baseline"),
        compile_flags: vec!["-Intl-", "-ES6StringPrototypeFixes"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stringindex_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringindex.js",
        baseline_path: Some("stringindex_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "length.js",
        baseline_path: Some("length.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stringtypespec_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringtypespec.js",
        baseline_path: Some("stringtypespec.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
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
        directory: DIRECTORY,
        source_path: "concatmulti.js",
        baseline_path: Some("concatmulti.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_compoundstring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concatmulti_compoundstring.js",
        baseline_path: Some("concatmulti_compoundstring.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_large_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concatmulti_large.js",
        baseline_path: Some("concatmulti_large.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concatmulti_loop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "concatmulti_loop.js",
        baseline_path: Some("concatmulti_loop.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn long_concatstr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "long_concatstr.js",
        baseline_path: Some("long_concatstr.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_tag_functions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StringTagFunctions.js",
        baseline_path: Some("StringTagFunctions.baseline"),
        compile_flags: vec!["-ES6StringPrototypeFixes"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn string_object_indices_589140_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "string_object_indices_589140.js",
        baseline_path: Some("string_object_indices_589140.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn property_and_index_of_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "property_and_index_of_string.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_3080673_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_3080673.js",
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
        directory: DIRECTORY,
        source_path: "null_embedded_string_toDouble.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constructor_contact_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructorConcat.js",
        compile_flags: vec!["-lic:1", "-mic:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}
