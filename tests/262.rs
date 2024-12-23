mod common;

#[test]
fn test262() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/262/262test.js",
        compile_flags: vec!["-ESSharedArrayBuffer", "-Test262"],
        ..Default::default()
    };
    common::run_test(&test);
}
