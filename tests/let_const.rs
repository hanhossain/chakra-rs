mod common;

#[test]
fn a_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/a.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/a.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn b_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/b.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/b.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn c_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/c.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/c.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn d_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/d.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/d.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/e.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/e.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/f.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/f.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn g_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/g.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/g.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn h_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/h.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/h.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn i_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/i.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/i.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn j_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/j.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/j.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn k_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/k.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/k.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn l_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/l.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/l.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn m_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/m.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/m.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn n_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/n.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/n.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn o_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/o.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/o.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn p_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/p.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/p.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn q_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/q.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/q.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn redeclaration_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/redeclaration.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/redeclaration.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/r.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/r.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn assignment_to_const_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/AssignmentToConst.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/AssignmentToConst.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/DeclOutofBlock.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/DeclOutofBlock.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/defer3.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/defer3.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/tdz1.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/tdz1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tdz2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/tdz2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/eval1.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/eval1.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/scopegen1.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/scopegen1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constreassign1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/constreassign1.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/constreassign1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mixedscope_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/mixedscope.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/mixedscope.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn for_loop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/for-loop.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/for-loop.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/letvar.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/letvar.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn eval_letconst_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/eval_letconst.js",
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
        source_path: "chakracore-cxx/test/LetConst/eval_fncdecl.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/eval_fncdecl.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn storeundecl_multiscript_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/storeundecl_multiscript.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/storeundecl_multiscript.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn storeundecl_eval_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/storeundecl_eval.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/storeundecl_eval.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn with_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/with.js",
        baseline_path: Some("chakracore-cxx/test/LetConst/with.baseline"),
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
        source_path: "chakracore-cxx/test/LetConst/shadowedsetter.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn func_decl_conflict_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/LetConst/funcDeclConflict.js",
        ..Default::default()
    };
    common::run_test(&test);
}
