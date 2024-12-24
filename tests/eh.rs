mod common;

#[test]
fn capture_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/capture.js",
        baseline_path: Some("chakracore-cxx/test/EH/capture.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn capture_js_defer_parse() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/capture.js",
        baseline_path: Some("chakracore-cxx/test/EH/capture.baseline"),
        compile_flags: vec!["-force:deferparse"],
    };
    common::run_test(&test);
}

#[test]
fn oos2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/oos2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try1.js",
        baseline_path: Some("chakracore-cxx/test/EH/try1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try2.js",
        baseline_path: Some("chakracore-cxx/test/EH/try2-es5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try3.js",
        baseline_path: Some("chakracore-cxx/test/EH/try3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try4.js",
        baseline_path: Some("chakracore-cxx/test/EH/try4.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try5_es3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try5-ES3.js",
        baseline_path: Some("chakracore-cxx/test/EH/try5-ES3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try6_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try6.js",
        baseline_path: Some("chakracore-cxx/test/EH/try6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try_bug188541_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try.bug188541.js",
        baseline_path: Some("chakracore-cxx/test/EH/try.bug188541.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try_bug188541_v5_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/try.bug188541.v5.js",
        baseline_path: Some("chakracore-cxx/test/EH/try.bug188541.v5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn so_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/so.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newso_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/newso.js",
        baseline_path: Some("chakracore-cxx/test/EH/newso.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trylabel_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/trylabel.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate, but it needs the baseline for some reason
// <test>
//   <default>
//     <files>alignment.js</files>
//     <compile-flags>-off:inline -checkalignment</compile-flags>
//     <baseline />
//   </default>
// </test>

#[test]
fn test101832_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/101832.js",
        baseline_path: Some("chakracore-cxx/test/EH/101832.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn early1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/early1.js",
        baseline_path: Some("chakracore-cxx/test/EH/early1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn early2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/early2.js",
        baseline_path: Some("chakracore-cxx/test/EH/early2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallybug0_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallybug0.js",
        baseline_path: Some("chakracore-cxx/test/EH/tryfinallybug0.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallytests_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallytests.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyldelembug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallyldelembug.js",
        compile_flags: vec!["-off:arraycheckhoist"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallybug1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallybug1.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfinlinebug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tfinlinebug.js",
        compile_flags: vec!["-force:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inlinestackwalkbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/inlinestackwalkbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nestedinlinestackwalkbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/nestedinlinestackwalkbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyinlinebug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallyinlinebug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn asyncintrystackwalkbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/asyncintrystackwalkbug.js",
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ehinlinearmbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/ehinlinearmbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn helperlabelbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/helperlabelbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn helperlabelbug2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/helperlabelbug2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tryfinallyinlineswbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tryfinallyinlineswbug.js",
        compile_flags: vec!["-force:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/hasBailedOutBug.js",
        baseline_path: Some("chakracore-cxx/test/EH/hasBailedOutBug.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>hasBailedOutBug2.js</files>
//       <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn has_bailed_out_bug3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/hasBailedOutBug3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_bailed_out_bug4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/hasBailedOutBug4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stack_overflow_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/StackOverflow.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfjitlooparmbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tfjitlooparmbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tfjitloopbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/tfjitloopbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn optional_catch_binding_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/optional-catch-binding.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn region_bug_spec_hoisting_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/EH/regionBugSpecHoisting.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}
