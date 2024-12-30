mod common;
const DIRECTORY: &str = "chakracore-cxx/test/utf8";

#[test]
fn invalidutf8_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalidutf8.js",
        baseline_path: Some("invalidutf8.baseline"),
        tags: vec!["exclude_serialized"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_digit_as_identifier_should_work.js</files>
//     <tags>exclude_serialized,exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn os_2977448_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_2977448.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn surrogatepair_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "surrogatepair.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_gh2386_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugGH2386.js",
        compile_flags: vec!["-off:CaptureBytecodeRegUse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unicode_sequence_serialized_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_sequence_serialized.js",
        compile_flags: vec!["-forceserialized", "-oopjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bugGH2656.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//     <tags>exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn utf8_console_log_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "utf8_console_log.js",
        baseline_path: Some("utf8_console_log.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn utf8_console_log_null_embedded_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "utf8_console_log_null_embedded.js",
        baseline_path: Some("utf8_console_log_null_embedded.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
