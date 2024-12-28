mod common;
const DIRECTORY: &str = "chakracore-cxx/test/LetConst";

#[test]
fn a_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "a.js",
        baseline_path: Some("a.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn b_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "b.js",
        baseline_path: Some("b.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn c_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "c.js",
        baseline_path: Some("c.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn d_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "d.js",
        baseline_path: Some("d.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn d_js_forceundodefer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "d.js",
        baseline_path: Some("d.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn e_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "e.js",
        baseline_path: Some("e.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn e_js_forceundodefer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "e.js",
        baseline_path: Some("e.baseline"),
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn f_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "f.js",
        baseline_path: Some("f.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn g_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "g.js",
        baseline_path: Some("g.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn h_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "h.js",
        baseline_path: Some("h.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn i_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "i.js",
        baseline_path: Some("i.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn j_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "j.js",
        baseline_path: Some("j.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn k_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "k.js",
        baseline_path: Some("k.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn l_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "l.js",
        baseline_path: Some("l.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn m_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "m.js",
        baseline_path: Some("m.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn n_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "n.js",
        baseline_path: Some("n.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn o_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "o.js",
        baseline_path: Some("o.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn p_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "p.js",
        baseline_path: Some("p.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn q_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "q.js",
        baseline_path: Some("q.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn redeclaration_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redeclaration.js",
        baseline_path: Some("redeclaration.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn redeclaration_js_forcedeferparse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redeclaration.js",
        baseline_path: Some("redeclaration.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn r_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "r.js",
        baseline_path: Some("r.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn assignment_to_const_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AssignmentToConst.js",
        baseline_path: Some("AssignmentToConst.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn assignment_to_const_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "AssignmentToConst.js",
        baseline_path: Some("AssignmentToConst.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn decl_outof_block_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeclOutofBlock.js",
        baseline_path: Some("DeclOutofBlock.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn decl_out_of_block_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeclOutofBlock.js",
        baseline_path: Some("DeclOutofBlock.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer1.js",
        baseline_path: Some("defer1.baseline"),
        compile_flags: vec!["-force:deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn defer2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer2.js",
        baseline_path: Some("defer2.baseline"),
        compile_flags: vec!["-force:deferparse"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn defer3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer3.js",
        baseline_path: Some("defer3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer4.js",
        baseline_path: Some("defer4.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defer5.js",
        baseline_path: Some("defer5.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tdz1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tdz1.js",
        baseline_path: Some("tdz1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tdz2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tdz2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval1.js",
        baseline_path: Some("eval1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval1_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval1.js",
        baseline_path: Some("eval1.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn scopegen1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "scopegen1.js",
        baseline_path: Some("scopegen1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constreassign1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constreassign1.js",
        baseline_path: Some("constreassign1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mixedscope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mixedscope.js",
        baseline_path: Some("mixedscope.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn for_loop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for-loop.js",
        baseline_path: Some("for-loop.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn for_loop_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "for-loop.js",
        baseline_path: Some("for-loop.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letvar_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letvar.js",
        baseline_path: Some("letvar.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_letconst_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_letconst_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:deferparse"],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_letconst_js_force_eval_compile() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:evalcompile"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_letconst_js_force_eval_compile_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_letconst.js",
        compile_flags: vec!["-force:deferparse", "-force:evalcompile"],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments.js",
        baseline_path: Some("arguments.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn seal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal.js",
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn seal1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal1.js",
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn seal2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal2.js",
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn dop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dop.js",
        baseline_path: Some("dop.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn dop1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dop1.js",
        baseline_path: Some("dop1.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete.js",
        baseline_path: Some("delete.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_fncdecl_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_fncdecl.js",
        baseline_path: Some("eval_fncdecl.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn storeundecl_multiscript_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "storeundecl_multiscript.js",
        baseline_path: Some("storeundecl_multiscript.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn storeundecl_eval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "storeundecl_eval.js",
        baseline_path: Some("storeundecl_eval.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with.js",
        baseline_path: Some("with.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unassignedconst_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unassignedconst.js",
        compile_flags: vec!["-Intl-"],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unassignedconst_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unassignedconst.js",
        compile_flags: vec!["-Intl-", "-force:deferparse"],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_undeclinlinecache_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_undeclinlinecache.js",
        compile_flags: vec!["-Intl-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn loopinit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "loopinit.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        baseline_path: Some("loopinit.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letlet_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letlet.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn shadowedsetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "shadowedsetter.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_decl_conflict_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcDeclConflict.js",
        ..Default::default()
    };
    common::run_test(&test);
}
