mod common;
const DIRECTORY: &str = "chakracore-cxx/test/fieldopts";

#[test]
fn depolymorph01_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "depolymorph01.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_finaltypeandpoly_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-finaltypeandpoly.js",
        compile_flags: vec!["-maxSimpleJitRunCount:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_missing_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-missing.js",
        baseline_path: Some("equiv-missing.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_mismatch_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-mismatch.js",
        baseline_path: Some("equiv-mismatch.baseline"),
        compile_flags: vec!["-CollectGarbage", "-maxsimplejitruncount:2"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_mismatch2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-mismatch2.js",
        compile_flags: vec!["-force:rejit", "-off:bailonnoprofile"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_locktypeid_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-locktypeid.js",
        baseline_path: Some("equiv-locktypeid.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_needmonocheck_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-needmonocheck.js",
        compile_flags: vec!["-maxsimplejitruncount:4", "-force:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn equiv_needsmonocheck2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-needsmonocheck2.js",
        baseline_path: Some("equiv-needsmonocheck2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldconstprop_ldmethodfld_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldconstprop_ldmethodfld.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_assign_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_assign.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_delete.js",
        baseline_path: Some("fieldcopyprop_delete.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_deletestrict_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_deletestrict.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist2.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist4.js",
        baseline_path: Some("fieldhoist4.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>fieldhoist5.js</files>
//     <compile-flags>-force:fieldcopyprop</compile-flags>
//     <baseline />
//   </default>
// </test>

#[test]
fn fieldhoist6_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist6.js",
        baseline_path: Some("fieldhoist6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist6b_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist6b.js",
        baseline_path: Some("fieldhoist6b.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist7_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist7.js",
        baseline_path: Some("fieldhoist7.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist8_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist8.js",
        baseline_path: Some("fieldhoist8.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_nullfieldhoist_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_nullfieldhoist.js",
        baseline_path: Some("fieldhoist_nullfieldhoist.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist9_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist9.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_unreachable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_unreachable.js",
        baseline_path: Some("fieldhoist_unreachable.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typespec_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typespec_js_aggressive_int_type_spec() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        compile_flags: vec!["-off:aggressiveIntTypeSpec"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typespec_js_aggressive_int_type_spec_off_inline() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        compile_flags: vec!["-off:aggressiveIntTypeSpec", "-off:inline"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typespec2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec2.js",
        compile_flags: vec!["-force:jitLoopBody", "-off:aggressiveIntTypeSpec"],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typespec3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec3.js",
        baseline_path: Some("fieldhoist_typespec3.baseline"),
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_undefined_global_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_undefined_global.js",
        baseline_path: Some("fieldhoist_undefined_global.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_negzero_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_negzero.js",
        baseline_path: Some("fieldhoist_negzero.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_negzero_js_serialized() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_negzero.js",
        baseline_path: Some("fieldhoist_negzero.baseline"),
        compile_flags: vec!["-Serialized"],
        tags: vec!["exclude_forceserialized"],
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_typeof_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typeof.js",
        baseline_path: Some("fieldhoist_typeof.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_sideeffect_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_sideeffect.js",
        baseline_path: Some("fieldhoist_sideeffect.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_nonwritable_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_nonwritable.js",
        baseline_path: Some("fieldcopyprop_nonwritable.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_primitive_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_primitive.js",
        baseline_path: Some("fieldcopyprop_primitive.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_preventextensions_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_preventextensions.js",
        baseline_path: Some("fieldcopyprop_preventextensions.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_freeze_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_freeze.js",
        baseline_path: Some("fieldcopyprop_freeze.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn redundanttype1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redundanttype1.js",
        baseline_path: Some("redundanttype1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_join_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_join.js",
        baseline_path: Some("fieldhoist_join.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_slots_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_slots.js",
        baseline_path: Some("fieldhoist_slots.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_slots2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_slots2.js",
        baseline_path: Some("fieldhoist_slots2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>fieldhoist_objptrcopyprop.js</files>
//     <compile-flags></compile-flags>
//     <baseline />
//   </default>
// </test>

#[test]
fn fieldhoist_objptrcopyprop2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_objptrcopyprop2.js",
        baseline_path: Some("fieldhoist_objptrcopyprop2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_kills_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_kills.js",
        baseline_path: Some("fieldhoist_kills.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_stripbailouts_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_stripbailouts.js",
        baseline_path: Some("fieldhoist_stripbailouts.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>redundanttype2.js</files>
//     <baseline />
//   </default>
// </test>

#[test]
fn check_this_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CheckThis.js",
        baseline_path: Some("CheckThis.baseline"),
        compile_flags: vec!["-force:inline", "-force:checkthis"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objptrcopyprop_bug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objptrcopyprop_bug.js",
        baseline_path: Some("objptrcopyprop_bug.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objptrcopyprop_typescript_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objptrcopyprop_typescript.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldcopyprop_typespec_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_typespec.js",
        baseline_path: Some("fieldcopyprop_typespec.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>fieldhoist_deletefld.js</files>
//     <baseline />
//   </default>
// </test>

#[test]
fn forcefixdataprops_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forcefixdataprops.js",
        compile_flags: vec!["-off:simplejit", "-force:fixdataprops"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>PropObjectPointerCopyProp.js</files>
//     <baseline />
//   </default>
// </test>

#[test]
fn redundanttype_kills_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redundanttype_kills.js",
        baseline_path: Some("redundanttype_kills.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_copypropdep_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_copypropdep.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_number_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_number.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objtypespec1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec1.js",
        baseline_path: Some("objtypespec1.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec2.js",
        baseline_path: Some("objtypespec2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec3.js",
        baseline_path: Some("objtypespec3.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_fieldhoist_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-fieldhoist.js",
        baseline_path: Some("objtypespec-fieldhoist.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:1",
            "-off:simpleJit",
            "-bgjit-",
            "-loopinterpretcount:0",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_fieldhoist_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-fieldhoist.2.js",
        baseline_path: Some("objtypespec-fieldhoist.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_proto_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec_proto.js",
        baseline_path: Some("objtypespec_proto.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_add_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add.js",
        baseline_path: Some("objtypespec-add.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_add_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add-2.js",
        baseline_path: Some("objtypespec-add-2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-loopInterpretCount:1"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_add_4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add-4.js",
        baseline_path: Some("objtypespec-add-4.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_glob_opt() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_inline() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_obj_type_spec() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_obj_type_spec_new_obj() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_fixed_new_obj() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_fixed_methods() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2 -off:fixedMethods"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_obj_type_spec_new_obj_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_1_js_off_fixed_new_obj_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_glob_opt() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_inline() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_obj_type_spec() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_obj_type_spec_new_obj() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_fixed_new_obj() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_fixed_methods() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:fixedMethods"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_obj_type_spec_new_obj_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_2_js_off_fixed_new_obj_split_new_sc_object() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js1() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js3() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js4() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js5() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js6() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js7() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js8() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_1_js9() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js1() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js3() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js4() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js5() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js6() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js7() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js8() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn objtypespec_newobj_invalidation_2_js9() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
            "-off:splitNewScObject",
        ],
        tags: vec!["exclude_dynapogo"],
    };
    common::run_test(&test);
}

#[test]
fn field_hoist_max_interpret_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FieldHoist_MaxInterpret.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: vec!["exclude_dynapogo"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn mark_temp_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "markTemp.js",
        baseline_path: Some("markTemp.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn root_obj_1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rootObj-1.js",
        baseline_path: Some("rootObj-1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltypebug_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltypebug.js",
        baseline_path: Some("finaltypebug.baseline"),
        compile_flags: vec!["-MaxSimpleJITRunCount:1", "-MaxinterpretCount:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltype2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltype2_js_bgjit() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype2.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltype_objheaderinlining1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining1.js",
        compile_flags: vec!["-off:simplejit", "-fja:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltype_objheaderinlining2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn finaltype_objheaderinlining3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining3.js",
        compile_flags: vec!["-lic:1", "-bgjit-", "-force:deferparse"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_accessorinlining1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_accessorinlining1.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fieldhoist_accessorinlining2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_accessorinlining2.js",
        baseline_path: Some("fieldhoist_accessorinlining2.baseline"),
        compile_flags: vec!["-off:inlinegetters", "-off:fixedmethods"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck.js",
        compile_flags: vec!["-force:fixdataprops"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck2.js",
        compile_flags: vec![
            "-force:deferparse",
            "-force:inline",
            "-force:scriptfunctionwithinlinecache",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck3_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck3.js",
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-force:jitloopbody",
            "-force:rejit",
            "-off:bailonnoprofile",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck4_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck5_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck5.js",
        compile_flags: vec!["-maxinterpretcount:2", "-maxsimplejitruncount:6"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn fixedfieldmonocheck6_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck6.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn add_prop_to_dictionary_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "add-prop-to-dictionary.js",
        baseline_path: Some("add-prop-to-dictionary.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn argobjlengthhoist_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argobjlengthhoist.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn os23440664_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "OS23440664.js",
        compile_flags: vec![
            "-off:bailonnoprofile",
            "-force:fixdataprops",
            "-forcejitloopbody",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}
