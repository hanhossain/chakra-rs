add_library (Chakra.Common.Core STATIC
    Api.cpp
    BinaryFeatureControl.cpp
    CmdParser.cpp
    CodexAssert.cpp
    CommonCorePch.cpp
    ConfigFlagsTable.cpp
    ConfigParser.cpp
    CRC.cpp
    DbgHelpSymbolManager.cpp
    DelayLoadLibrary.cpp
    EtwTraceCore.cpp
    FaultInjection.cpp
    GlobalSecurityPolicy.cpp
    Output.cpp
    PerfCounter.cpp
    PerfCounterImpl.cpp
    PerfCounterSet.cpp
    ProfileInstrument.cpp
    ProfileMemory.cpp
    StackBackTrace.cpp
    SysInfo.cpp
)

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.Core PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/pal
    # ${CHAKRACORE_SOURCE_DIR}/pal/inc
    # ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
    # ${CHAKRACORE_SOURCE_DIR}/lib/Backend
)
