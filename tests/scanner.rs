mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Scanner";

#[test]
fn numeric_literal_suffix_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumericLiteralSuffix.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn invalid_character_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InvalidCharacter.js",
        baseline_path: Some("InvalidCharacter.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn hashbang_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Hashbang.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-ESHashbang"],
        ..Default::default()
    };
    common::run_test(&test);
}
