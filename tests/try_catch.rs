mod common;
const DIRECTORY: &str = "chakracore-cxx/test/TryCatch";

#[test]
fn try_catch_stack_overflow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TryCatchStackOverflow.js",
        ..Default::default()
    };
    common::run_test(&test);
}
