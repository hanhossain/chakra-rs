use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

// TODO (hanhossain): tags - <tags>exclude_serialized,exclude_snap,require_debugger,exclude_sanitize_address</tags>

const DIRECTORY: &str = "chakracore-cxx/test/Debugger";
// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-dbgbaseline:empty.baseline</compile-flags>
//       <files>failfast.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-dbgbaseline:JsDiagBreakpoints.js.dbg.baseline</compile-flags>
//       <files>JsDiagBreakpoints.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-dbgbaseline:JsDiagBreakpoints.js.dbg.baseline</compile-flags>
//       <files>JsDiagBreakpoints_ArrayBuffer.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagBreakOnUncaughtException.js.dbg.baseline</compile-flags>
//       <baseline>JsDiagBreakOnUncaughtException.baseline</baseline>
//       <files>JsDiagBreakOnUncaughtException.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.js.dbg.baseline</compile-flags>
//       <baseline>JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.baseline</baseline>
//       <files>JsDiagExceptionsInPromises_BreakOnUncaughtExceptions.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.js.dbg.baseline</compile-flags>
//       <baseline>JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.baseline</baseline>
//       <files>JsDiagExceptionsInPromises_BreakOnFirstChanceExceptions.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.js.dbg.baseline</compile-flags>
//       <baseline>JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.baseline</baseline>
//       <files>JsDiagExceptionsInAsyncFunctions_BreakOnUncaughtExceptions.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagEvaluate.js.dbg.baseline</compile-flags>
//       <files>JsDiagEvaluate.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagGetFunctionPosition.js.dbg.baseline</compile-flags>
//       <files>JsDiagGetFunctionPosition.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagGetFunctionPositionIntl.js.dbg.baseline -Intl</compile-flags>
//       <files>JsDiagGetFunctionPositionIntl.js</files>
//       <!-- xplat-todo: enable on xplat when Intl is supported on xplat (Microsoft/ChakraCore#2919) -->
//       <tags>exclude_xplat,Intl</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagGetScripts.js.dbg.baseline</compile-flags>
//       <files>JsDiagGetScripts.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagGetStackProperties.js.dbg.baseline</compile-flags>
//       <files>JsDiagGetStackProperties.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagGetStackTrace.js.dbg.baseline</compile-flags>
//       <files>JsDiagGetStackTrace.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsDiagRequestAsyncBreak.js.dbg.baseline</compile-flags>
//       <files>JsDiagRequestAsyncBreak.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:JsrtDebugUtilsAddPropertyType.js.dbg.baseline</compile-flags>
//       <files>JsrtDebugUtilsAddPropertyType.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:MultipleContextStack.js.dbg.baseline</compile-flags>
//       <files>MultipleContextStack.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:dumpFunctionProperties.js.dbg.baseline</compile-flags>
//       <files>dumpFunctionProperties.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-dbgbaseline:emptyJson.dbg.baseline</compile-flags>
//       <files>loadscript_after_detach.js</files>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-dbgbaseline:JsDebuggerAttachDetach.js.dbg.baseline</compile-flags>
//       <files>JsDebuggerAttachDetach.js</files>
//       <baseline>JsDebuggerAttachDetach.js.baseline</baseline>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <compile-flags>-debuglaunch -dbgbaseline:exceptionWhileFetchingProp.js.dbg.baseline</compile-flags>
//       <files>exceptionWhileFetchingProp.js</files>
//     </default>
//   </test>
