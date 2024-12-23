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

#[test]
fn array_findlast_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_findlast.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_findlastindex_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_findlastindex.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn slice_array_force_btree_bug616623_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/sliceArrayForceBtreeBug616623.js",
        compile_flags: vec![
            "-forcearraybtree",
            "-off:nativearray",
            "-args",
            "summary",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug945376_seg_left_plus_size_greater_than_max_array_len_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug945376SegLeftPlusSizeGreaterThanMaxArrayLen.js",
        compile_flags: vec!["-ForceArrayBTree"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn bug4916987_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug4916987.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug6268659_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug6268659.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_btree_bad_code_gen_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/ArrayBtreeBadCodeGen.js",
        baseline_path: Some("chakracore-cxx/test/Array/ArrayBtreeBadCodeGen.baseline"),
        compile_flags: vec!["-ForceArrayBTree"],
    };
    common::run_test(&test);
}

#[test]
fn slice_and_concat_alter_original_array_bug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/SliceandConcatAlterOriginalArrayBug.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn raw_last_used_segment_bug_in_float_array_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/rawLastUsedSegmentBugInFloatArray.js",
        compile_flags: vec![
            "-ForceArrayBTree",
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_element_missing_value_set_to_zero_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/ArrayElementMissingValueSetToZero.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:1",
            "-force:rejit",
            "-ForceArrayBTree",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn try_grow_head_segment_bug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/TryGrowHeadSegmentBug.js",
        compile_flags: vec![
            "-forcejitloopbody",
            "-ForceArrayBTree",
            "-off:ArrayCheckHoist",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn segment_map_flag_reset_in_jsarray_constructor_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/SegmentMapFlagResetInJSArrayConstructor.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
            "-ForceArrayBTree",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn last_used_segment_has_null_element_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/LastUsedSegmentHasNULLElement.js",
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
            "-ForceArrayBTree",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn push2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/push2.js",
        baseline_path: Some("chakracore-cxx/test/Array/push2.baseline"),
        compile_flags: vec!["-es6toLength"],
    };
    common::run_test(&test);
}

#[test]
fn push3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/push3.js",
        compile_flags: vec!["-off:nativearray", "-ForceES5Array"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn array_sort_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_sort.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_includes_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_includes.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn array_index_of_js_force_array_btree() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_indexOf.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_indexOf.baseline"),
        compile_flags: vec!["-ForceArrayBTree"],
    };
    common::run_test(&test);
}

#[test]
fn array_index_of_js_force_array_jsbuiltin() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_indexOf.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_indexOf.baseline"),
        compile_flags: vec!["-JsBuiltIn-"],
    };
    common::run_test(&test);
}

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

#[test]
fn nativearray_gen6_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen6.js",
        compile_flags: vec!["-loopinterpretcount:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nativearray_gen7_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen7.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativearray_gen7.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn nativearray_gen8_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativearray_gen8.js",
        compile_flags: vec!["-force:jitloopbody"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn native_array_push_inlining_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativeArrayPushInlining.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativeArrayPushInlining.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:arraycheckhoist",
            "-off:fixedmethods",
        ],
    };
    common::run_test(&test);
}

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

#[test]
fn missing_item_fast_path_check_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/missingItemFastPathCheck.js",
        baseline_path: Some("chakracore-cxx/test/Array/missingItemFastPathCheck.baseline"),
        compile_flags: vec!["-maxinterpretcount:1"],
    };
    common::run_test(&test);
}

#[test]
fn array_opts_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_opts.js",
        compile_flags: vec!["-maxinterpretcount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn native_int_pop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativeIntPop.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativeIntPop.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:fixedmethods",
            "-off:Arraycheckhoist",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn native_float_pop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/nativeFloatPop.js",
        baseline_path: Some("chakracore-cxx/test/Array/nativeFloatPop.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:fixedmethods",
            "-off:Arraycheckhoist",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pop_implicit_call_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/popImplicitCall.js",
        baseline_path: Some("chakracore-cxx/test/Array/popImplicitCall.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:fixedmethods",
            "-off:Arraycheckhoist",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_splice_515632_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/array_splice_515632.js",
        baseline_path: Some("chakracore-cxx/test/Array/array_splice_515632.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inline_array_pop_with_int_const_src_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/InlineArrayPopWithIntConstSrc.js",
        baseline_path: Some("chakracore-cxx/test/Array/InlineArrayPopWithIntConstSrc.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
    };
    common::run_test(&test);
}

#[test]
fn fail_to_set_length_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/FailToSetLength.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn bug945376size_bound_start_seg_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug945376SizeBoundStartSeg.js",
        compile_flags: vec!["-ForceArrayBTree"],
        ..Default::default()
    };
    common::run_test(&test);
}

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

#[test]
fn memop_lifetime_bug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_lifetime_bug.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memset_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memset.js",
        compile_flags: vec![
            "-mic:1",
            "-off:simplejit",
            "-off:JITLoopBody",
            "-off:inline",
            "-mmoc:0",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>memset_invariant.js</files>
//     <tags>Slow</tags>
//     <compile-flags>-mic:1 -off:simplejit -mmoc:0 -off:JITLoopBody</compile-flags>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn memset2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memset2.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:JITLoopBody", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memcopy.js",
        compile_flags: vec![
            "-mic:1",
            "-off:simplejit",
            "-off:JITLoopBody",
            "-off:inline",
            "-off:globopt:1.18-1.30",
            "-mmoc:0",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_js_float() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memcopy.js",
        compile_flags: vec![
            "-mic:1",
            "-off:simplejit",
            "-off:JITLoopBody",
            "-off:inline",
            "-off:globopt:1.18-1.30",
            "-mmoc:0",
            "-args",
            "float",
            "-endargs",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_length_bug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memcopy_length_bug.js",
        compile_flags: vec!["-bgjit-", "-lic:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_missing_values_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memcopy_missing_values.js",
        baseline_path: Some("chakracore-cxx/test/Array/memcopy_missing_values.baseline"),
        compile_flags: vec!["-mic:1", "-off:simplejit", "-bgjit-", "-lic:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memop_alias_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_alias.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:jitloopbody", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memop_field_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_field.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memop_slot_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_slot.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memop_bounds_check_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_bounds_check.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memop_missing_values_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/memop_missingValues.js",
        compile_flags: vec!["-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug4587739_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug4587739.js",
        compile_flags: vec!["-mic:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug8159763_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug8159763.js",
        compile_flags: vec![
            "-mic:1",
            "-off:simplejit",
            "-mmoc:0",
            "-off:bailonnoprofile",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_type_confusion_bugs_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/Array_TypeConfusion_bugs.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn array_type_confusion_bugs_js_force_array_btree() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/Array_TypeConfusion_bugs.js",
        compile_flags: vec!["-ForceArrayBTree", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_9575461_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug_9575461.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_12044876_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Array/bug_12044876.js",
        compile_flags: vec!["-forcearraybtree"],
        ..Default::default()
    };
    common::run_test(&test);
}

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
