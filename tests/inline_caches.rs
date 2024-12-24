mod common;

#[test]
fn t0_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t0.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t0.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn t1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t1.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn t2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t2.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn t3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t3.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn t4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t4.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t4.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn t5_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/t5.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/t5.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn test6_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/test6.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/test6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>TypePropertyCache_CrossContext.js</files>
//     <baseline>TypePropertyCache_CrossContext.baseline</baseline>
//     <tags>exclude_jsrt</tags>
//   </default>
// </test>

#[test]
fn getter_sideeffect_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/getter_sideeffect.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn prototype_chain_modifications_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/prototypeChainModifications.js",
        baseline_path: Some(
            "chakracore-cxx/test/InlineCaches/prototypeChainModifications.baseline",
        ),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn inline_local_cache_without_property_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/inlineLocalCacheWithoutProperty.js",
        baseline_path: Some(
            "chakracore-cxx/test/InlineCaches/inlineLocalCacheWithoutProperty.baseline",
        ),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn writable1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/writable1.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn writable2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/writable2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn writable3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/writable3.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn defaultsetterbug_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/defaultsetterbug.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn big_dictionary_type_handler_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/BigDictionaryTypeHandler.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn add_fld_fast_path_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/addFldFastPath.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/addFldFastPath.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_property_cache1_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/MissingPropertyCache1.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/MissingPropertyCache1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_property_cache2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/MissingPropertyCache2.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/MissingPropertyCache2.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_property_cache3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/MissingPropertyCache3.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/MissingPropertyCache3.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_property_cache4_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/MissingPropertyCache4.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn missing_proto_inval_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/MissingProtoInval.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn instance_of_cache_cross_registration_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/instanceOfCacheCrossRegistration.js",
        baseline_path: Some(
            "chakracore-cxx/test/InlineCaches/instanceOfCacheCrossRegistration.baseline",
        ),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_vso_os_1206083_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/InlineCaches/bug_vso_os_1206083.js",
        baseline_path: Some("chakracore-cxx/test/InlineCaches/bug_vso_os_1206083.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}
