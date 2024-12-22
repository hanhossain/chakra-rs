mod common;

#[test]
fn accessors() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Accessors.js",
        baseline_path: "chakracore-cxx/test/Basics/Accessors.baseline",
    };
    common::run_test(&test);
}
