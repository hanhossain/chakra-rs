use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 1] = ["require_backend"];
const DIRECTORY: &str = "chakracore-cxx/test/FixedFields";

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn non_fixed_field_hoist_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NonFixedFieldHoist.js",
        baseline_path: Some("NonFixedFieldHoist.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-bgjit-", "-off:simplejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn non_fixed_field_hoist_js2(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "NonFixedFieldHoist.js",
        baseline_path: Some("NonFixedFieldHoist.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_fields_on_singletons_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedFieldsOnSingletons.js",
        baseline_path: Some("FixedFieldsOnSingletons.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_fields_on_prototypes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedFieldsOnPrototypes.js",
        baseline_path: Some("FixedFieldsOnPrototypes.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_fields_type_system_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedFieldsTypeSystem.js",
        baseline_path: Some("FixedFieldsTypeSystem.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_fields_invalidation_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedFieldsInvalidation.js",
        baseline_path: Some("FixedFieldsInvalidation.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_fields_with_property_string_cache_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedFieldsWithPropertyStringCache.js",
        baseline_path: Some("FixedFieldsWithPropertyStringCache.baseline"),
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_with_different_type_handlers_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedDataWithDifferentTypeHandlers.js",
        baseline_path: Some("fixedDataWithDifferentTypeHandlers.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_with_different_type_handlers_prototypes_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedDataWithDifferentTypeHandlersPrototypes.js",
        baseline_path: Some("fixedDataWithDifferentTypeHandlersPrototypes.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_polymorphism_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedDataPolymorphism.js",
        baseline_path: Some("FixedDataPolymorphism.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_type_transition_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedDataTypeTransition.js",
        baseline_path: Some("FixedDataTypeTransition.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_dictionary_type_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FixedDataDictionaryType.js",
        baseline_path: Some("FixedDataDictionaryType.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_with_subsequent_uses_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedDataWithSubsequentUses.js",
        baseline_path: Some("fixedDataWithSubsequentUses.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-MaxSimpleJitRunCount:0", "-bgjit-"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixed_data_with_cache_sharing_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedDataWithCacheSharing.js",
        baseline_path: Some("fixedDataWithCacheSharing.baseline"),
        compile_flags: vec!["-forcejitloopbody"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug677247_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug677247.js",
        baseline_path: Some("bug677247.baseline"),
        compile_flags: vec!["-off:inlineconstructors"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug712503_fixed_accessors_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug712503_fixedAccessors.js",
        baseline_path: Some("bug712503_fixedAccessors.baseline"),
        compile_flags: vec!["-maxinterpretcount:2", "-off:simplejit", "-force:rejit"],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn fixedmethods_poly_inlining_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "fixedmethods_polyInlining.js",
        compile_flags: vec![
            "-CollectGarbage",
            "-maxinterpretcount:1",
            "-maxsimplejitruncount:2",
        ],
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}

#[cfg(feature = "optimized-tests")]
#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
fn bug_vso_os_1015467_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bugVSO_OS_1015467.js",
        tags: HashSet::from(["require_backend"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, COMMON_TAGS);
}
