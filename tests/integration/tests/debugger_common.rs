use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 4] = [
    "exclude_serialized",
    "exclude_snap",
    "require_debugger",
    "exclude_sanitize_address",
];
const DIRECTORY: &str = "chakracore-cxx/test/DebuggerCommon";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn arguments_map_es6_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments_mapES6_attach.js",
        baseline_path: Some("arguments_mapES6_attach.js.baseline"),
        compile_flags: vec!["-dbgbaseline:arguments_mapES6_attach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockscope_fastdebug_es5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_fastdebug_ES5.js",
        baseline_path: Some("blockscope_fastdebug_ES5.js.baseline"),
        compile_flags: vec!["-dbgbaseline:blockscope_fastdebug_ES5.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockscope_fastdebug_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_fastdebug_ES6.js",
        baseline_path: Some("blockscope_fastdebug_ES6.js.baseline"),
        compile_flags: vec!["-dbgbaseline:blockscope_fastdebug_ES6.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockscope_func_insidescopes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_func_insidescopes.js",
        baseline_path: Some("blockscope_func_insidescopes.js.baseline"),
        compile_flags: vec![
            "-dbgbaseline:blockscope_func_insidescopes.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:IntlInit.js.dbg.baseline -Intl</compile-flags>
//       <files>IntlInit.js</files>
//       <tags>Intl,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_intl_stepinto_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_intl_stepinto.js",
        compile_flags: vec!["-dbgbaseline:ES6_intl_stepinto.js.dbg.baseline", "-Intl"],
        tags: HashSet::from([
            "Intl",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_const_reassignment_fnscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_const_reassignment_fnscope.js",
        baseline_path: Some("ES6_letconst_const_reassignment_fnscope.js.baseline"),
        compile_flags: vec!["-dbgbaseline:ES6_letconst_const_reassignment_fnscope.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_eval_strict_fn_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_eval_strict_fn.js",
        baseline_path: Some("ES6_letconst_eval_strict_fn.js.baseline"),
        compile_flags: vec!["-dbgbaseline:ES6_letconst_eval_strict_fn.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_redeclaration_indebugger_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_redeclaration_indebugger.js",
        baseline_path: Some("ES6_letconst_redeclaration_indebugger.js.baseline"),
        compile_flags: vec!["-dbgbaseline:ES6_letconst_redeclaration_indebugger.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_shadow_evaluation_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_shadow_evaluation.js",
        compile_flags: vec!["-dbgbaseline:ES6_letconst_shadow_evaluation.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_shadow_eval_with_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_shadow_eval_with.js",
        baseline_path: Some("ES6_letconst_shadow_eval_with.js.baseline"),
        compile_flags: vec![
            "-dbgbaseline:ES6_letconst_shadow_eval_with.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl.js",
        baseline_path: Some("es6_forof_decl.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl-2.js",
        baseline_path: Some("es6_forof_decl-2.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl-2.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl-3.js",
        baseline_path: Some("es6_forof_decl-3.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl-3.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl-4.js",
        baseline_path: Some("es6_forof_decl-4.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl-4.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl-5.js",
        baseline_path: Some("es6_forof_decl-5.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl-5.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_forof_decl_6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6_forof_decl-6.js",
        baseline_path: Some("es6_forof_decl-6.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-dbgbaseline:es6_forof_decl-6.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn frames_values_map_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "frames_values_mapES6.js",
        baseline_path: Some("frames_values_mapES6.js.baseline"),
        compile_flags: vec![
            "-dbgbaseline:frames_values_mapES6.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_in_es6_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_in_ES6_attach.js",
        compile_flags: vec!["-dbgbaseline:step_in_ES6_attach.js.dbg.baseline", "-Intl-"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_in_from_interpreted_function_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_in_from_interpreted_function_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_in_from_interpreted_function_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_in_from_jitted_function_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_in_from_JITted_function_attach.js",
        compile_flags: vec!["-dbgbaseline:step_in_from_JITted_function_attach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_in_only_debug_jit_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_in_only_debugJIT_attach.js",
        baseline_path: Some("step_in_only_debugJIT_attach.js.baseline"),
        compile_flags: vec![
            "-dbgbaseline:step_in_only_debugJIT_attach.js.dbg.baseline",
            "-maxinterpretcount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_direct_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_direct_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_out_direct_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_es5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_ES5.js",
        compile_flags: vec!["-dbgbaseline:step_out_ES5.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_ES6.js",
        baseline_path: Some("step_out_ES6.js.baseline"),
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-dbgbaseline:step_out_ES6.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_from_catch_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_from_catch_attach.js",
        baseline_path: Some("step_out_from_catch_attach.js.baseline"),
        compile_flags: vec![
            "-dbgbaseline:step_out_from_catch_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_from_interpreted_function_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_from_interpreted_function_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_out_from_interpreted_function_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_from_jitted_function_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_from_JITted_function_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_out_from_JITted_function_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_out_only_debug_jit_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_out_only_debugJIT_attach.js",
        compile_flags: vec!["-dbgbaseline:step_out_only_debugJIT_attach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_over_es6_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_over_ES6_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_over_ES6_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn step_over_jitd_fn_from_intrprt_fn_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "step_over_JITd_fn_from_Intrprt_fn_attach.js",
        compile_flags: vec![
            "-dbgbaseline:step_over_JITd_fn_from_Intrprt_fn_attach.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn temp_str_expr_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TempStrExpr.js",
        baseline_path: Some("TempStrExpr.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-ES6",
            "-ES6ObjectLiterals",
            "-dbgbaseline:TempStrExpr.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ES6_intl_simple_attach.js</files>
//       <baseline>ES6_intl_simple_attach.js.baseline</baseline>
//       <compile-flags>-dbgbaseline:ES6_intl_simple_attach.js.dbg.baseline -Intl</compile-flags>
//       <!-- This test is still require_winglob because it has winglob-specific output in the .dbg.baseline -->
//       <tags>Intl,require_winglob,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn frames_inspection_array_es5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "frames_inspection_arrayES5.js",
        baseline_path: Some("frames_inspection_arrayES5.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:frames_inspection_arrayES5.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn shadow_with_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadow_with.js",
        baseline_path: Some("shadow_with.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:shadow_with.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockscope_func_declaration_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_func_declaration_ES6.js",
        baseline_path: Some("blockscope_func_declaration_ES6.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockscope_func_declaration_ES6.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn blockscope_func_expression_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_func_expression_ES6.js",
        baseline_path: Some("blockscope_func_expression_ES6.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-es6functionnamefull",
            "-dbgbaseline:blockscope_func_expression_ES6.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_eval_nonstrict_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_eval_nonstrict.js",
        baseline_path: Some("ES6_letconst_eval_nonstrict.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_eval_nonstrict.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_for_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_for.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_for.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_trycatch_simple_fast_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_trycatch_simple_fast.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_trycatch_simple_fast.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_proto_chained_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_proto_chained.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_proto_chained.js.dbg.baseline",
            "-Intl-",
            "-disableDebugObject",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_proto_simple_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_proto_simple.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_proto_simple.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_proto_user_defined_object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_proto_userDefinedObject.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_proto_userDefinedObject.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_intl_stepinto_libexpandos_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_intl_stepinto_libexpandos.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_intl_stepinto_libexpandos.js.dbg.baseline",
            "-Intl",
        ],
        tags: HashSet::from([
            "Intl",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_forin_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_forin.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_forin.js.dbg.baseline",
            "-Intl",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_const_usebeforedeclaration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_const_usebeforedeclaration.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_const_usebeforedeclaration.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_proto_invalidation_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_proto_invalidation.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_proto_invalidation.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn frames_letconst_reassignobjects_es6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "frames_letconst_reassignobjects_ES6.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:frames_letconst_reassignobjects_ES6.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_const_reassignment_globalscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_const_reassignment_globalscope.js",
        baseline_path: Some("ES6_letconst_const_reassignment_globalscope.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_const_reassignment_globalscope.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_letconst_redcl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_letconst_redcl.js",
        baseline_path: Some("ES6_letconst_redcl.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_letconst_redcl.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn native_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "native_array.js",
        baseline_path: Some("native_array.js.baseline"),
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:native_array.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn argument_disp_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argument_disp.js",
        baseline_path: Some("argument_disp.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:argument_disp.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multiple_argumentsdisp_safeguard_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiple_argumentsdisp_safeguard.js",
        baseline_path: Some("multiple_argumentsdisp_safeguard.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:multiple_argumentsdisp_safeguard.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn level_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "level_1.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:level_1.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn failedasm_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "failedasm.js",
        compile_flags: vec!["-dbgbaseline:emptyJson.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_spread_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_spread.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ES6_spread.js.dbg.baseline",
            "-es6spread",
            "-nonative",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn specialproperties_fn_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "specialproperties_fn.js",
        compile_flags: vec![
            "-debuglaunch",
            "-inspectmaxstringlength:100",
            "-dbgbaseline:specialproperties_fn.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_reg_exp_specialproperties_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_RegExp_specialproperties.js",
        compile_flags: vec![
            "-ES6Unicode-",
            "-ES2018RegExDotAll-",
            "-ES6RegExSticky-",
            "-debuglaunch",
            "-inspectmaxstringlength:100",
            "-dbgbaseline:ES6_RegExp_specialproperties_default.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_reg_exp_specialproperties_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_RegExp_specialproperties.js",
        compile_flags: vec![
            "-ES6Unicode",
            "-ES2018RegExDotAll-",
            "-ES6RegExSticky-",
            "-debuglaunch",
            "-inspectmaxstringlength:100",
            "-dbgbaseline:ES6_RegExp_specialproperties_with_unicode.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_reg_exp_specialproperties_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_RegExp_specialproperties.js",
        compile_flags: vec![
            "-ES6Unicode-",
            "-ES2018RegExDotAll-",
            "-ES6RegExSticky",
            "-debuglaunch",
            "-inspectmaxstringlength:100",
            "-dbgbaseline:ES6_RegExp_specialproperties_with_sticky.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn es6_reg_exp_specialproperties_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6_RegExp_specialproperties.js",
        compile_flags: vec![
            "-ES6Unicode",
            "-ES6RegExSticky",
            "-debuglaunch",
            "-inspectmaxstringlength:100",
            "-dbgbaseline:ES6_RegExp_specialproperties_all.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn specialproperties_level2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "specialproperties_level2.js",
        compile_flags: vec![
            "-debuglaunch",
            "-es6functionnamefull",
            "-inspectmaxstringlength:256",
            "-dbgbaseline:specialproperties_level2.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn testdynamicattach1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "testdynamicattach1.js",
        baseline_path: Some("testdynamicattach1.baseline"),
        compile_flags: vec![
            "-dbgbaseline:testdynamicattach1.js.dbg.baseline",
            "-DeferLoadingAvailableSource",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn testdynamicattach1_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "testdynamicattach1.js",
        baseline_path: Some("testdynamicattach1.baseline"),
        compile_flags: vec![
            "-dbgbaseline:testdynamicattach1.js.dbg.baseline",
            "-Intl-",
            "-DeferLoadingAvailableSource",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn targeted_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "targeted.js",
        baseline_path: Some("targeted.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:targeted.js.dbg.baseline",
            "-DeferLoadingAvailableSource",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn proto_test2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "protoTest2.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:protoTest2.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn testdynamicattach2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "testdynamicattach2.js",
        baseline_path: Some("testdynamicattach2.baseline"),
        compile_flags: vec!["-dbgbaseline:testdynamicattach2.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn defer_parse_detach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferParseDetach.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:deferParseDetach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn defer_parse_detach2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferParseDetach2.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:deferParseDetach2.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn attach_with_defer_parse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "attachWithDeferParse.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-force:deferparse",
            "-dbgbaseline:attachWithDeferParse.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn array_prototest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_prototest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:array_prototest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn breakpoints_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "breakpoints.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:breakpoints.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_jsrt",
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn indexprop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "indexprop.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:indexprop.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn func_source_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcSource.js",
        compile_flags: vec![
            "-inspectmaxstringlength:100",
            "-debuglaunch",
            "-dbgbaseline:funcSource.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn evaluate_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evaluate.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:evaluate.js.dbg.baseline",
            "-InspectMaxStringLength:100",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn attach_after_exception_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "attachAfterException.js",
        compile_flags: vec!["-dbgbaseline:attachAfterException.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn catch_inspection_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "catchInspection.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:catchInspection.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn func_expr_name_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcExprName.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:funcExprName.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn attachdetach_delaycapture_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "attachdetach-delaycapture.js",
        compile_flags: vec!["-dbgbaseline:attachdetach-delaycapture.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn global_func_vars_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "globalFuncVars.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:globalFuncVars.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_capture_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArrayCaptureAttach.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-dbgbaseline:blockScopeSlotArrayCaptureAttach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_capture_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArrayCapture.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeSlotArrayCapture.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_activation_object_capture_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeActivationObjectCapture.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeActivationObjectCapture.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_basic_let_const_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeBasicLetConstTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeBasicLetConstTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_activation_object_as_slot_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeActivationObjectAsSlotArray.js",
        compile_flags: vec![
            "-InspectMaxStringLength:150",
            "-debuglaunch",
            "-dbgbaseline:blockScopeActivationObjectAsSlotArray.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_basic_scoping_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeBasicScopingTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeBasicScopingTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_for_test_bug183991_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeForTest.bug183991.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeForTest.bug183991.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_nested_function_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeNestedFunctionTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeNestedFunctionTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeDeadZoneTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_eval_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeEvalTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeEvalTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_global_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeGlobalTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeGlobalTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_for_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeForTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeForTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_with_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeWithTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeWithTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_switch_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSwitchTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeSwitchTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_activation_object_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeActivationObjectDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeActivationObjectDeadZoneTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_reg_slot_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeRegSlotDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeRegSlotDeadZoneTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArrayDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeSlotArrayDeadZoneTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_global_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeGlobalDeadZoneTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeGlobalDeadZoneTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_global_block_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeGlobalBlockTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeGlobalBlockTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArrayTest.js",
        compile_flags: vec![
            "-dbgbaseline:blockScopeSlotArrayTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_test_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArrayTest.js",
        compile_flags: vec![
            "-forceserialized",
            "-dbgbaseline:blockScopeSlotArrayTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_slot_array_sibling_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSlotArraySiblingTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeSlotArraySiblingTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_global_slot_array_test_bug222631_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeGlobalSlotArrayTest.bug222631.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeGlobalSlotArrayTest.bug222631.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_sibling_bug263635_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSibling.bug263635.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeSibling.bug263635.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_sibling_scope_tracked_in_non_debug_mode_bug321751_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeSiblingScopeTrackedInNonDebugMode.bug321751.js",
        compile_flags: vec![
            "-dbgbaseline:blockScopeSiblingScopeTrackedInNonDebugMode.bug321751.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_reg_slot_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationRegSlotTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionDeclarationRegSlotTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_slot_array_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationSlotArrayTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionDeclarationSlotArrayTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_activation_object_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationActivationObjectTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionDeclarationActivationObjectTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_global_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationGlobalTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionDeclarationGlobalTest.js.dbg.baseline",
            "-Intl-",
            "-InspectMaxStringLength:100",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_global_shadowing_test_bug305562_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationGlobalShadowingTest.bug305562.js",
        compile_flags: vec!["-debuglaunch","-dbgbaseline:blockScopeFunctionDeclarationGlobalShadowingTest.bug305562.js.dbg.baseline","-Intl-"],
        tags: HashSet::from(["exclude_serialized","exclude_snap","require_debugger","exclude_sanitize_address"]),
..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_declaration_global_shadowing_test_bug308191_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionDeclarationGlobalShadowingTest.bug308191.js",
        compile_flags: vec!["-debuglaunch","-InspectMaxStringLength:33","-dbgbaseline:blockScopeFunctionDeclarationGlobalShadowingTest.bug308191.js.dbg.baseline","-Intl-"],
        tags: HashSet::from(["exclude_serialized","exclude_snap","require_debugger","exclude_sanitize_address"]),
..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_redeclaration_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionRedeclarationTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionRedeclarationTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_function_redeclaration_blue523098_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeFunctionRedeclaration_blue523098.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeFunctionRedeclaration_blue523098.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn disablebp_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "disablebp.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:disablebp.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn disablebp2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "disablebp2.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:disablebp2.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn setframe_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "setframe.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:setframe.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn func_expr_crash_150491_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcExprCrash_150491.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:funcExprCrash_150491.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_locals_at_native_frame1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JIT_localsAtNativeFrame1.js",
        compile_flags: vec![
            "-forceNative",
            "-off:simpleJit",
            "-debuglaunch",
            "-dbgbaseline:JIT_localsAtNativeFrame1.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_arm",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(target_arch = "x86_64")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_locals_at_native_frame2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "JIT_localsAtNativeFrame2.js",
        compile_flags: vec![
            "-forceNative",
            "-off:simpleJit",
            "-debuglaunch",
            "-dbgbaseline:JIT_localsAtNativeFrame2.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_arm",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug594394_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug594394.js",
        baseline_path: Some("bug594394.baseline"),
        compile_flags: vec!["-DebugLaunch", "-dbgbaseline:bug594394.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn fast_f12_bobranch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FastF12_BOBranch.js",
        compile_flags: vec![
            "-debuglaunch",
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-dbgbaseline:FastF12_BOBranch.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn negzerotest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negzerotest.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:negzerotest.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn detach_basic_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "detachBasicTest.js",
        compile_flags: vec!["-dbgbaseline:detachBasicTest.js.dbg.baseline", "-Intl-"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn detach_basic_test_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "detachBasicTest.js",
        compile_flags: vec!["-dbgbaseline:detachBasicTest.js.dbg.baseline", "-Intl-"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn testdynamicdetach1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "testdynamicdetach1.js",
        baseline_path: Some("testdynamicdetach1.baseline"),
        compile_flags: vec!["-dbgbaseline:testdynamicdetach1.js.dbg.baseline", "-Intl-"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -nonative -dbgbaseline:jitStepping2.js.dbg.baseline</compile-flags>
//       <files>jitStepping2.js</files>
//       <tags>exclude_dynapogo,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -forcenative -dbgbaseline:jitStepping2.js.dbg.baseline</compile-flags>
//       <files>jitStepping2.js</files>
//       <tags>exclude_dynapogo,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_expr_eval1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit_exprEval1.js",
        compile_flags: vec![
            "-debuglaunch",
            "-es6functionnamefull",
            "-forceNative",
            "-off:simpleJit",
            "-dbgbaseline:jit_exprEval1.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_editvalue1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jit_editvalue1.js",
        compile_flags: vec![
            "-es6functionnamefull",
            "-debuglaunch",
            "-forceNative",
            "-off:simpleJit",
            "-dbgbaseline:jit_editvalue1.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jitAttach.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-dbgbaseline:jitAttach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stringkeyedtypehandler_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringkeyedtypehandler.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:stringkeyedtypehandler.js.dbg.baseline",
            "-DeletedPropertyReuseThreshold:1",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn function_name_shows_in_scope_group_test_bug157127_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functionNameShowsInScopeGroupTest.bug157127.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:functionNameShowsInScopeGroupTest.bug157127.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn function_name_shows_in_nested_scope_group_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "functionNameShowsInNestedScopeGroupTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:functionNameShowsInNestedScopeGroupTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_expression_no_write_of_const_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeExpressionNoWriteOfConst.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeExpressionNoWriteOfConst.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_reg_slot_shadowing_expression_evaluation_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeRegSlotShadowingExpressionEvaluationTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeRegSlotShadowingExpressionEvaluationTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_expression_simple_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeExpressionSimpleDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeExpressionSimpleDeadZoneTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_expression_equation_dead_zone_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeExpressionEquationDeadZoneTest.js",
        compile_flags: vec![
            "-InspectMaxStringLength:33",
            "-debuglaunch",
            "-dbgbaseline:blockScopeExpressionEquationDeadZoneTest.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn block_scope_try_catch_test_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockScopeTryCatchTest.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:blockScopeTryCatchTest.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn locals_inspection_on_non_top_frame_in_block_test_bug163347_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "localsInspectionOnNonTopFrameInBlockTest.bug163347.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:localsInspectionOnNonTopFrameInBlockTest.bug163347.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn jit_attach_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jitAttach.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-dbgbaseline:jitAttach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn getter_inspection_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getterInspection.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:getterInspection.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn promise_defer_nested_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "promise_deferNestedAttach.js",
        compile_flags: vec![
            "-es6functionnamefull",
            "-dbgbaseline:promise_deferNestedAttach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn promise_defer_nested_attach_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "promise_deferNestedAttach.js",
        compile_flags: vec![
            "-ForceStaticInterpreterThunk",
            "-es6functionnamefull",
            "-dbgbaseline:promise_deferNestedAttach.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_222633_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_222633.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_222633.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_149118_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_149118.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:bug_149118.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_149118_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_149118.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:bug_149118.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_204064_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_204064.js",
        compile_flags: vec!["-dbgbaseline:bug_204064.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_177146_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_177146.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:bug_177146.js.dbg.baseline -Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_177146_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_177146.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:bug_177146.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_256729_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_256729.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_256729.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_266843_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_266843.js",
        baseline_path: Some("bug_266843.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-dbgbaseline:bug_266843.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_350674_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_350674.js",
        compile_flags: vec![
            "-debuglaunch",
            "-forceNative",
            "-off:simpleJit",
            "-dbgbaseline:bug_350674.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn with_shadow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with_shadow.js",
        compile_flags: vec![
            "-debuglaunch",
            "-InspectMaxStringLength:100",
            "-dbgbaseline:with_shadow.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn var_shadow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "var_shadow.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:var_shadow.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn arraytoes5array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arraytoes5array.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:arraytoes5array.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn property_enumeration_bug241480_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyEnumeration.bug241480.js",
        compile_flags: vec![
            "-dbgbaseline:propertyEnumeration.bug241480.js.dbg.baseline",
            "-debuglaunch",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_271356_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_271356.js",
        baseline_path: Some("bug_271356.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-maxinterpretcount:4",
            "-off:simpleJit",
            "-dbgbaseline:bug_271356.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_291582_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_291582.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:bug_291582.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_355097_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_355097.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:bug_355097.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_301517_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_301517.js",
        compile_flags: vec!["-dbgbaseline:bug_301517.js.dbg.baseline", "-Intl-"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_325839_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_325839.js",
        baseline_path: Some("bug_325839.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-forceNative",
            "-off:simpleJit",
            "-dbgbaseline:bug_325839.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn defer_parse_210165_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferParse_210165.js",
        baseline_path: Some("deferParse_210165.baseline"),
        compile_flags: vec!["-dbgbaseline:deferParse_210165.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn qualified_names1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "qualified_names1.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:qualified_names1.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn qualified_names2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "qualified_names2.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:qualified_names2.js.dbg.baseline",
            "-Intl-",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn eval_detection_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "evalDetection.js",
        compile_flags: vec!["-dbgbaseline:evalDetection.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_507528_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_507528.js",
        baseline_path: Some("bug_507528.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-Intl-",
            "-dbgbaseline:emptyJson.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_543550_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_543550.js",
        compile_flags: vec![
            "-debuglaunch",
            "-inspectmaxstringlength:110",
            "-dbgbaseline:bug_543550.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_523101_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_523101.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_523101.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn symbols_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "symbols.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:symbols.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn qualified_names5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "qualified_names5.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:qualified_names5.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_538163_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_538163.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_538163.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_575634_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_575634.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_575634.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn nested_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nested_eval.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:nested_eval.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_592506_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_592506.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_592506.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn permanent_arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "permanentArguments.js",
        compile_flags: vec![
            "-forcenative",
            "-debuglaunch",
            "-dbgbaseline:permanentArguments.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn source_info_mismatch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sourceInfoMismatch.js",
        compile_flags: vec![
            "-Force:Deferparse",
            "-ForceUndoDefer",
            "-dbgbaseline:emptyJson.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn spread_debugging_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spread_debugging.js",
        compile_flags: vec![
            "-ES6Spread",
            "-DebugLaunch",
            "-InspectMaxStringLength:100",
            "-dbgbaseline:spread_debugging.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_622304_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_622304.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:bug_622304.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn returnedvaluetests_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnedvaluetests.js",
        compile_flags: vec![
            "-debugLaunch",
            "-dbgbaseline:returnedvaluetests.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>returnedvaluetests1.js</files>
//       <compile-flags>-dbgbaseline:returnedvaluetests1.js.dbg.baseline</compile-flags>
//       <!-- xplat-todo: enable on xplat when Intl is supported on xplat (Microsoft/ChakraCore#2919) -->
//       <tags>exclude_dynapogo,exclude_serialized,exclude_xplat,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>returnedvaluetests2.js</files>
//       <compile-flags>-debugLaunch -dbgbaseline:returnedvaluetests2.js.dbg.baseline</compile-flags>
//       <!-- xplat-todo: enable on xplat when Intl is supported on xplat (Microsoft/ChakraCore#2919) -->
//       <tags>exclude_dynapogo,exclude_serialized,exclude_xplat,exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn delaycapture_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delaycapture.js",
        compile_flags: vec!["-dbgbaseline:delaycapture.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn returnedvaluetests3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnedvaluetests3.js",
        compile_flags: vec![
            "-debugLaunch",
            "-dbgbaseline:returnedvaluetests3.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn returnedvaluetests4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnedvaluetests4.js",
        compile_flags: vec!["-dbgbaseline:returnedvaluetests4.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn returnedvaluetests4_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "returnedvaluetests4.js",
        compile_flags: vec![
            "-force:deferparse",
            "-dbgbaseline:returnedvaluetests4.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_261867_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_261867.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-debuglaunch",
            "-maxinterpretcount:1",
            "-dbgbaseline:bug_261867.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn rest_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rest.js",
        compile_flags: vec![
            "-dbgbaseline:rest.js.dbg.baseline",
            "-ES6Rest",
            "-InspectMaxStringLength:100",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn obj_lit_step_into_more_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLit_step_into_more.js",
        baseline_path: Some("ObjLit_step_into_more.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ObjLit_step_into_more.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn obj_lit_step_into_out_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLit_step_into_out.js",
        baseline_path: Some("ObjLit_step_into_out.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ObjLit_step_into_out.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn obj_lit_step_over_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLit_step_over.js",
        baseline_path: Some("ObjLit_step_over.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ObjLit_step_over.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn generators_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generators.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:generators.js.dbg.baseline",
            "-ES6Generators",
            "-InspectMaxStringLength:200",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn async_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:async.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn async_step_out_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async_step_out.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:async_step_out.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn async_step_over_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "async_step_over.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:async_step_over.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn typed_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypedArray.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:TypedArray.js.dbg.baseline",
            "-InspectMaxStringLength:200",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn computed_property_names_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ComputedPropertyNames.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ComputedPropertyNames.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn parented_dynamic_code2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parentedDynamicCode2.js",
        compile_flags: vec![
            "-InspectMaxStringLength:1000",
            "-dbgbaseline:parentedDynamicCode2.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn parented_dynamic_code3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "parentedDynamicCode3.js",
        compile_flags: vec![
            "-InspectMaxStringLength:1000",
            "-dbgbaseline:parentedDynamicCode3.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os_2946365_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_os_2946365.js",
        compile_flags: vec!["-dbgbaseline:bug_os_2946365.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn console_scope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ConsoleScope.js",
        baseline_path: Some("ConsoleScope.js.baseline"),
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:ConsoleScope.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn console_scope_pmspec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ConsoleScopePMSpec.js",
        baseline_path: Some("ConsoleScopePMSpec.js.baseline"),
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:ConsoleScopePMSpec.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn infiniteloop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "infiniteloop.js",
        compile_flags: vec!["-debuglaunch", "-dbgbaseline:infiniteloop.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn destructuring_debug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring-debug.js",
        compile_flags: vec![
            "-debuglaunch",
            "-InspectMaxStringLength:100",
            "-dbgbaseline:destructuring-debug.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn regex_symbols_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-symbols.js",
        compile_flags: vec![
            "-ES6RegExSymbols",
            "-ES6RegExPrototypeProperties",
            "-debuglaunch",
            "-dbgbaseline:regex-symbols.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn default_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default.js",
        compile_flags: vec!["-debugLaunch", "-dbgbaseline:default.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn default_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default_attach.js",
        compile_flags: vec!["-dbgbaseline:default_attach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_vso5792108_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_vso5792108.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:bug_vso5792108.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn promise_display_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "promiseDisplay.js",
        compile_flags: vec![
            "-debuglaunch",
            "-dbgbaseline:promiseDisplay.js.dbg.baseline",
        ],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os12814968_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS12814968.js",
        compile_flags: vec!["-forcedeferparse"],
        tags: HashSet::from([
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn async_dynamic_attach_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AsyncDynamicAttach.js",
        compile_flags: vec!["-dbgbaseline:AsyncDynamicAttach.js.dbg.baseline"],
        tags: HashSet::from([
            "exclude_dynapogo",
            "exclude_serialized",
            "exclude_snap",
            "require_debugger",
            "exclude_sanitize_address",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
