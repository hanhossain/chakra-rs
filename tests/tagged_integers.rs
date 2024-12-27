mod common;
const DIRECTORY: &str = "chakracore-cxx/test/TaggedIntegers";

#[test]
fn rem_bailout_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "remBailout.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn comparison_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "comparison.js",
        ..Default::default()
    };
    common::run_test(&test);
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

#[test]
fn div_min_int_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "div_min_int.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>multiplication.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn divide_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "divide.js",
        ..Default::default()
    };
    common::run_test(&test);
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

#[test]
fn not_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn negate_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negate.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn signedshiftleft_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftleft.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn signedshiftright_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "signedshiftright.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unsignedshiftright_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unsignedshiftright.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>modulus.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn loopbounds_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopbounds.js",
        baseline_path: Some("loopbounds.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn not_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "not_1.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn shift_constants_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shift_constants.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loops.js</files>
//     <baseline>loops.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn predecrement_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "predecrement.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn preincrement_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "preincrement.js",
        ..Default::default()
    };
    common::run_test(&test);
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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>arrays.js</files>
//     <tags>exclude_dynapogo,exclude_serialized,require_backend</tags>
//     <compile-flags>-dynamicprofilecache:profile.dpl.arrays.js</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>arrays.js</files>
//     <tags>exclude_interpreted,exclude_serialized,require_backend</tags>
//     <compile-flags>-dynamicprofileinput:profile.dpl.arrays.js -off:constprop -off:copyprop -off:constfold -off:typespec</compile-flags>
//   </default>
// </test>

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
