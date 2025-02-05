use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_asmjs</tags>

const DIRECTORY: &str = "chakracore-cxx/test/AsmJsFloat";
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>BasicMathOp.js</files>
//       <baseline>BasicMathOp.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>Float64-LoadandStore.js</files>
//       <baseline>Float64-LoadandStore.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>LdUndefFromHeap.js</files>
//       <baseline>LdUndefFromHeap.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>NestedMathLibCalls.js</files>
//       <baseline>NestedMathLibCalls.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>NotOperator.js</files>
//       <baseline>NotOperator.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>StoreDoubleToFloat32.js</files>
//       <baseline>StoreDoubleToFloat32.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -on:asmjs -testtrace:asmjsinterpreter</compile-flags>
//       <files>StoreFloatToFloat32.js</files>
//       <baseline>StoreFloatToFloat32.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>BasicMathOp.js</files>
//       <baseline>BasicMathOp.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>Float64-LoadandStore.js</files>
//       <baseline>Float64-LoadandStore.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>LdUndefFromHeap.js</files>
//       <baseline>LdUndefFromHeap.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>NestedMathLibCalls.js</files>
//       <baseline>NestedMathLibCalls.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>NotOperator.js</files>
//       <baseline>NotOperator.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>StoreDoubleToFloat32.js</files>
//       <baseline>StoreDoubleToFloat32.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs -mtjrc:1</compile-flags>
//       <files>StoreFloatToFloat32.js</files>
//       <baseline>StoreFloatToFloat32.baseline</baseline>
//     </default>
//   </test>
