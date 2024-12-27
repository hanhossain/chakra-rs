mod common;
const DIRECTORY: &str = "chakracore-cxx/test/es5";

#[test]
fn reserved_words_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "reservedWords.js",
        baseline_path: Some("reservedWords.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn lex_u3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Lex_u3.js",
        baseline_path: Some("Lex_u3.baseline"),
        compile_flags: vec!["-ES6Unicode-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_every_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_every.js",
        baseline_path: Some("array_every.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_some_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_some.js",
        baseline_path: Some("array_some.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_for_each_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_forEach.js",
        baseline_path: Some("array_forEach.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_map_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_map.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_filter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_filter.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_reduce_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_reduce.js",
        baseline_path: Some("array_reduce.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_reduceright_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_reduceright.js",
        baseline_path: Some("array_reduceright.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn date_get_set9_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "DateGetSet9.js",
        baseline_path: Some("DateGetSet9.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn semicolon_after_block_es5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "SemicolonAfterBlockEs5.js",
        baseline_path: Some("SemicolonAfterBlockEs5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn exceptions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "exceptions.js",
        baseline_path: Some("exceptions3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn obj_lit_get_set_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLitGetSet.js",
        baseline_path: Some("ObjLitGetSet.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn obj_lit_get_set2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlitgetset2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn obj_lit_get_set3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlitgetset3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn obj_lit_get_set_parse_only_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLitGetSetParseOnly.js",
        baseline_path: Some("ObjLitGetSetParseOnly.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <files>ObjLitGetSetParseOnly.js</files>
//     <baseline>ObjLitGetSetParseOnlyFdp.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn obj_lit_init_fld_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjLitInitFld.js",
        baseline_path: Some("ObjLitInitFld.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn seal_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "seal.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn freeze_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "freeze.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn extensible_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "extensible.js",
        baseline_path: Some("extensible.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "array_length.js",
        baseline_path: Some("array_length.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-ForceES5Array</compile-flags>
//     <tags>exclude_test</tags>
//     <files>array_length.js</files>
//     <baseline>array_length.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ES5ArrayIndexList.js</files>
//     <tags>exclude_debug,Slow</tags>
//     <timeout>900</timeout>
//   </default>
// </test>

#[test]
fn createdp_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "createdp.js",
        baseline_path: Some("createdp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn define_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defineProperty.js",
        baseline_path: Some("defineProperty.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Serialized</compile-flags>
//     <files>defineProperty.js</files>
//     <baseline>defineProperty.baseline</baseline>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

#[test]
fn define_index_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defineIndexProperty.js",
        baseline_path: Some("defineIndexProperty.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Serialized</compile-flags>
//     <files>defineIndexProperty.js</files>
//     <baseline>defineIndexProperty.baseline</baseline>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

#[test]
fn enumerable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "enumerable.js",
        baseline_path: Some("enumerable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_item_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasItem.js",
        baseline_path: Some("hasItem.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_space_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regexSpace.js",
        baseline_path: Some("regexSpace.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn enumerating_with_es5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "EnumeratingWithES5.js",
        baseline_path: Some("EnumeratingWithES5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn insufficient_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "InsufficientArguments.js",
        baseline_path: Some("InsufficientArguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn recursivesetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "recursivesetter.js",
        baseline_path: Some("recursivesetter.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn valueof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valueof.js",
        baseline_path: Some("valueof5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tostring_override_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tostring_override.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn valueof_override_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valueof_override.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn tostring_override_js_force_string_keyed_simple_dictionary_type_handler() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "tostring_override.js",
        compile_flags: vec!["-ForceStringKeyedSimpleDictionaryTypeHandler"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn valueof_override_js_force_string_keyed_simple_dictionary_type_handler() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "valueof_override.js",
        compile_flags: vec!["-ForceStringKeyedSimpleDictionaryTypeHandler"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn type_conversions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypeConversions.js",
        baseline_path: Some("TypeConversions.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reg_exp_strict_delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "RegExpStrictDelete.js",
        baseline_path: Some("RegExpStrictDelete.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn setters_arguments_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "settersArguments.js",
        baseline_path: Some("settersArguments.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Force:Deferparse</compile-flags>
//     <files>settersArguments.js</files>
//     <baseline>settersArguments.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn augment_primitive_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "augmentPrimitive.js",
        baseline_path: Some("augmentPrimitive.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn setget_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "setget.js",
        baseline_path: Some("setget.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_objproto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_objproto.js",
        baseline_path: Some("es5array_objproto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_objproto_builtin_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_objproto_builtin.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_arrayproto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_arrayproto.js",
        baseline_path: Some("es5array_arrayproto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_arrayproto_opt_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_arrayproto_opt.js",
        baseline_path: Some("es5array_arrayproto_opt.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_arrayproto_crosssite_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_arrayproto_crosssite.js",
        baseline_path: Some("es5array_arrayproto_crosssite.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_protoarr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_protoarr.js",
        baseline_path: Some("es5array_proto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_protoobj_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_protoobj.js",
        baseline_path: Some("es5array_proto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_protoobj_crosssite_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_protoobj_crosssite.js",
        baseline_path: Some("es5array_protoobj_crosssite.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_replaceprotoarr_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_replaceprotoarr.js",
        baseline_path: Some("es5array_proto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_replaceprotoobj_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_replaceprotoobj.js",
        baseline_path: Some("es5array_proto.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn resetproperty_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "resetproperty.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_enum_edit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5array_enum_edit.js",
        baseline_path: Some("es5array_enum_edit.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5_define_property_array_length_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "es5_defineProperty_arrayLength.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn strictdefaultsetter_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "strictdefaultsetter.js",
        ..Default::default()
    };
    common::run_test(&test);
}
