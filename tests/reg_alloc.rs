mod common;
const DIRECTORY: &str = "chakracore-cxx/test/RegAlloc";

#[test]
fn reg_alloc_bug_0_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "RegAllocBug_0.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}
