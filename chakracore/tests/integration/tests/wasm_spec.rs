use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 4] = [
    "exclude_serialized",
    "require_backend",
    "exclude_win7",
    "require_wasm",
];
const DIRECTORY: &str = "../chakracore-cxx/test/WasmSpec";
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec!["-wasm", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["chakra/chakra_atomic_load.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["chakra/chakra_atomic_load.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec![
            "-wasm",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
        host_args: vec!["chakra/chakra_atomic_load.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec!["-wasm", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["chakra/chakra_atomic_store.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["chakra/chakra_atomic_store.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_store.baseline"),
        compile_flags: vec![
            "-wasm",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
        host_args: vec!["chakra/chakra_atomic_store.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i32.baseline"),
        compile_flags: vec!["-wasm", "-WasmSignExtends"],
        host_args: vec!["chakra_generated/chakra_extends_i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js8(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i32.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmSignExtends"],
        host_args: vec!["chakra_generated/chakra_extends_i32.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js9(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i64.baseline"),
        compile_flags: vec!["-wasm", "-WasmSignExtends"],
        host_args: vec!["chakra_generated/chakra_extends_i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js10(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_extends_i64.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmSignExtends"],
        host_args: vec!["chakra_generated/chakra_extends_i64.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js11(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["chakra_generated/chakra_i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js12(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["chakra_generated/chakra_i32.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js13(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i32.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["chakra_generated/chakra_i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js14(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["chakra_generated/chakra_i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js15(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["chakra_generated/chakra_i64.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js16(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra_generated/chakra_i64.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["chakra_generated/chakra_i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js17(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/address.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js18(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/address.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js19(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/address.baseline"),
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        host_args: vec!["testsuite/core/address.wast"],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js20(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/align.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/align.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js21(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/align.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/align.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js22(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/binary.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/binary.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js23(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/binary.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/binary.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js24(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/block.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/block.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js25(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/block.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/block.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js26(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/br.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js27(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/br.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js28(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_if.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/br_if.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js29(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_if.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/br_if.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js30(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_table.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/br_table.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js31(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/br_table.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/br_table.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js32(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/break-drop.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/break-drop.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js33(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/break-drop.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/break-drop.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js34(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/call.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js35(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/call.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js36(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call_indirect.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/call_indirect.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js37(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/call_indirect.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/call_indirect.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js38(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/const.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/const.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js39(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/const.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/const.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js40(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/conversions.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/conversions.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js41(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/conversions.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/conversions.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js42(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/custom.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/custom.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js43(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/custom.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/custom.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js44(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/data.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/data.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js45(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/data.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/data.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js46(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/elem.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/elem.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js47(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/elem.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/elem.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js48(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/endianness.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/endianness.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js49(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/endianness.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/endianness.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js50(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/exports.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/exports.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js51(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/exports.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/exports.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js52(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js53(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f32.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js54(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_bitwise.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f32_bitwise.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js55(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_bitwise.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f32_bitwise.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js56(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_cmp.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f32_cmp.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js57(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f32_cmp.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f32_cmp.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js58(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js59(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f64.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js60(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_bitwise.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f64_bitwise.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js61(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_bitwise.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f64_bitwise.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js62(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_cmp.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/f64_cmp.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js63(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/f64_cmp.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/f64_cmp.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js64(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/fac.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/fac.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js65(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/fac.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/fac.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js66(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_exprs.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/float_exprs.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js67(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_exprs.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/float_exprs.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js68(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_literals.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/float_literals.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js69(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_literals.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/float_literals.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js70(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/float_memory.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js71(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/float_memory.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js72(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_memory.baseline"),
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        host_args: vec!["testsuite/core/float_memory.wast"],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js73(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_misc.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/float_misc.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js74(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/float_misc.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/float_misc.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js75(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/forward.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/forward.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js76(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/forward.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/forward.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js77(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/func.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js78(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/func.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js79(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func_ptrs.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/func_ptrs.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js80(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/func_ptrs.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/func_ptrs.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js81(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/get_local.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/get_local.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js82(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/get_local.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/get_local.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js83(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/globals.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/globals.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js84(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/globals.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/globals.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js85(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js86(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/i32.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js87(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i32.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["testsuite/core/i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js88(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js89(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/i64.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js90(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/i64.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["testsuite/core/i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js91(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/if.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/if.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js92(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/if.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/if.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js93(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/imports.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/imports.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js94(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/imports.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/imports.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js95(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/inline-module.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/inline-module.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js96(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/inline-module.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/inline-module.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js97(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/int_exprs.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js98(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/int_exprs.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js99(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_exprs.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["testsuite/core/int_exprs.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js100(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_literals.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/int_literals.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js101(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/int_literals.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/int_literals.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js102(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/labels.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/labels.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js103(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/labels.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/labels.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js104(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/left-to-right.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/left-to-right.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js105(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/left-to-right.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/left-to-right.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js106(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/linking.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/linking.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js107(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/linking.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/linking.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js108(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/load.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/load.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js109(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/load.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/load.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js110(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/loop.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/loop.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js111(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/loop.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/loop.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js112(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/memory.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js113(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/memory.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js114(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory.baseline"),
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        host_args: vec!["testsuite/core/memory.wast"],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js115(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_grow.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/memory_grow.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js116(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_grow.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/memory_grow.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js117(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_redundancy.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/memory_redundancy.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js118(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_redundancy.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/memory_redundancy.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js119(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/memory_trap.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js120(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/memory_trap.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js121(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/memory_trap.baseline"),
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        host_args: vec!["testsuite/core/memory_trap.wast"],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js122(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/nop.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/nop.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js123(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/nop.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/nop.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js124(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/return.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/return.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js125(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/return.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/return.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js126(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/select.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/select.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js127(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/select.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/select.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js128(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/set_local.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/set_local.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js129(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/set_local.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/set_local.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js130(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/skip-stack-guard-page.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/skip-stack-guard-page.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js131(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/skip-stack-guard-page.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/skip-stack-guard-page.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js132(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/stack.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/stack.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js133(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/stack.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/stack.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js134(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/start.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/start.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js135(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/start.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/start.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js136(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/store.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/store.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js137(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/store.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/store.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js138(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/switch.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/switch.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js139(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/switch.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/switch.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js140(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/tee_local.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/tee_local.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js141(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/tee_local.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/tee_local.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js142(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/token.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/token.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js143(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/token.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/token.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js144(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/traps.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js145(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/traps.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js146(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec!["-wasm", "-wasmfastarray-"],
        host_args: vec!["testsuite/core/traps.wast"],
        tags: HashSet::from(["exclude_x86"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js147(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/traps.baseline"),
        compile_flags: vec!["-wasm", "-wasmMathExFilter"],
        host_args: vec!["testsuite/core/traps.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js148(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/type.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/type.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js149(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/type.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/type.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js150(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/typecheck.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/typecheck.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js151(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/typecheck.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/typecheck.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js152(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreachable.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/unreachable.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js153(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreachable.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/unreachable.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js154(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreached-invalid.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/unreached-invalid.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js155(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unreached-invalid.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/unreached-invalid.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js156(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unwind.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/unwind.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js157(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/unwind.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/unwind.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js158(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-custom-section-id.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/utf8-custom-section-id.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js159(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-custom-section-id.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/utf8-custom-section-id.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js160(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-field.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/utf8-import-field.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js161(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-field.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/utf8-import-field.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js162(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-module.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/utf8-import-module.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js163(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-import-module.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/utf8-import-module.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js164(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-invalid-encoding.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/core/utf8-invalid-encoding.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js165(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/testsuite/core/utf8-invalid-encoding.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/core/utf8-invalid-encoding.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/interface.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/interface.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/interface.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/interface.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/compile.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/constructor/compile.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/compile.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/constructor/compile.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/constructor/instantiate-bad-imports.any.baseline",
        ),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/constructor/instantiate-bad-imports.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/constructor/instantiate-bad-imports.any.baseline",
        ),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/constructor/instantiate-bad-imports.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/instantiate.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/constructor/instantiate.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js8(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/instantiate.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/constructor/instantiate.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js9(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/validate.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/constructor/validate.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js10(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/constructor/validate.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/constructor/validate.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js11(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/constructor.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/global/constructor.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js12(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/constructor.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/global/constructor.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js13(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/toString.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/global/toString.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js14(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/toString.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/global/toString.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js15(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/value-get-set.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/global/value-get-set.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js16(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/value-get-set.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/global/value-get-set.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js17(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/valueOf.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/global/valueOf.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js18(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/global/valueOf.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/global/valueOf.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js19(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/instance/constructor-bad-imports.any.baseline",
        ),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/instance/constructor-bad-imports.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js20(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some(
            "baselines/testsuite/js-api/instance/constructor-bad-imports.any.baseline",
        ),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/instance/constructor-bad-imports.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js21(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/constructor.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/instance/constructor.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js22(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/constructor.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/instance/constructor.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js23(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/exports.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/instance/exports.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js24(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/exports.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/instance/exports.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js25(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/toString.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/instance/toString.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js26(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/instance/toString.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/instance/toString.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js27(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/buffer.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/memory/buffer.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js28(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/buffer.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/memory/buffer.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js29(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/constructor.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/memory/constructor.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js30(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/constructor.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/memory/constructor.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js31(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/grow.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/memory/grow.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js32(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/grow.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/memory/grow.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js33(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/toString.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/memory/toString.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js34(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/memory/toString.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/memory/toString.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js35(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/constructor.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/module/constructor.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js36(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/constructor.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/module/constructor.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js37(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/customSections.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/module/customSections.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js38(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/customSections.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/module/customSections.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js39(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/exports.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/module/exports.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js40(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/exports.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/module/exports.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js41(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/imports.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/module/imports.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js42(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/imports.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/module/imports.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js43(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/toString.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/module/toString.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js44(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/module/toString.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/module/toString.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js45(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/constructor.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/table/constructor.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js46(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/constructor.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/table/constructor.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js47(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/get-set.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/table/get-set.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js48(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/get-set.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/table/get-set.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js49(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/grow.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/table/grow.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js50(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/grow.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/table/grow.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js51(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/length.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/table/length.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js52(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/length.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/table/length.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn jsapi_js53(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/toString.any.baseline"),
        compile_flags: vec!["-wasm"],
        host_args: vec!["testsuite/js-api/table/toString.any.js"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jsapi_js54(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jsapi.js",
        baseline_path: Some("baselines/testsuite/js-api/table/toString.any.baseline"),
        compile_flags: vec!["-wasm", "-nonative"],
        host_args: vec!["testsuite/js-api/table/toString.any.js"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js166(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i32.baseline"),
        compile_flags: vec!["-wasm", "-WasmSignExtends"],
        host_args: vec!["features/extends/extends_i32.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js167(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i32.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmSignExtends"],
        host_args: vec!["features/extends/extends_i32.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js168(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i64.baseline"),
        compile_flags: vec!["-wasm", "-WasmSignExtends"],
        host_args: vec!["features/extends/extends_i64.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js169(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/extends/extends_i64.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmSignExtends"],
        host_args: vec!["features/extends/extends_i64.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js170(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/nontrapping/nontrapping_conversions.baseline"),
        compile_flags: vec!["-wasm", "-WasmNontrapping"],
        host_args: vec!["features/nontrapping/nontrapping_conversions.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js171(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/nontrapping/nontrapping_conversions.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmNontrapping"],
        host_args: vec!["features/nontrapping/nontrapping_conversions.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js172(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec!["-wasm", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["features/threads/atomic_load.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js173(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["features/threads/atomic_load.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js174(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_load.baseline"),
        compile_flags: vec![
            "-wasm",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
        host_args: vec!["features/threads/atomic_load.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js175(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec!["-wasm", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["features/threads/atomic_store.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn spec_js176(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec!["-wasm", "-nonative", "-WasmThreads", "-ESSharedArrayBuffer"],
        host_args: vec!["features/threads/atomic_store.wast"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js177(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/features/threads/atomic_store.baseline"),
        compile_flags: vec![
            "-wasm",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
        host_args: vec!["features/threads/atomic_store.wast"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
