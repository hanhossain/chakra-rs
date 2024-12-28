mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Function";

#[test]
fn label_func_as_with_stmt_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LabelFuncAsWithStmt.js",
        baseline_path: Some("LabelFuncAsWithStmt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn apply_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "apply.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn apply3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "apply3.js",
        baseline_path: Some("apply3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn apply_args_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "applyArgs.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments1.js",
        baseline_path: Some("arguments1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments2.js",
        baseline_path: Some("arguments2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments3.js",
        baseline_path: Some("arguments3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_misc_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argumentsMisc.js",
        baseline_path: Some("argumentsMisc_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_es5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.es5.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_resolution_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argumentsResolution.js",
        baseline_path: Some("argumentsResolution_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_limits_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argumentsLimits.js",
        baseline_path: Some("argumentsLimits.baseline"),
        compile_flags: vec!["-EnableFatalErrorOnOOM-"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn some_more_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "someMoreArguments.js",
        baseline_path: Some("someMoreArguments_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bind_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bind.js",
        baseline_path: Some("bind.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn builtin_func_has_own_prop_caller_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "builtinFuncHasOwnPropCallerArguments.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_and_bound_func_length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcAndboundFuncLength.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn call1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "call1.js",
        baseline_path: Some("call1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn call2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "call2.js",
        baseline_path: Some("call2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn caller_args_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CallerArgs.js",
        baseline_path: Some("CallerArgsES5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn callsideeffects_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callsideeffects.js",
        baseline_path: Some("callsideeffects_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn catchsymbolvar_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "catchsymbolvar.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newsideeffect_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newsideeffect.js",
        baseline_path: Some("newsideeffect.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newsideeffect_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newsideeffect.js",
        baseline_path: Some("newsideeffect.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn callmissingtgt_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callmissingtgt.js",
        baseline_path: Some("callmissingtgt.6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defernested_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defernested.js",
        baseline_path: Some("defernested.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defernested_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defernested.js",
        baseline_path: Some("defernested.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn jit_loop_body_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jitLoopBody.js",
        baseline_path: Some("jitLoopBody.baseline"),
        compile_flags: vec!["-force:jitLoopBody"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferredParsing.js</files>
//     <baseline>deferredParsing_3.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferredParsing.js</files>
//     <baseline>deferredParsing_3.baseline</baseline>
//     <compile-flags>-forceUndoDefer</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferredGetterSetter.js</files>
//     <baseline>deferredGetterSetter.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn deferred_bad_continue_js1() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferredBadContinue.js",
        baseline_path: Some("deferredBadContinue.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn deferred_bad_continue_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferredBadContinue.js",
        baseline_path: Some("deferredBadContinue.baseline"),
        compile_flags: vec!["-Force:Deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn deferredstuboob_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferredstuboob.js",
        compile_flags: vec!["-force:deferparse", "-pageheap:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferredWith.js</files>
//     <baseline>deferredWith.v4.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>deferredWith2.js</files>
//     <baseline>deferredWith2.baseline</baseline>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn delete_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deleteProperty.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_function_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newFunction.js",
        baseline_path: Some("newFunction.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prototype.js",
        baseline_path: Some("prototype.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype_set_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prototype_set.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tapply1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TApply1.js",
        baseline_path: Some("TApply1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString.js",
        baseline_path: Some("toString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_all_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringAll.js",
        baseline_path: Some("toStringAll.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_all_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringAll.js",
        baseline_path: Some("toStringAll.baseline"),
        compile_flags: vec!["-force:DeferParse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_all_js_defer_parse_force_serialized() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringAll.js",
        baseline_path: Some("toStringAll.baseline"),
        compile_flags: vec!["-force:DeferParse", "-ForceSerialized"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_all_js_defer_parse_parser_state_cache() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringAll.js",
        baseline_path: Some("toStringAll.baseline"),
        compile_flags: vec![
            "-force:DeferParse",
            "-UseParserStateCache",
            "-ParserStateCache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_expr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcExpr.js",
        baseline_path: Some("funcExpr5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn more_func_expr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreFuncExpr.js",
        baseline_path: Some("moreFuncExpr3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn more_func_expr_js_force_undo_defer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreFuncExpr.js",
        baseline_path: Some("moreFuncExpr3.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn even_more_func_expr3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evenMoreFuncExpr3.js",
        baseline_path: Some("evenMoreFuncExpr3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn some_more_func_expr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "someMoreFuncExpr.js",
        baseline_path: Some("someMoreFuncExpr3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constructor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor.js",
        baseline_path: Some("constructor.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ctr_flags_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ctrFlags.js",
        baseline_path: Some("ctrFlags3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn type_error_accessor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typeErrorAccessor.js",
        baseline_path: Some("typeErrorAccessor.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_body_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FuncBody.js",
        baseline_path: Some("FuncBodyES5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>FuncBody.bug133933.js</files>
//     <baseline>FuncBody.bug133933.baseline</baseline>
//     <compile-flags>-trace:FunctionSourceInfoParse -off:deferparse</compile-flags>
//     <tags>exclude_test,exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn func_body_bug227901_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FuncBody.bug227901.js",
        baseline_path: Some("FuncBody.bug227901.baseline"),
        compile_flags: vec!["-off:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_body_bug232281_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FuncBody.bug232281.js",
        baseline_path: Some("FuncBody.bug232281.baseline"),
        compile_flags: vec!["-off:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_body_bug236810_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FuncBody.bug236810.js",
        compile_flags: vec!["-off:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>FuncBody.bug231397.js</files>
//     <baseline>FuncBody.bug231397.baseline</baseline>
//     <compile-flags>-dump:bytecode</compile-flags>
//     <tags>exclude_bytecodelayout,exclude_test,exclude_nonative,require_backend</tags>
//   </default>
// </test>

#[test]
fn bug_258259_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_258259.js",
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn same_name_para_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sameNamePara.js",
        baseline_path: Some("sameNamePara.baseline"),
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
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn undef_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "undefThis.js",
        baseline_path: Some("undefThis.v10.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stackargs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackargs.js",
        baseline_path: Some("stackargs.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stack_args_with_formals_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackArgsWithFormals.js",
        compile_flags: vec!["-mic:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stack_args_with_formals_js_force_serialized() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackArgsWithFormals.js",
        compile_flags: vec!["-mic:1", "-off:simpleJit", "-forceserialized"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stack_args_with_formals_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackArgsWithFormals.js",
        compile_flags: vec!["-mic:1", "-off:simpleJit", "-forcedeferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>StackArgsWithFormals.js</files>
//     <compile-flags>-mic:1 -off:simpleJit -trace:stackargformalsopt</compile-flags>
//     <tags>exclude_dynapogo,exclude_test,exclude_nonative,require_backend,exclude_forceserialized,exclude_arm64</tags>
//     <baseline>StackArgsWithFormals.baseline</baseline>
//   </default>
// </test>

#[test]
fn stack_args_max_interpret_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "StackArgs_MaxInterpret.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stackArgsLenConstOpt.js</files>
//     <compile-flags>-mic:1 -off:simpleJit -testtrace:StackArgLenConstOpt</compile-flags>
//     <tags>exclude_dynapogo,exclude_nonative,require_backend,exclude_forceserialized,exclude_arm64</tags>
//     <baseline>stackArgsLenConstOpt.baseline</baseline>
//   </default>
// </test>

#[test]
fn stack_args_with_inlinee_bail_out_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackArgsWithInlineeBailOut.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>childCallsEvalJitLoopBody.js</files>
//     <baseline />
//     <compile-flags>-prejit</compile-flags>
//     <tags>exclude_test,exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>631838.js</files>
//     <compile-flags>-nonative</compile-flags>
//     <!-- On ARM it would take TOO long (700s on Tegra 2 vs 20s on Intel Core i7) to run, as we need to use even bigger object -->
//     <tags>exclude_dynapogo,exclude_arm,Slow</tags>
//   </default>
// </test>

#[test]
fn calli_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "calli.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn caller_replaced_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "caller_replaced_proto.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug542360_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug542360.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug6738_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug6738.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn crosssite_bind_main_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "crosssite_bind_main.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>failnativecodeinstall.js</files>
//     <!-- This test expects OOM, -EnableFatalErrorOnOOM- to disable fatal error for this test case. Bug will be filed to address this later -->
//     <compile-flags>-EnableFatalErrorOnOOM- -maxinterpretcount:2 -lic:1 -bgjit -off:simplejit -on:failnativecodeinstall</compile-flags>
//     <tags>exclude_dynapogo,exclude_nonative,require_backend</tags>
//     <baseline>failnativecodeinstall.baseline</baseline>
//   </default>
// </test>

#[test]
fn redefer_recursive_inlinees_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redefer-recursive-inlinees.js",
        baseline_path: Some("redefer-recursive-inlinees.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn redefer_f_i_b_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redefer-f-i-b-eval.js",
        compile_flags: vec!["-force:deferparse", "-force:redeferral"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os17698041_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_os17698041.js",
        compile_flags: vec![
            "-off:deferparse",
            "-force:redeferral",
            "-collectgarbage",
            "-parserstatecache",
            "-useparserstatecache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}
