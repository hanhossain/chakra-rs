mod common;
const DIRECTORY: &str = "chakracore-cxx/test/Object";

#[test]
fn constructor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor.js",
        baseline_path: Some("constructor.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constructor1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor1.js",
        baseline_path: Some("constructor1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn expandos_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "expandos.js",
        baseline_path: Some("expandos.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn has_own_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasOwnProperty.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn from_entries_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fromEntries.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn is_enumerable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isEnumerable.js",
        baseline_path: Some("isEnumerable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn is_prototype_of_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isPrototypeOf.js",
        baseline_path: Some("isPrototypeOf_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bigconstructorbug.js</files>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Object.js",
        baseline_path: Some("Object.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn null_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "null.js",
        baseline_path: Some("null.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>propertyIsEnumerable.js</files>
//     <baseline>propertyIsEnumerable.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn property_descriptor_non_object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyDescriptorNonObject.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn property_record_large_heap_block_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyRecordLargeHeapBlock.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_locale_string2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toLocaleString2.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleStringBasics.js</files>
//       <baseline>toLocaleStringBasics.baseline</baseline>
//       <compile-flags>-args summary -endargs</compile-flags>
//       <!-- The output is different on windows with Intl off, may want to merge the windows and linux implementation -->
//       <tags>Intl,exclude_sanitize_address</tags>
//   </default>
// </test>

#[test]
fn to_string1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString1.js",
        baseline_path: Some("toString3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString2.js",
        baseline_path: Some("toString2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newobj_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newobj.js",
        baseline_path: Some("newobj.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn regex_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex.js",
        baseline_path: Some("regex.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn var_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "var.js",
        baseline_path: Some("var.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>moreProperties-enumeration.js</files>
//     <baseline>moreProperties-enumeration.baseline</baseline>
//     <tags>Slow</tags>
//     <timeout>600</timeout>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Slow.js</files>
//     <tags>exclude_debug,Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bigES5Array.js</files>
//     <baseline>bigES5Array.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

#[test]
fn numeric_property_is_enumerable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumericPropertyIsEnumerable.js",
        baseline_path: Some("NumericPropertyIsEnumerable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn define_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defineProperty.js",
        baseline_path: Some("defineProperty_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn get_own_property_descriptor_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getOwnPropertyDescriptor.js",
        baseline_path: Some("getOwnPropertyDescriptor_v3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn get_own_property_descriptors_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getOwnPropertyDescriptors.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-ESObjectGetOwnPropertyDescriptors",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_creation_optimizations_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectCreationOptimizations.js",
        baseline_path: Some("objectCreationOptimizations.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn multivardecl_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multivardecl.js",
        baseline_path: Some("multivardecl.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn property_strings_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyStrings.js",
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn forinenumcache_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinenumcache.js",
        baseline_path: Some("forinenumcache.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn forinnonenumerableshadowing_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinnonenumerableshadowing.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn forinfastpath_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinfastpath.js",
        baseline_path: Some("forinfastpath.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn for_in_error_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forIn.error.js",
        baseline_path: Some("forIn.error.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn hash_table_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HashTable.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn type_snapshot_enumeration_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypeSnapshotEnumeration.js",
        baseline_path: Some("TypeSnapshotEnumeration.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn type_snapshot_enumeration_cached_type_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypeSnapshotEnumerationCachedType.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-off:ArrayCheckHoist",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn new_sc_object_inline_slot_capacity_locking_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NewScObject-InlineSlotCapacityLocking.js",
        baseline_path: Some("NewScObject-InlineSlotCapacityLocking.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objlit_type_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit_type.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn path_type_delete_last_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PathTypeDeleteLastProperty.js",
        compile_flags: vec!["-mic:1", "-msjrc:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stackobject_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject.js",
        baseline_path: Some("stackobject.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn stackobject_escape_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject_escape.js",
        baseline_path: Some("stackobject_escape.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn large_aux_array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LargeAuxArray.js",
        compile_flags: vec!["-BigDictionaryTypeHandlerThreshold:20"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stackobject_dependency.js</files>
//     <baseline />
//     <compile-flags>-off:inline</compile-flags>
//   </default>
// </test>

#[test]
fn object_create_null_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectCreateNull.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_header_inlining_other_instrs_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_otherInstrs.js",
        baseline_path: Some("ObjectHeaderInlining_otherInstrs.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_header_inlining_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining.js",
        baseline_path: Some("ObjectHeaderInlining.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_SimpleDictTypeHandler.js</files>
//     <baseline>ObjectHeaderInlining_SimpleDictTypeHandler.baseline</baseline>
//     <compile-flags>-nonative</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_DictTypeHandler.js</files>
//     <baseline>ObjectHeaderInlining_DictTypeHandler.baseline</baseline>
//     <compile-flags>-nonative</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_deleteProps.js</files>
//     <baseline>ObjectHeaderInlining_deleteProps.baseline</baseline>
//     <compile-flags>-nonative</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_prototype.js</files>
//     <compile-flags>-nonative</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_prototypeTypeChange.js</files>
//     <baseline>ObjectHeaderInlining_prototypeTypeChange.baseline</baseline>
//     <compile-flags>-nonative</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

#[test]
fn object_header_inlining_obj_array_fast_path_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_objArrayFastPath.js",
        baseline_path: Some("ObjectHeaderInlining_objArrayFastPath.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_header_inlining_st_fld_opt_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_StFldOpt.js",
        baseline_path: Some("ObjectHeaderInlining_StFldOpt.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stackobject_dependency.js</files>
//     <baseline />
//     <compile-flags>-off:inline</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>stackobject_dependency.js</files>
//     <baseline />
//     <compile-flags>-off:inline</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_NewPropNoInlineCache.js</files>
//     <baseline />
//     <compile-flags>-forcejitloopbody</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_NewPropNoInlineCache_StaticType.js</files>
//     <baseline />
//     <compile-flags>-msjrc:1 -mic:1</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_NewPropSharedInlineCache.js</files>
//     <baseline />
//     <compile-flags>-mic:1 -forcejitloopbody -off:interpreterautoprofile</compile-flags>
//   </default>
// </test>

#[test]
fn for_in_inline_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ForInInline.js",
        baseline_path: Some("ForInInline.baseline"),
        compile_flags: vec!["-loopinterpretcount:1", "-force:inline", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>forinenumcachebuiltin.js</files>
//     <baseline />
//   </default>
// </test>

#[test]
fn assign_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "assign.js",
        baseline_path: Some("assign.baseline"),
        compile_flags: vec!["-args", "summary", "-endargs", "-trace:ObjectCopy"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_spread_simple_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectSpread_Simple.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-NoNative",
            "-ES2018ObjectRestSpread",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_spread_jit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectSpread_JIT.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-ES2018ObjectRestSpread",
            "-bgjit-",
            "-maxinterpretcount:1",
            "-off:simplejit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_spread_limits_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectSpread_Limits.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-NoNative",
            "-ES2018ObjectRestSpread",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_rest_simple_js_force_serialized() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectRest_Simple.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-NoNative",
            "-ES2018ObjectRestSpread",
            "-forceserialized",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_rest_simple_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectRest_Simple.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-NoNative",
            "-ES2018ObjectRestSpread",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn object_rest_jit_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectRest_JIT.js",
        compile_flags: vec![
            "-args",
            "summary",
            "-endargs",
            "-ES2018ObjectRestSpread",
            "-off:simplejit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_with_global_object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringWithGlobalObject.js",
        baseline_path: Some("toStringWithGlobalObject.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
