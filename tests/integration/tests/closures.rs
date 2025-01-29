use common::Variant;
use rstest::rstest;
use std::collections::HashSet;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Closures";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn cachedscope_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cachedscope_1.js",
        baseline_path: Some("cachedscope_1.baseline"),
        compile_flags: vec!["-force:cachedscope"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn cachedscope_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cachedscope_2.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure.js",
        baseline_path: Some("closure.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_callback_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-callback.js",
        baseline_path: Some("closure-callback.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_multiple_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_multiple_1.js",
        baseline_path: Some("closure_multiple_1.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_multiple_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_multiple_2.js",
        baseline_path: Some("closure_multiple_2.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_binding_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_binding.js",
        baseline_path: Some("closure_binding.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_binding_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_binding_2.js",
        baseline_path: Some("closure_binding_2.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_funcexpr_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-funcexpr-eval.js",
        baseline_path: Some("closure-funcexpr-eval-3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_qmark_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-qmark.js",
        baseline_path: Some("closure-qmark.baseline"),
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_ole_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_ole.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn closure_ole_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_ole.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(optimized))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn delaycapture_loopbody_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delaycapture-loopbody.js",
        compile_flags: vec![
            "-lic:1",
            "-off:simplejit",
            "-bgjit-",
            "-off:stackfunc",
            "-InitializeInterpreterSlotsWithInvalidStackVar",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(optimized))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn delaycapture_loopbody2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delaycapture-loopbody2.js",
        compile_flags: vec![
            "-lic:1",
            "-bgjit-",
            "-InitializeInterpreterSlotsWithInvalidStackVar",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(optimized))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn initcachedscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "initcachedscope.js",
        baseline_path: Some("initcachedscope.baseline"),
        compile_flags: vec!["-recyclerstress", "-force:cachedscope"],
        tags: HashSet::from(["exclude_test", "Slow"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn initcachedscope_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "initcachedscope.js",
        baseline_path: Some("initcachedscope.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalcachedscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope.js",
        baseline_path: Some("invalcachedscope.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalcachedscope_js_serialized(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope.js",
        baseline_path: Some("invalcachedscope.baseline"),
        compile_flags: vec!["-Serialized"],
        tags: HashSet::from(["exclude_forceserialized"]),
    };
    common::run_test_variant(&test, variant);
}

#[cfg(not(optimized))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalcachedscope_js_deferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope.js",
        baseline_path: Some("invalcachedscope.baseline"),
        compile_flags: vec!["-force:deferparse", "-Intl-"],
        tags: HashSet::from(["exclude_test"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn invalcachedscope_caller_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope-caller.js",
        baseline_path: Some("invalcachedscope-caller.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_2299723_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2299723.js",
        baseline_path: Some("bug_OS_2299723.baseline"),
        compile_flags: vec!["-force:cachedscope"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_2671095_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2671095.js",
        compile_flags: vec!["-forcedeferparse", "-force:cachedscope"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_2903083_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2903083.js",
        compile_flags: vec!["-force:cachedscope"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_9781249_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_9781249.js",
        compile_flags: vec!["-forcejitloopbody", "-force:inline", "-force:rejit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_9008744_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_9008744.js",
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_10735999_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_10735999.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_13412380_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_13412380.js",
        tags: HashSet::from(["BugFix", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn copy_prop_stack_slot_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "copy-prop-stack-slot.js",
        baseline_path: Some("copy-prop-stack-slot.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn update_funcexpr_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "update-funcexpr.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
