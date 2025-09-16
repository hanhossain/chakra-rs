add_library (Chakra.Common.Core OBJECT
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

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Common.Core PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
)

target_compile_definitions(Chakra.Common.Core PUBLIC
    PAL_STDCPP_COMPAT
)

target_precompile_headers(Chakra.Common.Core PUBLIC CommonCorePch.h)
