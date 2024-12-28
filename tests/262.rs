mod common;
const DIRECTORY: &str = "chakracore-cxx/test/262";

#[test]
fn test262() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "262test.js",
        compile_flags: vec!["-ESSharedArrayBuffer", "-Test262"],
        ..Default::default()
    };
    common::run_test(&test);
}
