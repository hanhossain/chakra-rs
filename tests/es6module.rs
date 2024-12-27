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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>moduletest1.js</files>
//     <compile-flags>-ES6Module</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>moduletest2.js</files>
//     <compile-flags>-ES6Module</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-syntax.js</files>
//     <compile-flags>-MuteHostErrorMsg -ES6Module -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-syntax1.js</files>
//     <compile-flags>-ES6Module -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-functionality.js</files>
//     <compile-flags>-MuteHostErrorMsg -ES6Module -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>moduleUrlInError.js</files>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>dynamic-module-functionality.js</files>
//       <compile-flags>-ES6Module -ESDynamicImport -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>dynamic-module-import-specifier.js</files>
//       <compile-flags>-MuteHostErrorMsg -ES6Module -ESDynamicImport -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn dynamic_import_promises_5796_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dynamic_import_promises_5796.js",
        compile_flags: vec!["-ESDynamicImport"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-syntax.js</files>
//     <compile-flags>-MuteHostErrorMsg -ES6Module -force:deferparse -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-syntax1.js</files>
//     <compile-flags>-ES6Module -force:deferparse -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-namespace.js</files>
//     <compile-flags>-ES6Module -Es6ToStringTag -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_drt,exclude_sanitize_address,exclude_noicu</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>module-bugfixes.js</files>
//     <compile-flags>-MuteHostErrorMsg -ES6Module -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>exportBindingLoader.js</files>
//     <compile-flags>-ES6Module -args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>test_bug_2645.js</files>
//     <compile-flags>-ES6Module</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS12095746.js</files>
//       <compile-flags>-MuteHostErrorMsg -IgnoreScriptErrorCode -TraceHostCallback -ES6Module -ESDynamicImport</compile-flags>
//     <tags>exclude_dynapogo,exclude_sanitize_address,exclude_drt</tags>
//     <baseline>bug_OS12095746.baseline</baseline>
//   </default>
// </test>

#[test]
fn bug_os14562349_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14562349.js",
        compile_flags: vec!["-ESDynamicImport", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_issue_3076.js</files>
//   <compile-flags>-force:deferparse -ESDynamicImport</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_issue_3257.js</files>
//     <compile-flags>-ESDynamicImport</compile-flags>
//     <baseline>bug_issue_3257.baseline</baseline>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_issue_5777.js</files>
//     <compile-flags>-ESDynamicImport -MuteHostErrorMsg</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>export_namespace_as.js</files>
//     <compile-flags>-ESExportNsAs -MuteHostErrorMsg -args summary -endargs</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>
//   <test>
//   <default>
//     <files>multiple-roots-circular.js</files>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>GetModuleNamespace.js</files>
//     <compile-flags>-ESDynamicImport</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn module_load_twice_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "module-load-twice.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn passmodule_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "passmodule.js",
        compile_flags: vec!["-module"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS17830745.js</files>
//     <compile-flags>-MuteHostErrorMsg</compile-flags>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn bug_os18460517_js() {
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
    common::run_test(&test);
}

#[test]
fn import_meta_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ImportMeta.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-esimportmeta"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>top-level-await.js</files>
//     <compile-flags>-ESDynamicImport -ESTopLevelAwait -module</compile-flags>
//   </default>
// </test>
