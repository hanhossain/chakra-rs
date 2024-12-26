mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Boolean";

#[test]
fn basics_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "basics.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equality.js",
        compile_flags: vec!["-maxsimplejitruncount:4", "-maxinterpretcount:3"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn boolprop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boolprop.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:2",
            "-maxinterpretcount:1",
            "-off:simplejit",
            "-off:lossyinttypespec",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}
