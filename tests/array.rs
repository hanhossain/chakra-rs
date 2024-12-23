mod common;

#[test]
fn array_fastinit_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_fastinit.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_fastinit.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_at_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_at.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_flat_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_flat.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_findlast.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_findlastindex.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn array_sort_order_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_sort_order.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_init.js</files>
//     <baseline>array_init.baseline</baseline>
//     <tags>Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-Serialized</compile-flags>
//     <files>array_init2.js</files>
//     <baseline>array_init2.baseline</baseline>
//     <tags>exclude_forceserialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-args summary -endargs -ForceArrayBTree -recyclerStress</compile-flags>
//     <files>SpliceBtreeMemoryCorruption.js</files>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-forcearraybtree -off:nativearray -args summary -endargs</compile-flags>
//     <files>sliceArrayForceBtreeBug616623.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-ForceArrayBTree</compile-flags>
//     <files>bug945376SegLeftPlusSizeGreaterThanMaxArrayLen.js</files>
//   </default>
// </test>

#[test]
fn bug1062870_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug1062870.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug1065362_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug1065362.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug11370283.js</files>
//     <compile-flags>-bgjit- -lic:1</compile-flags>
//     <tags>exclude_dynapogo</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug4916987.js</files>
//     <compile-flags>-mic:1 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug6268659.js</files>
//     <compile-flags>-mic:1 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-ForceArrayBTree</compile-flags>
//     <files>ArrayBtreeBadCodeGen.js</files>
//     <baseline>ArrayBtreeBadCodeGen.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>SliceandConcatAlterOriginalArrayBug.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-ForceArrayBTree -maxinterpretcount:1 -maxsimplejitruncount:2</compile-flags>
//     <files>rawLastUsedSegmentBugInFloatArray.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -force:rejit -ForceArrayBTree</compile-flags>
//     <files>ArrayElementMissingValueSetToZero.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags> -forcejitloopbody -ForceArrayBTree -off:ArrayCheckHoist</compile-flags>
//     <files>TryGrowHeadSegmentBug.js</files>
//   </default>
// </test>

#[test]
fn array_init2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_init2.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_init2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_ctr_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_ctr.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_ctr.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-arrayValidate</compile-flags>
//     <files>array_ctr.js</files>
//     <baseline>array_ctr.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn arr_bailout_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/arr_bailout.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/concat1.js",
        baseline_path: Some("chakracore-cxx/test/Array/concat1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn concat2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/concat2.js",
        baseline_path: Some("chakracore-cxx/test/Array/concat2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/delete.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn es5array_push_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/es5array_push.js",
        baseline_path: Some("chakracore-cxx/test/Array/es5array_push.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn ldindex_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/ldindex.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug612012.js</files>
//     <compile-flags>-maxinterpretcount:1 -loopinterpretcount:1 -ForceArrayBTree -oopjit-</compile-flags>
//     <tags>exclude_serialized</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>SegmentMapFlagResetInJSArrayConstructor.js</files>
//     <compile-flags> -maxinterpretcount:1 -maxsimplejitruncount:2  -ForceArrayBTree</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>LastUsedSegmentHasNULLElement.js</files>
//     <compile-flags> -maxinterpretcount:1 -maxsimplejitruncount:2  -ForceArrayBTree</compile-flags>
//   </default>
// </test>

#[test]
fn array_length_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_length.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_length.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn join2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/join2.js",
        baseline_path: Some("chakracore-cxx/test/Array/join2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/missing.js",
        baseline_path: Some("chakracore-cxx/test/Array/missing.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/pop1.js",
        baseline_path: Some("chakracore-cxx/test/Array/pop1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/pop2.js",
        baseline_path: Some("chakracore-cxx/test/Array/pop2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/pop3.js",
        baseline_path: Some("chakracore-cxx/test/Array/pop3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/pop4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop5_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/pop5.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn push1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/push1.js",
        baseline_path: Some("chakracore-cxx/test/Array/push1_2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>push2.js</files>
//     <compile-flags>-es6toLength</compile-flags>
//     <baseline>push2.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>push3.js</files>
//     <compile-flags>-off:nativearray -ForceES5Array</compile-flags>
//   </default>
// </test>

#[test]
fn push4_traps_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/push4_traps.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reverse1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/reverse1.js",
        baseline_path: Some("chakracore-cxx/test/Array/reverse1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reverse2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/reverse2.js",
        baseline_path: Some("chakracore-cxx/test/Array/reverse2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn shift_unshift_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/shift_unshift.js",
        baseline_path: Some("chakracore-cxx/test/Array/shift_unshift.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn to_string_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/toString.js",
        baseline_path: Some("chakracore-cxx/test/Array/toString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>toString.js</files>
//     <compile-flags>-ForceES5Array</compile-flags>
//     <tags>exclude_test</tags>
//     <baseline>toString.baseline</baseline>
//   </default>
// </test>

#[test]
fn to_locale_string_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/toLocaleString.js",
        baseline_path: Some("chakracore-cxx/test/Array/toLocaleString.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>toLocaleString.js</files>
//     <compile-flags>-ForceES5Array</compile-flags>
//     <tags>exclude_test</tags>
//     <baseline>toLocaleString.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_slice.js</files>
//     <baseline>array_slice.baseline</baseline>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn array_slice2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_slice2.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_slice2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_sort.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_includes.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn array_splice_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_splice_double_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice_double.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice_double.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-arrayValidate</compile-flags>
//     <files>array_splice.js</files>
//     <baseline>array_splice.baseline</baseline>
//     <tags>exclude_test</tags>
//   </default>
// </test>

#[test]
fn array_splice1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice1.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_splice2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice2.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_splice3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice3.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice3_3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_splice4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice4.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice4.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn sparsearray_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/sparsearray.js",
        baseline_path: Some("chakracore-cxx/test/Array/sparsearray.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_lastindexof_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_lastindexof.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_lastindexof.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_index_of_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_indexOf.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_indexOf.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_indexOf.js</files>
//     <compile-flags>-ForceArrayBTree</compile-flags>
//     <baseline>array_indexOf.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_indexOf.js</files>
//     <compile-flags>-JsBuiltIn-</compile-flags>
//     <baseline>array_indexOf.baseline</baseline>
//   </default>
// </test>

#[test]
fn array_index_of_sparse_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_indexOfSparse.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_indexOfSparse.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn negindex_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/negindex.js",
        baseline_path: Some("chakracore-cxx/test/Array/negindex.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_forin_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_forin.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_forin.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_literal_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_literal.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_literal.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-recyclerStress</compile-flags>
//     <files>array_literal.js</files>
//     <baseline>array_literal.baseline</baseline>
//     <tags>exclude_test,Slow</tags>
//   </default>
// </test>

#[test]
fn nativearray_gen1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen1.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativearray_gen1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativearray_gen1.js</files>
//     <compile-flags>-sse:0 -forceserialized</compile-flags>
//     <baseline>nativearray_gen1.baseline</baseline>
//     <!-- Exclude the serialized variants because we're using -forceserialized here. -->
//     <tags>exclude_serialized</tags>
//   </default>
// </test>

#[test]
fn nativearray_gen2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nativearray_gen3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nativearray_gen4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen4.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativearray_gen4.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nativearray_gen5_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen5.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativearray_gen6.js</files>
//     <compile-flags>-loopinterpretcount:0</compile-flags>
//   </default>
// </test>

#[test]
fn nativearray_gen7_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen7.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativearray_gen7.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativearray_gen8.js</files>
//     <compile-flags>-force:jitloopbody</compile-flags>
//   </default>
// </test>

#[test]
fn arrlit_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/arrlit.js",
        baseline_path: Some("chakracore-cxx/test/Array/arrlit.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_lookup_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/protoLookup.js",
        baseline_path: Some("chakracore-cxx/test/Array/protoLookup.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_lookup_native_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/protoLookup_native.js",
        baseline_path: Some("chakracore-cxx/test/Array/protoLookup_native.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn proto_lookup_with_getters_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/protoLookupWithGetters.js",
        baseline_path: Some("chakracore-cxx/test/Array/protoLookupWithGetters.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_apply_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_apply.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_apply.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativeArrayPushInlining.js</files>
//     <baseline>nativeArrayPushInlining.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1 -off:arraycheckhoist -off:fixedmethods</compile-flags>
//   </default>
// </test>

#[test]
fn reverse_native_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/reverse_native.js",
        baseline_path: Some("chakracore-cxx/test/Array/reverse_native.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn native_float_array_shift_unshift_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativeFloatArray_shift_unshift.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativeFloatArray_shift_unshift.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn native_float_array_sort_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativeFloatArray_sort.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativeFloatArray_sort.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>missingItemFastPathCheck.js</files>
//     <baseline>missingItemFastPathCheck.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_opts.js</files>
//     <compile-flags>-maxinterpretcount:1</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativeIntPop.js</files>
//     <baseline>nativeIntPop.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1 -off:fixedmethods -off:Arraycheckhoist</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nativeFloatPop.js</files>
//     <baseline>nativeFloatPop.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1 -off:fixedmethods -off:Arraycheckhoist</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>popImplicitCall.js</files>
//     <baseline>popImplicitCall.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1 -off:fixedmethods -off:Arraycheckhoist</compile-flags>
//   </default>
// </test>

#[test]
fn array_splice_515632_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice_515632.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice_515632.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>InlineArrayPopWithIntConstSrc.js</files>
//     <baseline>InlineArrayPopWithIntConstSrc.baseline</baseline>
//     <compile-flags>-maxinterpretcount:1 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>FailToSetLength.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn foreach_nativearray_change_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/foreach_nativearray_change.js",
        baseline_path: Some("chakracore-cxx/test/Array/foreach_nativearray_change.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn newfromargs_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/newfromargs.js",
        baseline_path: Some("chakracore-cxx/test/Array/newfromargs.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <compile-flags>-ForceArrayBTree</compile-flags>
//     <files>bug945376SizeBoundStartSeg.js</files>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>CopyOnAccessArray_bugs.js</files>
//     <tags>require_backend</tags>
//     <compile-flags>-force:copyonaccessarray</compile-flags>
//     <baseline>CopyOnAccessArray_bugs.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>CopyOnAccessArray_cache_index_overflow.js</files>
//     <tags>exclude_nonative,exclude_forceserialized,require_backend</tags>
//     <compile-flags>-force:copyonaccessarray -testtrace:CopyOnAccessArray</compile-flags>
//     <baseline>CopyOnAccessArray_cache_index_overflow.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_lifetime_bug.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memset.js</files>
//     <compile-flags>-mic:1 -off:simplejit -off:JITLoopBody -off:inline -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memset_invariant.js</files>
//     <tags>Slow</tags>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0 -off:JITLoopBody</compile-flags>
//     <timeout>300</timeout>
//   </default>
// </test>
// <!--
// TODO fix and re-enable, or remove this test (disabled in 91e0e9128)
// Microsoft/ChakraCore#2977

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memset_simd.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0 -off:JITLoopBody -simdjs -simd128typespec</compile-flags>
//   </default>
// </test>
// -->

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memset2.js</files>
//     <compile-flags>-mic:1 -off:simplejit -off:JITLoopBody -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memcopy.js</files>
//     <compile-flags>-mic:1 -off:simplejit -off:JITLoopBody -off:inline -off:globopt:1.18-1.30 -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memcopy.js</files>
//     <compile-flags>-mic:1 -off:simplejit -off:JITLoopBody -off:inline -off:globopt:1.18-1.30 -mmoc:0 -args float -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memcopy_length_bug.js</files>
//     <compile-flags>-bgjit- -lic:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memcopy_missing_values.js</files>
//     <compile-flags>-mic:1 -off:simplejit -bgjit- -lic:1</compile-flags>
//     <baseline>memcopy_missing_values.baseline</baseline>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_alias.js</files>
//     <compile-flags>-mic:1 -off:simplejit -off:jitloopbody -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_field.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_slot.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_bounds_check.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memop_missingValues.js</files>
//     <compile-flags>-mmoc:0</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug4587739.js</files>
//     <compile-flags>-mic:1 -off:simplejit</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug8159763.js</files>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0 -off:bailonnoprofile</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Array_TypeConfusion_bugs.js</files>
//     <compile-flags>-args summary -endargs</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>Array_TypeConfusion_bugs.js</files>
//     <compile-flags>-ForceArrayBTree -args summary -endargs</compile-flags>
//   </default>
// </test>

#[test]
fn bug_9575461_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug_9575461.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_12044876.js</files>
//     <compile-flags>-forcearraybtree</compile-flags>
//   </default>
// </test>

#[test]
fn array_conv_src_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_conv_src.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug12340575_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug12340575.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn constructor_fastpath_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/constructor_fastpath.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug16717501_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug16717501.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn filter_with_typed_array_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/FilterWithTypedArray.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_gh5667.js</files>
//     <tags>exclude_windows</tags>
//     <compile-flags>-JsBuiltIn-</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>bug_gh6320.js</files>
//     <tags>exclude_nonative</tags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>array_sort_random.js</files>
//     <tags>exclude_disable_jit,exclude_lite</tags>
//   </default>
// </test>
