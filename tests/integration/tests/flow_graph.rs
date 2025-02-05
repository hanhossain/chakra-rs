use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>require_backend</tags>

const DIRECTORY: &str = "chakracore-cxx/test/FlowGraph";
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>mic1msjrc1.js</files>
//       <compile-flags>-mic:1 -msjrc:1 -oopjit- -bgjit- -loopinterpretcount:1</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>weird1.js</files>
//       <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -oopjit- -off:bailonnoprofile -off:cachedScope</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>weird2.js</files>
//       <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -bgjit- -loopinterpretcount:1 -oopjit- -off:simplejit -force:inline</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>for_of_try_catch.js</files>
//       <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -bgjit- -oopjit- -off:jitloopbody</compile-flags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>byteCodeUpwardExposedUsed.js</files>
//       <compile-flags>-maxinterpretcount:1 -maxsimplejitruncount:1 -bgjit- -oopjit-</compile-flags>
//     </default>
//   </test>
