mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Number";

#[test]
fn negative_na_n_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NegativeNaN.js",
        baseline_path: Some("NegativeNaN.baseline"),
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn negative_na_n_js_no_native() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NegativeNaN.js",
        baseline_path: Some("NegativeNaN.baseline"),
        compile_flags: vec!["-NoNative"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO Investigate and re-enable this test on ARM64 macOS
#[cfg(not(target_os = "macos"))]
#[test]
fn to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString.js",
        baseline_path: Some("toString_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn floatcmp_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "floatcmp.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn na_n_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NaN.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn integer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "integer.js",
        baseline_path: Some("integer.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_uint16_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toUint16.js",
        baseline_path: Some("toUint16.baseline"),
        tags: vec!["exclude_drt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn boundaries_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boundaries.js",
        baseline_path: Some("boundaries.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn no_sse_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NoSse.js",
        baseline_path: Some("NoSse.baseline"),
        compile_flags: vec!["-sse:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn property_and_index_of_number_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "property_and_index_of_number.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn numeric_separator_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumericSeparator.js",
        compile_flags: vec!["-ESNumericSeparator", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
