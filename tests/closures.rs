mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Closures";

#[test]
fn cachedscope_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cachedscope_1.js",
        baseline_path: Some("cachedscope_1.baseline"),
        compile_flags: vec!["-force:cachedscope"],
    };
    common::run_test(&test);
}

#[test]
fn cachedscope_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cachedscope_2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn closure_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure.js",
        baseline_path: Some("closure.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn closure_callback_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-callback.js",
        baseline_path: Some("closure-callback.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn closure_multiple_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_multiple_1.js",
        baseline_path: Some("closure_multiple_1.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn closure_multiple_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_multiple_2.js",
        baseline_path: Some("closure_multiple_2.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn closure_binding_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_binding.js",
        baseline_path: Some("closure_binding.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn closure_binding_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_binding_2.js",
        baseline_path: Some("closure_binding_2.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn closure_funcexpr_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-funcexpr-eval.js",
        baseline_path: Some("closure-funcexpr-eval-3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn closure_qmark_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-qmark.js",
        baseline_path: Some("closure-qmark.baseline"),
        compile_flags: vec!["-Intl-"],
    };
    common::run_test(&test);
}

#[test]
fn closure_ole_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_ole.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn closure_ole_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure_ole.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>delaycapture-loopbody.js</files>
//     <compile-flags>-lic:1 -off:simplejit -bgjit- -off:stackfunc -InitializeInterpreterSlotsWithInvalidStackVar</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>delaycapture-loopbody2.js</files>
//     <compile-flags>-lic:1 -bgjit- -InitializeInterpreterSlotsWithInvalidStackVar</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>initcachedscope.js</files>
//     <compile-flags>-recyclerstress -force:cachedscope</compile-flags>
//     <baseline>initcachedscope.baseline</baseline>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

#[test]
fn initcachedscope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "initcachedscope.js",
        baseline_path: Some("initcachedscope.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn invalcachedscope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope.js",
        baseline_path: Some("invalcachedscope.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Serialized</compile-flags>
//     <files>invalcachedscope.js</files>
//     <baseline>invalcachedscope.baseline</baseline>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>invalcachedscope.js</files>
//     <compile-flags>-force:deferparse -Intl-</compile-flags>
//     <baseline>invalcachedscope.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn invalcachedscope_caller_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "invalcachedscope-caller.js",
        baseline_path: Some("invalcachedscope-caller.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_2299723_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2299723.js",
        baseline_path: Some("bug_OS_2299723.baseline"),
        compile_flags: vec!["-force:cachedscope"],
    };
    common::run_test(&test);
}

#[test]
fn bug_os_2671095_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2671095.js",
        compile_flags: vec!["-forcedeferparse", "-force:cachedscope"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_2903083_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2903083.js",
        compile_flags: vec!["-force:cachedscope"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_9781249_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_9781249.js",
        compile_flags: vec!["-forcejitloopbody", "-force:inline", "-force:rejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS_9008744.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn bug_os_10735999_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_10735999.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS_13412380.js</files>
//     <tags>BugFix,exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn copy_prop_stack_slot_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "copy-prop-stack-slot.js",
        baseline_path: Some("copy-prop-stack-slot.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS_13412380.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn update_funcexpr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "update-funcexpr.js",
        ..Default::default()
    };
    common::run_test(&test);
}
