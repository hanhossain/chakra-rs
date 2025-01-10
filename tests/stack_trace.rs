use common::Variant;
use rstest::rstest;
use std::collections::HashSet;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/StackTrace";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn simple_throw_js_stack_trace_disabled(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SimpleThrow.js",
        baseline_path: Some("simpleThrow.js.stackTraceDisabled.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-errorStackTrace-",
            "-args",
            "runTest",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn property_validation_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PropertyValidation.js",
        baseline_path: Some("PropertyValidation.nostrict.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn property_validation_js_force_strict_mode(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PropertyValidation.js",
        baseline_path: Some("PropertyValidation.nostrict.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-forceStrictMode"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn simple_throw_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SimpleThrow.js",
        baseline_path: Some("SimpleThrow.js.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-args",
            "runTest",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn long_call_stack_throw_js_args5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LongCallStackThrow.js",
        baseline_path: Some("LongCallStackThrow.js.Args5.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-args",
            "runTest",
            "5",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn long_call_stack_throw_js_args6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LongCallStackThrow.js",
        baseline_path: Some("LongCallStackThrow.js.Args6.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-args",
            "runTest",
            "6",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn long_call_stack_throw_js_args7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LongCallStackThrow.js",
        baseline_path: Some("LongCallStackThrow.js.Args7.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-args",
            "runTest",
            "7",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn long_call_stack_throw_js_args30(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LongCallStackThrow.js",
        baseline_path: Some("LongCallStackThrow.js.Args30.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-args",
            "runTest",
            "30",
            "-endargs",
        ],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn stack_trace_limit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackTraceLimit.js",
        baseline_path: Some("StackTraceLimit.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <tags>StackTrace,xplat_verifymark_slow</tags>
//     <files>StackTraceLimitOOS.js</files>
//     <baseline>StackTraceLimitOOS.baseline</baseline>
//     <compile-flags>-ExtendedErrorStackForTestHost</compile-flags>
// </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <tags>StackTrace,xplat_verifymark_slow</tags>
//     <files>StackTraceLimitOOS.js</files>
//     <baseline>StackTraceLimitOOS.baseline</baseline>
//     <compile-flags>-ExtendedErrorStackForTestHost -on:interruptprobe</compile-flags>
//   </default>
// </test>

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dynamic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic.js",
        baseline_path: Some("dynamic.js.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn error_prototype_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorPrototype.js",
        baseline_path: Some("ErrorPrototype.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn error_dot_stack_already_exists_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorDotStackAlreadyExists.js",
        baseline_path: Some("ErrorDotStackAlreadyExists.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn function_name_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionName.js",
        baseline_path: Some("FunctionName.js.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["StackTrace"]),
    };
    common::run_test_variant(&test, variant);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn x64_stack_walk_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "x64StackWalk.js",
        baseline_path: Some("x64StackWalk.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn x64_stack_walk_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "x64StackWalkLoopBody.js",
        baseline_path: Some("x64StackWalkLoopBody.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-loopinterpretcount:1"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dot_chain_name_hint_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dotChainNameHint.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
