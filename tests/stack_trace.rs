mod common;
const DIRECTORY: &str = "chakracore-cxx/test/StackTrace";

#[test]
fn simple_throw_js_stack_trace_disabled() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn property_validation_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PropertyValidation.js",
        baseline_path: Some("PropertyValidation.nostrict.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn property_validation_js_force_strict_mode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PropertyValidation.js",
        baseline_path: Some("PropertyValidation.nostrict.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-forceStrictMode"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn simple_throw_js() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn long_call_stack_throw_js_args5() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn long_call_stack_throw_js_args6() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn long_call_stack_throw_js_args7() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn long_call_stack_throw_js_args30() {
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
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn stack_trace_limit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackTraceLimit.js",
        baseline_path: Some("StackTraceLimit.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
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

#[test]
fn dynamic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic.js",
        baseline_path: Some("dynamic.js.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn error_prototype_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorPrototype.js",
        baseline_path: Some("ErrorPrototype.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn error_dot_stack_already_exists_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorDotStackAlreadyExists.js",
        baseline_path: Some("ErrorDotStackAlreadyExists.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[test]
fn function_name_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FunctionName.js",
        baseline_path: Some("FunctionName.js.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["StackTrace"],
    };
    common::run_test(&test);
}

#[cfg(target_arch = "x86_64")]
#[test]
fn x64_stack_walk_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "x64StackWalk.js",
        baseline_path: Some("x64StackWalk.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[cfg(target_arch = "x86_64")]
#[test]
fn x64_stack_walk_loop_body_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "x64StackWalkLoopBody.js",
        baseline_path: Some("x64StackWalkLoopBody.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-loopinterpretcount:1"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn dot_chain_name_hint_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dotChainNameHint.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
