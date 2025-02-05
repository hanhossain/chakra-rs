use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,require_backend,require_asmjs</tags>

const DIRECTORY: &str = "chakracore-cxx/test/AsmJsParser";
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -testtrace:asmjs</compile-flags>
//       <files>UnaryPos.js</files>
//       <baseline>UnaryPos.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-on:asmjs -forcedeferparse</compile-flags>
//       <files>deferReparseBug.js</files>
//     </default>
//   </test>
