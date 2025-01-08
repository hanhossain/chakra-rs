use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/RegAlloc";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn reg_alloc_bug_0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "RegAllocBug_0.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
