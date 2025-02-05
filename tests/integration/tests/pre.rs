use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_backend</tags>

const DIRECTORY: &str = "chakracore-cxx/test/PRE";
// TODO (hanhossain): migrate
//    <test>
//     <default>
//       <files>pre1.js</files>
//       <baseline>pre1.baseline</baseline>
//       <tags>exclude_forceserialized,exclude_dynapogo</tags>
//       <compile-flags>-testtrace:fieldcopyprop -oopjit-</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>bug0.js</files>
//       <compile-flags>-args summary -endargs</compile-flags>
//     </default>
//   </test>
