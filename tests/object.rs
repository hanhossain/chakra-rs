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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>propertyDescriptorNonObject.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>propertyRecordLargeHeapBlock.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString2.js</files>
//       <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

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
// <!-- Disabled until we can resolve failures on Windows Server 2012 R2 (Microsoft/ChakraCore#3030)

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString.js</files>
//       <baseline>toLocaleString.v4.baseline</baseline>
//       <compile-flags>-version:4</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString.js</files>
//       <baseline>toLocaleString.arm.baseline</baseline>
//       <compile-flags>-Intl</compile-flags>
//       <tags>exclude_x86,exclude_x64</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString.js</files>
//       <baseline>toLocaleString.baseline</baseline>
//       <compile-flags>-Intl</compile-flags>
//       <tags>exclude_arm,exclude_drt</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString.js</files>
//       <baseline>toLocaleString.winBlue.baseline</baseline>
//       <compile-flags>-Intl</compile-flags>
//       <tags>exclude_arm,exclude_snap,exclude_win7,exclude_win8</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString1.js</files>
//       <baseline>toLocaleString1.v4.baseline</baseline>
//       <compile-flags>-version:4</compile-flags>
//       <tags>exclude_win7,exclude_snap</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString1.js</files>
//       <baseline>toLocaleString1.win7.v4.baseline</baseline>
//       <compile-flags>-version:4</compile-flags>
//       <tags>exclude_win8,exclude_winBlue</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>toLocaleString1.js</files>
//       <baseline>toLocaleString1.baseline</baseline>
//       <compile-flags>-Intl</compile-flags>
//   </default>
// </test>
// !-->

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>getOwnPropertyDescriptors.js</files>
//     <compile-flags>-args summary -endargs -ESObjectGetOwnPropertyDescriptors</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>propertyStrings.js</files>
//     <compile-flags>-CollectGarbage</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>forinnonenumerableshadowing.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>forIn.error.js</files>
//     <baseline>forIn.error.baseline</baseline>
//     <compile-flags>-ExtendedErrorStackForTestHost</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>TypeSnapshotEnumerationCachedType.js</files>
//     <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -off:ArrayCheckHoist -args summary -endargs</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>PathTypeDeleteLastProperty.js</files>
//     <compile-flags>-mic:1 -msjrc:1</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>LargeAuxArray.js</files>
//     <compile-flags>-BigDictionaryTypeHandlerThreshold:20</compile-flags>
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
//     <files>objectCreateNull.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_otherInstrs.js</files>
//     <baseline>ObjectHeaderInlining_otherInstrs.baseline</baseline>
//     <compile-flags>-off:simplejit -mic:2</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining.js</files>
//     <baseline>ObjectHeaderInlining.baseline</baseline>
//     <compile-flags>-off:simplejit -mic:2</compile-flags>
//   </default>
// </test>

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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_objArrayFastPath.js</files>
//     <baseline>ObjectHeaderInlining_objArrayFastPath.baseline</baseline>
//     <compile-flags>-mic:2 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectHeaderInlining_StFldOpt.js</files>
//     <baseline>ObjectHeaderInlining_StFldOpt.baseline</baseline>
//     <compile-flags>-mic:2 -off:simplejit</compile-flags>
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

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ForInInline.js</files>
//     <baseline>ForInInline.baseline</baseline>
//     <compile-flags>-loopinterpretcount:1 -force:inline -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>forinenumcachebuiltin.js</files>
//     <baseline />
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>assign.js</files>
//     <compile-flags>-args summary -endargs -trace:ObjectCopy</compile-flags>
//     <baseline>assign.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectSpread_Simple.js</files>
//     <compile-flags>-args summary -endargs -NoNative -ES2018ObjectRestSpread</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectSpread_JIT.js</files>
//     <compile-flags>-args summary -endargs -ES2018ObjectRestSpread -bgjit- -maxinterpretcount:1 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectSpread_Limits.js</files>
//     <compile-flags>-args summary -endargs -ES2018ObjectRestSpread</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectRest_Simple.js</files>
//     <compile-flags>-args summary -endargs -NoNative -ES2018ObjectRestSpread -forceserialized</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectRest_Simple.js</files>
//     <compile-flags>-args summary -endargs -NoNative -ES2018ObjectRestSpread</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>ObjectRest_JIT.js</files>
//     <compile-flags>-args summary -endargs -ES2018ObjectRestSpread -off:simplejit</compile-flags>
//   </default>
// </test>

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
