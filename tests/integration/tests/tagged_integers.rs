use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/TaggedIntegers";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn rem_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "remBailout.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn comparison_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comparison.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn addition_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "addition.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn subtraction_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "subtraction.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn div_min_int_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "div_min_int.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn multiplication_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiplication.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn divide_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divide.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn and_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "and.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn or_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "or.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn xor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "xor.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn not_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn negate_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negate.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn signedshiftleft_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftleft.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn signedshiftright_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftright.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn unsignedshiftright_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsignedshiftright.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn modulus_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "modulus.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopbounds_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopbounds.js",
        baseline_path: Some("loopbounds.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn not_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not_1.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn shift_constants_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shift_constants.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn loops_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loops.js",
        baseline_path: Some("loops.baseline"),
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn predecrement_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "predecrement.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn preincrement_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "preincrement.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn comparison_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comparison.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn addition_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "addition.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn subtraction_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "subtraction.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn multiplication_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiplication.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn divide_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divide.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn and_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "and.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn or_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "or.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn xor_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "xor.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn not_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn negate_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negate.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn signedshiftleft_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftleft.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn signedshiftright_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftright.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn unsignedshiftright_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsignedshiftright.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn modulus_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "modulus.js",
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn loopbounds_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopbounds.js",
        baseline_path: Some("loopbounds.baseline"),
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(disable_jit))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn arrays_js_dynamicprofilecache(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrays.js",
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.arrays.js"],
        tags: HashSet::from(["exclude_dynapogo", "exclude_serialized", "require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(disable_jit))]
#[rstest]
#[case::dynapogo(Variant::Dynapogo)]
fn arrays_js_dynamicprofilinput(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrays.js",
        compile_flags: vec![
            "-dynamicprofilecache:profile.dpl.arrays.js",
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        tags: HashSet::from([
            "exclude_interpreted",
            "exclude_serialized",
            "require_backend",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn not_1_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not_1.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn shift_constants_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shift_constants.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn loops_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loops.js",
        baseline_path: Some("loops.baseline"),
        tags: HashSet::from(["exclude_test", "Slow"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn predecrement_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "predecrement.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn preincrement_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "preincrement.js",
        tags: HashSet::from(["exclude_test"]),
        compile_flags: vec![
            "-off:constprop",
            "-off:copyprop",
            "-off:constfold",
            "-off:typespec",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}
