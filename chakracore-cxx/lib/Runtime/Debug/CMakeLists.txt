add_library (Chakra.Runtime.Debug STATIC
    BreakpointProbe.cpp
    DebugContext.cpp
    DebugDocument.cpp
    DebugManager.cpp
    DebuggingFlags.cpp
    DiagHelperMethodWrapper.cpp
    DiagObjectModel.cpp
    DiagProbe.cpp
    DiagStackFrame.cpp
    MutationBreakpoint.cpp
    ProbeContainer.cpp
    RuntimeDebugPch.cpp
    TTActionEvents.cpp
    TTEventLog.cpp
    TTEvents.cpp
    TTExecutionInfo.cpp
    TTInflateMap.cpp
    TTRuntimeInfoTracker.cpp
    TTSerialize.cpp
    TTSnapObjects.cpp
    TTSnapshot.cpp
    TTSnapshotExtractor.cpp
    TTSnapTypes.cpp
    TTSnapValues.cpp
    TTSupport.cpp
    )

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Runtime.Debug PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    # ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
    # ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    # ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    # ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    # ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
)

target_link_libraries(Chakra.Runtime.Debug PUBLIC
    Chakra.Common.PlatformAgnostic
)
