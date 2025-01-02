use common::Variant;
use rstest::rstest;

mod common;
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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>addition.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>subtraction.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>multiplication.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>and.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>or.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>xor.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>modulus.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loops.js</files>
//     <baseline>loops.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

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
    common::run_test_variant(&test, variant);
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
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>comparison.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>addition.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>subtraction.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>multiplication.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>divide.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>and.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>or.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>xor.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>not.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>negate.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>signedshiftleft.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>signedshiftright.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unsignedshiftright.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>modulus.js</files>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loopbounds.js</files>
//     <baseline>loopbounds.baseline</baseline>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn arrays_js_dynamicprofilecache(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrays.js",
        compile_flags: vec!["-dynamicprofilecache:profile.dpl.arrays.js"],
        tags: vec!["exclude_dynapogo", "exclude_serialized", "require_backend"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

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
        tags: vec![
            "exclude_interpreted",
            "exclude_serialized",
            "require_backend",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>not_1.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>shift_constants.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loops.js</files>
//     <baseline>loops.baseline</baseline>
//     <tags>exclude_test,Slow</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>predecrement.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>preincrement.js</files>
//     <tags>exclude_test</tags>
//     <compile-flags>-off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>
