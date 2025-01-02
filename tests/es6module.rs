use common::Variant;
use rstest::rstest;

mod common;
const DIRECTORY: &str = "chakracore-cxx/test/es6module";

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>test001.js</files>
//     <baseline/>
//     <compile-flags>-PrintSystemException  -es6module -es6generators -es7asyncawait -ArrayMutationTestSeed:456986689  -maxinterpretcount:1 -maxsimplejitruncount:2 -MinMemOpCount:0 -werexceptionsupport  -bgjit- -loopinterpretcount:1 -off:lossyinttypespec</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>test002.js</files>
//     <baseline/>
//     <compile-flags>-PrintSystemException  -es6module -es6generators -es7asyncawait -ArrayMutationTestSeed:1580332815  -maxinterpretcount:1 -maxsimplejitruncount:2 -MinMemOpCount:0 -werexceptionsupport  -MinSwitchJumpTableSize:3 -MaxLinearStringCaseCount:2 -MaxLinearIntCaseCount:2 -force:fieldcopyprop -force:rejit -UseJITTrampoline- -force:fixdataprops -off:stackargopt -force:atom -oopjit- -force:ScriptFunctionWithInlineCache -off:lossyinttypespec -ForceArrayBTree -off:trackintusage -ForceJITCFGCheck -ForceStaticInterpreterThunk -off:cachedScope -off:DelayCapture -off:fefixedmethods -off:ArrayCheckHoist</compile-flags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn moduletest1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moduletest1.js",
        compile_flags: vec!["-ES6Module"],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn moduletest2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moduletest2.js",
        compile_flags: vec!["-ES6Module"],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_syntax_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-syntax.js",
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-ES6Module",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_syntax1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-syntax1.js",
        compile_flags: vec!["-ES6Module", "-args", "summary", "-endargs"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_functionality_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-functionality.js",
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-ES6Module",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_url_in_error_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moduleUrlInError.js",
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dynamic_module_functionality_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic-module-functionality.js",
        compile_flags: vec![
            "-ES6Module",
            "-ESDynamicImport",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dynamic_module_import_specifier_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic-module-import-specifier.js",
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-ES6Module",
            "-ESDynamicImport",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn dynamic_import_promises_5796_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic_import_promises_5796.js",
        compile_flags: vec!["-ESDynamicImport"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_syntax_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-syntax.js",
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-ES6Module",
            "-force:deferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_syntax1_js_defer_parse(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-syntax1.js",
        compile_flags: vec![
            "-ES6Module",
            "-force:deferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-namespace.js</files>
//     <compile-flags>-ES6Module -Es6ToStringTag -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_drt,exclude_sanitize_address,exclude_noicu</tags>
//   </default>
// </test>

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_bugfixes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-bugfixes.js",
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-ES6Module",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn export_binding_loader_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exportBindingLoader.js",
        compile_flags: vec!["-ES6Module", "-args", "summary", "-endargs"],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test_bug_2645_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test_bug_2645.js",
        compile_flags: vec!["-ES6Module"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os12095746_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS12095746.js",
        baseline_path: Some("bug_OS12095746.baseline"),
        compile_flags: vec![
            "-MuteHostErrorMsg",
            "-IgnoreScriptErrorCode",
            "-TraceHostCallback",
            "-ES6Module",
            "-ESDynamicImport",
        ],
        tags: vec!["exclude_dynapogo", "exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os14562349_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14562349.js",
        compile_flags: vec!["-ESDynamicImport", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_issue_3076_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_3076.js",
        compile_flags: vec!["-force:deferparse", "-ESDynamicImport"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_issue_3257_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_3257.js",
        baseline_path: Some("bug_issue_3257.baseline"),
        compile_flags: vec!["-ESDynamicImport"],
        tags: vec!["exclude_sanitize_address"],
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_issue_5777_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_5777.js",
        compile_flags: vec!["-ESDynamicImport", "-MuteHostErrorMsg"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn export_namespace_as_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "export_namespace_as.js",
        compile_flags: vec![
            "-ESExportNsAs",
            "-MuteHostErrorMsg",
            "-args",
            "summary",
            "-endargs",
        ],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multiple_roots_circular_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multiple-roots-circular.js",
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn esdynamic_import_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "GetModuleNamespace.js",
        compile_flags: vec!["-ESDynamicImport"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn module_load_twice_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-load-twice.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn passmodule_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "passmodule.js",
        compile_flags: vec!["-module"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os17830745_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS17830745.js",
        compile_flags: vec!["-MuteHostErrorMsg"],
        tags: vec!["exclude_sanitize_address"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_os18460517_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS18460517.js",
        baseline_path: Some("bug_OS18460517.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
            "-force:rejit",
        ],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn import_meta_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ImportMeta.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-esimportmeta"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn top_level_await_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "top-level-await.js",
        compile_flags: vec!["-ESDynamicImport", "-ESTopLevelAwait", "-module"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
