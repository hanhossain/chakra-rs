use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_backend</tags>

const DIRECTORY: &str = "chakracore-cxx/test/FlowGraph";
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn mic1msjrc1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mic1msjrc1.js",
        compile_flags: vec![
            "-mic:1",
            todo!("-msjrc:1 -oopjit- -bgjit- -loopinterpretcount:1"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn weird1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weird1.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            todo!("-maxsimplejitruncount:1 -oopjit- -off:bailonnoprofile -off:cachedScope"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn weird2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weird2.js",
        compile_flags: vec!["-maxinterpretcount:1",todo!("-maxsimplejitruncount:1 -bgjit- -loopinterpretcount:1 -oopjit- -off:simplejit -force:inline")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_of_try_catch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for_of_try_catch.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            todo!("-maxsimplejitruncount:1 -bgjit- -oopjit- -off:jitloopbody"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn byteCodeUpwardExposedUsed_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "byteCodeUpwardExposedUsed.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            todo!("-maxsimplejitruncount:1 -bgjit- -oopjit-"),
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
