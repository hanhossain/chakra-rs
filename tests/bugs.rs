mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Bugs";

#[test]
fn bug602_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug602.js",
        baseline_path: Some("bug602_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug764_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug764.js",
        baseline_path: Some("bug764.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn withnonative_apply_optimization_bug3433559_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "withnonativeApplyOptimizationBug3433559.js",
        compile_flags: vec!["-nonative"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn win8_486977_branch_strict_equal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Win8_486977_BranchStrictEqual.js",
        baseline_path: Some("Win8_486977_BranchStrictEqual.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Win8_459638.js</files>
//     <baseline>Win8_459638.baseline</baseline>
//     <tags>exclude_x86,exclude_x64</tags>
//     <!-- The bug is related to ARM only -->
//     <compile-flags>-forceNative -off:simpleJit</compile-flags>
//   </default>
// </test>

#[test]
fn bug_os_1197716_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_1197716.js",
        compile_flags: vec!["-deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn addexception_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "addexception.js",
        baseline_path: Some("addexception.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regalloc_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regalloc.js",
        baseline_path: Some("regalloc.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn randombug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "randombug.js",
        baseline_path: Some("randombug.baseline"),
        compile_flags: vec![
            "-CollectGarbage",
            "-ExtendedErrorStackForTestHost",
            "-off:earlyreferenceerrors",
        ],
    };
    common::run_test(&test);
}

#[test]
fn blue_532460_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blue_532460.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-MaxinterpretCount:1",
            "-MaxSimpleJITRunCount:0",
            "-loopinterpretcount:1",
            "-force:deferparse",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug56026.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn bug56026_minimal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug56026_minimal.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug56026_minimal_with_properties_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug56026_minimalWithProperties.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug56026_nested_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug56026_nested.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug56026_trycatch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug56026_trycatch.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blue_245702_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blue_245702.js",
        baseline_path: Some("blue_245702.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:2 -maxsimplejitruncount:5  -off:inline</compile-flags>
//     <files>bug547302.js</files>
//     <tags>exclude_shp</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:2  -MaxLinearStringCaseCount:2 -MaxLinearIntCaseCount:2 -forceserialized -MinSwitchJumpTableSize:3 -bgjit- -loopinterpretcount:1 -force:polymorphicinlinecache -force:fieldcopyprop -sse:2 -force:interpreterautoprofile</compile-flags>
//     <files>bug215238.mul-53-ovf.js</files>
//     <baseline>bug215238.mul-53-ovf.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1  -MinSwitchJumpTableSize:3 -bgjit- -loopinterpretcount:1 -force:rejit -force:ScriptFunctionWithInlineCache -force:fixdataprops</compile-flags>
//     <files>bug215238-shrua.js</files>
//     <baseline>bug215238-shrua.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1  -force:polymorphicinlinecache -MinSwitchJumpTableSize:2 -force:rejit -force:ScriptFunctionWithInlineCache -force:atom -off:ArrayCheckHoist -force:fixdataprops -ForceArrayBTree</compile-flags>
//     <files>bug215238.shrua-2.js</files>
//     <baseline>bug215238.shrua-2.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -off:simplejit</compile-flags>
//     <files>bug435809.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -off:simplejit -force:fixdataprops</compile-flags>
//     <files>bug594298.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-loopinterpretcount:1 -bgjit- -maxsimplejitruncount:1 -maxinterpretcount:1 -force:inline</compile-flags>
//     <files>bug661952.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-loopinterpretcount:1 -bgjit- -maxsimplejitruncount:1 -maxinterpretcount:1 -force:deferparse -force:inline</compile-flags>
//     <files>bug724121.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -force:fieldcopyprop -forceserialized</compile-flags>
//     <files>deserializationbug339404.js</files>
//   </default>
// </test>

#[test]
fn bug843670_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug843670.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug934443.js</files>
//     <compile-flags>-force:fieldcopyprop -off:dynamicprofile</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn vso_os_1091425_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "vso_os_1091425.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// this
#[test]
fn bug1092916_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1092916.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>blue_1096569.js</files>
//     <compile-flags>-MaxinterpretCount:2 -off:simplejit -force:Inline -off:insertnops</compile-flags>
//     <baseline>blue_1096569.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>blue_1086262.js</files>
//     <compile-flags>-off:insertnops -bgjit- -fja:2 -off:simplejit -force:fieldcopyprop -off:aggressiveinttypespec</compile-flags>
//     <baseline>blue_1086262.baseline</baseline>
//   </default>
// </test>

#[test]
fn bug1288931_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug1288931.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:fastpath"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn os_1362136_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_1362136.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:fastpath"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS_4683246.js</files>
//     <tags>exclude_dynapogo</tags>
//     <compile-flags>-loopinterpretcount:0 -args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>fabs1.js</files>
//     <tags>exclude_dynapogo,require_backend,require_asmjs</tags>
//     <compile-flags>-off:backend -asmjs -testtrace:asmjs</compile-flags>
//     <baseline>fabs1.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>OS_5248645.js</files>
//     <tags>exclude_dynapogo</tags>
//     <compile-flags>-maxinterpretCount:2 -off:simplejit -off:dynamicProfile -args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn os_5553123_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_5553123.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn symbol_tostring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symbol_tostring.js",
        compile_flags: vec![
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-force:fieldcopyprop",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_undodefer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default_undodefer.js",
        compile_flags: vec!["-forcedeferparse", "-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_resetisdead_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug_resetisdead.js",
        compile_flags: vec!["-on:prelowererpeeps"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_es5array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_es5array.js",
        compile_flags: vec!["-off:NativeArray", "-ForceES5Array"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn simpletypehandler_property_deletion_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simpletypehandler-property-deletion.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_only_writable_data_properties_cross_context_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HasOnlyWritableDataProperties-cross-context.js",
        compile_flags: vec!["-nonative"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug9080773_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug9080773.js",
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug9080773_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug9080773_2.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug8554038_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug8554038.js",
        baseline_path: Some("bug8554038.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>invertloop_bug.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn typespec_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typespec_bug.js",
        compile_flags: vec!["-maxinterpretcount:1", "-maxsimplejitruncount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn deletenonconfig_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deletenonconfig.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>misc_bugs.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//     <tags>exclude_windows</tags>
//   </default>
// </test>

#[test]
fn cross_context_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cross_context_test.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn json_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "json_bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug10191241_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug10191241.js",
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inline_function_parameter_with_infinite_loop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InlineFunctionParameterWithInfiniteLoop.js",
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn math_abs_type_spec_on_int_min_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Math.abs_TypeSpecOnIntMin.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-oopjit-",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug10026875_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug10026875.js",
        baseline_path: Some("bug10026875.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
    };
    common::run_test(&test);
}

#[test]
fn miss_to_generate_st_st_slot_for_jitloop_body_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissToGenerateStStSlotForJITLoopBody.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-oopjit-", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cmpfgpeep_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cmpfgpeep.js",
        compile_flags: vec!["-maxsimplejitruncount:1", "-maxinterpretcount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug11026788_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug11026788.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug11576900.js</files>
//     <compile-flags>-lic:1 -bgjit-</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn bug12628506_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug12628506.js",
        compile_flags: vec!["-loopinterpretcount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug13172050_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug13172050.js",
        compile_flags: vec!["-pageheap:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug13213828_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug13213828.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn value_info_loss_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valueInfoLossBug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn os11907290_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "os11907290.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug13383062.js</files>
//     <compile-flags>-maxinterpretcount:2 -off:simplejit -off:lossyinttypespec -bgjit-</compile-flags>
//     <baseline>bug13383062.baseline</baseline>
//   </default>
// </test>

#[test]
fn profiled_args_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "profiledArgs.js",
        compile_flags: vec!["-off:inlineConstructors"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug14323330_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug14323330.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug13830477_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug13830477.js",
        baseline_path: Some("bug13830477.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug15490382_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug15490382.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os14326981_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14326981.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug14057294_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug14057294.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os14115684_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14115684.js",
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inbug2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inbug2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn instancebug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "instancebug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missingvalue_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "missingvalue.js",
        compile_flags: vec!["-off:ArrayMissingValueCheckHoist"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn symcmpbug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symcmpbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os17417473_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS17417473.js",
        compile_flags: vec![
            "-pageheap:2",
            "-CollectGarbage",
            "-lic:4",
            "-Sja:4",
            "-Fja:6",
            "-maxInterpretCount:2",
            "-MinBailOutsBeforeRejit:2",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn home_obj_in_loop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HomeObjInLoop.js",
        compile_flags: vec![
            "-forceNative",
            "-forcejitloopbody",
            "-off:aggressiveinttypespec",
            "-off:ArrayCheckHoist",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug17785360_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug17785360.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os17530048_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS17530048.js",
        compile_flags: vec![
            "-force:deferparse",
            "-parserstatecache",
            "-useparserstatecache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn skipping_nested_deferred_incorrect_function_id_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "skipping_nested_deferred_incorrect_function_id.js",
        compile_flags: vec![
            "-force:deferparse",
            "-parserstatecache",
            "-useparserstatecache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_split_scope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "withSplitScope.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os17614914_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS17614914.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn os_17745531_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_17745531.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn super_undo_defer_issue_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SuperUndoDeferIssue.js",
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_attr_issue_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ArgumentsAttrIssue.js",
        compile_flags: vec!["-force:cachedScope"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn deferred_stub_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferredStubBugs.js",
        compile_flags: vec![
            "-force:deferparse",
            "-parserstatecache",
            "-useparserstatecache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn super_access_in_global_lambda_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SuperAccessInGlobalLambda.js",
        baseline_path: Some("SuperAccessInGlobalLambda.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_5572_wscript_loadscript_loadmodule_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_5572_wscript_loadscript_loadmodule.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn function_id_destructured_reparse_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "function_id_destructured_reparse.js",
        compile_flags: vec![
            "-useparserstatecache",
            "-parserstatecache",
            "-force:deferparse",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_5585_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_5585.js",
        compile_flags: vec![
            "-esdynamicimport",
            "-mutehosterrormsg",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn loopcrash_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopcrash.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-MinMemOpCount:0",
            "-werexceptionsupport",
            "-bgjit-",
            "-loopinterpretcount:1",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os18260560_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS18260560.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_5883_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_5883.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os18926499_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS18926499.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug19767482_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug19767482.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-bgjit-",
            "-oopjit-",
            "-loopinterpretcount:1",
            "-maxsimplejitruncount:2",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug19948792_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug19948792.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-bgjit-",
            "-oopjit-",
            "-loopinterpretcount:1",
            "-maxsimplejitruncount:2",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_6179_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_6179.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os21193960_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS21193960.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_6239_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_6239.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_6277_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_6277.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os23102586_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS23102586.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}
