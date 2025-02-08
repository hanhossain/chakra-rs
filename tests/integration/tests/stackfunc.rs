use crate::common;
use crate::common::Variant;
use rstest::rstest;
#[cfg(not(feature = "optimized-tests"))]
use std::collections::HashSet;

const COMMON_TAGS: [&str; 2] = ["exclude_serialized", "require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/stackfunc";

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn simple_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_escape.js",
        baseline_path: Some("simple_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn simple_stackfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_stackfunc.js",
        baseline_path: Some("simple_stackfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simplejit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn simple_stackfunc_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_stackfunc.js",
        baseline_path: Some("simple_stackfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-Off:Deferparse",
            "-on:stackfunc",
            "-nonative",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn trycatch_stackfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trycatch_stackfunc.js",
        baseline_path: Some("trycatch_stackfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
            "-nonative",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn simple_namedstackfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_namedstackfunc.js",
        baseline_path: Some("simple_namedstackfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn to_string_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString_escape.js",
        baseline_path: Some("toString_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn chain_assign_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "chain_assign.js",
        baseline_path: Some("chain_assign.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn nested_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nested_escape.js",
        baseline_path: Some("nested_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn funcname_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcname_escape.js",
        baseline_path: Some("funcname_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn call_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "call_escape.js",
        baseline_path: Some("call_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argout_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argout_escape.js",
        baseline_path: Some("argout_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn throw_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "throw_escape.js",
        baseline_path: Some("throw_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn funcname_asg_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcname_asg.js",
        baseline_path: Some("funcname_asg.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arrlit_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrlit_escape.js",
        baseline_path: Some("arrlit_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arrlit_asg_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrlit_asg_escape.js",
        baseline_path: Some("arrlit_asg_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn objlit_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit_escape.js",
        baseline_path: Some("objlit_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn formal_asg_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_asg.js",
        baseline_path: Some("formal_asg.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn argument_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argument_escape.js",
        baseline_path: Some("argument_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn arguments_assignment_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arguments_assignment.js",
        baseline_path: Some("arguments_assignment.baseline"),
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn cross_scope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cross_scope.js",
        baseline_path: Some("cross_scope.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn eval_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_escape.js",
        baseline_path: Some("eval_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn child_eval_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "child_eval_escape.js",
        baseline_path: Some("child_eval_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn with_namedfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with_namedfunc.js",
        baseline_path: Some("with_namedfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn formal_namedfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_namedfunc.js",
        baseline_path: Some("formal_namedfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn throw_func_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "throw_func.js",
        baseline_path: Some("throw_func.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn glo_asg_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "glo_asg.js",
        baseline_path: Some("glo_asg.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn multinested_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multinested_escape.js",
        baseline_path: Some("multinested_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn let_stackfunc_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "let_stackfunc.js",
        baseline_path: Some("let_stackfunc.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-off:deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn let_blockescape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "let_blockescape.js",
        baseline_path: Some("let_blockescape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-off:deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn simple_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_escape.js",
        baseline_path: Some("simple_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn simple_stackfunc_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_stackfunc.js",
        baseline_path: Some("simple_stackfunc.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn simple_namedstackfunc_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "simple_namedstackfunc.js",
        baseline_path: Some("simple_namedstackfunc.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn to_string_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString_escape.js",
        baseline_path: Some("toString_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn chain_assign_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "chain_assign.js",
        baseline_path: Some("chain_assign.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn nested_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nested_escape.js",
        baseline_path: Some("nested_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-off:DisableStackFuncOnDeferredEscape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn funcname_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcname_escape.js",
        baseline_path: Some("funcname_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>call_escape.js</files>
//       <baseline>call_escape.deferparse.baseline</baseline>
//       <!--Top Level function parsing on first call to script is turned off here, as this tests order of functions executed-->
//       <compile-flags>-DeferTopLevelTillFirstCall- -testtrace:stackfunc -off:redeferral -off:simpleJit -Force:Deferparse -on:stackfunc</compile-flags>
//       <tags>exclude_test,exclude_dynapogo</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>throw_escape.js</files>
//       <baseline>throw_escape.deferparse.baseline</baseline>
//       <!--Top Level function parsing on first call to script is turned off here, as this tests order of functions executed-->
//       <compile-flags>-DeferTopLevelTillFirstCall- -testtrace:stackfunc -off:redeferral -off:simpleJit -Force:Deferparse -on:stackfunc</compile-flags>
//       <tags>exclude_test,exclude_dynapogo</tags>
//     </default>
//   </test>

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn funcname_asg_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcname_asg.js",
        baseline_path: Some("funcname_asg.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn arrlit_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrlit_escape.js",
        baseline_path: Some("arrlit_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn arrlit_asg_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arrlit_asg_escape.js",
        baseline_path: Some("arrlit_asg_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn objlit_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit_escape.js",
        baseline_path: Some("objlit_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn formal_asg_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_asg.js",
        baseline_path: Some("formal_asg.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn argument_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argument_escape.js",
        baseline_path: Some("argument_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn cross_scope_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cross_scope.js",
        baseline_path: Some("cross_scope.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-off:cachescopeinfonames",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn eval_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "eval_escape.js",
        baseline_path: Some("eval_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn child_eval_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "child_eval_escape.js",
        baseline_path: Some("child_eval_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn with_namedfunc_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with_namedfunc.js",
        baseline_path: Some("with_namedfunc.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn formal_namedfunc_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "formal_namedfunc.js",
        baseline_path: Some("formal_namedfunc.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>throw_func.js</files>
//       <baseline>throw_func.deferparse.baseline</baseline>
//       <!--Top Level function parsing on first call to script is turned off here, as this tests order of functions executed-->
//       <compile-flags>-DeferTopLevelTillFirstCall- -testtrace:stackfunc -off:redeferral -off:simpleJit -Force:Deferparse -on:stackfunc</compile-flags>
//       <tags>exclude_test,exclude_dynapogo</tags>
//     </default>
//   </test>

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn glo_asg_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "glo_asg.js",
        baseline_path: Some("glo_asg.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn multinested_escape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multinested_escape.js",
        baseline_path: Some("multinested_escape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-off:cachescopeinfonames",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn let_stackfunc_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "let_stackfunc.js",
        baseline_path: Some("let_stackfunc.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn let_blockescape_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "let_blockescape.js",
        baseline_path: Some("let_blockescape.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn box_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box.js",
        baseline_path: Some("box.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-off:cachescopeinfonames",
            "-on:stackfunc",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn box_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box.js",
        baseline_path: Some("box.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-off:cachescopeinfonames",
            "-force:inline",
            "-on:stackfunc",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn callee_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callee_escape.js",
        baseline_path: Some("callee_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-force:inline",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn callee_escape2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callee_escape2.js",
        baseline_path: Some("callee_escape2.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-force:inline",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn callee_escape2_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "callee_escape2.js",
        baseline_path: Some("callee_escape2.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-force:inline",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn caller_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "caller_escape.js",
        baseline_path: Some("caller_escape.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Off:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn singleuse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "singleuse.js",
        baseline_path: Some("singleuse.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-Force:Deferparse",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn iffuncdecl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "iffuncdecl.js",
        baseline_path: Some("iffuncdecl.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn cachescope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cachescope.js",
        baseline_path: Some("cachescope.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn box_callparam_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_callparam.js",
        baseline_path: Some("box_callparam.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:deferparse",
            "-off:cachescopeinfonames",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn inlinee_box_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlinee_box.js",
        baseline_path: Some("inlinee_box.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:inline",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_nonative",
            "exclude_arm",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn blockscope_funcdecl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope_funcdecl.js",
        baseline_path: Some("blockscope_funcdecl.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn recurse_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "recurse.js",
        baseline_path: Some("recurse.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:deferparse",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn jitdefer_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "jitdefer.js",
        baseline_path: Some("jitdefer.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:deferparse",
            "-off:cachescopeinfonames",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_arm",
            "exclude_nonative",
            "exclude_arm64",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>box_bailout.js</files>
//       <baseline>box_bailout.deferparse.baseline</baseline>
//       <!--Top Level function parsing on first call to script is turned off here, as this tests order of functions executed-->
//       <compile-flags>-DeferTopLevelTillFirstCall- -testtrace:stackfunc -off:redeferral -off:simpleJit -on:stackfunc -force:deferparse -off:disablestackfuncondeferredescape</compile-flags>
//       <tags>exclude_test,exclude_dynapogo,exclude_arm,exclude_nonative,exclude_arm64</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>box_inline_bailout.js</files>
//       <baseline>box_inline_bailout.deferparse.baseline</baseline>
//       <!--Top Level function parsing on first call to script is turned off here, as this tests order of functions executed-->
//       <compile-flags>-DeferTopLevelTillFirstCall- -testtrace:stackfunc -off:redeferral -off:simpleJit -on:stackfunc -force:deferparse -off:disablestackfuncondeferredescape</compile-flags>
//       <tags>exclude_test,exclude_dynapogo,exclude_arm,exclude_nonative,exclude_arm64</tags>
//     </default>
//   </test>

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn withref_delayobjscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "withref_delayobjscope.js",
        baseline_path: Some("withref_delayobjscope.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn funcexpr_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcexpr.js",
        baseline_path: Some("funcexpr.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:deferparse",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn funcexpr_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcexpr_2.js",
        baseline_path: Some("funcexpr_2.deferparse.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
            "-force:deferparse",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn funcexpr_2_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "funcexpr_2.js",
        baseline_path: Some("funcexpr_2.deferparse.native.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-on:stackfunc",
            "-force:deferparse",
            "-forceNative",
            "-off:simpleJit",
            "-off:disablestackfuncondeferredescape",
        ],
        tags: HashSet::from([
            "exclude_test",
            "exclude_dynapogo",
            "exclude_arm",
            "exclude_nonative",
        ]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn with_existing_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with_existing.js",
        baseline_path: Some("with_existing.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn with_crossscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "with_crossscope.js",
        baseline_path: Some("with_crossscope.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn bug565705_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug565705.js",
        baseline_path: Some("bug565705.baseline"),
        compile_flags: vec![
            "-testtrace:stackfunc",
            "-off:redeferral",
            "-off:simpleJit",
            "-on:stackfunc",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn box_postjit_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_postjit.js",
        baseline_path: Some("box_postjit.deferparse.baseline"),
        compile_flags: vec![
            "-off:simplejit",
            "-mic:1",
            "-off:inline",
            "-force:deferparse",
        ],
        tags: HashSet::from(["exclude_test", "exclude_dynapogo", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn box_jitloopbody_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_jitloopbody.js",
        baseline_path: Some("box_jitloopbody.baseline"),
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["exclude_test", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn box_jitloopbody2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_jitloopbody2.js",
        baseline_path: Some("box_jitloopbody2.baseline"),
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["exclude_test", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn box_jitloopbody3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_jitloopbody3.js",
        compile_flags: vec!["-forcejitloopbody", "-force:deferparse", "-bgjit-"],
        tags: HashSet::from(["exclude_test", "exclude_arm"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>602481.js</files>
//       <tags>exclude_dynapogo</tags>
//     </default>
//   </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn test605893_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "605893.js",
        compile_flags: vec!["-force:deferparse", "-force:inline"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(not(feature = "optimized-tests"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn test622043_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "622043.js",
        compile_flags: vec![
            "-force:deferparse",
            "-mic:1",
            "-off:bailonnoprofile",
            "-force:inline",
            "-off:simplejit",
        ],
        tags: HashSet::from(["exclude_dynapogo", "exclude_test"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn delaycapture_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delaycapture.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn closure_1129602_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "closure-1129602.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(all(not(feature = "optimized-tests"), target_arch = "x86_64"))]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
fn box_blockscope_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_blockscope.js",
        baseline_path: Some("box_blockscope.baseline"),
        compile_flags: vec!["-off:simplejit", "-testtrace:stackfunc", "-off:redeferral"],
        tags: HashSet::from(["exclude_test", "exclude_arm", "exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn box_native_emptyframe_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_native_emptyframe.js",
        baseline_path: Some("box_native_emptyframe.baseline"),
        compile_flags: vec!["-lic:1"],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn box_inlinee_frame_in_loop_body_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "box_inlineeFrameInLoopBody.js",
        compile_flags: vec![
            "-forcedeferparse",
            "-forcejitloopbody",
            "-force:rejit",
            "-force:inline",
        ],
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
