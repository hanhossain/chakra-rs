use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const DIRECTORY: &str = "chakracore-cxx/test/InlineCaches";

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t0_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t0.js",
        baseline_path: Some("t0.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t1.js",
        baseline_path: Some("t1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t2.js",
        baseline_path: Some("t2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t3.js",
        baseline_path: Some("t3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t4.js",
        baseline_path: Some("t4.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn t5_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "t5.js",
        baseline_path: Some("t5.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn test6_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "test6.js",
        baseline_path: Some("test6.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn type_property_cache_cross_context_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypePropertyCache_CrossContext.js",
        baseline_path: Some("TypePropertyCache_CrossContext.baseline"),
        tags: HashSet::from(["exclude_jsrt"]),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn getter_sideeffect_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "getter_sideeffect.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn prototype_chain_modifications_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "prototypeChainModifications.js",
        baseline_path: Some("prototypeChainModifications.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn inline_local_cache_without_property_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "inlineLocalCacheWithoutProperty.js",
        baseline_path: Some("inlineLocalCacheWithoutProperty.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn writable1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "writable1.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn writable2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "writable2.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn writable3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "writable3.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn defaultsetterbug_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "defaultsetterbug.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn big_dictionary_type_handler_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BigDictionaryTypeHandler.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn add_fld_fast_path_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "addFldFastPath.js",
        baseline_path: Some("addFldFastPath.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_property_cache1_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissingPropertyCache1.js",
        baseline_path: Some("MissingPropertyCache1.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_property_cache2_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissingPropertyCache2.js",
        baseline_path: Some("MissingPropertyCache2.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_property_cache3_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissingPropertyCache3.js",
        baseline_path: Some("MissingPropertyCache3.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_property_cache4_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissingPropertyCache4.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn missing_proto_inval_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "MissingProtoInval.js",
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn instance_of_cache_cross_registration_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "instanceOfCacheCrossRegistration.js",
        baseline_path: Some("instanceOfCacheCrossRegistration.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}

#[rstest]
#[case::interpreted(Variant::Interpreted)]
#[case::dynapogo(Variant::Dynapogo)]
#[case::disable_jit(Variant::DisableJit)]
fn bug_vso_os_1206083_js(#[case] variant: Variant) {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_vso_os_1206083.js",
        baseline_path: Some("bug_vso_os_1206083.baseline"),
        ..Default::default()
    };
    common::run_test_variant(test, variant, common::DEFAULT_TAGS);
}
