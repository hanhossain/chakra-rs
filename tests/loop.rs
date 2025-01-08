use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/loop";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn bail_out_of_mem_op_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bailOutOfMemOp.js",
        baseline_path: Some("bailOutOfMemOp.baseline"),
        compile_flags: vec!["-bgjit-", "-mic:1", "-off:simplejit", "-minmemopcount:0"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loop.js",
        baseline_path: Some("loop.baseline"),
        compile_flags: vec!["-forcejitloopbody", "-off:earlyreferenceerrors"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loop_js_interrupt_probe(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loop.js",
        baseline_path: Some("loop.baseline"),
        compile_flags: vec![
            "-forcejitloopbody",
            "-on:interruptprobe",
            "-off:earlyreferenceerrors",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loopinversion_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loopinversion_js_interrupt_probe(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        compile_flags: vec!["-on:interruptprobe"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loopinversion_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinversion.js",
        baseline_path: Some("loopinversion.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn infinite_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "infinite.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn mem_op_js_simple_jit_off(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOp.js",
        baseline_path: Some("MemOp.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn mem_op_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOp.js",
        baseline_path: Some("MemOp.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn mem_op_incr0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MemOpIncr0.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
