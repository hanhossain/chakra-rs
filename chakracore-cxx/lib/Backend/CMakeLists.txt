if(CC_TARGETS_AMD64)
    set (CC_BACKEND_ARCH_FOLDER amd64)
    set (CC_BACKEND_ARCH_FILES
        amd64/EncoderMD.cpp
        amd64/LinearScanMD.cpp
        amd64/LowererMDArch.cpp
        amd64/PeepsMD.cpp
        amd64/PrologEncoderMD.cpp
        amd64/LinearScanMdA.S
        amd64/Thunks.S
        AgenPeeps.cpp
        EhFrame.cpp
        LowerMDShared.cpp
        LowerMDSharedSimd128.cpp
        PrologEncoder.cpp
    )
elseif(CC_TARGETS_ARM64)
    # JIT isn't currently supported on ARM64 linux/macos
    set (CC_BACKEND_ARCH_FOLDER arm64)
    set (CC_BACKEND_ARCH_FILES
        arm64/ARM64LogicalImmediates.cpp
        arm64/ARM64UnwindEncoder.cpp
        arm64/EncoderMD.cpp
        arm64/LegalizeMD.cpp
        arm64/LinearScanMD.cpp
        arm64/LinearScanMdA.S
        arm64/LowerMD.cpp
        arm64/PeepsMD.cpp
        arm64/Thunks.S
        arm64/UnwindInfoManager.cpp
    )
endif()

add_library (Chakra.Backend OBJECT
    AsmJsJITInfo.cpp
    Backend.cpp
    BackendApi.cpp
    BackendOpCodeAttrAsmJs.cpp
    BackwardPass.cpp
    BailOut.cpp
    CaseNode.cpp
    CodeGenAllocators.cpp
    CodeGenNumberAllocator.cpp
    CodeGenWorkItem.cpp
    DbCheckPostLower.cpp
    Debug.cpp
    EmitBuffer.cpp
    Encoder.cpp
    EquivalentTypeSet.cpp
    FixedFieldInfo.cpp
    FlowGraph.cpp
    Func.cpp
    FunctionCodeGenJitTimeData.cpp
    FunctionJITRuntimeInfo.cpp
    FunctionJITTimeInfo.cpp
    GlobOpt.cpp
    GlobOptArrays.cpp
    GlobOptBailOut.cpp
    GlobOptBlockData.cpp
    GlobOptExpr.cpp
    GlobOptFields.cpp
    GlobOptIntBounds.cpp
    IR.cpp
    IRBuilder.cpp
    IRBuilderAsmJs.cpp
    IRType.cpp
    IRViewer.cpp
    InductionVariable.cpp
    Inline.cpp
    InlineeFrameInfo.cpp
    InliningDecider.cpp
    InliningHeuristics.cpp
    IntBounds.cpp
    IntConstMath.cpp
    InterpreterThunkEmitter.cpp
    JavascriptNativeOperators.cpp
    JITThunkEmitter.cpp
    JITOutput.cpp
    JITTimeConstructorCache.cpp
    JITTimeFunctionBody.cpp
    JITTimePolymorphicInlineCache.cpp
    JITTimePolymorphicInlineCacheInfo.cpp
    JITTimeProfileInfo.cpp
    JITTimeWorkItem.cpp
    JitTransferData.cpp
    JITType.cpp
    JITTypeHandler.cpp
    JnHelperMethod.cpp
    LinearScan.cpp
    Lower.cpp
    NativeCodeData.cpp
    NativeCodeGenerator.cpp
    NativeEntryPointData.cpp
    ObjTypeSpecFldInfo.cpp
    Opnd.cpp
    PDataManager.cpp
    PageAllocatorPool.cpp
    Peeps.cpp
    PreLowerPeeps.cpp
    QueuedFullJitWorkItem.cpp
    Region.cpp
    SccLiveness.cpp
    Security.cpp
    ServerScriptContext.cpp
    ServerThreadContext.cpp
    SimpleJitProfilingHelpers.cpp
    SimpleLayout.cpp
    SwitchIRBuilder.cpp
    Sym.cpp
    SymTable.cpp
    TempTracker.cpp
    ValueInfo.cpp
    ValueRelativeOffset.cpp
    ${CC_BACKEND_ARCH_FILES}
    )

# TODO (hanhossain): remove most directory paths
#target_include_directories (
#    Chakra.Backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#    ${CC_BACKEND_ARCH_FOLDER}
#    ../Common
#    ../JITIDL
#    ../Runtime
#    ../Runtime/ByteCode
#    ../Runtime/Math
#    ../Parser
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
#    )
