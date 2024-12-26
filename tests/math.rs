mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Math";

#[test]
fn neg_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "neg.js",
        baseline_path: Some("neg.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn min_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "min.js",
        baseline_path: Some("min.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn max_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "max.js",
        baseline_path: Some("max.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn miscellaneous_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "miscellaneous.js",
        baseline_path: Some("miscellaneous.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn basics_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn basics_js_sse3() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics.js",
        compile_flags: vec!["-sse:3", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn clz32_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz32.js",
        baseline_path: Some("clz32.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn max2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "max2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn max2_js_jsbuiltin() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "max2.js",
        compile_flags: vec!["-JsBuiltin-"],
        ..Default::default()
    };
    common::run_test(&test);
}
