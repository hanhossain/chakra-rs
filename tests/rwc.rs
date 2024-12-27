mod common;
const DIRECTORY: &str = "chakracore-cxx/test/RWC";

#[test]
fn onenote_ribbon_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OneNote.ribbon.js",
        baseline_path: Some("OneNote.ribbon.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
