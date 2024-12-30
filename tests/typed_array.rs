mod common;

const DIRECTORY: &str = "chakracore-cxx/test/typedarray";

#[test]
fn typedarray_at_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typedarray_at.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn likely_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "likely.js",
        compile_flags: vec!["-bgjit-", "-lic:1"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arraybuffer_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arraybuffer.js",
        baseline_path: Some("arrayBuffer.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn arraybuffer_type_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "arraybufferType.js",
        baseline_path: Some("arraybufferType.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typed_array_builtins_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "TypedArrayBuiltins.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn integer_indexed_exotic_object_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "IntegerIndexedExoticObject.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bad_na_n_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "BadNaN.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn int8array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "int8array.js",
        baseline_path: Some("int8array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint8array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uint8array.js",
        baseline_path: Some("uint8array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn int16array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "int16array.js",
        baseline_path: Some("int16array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint16array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uint16array.js",
        baseline_path: Some("uint16array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn int32array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "int32array.js",
        baseline_path: Some("int32array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint32array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "uint32array.js",
        baseline_path: Some("uint32array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float32array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "float32array.js",
        baseline_path: Some("float32array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float64array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "float64array.js",
        baseline_path: Some("float64array_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>dataview.js</files>
//     <baseline>dataview.baseline</baseline>
//     <timeout>300</timeout>
//   </default>
// </test>

#[test]
fn dataview2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dataview2.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn reflect_define_property_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "reflect_defineProperty.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objectproperty_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectproperty.js",
        baseline_path: Some("objectproperty_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn objectproperty_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "objectproperty.js",
        baseline_path: Some("objectproperty_es6.baseline"),
        compile_flags: vec!["-force:typedarrayvirtual"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>nan.js</files>
//     <baseline>nan.baseline</baseline>
//     <tags>typedarray,exclude_nonrazzle</tags>
//   </default>
//   <condition order="1" type="include">
//     <target>amd64</target>
//     <override>
//       <baseline>nan.x64.baseline</baseline>
//     </override>
//   </condition>
//   <condition order="2" type="include">
//     <target>arm64</target>
//     <override>
//       <baseline>nan.x64.baseline</baseline>
//     </override>
//   </condition>
// </test>

#[test]
fn neg_indexes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "negIndexes.js",
        baseline_path: Some("negIndexes.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "set.js",
        baseline_path: Some("set.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "set.js",
        baseline_path: Some("set.baseline"),
        compile_flags: vec!["-force:typedarrayvirtual"],
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>samethread.js</files>
//     <baseline>samethread.baseline</baseline>
//     <timeout>300</timeout>
//   </default>
// </test>
// <!--
// Below test fails with difference in space. Investigate the cause and re-enable them (Microsoft/ChakraCore#3038)

// TODO (hanhossain): migrate
// <test>
//   <default>
//       <files>crossthread.js</files>
//       <baseline>crossthread_es6.baseline</baseline>
//       <tags>typedarray,exclude_arm,exclude_debug</tags>
//   </default>
// </test>
// -->

#[test]
fn int8_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Int8Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint8_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UInt8Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn int16_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Int16Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint16_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UInt16Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn int32_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Int32Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint32_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "UInt32Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float32_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Float32Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float64_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Float64Array2.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn float_helper_access_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "FloatHelperAccess.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn subarray_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "subarray.js",
        baseline_path: Some("subarray.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn dataview1_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "dataview1.js",
        baseline_path: Some("dataview1.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>allocation.js</files>
//     <tags>typedarray,exclude_arm,xplatslow,Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>allocation2.js</files>
//     <tags>typedarray,exclude_arm,xplatslow,Slow</tags>
//     <timeout>300</timeout>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <tags>exclude_test,exclude_dynapogo,typedarray</tags>
//     <compile-flags>-maxinterpretcount:1 -off:simpleJit</compile-flags>
//     <files>typedArrayProfile.js</files>
//   </default>
// </test>

#[test]
fn pixel_array_rounding_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "pixelArrayRounding.js",
        baseline_path: Some("pixelArrayRounding.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
            "-ES6-",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn pixel_array_rounding_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "pixelArrayRounding.js",
        baseline_path: Some("pixelArrayRounding.es6.baseline"),
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn cse_typed_array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "cseTypedArray.js",
        baseline_path: Some("cseTypedArray.baseline"),
        compile_flags: vec!["-maxInterpretCount:1", "-off:simpleJit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint8_clamped_array_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Uint8ClampedArray.js",
        baseline_path: Some("Uint8ClampedArray_es6.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn uint8_clamped_array2_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "Uint8ClampedArray2.js",
        compile_flags: vec![
            "-minInterpretCount:1",
            "-maxInterpretCount:1",
            "-off:simpleJit",
        ],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_different_types_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "setDifferentTypes.js",
        baseline_path: Some("setDifferentTypes.baseline"),
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn set_different_types_js2() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "setDifferentTypes.js",
        baseline_path: Some("setDifferentTypes.baseline"),
        compile_flags: vec!["-force:typedarrayvirtual"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug2230916_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug2230916.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug2268573_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug2268573.js",
        baseline_path: Some("bug2268573.baseline"),
        compile_flags: vec!["-maxinterpretcount:1", "-off:simplejit"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_4653428_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_4653428.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memset_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memset.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:JITLoopBody", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memset_neg_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memset_neg.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-bgjit-", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memcopy.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:JITLoopBody", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn memcopy_negative_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "memcopy_negative.js",
        compile_flags: vec!["-mic:1", "-off:simplejit", "-off:JITLoopBody", "-mmoc:0"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typedarray_bugfixes_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typedarray_bugfixes.js",
        compile_flags: vec!["-Off:Deferparse", "-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug_os_6911900_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug_OS_6911900.js",
        ..Default::default()
    };
    common::run_test(&test);
}

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>reentry1.js</files>
//     <timeout>480</timeout> <!-- ARM64 takes ~8 min -->
//     <tags>xplatslow,Slow</tags>
//   </default>
// </test>

#[test]
fn cross_site_virtual_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "CrossSiteVirtual.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn builtin_from_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "builtin_from.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn delete_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "delete.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn typeof_detached_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "typeofDetached.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn bug18321215_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "bug18321215.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn definitetypedarray_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "definitetypedarray.js",
        ..Default::default()
    };
    common::run_test(&test);
}

#[test]
fn sort_js() {
    let test = common::Test {
        directory: DIRECTORY,
        source_path: "sort.js",
        compile_flags: vec!["-args", "summary", "-endargs"],
        ..Default::default()
    };
    common::run_test(&test);
}
