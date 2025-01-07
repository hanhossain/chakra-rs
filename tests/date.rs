use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Date";
// TODO (hanhossain): many of these tests depend on the machine's timezone.

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>DateCtr.js</files>
//     <tags>slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>DateGetSet.js</files>
//     <baseline>DateGetSet.baseline</baseline>
//     <!-- XPLAT doesn't provide expected DST info for year 35816 -->
//     <tags>exclude_xplat,slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>DateParse.js</files>
//     <baseline>DateParse_es5.baseline</baseline>
//     <tags>slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>DateParse2.js</files>
//     <baseline>DateParse2.v5.baseline</baseline>
//     <!-- xplat DST info for BC years don't mach to ones for Windows. Internally trying to match them is very expensive -->
//     <tags>slow,exclude_xplat</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Conversions.js</files>
//     <baseline>Conversions.win8.baseline</baseline>
//     <compile-flags>-Intl-</compile-flags>
//     <tags>exclude_win7,exclude_winBlue,exclude_snap,exclude_xplat,slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Conversions.js</files>
//     <baseline>Conversions.baseline</baseline>
//     <compile-flags>-Intl-</compile-flags>
//     <tags>exclude_win7,exclude_win8,exclude_snap,exclude_xplat,slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>date_cache_bug.js</files>
//     <baseline>date_cache_bug.baseline</baseline>
//     <timeout>120</timeout> <!-- ARM64 take more than 60 -->
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>formatting_xplat.js</files>
//     <tags>slow,exclude_windows</tags>
//   </default>
// </test>

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

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn marshalbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marshalbug.js",
        baseline_path: Some(""),
        compile_flags: vec!["-nonative", "-intl-"],
        tags: vec!["exclude_test", "exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>xplatInterval.js</files>
//     <tags>Slow,exclude_windows</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>toStringAndToUTCStringYearPadding.js</files>
//     <!-- xplat tz info for BC != Windows tz info for BC -->
//     <tags>slow,exclude_xplat</tags>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

#[cfg(windows)]
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
    common::run_test_variant(&test, variant);
}

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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>parseValidISO.js</files>
//     <baseline>parseValidISO.baseline</baseline>
//     <!-- xplat tz info for BC != Windows tz info for BC -->
//     <tags>slow,exclude_xplat</tags>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>parseInvalidISO.js</files>
//     <baseline>parseInvalidISO.baseline</baseline>
//     <!-- OSX doesn't provide expected DST for minus years -->
//     <tags>slow,exclude_xplat</tags>
//   </default>
// </test>
