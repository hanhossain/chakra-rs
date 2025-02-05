use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,exclude_win7,require_wasm</tags>

const DIRECTORY: &str = "chakracore-cxx/test/WasmSpec";
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!(
                "-args chakra/chakra_atomic_load.wast -endargs -WasmThreads -ESSharedArrayBuffer"
            ),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra/chakra_atomic_load.wast -endargs -nonative -WasmThreads -ESSharedArrayBuffer")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra/chakra_atomic_load.wast -endargs -WasmThreads -ESSharedArrayBuffer -WasmSharedArrayVirtualBuffer- -WasmFastArray-")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!(
                "-args chakra/chakra_atomic_store.wast -endargs -WasmThreads -ESSharedArrayBuffer"
            ),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra/chakra_atomic_store.wast -endargs -nonative -WasmThreads -ESSharedArrayBuffer")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra/chakra_atomic_store.wast -endargs -WasmThreads -ESSharedArrayBuffer -WasmSharedArrayVirtualBuffer- -WasmFastArray-")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_extends_i32.wast -endargs -WasmSignExtends"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i32.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra_generated/chakra_extends_i32.wast -endargs -nonative -WasmSignExtends")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_extends_i64.wast -endargs -WasmSignExtends"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i64.baseline"),
        compile_flags: vec!["-wasm",todo!("-args chakra_generated/chakra_extends_i64.wast -endargs -nonative -WasmSignExtends")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i32.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i32.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i32.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i64.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i64.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args chakra_generated/chakra_i64.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/address.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/address.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/address.wast -endargs -wasmfastarray-"),
        ],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/align.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/align.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/align.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/align.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/binary.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/binary.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/binary.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/binary.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/block.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/block.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/block.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/block.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/br.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/br.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_if.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/br_if.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_if.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/br_if.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_table.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/br_table.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_table.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/br_table.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/break-drop.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/break-drop.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/break-drop.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/break-drop.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/call.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/call.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call_indirect.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/call_indirect.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call_indirect.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/call_indirect.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/const.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/const.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/const.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/const.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/conversions.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/conversions.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/conversions.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/conversions.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/custom.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/custom.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/custom.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/custom.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/data.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/data.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/data.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/data.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/elem.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/elem.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/elem.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/elem.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/endianness.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/endianness.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/endianness.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/endianness.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/exports.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/exports.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/exports.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/exports.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/f32.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f32.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_bitwise.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f32_bitwise.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_bitwise.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f32_bitwise.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_cmp.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/f32_cmp.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_cmp.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f32_cmp.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/f64.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f64.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_bitwise.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f64_bitwise.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_bitwise.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f64_bitwise.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_cmp.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/f64_cmp.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_cmp.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/f64_cmp.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/fac.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/fac.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/fac.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/fac.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_exprs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_exprs.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_exprs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_exprs.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_literals.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_literals.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_literals.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_literals.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_memory.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_memory.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_memory.wast -endargs -wasmfastarray-"),
        ],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_misc.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_misc.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_misc.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/float_misc.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/forward.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/forward.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/forward.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/forward.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/func.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/func.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func_ptrs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/func_ptrs.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func_ptrs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/func_ptrs.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/get_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/get_local.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/get_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/get_local.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/globals.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/globals.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/globals.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/globals.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/i32.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/i32.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/i32.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/i64.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/i64.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/i64.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/if.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/if.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/if.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/if.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/imports.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/imports.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/imports.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/imports.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/inline-module.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/inline-module.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/inline-module.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/inline-module.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/int_exprs.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/int_exprs.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/int_exprs.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_literals.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/int_literals.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_literals.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/int_literals.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/labels.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/labels.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/labels.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/labels.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/left-to-right.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/left-to-right.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/left-to-right.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/left-to-right.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/linking.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/linking.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/linking.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/linking.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/load.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/load.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/load.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/load.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/loop.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/loop.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/loop.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/loop.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/memory.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory.wast -endargs -wasmfastarray-"),
        ],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_grow.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_grow.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_grow.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_grow.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_redundancy.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_redundancy.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_redundancy.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_redundancy.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_trap.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_trap.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/memory_trap.wast -endargs -wasmfastarray-"),
        ],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/nop.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/nop.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/nop.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/nop.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/return.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/return.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/return.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/return.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/select.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/select.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/select.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/select.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/set_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/set_local.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/set_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/set_local.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/skip-stack-guard-page.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/skip-stack-guard-page.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/skip-stack-guard-page.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/skip-stack-guard-page.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/stack.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/stack.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/stack.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/stack.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/start.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/start.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/start.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/start.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/store.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/store.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/store.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/store.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/switch.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/switch.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/switch.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/switch.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/tee_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/tee_local.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/tee_local.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/tee_local.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/token.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/token.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/token.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/token.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/traps.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/traps.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/traps.wast -endargs -wasmfastarray-"),
        ],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/traps.wast -endargs -wasmMathExFilter"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/type.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/type.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/type.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/type.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/typecheck.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/typecheck.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/typecheck.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/typecheck.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreachable.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/unreachable.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreachable.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/unreachable.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreached-invalid.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/unreached-invalid.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreached-invalid.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/unreached-invalid.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unwind.baseline"),
        compile_flags: vec!["-wasm", todo!("-args testsuite/core/unwind.wast -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unwind.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/unwind.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-custom-section-id.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-custom-section-id.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-custom-section-id.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-custom-section-id.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-field.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-import-field.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-field.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-import-field.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-module.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-import-module.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-module.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-import-module.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-invalid-encoding.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-invalid-encoding.wast -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-invalid-encoding.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/core/utf8-invalid-encoding.wast -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/interface.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/interface.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/interface.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/interface.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/compile.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/compile.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/compile.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/compile.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/constructor/instantiate-bad-imports.any.baseline",
        ),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/instantiate-bad-imports.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/instantiate-bad-imports.any.baseline"),
        compile_flags: vec!["-wasm",todo!("-args testsuite/js-api/constructor/instantiate-bad-imports.any.js -endargs -nonative")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/instantiate.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/instantiate.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/instantiate.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/instantiate.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/validate.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/validate.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/validate.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/constructor/validate.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/constructor.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/constructor.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/toString.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/toString.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/value-get-set.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/value-get-set.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/value-get-set.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/value-get-set.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/valueOf.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/valueOf.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/valueOf.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/global/valueOf.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/instance/constructor-bad-imports.any.baseline",
        ),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/constructor-bad-imports.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/instance/constructor-bad-imports.any.baseline",
        ),
        compile_flags: vec![
            "-wasm",
            todo!(
                "-args testsuite/js-api/instance/constructor-bad-imports.any.js -endargs -nonative"
            ),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/constructor.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/constructor.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/exports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/exports.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/exports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/exports.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/toString.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/instance/toString.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/buffer.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/buffer.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/buffer.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/buffer.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/constructor.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/constructor.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/grow.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/grow.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/grow.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/grow.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/toString.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/memory/toString.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/constructor.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/constructor.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/customSections.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/customSections.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/customSections.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/customSections.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/exports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/exports.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/exports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/exports.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/imports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/imports.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/imports.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/imports.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/toString.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/module/toString.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/constructor.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/constructor.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/constructor.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/get-set.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/get-set.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/get-set.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/get-set.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/grow.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/grow.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/grow.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/grow.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/length.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/length.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/length.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/length.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/toString.any.js -endargs"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/toString.any.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args testsuite/js-api/table/toString.any.js -endargs -nonative"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args features/extends/extends_i32.wast -endargs -WasmSignExtends"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i32.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args features/extends/extends_i32.wast -endargs -nonative -WasmSignExtends"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args features/extends/extends_i64.wast -endargs -WasmSignExtends"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i64.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!("-args features/extends/extends_i64.wast -endargs -nonative -WasmSignExtends"),
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/nontrapping/nontrapping_conversions.baseline"),
        compile_flags: vec![
            "-wasm",
            todo!(
                "-args features/nontrapping/nontrapping_conversions.wast -endargs -WasmNontrapping"
            ),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/nontrapping/nontrapping_conversions.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/nontrapping/nontrapping_conversions.wast -endargs -nonative -WasmNontrapping")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_load.wast -endargs -WasmThreads -ESSharedArrayBuffer")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_load.wast -endargs -nonative -WasmThreads -ESSharedArrayBuffer")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_load.wast -endargs -WasmThreads -ESSharedArrayBuffer -WasmSharedArrayVirtualBuffer- -WasmFastArray-")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_store.wast -endargs -WasmThreads -ESSharedArrayBuffer")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_store.wast -endargs -nonative -WasmThreads -ESSharedArrayBuffer")],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec!["-wasm",todo!("-args features/threads/atomic_store.wast -endargs -WasmThreads -ESSharedArrayBuffer -WasmSharedArrayVirtualBuffer- -WasmFastArray-")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
