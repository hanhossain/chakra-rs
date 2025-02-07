use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_backend</tags>

const DIRECTORY: &str = "chakracore-cxx/test/FlowGraph";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn mic1msjrc1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mic1msjrc1.js",
        compile_flags: vec![
            "-mic:1",
            "-msjrc:1",
            "-oopjit-",
            "-bgjit-",
            "-loopinterpretcount:1",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn weird1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weird1.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-oopjit-",
            "-off:bailonnoprofile",
            "-off:cachedScope",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn weird2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weird2.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-bgjit-",
            "-loopinterpretcount:1",
            "-oopjit-",
            "-off:simplejit",
            "-force:inline",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn for_of_try_catch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for_of_try_catch.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-bgjit-",
            "-oopjit-",
            "-off:jitloopbody",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn byte_code_upward_exposed_used_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "byteCodeUpwardExposedUsed.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-bgjit-",
            "-oopjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
