mod common;
const DIRECTORY: &str = "chakracore-cxx/test/JSON";

#[test]
fn jx1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jx1.js",
        baseline_path: Some("jx1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>jx2.js</files>
//     <baseline>jx2.baseline</baseline>
//     <tags>slow</tags>
//   </default>
// </test>

#[test]
fn cacheassert_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cacheassert.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stringify-replacer.js</files>
//     <baseline>stringify-replacer.baseline</baseline>
//     <compile-flags>-recyclerstress</compile-flags>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

#[test]
fn arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>replacerFunction.js</files>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn space_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "space.js",
        baseline_path: Some("space.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn simple_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple.js",
        baseline_path: Some("simple.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>simple.withLog.js</files>
//     <baseline>simple.withLog.baseline</baseline>
//     <compile-flags>-recyclerstress -trace:JSON</compile-flags>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

#[test]
fn simple_stringify_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple.stringify.js",
        baseline_path: Some("simple.stringify.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>parseWithGc.js</files>
//     <compile-flags>-ForceGCAfterJSONParse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>jsonCache.js</files>
//     <compile-flags>-ForceGCAfterJSONParse</compile-flags>
//     <baseline>jsonCache.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn json_cache_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonCache.js",
        baseline_path: Some("jsonCache.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn json_parse_blue_548957_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "json_parse_Blue_548957.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn json_parse_walk_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonParseWalkTest.js",
        baseline_path: Some("jsonParseWalkTest.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn syntax_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "syntaxError.js",
        baseline_path: Some("syntaxError.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_json_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toJSON.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stackoverflow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackoverflow.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn jsonerrorbuffer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsonerrorbuffer.js",
        ..Default::default()
    };
    common::run_test(&test);
}
