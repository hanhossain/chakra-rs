mod common;
const DIRECTORY: &str = "chakracore-cxx/test/EH";

#[test]
fn capture_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "capture.js",
        baseline_path: Some("capture.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn capture_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "capture.js",
        baseline_path: Some("capture.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn oos2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "oos2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try1.js",
        baseline_path: Some("try1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try2.js",
        baseline_path: Some("try2-es5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try3.js",
        baseline_path: Some("try3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try4.js",
        baseline_path: Some("try4.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try5_es3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try5-ES3.js",
        baseline_path: Some("try5-ES3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try6_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try6.js",
        baseline_path: Some("try6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try_bug188541_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try.bug188541.js",
        baseline_path: Some("try.bug188541.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try_bug188541_v5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "try.bug188541.v5.js",
        baseline_path: Some("try.bug188541.v5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn so_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "so.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newso_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newso.js",
        baseline_path: Some("newso.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trylabel_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trylabel.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn alignment_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "alignment.js",
        baseline_path: Some(""),
        compile_flags: vec!["-off:inline", "-checkalignment"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test101832_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "101832.js",
        baseline_path: Some("101832.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn early1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "early1.js",
        baseline_path: Some("early1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn early2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "early2.js",
        baseline_path: Some("early2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallybug0_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallybug0.js",
        baseline_path: Some("tryfinallybug0.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallytests_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallytests.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyldelembug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallyldelembug.js",
        compile_flags: vec!["-off:arraycheckhoist"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallybug1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallybug1.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfinlinebug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tfinlinebug.js",
        compile_flags: vec!["-force:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inlinestackwalkbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlinestackwalkbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nestedinlinestackwalkbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nestedinlinestackwalkbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyinlinebug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallyinlinebug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncintrystackwalkbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "asyncintrystackwalkbug.js",
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ehinlinearmbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ehinlinearmbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn helperlabelbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "helperlabelbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn helperlabelbug2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "helperlabelbug2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyinlineswbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tryfinallyinlineswbug.js",
        compile_flags: vec!["-force:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasBailedOutBug.js",
        baseline_path: Some("hasBailedOutBug.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasBailedOutBug2.js",
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasBailedOutBug3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasBailedOutBug4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stack_overflow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackOverflow.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfjitlooparmbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tfjitlooparmbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfjitloopbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tfjitloopbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn optional_catch_binding_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "optional-catch-binding.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn region_bug_spec_hoisting_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regionBugSpecHoisting.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}
