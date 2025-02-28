add_library (Chakra.Common.Core STATIC
    Api.cpp
    BinaryFeatureControl.cpp
    CmdParser.cpp
    CodexAssert.cpp
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
# target_include_directories (
#     Chakra.Common.Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Chakra.Common.Core
    PUBLIC include)
target_link_libraries(Chakra.Common.Core
    PUBLIC Chakra.Pal)
