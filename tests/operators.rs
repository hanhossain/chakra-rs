mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Operators";

#[test]
fn access_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "access.js",
        baseline_path: Some("access.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn add_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "add.js",
        baseline_path: Some("add.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn addcross_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "addcross.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>biops.js</files>
//     <baseline>biops.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn binop_kills_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "binop-kills.js",
        baseline_path: Some("binop-kills.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete1.js",
        baseline_path: Some("delete1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete2.js",
        baseline_path: Some("delete2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete3.js",
        baseline_path: Some("delete3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn div_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "div.js",
        baseline_path: Some("div.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equals_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equals.js",
        baseline_path: Some("equals.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn instanceof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "instanceof.js",
        baseline_path: Some("instanceof.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inst_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inst_bug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn isin_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isin.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn log_and_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "logAnd.js",
        baseline_path: Some("logAnd.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn log_or_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "logOr.js",
        baseline_path: Some("logOr.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mod_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mod.js",
        baseline_path: Some("mod.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretCount:1 -off:simpleJit -testtrace:rejit</compile-flags>
//     <files>modopt.js</files>
//     <baseline>modopt.baseline</baseline>
//     <tags>exclude_dynapogo,exclude_nonative,exclude_arm,require_backend</tags>
//   </default>
// </test>

#[test]
fn mul_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "mul.js",
        baseline_path: Some("mul.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn op_eq_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OpEq.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn or_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "or.js",
        baseline_path: Some("or.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>property.js</files>
//     <baseline>property.baseline</baseline>
//     <tags>Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn relops_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "relops.js",
        baseline_path: Some("relops.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn strictequal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictequal.js",
        baseline_path: Some("strictequal.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unary_ops_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unaryOps.js",
        baseline_path: Some("unaryOps.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn xor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "xor.js",
        baseline_path: Some("xor.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "new.js",
        baseline_path: Some("new.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_return_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newReturn.js",
        baseline_path: Some("newReturn.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_builtin_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newBuiltin.js",
        baseline_path: Some("newBuiltin.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newProto.js",
        baseline_path: Some("newProto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype_inheritance_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prototypeInheritance.js",
        baseline_path: Some("prototypeInheritance.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype_inheritance2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prototypeInheritance2.js",
        baseline_path: Some("prototypeInheritance2.baseline"),
        compile_flags: vec!["-ES6RegExPrototypeProperties-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn zero_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "zero.js",
        baseline_path: Some("zero.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
