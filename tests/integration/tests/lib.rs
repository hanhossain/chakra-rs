use crate::common;
use crate::common::Variant;
use rstest::rstest;

const DIRECTORY: &str = "chakracore-cxx/test/Lib";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn construct_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "construct.js",
        baseline_path: Some("construct.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn getclass_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getclass.js",
        baseline_path: Some("getclass1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn length2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "length2.js",
        baseline_path: Some("length.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn lib_length_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LibLength.js",
        baseline_path: Some("LibLength.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn noargs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "noargs.js",
        baseline_path: Some("noargs_2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn tostring_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tostring.js",
        baseline_path: Some("toString.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forin_lib_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forin_lib.js",
        baseline_path: Some("forin_lib_v3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn uri_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uri.js",
        baseline_path: Some("uri.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "error.js",
        baseline_path: Some("error.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn workingset_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "workingset.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn proxyenum_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyenum.js",
        baseline_path: Some("proxyenum.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}
