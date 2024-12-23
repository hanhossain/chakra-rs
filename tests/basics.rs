mod common;

#[test]
fn accessors_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/Accessors.js",
        baseline_path: "chakracore-cxx/test/Basics/Accessors.baseline",
    };
    common::run_test(&test);
}

#[test]
fn with_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With.js",
        baseline_path: "chakracore-cxx/test/Basics/With.baseline",
    };
    common::run_test(&test);
}

#[test]
fn with2_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/With2.js",
        baseline_path: "chakracore-cxx/test/Basics/With2.baseline",
    };
    common::run_test(&test);
}

#[test]
fn defprop_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/DefProp.js",
        baseline_path: "chakracore-cxx/test/Basics/DefProp.baseline",
    };
    common::run_test(&test);
}

#[test]
fn scopedaccessors_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/scopedaccessors.js",
        baseline_path: "chakracore-cxx/test/Basics/scopedaccessors.baseline",
    };
    common::run_test(&test);
}

#[test]
fn flags_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/flags.js",
        baseline_path: "chakracore-cxx/test/Basics/flagsES5.baseline",
    };
    common::run_test(&test);
}

#[test]
fn inlinecache_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/inlinecache.js",
        baseline_path: "chakracore-cxx/test/Basics/inlinecache.baseline",
    };
    common::run_test(&test);
}

#[test]
fn scan_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/scan.js",
        baseline_path: "chakracore-cxx/test/Basics/scan.baseline",
    };
    common::run_test(&test);
}

#[test]
fn enum_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/enum.js",
        baseline_path: "chakracore-cxx/test/Basics/enum.baseline",
    };
    common::run_test(&test);
}

#[test]
fn with3_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/with3.js",
        baseline_path: "chakracore-cxx/test/Basics/with3.baseline",
    };
    common::run_test(&test);
}

#[test]
fn bug650104_js() {
    let test = common::Test {
        source_path: "chakracore-cxx/test/Basics/bug650104.js",
        baseline_path: "chakracore-cxx/test/Basics/bug650104.baseline",
    };
    common::run_test(&test);
}
