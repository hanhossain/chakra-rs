use common::Variant;
use rstest::rstest;
use std::collections::HashSet;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Date";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn date_ctr_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateCtr.js",
        tags: HashSet::from(["slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn date_parse3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateParse3.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn to_iso_3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toISO_3.js",
        baseline_path: Some("toISO_3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dateutc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dateutc.js",
        baseline_path: Some("dateutc.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dateutc_dategmt_same_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateUTC-DateGMT-same.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn millisecond_truncation_check_after_copy_constructor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MillisecondTruncationCheckAfterCopyConstructor.js",
        baseline_path: Some("MillisecondTruncationCheckAfterCopyConstructor.es6.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn conversions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Conversions.js",
        baseline_path: Some("Conversions.win8.baseline"),
        compile_flags: vec!["-Intl-"],
        tags: vec![
            "exclude_win7",
            "exclude_winBlue",
            "exclude_snap",
            "exclude_xplat",
            "slow",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn conversions_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Conversions.js",
        baseline_path: Some("Conversions.baseline"),
        compile_flags: vec!["-Intl-"],
        tags: vec![
            "exclude_win7",
            "exclude_winBlue",
            "exclude_snap",
            "exclude_xplat",
            "slow",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>date_cache_bug.js</files>
//     <baseline>date_cache_bug.baseline</baseline>
//     <timeout>120</timeout> <!-- ARM64 take more than 60 -->
//   </default>
// </test>

#[cfg(unix)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn formatting_xplat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formatting_xplat.js",
        tags: HashSet::from(["slow", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formatting.js</files>
//     <baseline>formatting.baseline</baseline>
//     <compile-flags>-ForceOldDateAPI</compile-flags>
//     <!-- on DST pass Win OldDateAPI jumps back to 01:00 after 01:59 -->
//     <!-- todo: Do not force OLDDateAPI ? -->
//     <tags>slow,exclude_xplat</tags>
//   </default>
//   <condition order="1" type="include">
//     <os>win8</os>
//     <override>
//       <baseline>formatting.win8.baseline</baseline>
//     </override>
//   </condition>
// </test>

#[cfg(not(feature = "optimized-test"))]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn marshalbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marshalbug.js",
        baseline_path: Some(""),
        compile_flags: vec!["-nonative", "-intl-"],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): this should be all unix, but this takes way too long on mac. maybe it should be exclude_debug
#[cfg(target_os = "linux")]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn xplat_interval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "xplatInterval.js",
        tags: HashSet::from(["slow", "exclude_windows"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn military_time_zone_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MilitaryTimeZone.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn two_digit_years_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TwoDigitYears.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn parse_to_string_results_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseToStringResults.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn parse_to_utcstring_and_to_isostring_results_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parseToUTCStringAndToISOStringResults.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}

#[cfg(windows)]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
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
    common::run_test_variant(&test, variant);
}
