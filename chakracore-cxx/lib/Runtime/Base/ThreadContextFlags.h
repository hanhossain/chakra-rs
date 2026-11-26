#pragma once

enum ThreadContextFlags {
    ThreadContextFlagNoFlag                        = 0x00000000,
    ThreadContextFlagCanDisableExecution           = 0x00000001,
    ThreadContextFlagEvalDisabled                  = 0x00000002,
    ThreadContextFlagNoJIT                         = 0x00000004,
    ThreadContextFlagDisableFatalOnOOM             = 0x00000008,
    ThreadContextFlagNoDynamicThunks               = 0x00000010,
};
