use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;


const DIRECTORY: &str = "chakracore-cxx/test/utf8";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalidutf8_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalidutf8.js",
        baseline_path: Some("invalidutf8.baseline"),
        tags: HashSet::from(["exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unicode_digit_as_identifier_should_work_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_digit_as_identifier_should_work.js",
        tags: HashSet::from(["exclude_serialized", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn os_2977448_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_2977448.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn surrogatepair_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "surrogatepair.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_gh2386_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugGH2386.js",
        compile_flags: vec!["-off:CaptureBytecodeRegUse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unicode_sequence_serialized_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_sequence_serialized.js",
        compile_flags: vec!["-forceserialized", "-oopjit-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_gh2656_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugGH2656.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn utf8_console_log_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "utf8_console_log.js",
        baseline_path: Some("utf8_console_log.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn utf8_console_log_null_embedded_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "utf8_console_log_null_embedded.js",
        baseline_path: Some("utf8_console_log_null_embedded.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
