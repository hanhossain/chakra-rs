use crate::common;
use crate::common::Variant;
use rstest::rstest;

const DIRECTORY: &str = "chakracore-cxx/test/Miscellaneous";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn has_only_writable_data_properties_cache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HasOnlyWritableDataPropertiesCache.js",
        baseline_path: Some("HasOnlyWritableDataPropertiesCache.baseline"),
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(optimized))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn long_string_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "longstring.js",
        compile_flags: vec!["-recyclerVerify"],
        tags: std::collections::HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_alias_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalAlias.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn set_timeout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SetTimeout.js",
        baseline_path: Some("SetTimeout.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn monotonic_now_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "monotonicNow.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn null_byte_comment_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nullByte-comment.js",
        baseline_path: Some("nullByte-comment.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn null_byte_regex_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nullByte-regex.js",
        baseline_path: Some("nullByte-regex.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn null_byte_string_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nullByte-string.js",
        baseline_path: Some("nullByte-string.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn may_have_side_effect_on_node_so_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MayHaveSideEffectOnNodeSO.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn keep_context_in_super_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "KeepContextInSuper.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
