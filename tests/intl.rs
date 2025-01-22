use common::Variant;
use rstest::rstest;
use std::collections::HashSet;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Intl";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn collator_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Collator.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn common_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "common.js",
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn date_time_format_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateTimeFormat.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn number_format_options_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumberFormatOptions.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn supported_locales_of_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SupportedLocalesOf.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_hidden_internals_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlHiddenInternals.js",
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_tainting_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlTaintingTests.js",
        baseline_path: Some("IntlTaintingTests.baseline"),
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_tainting_pre_init_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlTaintingPreInitTests.js",
        baseline_path: Some("IntlTaintingPreInitTests.baseline"),
        tags: HashSet::from(["Intl", "exclude_noicu", "exclude_drt"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_built_ins_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlBuiltIns.js",
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_identities_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlIdentities.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_returned_value_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlReturnedValueTests.js",
        compile_flags: vec![
            "-Intl",
            "-debugLaunch",
            "-dbgbaseline:IntlReturnedValueTests.js.dbg.baseline",
        ],
        tags: HashSet::from(["Intl", "exclude_noicu", "require_winglob"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_internals_hidden_from_exception_stack_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlInternalsHiddenFromExceptionStackTest.js",
        baseline_path: Some("IntlInternalsHiddenFromExceptionStackTest.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost-"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_internals_hidden_from_first_chance_exception_stack_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlInternalsHiddenFromFirstChanceExceptionStackTest.js",
        baseline_path: Some("IntlInternalsHiddenFromFirstChanceExceptionStackTest.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost-"],
        tags: HashSet::from(["Intl", "exclude_noicu"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(unix)]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn intl_platform_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntlPlatform.js",
        compile_flags: vec!["-IntlPlatform"],
        tags: HashSet::from(["Intl", "exclude_noicu", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(unix)]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn plural_rules_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PluralRules.js",
        tags: HashSet::from(["Intl", "exclude_noicu", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(std::time::Duration::from_secs(900))]
fn tainting_prevention_tests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TaintingPreventionTests.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_noicu", "Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
