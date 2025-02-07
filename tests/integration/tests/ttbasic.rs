use crate::common;
use crate::common::Variant;
use rstest::rstest;
use std::collections::HashSet;

const COMMON_TAGS: [&str; 6] = [
    "sequential",
    "exclude_dynapogo",
    "exclude_snap",
    "exclude_serialized",
    "require_debugger",
    "exclude_sanitize_address",
];
const DIRECTORY: &str = "chakracore-cxx/test/TTBasic";

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>accessor.js</files>
//       <baseline>accessorRecord.baseline</baseline>
//       <compile-flags>-TTRecord=accessorTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>accessorReplay.baseline</baseline>
//       <compile-flags>-TTReplay=accessorTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>arguments.js</files>
//       <baseline>argumentsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=argumentsTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>argumentsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=argumentsTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>array.js</files>
//       <baseline>arrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=arrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>arrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=arrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>bind.js</files>
//       <baseline>bindRecord.baseline</baseline>
//       <compile-flags>-TTRecord=bindTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>bindReplay.baseline</baseline>
//       <compile-flags>-TTReplay=bindTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>boolean.js</files>
//       <baseline>booleanRecord.baseline</baseline>
//       <compile-flags>-TTRecord=booleanTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>booleanReplay.baseline</baseline>
//       <compile-flags>-TTReplay=booleanTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>boundFunction.js</files>
//       <baseline>boundFunctionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=boundFunctionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>boundFunctionReplay.baseline</baseline>
//       <compile-flags>-TTReplay=boundFunctionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>boxedObject.js</files>
//       <baseline>boxedObjectRecord.baseline</baseline>
//       <compile-flags>-TTRecord=boxedObjectTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>boxedObjectReplay.baseline</baseline>
//       <compile-flags>-TTReplay=boxedObjectTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>crossSiteMain.js</files>
//       <baseline>crossSiteRecord.baseline</baseline>
//       <compile-flags>-TTRecord=crossSiteTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>crossSiteReplay1.baseline</baseline>
//       <compile-flags>-TTReplay=crossSiteTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>crossSiteReplay2.baseline</baseline>
//       <compile-flags>-TTReplay=crossSiteTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>constructor.js</files>
//       <baseline>constructorRecord.baseline</baseline>
//       <compile-flags>-TTRecord=constructorTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>constructorReplay.baseline</baseline>
//       <compile-flags>-TTReplay=constructorTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>dateBasic.js</files>
//       <baseline>dateBasicRecord.baseline</baseline>
//       <compile-flags>-TTRecord=dateBasicTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>dateBasicReplay.baseline</baseline>
//       <compile-flags>-TTReplay=dateBasicTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>dateBasicReplay.baseline</baseline>
//       <compile-flags>-TTReplay=dateBasicTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>deleteArray.js</files>
//       <baseline>deleteArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=deleteArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>deleteArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=deleteArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>es5Array.js</files>
//       <baseline>es5ArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=es5ArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>es5ArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=es5ArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>es5Arguments.js</files>
//       <baseline>es5ArgumentsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=es5ArgumentsTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>es5ArgumentsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=es5ArgumentsTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>eval.js</files>
//       <baseline>evalRecord.baseline</baseline>
//       <compile-flags>-TTRecord=evalTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>evalReplay.baseline</baseline>
//       <compile-flags>-TTReplay=evalTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>extensible.js</files>
//       <baseline>extensibleRecord.baseline</baseline>
//       <compile-flags>-TTRecord=extensibleTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>extensibleReplay.baseline</baseline>
//       <compile-flags>-TTReplay=extensibleTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forInShadowing.js</files>
//       <baseline>forInShadowingRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forInShadowingTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forInShadowingReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forInShadowingTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>freeze.js</files>
//       <baseline>freezeRecord.baseline</baseline>
//       <compile-flags>-TTRecord=freezeTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>freezeReplay.baseline</baseline>
//       <compile-flags>-TTReplay=freezeTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>function.js</files>
//       <baseline>functionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=functionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>functionReplay.baseline</baseline>
//       <compile-flags>-TTReplay=functionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>largeAuxArray.js</files>
//       <baseline>largeAuxArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=largeAuxArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>largeAuxArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=largeAuxArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>loadReEntrant.js</files>
//       <baseline>loadReEntrantRecord.baseline</baseline>
//       <compile-flags>-TTRecord=loadReEntrantTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>loadReEntrantReplay.baseline</baseline>
//       <compile-flags>-TTReplay=loadReEntrantTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>loadReEntrantReplay.baseline</baseline>
//       <compile-flags>-TTReplay=loadReEntrantTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>map.js</files>
//       <baseline>mapRecord.baseline</baseline>
//       <compile-flags>-TTRecord=mapTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>mapReplay.baseline</baseline>
//       <compile-flags>-TTReplay=mapTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>missingArray.js</files>
//       <baseline>missingArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=missingArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>missingArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=missingArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>nativeArray.js</files>
//       <baseline>nativeArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=nativeArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>nativeArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=nativeArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>newFromArgs.js</files>
//       <baseline>newFromArgsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=newFromArgsTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>newFromArgsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=newFromArgsTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>newFunction.js</files>
//       <baseline>newFunctionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=newFunctionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>newFunctionReplay.baseline</baseline>
//       <compile-flags>-TTReplay=newFunctionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>number.js</files>
//       <baseline>numberRecord.baseline</baseline>
//       <compile-flags>-TTRecord=numberTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>numberReplay.baseline</baseline>
//       <compile-flags>-TTReplay=numberTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>numericPropertyIsEnumerable.js</files>
//       <baseline>numericPropertyIsEnumerableRecord.baseline</baseline>
//       <compile-flags>-TTRecord=numericPropertyIsEnumerableTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>numericPropertyIsEnumerableReplay.baseline</baseline>
//       <compile-flags>-TTReplay=numericPropertyIsEnumerableTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>object.js</files>
//       <baseline>objectRecord.baseline</baseline>
//       <compile-flags>-TTRecord=objectTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>objectReplay.baseline</baseline>
//       <compile-flags>-TTReplay=objectTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>popArrayImplicitCall.js</files>
//       <baseline>popArrayImplicitCallRecord.baseline</baseline>
//       <compile-flags>-TTRecord=popArrayImplicitCallTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>popArrayImplicitCallReplay.baseline</baseline>
//       <compile-flags>-TTReplay=popArrayImplicitCallTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>promise.js</files>
//       <baseline>promiseRecord.baseline</baseline>
//       <compile-flags>-TTRecord=promiseTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promiseReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promiseTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promiseReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promiseTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promiseReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promiseTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promiseReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promiseTest -TTDStartEvent=6</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promiseReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promiseTest -TTDStartEvent=7</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>promise_MultipleThenCalls.js</files>
//       <baseline>promise_MultipleThenCallsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=promise_MultipleThenCallsTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=3</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=6</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>promise_MultipleThenCallsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=promise_MultipleThenCallsTest -TTDStartEvent=7</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>proxy.js</files>
//       <baseline>proxyRecord.baseline</baseline>
//       <compile-flags>-TTRecord=proxyTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>proxyReplay.baseline</baseline>
//       <compile-flags>-TTReplay=proxyTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>regex.js</files>
//       <baseline>regexRecord.baseline</baseline>
//       <compile-flags>-TTRecord=regexTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>regexReplay.baseline</baseline>
//       <compile-flags>-TTReplay=regexTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>seal.js</files>
//       <baseline>sealRecord.baseline</baseline>
//       <compile-flags>-TTRecord=sealTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>sealReplay.baseline</baseline>
//       <compile-flags>-TTReplay=sealTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>scopedAccessors.js</files>
//       <baseline>scopedAccessorsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=scopedAccessorsTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>scopedAccessorsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=scopedAccessorsTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>scopeFunction.js</files>
//       <baseline>scopeFunctionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=scopeFunctionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>scopeFunctionReplay.baseline</baseline>
//       <compile-flags>-TTReplay=scopeFunctionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>set.js</files>
//       <baseline>setRecord.baseline</baseline>
//       <compile-flags>-TTRecord=setTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>setReplay.baseline</baseline>
//       <compile-flags>-TTReplay=setTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>shadowPrototype.js</files>
//       <baseline>shadowPrototypeRecord.baseline</baseline>
//       <compile-flags>-TTRecord=shadowPrototypeTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>shadowPrototypeReplay.baseline</baseline>
//       <compile-flags>-TTReplay=shadowPrototypeTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>sparseArray.js</files>
//       <baseline>sparseArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=sparseArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>sparseArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=sparseArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>string.js</files>
//       <baseline>stringRecord.baseline</baseline>
//       <compile-flags>-TTRecord=stringTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>stringReplay.baseline</baseline>
//       <compile-flags>-TTReplay=stringTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>symbol.js</files>
//       <baseline>symbolRecord.baseline</baseline>
//       <compile-flags>-TTRecord=symbolTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>symbolReplay.baseline</baseline>
//       <compile-flags>-TTReplay=symbolTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>symbolReplay.baseline</baseline>
//       <compile-flags>-TTReplay=symbolTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>typeConversions.js</files>
//       <baseline>typeConversionsRecord.baseline</baseline>
//       <compile-flags>-TTRecord=typeConversionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>typeConversionsReplay.baseline</baseline>
//       <compile-flags>-TTReplay=typeConversionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>typedArray.js</files>
//       <baseline>typedArrayRecord.baseline</baseline>
//       <compile-flags>-TTRecord=typedArrayTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>typedArrayReplay.baseline</baseline>
//       <compile-flags>-TTReplay=typedArrayTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>typePromotion.js</files>
//       <baseline>typePromotionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=typePromotionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>typePromotionReplay.baseline</baseline>
//       <compile-flags>-TTReplay=typePromotionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorBasic.js</files>
//       <baseline>generatorBasicRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorBasicTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorBasicReplay_1.baseline</baseline>
//       <compile-flags>-TTReplay=generatorBasicTest -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorBasicReplay_2.baseline</baseline>
//       <compile-flags>-TTReplay=generatorBasicTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorBasicReplay_3.baseline</baseline>
//       <compile-flags>-TTReplay=generatorBasicTest -TTDStartEvent=3</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorBasicReplay_4.baseline</baseline>
//       <compile-flags>-TTReplay=generatorBasicTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorBasicReplay_5.baseline</baseline>
//       <compile-flags>-TTReplay=generatorBasicTest -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorReturnYieldResult.js</files>
//       <baseline>generatorReturnYieldResultRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorReturnYieldResult -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorReturnYieldResultReplay_1.baseline</baseline>
//       <compile-flags>-TTReplay=generatorReturnYieldResult -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorReturnYieldResultReplay_2.baseline</baseline>
//       <compile-flags>-TTReplay=generatorReturnYieldResult -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorReturnYieldResultReplay_3.baseline</baseline>
//       <compile-flags>-TTReplay=generatorReturnYieldResult -TTDStartEvent=3</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorReturnYieldResultReplay_4.baseline</baseline>
//       <compile-flags>-TTReplay=generatorReturnYieldResult -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorIntParam.js</files>
//       <baseline>generatorIntParamRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorIntParamTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorIntParamReplay.baseline</baseline>
//       <compile-flags>-TTReplay=generatorIntParamTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorObjectParam.js</files>
//       <baseline>generatorObjectParamRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorObjectParamTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorObjectParamReplay.baseline</baseline>
//       <compile-flags>-TTReplay=generatorObjectParamTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorClassMethod.js</files>
//       <baseline>generatorClassMethodRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorClassMethodTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorClassMethodReplay.baseline</baseline>
//       <compile-flags>-TTReplay=generatorClassMethodTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorNested.js</files>
//       <baseline>generatorNestedRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorNestedTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorNestedReplay.baseline</baseline>
//       <compile-flags>-TTReplay=generatorNestedTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorRestoreCompletedGenerator.js</files>
//       <baseline>generatorRestoreCompletedGeneratorRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorRestoreCompletedGeneratorTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorRestoreCompletedGeneratorReplay.baseline</baseline>
//       <compile-flags>-TTReplay=generatorRestoreCompletedGeneratorTest -TTDStartEvent=6</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>generatorWriteLogDuringGeneratorExecution.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionRecord.baseline</baseline>
//       <compile-flags>-TTRecord=generatorWriteLogDuringGeneratorExecutionTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_1.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_2.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_3.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=3</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_4.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_5.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>generatorWriteLogDuringGeneratorExecutionReplay_6.baseline</baseline>
//       <compile-flags>-TTReplay=generatorWriteLogDuringGeneratorExecutionTest -TTDStartEvent=6</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>asyncAwaitBasic.js</files>
//       <baseline>asyncAwaitBasicRecord.baseline</baseline>
//       <compile-flags>-TTRecord=asyncAwaitBasicTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwaitBasicReplay_1.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwaitBasicTest -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwaitBasicReplay_2.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwaitBasicTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwaitBasicReplay_3.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwaitBasicTest -TTDStartEvent=3</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwaitBasicReplay_4.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwaitBasicTest -TTDStartEvent=4</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwaitBasicReplay_5.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwaitBasicTest -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>asyncAwait2.js</files>
//       <baseline>asyncAwait2Record.baseline</baseline>
//       <compile-flags>-TTRecord=asyncAwait2Test -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwait2Replay_1.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwait2Test -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwait2Replay_2.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwait2Test -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>asyncAwait3.js</files>
//       <baseline>asyncAwait3Record.baseline</baseline>
//       <compile-flags>-TTRecord=asyncAwait3Test -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwait3Replay_1.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwait3Test -TTDStartEvent=1</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//     <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>asyncAwait3Replay_2.baseline</baseline>
//       <compile-flags>-TTReplay=asyncAwait3Test -TTDStartEvent=5</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>
