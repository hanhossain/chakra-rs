mod common;
const DIRECTORY: &str = "chakracore-cxx/test/es7";

#[test]
fn asyncawait_syntax_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-syntax.js",
        compile_flags: vec!["-nodeferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncawait_syntax_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-syntax.js",
        compile_flags: vec!["-forcedeferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncawait_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-functionality.js",
        baseline_path: Some("asyncawait-functionality.baseline"),
        compile_flags: vec!["-nodeferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncawait_functionality_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-functionality.js",
        baseline_path: Some("asyncawait-functionality.baseline"),
        compile_flags: vec![
            "-es6experimental",
            "-forcedeferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncawait_undodefer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-undodefer.js",
        baseline_path: Some("asyncawait-undodefer.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stringpad_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringpad.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncawait_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncawait-apis.js",
        compile_flags: vec!["-es7asyncawait", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn values_and_entries_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valuesAndEntries.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn misc_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "misc_bugs.js",
        compile_flags: vec!["-es6experimental", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn misc_bugs_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "misc_bugs.js",
        compile_flags: vec![
            "-es6experimental",
            "-ForceDeferParse",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn immutable_prototype_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "immutable-prototype.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lookupgettersetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lookupgettersetter.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn sharedarraybuffer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sharedarraybuffer.js",
        compile_flags: vec!["-ESSharedArrayBuffer", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn atomics_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "atomics_test.js",
        compile_flags: vec!["-ESSharedArrayBuffer", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
//   <test>
//   <default>
//     <files>PromiseRejectionTracking.js</files>
//     <compile-flags>-TrackRejectedPromises -args summary -endargs -nodeferparse</compile-flags>
//     <baseline>PromiseRejectionTracking.baseline</baseline>
//   </default>
// </test>

#[test]
fn symboldescription_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symboldescription.js",
        compile_flags: vec!["-ESSymbolDescription", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn global_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "globalThis.js",
        compile_flags: vec!["-ESGlobalThis", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn async_generator_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-generator-apis.js",
        compile_flags: vec!["-ES2018AsyncIteration", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn async_generator_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-generator-functionality.js",
        compile_flags: vec!["-ES2018AsyncIteration", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn for_await_of_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for-await-of.js",
        compile_flags: vec!["-ES2018AsyncIteration", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn wellformed_json_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "wellformedJSON.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn json_superset_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "json_superset.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn deferparseclass_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferparseclass.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nullish_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nullish.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
