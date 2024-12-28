mod common;
const DIRECTORY: &str = "chakracore-cxx/test/UnifiedRegex";

#[test]
fn acid_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "acid.js",
        baseline_path: Some("acid.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn assertion_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "assertion.js",
        baseline_path: Some("assertion.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bugFixRegression.js</files>
//     <baseline>bugFixRegression.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bugFixRegression.js</files>
//     <baseline>bugFixRegression.baseline</baseline>
//     <compile-flags>-forceSerialized</compile-flags>
//     <tags>exclude_serialized,Slow</tags>
//   </default>
// </test>

#[test]
fn captures_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "captures.js",
        baseline_path: Some("captures.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn class_case_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "class-case.js",
        baseline_path: Some("class-case.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn crazy_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "crazy.js",
        baseline_path: Some("crazy.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5spec_examples_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5SpecExamples.js",
        baseline_path: Some("es5SpecExamples.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn escapes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "escapes.js",
        baseline_path: Some("escapes.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fast_regex_captures_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fastRegexCaptures.js",
        baseline_path: Some("fastRegexCaptures.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>lastIndex.js</files>
//     <baseline>lastIndex.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn lazy_vs_eager_last_index_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lazyVsEagerLastIndex.js",
        baseline_path: Some("lazyVsEagerLastIndex.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn match_global_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "match_global.js",
        baseline_path: Some("match_global.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn multiline_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiline.js",
        baseline_path: Some("multiline.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn no_backtracking_chomp_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NoBacktrackingChomp.js",
        baseline_path: Some("NoBacktrackingChomp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nul_character_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nul_character.js",
        baseline_path: Some("nul_character.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prioritizedalternatives_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prioritizedalternatives.js",
        baseline_path: Some("prioritizedalternatives.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>quantifiableAssertions.js</files>
//     <baseline>quantifiableAssertions.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn sets_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sets.js",
        baseline_path: Some("sets.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn split_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "split.js",
        baseline_path: Some("split.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>WOOB1138949.js</files>
//     <baseline>WOOB1138949.baseline</baseline>
//     <tags>exclude_debug,Slow</tags>
//   </default>
// </test>

#[test]
fn property_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyString.js",
        baseline_path: Some("propertyString.baseline"),
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-CollectGarbage -Serialized</compile-flags>
//     <files>propertyString.js</files>
//     <baseline>propertyString.baseline</baseline>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

#[test]
fn bug_fix_versioned_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugFixVersioned.js",
        baseline_path: Some("bugFixVersioned.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mru_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mru.js",
        baseline_path: Some("mru.baseline"),
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn source_to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SourceToString.js",
        baseline_path: Some("SourceToString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scanner_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scanner.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
