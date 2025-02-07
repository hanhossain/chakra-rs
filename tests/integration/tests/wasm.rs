use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 3] = ["exclude_serialized", "require_backend", "require_wasm"];
const DIRECTORY: &str = "chakracore-cxx/test/wasm";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        compile_flags: vec!["-wasm", "-args", "0", "5", "-endargs"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        compile_flags: vec!["-wasm", "-args", "6", "10", "-endargs"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        compile_flags: vec!["-wasm", "-args", "11", "15", "-endargs"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        compile_flags: vec!["-wasm", "-args", "16", "20", "-endargs"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsigned_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsigned.js",
        compile_flags: vec!["-wasm", "-args", "21", "-endargs"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn regress_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regress.js",
        compile_flags: vec!["-wasm", "-args", "--no-verbose", "-endargs"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>regress.js</files>
//       <!-- Variant running without tests using wabt for jshost -->
//       <compile-flags>-wasm -args --no-verbose --no-wabt -endargs</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn rot_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rot.js",
        baseline_path: Some("rot.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fastarray_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fastarray.js",
        compile_flags: vec!["-wasm", "-WasmFastArray-"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fastarray_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fastarray.js",
        compile_flags: vec!["-wasm", "-WasmFastArray"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn misc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "misc.js",
        baseline_path: Some("misc.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn controlflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "controlflow.js",
        baseline_path: Some("controlflow.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn f32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "f32.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn f64_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "f64.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn math_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "math.js",
        compile_flags: vec!["-wasm", "-wasmi64"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dropteelocal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dropteelocal.js",
        baseline_path: Some("dropteelocal.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn i32_popcnt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "i32_popcnt.js",
        baseline_path: Some("i32_popcnt.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn f32address_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "f32address.js",
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn div_by_constants_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn div_by_constants_unsigned_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divByConstants_unsigned.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn global_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "global.js",
        baseline_path: Some("baselines/global.baseline"),
        compile_flags: vec!["-wasm", "-wasmi64"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basic.js",
        baseline_path: Some("basic.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn basic_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basic.js",
        baseline_path: Some("basic.baseline"),
        compile_flags: vec!["-ForceStaticInterpreterThunk", "-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn table_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "table.js",
        baseline_path: Some("table.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn badfuncformat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "badfuncformat.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn table_imports_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "table_imports.js",
        baseline_path: Some("baselines/table_imports.baseline"),
        compile_flags: vec!["-wasm", "-wasmi64"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>table_signatures.js</files>
//       <tags>exclude_drt,exclude_win7</tags>
//       <compile-flags>-wasm -args --no-verbose -endargs</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn call_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "call.js",
        baseline_path: Some("baselines/call.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array.js",
        baseline_path: Some("array.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn trunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trunc.js",
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn api_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "api.js",
        baseline_path: Some("baselines/api.baseline"),
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalid_global_mut_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalid_global_mut.js",
        compile_flags: vec!["-wasm"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugs.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn params_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "params.js",
        baseline_path: Some("baselines/params.baseline"),
        compile_flags: vec![
            "-wasm",
            "-EnableFatalErrorOnOOM-",
            "-args",
            "14000",
            "-endargs",
        ],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_win7",
            "exclude_dynapogo",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>inlining.js</files>
//       <baseline>inlining.baseline</baseline>
//       <tags>exclude_win7</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn params_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "params.js",
        baseline_path: Some("baselines/params.baseline"),
        compile_flags: vec!["-wasm", "-args", "14000", "-endargs"],
        tags: HashSet::from(["exclude_win7", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

//   TODO (hanhossain): migrate

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn debugger_basic_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "debugger_basic.js",
        compile_flags: vec!["-wasm", "-dbgbaseline:debugger_basic.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_win7",
            "exclude_drt",
            "exclude_snap",
            "require_debugger",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

//   TODO (hanhossain): migrate

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn debugger_basic_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "debugger_basic.js",
        compile_flags: vec![
            "-wasm",
            "-maic:1",
            "-dbgbaseline:debugger_basic.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_win7",
            "exclude_drt",
            "exclude_snap",
            "require_debugger",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

//   TODO (hanhossain): migrate

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn debugger_basic_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "debugger_basic.js",
        compile_flags: vec![
            "-wasm",
            "-debuglaunch",
            "-args",
            "debuglaunch",
            "-endargs",
            "-dbgbaseline:debugger_basic_launch.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_win7",
            "exclude_drt",
            "exclude_snap",
            "require_debugger",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

//   TODO (hanhossain): migrate

// TODO (hanhossain): migrate
//     <test>
//       <default>
//         <files>wasmcctx.js</files>
//         <compile-flags>-wasm -dbgbaseline:wasmcctx.js.dbg.baseline -InspectMaxStringLength:50</compile-flags>
//         <!-- todo-xplat: Fix this! The test is flaky on XPLAT -->
//         <tags>exclude_win7,exclude_drt,exclude_snap,require_debugger,exclude_xplat</tags>
//       </default>
//     </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>oom_wasm.js</files>
//         <!-- These tests expect OOM, -EnableFatalErrorOnOOM- to disable fatal error for this test case. Bug will be filed to address this later -->
//       <compile-flags>-EnableFatalErrorOnOOM- -wasm -args 0 16384 -endargs</compile-flags>
//       <tags>exclude_x64</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn oom_wasm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "oom_wasm.js",
        compile_flags: vec![
            "-EnableFatalErrorOnOOM-",
            "-wasm",
            "-args",
            "3",
            "16381",
            "-endargs",
        ],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn oom_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "oom.js",
        compile_flags: vec![
            "-EnableFatalErrorOnOOM-",
            "-wasm",
            "-args",
            "0",
            "16384",
            "-endargs",
        ],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn oom_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "oom.js",
        compile_flags: vec![
            "-EnableFatalErrorOnOOM-",
            "-wasm",
            "-args",
            "3",
            "16381",
            "-endargs",
        ],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn response_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "response.js",
        baseline_path: Some("baselines/response.baseline"),
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn i64_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "i64.js",
        compile_flags: vec!["-wasm", "-args", "--no-verbose", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn i64cf_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "i64cf.js",
        compile_flags: vec!["-wasm", "-maic:0"],
        tags: HashSet::from(["exclude_x64"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn nestedblocks_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nestedblocks.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn cse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cse.js",
        baseline_path: Some("baselines/cse.baseline"),
        compile_flags: vec![
            "-wasm",
            "-maic:0",
            "-WasmAssignModuleID",
            "-testtrace:cse:2.0-99.999",
        ],
        tags: HashSet::from([
            "exclude_drt",
            "exclude_win7",
            "exclude_interpreted",
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
fn signextend_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signextend.js",
        compile_flags: vec!["-wasm", "-args", "--no-verbose", "-endargs"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn memory_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memory.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn memory_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memory.js",
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn superlongsignaturemismatch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "superlongsignaturemismatch.js",
        compile_flags: vec!["-wasm"],
        tags: HashSet::from(["exclude_drt", "exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn binary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "binary.js",
        compile_flags: vec!["-wasm", "-args", "--no-verbose", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn binary_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "binary.js",
        compile_flags: vec!["-wasm", "-args", "--no-verbose", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn polyinline_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "polyinline.js",
        compile_flags: vec!["-maxinterpretcount:2", "-off:simplejit"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>limits.js</files>
//       <compile-flags>-wasm -args --no-verbose --end 4 -endargs</compile-flags>
//       <timeout>300</timeout>
//       <tags>exclude_drt,exclude_win7,exclude_debug,exclude_dynapogo,exclude_x86,Slow</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>limits.js</files>
//       <compile-flags>-wasm -args --no-verbose --start 4 --end 12 -endargs</compile-flags>
//       <timeout>300</timeout>
//       <tags>exclude_drt,exclude_win7,exclude_debug,exclude_dynapogo,exclude_x86,Slow</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>limits.js</files>
//       <compile-flags>-wasm -args --no-verbose --start 12 -endargs</compile-flags>
//       <timeout>300</timeout>
//       <tags>exclude_drt,exclude_win7,exclude_debug,exclude_dynapogo,exclude_x86,Slow</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopstslot_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopstslot.js",
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopyield_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopyield.js",
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopyieldnested_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopyieldnested.js",
        compile_flags: vec!["-lic:10", "-bgjit-"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopyieldtypes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopyieldtypes.js",
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopyieldregress_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopyieldregress.js",
        compile_flags: vec!["-lic:0", "-bgjit-"],
        tags: HashSet::from(["exclude_win7"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn atomics_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "atomics_test.js",
        compile_flags: vec![
            "-wasmthreads",
            "-ESSharedArrayBuffer",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn reload_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "reload.js",
        compile_flags: vec!["-wasmthreads", "-ESSharedArrayBuffer"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
