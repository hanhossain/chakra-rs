use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_wasm</tags>

const DIRECTORY: &str = "chakracore-cxx/test/WasmSpec.MultiValue";
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>simple_block.js</files>
//       <compile-flags>-wasm -WasmMultiValue</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>blocktypes.js</files>
//       <compile-flags>-wasm -WasmMultiValue</compile-flags>
//     </default>
//   </test>
