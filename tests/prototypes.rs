mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Prototypes";

#[test]
fn prototype_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Prototype.js",
        baseline_path: Some("Prototype.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Prototype2.js",
        baseline_path: Some("Prototype2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn deep_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deep.js",
        baseline_path: Some("deep.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn init_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "initProto.js",
        baseline_path: Some("initProto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn change_prototype_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ChangePrototype.js",
        baseline_path: Some("ChangePrototype.baseline"),
        compile_flags: vec!["-trace:TypeShareForChangePrototype", "-JsBuiltIn-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn read_only_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReadOnly.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn shadow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadow.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn shadow2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadow2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn no_prototype_for_method_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NoPrototypeForMethod.js",
        ..Default::default()
    };
    common::run_test(&test);
}
