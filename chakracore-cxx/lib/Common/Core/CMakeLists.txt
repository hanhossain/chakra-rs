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

# TODO (hanhossain): remove most directory paths
#target_include_directories (
#    Chakra.Common.Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#    ${CHAKRACORE_SOURCE_DIR}/pal
#    ${CHAKRACORE_SOURCE_DIR}/pal/inc
#    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
#    ${CHAKRACORE_SOURCE_DIR}/pal/src
#    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
#    ${CHAKRACORE_SOURCE_DIR}/bin
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
#    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common
#    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
#    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
#    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
#)
