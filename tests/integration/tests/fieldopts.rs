use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/fieldopts";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn depolymorph01_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "depolymorph01.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_finaltypeandpoly_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-finaltypeandpoly.js",
        compile_flags: vec!["-maxSimpleJitRunCount:2"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_missing_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-missing.js",
        baseline_path: Some("equiv-missing.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_mismatch_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-mismatch.js",
        baseline_path: Some("equiv-mismatch.baseline"),
        compile_flags: vec!["-CollectGarbage", "-maxsimplejitruncount:2"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_mismatch2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-mismatch2.js",
        compile_flags: vec!["-force:rejit", "-off:bailonnoprofile"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_locktypeid_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-locktypeid.js",
        baseline_path: Some("equiv-locktypeid.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_needmonocheck_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-needmonocheck.js",
        compile_flags: vec!["-maxsimplejitruncount:4", "-force:inline"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn equiv_needsmonocheck2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "equiv-needsmonocheck2.js",
        baseline_path: Some("equiv-needsmonocheck2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldconstprop_ldmethodfld_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldconstprop_ldmethodfld.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_assign_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_assign.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_delete_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_delete.js",
        baseline_path: Some("fieldcopyprop_delete.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_deletestrict_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_deletestrict.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist2.js",
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist4.js",
        baseline_path: Some("fieldhoist4.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist5.js",
        baseline_path: Some(""),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist6.js",
        baseline_path: Some("fieldhoist6.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist6b_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist6b.js",
        baseline_path: Some("fieldhoist6b.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist7_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist7.js",
        baseline_path: Some("fieldhoist7.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist8_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist8.js",
        baseline_path: Some("fieldhoist8.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_nullfieldhoist_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_nullfieldhoist.js",
        baseline_path: Some("fieldhoist_nullfieldhoist.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist9_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist9.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_unreachable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_unreachable.js",
        baseline_path: Some("fieldhoist_unreachable.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typespec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typespec_js_aggressive_int_type_spec(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        compile_flags: vec!["-off:aggressiveIntTypeSpec"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typespec_js_aggressive_int_type_spec_off_inline(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec.js",
        baseline_path: Some("fieldhoist_typespec.baseline"),
        compile_flags: vec!["-off:aggressiveIntTypeSpec", "-off:inline"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typespec2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec2.js",
        compile_flags: vec!["-force:jitLoopBody", "-off:aggressiveIntTypeSpec"],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typespec3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typespec3.js",
        baseline_path: Some("fieldhoist_typespec3.baseline"),
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_undefined_global_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_undefined_global.js",
        baseline_path: Some("fieldhoist_undefined_global.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_negzero_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_negzero.js",
        baseline_path: Some("fieldhoist_negzero.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_negzero_js_serialized(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_negzero.js",
        baseline_path: Some("fieldhoist_negzero.baseline"),
        compile_flags: vec!["-Serialized"],
        tags: HashSet::from(["exclude_forceserialized"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_typeof_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_typeof.js",
        baseline_path: Some("fieldhoist_typeof.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_sideeffect_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_sideeffect.js",
        baseline_path: Some("fieldhoist_sideeffect.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_nonwritable_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_nonwritable.js",
        baseline_path: Some("fieldcopyprop_nonwritable.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_primitive_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_primitive.js",
        baseline_path: Some("fieldcopyprop_primitive.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_preventextensions_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_preventextensions.js",
        baseline_path: Some("fieldcopyprop_preventextensions.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_freeze_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_freeze.js",
        baseline_path: Some("fieldcopyprop_freeze.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn redundanttype1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redundanttype1.js",
        baseline_path: Some("redundanttype1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_join_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_join.js",
        baseline_path: Some("fieldhoist_join.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_slots_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_slots.js",
        baseline_path: Some("fieldhoist_slots.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_slots2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_slots2.js",
        baseline_path: Some("fieldhoist_slots2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_objptrcopyprop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_objptrcopyprop.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_objptrcopyprop2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_objptrcopyprop2.js",
        baseline_path: Some("fieldhoist_objptrcopyprop2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_kills_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_kills.js",
        baseline_path: Some("fieldhoist_kills.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_stripbailouts_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_stripbailouts.js",
        baseline_path: Some("fieldhoist_stripbailouts.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn redundanttype2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redundanttype2.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn check_this_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CheckThis.js",
        baseline_path: Some("CheckThis.baseline"),
        compile_flags: vec!["-force:inline", "-force:checkthis"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn objptrcopyprop_bug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objptrcopyprop_bug.js",
        baseline_path: Some("objptrcopyprop_bug.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn objptrcopyprop_typescript_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objptrcopyprop_typescript.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldcopyprop_typespec_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldcopyprop_typespec.js",
        baseline_path: Some("fieldcopyprop_typespec.baseline"),
        compile_flags: vec!["-force:fieldcopyprop"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_deletefld_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_deletefld.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn forcefixdataprops_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "forcefixdataprops.js",
        compile_flags: vec!["-off:simplejit", "-force:fixdataprops"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn prop_object_pointer_copy_prop_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "PropObjectPointerCopyProp.js",
        baseline_path: Some(""),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn redundanttype_kills_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "redundanttype_kills.js",
        baseline_path: Some("redundanttype_kills.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_copypropdep_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_copypropdep.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_number_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_number.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec1.js",
        baseline_path: Some("objtypespec1.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec2.js",
        baseline_path: Some("objtypespec2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec3.js",
        baseline_path: Some("objtypespec3.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_fieldhoist_js(#[case] variant: Variant) {
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
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_fieldhoist_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-fieldhoist.2.js",
        baseline_path: Some("objtypespec-fieldhoist.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_proto_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec_proto.js",
        baseline_path: Some("objtypespec_proto.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_add_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add.js",
        baseline_path: Some("objtypespec-add.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simpleJit"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_add_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add-2.js",
        baseline_path: Some("objtypespec-add-2.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-loopInterpretCount:1"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_add_4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-add-4.js",
        baseline_path: Some("objtypespec-add-4.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_glob_opt(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_inline(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_obj_type_spec(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_obj_type_spec_new_obj(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_fixed_new_obj(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_fixed_methods(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2 -off:fixedMethods"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_split_new_sc_object(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_obj_type_spec_new_obj_split_new_sc_object(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.1.js",
        baseline_path: Some("objtypespec-newobj.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_1_js_off_fixed_new_obj_split_new_sc_object(#[case] variant: Variant) {
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
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_glob_opt(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_inline(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_obj_type_spec(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_obj_type_spec_new_obj(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_fixed_new_obj(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_fixed_methods(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:fixedMethods"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_split_new_sc_object(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_obj_type_spec_new_obj_split_new_sc_object(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj.2.js",
        baseline_path: Some("objtypespec-newobj.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_2_js_off_fixed_new_obj_split_new_sc_object(#[case] variant: Variant) {
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
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js1(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js8(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.1.js",
        baseline_path: Some("objtypespec-newobj-invalidation.1.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_1_js9(#[case] variant: Variant) {
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
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js1(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:globOpt"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js3(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:inline"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js4(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpec"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js5(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:objTypeSpecNewObj"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js6(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:fixedNewObj",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js7(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:splitNewScObject"],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js8(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objtypespec-newobj-invalidation.2.js",
        baseline_path: Some("objtypespec-newobj-invalidation.2.baseline"),
        compile_flags: vec![
            "-maxinterpretcount:2",
            "-off:objTypeSpecNewObj",
            "-off:splitNewScObject",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn objtypespec_newobj_invalidation_2_js9(#[case] variant: Variant) {
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
        tags: HashSet::from(["exclude_dynapogo"]),
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::disable_jit(Variant::DisableJit)]
fn field_hoist_max_interpret_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FieldHoist_MaxInterpret.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        tags: HashSet::from(["exclude_dynapogo"]),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn mark_temp_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "markTemp.js",
        baseline_path: Some("markTemp.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn root_obj_1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "rootObj-1.js",
        baseline_path: Some("rootObj-1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltypebug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltypebug.js",
        baseline_path: Some("finaltypebug.baseline"),
        compile_flags: vec!["-MaxSimpleJITRunCount:1", "-MaxinterpretCount:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltype2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype2.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltype2_js_bgjit(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype2.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltype_objheaderinlining1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining1.js",
        compile_flags: vec!["-off:simplejit", "-fja:1"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltype_objheaderinlining2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining2.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn finaltype_objheaderinlining3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "finaltype-objheaderinlining3.js",
        compile_flags: vec!["-lic:1", "-bgjit-", "-force:deferparse"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_accessorinlining1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_accessorinlining1.js",
        compile_flags: vec!["-loopinterpretcount:1", "-bgjit-"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fieldhoist_accessorinlining2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fieldhoist_accessorinlining2.js",
        baseline_path: Some("fieldhoist_accessorinlining2.baseline"),
        compile_flags: vec!["-off:inlinegetters", "-off:fixedmethods"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck.js",
        compile_flags: vec!["-force:fixdataprops"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck2_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck3_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck4.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck5.js",
        compile_flags: vec!["-maxinterpretcount:2", "-maxsimplejitruncount:6"],
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn fixedfieldmonocheck6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedfieldmonocheck6.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn add_prop_to_dictionary_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "add-prop-to-dictionary.js",
        baseline_path: Some("add-prop-to-dictionary.baseline"),
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn argobjlengthhoist_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "argobjlengthhoist.js",
        ..Default::default()
    };
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn os23440664_js(#[case] variant: Variant) {
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
    common::run_test_variant(&test, variant, common::DEFAULT_TAGS);
}
