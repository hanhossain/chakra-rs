use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;
#[cfg(feature = "optimized-tests")]
use std::time::Duration;

const DIRECTORY: &str = "chakracore-cxx/test/Date";

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn date_ctr_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateCtr.js",
        tags: HashSet::from(["slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn date_get_set_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateGetSet.js",
        baseline_path: Some("DateGetSet.baseline"),
        tags: HashSet::from(["exclude_xplat", "slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn date_parse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateParse.js",
        baseline_path: Some("DateParse_es5.baseline"),
        tags: HashSet::from(["slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn date_parse2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateParse2.js",
        baseline_path: Some("DateParse2.v5.baseline"),
        tags: HashSet::from(["slow", "exclude_xplat"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn date_parse3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateParse3.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_iso_3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toISO_3.js",
        baseline_path: Some("toISO_3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dateutc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dateutc.js",
        baseline_path: Some("dateutc.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dateutc_dategmt_same_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateUTC-DateGMT-same.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn millisecond_truncation_check_after_copy_constructor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MillisecondTruncationCheckAfterCopyConstructor.js",
        baseline_path: Some("MillisecondTruncationCheckAfterCopyConstructor.es6.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn conversions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Conversions.js",
        baseline_path: Some("Conversions.win8.baseline"),
        compile_flags: vec!["-Intl-"],
        tags: HashSet::from([
            "exclude_win7",
            "exclude_winBlue",
            "exclude_snap",
            "exclude_xplat",
            "slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn conversions_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Conversions.js",
        baseline_path: Some("Conversions.baseline"),
        compile_flags: vec!["-Intl-"],
        tags: HashSet::from([
            "exclude_win7",
            "exclude_winBlue",
            "exclude_snap",
            "exclude_xplat",
            "slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[timeout(Duration::from_secs(120))]
fn date_cache_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "date_cache_bug.js",
        baseline_path: Some("date_cache_bug.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(unix, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn formatting_xplat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formatting_xplat.js",
        tags: HashSet::from(["slow", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

// TODO: on DST pass Win OldDateAPI jumps back to 01:00 after 01:59. Do not force OLDDateAPI?
#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn formatting_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formatting.js",
        baseline_path: Some("formatting.baseline"),
        compile_flags: vec!["-ForceOldDateAPI"],
        tags: HashSet::from(["slow", "exclude_xplat"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn marshalbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marshalbug.js",
        baseline_path: Some(""),
        compile_flags: vec!["-nonative", "-intl-"],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

// TODO (hanhossain): this should be all unix, but this takes way too long on mac. maybe it should be exclude_debug
#[cfg(all(target_os = "linux", feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn xplat_interval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "xplatInterval.js",
        tags: HashSet::from(["slow", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn military_time_zone_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MilitaryTimeZone.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn two_digit_years_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TwoDigitYears.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn to_string_and_to_utcstring_year_padding_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringAndToUTCStringYearPadding.js",
        tags: HashSet::from(["slow", "exclude_xplat"]),
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn parse_to_string_results_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseToStringResults.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn parse_to_utcstring_and_to_isostring_results_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseToUTCStringAndToISOStringResults.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn parse_invalid_iso_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseInvalidISO.js",
        baseline_path: Some("parseInvalidISO.baseline"),
        tags: HashSet::from(["slow", "exclude_xplat"]),
        compile_flags: vec!["-args", "summary", "-endargs"],
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(all(windows, feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn parse_invalid_iso2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseInvalidISO.js",
        baseline_path: Some("parseInvalidISO.baseline"),
        tags: HashSet::from(["slow", "exclude_xplat"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}
