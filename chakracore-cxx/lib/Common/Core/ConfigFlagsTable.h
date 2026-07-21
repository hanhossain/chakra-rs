//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <mutex>

#include "DataStructures/BaseDictionary.h"
#include "DataStructures/SList.h"
#include "Memory/HeapAllocator.h"

namespace Js
{
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum FlagTypes
    ///
    /// Different types of flags supported by the debug table
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum FlagTypes
    {
        InvalidFlagType,
        FlagString,
        FlagPhases,
        FlagNumber,
        FlagBoolean,
        FlagNumberSet,
        FlagNumberPairSet,
        FlagNumberTrioSet,
        FlagNumberRange
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum Flag
    ///
    /// All the flags available. The list of flags are described in the file
    /// ConfigFlagsList.h
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum Flag: unsigned short
    {
        #if DBG
        ArrayValidateFlag,
        #endif
        AsmJsFlag,
        AsmJsStopOnErrorFlag,
        AsmJsEdgeFlag,
        WasmFlag,
        WasmI64Flag,
        WasmFastArrayFlag,
        WasmSharedArrayVirtualBufferFlag,
        WasmMathExFilterFlag,
        WasmCheckVersionFlag,
        WasmAssignModuleIDFlag,
        WasmIgnoreLimitsFlag,
        WasmFoldFlag,
        WasmIgnoreResponseFlag,
        WasmMaxTableSizeFlag,
        WasmThreadsFlag,
        WasmMultiValueFlag,
        WasmSignExtendsFlag,
        WasmNontrappingFlag,

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        WasmExperimentalFlag,

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        WasmSimdFlag,

        AssertBreakFlag,
        AssertPopUpFlag,
        AssertIgnoreFlag,
        AsyncDebuggingFlag,
        BailOnNoProfileLimitFlag,
        BailOnNoProfileRejitLimitFlag,
        BaselineModeFlag,
        DumpOnCrashFlag,
        FullMemoryDumpFlag,
        BailOutFlag,
        BailOutAtEveryLineFlag,
        BailOutAtEveryByteCodeFlag,
        BailOutAtEveryImplicitCallFlag,
        BailOutByteCodeFlag,
        BenchmarkFlag,
        BgJitFlag,
        BgParseFlag,
        BgJitDelayFlag,
        BgJitDelayFgBufferFlag,
        BgJitPendingFuncCapFlag,

        CreateFunctionProxyFlag,
        HybridFgJitFlag,
        HybridFgJitBgQueueLengthThresholdFlag,
        BytecodeHistFlag,
        CurrentSourceInfoFlag,
        CFGLogFlag,
        CheckAlignmentFlag,
        CheckEmitBufferPermissionsFlag,
        #ifdef CHECK_MEMORY_LEAK
        CheckMemoryLeakFlag,
        DumpOnLeakFlag,
        #endif
        CheckOpHelpersFlag,
        CloneInlinedPolymorphicCachesFlag,
        ConcurrentRuntimeFlag,
        ConstructorInlineThresholdFlag,
        ConstructorCallsRequiredToFinalizeCachedTypeFlag,
        PropertyCacheMissPenaltyFlag,
        PropertyCacheMissThresholdFlag,
        PropertyCacheMissResetFlag,
        DebugFlag,
        DebugBreakFlag,
        StatementDebugBreakFlag,
        DebugBreakOnPhaseBeginFlag,

        DebugWindowFlag,
        ParserStateCacheFlag,
        CompressParserStateCacheFlag,
        DeferTopLevelTillFirstCallFlag,
        DeferParseFlag,
        DirectCallTelemetryStatsFlag,
        DisableArrayBTreeFlag,
        DisableRentalThreadingFlag,
        DisableVTuneSourceLineInfoFlag,
        DisplayMemStatsFlag,
        DumpFlag,
        #ifdef DUMP_FRAGMENTATION_STATS
        DumpFragmentationStatsFlag,
        #endif
        DumpIRAddressesFlag,
        DumpLineNoInColorFlag,
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        DumpObjectGraphOnExitFlag,
        DumpObjectGraphOnCollectFlag,
        #endif
        DumpEvalStringOnRemovalFlag,
        DumpObjectGraphOnEnumFlag,
        #ifdef DYNAMIC_PROFILE_STORAGE
        DynamicProfileCacheFlag, DpcFlag,
        DynamicProfileCacheDirFlag,
        DynamicProfileInputFlag, DpiFlag,
        #endif
        #ifdef EDIT_AND_CONTINUE
        EditTestFlag,
        #endif
        WininetProfileCacheFlag,
        NoDynamicProfileInMemoryCacheFlag,
        ProfileBasedSpeculativeJitFlag,
        ProfileBasedSpeculationCapFlag,
        ExecuteByteCodeBufferReturnsInvalidByteCodeFlag,
        ExpirableCollectionGCCountFlag,
        ExpirableCollectionTriggerThresholdFlag,
        SkipSplitOnNoResultFlag,
        Force32BitByteCodeFlag,

        CollectGarbageFlag,

        IntlFlag,
        IntlBuiltInsFlag,
        IntlPlatformFlag,

        JsBuiltInFlag,
        JitReproFlag,
        EntryPointInfoRpcDataFlag,

        LdChakraLibFlag,
        TestChakraLibFlag,

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        ES6Flag,

        // Master ES6 flag to enable ALL sub ES6 features/flags
        ES6AllFlag,

        // Master ES6 flag to enable Threshold ES6 features/flags
        ES6ExperimentalFlag,

        // Per ES6 feature/flag

        ES7AsyncAwaitFlag,
        ES6DateParseFixFlag,
        ES6FunctionNameFullFlag,
        ES6GeneratorsFlag,
        ES7ExponentiationOperatorFlag,

        ES7ValuesEntriesFlag,
        ES7TrailingCommaFlag,
        ES6IsConcatSpreadableFlag,
        ES6MathFlag,
        ESDynamicImportFlag,

        ES6ModuleFlag,
        ES6ObjectFlag,
        ES6NumberFlag,
        ES6ObjectLiteralsFlag,
        ES6ProxyFlag,
        ES6RestFlag,
        ES6SpreadFlag,
        ES6StringFlag,
        ES6StringPrototypeFixesFlag,
        ES2018ObjectRestSpreadFlag,

        ES6PrototypeChainFlag,
        ES6ToPrimitiveFlag,
        ES6ToLengthFlag,
        ES6ToStringTagFlag,
        ES6UnicodeFlag,
        ES6UnicodeVerboseFlag,
        ES6UnscopablesFlag,
        ES6RegExStickyFlag,
        ES2018RegExDotAllFlag,
        ESExportNsAsFlag,
        ES2018AsyncIterationFlag,
        ESTopLevelAwaitFlag,
        ES6RegExPrototypePropertiesFlag,

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        ES6RegExSymbolsFlag,

        ES6VerboseFlag,
        ESObjectGetOwnPropertyDescriptorsFlag,

        ESSharedArrayBufferFlag,

        // Newer language feature flags

        // ES BigInt flag
        ESBigIntFlag,

        // ES Numeric Separator support for numeric constants
        ESNumericSeparatorFlag,

        // ES Nullish coalescing operator support (??)
        ESNullishCoalescingOperatorFlag,

        // ES Hashbang support for interpreter directive syntax
        ESHashbangFlag,

        // ES Symbol.prototype.description flag
        ESSymbolDescriptionFlag,

        ESArrayFindFromLastFlag,

        // ES Promise.any and AggregateError flag
        ESPromiseAnyFlag,

        // ES import.meta keyword meta-property
        ESImportMetaFlag,

        //ES globalThis flag
        ESGlobalThisFlag,

        // This flag to be removed once JITing generator functions is stable
        JitES6GeneratorsFlag,

        FastLineColumnCalculationFlag,
        FilenameFlag,
        FreeRejittedCodeFlag,
        ForceGuardPagesFlag,
        PrintGuardPageBoundsFlag,
        ForceLegacyEngineFlag,
        ForceFlag,
        StressFlag,
        ForceArrayBTreeFlag,
        StrongArraySortFlag,
        ForceCleanPropertyOnCollectFlag,
        ForceCleanCacheOnCollectFlag,
        ForceGCAfterJSONParseFlag,
        ForceDecommitOnCollectFlag,
        ForceDeferParseFlag,
        ForceDiagnosticsModeFlag,
        ForceGetWriteWatchOOMFlag,
        ForcePostLowerGlobOptInstrStringFlag,
        ForceSplitScopeFlag,
        EnumerateSpecialPropertiesInDebuggerFlag,
        EnableContinueAfterExceptionWrappersForHelpersFlag,
        EnableContinueAfterExceptionWrappersForBuiltInsFlag,
        EnableFunctionSourceReportForHeapEnumFlag,
        ForceFragmentAddressSpaceFlag,
        ForceOOMOnEBCommitFlag,
        ForceDynamicProfileFlag,
        ForceES5ArrayFlag,
        ForceAsmJsLinkFailFlag,
        ForceExpireOnNonCacheCollectFlag,
        ForceFastPathFlag,
        ForceFloatPrefFlag,
        ForceJITLoopBodyFlag,
        ForceStaticInterpreterThunkFlag,
        DumpCommentsFromReferencedFilesFlag,
        DelayFullJITSmallFuncFlag,
        EnableFatalErrorOnOOMFlag,

        #if defined(_M_ARM32_OR_ARM64)
        ForceLocalsPtrFlag,
        #endif
        DeferLoadingAvailableSourceFlag,
        ForceNativeFlag,
        ForceSerializedFlag,
        ForceSerializedBytecodeMajorVersionFlag,
        ForceSerializedBytecodeVersionSchemaFlag,
        ForceStrictModeFlag,
        ForceUndoDeferFlag,
        ForceBlockingConcurrentCollectFlag,
        FreTestDiagModeFlag,
        #ifdef BYTECODE_TESTING
        ByteCodeBranchLimitFlag,
        MediumByteCodeLayoutFlag,
        LargeByteCodeLayoutFlag,
        #endif
        InduceCodeGenFailureFlag,
        InduceCodeGenFailureSeedFlag,
        InjectPartiallyInitializedInterpreterFrameErrorFlag,
        InjectPartiallyInitializedInterpreterFrameErrorTypeFlag,
        GenerateByteCodeBufferReturnsCantGenerateFlag,
        GoptCleanupThresholdFlag,
        AsmGoptCleanupThresholdFlag,
        HighPrecisionDateFlag,
        InlineCountMaxFlag,
        InlineCountMaxInLoopBodiesFlag, icminlbFlag,
        InlineInLoopBodyScaleDownFactorFlag, iilbsdfFlag,
        InlineThresholdFlag,
        AggressiveInlineCountMaxFlag,
        AggressiveInlineThresholdFlag,
        InlineThresholdAdjustCountInLargeFunctionFlag,
        InlineThresholdAdjustCountInMediumSizedFunctionFlag,
        InlineThresholdAdjustCountInSmallFunctionFlag,
        AsmJsInlineAdjustFlag,
        InterpretFlag,
        InstrumentFlag,
        JitQueueThresholdFlag,
        LoopInlineThresholdFlag,
        LeafInlineThresholdFlag,
        ConstantArgumentInlineThresholdFlag,
        RecursiveInlineThresholdFlag,
        RecursiveInlineDepthMaxFlag,
        RecursiveInlineDepthMinFlag,
        RedeferralCapFlag,
        LoopFlag,
        LoopInterpretCountFlag, licFlag,
        LoopProfileIterationsFlag,
        OutsideLoopInlineThresholdFlag,
        MaxFuncInlineDepthFlag,
        MaxNumberOfInlineesWithLoopFlag,
        #ifdef MEMSPECT_TRACKING
        MemspectFlag,
        #endif
        PolymorphicInlineThresholdFlag,
        PrimeRecyclerFlag,
        TraceEngineRefcountFlag,
        #if defined(CHECK_MEMORY_LEAK)
        LeakStackTraceFlag,
        ForceMemoryLeakFlag,
        #endif
        DumpAfterFinalGCFlag,
        ForceOldDateAPIFlag,

        JitLoopBodyHotLoopThresholdFlag,
        LoopBodySizeThresholdToDisableOptsFlag,

        MaxJitThreadCountFlag,
        ForceMaxJitThreadCountFlag,

        MitigateSpectreFlag,

        AddMaskingBlocksFlag,

        PoisonVarArrayLoadFlag,
        PoisonIntArrayLoadFlag,
        PoisonFloatArrayLoadFlag,
        PoisonTypedArrayLoadFlag,
        PoisonStringLoadFlag,
        PoisonObjectsForLoadsFlag,

        PoisonVarArrayStoreFlag,
        PoisonIntArrayStoreFlag,
        PoisonFloatArrayStoreFlag,
        PoisonTypedArrayStoreFlag,
        PoisonStringStoreFlag,
        PoisonObjectsForStoresFlag,

        MinInterpretCountFlag,
        MinSimpleJitRunCountFlag,
        MaxInterpretCountFlag, MicFlag,
        MaxSimpleJitRunCountFlag, MsjrcFlag,
        MinMemOpCountFlag, MmocFlag,

        #if ENABLE_COPYONACCESS_ARRAY
        MaxCopyOnAccessArrayLengthFlag,
        MinCopyOnAccessArrayLengthFlag,
        CopyOnAccessArraySegmentCacheSizeFlag,
        #endif

        MinTemplatizedJitRunCountFlag,
        MinAsmJsInterpreterRunCountFlag,

        MinTemplatizedJitLoopRunCountFlag,
        MaxTemplatizedJitRunCountFlag, MtjrcFlag,
        MaxAsmJsInterpreterRunCountFlag, MaicFlag,

        AutoProfilingInterpreter0LimitFlag,
        ProfilingInterpreter0LimitFlag,
        AutoProfilingInterpreter1LimitFlag,
        SimpleJitLimitFlag,
        ProfilingInterpreter1LimitFlag,

        ExecutionModeLimitsFlag, EmlFlag,
        EnforceExecutionModeLimitsFlag, EemlFlag,

        SimpleJitAfterFlag, SjaFlag,
        FullJitAfterFlag, FjaFlag,

        NewSimpleJitFlag,

        MaxLinearIntCaseCountFlag,
        MaxSingleCharStrJumpTableSizeFlag,
        MaxSingleCharStrJumpTableRatioFlag,
        MinSwitchJumpTableSizeFlag,
        MaxLinearStringCaseCountFlag,
        MinDeferredFuncTokenCountFlag,
        #if DBG
        SkipFuncCountForBailOnNoProfileFlag,
        #endif
        MaxJITFunctionBytecodeByteLengthFlag,
        MaxJITFunctionBytecodeCountFlag,
        MaxLoopsPerFunctionFlag,
        FuncObjectInlineCacheThresholdFlag,
        NoDeferParseFlag,
        NoLogoFlag,
        OOPJITMissingOptsFlag,
        CrashOnOOPJITFailureFlag,
        OOPCFGRegistrationFlag,
        ForceJITCFGCheckFlag,
        UseJITTrampolineFlag,
        NoNativeFlag,
        NopFrequencyFlag,
        NoStrictModeFlag,
        NormalizeStatsFlag,
        OffFlag,
        OffProfiledByteCodeFlag,
        OnFlag,
        #ifdef ENABLE_TRACE
        InMemoryTraceFlag,
        InMemoryTraceBufferSizeFlag,
        #ifdef STACK_BACK_TRACE
        TraceWithStackFlag,
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        PrintRunTimeDataCollectionTraceFlag,
        #ifdef ENABLE_PREJIT
        PrejitFlag,
        #endif
        PrintSrcInDumpFlag,
        #if PROFILE_DICTIONARY
        ProfileDictionaryFlag,
        #endif
        #ifdef PROFILE_EXEC
        ProfileFlag,
        ProfileThresholdFlag,
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        ProfileObjectLiteralFlag,
        #endif
        #ifdef PROFILE_MEM
        ProfileMemoryFlag,
        #endif
        #ifdef PROFILE_STRINGS
        ProfileStringsFlag,
        #endif
        #ifdef PROFILE_TYPES
        ProfileTypesFlag,
        #endif
        #ifdef PROFILE_EVALMAP
        ProfileEvalMapFlag,
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        ProfileBailOutRecordMemoryFlag,
        #endif

        #if DBG
        ValidateIntRangesFlag,
        #endif
        RejitMaxBailOutCountFlag,
        CallsToBailoutsRatioForRejitFlag,
        LoopIterationsToBailoutsRatioForRejitFlag,
        MinBailOutsBeforeRejitFlag,
        MinBailOutsBeforeRejitForLoopsFlag,
        LibraryStackFrameFlag,
        LibraryStackFrameDebuggerFlag,
        #ifdef RECYCLER_STRESS
        RecyclerStressFlag,
        RecyclerBackgroundStressFlag,
        RecyclerConcurrentStressFlag,
        RecyclerConcurrentRepeatStressFlag,
        RecyclerPartialStressFlag,
        RecyclerTrackStressFlag,
        RecyclerInduceFalsePositivesFlag,
        #endif // RECYCLER_STRESS
        RecyclerForceMarkInteriorFlag,
        RecyclerPriorityBoostTimeoutFlag,
        RecyclerThreadCollectTimeoutFlag,
        EnableConcurrentSweepAllocFlag, ecsaFlag,
        PageHeapFlag,
        PageHeapAllocStackFlag,
        PageHeapFreeStackFlag,
        PageHeapBucketNumberFlag,
        PageHeapBlockTypeFlag,
        PageHeapDecommitGuardPageFlag,
        #ifdef RECYCLER_NO_PAGE_REUSE
        RecyclerNoPageReuseFlag,
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
        RecyclerVerifyFlag,
        RecyclerVerifyPadSizeFlag,
        #endif
        RecyclerTestFlag,
        RecyclerProtectPagesOnRescanFlag,
        #ifdef RECYCLER_VERIFY_MARK
        RecyclerVerifyMarkFlag,
        #endif
        LowMemoryCapFlag,
        NewPagesCapDuringBGSweepingFlag,
        AllocPolicyLimitFlag,
        RuntimeDataOutputFileFlag,
        SpeculationCapFlag,
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        StatsFlag,
        #endif
        #if EXCEPTION_RECOVERY
        SwallowExceptionsFlag,
        #endif
        PrintSystemExceptionFlag,
        SwitchOptHolesThresholdFlag,
        TempMinFlag,
        TempMaxFlag,
        TraceFlag,

        #if defined(_M_X64)
        LoopAlignNopLimitFlag,
        #endif

        #ifdef PROFILE_MEM
        TraceMemoryFlag,
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        TraceMetaDataParsingFlag,
        TraceWin8AllocationsFlag,
        TraceWin8DeallocationsImmediateFlag,
        PrintWin8StatsDetailedFlag,
        TraceProtectPagesFlag,
        #endif
        TraceAsyncDebugCallsFlag,
        #ifdef TRACK_DISPATCH
        TrackDispatchFlag,
        #endif
        VerboseFlag,
        UseFullNameFlag,
        Utf8Flag,
        VersionFlag,
        WERExceptionSupportFlag,
        ExtendedErrorStackForTestHostFlag,
        errorStackTraceFlag,
        DoHeapEnumOnEngineShutdownFlag,
        #ifdef HEAP_ENUMERATION_VALIDATION
        ValidateHeapEnumFlag,
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        RegexTracingFlag,
        RegexProfileFlag,
        RegexDebugFlag,
        RegexDebugASTFlag,
        RegexDebugAnnotatedASTFlag,
        RegexBytecodeDebugFlag,
        RegexOptimizeFlag,
        DynamicRegexMruListSizeFlag,
        #endif

        OptimizeForManyInstancesFlag,
        EnableArrayTypeMutationFlag,
        ArrayMutationTestSeedFlag,
        TestTraceFlag,
        EnableEvalMapCleanupFlag,
        #ifdef PROFILE_MEM
        TraceObjectAllocationFlag,
        #endif
        SseFlag,
        DeletedPropertyReuseThresholdFlag,
        ForceStringKeyedSimpleDictionaryTypeHandlerFlag,
        BigDictionaryTypeHandlerThresholdFlag,
        TypeSnapshotEnumerationFlag,
        IsolatePrototypesFlag,
        ChangeTypeOnProtoFlag,
        ShareInlineCachesFlag,
        DisableDebugObjectFlag,
        DumpHeapFlag,
        autoProxyFlag,
        PerfHintLevelFlag,
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        MemProtectHeapFlag,
        #endif
        #ifdef RECYCLER_STRESS
        MemProtectHeapStressFlag,
        MemProtectHeapBackgroundStressFlag,
        MemProtectHeapConcurrentStressFlag,
        MemProtectHeapConcurrentRepeatStressFlag,
        MemProtectHeapPartialStressFlag,
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        FixPropsOnPathTypesFlag,
        #endif
        BailoutTraceFilterFlag,
        RejitTraceFilterFlag,

        // recycler heuristic flags
        MaxBackgroundFinishMarkCountFlag,
        BackgroundFinishMarkWaitTimeFlag,
        MinBackgroundRepeatMarkRescanBytesFlag,

        #if defined(_M_X64)
        ZeroMemoryWithNonTemporalStoreFlag,
        #endif

        // recycler memory restrict test flags
        MaxMarkStackPageCountFlag,
        MaxTrackedObjectListCountFlag,

        // make the recycler page integration path easier to hit
        NumberAllocPlusSizeFlag,

        #if DBG
        InitializeInterpreterSlotsWithInvalidStackVarFlag,
        #endif

        #if DBG
        PRNGSeed0Flag,
        PRNGSeed1Flag,
        #endif

        ClearInlineCachesOnCollectFlag,
        InlineCacheInvalidationListCompactionThresholdFlag,
        ConstructorCacheInvalidationThresholdFlag,

        GCMemoryThresholdFlag,

        #if DBG
            SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag,
        #endif

        JITServerIdleTimeoutFlag,
        JITServerMaxInactivePageAllocatorCountFlag,

        StrictWriteBarrierCheckFlag,
        WriteBarrierTestFlag,
        ForceSoftwareWriteBarrierFlag,
        VerifyBarrierBitFlag,
        EnableBGFreeZeroFlag,
        KeepRecyclerTrackDataFlag,

        MaxSingleAllocSizeInMBFlag,

        FlagCount,
        InvalidFlag,
        NoParentFlag,
        FlagMax = NoParentFlag
    } ;


    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// enum Phase
    ///
    /// Different Phases/Subphases of the backend. The list of phases is described
    /// in the file ConfigFlagsList.h
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    enum Phase: unsigned short
    {
    AllPhase,
        BGJitPhase,
        ModulePhase,
        LibInitPhase,
            JsLibInitPhase,
        ParsePhase,
            RegexCompilePhase,
            DeferParsePhase,
            RedeferralPhase,
            DeferEventHandlersPhase,
            FunctionSourceInfoParsePhase,
            StringTemplateParsePhase,
            CreateParserStatePhase,
            SkipNestedDeferredPhase,
            CacheScopeInfoNamesPhase,
            ScanAheadPhase,
            ParallelParsePhase,
            EarlyReferenceErrorsPhase,
            EarlyErrorOnAssignToCallPhase,
            BgParsePhase,
        ByteCodePhase,
            CachedScopePhase,
            StackFuncPhase,
            StackClosurePhase,
            DisableStackFuncOnDeferredEscapePhase,
            DelayCapturePhase,
            DebuggerScopePhase,
            ByteCodeSerializationPhase,
                VariableIntEncodingPhase,
            NativeCodeSerializationPhase,
            OptimizeBlockScopePhase,
        DelayPhase,
            SpeculationPhase,
            GatherCodeGenDataPhase,
        WasmPhase,
            // Wasm frontend
            WasmBytecodePhase, // Supports -off,-dump,-trace,-profile
            WasmReaderPhase, // Support -trace,-profile
            WasmSectionPhase, // Supports -trace
            WasmOpCodeDistributionPhase, // Support -dump
            // Wasm features per functions
            WasmDeferredPhase,
            WasmValidatePrejitPhase,
            WasmInOutPhase, // Trace input and output of wasm calls
            WasmMemWritesPhase, // Trace memory writes
        AsmjsPhase,
            AsmjsTmpRegisterAllocationPhase,
            AsmjsEncoderPhase,
            AsmjsInterpreterPhase,
            AsmJsJITTemplatePhase,
            AsmjsFunctionEntryPhase,
            AsmjsInterpreterStackPhase,
            AsmjsEntryPointInfoPhase,
            AsmjsCallDebugBreakPhase,
        BackEndPhase,
            IRBuilderPhase,
                SwitchOptPhase,
                BailOnNoProfilePhase,
                BackendConcatExprOptPhase,
                ClosureRangeCheckPhase,
                ClosureRegCheckPhase,
            InlinePhase,
                InlineRecursivePhase,
                InlineAtEveryCallerPhase,      //Inlines a function, say, foo at every caller of foo. Doesn't guarantee all the calls within foo are inlined too.
                InlineTreePhase,               //Inlines every function within a top function, say, foo (which needs to be top function) Note: -force:inline achieves the effect of both -force:InlineTree & -force:InlineAtEveryCaller
                TryAggressiveInliningPhase,
                InlineConstructorsPhase,
                InlineBuiltInPhase,
                InlineInJitLoopBodyPhase,
                InlineAccessorsPhase,
                InlineGettersPhase,
                InlineSettersPhase,
                InlineApplyPhase,
                InlineApplyTargetPhase,
                InlineApplyWithoutArrayArgPhase,
                InlineAnyCallApplyTargetPhase,
                BailOutOnNotStackArgsPhase,
                InlineCallPhase,
                InlineCallTargetPhase,
                PartialPolymorphicInlinePhase,
                PolymorphicInlinePhase,
                PolymorphicInlineFixedMethodsPhase,
                InlineOutsideLoopsPhase,
                InlineFunctionsWithLoopsPhase,
                EliminateArgoutForInlineePhase,
                InlineBuiltInCallerPhase,
                InlineArgsOptPhase,
                    RemoveInlineFramePhase,
                InlinerConstFoldPhase,
                InlineCallbacksPhase,
        ExecBOIFastPathPhase,
            FGBuildPhase,
                OptimizeTryFinallyPhase,
                RemoveBreakBlockPhase,
                TailDupPhase,
            FGPeepsPhase,
            GlobOptPhase,
                PathDepBranchFoldingPhase,
                OptimizeTryCatchPhase,
                CaptureByteCodeRegUsePhase,
                BackwardPhase,
                    TrackIntUsagePhase,
                    TrackNegativeZeroPhase,
                    TypedArrayVirtualPhase,
                    TrackIntOverflowPhase,
                    TrackCompoundedIntOverflowPhase,
                ForwardPhase,
                    ValueTablePhase,
                    ValueNumberingPhase,
                    AVTInPrePassPhase,
                    PathDependentValuesPhase,
                        TrackRelativeIntBoundsPhase,
                            BoundCheckEliminationPhase,
                                BoundCheckHoistPhase,
                                    LoopCountBasedBoundCheckHoistPhase,
                    CopyPropPhase,
                        ObjPtrCopyPropPhase,
                    ConstPropPhase,
                    Int64ConstPropPhase,
                    ConstFoldPhase,
                    CSEPhase,
                    HoistConstIntPhase,
                    TypeSpecPhase,
                    AggressiveIntTypeSpecPhase,
                    AggressiveMulIntTypeSpecPhase,
                    LossyIntTypeSpecPhase,
                    FloatTypeSpecPhase,
                    StringTypeSpecPhase,
                    InductionVarsPhase,
                    InvariantsPhase,
                    FieldCopyPropPhase,
                    FieldPREPhase,
                    MakeObjSymLiveInLandingPadPhase,
                    HostOptPhase,
                    ObjTypeSpecPhase,
                        ObjTypeSpecNewObjPhase,
                        ObjTypeSpecIsolatedFldOpsPhase,
                        ObjTypeSpecIsolatedFldOpsWithBailOutPhase,
                        ObjTypeSpecStorePhase,
                        EquivObjTypeSpecPhase,
                        EquivObjTypeSpecByDefaultPhase,
                        TraceObjTypeSpecTypeGuardsPhase,
                        TraceObjTypeSpecWriteGuardsPhase,
                        LiveOutFieldsPhase,
                        DisabledObjTypeSpecPhase,
                        DepolymorphizeInlineesPhase,
                        ReuseAuxSlotPtrPhase,
                        PolyEquivTypeGuardPhase,
                        DeadStoreTypeChecksOnStoresPhase,
                        #if DBG
                            SimulatePolyCacheWithOneTypeForFunctionPhase,
                        #endif
                    CheckThisPhase,
                    StackArgOptPhase,
                    StackArgFormalsOptPhase,
                    StackArgLenConstOptPhase,
                    IndirCopyPropPhase,
                    ArrayCheckHoistPhase,
                        ArrayMissingValueCheckHoistPhase,
                        ArraySegmentHoistPhase,
                            JsArraySegmentHoistPhase,
                        ArrayLengthHoistPhase,
                        EliminateArrayAccessHelperCallPhase,
                    NativeArrayPhase,
                        NativeNewScArrayPhase,
                        NativeArrayConversionPhase,
                        CopyOnAccessArrayPhase,
                        NativeArrayLeafSegmentPhase,
                    TypedArrayTypeSpecPhase,
                    LdLenIntSpecPhase,
                    FixDataPropsPhase,
                    FixMethodPropsPhase,
                    FixAccessorPropsPhase,
                    FixDataVarPropsPhase,
                    UseFixedDataPropsPhase,
                    UseFixedDataPropsInInlinerPhase,
                    LazyBailoutPhase,
                        LazyBailoutOnImplicitCallsPhase,
                        LazyFixedDataBailoutPhase,
                        LazyFixedTypeBailoutPhase,
                    FixedMethodsPhase,
                        FEFixedMethodsPhase,
                        FixedFieldGuardCheckPhase,
                        FixedNewObjPhase,
                            JitAllocNewObjPhase,
                        FixedCtorInliningPhase,
                        FixedCtorCallsPhase,
                        FixedScriptMethodInliningPhase,
                        FixedScriptMethodCallsPhase,
                        FixedBuiltInMethodInliningPhase,
                        FixedBuiltInMethodCallsPhase,
                        SplitNewScObjectPhase,
                    OptTagChecksPhase,
                    MemOpPhase,
                        MemSetPhase,
                        MemCopyPhase,
                    IncrementalBailoutPhase,
                DeadStorePhase,
                    ReverseCopyPropPhase,
                    MarkTempPhase,
                        MarkTempNumberPhase,
                        MarkTempObjectPhase,
                        MarkTempNumberOnTempObjectPhase,
                    SpeculationPropagationAnalysisPhase,
            DumpGlobOptInstrPhase, // Print the Globopt instr string in post lower dumps
            LowererPhase,
                FastPathPhase,
                    LoopFastPathPhase,
                    MathFastPathPhase,
                    AtomPhase,
                        MulStrengthReductionPhase,
                        AgenPeepsPhase,
                    BranchFastPathPhase,
                    CallFastPathPhase,
                    BitopsFastPathPhase,
                    OtherFastPathPhase,
                    ObjectFastPathPhase,
                    ProfileBasedFldFastPathPhase,
                    AddFldFastPathPhase,
                    RootObjectFldFastPathPhase,
                    ArrayLiteralFastPathPhase,
                    ArrayCtorFastPathPhase,
                    NewScopeSlotFastPathPhase,
                    FrameDisplayFastPathPhase,
                    HoistMarkTempInitPhase,
                    HoistConstAddrPhase,
                JitWriteBarrierPhase,
                PreLowererPeepsPhase,
                CFGInJitPhase,
                TypedArrayPhase,
                TracePinnedTypesPhase,
            InterruptProbePhase,
            EncodeConstantsPhase,
            RegAllocPhase,
                LivenessPhase,
                    RegParamsPhase,
                LinearScanPhase,
                    OpHelperRegOptPhase,
                    StackPackPhase,
                    SecondChancePhase,
                    RegionUseCountPhase,
                    RegHoistLoadsPhase,
                    ClearRegLoopExitPhase,
            PeepsPhase,
            LayoutPhase,
            EHBailoutPatchUpPhase,
            FinalLowerPhase,
            PrologEpilogPhase,
            InsertNOPsPhase,
            EncoderPhase,
                AssemblyPhase,
                EmitterPhase,
                DebugBreakPhase,
    #if defined(_M_X64)
                BrShortenPhase,
                    LoopAlignPhase,
    #endif

    #if DBG_DUMP
        SWBPhase,
    #endif
        RunPhase,
            InterpreterPhase,
            EvalCompilePhase,
                FastIndirectEvalPhase,
            IdleDecommitPhase,
            IdleCollectPhase,
            MarshalPhase,
            MemoryAllocationPhase,
                PageHeapPhase,
                LargeMemoryAllocationPhase,
                PageAllocatorAllocPhase,
            RecyclerPhase,
                ThreadCollectPhase,
                ExplicitFreePhase,
                ExpirableCollectPhase,
                GarbageCollectPhase,
                ConcurrentCollectPhase,
                    BackgroundResetMarksPhase,
                    BackgroundFindRootsPhase,
                    BackgroundRescanPhase,
                    BackgroundRepeatMarkPhase,
                    BackgroundFinishMarkPhase,
                ConcurrentPartialCollectPhase,
                ParallelMarkPhase,
                PartialCollectPhase,
                    ResetMarksPhase,
                    ResetWriteWatchPhase,
                    FindRootPhase,
                        FindRootArenaPhase,
                        FindImplicitRootPhase,
                        FindRootExtPhase,
                    ScanStackPhase,
                    ConcurrentMarkPhase,
                    ConcurrentWaitPhase,
                    RescanPhase,
                    MarkPhase,
                    SweepPhase,
                        SweepWeakPhase,
                        SweepSmallPhase,
                        SweepLargePhase,
                        SweepPartialReusePhase,
                    ConcurrentSweepPhase,
                    FinalizePhase,
                    DisposePhase,
                    FinishPartialPhase,
            HostPhase,
            BailOutPhase,
            BailInPhase,
            GeneratorGlobOptPhase,
            RegexQcPhase,
            RegexOptBTPhase,
            InlineCachePhase,
            PolymorphicInlineCachePhase,
            MissingPropertyCachePhase,
            PropertyCachePhase, // Trace caching of property lookups using PropertyString and JavascriptSymbol
            CloneCacheInCollisionPhase,
            ConstructorCachePhase,
            InlineCandidatePhase,
            ScriptFunctionWithInlineCachePhase,
            IsConcatSpreadableCachePhase,
            ArenaPhase,
            ApplyUsagePhase,
            ObjectHeaderInliningPhase,
                ObjectHeaderInliningForConstructorsPhase,
                ObjectHeaderInliningForObjectLiteralsPhase,
                ObjectHeaderInliningForEmptyObjectsPhase,
            OptUnknownElementNamePhase,
            TypePropertyCachePhase,
    #if DBG_DUMP
            InlineSlotsPhase,
    #endif
            DynamicProfilePhase,
    #ifdef DYNAMIC_PROFILE_STORAGE
            DynamicProfileStoragePhase,
    #endif
            JITLoopBodyPhase,
            JITLoopBodyInTryCatchPhase,
            JITLoopBodyInTryFinallyPhase,
            ReJITPhase,
            ExecutionModePhase,
            SimpleJitDynamicProfilePhase,
            SimpleJitPhase,
            FullJitPhase,
            FailNativeCodeInstallPhase,
            PixelArrayPhase,
            EtwPhase,
            ProfilerPhase,
            CustomHeapPhase,
            XDataAllocatorPhase,
            PageAllocatorPhase,
            StringConcatPhase,
    #if DBG_DUMP
            PRNGPhase,
    #endif
            PreReservedHeapAllocPhase,
            CFGPhase,
            ExceptionStackTracePhase,
            ExtendedExceptionInfoStackTracePhase,
            TypeHandlerTransitionPhase,
            DebuggerPhase,
                ENCPhase,
            ConsoleScopePhase,
            ScriptProfilerPhase,
            JSONPhase,
            IntlPhase,
            RegexResultNotUsedPhase,
            ErrorPhase,
            PropertyRecordPhase,
            TypePathDynamicSizePhase,
            ObjectCopyPhase,
            ShareTypesWithAttributesPhase,
            ShareAccessorTypesPhase,
            ShareFuncTypesPhase,
            ShareCrossSiteFuncTypesPhase,
            ConditionalCompilationPhase,
            InterpreterProfilePhase,
            InterpreterAutoProfilePhase,
            ByteCodeConcatExprOptPhase,
            TraceInlineCacheInvalidationPhase,
            TracePropertyGuardsPhase,
            PerfHintPhase,
            TypeShareForChangePrototypePhase,
            DeferSourceLoadPhase,
            DataCachePhase,
            ObjectMutationBreakpointPhase,
            NativeCodeDataPhase,
            XDataPhase,
        PhaseCount,
        InvalidPhase
    };

    extern const char16_t* const FlagNames[FlagCount + 1];
    extern const char16_t* const PhaseNames[PhaseCount + 1];
    extern const Flag           FlagParents[FlagCount + 1];

    typedef     int             Number;
    typedef     bool            Boolean;

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class String
    ///
    /// A simple string wrapper for storing strings for the flags. It creates a
    /// copy of the string which it releases in the destructor. Simple operator
    /// fiddling to make this class as interoperable with strings as possible :-
    ///
    ///     1.  Single argument constructor takes care of initialization
    ///     2.  Assignment operator overloaded for char16_t*
    ///     3.  JavascriptConversion to const char16_t * is defined. Making this constant ensures
    ///         that a non constant char16_t * does not point to our buffer which can
    ///         potentially corrupt it.
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class String
    {
        PREVENT_COPYCONSTRUCT(String)


    // Data
    private:
        char16_t*           pszValue;

    // Construction
    public:
        inline String();
        inline String(__in_z_opt const char16_t* psz);
        inline ~String();


    // Methods
    public:

        ///----------------------------------------------------------------------------
        ///
        /// Assignment operator defined for easy access
        ///
        ///----------------------------------------------------------------------------

        String& operator=(__in_z_opt const char16_t* psz)
        {
            Set(psz);
            return *this;
        }

        String &operator =(const String &other)
        {
            return (*this = other.pszValue);
        }

        ///----------------------------------------------------------------------------
        ///
        /// Typecast operator defined so we can directly use it with strings
        ///
        ///----------------------------------------------------------------------------

        operator const char16_t* () const
        {
            return this->pszValue;
        }

    // Implementation
    private:
        void Set(__in_z_opt const char16_t* pszValue);
    };

    class NumberSet
    {
    public:
        NumberSet();
        void Add(uint32_t x);
        bool Contains(uint32_t x);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<uint32_t, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    class NumberPair
    {
    public:
        NumberPair(uint32_t x, uint32_t y) : x(x), y(y) {}
        NumberPair() : x(static_cast<uint32_t>(-1)), y(static_cast<uint32_t>(-1)) {}

        operator hash_t() const { return (x << 16) + y; }
        bool operator ==(const NumberPair &other) const { return x == other.x && y == other.y; }
    private:
        uint32_t x;
        uint32_t y;
    };

    class NumberPairSet
    {
    public:
        NumberPairSet();
        void Add(uint32_t x, uint32_t y);
        bool Contains(uint32_t x, uint32_t y);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<NumberPair, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    class NumberTrio
    {
    public:
        NumberTrio(uint32_t x, uint32_t y, uint32_t z) : x(x), y(y), z(z) {}
        NumberTrio() : x(static_cast<uint32_t>(-1)), y(static_cast<uint32_t>(-1)) {}

        operator hash_t() const { return (x << 20) + (y << 10) + z; }
        bool operator ==(const NumberTrio &other) const { return x == other.x && y == other.y && z == other.z; }
    private:
        uint32_t x;
        uint32_t y;
        uint32_t z;
    };

    class NumberTrioSet
    {
    public:
        NumberTrioSet();
        void Add(uint32_t x, uint32_t y, uint32_t z);
        bool Contains(uint32_t x, uint32_t y, uint32_t z);
        bool Empty() const { return set.Count() == 0; }
    private:
        JsUtil::BaseHashSet<NumberTrio, NoCheckHeapAllocator, PrimeSizePolicy> set;
    };

    struct SourceFunctionNode
    {
        uint sourceContextId;
        Js::LocalFunctionId functionId;
        SourceFunctionNode(uint sourceContextId, Js::LocalFunctionId functionId) : sourceContextId(sourceContextId), functionId(functionId) {}
        SourceFunctionNode() : sourceContextId(0), functionId(0){}
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// struct RangeUnit
    ///
    /// Used to store a pair of bounds
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    template <typename TRangeUnitData>
    struct RangeUnit
    {
        TRangeUnitData i, j;
        RangeUnit() {}
        RangeUnit(TRangeUnitData x, TRangeUnitData y) :
            i(x),
            j(y)
        {}
    };

    template <typename TRangeUnitData>
    bool RangeUnitContains(RangeUnit<TRangeUnitData> unit, TRangeUnitData item)
    {
        return (item >= unit.i && item <= unit.j);
    }

    template <>
    bool RangeUnitContains<SourceFunctionNode>(RangeUnit<SourceFunctionNode> unit, SourceFunctionNode n);

    template <typename TRangeUnitData>
    RangeUnit<TRangeUnitData> GetFullRange()
    {
        RangeUnit<TRangeUnitData> unit;
        unit.i = INT_MIN;
        unit.j = INT_MAX;
        return unit;
    }

    template <>
    RangeUnit<SourceFunctionNode> GetFullRange();

    template <typename TRangeUnitData>
    TRangeUnitData GetPrevious(TRangeUnitData unit)
    {
        return unit - 1;
    }

    template <>
    SourceFunctionNode GetPrevious(SourceFunctionNode unit);

    template <typename TRangeUnitData>
    TRangeUnitData GetNext(TRangeUnitData unit)
    {
        return unit + 1;
    }

    template <>
    SourceFunctionNode GetNext(SourceFunctionNode unit);

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Range
    ///
    /// Data structure to store a range of integers. (Can trivially be templatized
    /// but resisting the temptation for the sake of simplicity.)
    ///
    /// Sample:     1,3-11,15,20
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    template <typename TRangeUnitData>
    class RangeBase
    {
    // Nested Classes
    public:
        typedef RangeUnit<TRangeUnitData> Unit;


    // Data
    protected:
        SList<Unit, NoCheckHeapAllocator>      range;

    // Construction
    public:
        RangeBase() : range(&NoCheckHeapAllocator::Instance) {}

    // Methods
    public:
        inline  bool            InRange(TRangeUnitData i);
        inline  bool            ContainsAll();
        inline  void            Add(TRangeUnitData i, RangeBase<TRangeUnitData>* oppositeRange = nullptr);
        inline  void            Add(TRangeUnitData i, TRangeUnitData j, RangeBase<TRangeUnitData>* oppositeRange = nullptr);
        inline  void            Clear();

#if DBG_RANGE
        template <typename TFunction>
        void Map(TFunction fn) const
        {
            range.Map(fn);
        }
#endif
    };

    class Range : public RangeBase<SourceFunctionNode> {};
    class NumberRange : public RangeBase<int> {};

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Phases
    ///
    /// Maintains the list of all the phases. Each individual phase can be enabled
    /// and an optional range of numbers can be added to it
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class Phases
    {
    // Nested Types
    private:

        ///----------------------------------------------------------------------------
        ///----------------------------------------------------------------------------
        ///
        /// struct UnitPhase
        ///
        /// Stores information about a particular phase. Current information is :
        ///     1.  Is the phase enabled/disabled?
        ///     2.  A range of numbers for the given phase. This range can represent a
        ///         variety of information. For example, this range can be the function
        ///         numbers of all the functions where a particular optimization is
        ///         enabled or where opcode dump is enabled.
        ///
        ///
        ///  Its a tiny private struct used by Phases for simply storing data. So
        ///  making this a struct with all members public.
        ///
        ///----------------------------------------------------------------------------
        ///----------------------------------------------------------------------------

        struct UnitPhase
        {

        // Construction
        public:
            UnitPhase() : valid(false) { }

        // Data
        public:
                bool            valid;
                Range           range;

        };

    // Data
    private:
                UnitPhase       phaseList[PhaseCount];

    // Constructors
    public:
        Phases() {}

    // Methods
    public:

        void            Enable(Phase phase);
        void            Disable(Phase phase);
        bool            IsEnabled(Phase phase);
        bool            IsEnabled(Phase phase, uint sourceContextId, Js::LocalFunctionId functionId);
        bool            IsEnabledForAll(Phase phase);
        Range *         GetRange(Phase phase);
        Phase           GetFirstPhase();
    };

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class ConfigFlagsTable
    ///
    /// Maintains the list of all the flags. Individual flags can be accessed by :-
    ///
    ///     1.  flagTable->verbose;     // here verbose is the name of the flag
    ///                                 // It will be strongly typed with the
    ///                                 // type of the flag is as described in
    ///                                 // ConfigFlagsTable.h
    ///
    ///     2.  flagTable->GetAsBoolean(verboseFlag);
    ///                                 // this access is weekly typed. Even if the
    ///                                 // flag is not of type Boolean, it will be
    ///                                 // reinterpret_cast to Boolean *
    ///
    /// The former access should be used in the code for reading the flag values
    /// while the later is useful for adding values into the table.
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    class ConfigFlagsTable
    {

    // Constructor
    public:
        ConfigFlagsTable();

    // Methods
    public:

        static  Flag            GetFlag(const char16_t* str);
        static  Phase           GetPhase(const char16_t* str);
        static  void            PrintUsageString();

        static  FlagTypes       GetFlagType(Flag flag);

                String*         GetAsString(Flag flag) const;
                Phases*         GetAsPhase(Flag flag) const;
                Flag            GetOppositePhaseFlag(Flag flag) const;
                Boolean*        GetAsBoolean(Flag flag) const;
                Number*         GetAsNumber(Flag flag) const;
                NumberSet*      GetAsNumberSet(Flag flag) const;
                NumberPairSet * GetAsNumberPairSet(Flag flag) const;
                NumberTrioSet * GetAsNumberTrioSet(Flag flag) const;
                NumberRange *   GetAsNumberRange(Flag flag) const;

                void            SetAsBoolean(Flag flag, Boolean value);

                Boolean         GetDefaultValueAsBoolean(Flag flag) const;

        // indicates whether a flag is a parent flag
                bool            IsParentFlag(Flag flag) const;

        // get the parent flag of a given flag, if any, otherwise returns InvalidFlag.
                Flag            GetParentFlag(Flag flag) const;

        // get the next child flag of a given parent flag, if any, otherwise returns InvalidFlag. Pass InvalidFlag as currentChildFlag if no current child flag to iterate from.
                Flag            GetNextChildFlag(Flag parentFlag, Flag currentChildFlag) const;

                void            Enable(Flag flag);
                bool            IsEnabled(Flag flag);
                void            Disable(Flag flag);

                // Data
    public:

        ///----------------------------------------------------------------------------
        ///
        /// Declaration of each flag as a member variable of the corresponding type.
        /// These variables are made public because get/set are too ugly for simple
        /// flags. Besides there are flags like phase lists which are not simple and
        /// its better to have a uniform way to access them.
        ///
        ///     if(ConfigFlag->dump)     is much better than
        ///     if(ConfigFlag->GetDump())
        ///
        ///----------------------------------------------------------------------------

        #if DBG
            Boolean ArrayValidate;
        #endif
        Boolean AsmJs;
        // TODO: remove
        Boolean AsmJsStopOnError;
        // TODO: remove
        Boolean AsmJsEdge;
        Boolean Wasm;
        Boolean WasmI64;
        Boolean WasmFastArray;
        Boolean WasmSharedArrayVirtualBuffer;
        Boolean WasmMathExFilter;
        // TODO: remove
        Boolean WasmCheckVersion;
        Boolean WasmAssignModuleID;
        // TODO: remove
        Boolean WasmIgnoreLimits;
        // TODO: remove
        Boolean WasmFold;
        // TODO: remove
        Boolean WasmIgnoreResponse;
        // TODO: remove
        Number WasmMaxTableSize;
        Boolean WasmThreads;
        Boolean WasmMultiValue;
        Boolean WasmSignExtends;
        Boolean WasmNontrapping;

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        // TODO: remove
        Boolean WasmExperimental;

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        Boolean WasmSimd;

        // TODO: remove
        Boolean AssertBreak;
        // TODO: remove
        Boolean AssertPopUp;
        // TODO: remove
        Boolean AssertIgnore;
        // TODO: remove
        Boolean AsyncDebugging;
        // TODO: remove
        Number BailOnNoProfileLimit;
        // TODO: remove
        Number BailOnNoProfileRejitLimit;
        Boolean BaselineMode;
        // TODO: remove
        String DumpOnCrash;
        // TODO: remove
        String FullMemoryDump;
            NumberPairSet BailOut;
        // TODO: remove
            Boolean BailOutAtEveryLine;
            Boolean BailOutAtEveryByteCode;
        // TODO: remove
            Boolean BailOutAtEveryImplicitCall;
            NumberSet BailOutByteCode;
        // TODO: remove
        Boolean Benchmark;
        Boolean BgJit;
        // TODO: remove
        Boolean BgParse;
        Number BgJitDelay;
        // TODO: remove
        Number BgJitDelayFgBuffer;
        // TODO: remove
        Number BgJitPendingFuncCap;

        // TODO: remove
        Boolean CreateFunctionProxy;
        // TODO: remove
        Boolean HybridFgJit;
        // TODO: remove
        Number HybridFgJitBgQueueLengthThreshold;
        // TODO: remove
        Boolean BytecodeHist;
        // TODO: remove
        Boolean CurrentSourceInfo;
        // TODO: remove
        Boolean CFGLog;
        Boolean CheckAlignment;
        // TODO: remove
        Boolean CheckEmitBufferPermissions;
        #ifdef CHECK_MEMORY_LEAK
        // TODO: remove
            Boolean CheckMemoryLeak;
        // TODO: remove
            String DumpOnLeak;
        #endif
        // TODO: remove
        Boolean CheckOpHelpers;
        // TODO: remove
        Boolean CloneInlinedPolymorphicCaches;
        // TODO: remove
        Boolean ConcurrentRuntime;
        // TODO: remove
        Number ConstructorInlineThreshold;
        // TODO: remove
        Number ConstructorCallsRequiredToFinalizeCachedType;
        // TODO: remove
        Number PropertyCacheMissPenalty;
        // TODO: remove
        Number PropertyCacheMissThreshold;
        // TODO: remove
        Number PropertyCacheMissReset;
        // TODO: remove
        Boolean Debug;
        // TODO: remove
        NumberSet DebugBreak;
        // TODO: remove
        NumberTrioSet StatementDebugBreak;
        // TODO: remove
        Phases DebugBreakOnPhaseBegin;

        // TODO: remove
        Boolean DebugWindow;
        Boolean ParserStateCache;
        // TODO: remove
        Boolean CompressParserStateCache;
        Boolean DeferTopLevelTillFirstCall;
        Number DeferParse;
        // TODO: remove
        Boolean DirectCallTelemetryStats;
        // TODO: remove
        Boolean DisableArrayBTree;
        // TODO: remove
        Boolean DisableRentalThreading;
        // TODO: remove
        Boolean DisableVTuneSourceLineInfo;
        // TODO: remove
        Boolean DisplayMemStats;
        Phases Dump;
        #ifdef DUMP_FRAGMENTATION_STATS
        // TODO: remove
            Boolean DumpFragmentationStats;
        #endif
        // TODO: remove
        Boolean DumpIRAddresses;
        // TODO: remove
        Boolean DumpLineNoInColor;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        // TODO: remove
            Boolean DumpObjectGraphOnExit;
        // TODO: remove
            Boolean DumpObjectGraphOnCollect;
        #endif
        // TODO: remove
        Boolean DumpEvalStringOnRemoval;
        // TODO: remove
        Boolean DumpObjectGraphOnEnum;
        #ifdef DYNAMIC_PROFILE_STORAGE
            String DynamicProfileCache;
        // TODO: remove
        String Dpc;
        // TODO: remove
            String DynamicProfileCacheDir;
            String DynamicProfileInput;
        // TODO: remove
        String Dpi;
        #endif
        #ifdef EDIT_AND_CONTINUE
        // TODO: remove
            Boolean EditTest;
        #endif
        // TODO: remove
        Boolean WininetProfileCache;
        // TODO: remove
        Boolean NoDynamicProfileInMemoryCache;
        // TODO: remove
        Boolean ProfileBasedSpeculativeJit;
        // TODO: remove
        Number ProfileBasedSpeculationCap;
        // TODO: remove
        Boolean ExecuteByteCodeBufferReturnsInvalidByteCode;
        // TODO: remove
        Number ExpirableCollectionGCCount;
        // TODO: remove
        Number ExpirableCollectionTriggerThreshold;
        Boolean SkipSplitOnNoResult;
        // TODO: remove
        Boolean Force32BitByteCode;

        Boolean CollectGarbage;

        Boolean Intl;
        // TODO: remove
        Boolean IntlBuiltIns;
        Boolean IntlPlatform;

        Boolean JsBuiltIn;
        // TODO: remove
        Boolean JitRepro;
        // TODO: remove
        Boolean EntryPointInfoRpcData;

        // TODO: remove
        Boolean LdChakraLib;
        Boolean TestChakraLib;

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        Boolean ES6;

        // Master ES6 flag to enable ALL sub ES6 features/flags
        Boolean ES6All;

        // Master ES6 flag to enable Threshold ES6 features/flags
        Boolean ES6Experimental;

        // Per ES6 feature/flag

        Boolean ES7AsyncAwait;
        // TODO: remove
        Boolean ES6DateParseFix;
        Boolean ES6FunctionNameFull;
        Boolean ES6Generators;
        Boolean ES7ExponentiationOperator;

        // TODO: remove
        Boolean ES7ValuesEntries;
        // TODO: remove
        Boolean ES7TrailingComma;
        // TODO: remove
        Boolean ES6IsConcatSpreadable;
        Boolean ES6Math;

        Boolean ESDynamicImport;

        Boolean ES6Module;
        // TODO: remove
        Boolean ES6Object;
        Boolean ES6Number;
        Boolean ES6ObjectLiterals;
        // TODO: remove
        Boolean ES6Proxy;
        Boolean ES6Rest;
        Boolean ES6Spread;
        Boolean ES6String;
        Boolean ES6StringPrototypeFixes;
        Boolean ES2018ObjectRestSpread;

        Boolean ES6PrototypeChain;
        // TODO: remove
        Boolean ES6ToPrimitive;
        Boolean ES6ToLength;
        Boolean ES6ToStringTag;
        Boolean ES6Unicode;
        // TODO: remove
        Boolean ES6UnicodeVerbose;
        // TODO: remove
        Boolean ES6Unscopables;
        Boolean ES6RegExSticky;
        Boolean ES2018RegExDotAll;
        Boolean ESExportNsAs;
        Boolean ES2018AsyncIteration;
        Boolean ESTopLevelAwait;

        Boolean ES6RegExPrototypeProperties;

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        Boolean ES6RegExSymbols;

        // TODO: remove
        Boolean ES6Verbose;
        Boolean ESObjectGetOwnPropertyDescriptors;

        Boolean ESSharedArrayBuffer;

        // Newer language feature flags

        // ES BigInt flag
        Boolean ESBigInt;

        // ES Numeric Separator support for numeric constants
        Boolean ESNumericSeparator;

        // ES Nullish coalescing operator support (??)
        // TODO: remove
        Boolean ESNullishCoalescingOperator;

        // ES Hashbang support for interpreter directive syntax
        Boolean ESHashbang;

        // ES Symbol.prototype.description flag
        Boolean ESSymbolDescription;

        // TODO: remove
        Boolean ESArrayFindFromLast;

        // ES Promise.any and AggregateError flag
        // TODO: remove
        Boolean ESPromiseAny;

        // ES import.meta keyword meta-property
        Boolean ESImportMeta;

        //ES globalThis flag
        Boolean ESGlobalThis;

        // This flag to be removed once JITing generator functions is stable
        Boolean JitES6Generators;

        // TODO: remove
        Boolean FastLineColumnCalculation;
        // TODO: remove
        String Filename;
        // TODO: remove
        Boolean FreeRejittedCode;
        // TODO: remove
        Boolean ForceGuardPages;
        // TODO: remove
        Boolean PrintGuardPageBounds;
        // TODO: remove
        Boolean ForceLegacyEngine;
        Phases Force;
        Phases Stress;
        Boolean ForceArrayBTree;
        // TODO: remove
        Boolean StrongArraySort;
        // TODO: remove
        Boolean ForceCleanPropertyOnCollect;
        // TODO: remove
        Boolean ForceCleanCacheOnCollect;
        Boolean ForceGCAfterJSONParse;
        // TODO: remove
        Boolean ForceDecommitOnCollect;
        Boolean ForceDeferParse;
        // TODO: remove
        Boolean ForceDiagnosticsMode;
        // TODO: remove
        Boolean ForceGetWriteWatchOOM;
        // TODO: remove
        Boolean ForcePostLowerGlobOptInstrString;
        // TODO: remove
        Boolean ForceSplitScope;
        // TODO: remove
        Boolean EnumerateSpecialPropertiesInDebugger;
        // TODO: remove
        Boolean EnableContinueAfterExceptionWrappersForHelpers;
        // TODO: remove
        Boolean EnableContinueAfterExceptionWrappersForBuiltIns;
        // TODO: remove
        Boolean EnableFunctionSourceReportForHeapEnum;
        // TODO: remove
        Number ForceFragmentAddressSpace;
        // TODO: remove
        Number ForceOOMOnEBCommit;
        // TODO: remove
        Boolean ForceDynamicProfile;
        Boolean ForceES5Array;
        Boolean ForceAsmJsLinkFail;
        // TODO: remove
        Boolean ForceExpireOnNonCacheCollect;
        // TODO: remove
        Boolean ForceFastPath;
        // TODO: remove
        Boolean ForceFloatPref;
        Boolean ForceJITLoopBody;
        Boolean ForceStaticInterpreterThunk;
        // TODO: remove
        Boolean DumpCommentsFromReferencedFiles;
        // TODO: remove
        Number DelayFullJITSmallFunc;
        Boolean EnableFatalErrorOnOOM;

        #if defined(_M_ARM32_OR_ARM64)
        // TODO: remove
            Boolean ForceLocalsPtr;
        #endif
        // TODO: remove
        Boolean DeferLoadingAvailableSource;
        Boolean ForceNative;
        Boolean ForceSerialized;
        // TODO: remove
        Number ForceSerializedBytecodeMajorVersion;
        // TODO: remove
        Number ForceSerializedBytecodeVersionSchema;
        Boolean ForceStrictMode;
        Boolean ForceUndoDefer;
        // TODO: remove
        Boolean ForceBlockingConcurrentCollect;
        // TODO: remove
        Boolean FreTestDiagMode;
        #ifdef BYTECODE_TESTING
        // TODO: remove
            Number ByteCodeBranchLimit;
        // TODO: remove
            Boolean MediumByteCodeLayout;
        // TODO: remove
            Boolean LargeByteCodeLayout;
        #endif
        // TODO: remove
        Number InduceCodeGenFailure;
        // TODO: remove
        Number InduceCodeGenFailureSeed;
        Number InjectPartiallyInitializedInterpreterFrameError;
        Number InjectPartiallyInitializedInterpreterFrameErrorType;
        // TODO: remove
        Boolean GenerateByteCodeBufferReturnsCantGenerate;
        Number GoptCleanupThreshold;
        // TODO: remove
        Number AsmGoptCleanupThreshold;
        Boolean HighPrecisionDate;
        // TODO: remove
        Number InlineCountMax;
        // TODO: remove
        Number InlineCountMaxInLoopBodies;
        // TODO: remove
        Number icminlb;
        Number InlineInLoopBodyScaleDownFactor;
        // TODO: remove
        Number iilbsdf;
        // TODO: remove
        Number InlineThreshold;
        // TODO: remove
        Number AggressiveInlineCountMax;
        // TODO: remove
        Number AggressiveInlineThreshold;
        // TODO: remove
        Number InlineThresholdAdjustCountInLargeFunction;
        // TODO: remove
        Number InlineThresholdAdjustCountInMediumSizedFunction;
        // TODO: remove
        Number InlineThresholdAdjustCountInSmallFunction;
        // TODO: remove
        Number AsmJsInlineAdjust;
        // TODO: remove
        String Interpret;
        // TODO: remove
        Phases Instrument;
        // TODO: remove
        Number JitQueueThreshold;
        // TODO: remove
        Number LoopInlineThreshold;
        // TODO: remove
        Number LeafInlineThreshold;
        // TODO: remove
        Number ConstantArgumentInlineThreshold;
        // TODO: remove
        Number RecursiveInlineThreshold;
        // TODO: remove
        Number RecursiveInlineDepthMax;
        // TODO: remove
        Number RecursiveInlineDepthMin;
        // TODO: remove
        Number RedeferralCap;
        // TODO: remove
        Number Loop;
        Number LoopInterpretCount;
        Number lic;
        // TODO: remove
        Number LoopProfileIterations;
        // TODO: remove
        Number OutsideLoopInlineThreshold;
        // TODO: remove
        Number MaxFuncInlineDepth;
        // TODO: remove
        Number MaxNumberOfInlineesWithLoop;
        #ifdef MEMSPECT_TRACKING
        // TODO: remove
            Phases Memspect;
        #endif
        // TODO: remove
        Number PolymorphicInlineThreshold;
        // TODO: remove
        Boolean PrimeRecycler;
        // TODO: remove
        Boolean TraceEngineRefcount;
        #if defined(CHECK_MEMORY_LEAK)
        // TODO: remove
            Boolean LeakStackTrace;
        // TODO: remove
            Boolean ForceMemoryLeak;
        #endif
        // TODO: remove
        Boolean DumpAfterFinalGC;
        Boolean ForceOldDateAPI;

        // TODO: remove
        Number JitLoopBodyHotLoopThreshold;
        // TODO: remove
        Number LoopBodySizeThresholdToDisableOpts;

        // TODO: remove
        Number MaxJitThreadCount;
        // TODO: remove
        Boolean ForceMaxJitThreadCount;

        // TODO: remove
        Boolean MitigateSpectre;

        // TODO: remove
        Boolean AddMaskingBlocks;

        // TODO: remove
        Boolean PoisonVarArrayLoad;
        // TODO: remove
        Boolean PoisonIntArrayLoad;
        // TODO: remove
        Boolean PoisonFloatArrayLoad;
        // TODO: remove
        Boolean PoisonTypedArrayLoad;
        // TODO: remove
        Boolean PoisonStringLoad;
        // TODO: remove
        Boolean PoisonObjectsForLoads;

        // TODO: remove
        Boolean PoisonVarArrayStore;
        // TODO: remove
        Boolean PoisonIntArrayStore;
        // TODO: remove
        Boolean PoisonFloatArrayStore;
        // TODO: remove
        Boolean PoisonTypedArrayStore;
        // TODO: remove
        Boolean PoisonStringStore;
        // TODO: remove
        Boolean PoisonObjectsForStores;

        Number MinInterpretCount;
        // TODO: remove
        Number MinSimpleJitRunCount;
        Number MaxInterpretCount;
        Number Mic;
        Number MaxSimpleJitRunCount; Number Msjrc;
        Number MinMemOpCount; Number Mmoc;

        #if ENABLE_COPYONACCESS_ARRAY
        // TODO: remove
            Number MaxCopyOnAccessArrayLength;
        // TODO: remove
            Number MinCopyOnAccessArrayLength;
        // TODO: remove
            Number CopyOnAccessArraySegmentCacheSize;
        #endif

        // TODO: remove
        Number MinTemplatizedJitRunCount;
        // TODO: remove
        Number MinAsmJsInterpreterRunCount;

        // TODO: remove
        Number MinTemplatizedJitLoopRunCount;
        Number MaxTemplatizedJitRunCount; Number Mtjrc;
        Number MaxAsmJsInterpreterRunCount; Number Maic;

        // TODO: remove
        Number AutoProfilingInterpreter0Limit;
        // TODO: remove
        Number ProfilingInterpreter0Limit;
        // TODO: remove
        Number AutoProfilingInterpreter1Limit;
        // TODO: remove
        Number SimpleJitLimit;
        // TODO: remove
        Number ProfilingInterpreter1Limit;

        // TODO: remove
        String ExecutionModeLimits; String Eml;
        // TODO: remove
        Boolean EnforceExecutionModeLimits; Boolean Eeml;

        Number SimpleJitAfter; Number Sja;
        Number FullJitAfter; Number Fja;

        // TODO: remove
        Boolean NewSimpleJit;

        Number MaxLinearIntCaseCount;
        // TODO: remove
        Number MaxSingleCharStrJumpTableSize;
        // TODO: remove
        Number MaxSingleCharStrJumpTableRatio;
        Number MinSwitchJumpTableSize;
        Number MaxLinearStringCaseCount;
        // TODO: remove
        Number MinDeferredFuncTokenCount;
        #if DBG
        // TODO: remove
            Number SkipFuncCountForBailOnNoProfile;
        #endif
        // TODO: remove
        Number MaxJITFunctionBytecodeByteLength;
        // TODO: remove
        Number MaxJITFunctionBytecodeCount;
        // TODO: remove
        Number MaxLoopsPerFunction;
        // TODO: remove
        Number FuncObjectInlineCacheThreshold;
        Boolean NoDeferParse;
        // TODO: remove
        Boolean NoLogo;
        Boolean OOPJITMissingOpts;
        // TODO: remove
        Boolean CrashOnOOPJITFailure;
        // TODO: remove
        Boolean OOPCFGRegistration;
        Boolean ForceJITCFGCheck;
        Boolean UseJITTrampoline;
        Boolean NoNative;
        // TODO: remove
        Number NopFrequency;
        // TODO: remove
        Boolean NoStrictMode;
        // TODO: remove
        Boolean NormalizeStats;
        Phases Off;
        // TODO: remove
        Phases OffProfiledByteCode;
        Phases On;
        #ifdef ENABLE_TRACE
        // TODO: remove
            Boolean InMemoryTrace;
        // TODO: remove
            Number InMemoryTraceBufferSize;
            #ifdef STACK_BACK_TRACE
        // TODO: remove
                Boolean TraceWithStack;
            #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        // TODO: remove
        Boolean PrintRunTimeDataCollectionTrace;
        #ifdef ENABLE_PREJIT
            Boolean Prejit;
        #endif
        // TODO: remove
        Boolean PrintSrcInDump;
        #if PROFILE_DICTIONARY
        // TODO: remove
            Number ProfileDictionary;
        #endif
        #ifdef PROFILE_EXEC
            Phases Profile;
        // TODO: remove
            Number ProfileThreshold;
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        // TODO: remove
            Boolean ProfileObjectLiteral;
        #endif
        #ifdef PROFILE_MEM
        // TODO: remove
            String ProfileMemory;
        #endif
        #ifdef PROFILE_STRINGS
        // TODO: remove
            Boolean ProfileStrings;
        #endif
        #ifdef PROFILE_TYPES
        // TODO: remove
            Boolean ProfileTypes;
        #endif
        #ifdef PROFILE_EVALMAP
        // TODO: remove
            Boolean ProfileEvalMap;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        // TODO: remove
            Boolean ProfileBailOutRecordMemory;
        #endif

        #if DBG
        // TODO: remove
            Boolean ValidateIntRanges;
        #endif
        // TODO: remove
        Number RejitMaxBailOutCount;
        // TODO: remove
        Number CallsToBailoutsRatioForRejit;
        // TODO: remove
        Number LoopIterationsToBailoutsRatioForRejit;
        Number MinBailOutsBeforeRejit;
        // TODO: remove
        Number MinBailOutsBeforeRejitForLoops;
        // TODO: remove
        Boolean LibraryStackFrame;
        // TODO: remove
        Boolean LibraryStackFrameDebugger;
        #ifdef RECYCLER_STRESS
            Boolean RecyclerStress;
        // TODO: remove
                Boolean RecyclerBackgroundStress;
        // TODO: remove
                Boolean RecyclerConcurrentStress;
        // TODO: remove
                Boolean RecyclerConcurrentRepeatStress;
        // TODO: remove
                Boolean RecyclerPartialStress;
        // TODO: remove
            Boolean RecyclerTrackStress;
        // TODO: remove
            Boolean RecyclerInduceFalsePositives;
        #endif // RECYCLER_STRESS
        // TODO: remove
        Boolean RecyclerForceMarkInterior;
        // TODO: remove
            Number RecyclerPriorityBoostTimeout;
        // TODO: remove
            Number RecyclerThreadCollectTimeout;
        // TODO: remove
            Boolean EnableConcurrentSweepAlloc; Boolean ecsa;
            Number PageHeap;
        // TODO: remove
            Boolean PageHeapAllocStack;
        // TODO: remove
            Boolean PageHeapFreeStack;
        // TODO: remove
            NumberRange PageHeapBucketNumber;
        // TODO: remove
            Number PageHeapBlockType;
        // TODO: remove
            Boolean PageHeapDecommitGuardPage;
        #ifdef RECYCLER_NO_PAGE_REUSE
            Boolean RecyclerNoPageReuse;
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
            Phases RecyclerVerify;
        // TODO: remove
            Number RecyclerVerifyPadSize;
        #endif
        // TODO: remove
        Boolean RecyclerTest;
        // TODO: remove
        Boolean RecyclerProtectPagesOnRescan;
        #ifdef RECYCLER_VERIFY_MARK
        // TODO: remove
            Boolean RecyclerVerifyMark;
        #endif
        // TODO: remove
        Number LowMemoryCap;
        // TODO: remove
        Number NewPagesCapDuringBGSweeping;
        // TODO: remove
        Number AllocPolicyLimit;
        // TODO: remove
        String RuntimeDataOutputFile;
        // TODO: remove
        Number SpeculationCap;
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        // TODO: remove
            Phases Stats;
        #endif
        #if EXCEPTION_RECOVERY
        // TODO: remove
            Boolean SwallowExceptions;
        #endif
        Boolean PrintSystemException;
        // TODO: remove
        Number SwitchOptHolesThreshold;
        // TODO: remove
        Number TempMin;
        // TODO: remove
        Number TempMax;
        Phases Trace;

        #if defined(_M_X64)
        // TODO: remove
            Number LoopAlignNopLimit;
        #endif

        #ifdef PROFILE_MEM
        // TODO: remove
            Phases TraceMemory;
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
            //TraceMetaDataParsing flag with optional levels:
            //    Level 1 = interfaces only
            //    Level 2 = interfaces and methods
            //    Level 3 = interfaces, methods and parameters
            //    Level 4 = interfaces and properties
            //    Level 5 (default) = ALL
        // TODO: remove
            Number TraceMetaDataParsing;
        // TODO: remove
            Boolean TraceWin8Allocations;
        // TODO: remove
            Boolean TraceWin8DeallocationsImmediate;
        // TODO: remove
            Boolean PrintWin8StatsDetailed;
        // TODO: remove
            Boolean TraceProtectPages;
        #endif
        // TODO: remove
        Boolean TraceAsyncDebugCalls;
        #ifdef TRACK_DISPATCH
        // TODO: remove
            Boolean TrackDispatch;
        #endif
        // TODO: remove
        Boolean Verbose;
        // TODO: remove
        Boolean UseFullName;
        // TODO: remove
        Boolean Utf8;
        Number Version;
        Boolean WERExceptionSupport;
        Boolean ExtendedErrorStackForTestHost;
        Boolean errorStackTrace;
        // TODO: remove
        Boolean DoHeapEnumOnEngineShutdown;
        #ifdef HEAP_ENUMERATION_VALIDATION
        // TODO: remove
            Boolean ValidateHeapEnum;
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
            //
            // Regex flags
            //
        // TODO: remove
            Boolean RegexTracing;
        // TODO: remove
            Boolean RegexProfile;
            Boolean RegexDebug;
        // TODO: remove
            Boolean RegexDebugAST;
        // TODO: remove
            Boolean RegexDebugAnnotatedAST;
        // TODO: remove
            Boolean RegexBytecodeDebug;
        // TODO: remove
            Boolean RegexOptimize;
        // TODO: remove
            Number DynamicRegexMruListSize;
        #endif

        // TODO: remove
        Boolean OptimizeForManyInstances;
        // TODO: remove
        Boolean EnableArrayTypeMutation;
        Number ArrayMutationTestSeed;
        Phases TestTrace;
        // TODO: remove
        Boolean EnableEvalMapCleanup;
        #ifdef PROFILE_MEM
        // TODO: remove
            Boolean TraceObjectAllocation;
        #endif
        Number Sse;
        // TODO: remove
        Number DeletedPropertyReuseThreshold;
        Boolean ForceStringKeyedSimpleDictionaryTypeHandler;
        Number BigDictionaryTypeHandlerThreshold;
        // TODO: remove
        Boolean TypeSnapshotEnumeration;
        // TODO: remove
        Boolean IsolatePrototypes;
        // TODO: remove
        Boolean ChangeTypeOnProto;
        // TODO: remove
        Boolean ShareInlineCaches;
        // TODO: remove
        Boolean DisableDebugObject;
        // TODO: remove
        Boolean DumpHeap;
        // TODO: remove
        String autoProxy;
        Number PerfHintLevel;
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        // TODO: remove
            Boolean MemProtectHeap;
        #endif
        #ifdef RECYCLER_STRESS
        // TODO: remove
            Boolean MemProtectHeapStress;
        // TODO: remove
                Boolean MemProtectHeapBackgroundStress;
        // TODO: remove
                Boolean MemProtectHeapConcurrentStress;
        // TODO: remove
                Boolean MemProtectHeapConcurrentRepeatStress;
        // TODO: remove
                Boolean MemProtectHeapPartialStress;
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        // TODO: remove
            Boolean FixPropsOnPathTypes;
        #endif
        // TODO: remove
        NumberSet BailoutTraceFilter;
        // TODO: remove
        NumberSet RejitTraceFilter;

        // recycler heuristic flags
        // TODO: remove
        Number MaxBackgroundFinishMarkCount;
        // TODO: remove
        Number BackgroundFinishMarkWaitTime;
        // TODO: remove
        Number MinBackgroundRepeatMarkRescanBytes;

        #if defined(_M_X64)
        // TODO: remove
            Boolean ZeroMemoryWithNonTemporalStore;
        #endif

        // recycler memory restrict test flags
        // TODO: remove
        Number MaxMarkStackPageCount;
        // TODO: remove
        Number MaxTrackedObjectListCount;

        // make the recycler page integration path easier to hit
        // TODO: remove
        Number NumberAllocPlusSize;

        #if DBG
            Boolean InitializeInterpreterSlotsWithInvalidStackVar;
        #endif

        #if DBG
        // TODO: remove
            Number PRNGSeed0;
        // TODO: remove
            Number PRNGSeed1;
        #endif

        // TODO: remove
        Boolean ClearInlineCachesOnCollect;
        // TODO: remove
        Number InlineCacheInvalidationListCompactionThreshold;
        // TODO: remove
        Number ConstructorCacheInvalidationThreshold;

        // TODO: remove
        Number GCMemoryThreshold;

        #if DBG
            Number SimulatePolyCacheWithOneTypeForInlineCacheIndex;
        #endif

        // TODO: remove
        Number JITServerIdleTimeout;
        // TODO: remove
        Number JITServerMaxInactivePageAllocatorCount;

        // TODO: remove
        Boolean StrictWriteBarrierCheck;
        // TODO: remove
        Boolean WriteBarrierTest;
        // TODO: remove
        Boolean ForceSoftwareWriteBarrier;
        // TODO: remove
        Boolean VerifyBarrierBit;
        // TODO: remove
        Boolean EnableBGFreeZero;
        // TODO: remove
        Boolean KeepRecyclerTrackData;

        // TODO: remove
        Number MaxSingleAllocSizeInMB;
        bool            flagPresent[FlagCount];
        // save the jscript.config for easier to get the raw input while analyzing dump file
        char16_t          rawInputFromConfigFile[512];
        int             rawInputFromConfigFileIndex;

    private:
        //
        // This variable is defined for the constructor
        //
                int             nDummy;

                // indicates whether a flag is a parent flag
                bool            flagIsParent[FlagMax + 1];

    // Implementation
    private:
                void *      GetProperty(Flag flag) const;

                void        SetAllParentFlagsAsDefaultValue();

                // special callback logic
                void        FlagSetCallback_ES6All(Js::Boolean value);
                void        FlagSetCallback_ES6Experimental(Js::Boolean value);

    public:
        void FinalizeConfiguration();
        void EnableExperimentalFlag();

        // Some config flags are expected to be constant during the lifetime of
        // a ScriptContext object. However, some other flags can change,
        // possibly multiple times.
        //
        // To keep the flags constant during the lifetime of a ScriptContext
        // object, we copy the ones that it needs into ThreadContext and have
        // it fetch them from ThreadContext instead of here. Given that a
        // ScriptContext object is bound to a ThreadContext object and never
        // gets reassigned, this keeps the flags constant while the
        // ScriptContext object is alive.
        //
        // Currently, among the flags used by ScriptContext, only the
        // experimental flags are altered after initialization. Therefore, only
        // access to these flags are serialized.
        //
        // Note that this lock is acquired automatically only when the
        // experimental flags are initialized via the EnableExperimentalFlag()
        // method. It should be manually acquired anywhere else where these
        // flags are accessed.
        std::recursive_mutex csExperimentalFlags;

    private:
        void TransferAcronymFlagConfiguration();
        void TranslateFlagConfiguration();
    };


    class Profiler;

    class Configuration
    {
    public:
        ConfigFlagsTable           flags;
        static Configuration        Global;

        // Public in case the client wants to have
        // a separate config from the global one
        Configuration();
    };

//Create macros for a useful subset of the config options that either get the value from the configuration (if the option is enabled) or
//just use the hard coded default value (if not). All the ...IsEnabled(...) default to false.
#define CONFIG_ISENABLED(flag)      (Js::Configuration::Global.flags.IsEnabled(flag))
#define CUSTOM_CONFIG_ISENABLED(flags, flag)      (flags.IsEnabled(flag))
#define CONFIG_FLAG(flag)           (Js::Configuration::Global.flags.flag)
#define CUSTOM_CONFIG_FLAG(flags, flag) (flags.flag)
#define CONFIG_FLAG_RELEASE(flag)   CONFIG_FLAG(flag)
#define CONFIG_FLAG_CONTAINS(flag, func)  (Js::Configuration::Global.flags.##flag.Contains((func)->GetLocalFunctionId()))

#define PHASE_OFF_PROFILED_BYTE_CODE(phase, func) Js::Configuration::Global.flags.OffProfiledByteCode.IsEnabled((phase),(func)->GetSourceContextId(),(func)->GetLocalFunctionId())
#define PHASE_OFF_PROFILED_BYTE_CODE_ALL(phase) Js::Configuration::Global.flags.OffProfiledByteCode.IsEnabledForAll((phase))
#define PHASE_OFF_PROFILED_BYTE_CODE_OPTFUNC(phase, func) ((func) ? PHASE_OFF_PROFILED_BYTE_CODE(phase, func) : PHASE_OFF_PROFILED_BYTE_CODE_ALL(phase))

#define PHASE_OFF1(phase)           Js::Configuration::Global.flags.Off.IsEnabled((phase))
#define CUSTOM_PHASE_OFF1(flags, phase)           flags.Off.IsEnabled((phase))
#define PHASE_OFF_ALL(phase)        Js::Configuration::Global.flags.Off.IsEnabledForAll((phase))
#define PHASE_OFF(phase, func)      PHASE_OFF_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_OFF_OPTFUNC(phase, func) ((func) ? PHASE_OFF(phase, func) : PHASE_OFF_ALL(phase))
#define PHASE_OFF_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Off.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_ON1(phase)            Js::Configuration::Global.flags.On.IsEnabled((phase))
#define CUSTOM_PHASE_ON1(flags, phase) flags.On.IsEnabled((phase))
#define PHASE_ON(phase, func)       PHASE_ON_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_ON_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.On.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_FORCE1(phase)         Js::Configuration::Global.flags.Force.IsEnabled((phase))
#define CUSTOM_PHASE_FORCE1(flags, phase) flags.Force.IsEnabled((phase))
#define PHASE_FORCE(phase, func)    PHASE_FORCE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_FORCE_OPTFUNC(phase, func) ((func) ? PHASE_FORCE(phase, func) : PHASE_FORCE1(phase))
#define PHASE_FORCE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Force.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_STRESS1(phase)        Js::Configuration::Global.flags.Stress.IsEnabled((phase))
#define PHASE_STRESS(phase, func)   PHASE_STRESS_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_STRESS_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Stress.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_TRACE1(phase)         Js::Configuration::Global.flags.Trace.IsEnabled((phase))
#define CUSTOM_PHASE_TRACE1(flags, phase) flags.Trace.IsEnabled((phase))
#define PHASE_TRACE(phase, func)    PHASE_TRACE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_TRACE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Trace.IsEnabled((phase), (sourceId), (functionId))

#if DBG
    // Enabling the ability to trace the StringConcat phase only in debug builds due to performance impact
    #define PHASE_TRACE_StringConcat PHASE_TRACE1(Js::StringConcatPhase)
#else
    #define PHASE_TRACE_StringConcat (false)
#endif

#define PHASE_VERBOSE_TRACE1(phase) \
    ((PHASE_TRACE1((phase))) && Js::Configuration::Global.flags.Verbose)

#define CUSTOM_PHASE_VERBOSE_TRACE1(flags, phase) \
    ((CUSTOM_PHASE_TRACE1((flags), (phase))) && flags.Verbose)

#define PHASE_VERBOSE_TRACE(phase, func) \
    ((PHASE_TRACE((phase), (func))) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TRACE_RAW(phase, sourceId, functionId) \
    ((PHASE_TRACE_RAW((phase), (sourceId), (functionId))) && Js::Configuration::Global.flags.Verbose)

#define PHASE_DUMP1(phase)          Js::Configuration::Global.flags.Dump.IsEnabled((phase))
#define PHASE_DUMP(phase, func)     Js::Configuration::Global.flags.Dump.IsEnabled((phase), (func)->GetSourceContextId(),(func)->GetLocalFunctionId())

#define PHASE_DEBUGBREAK_ON_PHASE_BEGIN(phase, func) Js::Configuration::Global.flags.DebugBreakOnPhaseBegin.IsEnabled((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())

#define PHASE_STATS1(phase)         Js::Configuration::Global.flags.Stats.IsEnabled((phase))
#define CUSTOM_PHASE_STATS1(flags, phase) flags.Stats.IsEnabled((phase))
#define PHASE_VERBOSE_STATS1(phase) \
    ((PHASE_STATS1(phase)) && Js::Configuration::Global.flags.Verbose)

#define PHASE_STATS_ALL(phase)      Js::Configuration::Global.flags.Stats.IsEnabledForAll((phase))
#define PHASE_STATS(phase, func)    PHASE_STATS_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_STATS_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.Stats.IsEnabled((phase), (sourceId), (functionId))

#define PHASE_VERBOSE_STATS(phase, func) \
    ((PHASE_STATS(phase, func)) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_STATS_RAW(phase, sourceId, functionId) \
    ((PHASE_STATS_RAW(phase, sourceId, functionId)) && Js::Configuration::Global.flags.Verbose)

#define PHASE_TESTTRACE1(phase) Js::Configuration::Global.flags.TestTrace.IsEnabled((phase))
#define PHASE_TESTTRACE(phase, func) PHASE_TESTTRACE_RAW((phase), (func)->GetSourceContextId(), (func)->GetLocalFunctionId())
#define PHASE_TESTTRACE_RAW(phase, sourceId, functionId) \
                                    Js::Configuration::Global.flags.TestTrace.IsEnabled((phase), (sourceId), (functionId))
#define PHASE_TESTTRACE1_TELEMETRY(phase) (false)

#define PHASE_PRINT_TRACE1(phase, ...) \
    if (PHASE_TRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define CUSTOM_PHASE_PRINT_TRACE1(flags, phase, ...) \
    if (CUSTOM_PHASE_TRACE1(flags, phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TRACE(phase, func, ...) \
    if (PHASE_TRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_TRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE1(phase, ...) \
    if (PHASE_VERBOSE_TRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define CUSTOM_PHASE_PRINT_VERBOSE_TRACE1(flags, phase, ...) \
    if (CUSTOM_PHASE_VERBOSE_TRACE1(flags, phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE(phase, func, ...) \
    if (PHASE_VERBOSE_TRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_VERBOSE_TRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_VERBOSE_TESTTRACE1(phase) (PHASE_TESTTRACE1(phase) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TESTTRACE(phase, func) \
    (PHASE_TESTTRACE(phase, func) && Js::Configuration::Global.flags.Verbose)
#define PHASE_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId) \
    (PHASE_TESTTRACE_RAW(phase, sourceId, functionId) && Js::Configuration::Global.flags.Verbose)

#define PHASE_PRINT_TESTTRACE1(phase, ...) \
    if (PHASE_TESTTRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TESTTRACE(phase, func, ...) \
    if (PHASE_TESTTRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_TESTTRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_TESTTRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE1(phase, ...) \
    if (PHASE_VERBOSE_TESTTRACE1(phase)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE(phase, func, ...) \
    if (PHASE_VERBOSE_TESTTRACE(phase, func)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#define PHASE_PRINT_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId, ...) \
    if (PHASE_VERBOSE_TESTTRACE_RAW(phase, sourceId, functionId)) \
    { \
        Output::Print(__VA_ARGS__); \
        Output::Flush(); \
    }

#ifdef ENABLE_REGEX_CONFIG_OPTIONS
#define REGEX_CONFIG_FLAG(flag) (Js::Configuration::Global.flags.flag)
#else
#define REGEX_CONFIG_FLAG(flag) (DEFAULT_CONFIG_##flag)
#endif

#ifdef SUPPORT_INTRUSIVE_TESTTRACES
#define PHASE_PRINT_INTRUSIVE_TESTTRACE1(phase, ...) \
    PHASE_PRINT_TESTTRACE1(phase, __VA_ARGS__)
#else
#define PHASE_PRINT_INTRUSIVE_TESTTRACE1(phase, ...) (false)
#endif

#define PHASE_ENABLED1(phase)       (Js::PhaseIsEnabled::phase())
#define PHASE_ENABLED(phase, func)  (Js::PhaseIsEnabled::phase(func))
#define PHASE_ENABLED_RAW(phase, sourceId, functionId) \
                                    (Js::PhaseIsEnabled::phase(sourceId, functionId))
struct PhaseIsEnabled
{
    static bool DeferParsePhase() { return !PHASE_OFF1(Js::DeferParsePhase); }

    template <typename FUNC>
    static bool DeferParsePhase(FUNC func) { return !PHASE_OFF(Js::DeferParsePhase, func); }

    static bool DeferParsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::DeferParsePhase, sourceId, functionId);
    }
    static bool ParallelParsePhase() { return PHASE_ON1(Js::ParallelParsePhase); }

    template <typename FUNC>
    static bool ParallelParsePhase(FUNC func) { return PHASE_ON(Js::ParallelParsePhase, func); }

    static bool ParallelParsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::ParallelParsePhase, sourceId, functionId);
    }
    static bool WasmPhase() { return !PHASE_OFF1(Js::WasmPhase); }

    template <typename FUNC>
    static bool WasmPhase(FUNC func) { return !PHASE_OFF(Js::WasmPhase, func); }

    static bool WasmPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::WasmPhase, sourceId, functionId);
    }
    static bool WasmDeferredPhase() { return !PHASE_OFF1(Js::WasmDeferredPhase); }

    template <typename FUNC>
    static bool WasmDeferredPhase(FUNC func) { return !PHASE_OFF(Js::WasmDeferredPhase, func); }

    static bool WasmDeferredPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::WasmDeferredPhase, sourceId, functionId);
    }
    static bool WasmValidatePrejitPhase() { return PHASE_ON1(Js::WasmValidatePrejitPhase); }

    template <typename FUNC>
    static bool WasmValidatePrejitPhase(FUNC func) { return PHASE_ON(Js::WasmValidatePrejitPhase, func); }

    static bool WasmValidatePrejitPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::WasmValidatePrejitPhase, sourceId, functionId);
    }
    static bool AsmjsFunctionEntryPhase() { return PHASE_ON1(Js::AsmjsFunctionEntryPhase); }

    template <typename FUNC>
    static bool AsmjsFunctionEntryPhase(FUNC func) { return PHASE_ON(Js::AsmjsFunctionEntryPhase, func); }

    static bool AsmjsFunctionEntryPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return PHASE_ON_RAW(Js::AsmjsFunctionEntryPhase, sourceId, functionId);
    }
    static bool InlineCallbacksPhase() { return !PHASE_OFF1(Js::InlineCallbacksPhase); }

    template <typename FUNC>
    static bool InlineCallbacksPhase(FUNC func) { return !PHASE_OFF(Js::InlineCallbacksPhase, func); }

    static bool InlineCallbacksPhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::InlineCallbacksPhase, sourceId, functionId);
    }
    static bool CaptureByteCodeRegUsePhase() { return !PHASE_OFF1(Js::CaptureByteCodeRegUsePhase); }

    template <typename FUNC>
    static bool CaptureByteCodeRegUsePhase(FUNC func) { return !PHASE_OFF(Js::CaptureByteCodeRegUsePhase, func); }

    static bool CaptureByteCodeRegUsePhase(uint sourceId, Js::LocalFunctionId functionId)
    {
        return !PHASE_OFF_RAW(Js::CaptureByteCodeRegUsePhase, sourceId, functionId);
    }
};

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///
/// class RangeBase
///
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------

template <typename TRangeUnitData>
void
RangeBase<TRangeUnitData>::Add(TRangeUnitData i, RangeBase<TRangeUnitData>* oppositeRange)
{
    Add(i, i, oppositeRange);
}

template <typename TRangeUnitData>
void
RangeBase<TRangeUnitData>::Add(TRangeUnitData i, TRangeUnitData j, RangeBase<TRangeUnitData>* oppositeRange)
{
    RangeUnit<TRangeUnitData> a(i, j);
    range.Prepend(a);
    if (oppositeRange)
    {
        if (oppositeRange->range.Empty())
        {
            oppositeRange->range.Prepend(GetFullRange<TRangeUnitData>());
        }
        // Do an intersection
        auto it = oppositeRange->range.GetEditingIterator();
        while (it.Next())
        {
            Unit& unit = it.Data();
            bool c1 = RangeUnitContains(unit, i);
            bool c2 = RangeUnitContains(unit, j);
            bool c3 = RangeUnitContains(a, unit.i);
            bool c4 = RangeUnitContains(a, unit.j);
            enum IntersectionCase
            {
                NoIntersection = 0,
                AddedFullyContained = 3, // [ u [a] ]
                IntersectionAdded_Unit = 6, // [ a [ ] u ]
                CommonLowBound_Unit = 7, // [[ a ] u ]
                IntersectionUnit_Added = 9, // [ u [ ] a ]
                CommonTopBound_Unit = 11, // [ u [ a ]]
                UnitFullyContained = 12, // [ a [u] ]
                CommonLowBound_Added = 13, // [[ u ] a ]
                CommonTopBound_Added = 14, // [ a [ u ]]
                FullIntersection = 15 // [[a, u]]
            };
            IntersectionCase intersectionCase = (IntersectionCase)((int)c1 | (c2 << 1) | (c3 << 2) | (c4 << 3));
            switch (intersectionCase)
            {
            case NoIntersection:
                // Nothing to do
                break;
            case AddedFullyContained:
            {
                // Need to break the current in 2
                Unit lowUnit = unit;
                Unit topUnit = unit;
                lowUnit.j = GetPrevious(a.i);
                topUnit.i = GetNext(a.j);
                it.InsertBefore(lowUnit);
                it.InsertBefore(topUnit);
                it.RemoveCurrent();
                break;
            }
            case IntersectionAdded_Unit:
            case CommonLowBound_Unit:
                // Move the unit lower bound after the added upper bound
                unit.i = GetNext(a.j);
                break;
            case IntersectionUnit_Added:
            case CommonTopBound_Unit:
                // Move the unit upper bound before the added lower bound
                unit.j = GetPrevious(a.i);
                break;
            case CommonTopBound_Added:
            case CommonLowBound_Added:
            case UnitFullyContained:
            case FullIntersection:
                // Remove the unit
                it.RemoveCurrent();
                break;
            default:
                Assert(UNREACHED);
                break;
            }
        }
    }
}

template <typename TRangeUnitData>
bool
RangeBase<TRangeUnitData>::ContainsAll()
{
    return range.Empty();
}

template <typename TRangeUnitData>
void
Js::RangeBase<TRangeUnitData>::Clear()
{
    range.Clear();
}

///----------------------------------------------------------------------------
///
/// RangeBase::InRange
///
/// Searches for each element in the list of UnitRanges. If the given integer
/// is between the 2 values, then return true; If no element is present in range
/// then, then we return true
///
///----------------------------------------------------------------------------

template <typename TRangeUnitData>
bool RangeBase<TRangeUnitData>::InRange(TRangeUnitData n)
{
    if (range.Empty())
    {
        return true;
    }
    else
    {
        return range.MapUntil([n](RangeUnit<TRangeUnitData> const& unit)
        {
            return RangeUnitContains(unit, n);
        });
    }
}
}  // namespace Js

