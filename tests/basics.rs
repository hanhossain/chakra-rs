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
