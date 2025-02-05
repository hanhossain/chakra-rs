use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_backend</tags>

const DIRECTORY: &str = "chakracore-cxx/test/PRE";
// TODO (hanhossain): migrate
//    <test>
//     <default>
//       <files>pre1.js</files>
//       <baseline>pre1.baseline</baseline>
//       <tags>exclude_forceserialized,exclude_dynapogo</tags>
//       <compile-flags>-testtrace:fieldcopyprop -oopjit-</compile-flags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug0.js",
        compile_flags: vec![todo!("-args summary -endargs")],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
