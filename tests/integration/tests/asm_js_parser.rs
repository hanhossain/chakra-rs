use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_asmjs</tags>

const DIRECTORY: &str = "chakracore-cxx/test/ASMJSParser";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn unary_pos_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UnaryPos.js",
        baseline_path: Some("UnaryPos.baseline"),
        compile_flags: vec!["-on:asmjs", "-testtrace:asmjs"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn defer_reparse_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferReparseBug.js",
        compile_flags: vec!["-on:asmjs", "-forcedeferparse"],
        tags: HashSet::from(["exclude_serialized", "require_backend", "require_asmjs"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
