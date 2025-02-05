use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_wasm</tags>

const DIRECTORY: &str = "chakracore-cxx/test/WasmSpec.MultiValue";
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn simple_block_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_block.js",
        compile_flags: vec!["-wasm", "-WasmMultiValue"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blocktypes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blocktypes.js",
        compile_flags: vec!["-wasm", "-WasmMultiValue"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
