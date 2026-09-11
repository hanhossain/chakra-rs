use crate::common;
use crate::common::Variant;
use chakracore_sys::config::HostConfig;
use rstest::rstest;

const DIRECTORY: &str = "../chakracore-cxx/test/262";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test262(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "262test.js",
        compile_flags: vec!["-ESSharedArrayBuffer"],
        host_config: HostConfig {
            test262: true,
            ..Default::default()
        },
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}
