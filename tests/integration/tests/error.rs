use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;
use std::time::Duration;


const DIRECTORY: &str = "chakracore-cxx/test/Error";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_cause_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "error_cause.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn aggregate_error_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggregate_error.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_props_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorProps.js",
        baseline_path: Some("errorProps_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_ctor_props_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorCtorProps.js",
        baseline_path: Some("ErrorCtorProps_v3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn native_errors_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NativeErrors.js",
        baseline_path: Some("NativeErrors_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn outofmem_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "outofmem.js",
        baseline_path: Some("outofmem.baseline"),
        compile_flags: vec!["-EnableFatalErrorOnOOM-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(Duration::from_secs(600))]
fn stack_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stack.js",
        tags: HashSet::from(["Slow"]),
        compile_flags: vec!["-JsBuiltIn-", "-off:inline"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stack2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stack2.js",
        baseline_path: Some("stack2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_ctor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorCtor.js",
        baseline_path: Some("errorCtor_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_num_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorNum.js",
        baseline_path: Some("errorNum.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn call_non_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CallNonFunction.js",
        baseline_path: Some("CallNonFunction_3.baseline"),
        tags: HashSet::from(["exclude_native", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_00_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_00.js",
        baseline_path: Some("sourceInfo_00.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_01_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_01.js",
        baseline_path: Some("sourceInfo_01.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_10_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_10.js",
        baseline_path: Some("sourceInfo_10.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_11_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_11.js",
        baseline_path: Some("sourceInfo_11.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_12_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_12.js",
        baseline_path: Some("sourceInfo_12.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_13_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_13.js",
        baseline_path: Some("sourceInfo_13.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_20_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_20.js",
        baseline_path: Some("sourceInfo_20.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn various_errors_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "variousErrors.js",
        baseline_path: Some("variousErrors3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(all(windows, not(target_arch = "aarch64")))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn runtime_compile_stack_overflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "RuntimeCompileStackOverflow.js",
        baseline_path: Some("RuntimeCompileStackOverflow.baseline"),
        tags: HashSet::from(["exclude_forceundodefer"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(Duration::from_secs(300))]
fn encodeoverflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "encodeoverflow.js",
        baseline_path: Some("encodeoverflow.baseline"),
        compile_flags: vec!["-EnableFatalErrorOnOOM-"],
        tags: HashSet::from(["Slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug560940_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug560940.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn stackoverflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackoverflow.js",
        baseline_path: Some("stackoverflow.baseline"),
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn inline_same_func_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineSameFunc.js",
        baseline_path: Some("inlineSameFunc.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-maxinterpretcount:1",
            "-off:simpleJit",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn part_init_stack_frame_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PartInitStackFrame.js",
        baseline_path: Some("PartInitStackFrame.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-nonative",
            "-InjectPartiallyInitializedInterpreterFrameError:3",
            "-InjectPartiallyInitializedInterpreterFrameErrorType:1",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn validate_line_column_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "validate_line_column.js",
        baseline_path: Some("validate_line_column.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn validate_line_column_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "validate_line_column.js",
        baseline_path: Some("validate_line_column.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-force:DeferParse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn error_prototypeto_string_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorPrototypetoString.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn exception_in_error_to_string_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exceptionInErrorToString.js",
        baseline_path: Some("exceptionInErrorToString.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
