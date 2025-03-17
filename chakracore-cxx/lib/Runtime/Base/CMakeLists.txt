add_library (Chakra.Runtime.Base OBJECT
    CallInfo.cpp
    CharStringCache.cpp
    Constants.cpp
    CrossSite.cpp
    Debug.cpp
    # DelayLoadLibrary.cpp
    # Entropy.cpp
    EtwTrace.cpp
    Exception.cpp
    ExpirableObject.cpp
    FunctionBody.cpp
    FunctionExecutionStateMachine.cpp
    FunctionInfo.cpp
    LeaveScriptObject.cpp
    LineOffsetCache.cpp
    PerfHint.cpp
    PropertyRecord.cpp
    RuntimeBasePch.cpp
    ScriptContext.cpp
    ScriptContextOptimizationOverrideInfo.cpp
    ScriptContextProfiler.cpp
    ScriptMemoryDumper.cpp
    SourceContextInfo.cpp
    SourceHolder.cpp
    StackProber.cpp
    TempArenaAllocatorObject.cpp
    TestEtwEventSink.cpp
    ThreadBoundThreadContextManager.cpp
    ThreadContextInfo.cpp
    ThreadContext.cpp
    ThreadContextTlsEntry.cpp
    ThreadServiceWrapperBase.cpp
    Utf8SourceInfo.cpp
    VTuneChakraProfile.cpp
    # WindowsFoundationAdapter.cpp
    WindowsGlobalizationAdapter.cpp
    jitprofiling.cpp
    )

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Runtime.Base PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
)

target_precompile_headers(Chakra.Runtime.Base PUBLIC RuntimeBasePch.h)
