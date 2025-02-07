use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str] = ["exclude_xplat"];
const DIRECTORY: &str = "chakracore-cxx/test/ConfigParsing";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dummy_js1(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dummy.js",
        baseline_path: Some("dummy.baseline"),
        compile_flags: vec!["-CustomConfigFile:standard.testconfig"],
        tags: HashSet::from(["exclude_xplat"]),
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dummy_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dummy.js",
        baseline_path: Some("dummy.baseline"),
        compile_flags: vec!["-CustomConfigFile:leadingwhitespace.testconfig"],
        tags: HashSet::from(["exclude_xplat"]),
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dummy_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dummy.js",
        baseline_path: Some("dummy.baseline"),
        compile_flags: vec!["-CustomConfigFile:interspersednewline.testconfig"],
        tags: HashSet::from(["exclude_xplat"]),
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dummy_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dummy.js",
        baseline_path: Some("dummy.baseline"),
        compile_flags: vec!["-CustomConfigFile:interspersednewline_whitespace.testconfig"],
        tags: HashSet::from(["exclude_xplat"]),
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dummy_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dummy.js",
        baseline_path: Some("dummy.baseline"),
        compile_flags: vec!["-CustomConfigFile:interspersednewline_leadingwhitespace.testconfig"],
        tags: HashSet::from(["exclude_xplat"]),
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}
