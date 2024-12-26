mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Error";

#[test]
fn error_cause_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "error_cause.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn aggregate_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aggregate_error.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn error_props_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorProps.js",
        baseline_path: Some("errorProps_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn error_ctor_props_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ErrorCtorProps.js",
        baseline_path: Some("ErrorCtorProps_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn native_errors_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NativeErrors.js",
        baseline_path: Some("NativeErrors_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn outofmem_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "outofmem.js",
        baseline_path: Some("outofmem.baseline"),
        compile_flags: vec!["-EnableFatalErrorOnOOM-"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stack.js</files>
//     <compile-flags>-JsBuiltIn- -off:inline </compile-flags>
//     <tags>Slow</tags>
//     <timeout>600</timeout>
//   </default>
// </test>

#[test]
fn stack2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stack2.js",
        baseline_path: Some("stack2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn error_ctor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorCtor.js",
        baseline_path: Some("errorCtor_v4.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn error_num_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorNum.js",
        baseline_path: Some("errorNum.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>CallNonFunction.js</files>
//     <baseline>CallNonFunction_3.baseline</baseline>
//     <tags>exclude_native,exclude_dynapogo</tags>
//     <!-- one of the error is different in JIT'ed code (Microsoft/ChakraCore#3011) -->
//   </default>
// </test>

#[test]
fn source_info_00_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_00.js",
        baseline_path: Some("sourceInfo_00.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_01_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_01.js",
        baseline_path: Some("sourceInfo_01.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_10_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_10.js",
        baseline_path: Some("sourceInfo_10.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_11_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_11.js",
        baseline_path: Some("sourceInfo_11.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_12_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_12.js",
        baseline_path: Some("sourceInfo_12.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_13_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_13.js",
        baseline_path: Some("sourceInfo_13.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn source_info_20_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfo_20.js",
        baseline_path: Some("sourceInfo_20.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn various_errors_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "variousErrors.js",
        baseline_path: Some("variousErrors3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>RuntimeCompileStackOverflow.js</files>
//     <baseline>RuntimeCompileStackOverflow.baseline</baseline>
//     <tags>exclude_forceundodefer,exclude_arm,exclude_xplat</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>encodeoverflow.js</files>
//     <baseline>encodeoverflow.baseline</baseline>
//     <compile-flags>-EnableFatalErrorOnOOM-</compile-flags>
//     <tags>Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn bug560940_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug560940.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stackoverflow.js</files>
//     <baseline>stackoverflow.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn inline_same_func_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineSameFunc.js",
        baseline_path: Some("inlineSameFunc.baseline"),
        compile_flags: vec![
            "-ExtendedErrorStackForTestHost",
            "-maxinterpretcount:1",
            "-off:simpleJit",
        ],
    };
    common::run_test(&test);
}

#[test]
fn part_init_stack_frame_js() {
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
    };
    common::run_test(&test);
}

#[test]
fn validate_line_column_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "validate_line_column.js",
        baseline_path: Some("validate_line_column.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
    };
    common::run_test(&test);
}

#[test]
fn validate_line_column_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "validate_line_column.js",
        baseline_path: Some("validate_line_column.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost", "-force:DeferParse"],
    };
    common::run_test(&test);
}

#[test]
fn error_prototypeto_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "errorPrototypetoString.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn exception_in_error_to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exceptionInErrorToString.js",
        baseline_path: Some("exceptionInErrorToString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
