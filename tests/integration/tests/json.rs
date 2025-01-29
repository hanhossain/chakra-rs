use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;
use std::time::Duration;

const DIRECTORY: &str = "chakracore-cxx/test/JSON";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jx1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jx1.js",
        baseline_path: Some("jx1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn jx2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jx2.js",
        baseline_path: Some("jx2.baseline"),
        tags: HashSet::from(["slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn cacheassert_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cacheassert.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// This fails on macos for some reason
#[cfg(all(not(target_os = "macos"), not(feature = "optimize")))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn stringify_replacer_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringify-replacer.js",
        baseline_path: Some("stringify-replacer.baseline"),
        compile_flags: vec!["-recyclerstress"],
        tags: HashSet::from(["exclude_test", "slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[timeout(Duration::from_secs(300))]
fn replacer_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "replacerFunction.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn space_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "space.js",
        baseline_path: Some("space.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn simple_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple.js",
        baseline_path: Some("simple.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(feature = "optimize"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn simple_with_log_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple.withLog.js",
        baseline_path: Some("simple.withLog.baseline"),
        compile_flags: vec!["-recyclerstress", "-trace:JSON"],
        tags: HashSet::from(["exclude_test", "slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn simple_stringify_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple.stringify.js",
        baseline_path: Some("simple.stringify.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(feature = "optimize"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn parse_with_gc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseWithGc.js",
        compile_flags: vec!["-ForceGCAfterJSONParse"],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(feature = "optimize"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn json_cache_js_force_gc(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonCache.js",
        baseline_path: Some("jsonCache.baseline"),
        compile_flags: vec!["-ForceGCAfterJSONParse"],
        tags: HashSet::from(["exclude_test"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn json_cache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonCache.js",
        baseline_path: Some("jsonCache.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn json_parse_blue_548957_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "json_parse_Blue_548957.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn json_parse_walk_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonParseWalkTest.js",
        baseline_path: Some("jsonParseWalkTest.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn syntax_error_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "syntaxError.js",
        baseline_path: Some("syntaxError.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_json_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toJSON.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stackoverflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackoverflow.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsonerrorbuffer_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonerrorbuffer.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
