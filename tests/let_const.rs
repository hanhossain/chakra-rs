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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>d.js</files>
//     <baseline>d.baseline</baseline>
//     <compile-flags>-forceundodefer</compile-flags>
//   </default>
// </test>
// TODO (hanhossain): migrate

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>e.js</files>
//     <baseline>e.baseline</baseline>
//     <compile-flags>-forceundodefer</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>redeclaration.js</files>
//     <baseline>redeclaration.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>AssignmentToConst.js</files>
//     <baseline>AssignmentToConst.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>DeclOutofBlock.js</files>
//     <baseline>DeclOutofBlock.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>defer1.js</files>
//     <baseline>defer1.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>defer2.js</files>
//     <baseline>defer2.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>defer4.js</files>
//     <baseline>defer4.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>defer5.js</files>
//     <baseline>defer5.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>eval1.js</files>
//     <baseline>eval1.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>for-loop.js</files>
//     <baseline>for-loop.baseline</baseline>
//     <compile-flags>-force:deferparse</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>eval_letconst.js</files>
//     <compile-flags>-force:deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>eval_letconst.js</files>
//     <compile-flags>-force:evalcompile</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>eval_letconst.js</files>
//     <compile-flags>-force:deferparse -force:evalcompile</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>arguments.js</files>
//     <baseline>arguments.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>seal.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>seal1.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>seal2.js</files>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>dop.js</files>
//     <baseline>dop.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>dop1.js</files>
//     <baseline>dop1.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>delete.js</files>
//     <baseline>delete.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unassignedconst.js</files>
//     <compile-flags>-Intl-</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unassignedconst.js</files>
//     <compile-flags>-Intl- -force:deferparse</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>letconst_undeclinlinecache.js</files>
//     <compile-flags>-Intl-</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loopinit.js</files>
//     <compile-flags>-loopinterpretcount:1 -bgjit-</compile-flags>
//     <baseline>loopinit.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>letlet.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

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
