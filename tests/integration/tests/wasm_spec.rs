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
const DIRECTORY: &str = "chakracore-cxx/test/WasmSpec";
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spec.js",
        baseline_path: Some("baselines/chakra/chakra_atomic_load.baseline"),
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra/chakra_atomic_load.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra/chakra_atomic_load.wast",
            "-endargs",
            "-nonative",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
            "-args",
            "chakra/chakra_atomic_load.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra/chakra_atomic_store.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra/chakra_atomic_store.wast",
            "-endargs",
            "-nonative",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
            "-args",
            "chakra/chakra_atomic_store.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_extends_i32.wast",
            "-endargs",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_extends_i32.wast",
            "-endargs",
            "-nonative",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_extends_i64.wast",
            "-endargs",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_extends_i64.wast",
            "-endargs",
            "-nonative",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i32.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i32.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i32.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i64.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i64.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "chakra_generated/chakra_i64.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/address.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/address.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/address.wast",
            "-endargs",
            "-wasmfastarray-",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/align.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/align.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/binary.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/binary.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/block.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/block.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/br.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/br.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/br_if.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/br_if.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/br_table.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/br_table.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/break-drop.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/break-drop.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/call.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/call.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/call_indirect.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/call_indirect.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/const.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/const.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/conversions.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/conversions.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/custom.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/custom.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/data.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/data.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/elem.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/elem.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/endianness.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/endianness.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/exports.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/exports.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/f32.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f32.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f32_bitwise.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f32_bitwise.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/f32_cmp.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f32_cmp.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/f64.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f64.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f64_bitwise.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f64_bitwise.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/f64_cmp.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/f64_cmp.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/fac.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/fac.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_exprs.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_exprs.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_literals.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_literals.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_memory.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_memory.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_memory.wast",
            "-endargs",
            "-wasmfastarray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_misc.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/float_misc.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/forward.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/forward.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/func.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/func.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/func_ptrs.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/func_ptrs.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/get_local.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/get_local.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/globals.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/globals.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/i32.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/i32.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/i32.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/i64.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/i64.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/i64.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/if.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/if.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/imports.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/imports.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/inline-module.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/inline-module.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/int_exprs.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/int_exprs.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/int_exprs.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/int_literals.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/int_literals.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/labels.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/labels.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/left-to-right.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/left-to-right.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/linking.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/linking.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/load.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/load.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/loop.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/loop.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/memory.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory.wast",
            "-endargs",
            "-wasmfastarray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_grow.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_grow.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_redundancy.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_redundancy.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_trap.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_trap.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/memory_trap.wast",
            "-endargs",
            "-wasmfastarray-",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/nop.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/nop.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/return.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/return.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/select.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/select.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/set_local.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/set_local.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/skip-stack-guard-page.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/skip-stack-guard-page.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/stack.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/stack.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/start.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/start.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/store.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/store.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/switch.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/switch.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/tee_local.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/tee_local.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/token.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/token.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/traps.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/traps.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/traps.wast",
            "-endargs",
            "-wasmfastarray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/traps.wast",
            "-endargs",
            "-wasmMathExFilter",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite", "/core/type.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/type.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/typecheck.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/typecheck.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/unreachable.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/unreachable.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/unreached-invalid.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/unreached-invalid.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec!["-wasm", "-args", "testsuite/core/unwind.wast", "-endargs"],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/unwind.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-custom-section-id.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-custom-section-id.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-import-field.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-import-field.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-import-module.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-import-module.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-invalid-encoding.wast",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/core/utf8-invalid-encoding.wast",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/interface.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/interface.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/compile.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/compile.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/instantiate-bad-imports.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/instantiate-bad-imports.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/instantiate.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/instantiate.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/validate.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/constructor/validate.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/constructor.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/constructor.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/toString.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/toString.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/value-get-set.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/value-get-set.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/valueOf.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/global/valueOf.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/constructor-bad-imports.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/constructor-bad-imports.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/constructor.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/constructor.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/exports.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/exports.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/toString.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/instance/toString.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/buffer.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/buffer.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/constructor.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/constructor.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/grow.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/grow.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/toString.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/memory/toString.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/constructor.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/constructor.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/customSections.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/customSections.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/exports.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/exports.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/imports.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/imports.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/toString.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/module/toString.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/constructor.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/constructor.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/get-set.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/get-set.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/grow.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/grow.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/length.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/length.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/toString.any.js",
            "-endargs",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "testsuite/js-api/table/toString.any.js",
            "-endargs",
            "-nonative",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/extends/extends_i32.wast",
            "-endargs",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/extends/extends_i32.wast",
            "-endargs",
            "-nonative",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/extends/extends_i64.wast",
            "-endargs",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/extends/extends_i64.wast",
            "-endargs",
            "-nonative",
            "-WasmSignExtends",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/nontrapping/nontrapping_conversions.wast",
            "-endargs",
            "-WasmNontrapping",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/nontrapping/nontrapping_conversions.wast",
            "-endargs",
            "-nonative",
            "-WasmNontrapping",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/threads/atomic_load.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/threads/atomic_load.wast",
            "-endargs",
            "-nonative",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
            "-args",
            "features/threads/atomic_load.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/threads/atomic_store.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
        compile_flags: vec![
            "-wasm",
            "-args",
            "features/threads/atomic_store.wast",
            "-endargs",
            "-nonative",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
        ],
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
            "-args",
            "features/threads/atomic_store.wast",
            "-endargs",
            "-WasmThreads",
            "-ESSharedArrayBuffer",
            "-WasmSharedArrayVirtualBuffer-",
            "-WasmFastArray-",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
