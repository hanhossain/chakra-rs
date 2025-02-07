use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/Optimizer";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug318_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug318.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug41530_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug41530.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug42111_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug42111.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug70_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug70.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug713_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug713.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug1788761_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1788761.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug1868543_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1868543.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn isarrbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isarrbug.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug_divby_const_optim_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug-divbyConstOptim.js",
        compile_flags: vec!["-maxsimplejitruncount:2", "-maxinterpretcount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug469_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug469.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-bgjit-",
            "-loopinterpretcount:1",
            "-force:fixdataprops",
            "-off:aggressiveinttypespec",
            "-off:bailonnoprofile",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug3831075_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug3831075.js",
        compile_flags: vec!["-loopinterpretcount:1", "-off:bailonnoprofile"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug5579910_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug5579910.js",
        compile_flags: vec!["-bgjit-", "-mic:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn conv_bool_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "conv_bool.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cm_bail_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CmBail.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cm_peeps_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CmPeeps.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cse1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cse1.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cse2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cse2.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn clz_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "clz.js",
        baseline_path: Some("clz.baseline"),
        compile_flags: vec!["-lic:1", "-mic:1", "-msjrc:1", "-bgjit-", "-sse:2"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cse3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cse3.js",
        baseline_path: Some("cse3.baseline"),
        compile_flags: vec!["-loopinterpretcount:0"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn null_type_spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NullTypeSpec.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn optpeep_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "optpeep.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn shru_peep_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shru_peep.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn shru_intrange_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shru_intrange.js",
        baseline_path: Some("shru_intrange.baseline"),
        compile_flags: vec!["-lic:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test0.js",
        baseline_path: Some("test0.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test1.js",
        baseline_path: Some("test1.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test10_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test10.js",
        baseline_path: Some("test10.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test11_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test11.js",
        baseline_path: Some("test11.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test12_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test12.js",
        baseline_path: Some("test12.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test13_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test13.js",
        baseline_path: Some("test13.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test14_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test14.js",
        baseline_path: Some("test14.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test15_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test15.js",
        baseline_path: Some("test15.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test16_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test16.js",
        baseline_path: Some("test16.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test17_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test17.js",
        baseline_path: Some("test17.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test18_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test18.js",
        baseline_path: Some("test18.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test19_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test19.js",
        baseline_path: Some("test19.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test2.js",
        baseline_path: Some("test2.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test20_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test20.js",
        baseline_path: Some("test20.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test21_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test21.js",
        baseline_path: Some("test21.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test22_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test22.js",
        baseline_path: Some("test22.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test23_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test23.js",
        baseline_path: Some("test23.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test24_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test24.js",
        baseline_path: Some("test24.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test25_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test25.js",
        baseline_path: Some("test25.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test26_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test26.js",
        baseline_path: Some("test26.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test27_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test27.js",
        baseline_path: Some("test27.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test28_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test28.js",
        baseline_path: Some("test28.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test29_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test29.js",
        baseline_path: Some("test29.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test3.js",
        baseline_path: Some("test3.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test30_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test30.js",
        baseline_path: Some("test30.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test31_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test31.js",
        baseline_path: Some("test31.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test32.js",
        baseline_path: Some("test32.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test33_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test33.js",
        baseline_path: Some("test33.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test34_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test34.js",
        baseline_path: Some("test34.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test35_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test35.js",
        baseline_path: Some("test35.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test36_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test36.js",
        baseline_path: Some("test36.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test37_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test37.js",
        baseline_path: Some("test37.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test38_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test38.js",
        baseline_path: Some("test38.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test39_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test39.js",
        baseline_path: Some("test39.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test4.js",
        baseline_path: Some("test4.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test40_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test40.js",
        baseline_path: Some("test40.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test41_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test41.js",
        baseline_path: Some("test41.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test42_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test42.js",
        baseline_path: Some("test42.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test43_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test43.js",
        baseline_path: Some("test43.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test44_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test44.js",
        baseline_path: Some("test44.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test45_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test45.js",
        baseline_path: Some("test45.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test46_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test46.js",
        baseline_path: Some("test46.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test47_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test47.js",
        baseline_path: Some("test47.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test48_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test48.js",
        baseline_path: Some("test48.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test49_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test49.js",
        baseline_path: Some("test49.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test5.js",
        baseline_path: Some("test5.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test50_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test50.js",
        baseline_path: Some("test50.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test51_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test51.js",
        baseline_path: Some("test51.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test52_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test52.js",
        baseline_path: Some("test52.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test53_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test53.js",
        baseline_path: Some("test53.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test54_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test54.js",
        baseline_path: Some("test54.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test55_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test55.js",
        baseline_path: Some("test55.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test56_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test56.js",
        baseline_path: Some("test56.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test57_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test57.js",
        baseline_path: Some("test57.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test58_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test58.js",
        baseline_path: Some("test58.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test59_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test59.js",
        baseline_path: Some("test59.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test6.js",
        baseline_path: Some("test6.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test60_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test60.js",
        baseline_path: Some("test60.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test61_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test61.js",
        baseline_path: Some("test61.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test62_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test62.js",
        baseline_path: Some("test62.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test63_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test63.js",
        baseline_path: Some("test63.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test64_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test64.js",
        baseline_path: Some("test64.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test65_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test65.js",
        baseline_path: Some("test65.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test66_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test66.js",
        baseline_path: Some("test66.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test67_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test67.js",
        baseline_path: Some("test67.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test68_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test68.js",
        baseline_path: Some("test68.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test69_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test69.js",
        baseline_path: Some("test69.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test7_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test7.js",
        baseline_path: Some("test7.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test70_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test70.js",
        baseline_path: Some("test70.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test71_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test71.js",
        baseline_path: Some("test71.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test72_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test72.js",
        baseline_path: Some("test72.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test73_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test73.js",
        baseline_path: Some("test73.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test74_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test74.js",
        baseline_path: Some("test74.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test75_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test75.js",
        baseline_path: Some("test75.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test76_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test76.js",
        baseline_path: Some("test76.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test77_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test77.js",
        baseline_path: Some("test77.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test78_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test78.js",
        baseline_path: Some("test78.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test79_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test79.js",
        baseline_path: Some("test79.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test8_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test8.js",
        baseline_path: Some("test8.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test80_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test80.js",
        baseline_path: Some("test80.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test81_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test81.js",
        baseline_path: Some("test81.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test82_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test82.js",
        baseline_path: Some("test82.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test83_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test83.js",
        baseline_path: Some("test83.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test84_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test84.js",
        baseline_path: Some("test84.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test85_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test85.js",
        baseline_path: Some("test85.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test86_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test86.js",
        baseline_path: Some("test86.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test87_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test87.js",
        baseline_path: Some("test87.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test88_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test88.js",
        baseline_path: Some("test88.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test89_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test89.js",
        baseline_path: Some("test89.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test9_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test9.js",
        baseline_path: Some("test9.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test90_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test90.js",
        baseline_path: Some("test90.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test91_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test91.js",
        baseline_path: Some("test91.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test92_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test92.js",
        baseline_path: Some("test92.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test93_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test93.js",
        baseline_path: Some("test93.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test94_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test94.js",
        baseline_path: Some("test94.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test95_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test95.js",
        baseline_path: Some("test95.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test96_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test96.js",
        baseline_path: Some("test96.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test97_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test97.js",
        baseline_path: Some("test97.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test98_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test98.js",
        baseline_path: Some("test98.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test99_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test99.js",
        baseline_path: Some("test99.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test100_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test100.js",
        baseline_path: Some("test100.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test101_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test101.js",
        baseline_path: Some("test101.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test102_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test102.js",
        compile_flags: vec![
            "-minInterpretCount:4",
            "-maxInterpretCount:4",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test103_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test103.js",
        baseline_path: Some("test103.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:objTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test104_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test104.js",
        baseline_path: Some("test104.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-force:jitLoopBody",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test105_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test105.js",
        baseline_path: Some("test105.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:aggressiveIntTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test106_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test106.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test107_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test107.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:lossyIntTypeSpec",
            "-force:jitLoopBody",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test108_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test108.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test109_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test109.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:aggressiveIntTypeSpec",
            "-off:lossyIntTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test110_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test110.js",
        compile_flags: vec![
            "-bgJit-",
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-loopInterpretCount:0",
            "-off:aggressiveIntTypeSpec",
            "-off:lossyIntTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test111_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test111.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-force:rejit",
            "-force:deferparse",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test112_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test112.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:typedArrayTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test113_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test113.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:arrayMissingValueCheckHoist",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test115_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test115.js",
        compile_flags: vec!["-args", "bailout", "-endArgs"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test116_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test116.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test117_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test117.js",
        baseline_path: Some("test117.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:eliminateArrayAccessHelperCall",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test118_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test118.js",
        baseline_path: Some("test118.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test119_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test119.js",
        baseline_path: Some("test119.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test120_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test120.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test121_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test121.js",
        compile_flags: vec!["-bgJit-", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test122_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test122.js",
        compile_flags: vec!["-bgJit-", "-maxInterpretCount:2", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test123_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test123.js",
        baseline_path: Some("test123.baseline"),
        compile_flags: vec!["-bgJit-", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test124_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test124.js",
        compile_flags: vec![
            "-bgJit-",
            "-loopInterpretCount:1",
            "-maxInterpretCount:2",
            "-maxSimpleJitRunCount:0",
            "-forceSerialized",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test125_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test125.js",
        compile_flags: vec!["-forceNative", "-off:fullJit"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test126_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test126.js",
        compile_flags: vec!["-bgJit-", "-fullJitAfter:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test127_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test127.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test128_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test128.js",
        baseline_path: Some("test128.baseline"),
        compile_flags: vec!["-noNative"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test129_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test129.js",
        compile_flags: vec!["-bgJit-", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test130_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test130.js",
        compile_flags: vec!["-bgJit-", "-fullJitAfter:1", "-force:deferParse"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test131_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test131.js",
        compile_flags: vec!["-bgJit-", "-fullJitAfter:1", "-off:fieldCopyProp"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test132_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test132.js",
        compile_flags: vec![
            "-bgJit-",
            "-fullJitAfter:1",
            "-off:eliminateArrayAccessHelperCall",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test133_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test133.js",
        baseline_path: Some("test133.baseline"),
        compile_flags: vec!["-bgJit-", "-fullJitAfter:1", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test134_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test134.js",
        baseline_path: Some("test134.baseline"),
        compile_flags: vec!["-bgJit-", "-fullJitAfter:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test135_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test135.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test136_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test136.js",
        baseline_path: Some("test136.baseline"),
        compile_flags: vec![
            "-bgJit-",
            "-off:simpleJit",
            "-loopInterpretCount:1",
            "-off:nativeArray",
            "-off:jsArraySegmentHoist",
            "-off:arrayMissingValueCheckHoist",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test137_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test137.js",
        compile_flags: vec![
            "-bgJit-",
            "-simpleJitAfter:1",
            "-fullJitAfter:3",
            "-loopInterpretCount:1",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test138_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test138.js",
        compile_flags: vec!["-bgJit-", "-bgJitDelay:0", "-loopInterpretCount:50"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test138_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test138.js",
        compile_flags: vec!["-bgJit-", "-bgJitDelay:0"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test139_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test139.js",
        baseline_path: Some("test139.baseline"),
        compile_flags: vec!["-bgJit-", "-off:simpleJit", "-fullJitAfter:2"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test140_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test140.js",
        compile_flags: vec!["-bgJit-", "-off:simpleJit", "-fullJitAfter:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test141_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test141.js",
        compile_flags: vec![
            "-bgJit-",
            "-off:simpleJit",
            "-fullJitAfter:2",
            "-on:simulatePolyCacheWithOneTypeForFunction:1",
            "-simulatePolyCacheWithOneTypeForInlineCacheIndex:3",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test142_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test142.js",
        compile_flags: vec!["-bgJit-", "-off:simpleJit", "-fullJitAfter:2"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test143_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test143.js",
        baseline_path: Some("test143.baseline"),
        compile_flags: vec![
            "-bgJit-",
            "-off:simpleJit",
            "-loopInterpretCount:1",
            "-testTrace:arrayCheckHoist",
        ],
        // ch.exe doesn't output entire baseline before exiting; -testTrace flush issue?
        tags: HashSet::from([
            "require_backend",
            "exclude_dynapogo",
            "exclude_forceserialized",
            "exclude_nonative",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test144_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test144.js",
        compile_flags: vec![
            "-bgJit-",
            "-off:simpleJit",
            "-fullJitAfter:1",
            "-off:arrayCheckHoist",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test145_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test145.js",
        compile_flags: vec![
            "-bgJit-",
            "-off:simpleJit",
            "-fullJitAfter:1",
            "-force:deferParse",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn deadstore_field_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deadstore_field.js",
        baseline_path: Some("deadstore_field.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn deadstore_oneblockloop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deadstore_oneblockloop.js",
        baseline_path: Some("deadstore_oneblockloop.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn marktemp_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marktemp.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn marktemp2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marktemp2.js",
        baseline_path: Some("marktemp2.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn marktempnumberontempobjects_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "marktempnumberontempobjects.js",
        baseline_path: Some("marktempnumberontempobjects.baseline"),
        compile_flags: vec!["-fja:2", "-off:simplejit"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn mul_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mul.js",
        baseline_path: Some(""),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn negative_zero_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NegativeZero.js",
        baseline_path: Some("NegativeZero.baseline"),
        tags: HashSet::from(["require_backend", "Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn overflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Overflow.js",
        baseline_path: Some("Overflow.baseline"),
        tags: HashSet::from(["require_backend", "Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn overflow_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Overflow_MaxInterpret.js",
        baseline_path: Some("Overflow_MaxInterpret.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn invariants_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Invariants.js",
        baseline_path: Some("Invariants.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lossy_lossless_int32_js_off_aggressive_int_type_spec(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LossyLosslessInt32.js",
        baseline_path: Some("LossyLosslessInt32.baseline"),
        compile_flags: vec!["-off:aggressiveIntTypeSpec"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lossy_lossless_int32_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LossyLosslessInt32.js",
        baseline_path: Some("LossyLosslessInt32.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lossy_lossless_int32_js_off_simplejit(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LossyLosslessInt32.js",
        baseline_path: Some("LossyLosslessInt32.baseline"),
        compile_flags: vec!["-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn aggressive_int_type_spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AggressiveIntTypeSpec.js",
        baseline_path: Some("AggressiveIntTypeSpec.baseline"),
        compile_flags: vec!["-off:inline"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn aggressive_int_type_spec_off_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AggressiveIntTypeSpec_Off_MaxInterpret.js",
        baseline_path: Some("AggressiveIntTypeSpec_Off_MaxInterpret.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:aggressiveIntTypeSpec",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn float_type_spec_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FloatTypeSpec_MaxInterpret.js",
        baseline_path: Some("FloatTypeSpec_MaxInterpret.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:copyProp",
            "-off:inline",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn native_array_max_interpret_off_array_missing_value_check_hoist_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NativeArray_MaxInterpret_OffArrayMissingValueCheckHoist.js",
        baseline_path: Some("NativeArray_MaxInterpret_OffArrayMissingValueCheckHoist.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:arrayMissingValueCheckHoist",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn type_spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypeSpec.js",
        baseline_path: Some("TypeSpec.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inline_actual_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inline-actual.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn copyprop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "copyprop.js",
        baseline_path: Some("copyprop.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn copyprop_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "copyprop.js",
        baseline_path: Some("copyprop.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn dead_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dead.js",
        baseline_path: Some("dead.baseline"),
        compile_flags: vec!["-force:inlinecandidate"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn unreachable_code_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UnreachableCode.js",
        baseline_path: Some("UnreachableCode.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn pre_pass_values_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PrePassValues.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn pre_pass_entanglement_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PrePassEntanglement.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn missing_len_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "missing_len.js",
        baseline_path: Some("missing_len.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn array_check_hoist_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayCheckHoist.js",
        baseline_path: Some("ArrayCheckHoist_NoBailout.baseline"),
        compile_flags: vec![
            "-noNative",
            "-dynamicprofilecache:profile.dpl.ArrayCheckHoist.js",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_dynapogo",
            "exclude_serialized",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn array_check_hoist_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayCheckHoist.js",
        baseline_path: Some("ArrayCheckHoist_Bailout.baseline"),
        compile_flags: vec![
            "-off:arrayCheckHoist",
            "-args",
            "bailout",
            "-endArgs",
            "-dynamicprofileinput:profile.dpl.ArrayCheckHoist.js",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_interpreted",
            "exclude_serialized",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn array_check_hoist_bailout_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayCheckHoist.js",
        baseline_path: Some("ArrayCheckHoist_Bailout.baseline"),
        compile_flags: vec![
            "-args",
            "bailout",
            "-endArgs",
            "-off:fieldCopyProp",
            "-off:objTypeSpec",
            "-dynamicprofileinput:profile.dpl.ArrayCheckHoist.js",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_interpreted",
            "exclude_serialized",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
#[ignore]
#[timeout(std::time::Duration::from_secs(300))]
fn array_check_hoist_bailout_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArrayCheckHoist.js",
        baseline_path: Some("ArrayCheckHoist_Bailout.baseline"),
        compile_flags: vec![
            "-args",
            "bailout",
            "-endArgs",
            "-dynamicprofileinput:profile.dpl.ArrayCheckHoist.js",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_interpreted",
            "exclude_serialized",
            "Slow",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bound_check_elimination_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BoundCheckElimination.js",
        baseline_path: Some("BoundCheckElimination.baseline"),
        compile_flags: vec![
            "-bgJit-",
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-off:bailOnNoProfile",
            "-testTrace:boundCheckElimination",
            "-testTrace:boundCheckHoist",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_nonative",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bound_check_hoist_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boundCheckHoistBug.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn miscellaneous_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Miscellaneous_MaxInterpret.js",
        baseline_path: Some("Miscellaneous_MaxInterpret.baseline"),
        compile_flags: vec![
            "-bgJit-",
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn miscellaneous_loop_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Miscellaneous_LoopInterpret.js",
        baseline_path: Some("Miscellaneous_LoopInterpret.baseline"),
        compile_flags: vec!["-bgJit-", "-loopInterpretCount:1"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn miscellaneous_force_jit_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Miscellaneous_ForceJitLoopBody.js",
        compile_flags: vec!["-bgJit-", "-force:jitLoopBody"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn negative_zero_pow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NegativeZeroPow.js",
        baseline_path: Some("NegativeZeroPow.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn strength_reduction_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StrengthReduction.js",
        baseline_path: Some("StrengthReduction.baseline"),
        compile_flags: vec![
            "-force:MulStrengthReduction",
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-bgjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn directglofield_dictionarytypehandler_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "directglofield_dictionarytypehandler.js",
        baseline_path: Some("directglofield_dictionarytypehandler.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn int_div_type_spec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntDivTypeSpec.js",
        baseline_path: Some("IntDivTypeSpec.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bailonnoprofile_objtypespecstore_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailonnoprofile_objtypespecstore.js",
        baseline_path: Some(""),
        compile_flags: vec![
            "-recyclerverify:run",
            "-off:simplejit",
            "-maxinterpretcount:2",
        ],
        tags: HashSet::from(["require_backend", "exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn non32bit_overflow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Non32bitOverflow.js",
        baseline_path: Some("Non32bitOverflow.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn implicit_upwardexposed_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "implicit_upwardexposed.js",
        baseline_path: Some("implicit_upwardexposed.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug1288834_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1288834.js",
        baseline_path: Some("bug1288834.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit", "-off:peeps"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn opttagchecks1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "opttagchecks1.js",
        baseline_path: Some("opttagchecks1.baseline"),
        compile_flags: vec!["-loopinterpretcount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn opttagchecks2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "opttagchecks2.js",
        compile_flags: vec!["-off:bailonnoprofile"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn trycatch_functional_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trycatch_functional.js",
        baseline_path: Some("trycatch_functional.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn trycatch_assert_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trycatch_assert.js",
        baseline_path: Some("trycatch_assert.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn to_var_i32_x64_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ToVarI32_x64.js",
        compile_flags: vec![
            "-force:rejit",
            "-off:ArrayCheckHoist",
            "-off:aggressiveinttypespec",
            "-off:bailonnoprofile",
            "-off:nativearray",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn forced_type_spec_on_loop_header_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forcedTypeSpecOnLoopHeader.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn hasown_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasown.js",
        baseline_path: Some("hasown.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nonequivpoly_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nonequivpoly.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn propstrbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propstrbug.js",
        compile_flags: vec!["-lic:1", "-bgjit-", "-msjrc:2", "-mic:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn memop_upperbound_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memop-upperbound.js",
        baseline_path: Some("memop-upperbound.baseline"),
        compile_flags: vec!["-lic:1", "-off:nativearray"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn force_rejit_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forceRejitBugs.js",
        baseline_path: Some("forceRejitBugs.baseline"),
        compile_flags: vec!["-mic:1", "-off:simplejit", "-force:rejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn negative_zero_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negativeZero_bugs.js",
        baseline_path: Some("negativeZero_bugs.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn shladdpeep_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shladdpeep.js",
        baseline_path: Some("shladdpeep.baseline"),
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fix_type_after_hoisting_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixTypeAfterHoisting.js",
        compile_flags: vec![
            "-lic:1",
            "-off:simplejit",
            "-off:aggressiveinttypespec",
            "-bgjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn hoist_string_concat_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HoistStringConcat.js",
        compile_flags: vec!["-lic:1", "-off:simplejit", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn hoist_check_obj_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HoistCheckObjType.js",
        baseline_path: Some("HoistCheckObjType.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn invalid_ivrange_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalidIVRangeBug.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug14661401_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug14661401.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fgpeepbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fgpeepbug.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn captured_values_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "capturedValuesBugs.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test146_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test146.js",
        compile_flags: vec!["-off:bailonnoprofile", "-off:earlyreferenceerrors"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test147_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test147.js",
        compile_flags: vec!["-off:aggressiveinttypespec"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test148_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test148.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test149_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test149.js",
        compile_flags: vec![
            "-mic:1",
            "-off:simplejit",
            "-bgjit-",
            "-force:fixdataprops",
            "-lic:1",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test150_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test150.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test151_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test151.js",
        compile_flags: vec!["-off:usefixeddataprops", "-off:objtypespec"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test152_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test152.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn is_in_array_no_missing_values_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IsIn_ArrayNoMissingValues.js",
        baseline_path: Some("IsIn_ArrayNoMissingValues.baseline"),
        compile_flags: vec!["-testtrace:BoundCheckElimination"],
        tags: HashSet::from([
            "require_backend",
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_nonative",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn noedgebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "noedgebug.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-oopjit-",
            "-off:bailonnoprofile",
            "-loopinterpretcount:0",
            "-OOPJITMissingOpts-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn mul_rejit_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mul_rejit_bug.js",
        baseline_path: Some("mul_rejit_bug.baseline"),
        compile_flags: vec![
            "-mic:1",
            "-oopjit-",
            "-bgJit-",
            "-off:simplejit",
            "-trace:rejit",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_serialized",
        ]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn testsimplepathbrfold_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "testsimplepathbrfold.js",
        baseline_path: Some("testsimplepathbrfold.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-oopjit-",
            "-trace:pathdepbranchfolding",
            "-bgjit-",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bugsimplepathbrfold_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfold.js",
        baseline_path: Some("bugsimplepathbrfold.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-oopjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bugsimplepathbrfold1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfold1.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-oopjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bugsimplepathbrfold3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfold3.js",
        compile_flags: vec![
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-forcejitloopbody",
            "-oopjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bugsimplepathbrfold4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfold4.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-oopjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bugsimplepathbrfold5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfold5.js",
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bugsimplepathbrfoldgetter_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugsimplepathbrfoldgetter.js",
        baseline_path: Some("bugsimplepathbrfoldgetter.baseline"),
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bugconstfoldundefined_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugconstfoldundefined.js",
        baseline_path: Some("bugconstfoldundefined.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:6",
            "-maxsimplejitruncount:4",
            "-werexceptionsupport",
            "-oopjit-",
            "-mic:1",
            "-off:simplejit",
            "-forcejitloopbody",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bcebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bcebug.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-bgjit-", "-lic:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn rembug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rembug.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-loopinterpretcount:1",
            "-oopjit-",
            "-bgjit-",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn pathdepbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "pathdepbug.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-on:CaptureByteCodeRegUse",
            "-off:aggressiveinttypespec",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn lengthsymstorebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lengthsymstorebug.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn valuetypegap_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valuetypegap.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-force:inline",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn aux_slot_type_transition_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aux_slot_type_transition.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argrestoreintry_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argrestoreintry.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn aux_slot_type_transition_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "aux_slot_type_transition_2.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug_gh6325_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_gh6325.js",
        compile_flags: vec!["-off:backend", "-forcejitloopbody"],
        tags: HashSet::from(["require_backend", "exclude_nonative"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn stack_arguments_opt_negative_index_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackArgumentsOptNegativeIndex.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, COMMON_TAGS);
}
