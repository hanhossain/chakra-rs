use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 1] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/bailout";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arrayctor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrayctor.js",
        baseline_path: Some("arrayctor.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout.js",
        baseline_path: Some("bailout.baseline"),
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.bailout.js"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout.js",
        baseline_path: Some("bailout.baseline"),
        compile_flags: vec![
            "-off:typespec",
            "-bailout:5",
            "-dynamicprofileinput:profile.dpl.bailout.js",
        ],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout.js",
        baseline_path: Some("bailout.baseline"),
        compile_flags: vec!["-bailout:10", "-dynamicprofileinput:profile.dpl.bailout.js"],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout2.js",
        baseline_path: Some("bailout2.baseline"),
        compile_flags: vec!["-bailout:7"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout3.js",
        baseline_path: Some("bailout3.baseline"),
        compile_flags: vec!["-bailout:6", "-off:typespec"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout4.js",
        baseline_path: Some("bailout4.baseline"),
        compile_flags: vec!["-bailout:6", "-off:typespec"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout5.js",
        baseline_path: Some("bailout5.baseline"),
        compile_flags: vec!["-bailout:9", "-off:typespec"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout6.js",
        compile_flags: vec!["-bailoutateverybytecode"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout7_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout7.js",
        baseline_path: Some("bailout7.baseline"),
        compile_flags: vec!["-bailoutbytecode:742", "-off:deferparse"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bailout_loopbodystart_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout_loopbodystart.js",
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.bailout_loopbodystart.js"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_loopbodystart_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout_loopbodystart.js",
        compile_flags: vec![
            "-bailoutbytecode:17",
            "-dynamicprofileinput:profile.dpl.bailout_loopbodystart.js",
        ],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_loopbodystart_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout_loopbodystart.js",
        compile_flags: vec![
            "-bailoutbytecode:18",
            "-dynamicprofileinput:profile.dpl.bailout_loopbodystart.js",
        ],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_eh_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-eh.js",
        baseline_path: Some("bailout-eh.baseline"),
        compile_flags: vec!["-bailout:6"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_args_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-args.js",
        baseline_path: Some("bailout-args.baseline"),
        compile_flags: vec!["-bailoutbytecode:1"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_argobj_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-argobj.js",
        baseline_path: Some("bailout-argobj.baseline"),
        compile_flags: vec!["-bailout:3", "-bailout:11", "-forcejitloopbody"],
        // OS:12885336 this test fails without dynamicprofile cache/input args
        tags: HashSet::from(["require_backend", "exclude_forceserialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_throw_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-throw.js",
        baseline_path: Some("bailout-throw.baseline"),
        compile_flags: vec!["-bailout:5"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bailout_floatpref_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-floatpref.js",
        baseline_path: Some("bailout-floatpref.baseline"),
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.bailout-floatpref.js"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_floatpref_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-floatpref.js",
        baseline_path: Some("bailout-floatpref.baseline"),
        compile_flags: vec![
            "-bailout:3",
            "-dynamicprofileinput:profile.dpl.bailout-floatpref.js",
        ],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_floatpref_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-floatpref.js",
        baseline_path: Some("bailout-floatpref.baseline"),
        compile_flags: vec![
            "-bailoutateverybytecode",
            "-dynamicprofileinput:profile.dpl.bailout-floatpref.js",
        ],
        tags: HashSet::from(["require_backend", "exclude_interpreted", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_copy_prop1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-copyProp1.js",
        baseline_path: Some("bailout-copyProp1.baseline"),
        compile_flags: vec!["-bailout:6"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_strict_exception_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-strict-exception.js",
        baseline_path: Some("bailout-strict-exception.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn aggressive_int_type_spec_with_float_pref_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AggressiveIntTypeSpecWithFloatPref.js",
        baseline_path: Some("Void.baseline"),
        compile_flags: vec!["-on:aggressiveIntTypeSpec"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_inlined_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-inlined.js",
        baseline_path: Some(""),
        compile_flags: vec!["-force:inline"],
        tags: HashSet::from(["require_backend", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug10_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug10.js",
        baseline_path: Some("bug10.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn flags_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "flags.js",
        baseline_path: Some("flags.baseline"),
        compile_flags: vec!["-bailoutbytecode:312"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn initlocals_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "initlocals.js",
        compile_flags: vec!["-bailout:8"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn implicit_nosideeffect_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "implicit_nosideeffect.js",
        baseline_path: Some("implicit_nosideeffect.baseline"),
        compile_flags: vec!["-off:earlyreferenceerrors"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn inline_builtins_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineBuiltIns.js",
        baseline_path: Some("inlineBuiltIns.baseline"),
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.inlineBuiltIns.js"],
        tags: HashSet::from(["require_backend", "exclude_dynapogo", "exclude_serialized"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn inline_builtins_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineBuiltIns.js",
        baseline_path: Some("inlineBuiltIns.baseline"),
        compile_flags: vec![
            "-args",
            "dynapogo",
            "-endargs",
            "-dynamicprofileinput:profile.dpl.inlineBuiltIns.js",
        ],
        tags: HashSet::from([
            "require_backend",
            "exclude_interpreted",
            "exclude_serialized",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_branch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-branch.js",
        baseline_path: Some("bailout-branch.baseline"),
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bailout_checkthis_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailout-checkthis.js",
        baseline_path: Some(""),
        compile_flags: vec!["-force:checkthis"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inline_call_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inline_call_bailout.js",
        baseline_path: Some("inline_call_bailout.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inline_get_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inline_get_bailout.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spill_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spill.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug12782316_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug12782316.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug13674598_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug13674598.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug17449647_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug17449647.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
