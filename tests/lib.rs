mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Lib";

#[test]
fn construct_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "construct.js",
        baseline_path: Some("construct.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn getclass_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getclass.js",
        baseline_path: Some("getclass1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn length2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "length2.js",
        baseline_path: Some("length.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lib_length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LibLength.js",
        baseline_path: Some("LibLength.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn noargs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "noargs.js",
        baseline_path: Some("noargs_2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tostring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tostring.js",
        baseline_path: Some("toString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn forin_lib_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forin_lib.js",
        baseline_path: Some("forin_lib_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uri_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uri.js",
        baseline_path: Some("uri.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "error.js",
        baseline_path: Some("error.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn workingset_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "workingset.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxyenum_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyenum.js",
        baseline_path: Some("proxyenum.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
