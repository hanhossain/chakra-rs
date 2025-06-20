use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 4] = [
    "exclude_serialized",
    "exclude_snap",
    "require_debugger",
    "exclude_sanitize_address",
];
const DIRECTORY: &str = "chakracore-cxx/test/Debugger";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn failfast_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "failfast.js",
        compile_flags: vec!["-dbgbaseline:empty.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_breakpoints_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagBreakpoints.js",
        compile_flags: vec!["-dbgbaseline:JsDiagBreakpoints.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_breakpoints_array_buffer_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagBreakpoints_ArrayBuffer.js",
        compile_flags: vec!["-dbgbaseline:JsDiagBreakpoints.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_break_on_uncaught_exception_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagBreakOnUncaughtException.js",
        baseline_path: Some("JsDiagBreakOnUncaughtException.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagBreakOnUncaughtException.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_exceptions_in_promises_break_on_uncaught_exceptions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.js",
        baseline_path: Some("JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_exceptions_in_promises_break_on_first_chance_exceptions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.js",
        baseline_path: Some("JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_exceptions_in_async_functions_break_on_uncaught_exceptions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.js",
        baseline_path: Some("JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_evaluate_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagEvaluate.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagEvaluate.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_get_function_position_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagGetFunctionPosition.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagGetFunctionPosition.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// xplat-todo: enable on xplat when Intl is supported on xplat (Microsoft/ChakraCore#2919)
#[cfg(windows)]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_get_function_position_intl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagGetFunctionPositionIntl.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagGetFunctionPositionIntl.js.dbg.baseline",
            "-Intl",
        ],
        tags: HashSet::from([
            "exclude_xplat",
            "Intl",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_get_scripts_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagGetScripts.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagGetScripts.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_get_stack_properties_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagGetStackProperties.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagGetStackProperties.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_get_stack_trace_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagGetStackTrace.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagGetStackTrace.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_diag_request_async_break_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDiagRequestAsyncBreak.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsDiagRequestAsyncBreak.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsrt_debug_utils_add_property_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsrtDebugUtilsAddPropertyType.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:JsrtDebugUtilsAddPropertyType.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multiple_context_stack_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MultipleContextStack.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:MultipleContextStack.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dump_function_properties_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dumpFunctionProperties.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:dumpFunctionProperties.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loadscript_after_detach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loadscript_after_detach.js",
        compile_flags: vec!["-dbgbaseline:emptyJson.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn js_debugger_attach_detach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JsDebuggerAttachDetach.js",
        baseline_path: Some("JsDebuggerAttachDetach.js.baseline"),
        compile_flags: vec!["-dbgbaseline:JsDebuggerAttachDetach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn exception_while_fetching_prop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exceptionWhileFetchingProp.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:exceptionWhileFetchingProp.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
