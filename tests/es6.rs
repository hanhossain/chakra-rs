mod common;
const DIRECTORY: &str = "chakracore-cxx/test/es6";

#[test]
fn instance_of_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "instanceOfProto.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_issue_2747_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_2747.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lambda1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lambda1.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lambda_expr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lambda-expr.js",
        baseline_path: Some("lambda-expr.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lambda_params_shadow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "lambda-params-shadow.js",
        compile_flags: vec!["-off:deferparse", "-es7asyncawait"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn numeric_literal_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumericLiteralTest.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bound_bug3837520_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boundBug3837520.js",
        compile_flags: vec!["-es6generators", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6to_length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6toLength.js",
        compile_flags: vec!["-es6toLength", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6to_length_js_jsbuiltin() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6toLength.js",
        compile_flags: vec![
            "-es6toLength",
            "-JsBuiltIn-",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_primitive_cross_script_test_case_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toPrimitiveCrossScriptTestCase.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn computed_property_to_string_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "computedPropertyToString.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn computed_property_side_effect_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "computedPropertySideEffect.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_fix2214646_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BugFix2214646.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6is_concat_spreadable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6IsConcatSpreadable.js",
        compile_flags: vec!["-es6tolength", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_primitive_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toPrimitive.js",
        compile_flags: vec!["-es6regexsymbols", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unscopables_with_scope_test_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unscopablesWithScopeTest.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn function_name_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "function.name.js",
        compile_flags: vec![
            "-ES6Generators",
            "-es6functionnamefull",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>function.name.js</files>
//     <compile-flags>-ES6Generators -es6functionnamefull -force:deferparse -args summary -endargs</compile-flags>
//     <timeout>120</timeout> <!-- ARM64 take more than 60 -->
//   </default>
// </test>

#[test]
fn super_dot_osbug3930962_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "superDotOSBug3930962.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>toStringTag.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn proto_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proto_basic.js",
        baseline_path: Some("proto_basic.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_disable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proto_disable.js",
        baseline_path: Some("proto_disable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_initializer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proto_initializer.js",
        baseline_path: Some("proto_initializer.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>proto_initializer.js</files>
//     <baseline>proto_initializer.baseline</baseline>
//     <compile-flags>-ForceSerialized</compile-flags>
//     <tags>exclude_serialized</tags>
//   </default>
// </test>

#[test]
fn proto_addprop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proto_addprop.js",
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_addprop_js_maxinterpretcount2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proto_addprop.js",
        compile_flags: vec!["-maxinterpretcount:2", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn map_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "map_basic.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn map_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "map_functionality.js",
        compile_flags: vec!["-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "set_basic.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "set_functionality.js",
        compile_flags: vec!["-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn weakmap_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weakmap_basic.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn weakmap_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weakmap_functionality.js",
        compile_flags: vec!["-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn weakset_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weakset_basic.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn weakset_functionality_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "weakset_functionality.js",
        compile_flags: vec!["-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blockscope_activationobject_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope-activationobject.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blockscope_deferred_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope-deferred.js",
        baseline_path: Some("blockscope-deferred.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blockscope_deferred_js_deferparse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope-deferred.js",
        baseline_path: Some("blockscope-deferred.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blockscope_functionbinding_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope-functionbinding.js",
        baseline_path: Some("blockscope-functionbinding.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blockscope_functionbinding_js_deferparse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blockscope-functionbinding.js",
        baseline_path: Some("blockscope-functionbinding.baseline"),
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>blockscope-functionbinding.js</files>
//     <compile-flags>-lic:1 -InitializeInterpreterSlotsWithInvalidStackVar</compile-flags>
//     <baseline>blockscope-functionbinding.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn letconst_global_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global.js",
        baseline_path: Some("letconst_global.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_shadowing_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_shadowing.js",
        baseline_path: Some("letconst_global_shadowing.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_shadow_builtins_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_shadow_builtins.js",
        baseline_path: Some("letconst_global_shadow_builtins.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_shadow_builtins_nonconfigurable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_shadow_builtins_nonconfigurable.js",
        baseline_path: Some("letconst_global_shadow_builtins_nonconfigurable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_shadow_deleted_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_shadow_deleted.js",
        baseline_path: Some("letconst_global_shadow_deleted.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_shadow_accessor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_shadow_accessor.js",
        baseline_path: Some("letconst_global_shadow_accessor.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_global_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_global_bug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_eval_redecl_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_eval_redecl.js",
        baseline_path: Some("letconst_eval_redecl.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn letconst_eval_redecl_js_forcestrictmode() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "letconst_eval_redecl.js",
        baseline_path: Some("letconst_eval_redecl.strictmode.baseline"),
        compile_flags: vec!["-forcestrictmode"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn definegettersetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "definegettersetter.js",
        baseline_path: Some("definegettersetter.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn classes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classes.js",
        compile_flags: vec!["-Off:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn classes_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classes.js",
        compile_flags: vec!["-Force:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>classes_bugfixes.js</files>
//     <compile-flags>-Off:Deferparse -args summary -endargs</compile-flags>
//     <tags>exclude_noicu</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>classes_bugfixes.js</files>
//     <compile-flags>-Force:Deferparse -args summary -endargs</compile-flags>
//     <tags>exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn es6super_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Super.js",
        compile_flags: vec![
            "-nonative",
            "-off:dynamicprofile",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6super_js_defer_parse_off() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Super.js",
        compile_flags: vec!["-Off:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6super_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Super.js",
        compile_flags: vec!["-Force:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn classes_bug_os_6471427_js_force_serialized() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classes_bug_OS_6471427.js",
        compile_flags: vec!["-ForceSerialized"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn classes_bug_os_6471427_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classes_bug_OS_6471427.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn classes_bug_os_7100885_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "classes_bug_OS_7100885.js",
        compile_flags: vec!["-maxsimplejitruncount:4", "-maxinterpretcount:4"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6subclassable_builtins_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6SubclassableBuiltins.js",
        compile_flags: vec![
            "-ES6Spread",
            "-ES6Generators",
            "-ESSharedArrayBuffer",
            "-Off:Deferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6subclassable_builtins_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6SubclassableBuiltins.js",
        compile_flags: vec![
            "-ES6Spread",
            "-ES6Generators",
            "-ESSharedArrayBuffer",
            "-Force:Deferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6subclassable_async_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6SubclassableAsync.js",
        baseline_path: Some("ES6SubclassableAsync.baseline"),
        compile_flags: vec!["-ES6Spread", "-ES6Generators", "-Off:Deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6subclassable_async_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6SubclassableAsync.js",
        baseline_path: Some("ES6SubclassableAsync.baseline"),
        compile_flags: vec!["-ES6Spread", "-ES6Generators", "-Force:Deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6math_apis_js_es6_math() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6MathAPIs.js",
        compile_flags: vec!["-ES6Math", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6math_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6MathAPIs.js",
        compile_flags: vec!["-ES6", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6number_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6NumberAPIs.js",
        compile_flags: vec!["-ES6Number", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6string_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6StringAPIs.js",
        compile_flags: vec![
            "-ES6String",
            "-ES6RegExSymbols",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn code_point_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "codePointAt.js",
        baseline_path: Some("codePointAt.baseline"),
        compile_flags: vec!["-ES6Unicode"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stringtemplate_basic_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stringtemplate_basic.js",
        baseline_path: Some("stringtemplate_basic.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6string_template_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6StringTemplate.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6string_template_slow_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6StringTemplateSlow.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6typed_array_extensions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6TypedArrayExtensions.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-ES6ObjectLiterals",
            "-ESSharedArrayBuffer",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6array_api_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6ArrayAPI.js",
        compile_flags: vec!["-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6array_use_constructor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6ArrayUseConstructor.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6array_use_constructor_v5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6ArrayUseConstructor_v5.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6symbol_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Symbol.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-ES6ObjectLiterals",
            "-ES6RegExSymbols",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ES6Symbol_540238.js</files>
//     <compile-flags>-RecyclerStress</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn es6promise_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Promise.js",
        compile_flags: vec!["-ES6", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6promise_async_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6PromiseAsync.js",
        baseline_path: Some("ES6PromiseAsync.baseline"),
        compile_flags: vec!["-ES6"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>normalize.js</files>
//     <baseline>normalize.baseline</baseline>
//     <tags>exclude_noicu</tags>
//     <compile-flags> -ES6Unicode</compile-flags>
//   </default>
// </test>

#[test]
fn normalize_prop_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "normalizeProp.js",
        baseline_path: Some("normalizeProp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unicode_escape_sequences_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_escape_sequences.js",
        baseline_path: Some("unicode_escape_sequences.baseline"),
        compile_flags: vec!["-ES6Unicode", "-ES6RegExSticky"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_6_identifiers_utf8.js</files>
//     <baseline>unicode_6_identifiers_utf8.baseline</baseline>
//     <compile-flags> -ES6Unicode</compile-flags>
//     <tags>exclude_win7,exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn unicode_regex_surrogate_atoms_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_regex_surrogate_atoms.js",
        baseline_path: Some("unicode_regex_surrogate_atoms.baseline"),
        compile_flags: vec!["-ES6Unicode", "-ES6RegExSticky"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn spread_iterator_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spreadIterator.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reflect_construct_consume_new_target_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "reflectConstructConsumeNewTarget.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reflect_api_tests_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ReflectApiTests.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxy_trap_consume_new_target_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyTrapConsumeNewTarget.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bound_construction_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "boundConstruction.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cross_context_spreadfunction_call_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CrossContextSpreadfunctionCall.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cross_context_promise_file1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CrossContextPromiseFile1.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cross_context_promise_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CrossContextPromise.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn spread_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "spread.js",
        compile_flags: vec!["-ES6Spread", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_convertUTF8.js</files>
//     <compile-flags> -ES6Unicode</compile-flags>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn bug517864_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Bug517864.js",
        compile_flags: vec!["-force:deferparse", "-ES6Unicode"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug620694.js</files>
//     <compile-flags>-es6all -recyclerstress</compile-flags>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_6_identifier_Blue511452.js</files>
//     <baseline>unicode_6_identifier_Blue511452.baseline</baseline>
//     <compile-flags> -ES6Unicode</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_idDeferParseFunctions_utf8.js</files>
//     <compile-flags>-ES6Unicode</compile-flags>
//     <tags>exclude_noicu</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_6_identifier_Blue524737.js</files>
//     <baseline>unicode_6_identifier_Blue524737.baseline</baseline>
//     <compile-flags> -ES6Unicode</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>supersyntax02.js</files>
//     <baseline>supersyntax02.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>supersyntax05.js</files>
//     <baseline>supersyntax05.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>supersyntax06.js</files>
//     <baseline>supersyntax06.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn objlit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit.js",
        compile_flags: vec!["-ES6", "-ES6ObjectLiterals", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>unicode_regex_surrogate_utf8.js</files>
//     <baseline>unicode_regex_surrogate_utf8.baseline</baseline>
//     <compile-flags> -ES6Unicode -ES6RegExSticky</compile-flags>
//     <tags>Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn unicode_blue_533163_utf8_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_blue_533163_utf8.js",
        baseline_path: Some("unicode_blue_533163_utf8.baseline"),
        compile_flags: vec!["-ES6Unicode"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6iterators_forof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Iterators-forof.js",
        compile_flags: vec!["-ES6", "-Intl-", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6iterators_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Iterators-apis.js",
        compile_flags: vec!["-ES6", "-Intl-", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6species_apis_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Species-apis.js",
        compile_flags: vec!["-ES6", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6species_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Species-bugs.js",
        compile_flags: vec!["-CollectGarbage", "-ES6", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn blue595539_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "blue595539.js",
        compile_flags: vec!["-ES6Unicode"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxytest6_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxytest6.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxy_properties_api_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ProxyPropertiesAPI.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxybugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxybugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxyconstruction_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyconstruction.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxybug3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxybug3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxyprotobug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyprotobug.js",
        baseline_path: Some("proxyprotobug.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxybug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxybug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxy_call_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ProxyCall.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxyenumremoval_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxyenumremoval.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxy_issue884_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxy-issue884.js",
        baseline_path: Some("proxy-issue884.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn null_property_descriptor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "nullPropertyDescriptor.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_is_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "object-is.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_assign_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "object-assign.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_js_off_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_js_force_cached_scope() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default.js",
        compile_flags: vec!["-force:CachedScope", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_splitscope_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default-splitscope.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_splitscope_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default-splitscope.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_splitscope_undodeferparse_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default-splitscope-undodeferparse.js",
        compile_flags: vec!["-forceundodefer"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn default_splitscope_serialized_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "default-splitscope-serialized.js",
        compile_flags: vec!["-forceserialized"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn rest_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rest.js",
        compile_flags: vec![
            "-ES6Rest",
            "-ES6ObjectLiterals",
            "-ES6Spread",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn rest_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rest.js",
        compile_flags: vec![
            "-ES6Rest",
            "-ES6ObjectLiterals",
            "-ES6Spread",
            "-force:deferparse",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-syntax.js</files>
//     <compile-flags>-ES6Generators -args summary -endargs</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-syntax.js</files>
//     <compile-flags>-ES6Generators -force:deferparse -args summary -endargs</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

#[test]
fn for_in_edge_cases_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forInEdgeCases.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-deferparse.js</files>
//     <compile-flags>-force:deferparse -ES6Generators</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-apis.js</files>
//     <compile-flags>-ES6Generators -args summary -endargs</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-functionality.js</files>
//     <compile-flags>-ES6Generators -args summary -endargs</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-deferred.js</files>
//     <compile-flags>-ES6Generators -force:deferparse</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-deferred.js</files>
//     <compile-flags>-ES6Generators -serialized</compile-flags>
//     <tags>exclude_arm,exclude_forceserialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-undodefer.js</files>
//     <compile-flags>-ES6Generators -force:deferparse -forceundodefer</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-cachedscope.js</files>
//     <compile-flags>-ES6Generators</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-applyargs.js</files>
//     <!-- -off:inlineapply triggers the use of the ApplyArgs bytecode -->
//     <compile-flags>-ES6Generators -off:inlineapply</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>generators-applyargs.js</files>
//     <compile-flags>-ES6Generators</compile-flags>
//     <tags>exclude_arm</tags>
//   </default>
// </test>

#[test]
fn destructuring_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring.js",
        compile_flags: vec!["-ES6Rest", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_obj_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_obj.js",
        compile_flags: vec!["-ES6Rest", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_params_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_params.js",
        compile_flags: vec!["-ES6Rest", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_params_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_params.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_params_arguments_override_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_params_arguments_override.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_catch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_catch.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn destructuring_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "destructuring_bugs.js",
        compile_flags: vec!["-ForceDeferParse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_279376_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_279376.js",
        compile_flags: vec!["-es6spread"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>blue_641922.js</files>
//     <baseline>blue_641922.baseline</baseline>
//     <compile-flags> -ES6Rest -RecyclerNoPageReuse -PageHeap:2</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn bug_981217_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_981217.js",
        baseline_path: Some("bug_981217.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objlit_shorthand_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit-shorthand-error.js",
        baseline_path: Some("objlit-shorthand-error.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn generator_strict_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "generator-strict-error.js",
        baseline_path: Some("generator-strict-error.baseline"),
        compile_flags: vec!["-ES6Generators", "-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_annex_b_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-annex-b.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_case_folding_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-case-folding.js",
        compile_flags: vec![
            "-version:6",
            "-ES6RegExSticky",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>regex-octoquad.js</files>
//     <compile-flags>-RegexOptimize -args summary -endargs</compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn regex_quantifiers_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-quantifiers.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_code_point_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-code-point.js",
        compile_flags: vec!["-ES6RegExSticky", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_unicode_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-unicode.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_unicode_case_insensitive_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-unicode-CaseInsensitive.js",
        ..Default::default()
    };
    common::run_test(&test);
}
// <!--
//   The following two tests take close to 60 seconds in interpreted mode on OS X test machines,
//   so increase the timeout and mark the test as Slow, since there is no need to include this test in non-Slow:
//   all of the important cases are covered in regex-unicode-CaseInsensitive.js
// -->

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>regex-unicode-CaseInsensitive-all-i.js</files>
//     <tags>Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>regex-unicode-CaseInsensitive-all-iu.js</files>
//     <tags>Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn regex_set_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-set.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_prototype_properties_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-prototype-properties.js",
        compile_flags: vec![
            "-ES6RegExPrototypeProperties",
            "-ES6PrototypeChain",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_symbols_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-symbols.js",
        compile_flags: vec![
            "-ES6RegExSymbols",
            "-ES6RegExPrototypeProperties",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_w_sharp_s_kelvin_sign_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex-w-sharp-s-kelvin-sign.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regexflags_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regexflags.js",
        compile_flags: vec![
            "-version:6",
            "-ES6RegExSticky",
            "-ES6Unicode",
            "-ES6RegExPrototypeProperties",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regexflags_disabled_features_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regexflags-disabled-features.js",
        compile_flags: vec![
            "-ES2018RegExDotAll-",
            "-ES6RegExSticky-",
            "-ES6Unicode-",
            "-ES6RegExPrototypeProperties",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reg_exp_apis_test_with_sticky_flag_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "RegExpApisTestWithStickyFlag.js",
        compile_flags: vec![
            "-version:6",
            "-ES6RegExSticky",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stickyflag_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stickyflag.js",
        baseline_path: Some("stickyflag.baseline"),
        compile_flags: vec![
            "-version:6",
            "-ES6RegExSticky",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>utfbug.js</files>
//     <baseline />
//     <compile-flags>-dump:bytecode</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn proxybug_with_ld_fld_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxybugWithLdFld.js",
        compile_flags: vec![
            "-maxsimplejitruncount:2",
            "-maxinterpretcount:1",
            "-forcejitloopbody",
            "-off:bailonnoprofile",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxybug_withproto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxybugWithproto.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ProxyInProxy.js</files>
//     <baseline>ProxyInProxy.baseline</baseline>
//     <compile-flags> -mic:1 -off:simpleJIT </compile-flags>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn proxy_set_prototype_of_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ProxySetPrototypeOf.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arraywithproxy_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arraywithproxy.js",
        baseline_path: Some("arraywithproxy.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxytest8_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxytest8.js",
        baseline_path: Some("proxytest8.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proxytest9_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "proxytest9.js",
        baseline_path: Some("proxytest9.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ES6Function_bugs.js</files>
//     <tags>exclude_nonrazzle</tags>
//     <compile-flags>-ES6 -args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn os_2700778_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS_2700778.js",
        compile_flags: vec![
            "-loopinterpretcount:1",
            "-bgjit-",
            "-maxinterpretcount:3",
            "-off:simplejit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_2184795_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2184795.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn unicode_whitespace_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "unicode_whitespace.js",
        compile_flags: vec!["-ES6", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS_2553885.js</files>
//     <tags>exclude_win7,Intl,exclude_noicu</tags>
//   </default>
// </test>

#[test]
fn bug_os_2915477_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_2915477.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os3198161_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_os3198161.js",
        compile_flags: vec!["-es6all"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_4498031_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_4498031.js",
        compile_flags: vec![
            "-MaxinterpretCount:1",
            "-MaxSimpleJITRunCount:1",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6new_target_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6NewTarget.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-es6generators"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6new_target_bugfixes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6NewTarget_bugfixes.js",
        compile_flags: vec!["-Off:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6new_target_bugfixes_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6NewTarget_bugfixes.js",
        compile_flags: vec!["-Force:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6new_target_bugfixes_js_force_undo_defer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6NewTarget_bugfixes.js",
        compile_flags: vec!["-forceundodefer", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6class_super_chain_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Class_SuperChain.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-es6generators"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>globalLambdaNewTargetSyntaxError.js</files>
//     <baseline>globalLambdaNewTargetSyntaxError.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>globalNewTargetSyntaxError.js</files>
//     <baseline>globalNewTargetSyntaxError.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>globalCatchNewTargetSyntaxError.js</files>
//     <baseline>globalCatchNewTargetSyntaxError.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>globalParamCatchNewTargetSyntaxError.js</files>
//     <baseline>globalParamCatchNewTargetSyntaxError.baseline</baseline>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn es6class_base_class_construction_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Class_BaseClassConstruction.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-es6generators"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn expo_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "expo.js",
        compile_flags: vec!["-args", "summary", "-endargs", "-es7exponentiationoperator"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn trailingcomma_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "trailingcomma.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6has_instance_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es6HasInstance.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6restricted_properties_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6RestrictedProperties.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es6proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ES6Proto.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_literal_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "object_literal_bug.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>OS_5403724.js</files>
//     <tags>exclude_dynapogo</tags>
//     <compile-flags>-maxinterpretcount:3 -off:simpleJit -ES6 -args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn forloops_per_iteration_bindings_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forloops-per-iteration-bindings.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn htmlcomments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HTMLComments.js",
        baseline_path: Some("HTMLComments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>await-futreserved-only-in-modules.js</files>
//     <compile-flags>-MuteHostErrorMsg -ES6Module</compile-flags>
//     <tags>exclude_dynapogo,exclude_xplat</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>OS_5500719.js</files>
//     <compile-flags>-forceserialized</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>OS_8600339.js</files>
//     <compile-flags>-forceserialized</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn iteratorclose_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "iteratorclose.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn iterator_next_caches_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "iterator_next_caches.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn iteratorclose_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "iteratorclose.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_issue_1496_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_1496.js",
        baseline_path: Some("bug_issue_1496.baseline"),
        compile_flags: vec!["-mic:1", "-maxsimplejitruncount:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_issue_3247.js</files>
//     <baseline>bug_issue_3247.baseline</baseline>
//     <tags>exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn typedarray_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typedarray_bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os10595959_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug-OS10595959.js",
        compile_flags: vec![
            "-maxsimplejitruncount:1",
            "-maxinterpretcount:1",
            "-off:stackargopt",
            "-stress:bailonnoprofile",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_spread_rest_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "deferSpreadRestError.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os10898061_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS10898061.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os14880030_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14880030.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os14880030_js_force_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS14880030.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os13976524_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS13976524.js",
        compile_flags: vec!["-force:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os15679026_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS15679026.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_issue_4635_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_4635.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_OS13976524.js</files>
//     <tags>BugFix</tags>
//     <compile-flags>-force:deferparse -args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn defer_parse_lambda_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseLambda.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_parse_lambda_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseLambda.js",
        compile_flags: vec![
            "-off:deferparse",
            "-args",
            "summary",
            "-endargs",
            "-deferparse",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_parse_lambda_js_force_undo_defer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseLambda.js",
        compile_flags: vec![
            "-off:deferparse",
            "-args",
            "summary",
            "-endargs",
            "-deferparse",
            "-forceundodefer",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_parse_methods_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseMethods.js",
        compile_flags: vec!["-off:deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_parse_methods_js_defer_parse() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseMethods.js",
        compile_flags: vec![
            "-off:deferparse",
            "-args",
            "summary",
            "-endargs",
            "-deferparse",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defer_parse_methods_js_force_undo_defer() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DeferParseMethods.js",
        compile_flags: vec![
            "-off:deferparse",
            "-args",
            "summary",
            "-endargs",
            "-deferparse",
            "-forceundodefer",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn function_expr_capture_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "function-expr-capture.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_user_func_as_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "setUserFuncAsProto.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn function_expr_capture2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "function-expr-capture2.js",
        compile_flags: vec!["-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn supersetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "supersetter.js",
        baseline_path: Some("supersetter.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os16649604_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS16649604.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os17895855_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS17895855.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn super_bugs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "super_bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_issue_5994_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_issue_5994.js",
        baseline_path: Some("bug_issue_5994.baseline"),
        compile_flags: vec!["-MuteHostErrorMsg"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_19225364_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_19225364.js",
        baseline_path: Some("bug_19225364.baseline"),
        compile_flags: vec!["-bgjit-", "-loopinterpretcount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}
