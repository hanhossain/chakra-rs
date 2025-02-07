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
const DIRECTORY: &str = "chakracore-cxx/test/TTExecuteBasic";

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>callbackSingle.js</files>
//       <baseline>callbackSingleRecord.baseline</baseline>
//       <compile-flags>-TTRecord=callbackSingleTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>callbackSingleReplay.baseline</baseline>
//       <compile-flags>-TTReplay=callbackSingleTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>callbackSpread.js</files>
//       <baseline>callbackSpreadRecord.baseline</baseline>
//       <compile-flags>-TTRecord=callbackSpreadTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>callbackSpreadReplay.baseline</baseline>
//       <compile-flags>-TTReplay=callbackSpreadTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>callbackSequence.js</files>
//       <baseline>callbackSequenceRecord.baseline</baseline>
//       <compile-flags>-TTRecord=callbackSequenceTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>callbackSequenceReplay.baseline</baseline>
//       <compile-flags>-TTReplay=callbackSequenceTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>callbackClear.js</files>
//       <baseline>callbackClearRecord.baseline</baseline>
//       <compile-flags>-TTRecord=callbackClearTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>callbackClearReplay.baseline</baseline>
//       <compile-flags>-TTReplay=callbackClearTest</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>enumerable.js</files>
//       <baseline>enumerableRecord.baseline</baseline>
//       <compile-flags>-TTRecord=enumerableTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>enumerableReplay.baseline</baseline>
//       <compile-flags>-TTReplay=enumerableTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>enumeratingWithES5.js</files>
//       <baseline>enumeratingWithES5Record.baseline</baseline>
//       <compile-flags>-TTRecord=enumeratingWithES5Test -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>enumeratingWithES5Replay.baseline</baseline>
//       <compile-flags>-TTReplay=enumeratingWithES5Test -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>enumerationAddDelete.js</files>
//       <baseline>enumerationAddDeleteRecord.baseline</baseline>
//       <compile-flags>-TTRecord=enumerationAddDeleteTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>enumerationAddDeleteReplay.baseline</baseline>
//       <compile-flags>-TTReplay=enumerationAddDeleteTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forEach.js</files>
//       <baseline>forEachRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forEachTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forEachReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forEachTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forInArrayAdd.js</files>
//       <baseline>forInArrayAddRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forInArrayAddTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forInArrayAddReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forInArrayAddTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forInObjectAdd.js</files>
//       <baseline>forInObjectAddRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forInObjectAddTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forInObjectAddReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forInObjectAddTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forInObjectAddDelete.js</files>
//       <baseline>forInObjectAddDeleteRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forInObjectAddDeleteTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forInObjectAddDeleteReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forInObjectAddDeleteTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>forInObjectDelete.js</files>
//       <baseline>forInObjectDeleteRecord.baseline</baseline>
//       <compile-flags>-TTRecord=forInObjectDeleteTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>forInObjectDeleteReplay.baseline</baseline>
//       <compile-flags>-TTReplay=forInObjectDeleteTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>symbolFor.js</files>
//       <baseline>symbolForRecord.baseline</baseline>
//       <compile-flags>-TTRecord=symbolForTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>symbolForReplay.baseline</baseline>
//       <compile-flags>-TTReplay=symbolForTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>try.js</files>
//       <baseline>tryRecord.baseline</baseline>
//       <compile-flags>-TTRecord=tryTest -TTSnapInterval=0</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>

// TODO (hanhossain): migrate
//   <test>
//     <default>
//       <files>ttdSentinal.js</files>
//       <baseline>tryReplay.baseline</baseline>
//       <compile-flags>-TTReplay=tryTest -TTDStartEvent=2</compile-flags>
//       <tags>exclude_dynapogo,exclude_snap,exclude_serialized</tags>
//     </default>
//   </test>
