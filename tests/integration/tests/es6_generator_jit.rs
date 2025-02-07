use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 1] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/es6GeneratorJit";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn generator_jit_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generator-jit-bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn generator_jit_bugs_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generator-jit-bugs.js",
        compile_flags: vec!["-JitES6Generators", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn generator_jit_bugs_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generator-jit-bugs.js",
        compile_flags: vec![
            "-JitES6Generators",
            "-off:simplejit",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn generator_jit_bugs_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generator-jit-bugs.js",
        compile_flags: vec![
            "-JitES6Generators",
            "-off:fulljit",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: HashSet::from(["require_backend", "exclude_nonative", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn async_jit_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-jit-bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn async_jit_bugs_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-jit-bugs.js",
        compile_flags: vec!["-JitES6Generators", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn async_jit_bugs_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-jit-bugs.js",
        compile_flags: vec![
            "-JitES6Generators",
            "-off:simplejit",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn async_jit_bugs_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-jit-bugs.js",
        compile_flags: vec![
            "-JitES6Generators",
            "-off:fulljit",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: HashSet::from(["require_backend", "exclude_nonative", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn async_jit_bugs_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async-jit-bugs.js",
        compile_flags: vec!["-off:fulljit", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn new_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newtest.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn new_test_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newtest.js",
        compile_flags: vec!["-off:simplejit", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn new_test_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newtest.js",
        compile_flags: vec!["-off:fulljit", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn new_test_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newtest.js",
        compile_flags: vec!["-off:fulljit", "-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend", "exclude_nonative", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_gen_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit-gen-loop-body.js",
        baseline_path: Some("jit-gen-loop-body.baseline"),
        compile_flags: vec!["-testtrace:Backend"],
        tags: HashSet::from([
            "require_backend",
            "exclude_test",
            "exclude_nonative",
            "exclude_dynapogo",
            todo!(""),
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>jit-gen-loop-body.js</files>
//     <baseline>jit-gen-loop-body-2.baseline</baseline>
//     <tags>require_backend,exclude_nonative</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_async_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit-async-loop-body.js",
        baseline_path: Some("jit-async-loop-body.baseline"),
        compile_flags: vec!["-testtrace:Backend"],
        tags: HashSet::from([
            "require_backend",
            "exclude_test",
            "exclude_nonative",
            "exclude_dynapogo",
            todo!(""),
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>jit-async-loop-body.js</files>
//     <baseline>jit-async-loop-body-2.baseline</baseline>
//     <tags>require_backend,exclude_nonative</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_module_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit-module-loop-body.js",
        baseline_path: Some("jit-module-loop-body.baseline"),
        compile_flags: vec!["-testtrace:Backend", "-module"],
        tags: HashSet::from([
            "require_backend",
            "exclude_test",
            "exclude_nonative",
            "exclude_dynapogo",
            todo!(""),
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_module_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit-module-loop-body.js",
        baseline_path: Some("jit-module-loop-body-2.baseline"),
        compile_flags: vec!["-module"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
