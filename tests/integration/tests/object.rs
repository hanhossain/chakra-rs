use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;
use std::time::Duration;

const DIRECTORY: &str = "chakracore-cxx/test/Object";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constructor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor.js",
        baseline_path: Some("constructor.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn constructor1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "constructor1.js",
        baseline_path: Some("constructor1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn expandos_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "expandos.js",
        baseline_path: Some("expandos.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn has_own_property_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "hasOwnProperty.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn from_entries_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fromEntries.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn is_enumerable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isEnumerable.js",
        baseline_path: Some("isEnumerable.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn is_prototype_of_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "isPrototypeOf.js",
        baseline_path: Some("isPrototypeOf_v3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn big_constructor_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bigconstructorbug.js",
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Object.js",
        baseline_path: Some("Object.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn null_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "null.js",
        baseline_path: Some("null.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn property_is_enumerable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyIsEnumerable.js",
        baseline_path: Some("propertyIsEnumerable.baseline"),
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn property_descriptor_non_object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyDescriptorNonObject.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn property_record_large_heap_block_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyRecordLargeHeapBlock.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_locale_string2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toLocaleString2.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_locale_string_basics_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toLocaleStringBasics.js",
        baseline_path: Some("toLocaleStringBasics.baseline"),
        compile_flags: vec!["-args", "summary", "-endargs"],
        tags: HashSet::from(["Intl", "exclude_sanitize_address"]),
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_string1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString1.js",
        baseline_path: Some("toString3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_string2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toString2.js",
        baseline_path: Some("toString2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn newobj_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "newobj.js",
        baseline_path: Some("newobj.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn regex_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "regex.js",
        baseline_path: Some("regex.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn var_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "var.js",
        baseline_path: Some("var.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(Duration::from_secs(600))]
fn more_properties_enumeration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "moreProperties-enumeration.js",
        baseline_path: Some("moreProperties-enumeration.baseline"),
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[cfg(feature = "optimize")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn slow_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Slow.js",
        tags: HashSet::from(["exclude_debug", "Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
#[ignore]
#[timeout(common::SLOW_TEST_TIMEOUT)]
fn big_es5_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bigES5Array.js",
        baseline_path: Some("bigES5Array.baseline"),
        tags: HashSet::from(["Slow"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn numeric_property_is_enumerable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NumericPropertyIsEnumerable.js",
        baseline_path: Some("NumericPropertyIsEnumerable.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn define_property_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defineProperty.js",
        baseline_path: Some("defineProperty_v3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn get_own_property_descriptor_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getOwnPropertyDescriptor.js",
        baseline_path: Some("getOwnPropertyDescriptor_v3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn get_own_property_descriptors_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_creation_optimizations_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectCreationOptimizations.js",
        baseline_path: Some("objectCreationOptimizations.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn multivardecl_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "multivardecl.js",
        baseline_path: Some("multivardecl.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn property_strings_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "propertyStrings.js",
        compile_flags: vec!["-CollectGarbage"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forinenumcache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinenumcache.js",
        baseline_path: Some("forinenumcache.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forinnonenumerableshadowing_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinnonenumerableshadowing.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forinfastpath_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinfastpath.js",
        baseline_path: Some("forinfastpath.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_error_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forIn.error.js",
        baseline_path: Some("forIn.error.baseline"),
        compile_flags: vec!["-ExtendedErrorStackForTestHost"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn hash_table_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "HashTable.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn type_snapshot_enumeration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypeSnapshotEnumeration.js",
        baseline_path: Some("TypeSnapshotEnumeration.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn type_snapshot_enumeration_cached_type_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn new_sc_object_inline_slot_capacity_locking_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NewScObject-InlineSlotCapacityLocking.js",
        baseline_path: Some("NewScObject-InlineSlotCapacityLocking.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn objlit_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objlit_type.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn path_type_delete_last_property_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PathTypeDeleteLastProperty.js",
        compile_flags: vec!["-mic:1", "-msjrc:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stackobject_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject.js",
        baseline_path: Some("stackobject.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stackobject_escape_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject_escape.js",
        baseline_path: Some("stackobject_escape.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn large_aux_array_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "LargeAuxArray.js",
        compile_flags: vec!["-BigDictionaryTypeHandlerThreshold:20"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stackobject_dependency_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject_dependency.js",
        baseline_path: Some(""),
        compile_flags: vec!["-off:inline"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_create_null_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectCreateNull.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_other_instrs_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_otherInstrs.js",
        baseline_path: Some("ObjectHeaderInlining_otherInstrs.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:2"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining.js",
        baseline_path: Some("ObjectHeaderInlining.baseline"),
        compile_flags: vec!["-off:simplejit", "-mic:2"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_simple_dict_type_handler_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_SimpleDictTypeHandler.js",
        baseline_path: Some("ObjectHeaderInlining_SimpleDictTypeHandler.baseline"),
        compile_flags: vec!["-nonative"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_dict_type_handler_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_DictTypeHandler.js",
        baseline_path: Some("ObjectHeaderInlining_DictTypeHandler.baseline"),
        compile_flags: vec!["-nonative"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_delete_props_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_deleteProps.js",
        baseline_path: Some("ObjectHeaderInlining_deleteProps.baseline"),
        compile_flags: vec!["-nonative"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_prototype_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_prototype.js",
        compile_flags: vec!["-nonative"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_prototype_type_change_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_prototypeTypeChange.js",
        baseline_path: Some("ObjectHeaderInlining_prototypeTypeChange.baseline"),
        compile_flags: vec!["-nonative"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_obj_array_fast_path_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_objArrayFastPath.js",
        baseline_path: Some("ObjectHeaderInlining_objArrayFastPath.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_st_fld_opt_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_StFldOpt.js",
        baseline_path: Some("ObjectHeaderInlining_StFldOpt.baseline"),
        compile_flags: vec!["-mic:2", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn stackobject_dependency_js_off_inline(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject_dependency.js",
        baseline_path: Some(""),
        compile_flags: vec!["-off:inline"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_new_prop_no_inline_cache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "stackobject_dependency.js",
        baseline_path: Some(""),
        compile_flags: vec!["-forcejitloopbody"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_new_prop_no_inline_cache_static_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_NewPropNoInlineCache_StaticType.js",
        baseline_path: Some(""),
        compile_flags: vec!["-msjrc:1", "-mic:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_header_inlining_new_prop_shared_inline_cache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ObjectHeaderInlining_NewPropSharedInlineCache.js",
        baseline_path: Some(""),
        compile_flags: vec!["-mic:1", "-forcejitloopbody", "-off:interpreterautoprofile"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn for_in_inline_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "ForInInline.js",
        baseline_path: Some("ForInInline.baseline"),
        compile_flags: vec!["-loopinterpretcount:1", "-force:inline", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forinenumcachebuiltin_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forinenumcachebuiltin.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn assign_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "assign.js",
        baseline_path: Some("assign.baseline"),
        compile_flags: vec!["-args", "summary", "-endargs", "-trace:ObjectCopy"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_spread_simple_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_spread_jit_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_spread_limits_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_rest_simple_js_force_serialized(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_rest_simple_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn object_rest_jit_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn to_string_with_global_object_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "toStringWithGlobalObject.js",
        baseline_path: Some("toStringWithGlobalObject.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant);
}
