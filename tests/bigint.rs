mod common;
const DIRECTORY: &str = "chakracore-cxx/test/BigInt";

#[test]
fn comparison_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comparison.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn global_object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "global_object.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn increment_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "increment.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn decrement_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "decrement.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn assign_by_value_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "assign_by_value.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn add_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "add.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn subtract_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "subtract.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mixed_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mixed.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn strictly_equal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictly_equal.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bitwise_not_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bitwise_not.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn multiply_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiply.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESBigInt"],
        ..Default::default()
    };
    common::run_test(&test);
}
