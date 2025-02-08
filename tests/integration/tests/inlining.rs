use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 1] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/inlining";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arg_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arg.js",
        baseline_path: Some("arg.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber1.js</files>
//       <baseline>linenumber1.baseline</baseline>
//       <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// Variant of test without -force:inline to ensure output is the same
// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber1.js</files>
//       <baseline>linenumber1.baseline</baseline>
//       <compile-flags>-ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber2.js</files>
//       <baseline>linenumber2.baseline</baseline>
//       <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// Variant of test without -force:inline to ensure output is the same
// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber2.js</files>
//       <baseline>linenumber2.baseline</baseline>
//       <compile-flags>-ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber3.js</files>
//       <baseline>linenumber3.baseline</baseline>
//       <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// Variant of test without -force:inline to ensure output is the same
// TODO investigate why -ExtendedErrorStackForTestHost inserts tabs at the beginning of some frames (Microsoft/ChakraCore#3130)
// <compile-flags>-force:inline -ExtendedErrorStackForTestHost-</compile-flags>
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>linenumber3.js</files>
//       <baseline>linenumber3.baseline</baseline>
//       <compile-flags>-ExtendedErrorStackForTestHost-</compile-flags>
//       <tags>exclude_arm</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>InlineConstructors.js</files>
//       <baseline>InlineConstructors.baseline</baseline>
//       <compile-flags>-force:inline</compile-flags>
//       <tags>exclude_arm,Slow</tags>
//       <timeout>300</timeout>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inlined_constructor_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InlinedConstructorBailout.js",
        baseline_path: Some("InlinedConstructorBailout.baseline"),
        compile_flags: vec!["-force:inline"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inlining_with_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inliningWithArguments.js",
        compile_flags: vec!["-force:inline"],
        tags: HashSet::from(["require_backend", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inlining_apply_target_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inliningApplyTarget.js",
        baseline_path: Some("inliningApplyTarget.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn apply_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "applyBugs.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn apply_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "applyBailout.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:0"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn apply_bailout_args_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "applyBailoutArgs.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugs.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simplejit",
            "-loopinterpretcount:1",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn linenumber4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "linenumber4.js",
        baseline_path: Some("linenumber4.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-loopinterpretcount:0",
            "-force:inline",
            "-bgjit-",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn miscellaneous_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Miscellaneous_MaxInterpret.js",
        baseline_path: Some("Miscellaneous_MaxInterpret.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-force:inline",
        ],
        tags: HashSet::from(["require_backend", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn no_prof_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NoProf.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inline_built_ins_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineBuiltIns.js",
        compile_flags: vec!["-maxInterpretCount:1", "-msjrc:0"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn spread_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spread.js",
        compile_flags: vec!["-maxSimpleJitRunCount:1", "-maxInterpretCount:1"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn poly_inlining_fixed_methods_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "polyInliningFixedMethods.js",
        baseline_path: Some("polyInliningFixedMethods.baseline"),
        compile_flags: vec![
            "-maxInterpretCount:1",
            "-maxSimpleJitRunCount:1",
            "-ExtendedErrorStackForTestHost",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug650495_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug650495.js",
        baseline_path: Some("bug650495.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn poly_inlining_bugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "polyInliningBugs.js",
        compile_flags: vec![
            "-maxInterpretCount:1",
            "-maxSimpleJitRunCount:1",
            "-off:aggressiveinttypespec",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn poly_inlining_uninitialized_ret_val_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "polyInliningUninitializedRetVal.js",
        baseline_path: Some("polyInliningUninitializedRetVal.baseline"),
        compile_flags: vec![
            "-bgjit-",
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-force:inline",
            "-stress:BailOnNoProfile",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn call_target_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callTarget.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug594138_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug594138.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-force:scriptfunctionwithinlinecache",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inlinee_argout_count_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineeArgoutCount.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn mark_temp_arg_out_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "markTempArgOut.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug1469518_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1469518.js",
        baseline_path: Some("bug1469518.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug1355201_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1355201.js",
        baseline_path: Some("bug1355201.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn recursive_inline_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "recursive_inline.js",
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn recursive_inline2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "recursive_inline2.js",
        compile_flags: vec!["-maxinterpretcount:2", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug2328551_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug2328551.js",
        baseline_path: Some("bug2328551.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug2269097_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug2269097.js",
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn os_2733280_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_2733280.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-force:scriptfunctionwithinlinecache",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn os_2733280_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_2733280.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-force:scriptfunctionwithinlinecache",
            "-off:stackargopt",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn built_in_apply_target_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "builtInApplyTarget.js",
        baseline_path: Some("builtInApplyTarget.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn stack_trace_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackTrace.js",
        baseline_path: Some("stackTrace.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn missing_inlinee_end_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "missingInlineeEnd.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn inlining_in_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inliningInLoopBody.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-", "-force:inline"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug9936017_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug9936017.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug11265991_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug11265991.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn profilingbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "profilingbug.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug12528802_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug12528802.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn call_to_dynamic_script_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callToDynamicScript.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn inline_callbacks_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InlineCallbacks.js",
        baseline_path: Some("InlineCallbacks.baseline"),
        compile_flags: vec!["-testtrace:InlineCallbacks"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_forceserialized",
            "require_backend",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn inline_callback_call_bailout_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InlineCallbackCallBailout.js",
        baseline_path: Some("InlineCallbackCallBailout.baseline"),
        compile_flags: vec!["-testtrace:InlineCallbacks", "-testtrace:Bailout"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_forceserialized",
            "require_backend",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn inline_call_instance_function_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InlineCallInstanceFunction.js",
        baseline_path: Some("InlineCallInstanceFunction.baseline"),
        compile_flags: vec![
            "-TestChakraLib",
            "-testtrace:bailout",
            "-testtrace:inlineCallbacks",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_forceserialized",
            "require_backend",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn recursive_callbacks_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "recursiveCallbacks.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argoptbugs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argoptbugs.js",
        compile_flags: vec!["-mic:1", "-bgjit-", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>bug_gh6303.js</files>
//       <flags>exclude_nonative</flags>
//     </default>
//   </test>
