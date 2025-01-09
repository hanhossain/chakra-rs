use common::Variant;
use rstest::rstest;
use std::collections::HashSet;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/LetConst";

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn a_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "a.js",
        baseline_path: Some("a.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn b_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "b.js",
        baseline_path: Some("b.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn c_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "c.js",
        baseline_path: Some("c.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn d_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "d.js",
        baseline_path: Some("d.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn d_js_forceundodefer(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "d.js",
        baseline_path: Some("d.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn e_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "e.js",
        baseline_path: Some("e.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn e_js_forceundodefer(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "e.js",
        baseline_path: Some("e.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn f_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "f.js",
        baseline_path: Some("f.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn g_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "g.js",
        baseline_path: Some("g.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn h_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "h.js",
        baseline_path: Some("h.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn i_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "i.js",
        baseline_path: Some("i.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn j_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "j.js",
        baseline_path: Some("j.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn k_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "k.js",
        baseline_path: Some("k.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn l_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "l.js",
        baseline_path: Some("l.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn m_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "m.js",
        baseline_path: Some("m.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn n_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "n.js",
        baseline_path: Some("n.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn o_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "o.js",
        baseline_path: Some("o.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn p_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "p.js",
        baseline_path: Some("p.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn q_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "q.js",
        baseline_path: Some("q.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn redeclaration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redeclaration.js",
        baseline_path: Some("redeclaration.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn redeclaration_js_forcedeferparse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redeclaration.js",
        baseline_path: Some("redeclaration.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn r_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "r.js",
        baseline_path: Some("r.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn assignment_to_const_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AssignmentToConst.js",
        baseline_path: Some("AssignmentToConst.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn assignment_to_const_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AssignmentToConst.js",
        baseline_path: Some("AssignmentToConst.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn decl_outof_block_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeclOutofBlock.js",
        baseline_path: Some("DeclOutofBlock.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn decl_out_of_block_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeclOutofBlock.js",
        baseline_path: Some("DeclOutofBlock.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn defer1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer1.js",
        baseline_path: Some("defer1.baseline"),
        compile_flags: vec!["-force:deferparse"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn defer2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer2.js",
        baseline_path: Some("defer2.baseline"),
        compile_flags: vec!["-force:deferparse"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn defer3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer3.js",
        baseline_path: Some("defer3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn defer4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer4.js",
        baseline_path: Some("defer4.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn defer5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer5.js",
        baseline_path: Some("defer5.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn tdz1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tdz1.js",
        baseline_path: Some("tdz1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn tdz2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tdz2.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval1.js",
        baseline_path: Some("eval1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval1_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval1.js",
        baseline_path: Some("eval1.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn scopegen1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scopegen1.js",
        baseline_path: Some("scopegen1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn constreassign1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constreassign1.js",
        baseline_path: Some("constreassign1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn mixedscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mixedscope.js",
        baseline_path: Some("mixedscope.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn for_loop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for-loop.js",
        baseline_path: Some("for-loop.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn for_loop_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for-loop.js",
        baseline_path: Some("for-loop.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn letvar_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letvar.js",
        baseline_path: Some("letvar.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval_letconst_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval_letconst_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:deferparse"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval_letconst_js_force_eval_compile(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:evalcompile"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval_letconst_js_force_eval_compile_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:deferparse", "-force:evalcompile"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn arguments_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.js",
        baseline_path: Some("arguments.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn seal_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal.js",
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn seal1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal1.js",
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn seal2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal2.js",
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dop.js",
        baseline_path: Some("dop.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn dop1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dop1.js",
        baseline_path: Some("dop1.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete.js",
        baseline_path: Some("delete.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn eval_fncdecl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_fncdecl.js",
        baseline_path: Some("eval_fncdecl.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn storeundecl_multiscript_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "storeundecl_multiscript.js",
        baseline_path: Some("storeundecl_multiscript.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn storeundecl_eval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "storeundecl_eval.js",
        baseline_path: Some("storeundecl_eval.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn with_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with.js",
        baseline_path: Some("with.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn unassignedconst_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unassignedconst.js",
        compile_flags: vec!["-Intl-"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn unassignedconst_js_force_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unassignedconst.js",
        compile_flags: vec!["-Intl-", "-force:deferparse"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn letconst_undeclinlinecache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_undeclinlinecache.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn loopinit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinit.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        baseline_path: Some("loopinit.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn letlet_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letlet.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn shadowedsetter_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadowedsetter.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[cfg_attr(not(disable_jit), case::interpreted(Variant::Interpreted))]
#[cfg_attr(not(disable_jit), case::dynapogo(Variant::Dynapogo))]
#[cfg_attr(disable_jit, case::disable_jit(Variant::DisableJit))]
fn func_decl_conflict_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcDeclConflict.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
