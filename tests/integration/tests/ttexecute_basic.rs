use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 6] = [
    "sequential",
    "exclude_dynapogo",
    "exclude_snap",
    "exclude_serialized",
    "require_debugger",
    "exclude_sanitize_address",
];
const DIRECTORY: &str = "chakracore-cxx/test/TTExecuteBasic";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn callbackSingle_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callbackSingle.js",
        baseline_path: Some("callbackSingleRecord.baseline"),
        compile_flags: vec!["-TTRecord=callbackSingleTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("callbackSingleReplay.baseline"),
        compile_flags: vec!["-TTReplay=callbackSingleTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn callbackSpread_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callbackSpread.js",
        baseline_path: Some("callbackSpreadRecord.baseline"),
        compile_flags: vec!["-TTRecord=callbackSpreadTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("callbackSpreadReplay.baseline"),
        compile_flags: vec!["-TTReplay=callbackSpreadTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn callbackSequence_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callbackSequence.js",
        baseline_path: Some("callbackSequenceRecord.baseline"),
        compile_flags: vec!["-TTRecord=callbackSequenceTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("callbackSequenceReplay.baseline"),
        compile_flags: vec!["-TTReplay=callbackSequenceTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn callbackClear_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callbackClear.js",
        baseline_path: Some("callbackClearRecord.baseline"),
        compile_flags: vec!["-TTRecord=callbackClearTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("callbackClearReplay.baseline"),
        compile_flags: vec!["-TTReplay=callbackClearTest"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn enumerable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enumerable.js",
        baseline_path: Some("enumerableRecord.baseline"),
        compile_flags: vec!["-TTRecord=enumerableTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("enumerableReplay.baseline"),
        compile_flags: vec!["-TTReplay=enumerableTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn enumeratingWithES5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enumeratingWithES5.js",
        baseline_path: Some("enumeratingWithES5Record.baseline"),
        compile_flags: vec!["-TTRecord=enumeratingWithES5Test", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("enumeratingWithES5Replay.baseline"),
        compile_flags: vec!["-TTReplay=enumeratingWithES5Test", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn enumerationAddDelete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enumerationAddDelete.js",
        baseline_path: Some("enumerationAddDeleteRecord.baseline"),
        compile_flags: vec!["-TTRecord=enumerationAddDeleteTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("enumerationAddDeleteReplay.baseline"),
        compile_flags: vec!["-TTReplay=enumerationAddDeleteTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forEach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forEach.js",
        baseline_path: Some("forEachRecord.baseline"),
        compile_flags: vec!["-TTRecord=forEachTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forEachReplay.baseline"),
        compile_flags: vec!["-TTReplay=forEachTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forInArrayAdd_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInArrayAdd.js",
        baseline_path: Some("forInArrayAddRecord.baseline"),
        compile_flags: vec!["-TTRecord=forInArrayAddTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forInArrayAddReplay.baseline"),
        compile_flags: vec!["-TTReplay=forInArrayAddTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forInObjectAdd_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectAdd.js",
        baseline_path: Some("forInObjectAddRecord.baseline"),
        compile_flags: vec!["-TTRecord=forInObjectAddTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forInObjectAddReplay.baseline"),
        compile_flags: vec!["-TTReplay=forInObjectAddTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forInObjectAddDelete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectAddDelete.js",
        baseline_path: Some("forInObjectAddDeleteRecord.baseline"),
        compile_flags: vec!["-TTRecord=forInObjectAddDeleteTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forInObjectAddDeleteReplay.baseline"),
        compile_flags: vec!["-TTReplay=forInObjectAddDeleteTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forInObjectDelete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInObjectDelete.js",
        baseline_path: Some("forInObjectDeleteRecord.baseline"),
        compile_flags: vec!["-TTRecord=forInObjectDeleteTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("forInObjectDeleteReplay.baseline"),
        compile_flags: vec!["-TTReplay=forInObjectDeleteTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn symbolFor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symbolFor.js",
        baseline_path: Some("symbolForRecord.baseline"),
        compile_flags: vec!["-TTRecord=symbolForTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("symbolForReplay.baseline"),
        compile_flags: vec!["-TTReplay=symbolForTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn try_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try.js",
        baseline_path: Some("tryRecord.baseline"),
        compile_flags: vec!["-TTRecord=tryTest", "-TTSnapInterval=0"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn ttdSentinal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ttdSentinal.js",
        baseline_path: Some("tryReplay.baseline"),
        compile_flags: vec!["-TTReplay=tryTest", "-TTDStartEvent=2"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_snap", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
