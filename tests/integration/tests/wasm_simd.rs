use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_wasm,exclude_serialized,require_backend,exclude_xplat</tags>

const DIRECTORY: &str = "chakracore-cxx/test/wasm.simd";

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>loadTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>storeTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>constTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>splatNegTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>replaceLaneTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>truncConvTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>logicalTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>comparisonTests.js</files>
//     <compile-flags> -wasm -wasmsimd -off:jitloopbody</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>mathTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>binaryArithmeticTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>int64x2Tests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>

// TODO (hanhossain): migrate
// <test>
//   <default>
//     <files>buildExtractTests.js</files>
//     <compile-flags> -wasm -wasmsimd</compile-flags>
//   </default>
// </test>
