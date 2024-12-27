mod common;
const DIRECTORY: &str = "chakracore-cxx/test/loop";

#[test]
fn bail_out_of_mem_op_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailOutOfMemOp.js",
        baseline_path: Some("bailOutOfMemOp.baseline"),
        compile_flags: vec!["-bgjit-", "-mic:1", "-off:simplejit", "-minmemopcount:0"],
    };
    common::run_test(&test);
}

#[test]
fn loop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loop.js",
        baseline_path: Some("loop.baseline"),
        compile_flags: vec!["-forcejitloopbody", "-off:earlyreferenceerrors"],
    };
    common::run_test(&test);
}

#[test]
fn loop_js_interrupt_probe() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loop.js",
        baseline_path: Some("loop.baseline"),
        compile_flags: vec![
            "-forcejitloopbody",
            "-on:interruptprobe",
            "-off:earlyreferenceerrors",
        ],
    };
    common::run_test(&test);
}

#[test]
fn loopinversion_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        compile_flags: vec!["-force:deferparse"],
    };
    common::run_test(&test);
}

#[test]
fn loopinversion_js_interrupt_probe() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        compile_flags: vec!["-on:interruptprobe"],
    };
    common::run_test(&test);
}

#[test]
fn loopinversion_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn infinite_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "infinite.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mem_op_js_simple_jit_off() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOp.js",
        baseline_path: Some("MemOp.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:1", "-bgjit-"],
    };
    common::run_test(&test);
}

#[test]
fn mem_op_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOp.js",
        baseline_path: Some("MemOp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mem_op_incr0_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOpIncr0.js",
        ..Default::default()
    };
    common::run_test(&test);
}
