use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/262";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test262(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "262test.js",
        compile_flags: vec!["-ESSharedArrayBuffer", "-Test262"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
