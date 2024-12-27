mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Conversions";

#[test]
fn toint32_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toint32.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn toint32_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toint32_2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn touint32_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "touint32.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn implicit_conversions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ImplicitConversions.js",
        baseline_path: Some("ImplicitConversions_es5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1.js",
        ..Default::default()
    };
    common::run_test(&test);
}
