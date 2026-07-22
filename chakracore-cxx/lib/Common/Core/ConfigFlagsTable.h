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
        WasmFlag,
        WasmI64Flag,
        WasmFastArrayFlag,
        WasmSharedArrayVirtualBufferFlag,
        WasmMathExFilterFlag,
        WasmCheckVersionFlag,
        WasmAssignModuleIDFlag,
        WasmIgnoreResponseFlag,
        WasmMaxTableSizeFlag,
        WasmThreadsFlag,
        WasmMultiValueFlag,
        WasmSignExtendsFlag,
        WasmNontrappingFlag,

        // WebAssembly Experimental Features

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        WasmSimdFlag,

        BaselineModeFlag,
        BailOutFlag,
        BailOutAtEveryByteCodeFlag,
        BailOutByteCodeFlag,
        BgJitFlag,
        BgJitDelayFlag,

        CheckAlignmentFlag,

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
        RegexDebugFlag,
        RegexOptimizeFlag,
        #endif

        ArrayMutationTestSeedFlag,
        TestTraceFlag,
        SseFlag,
        ForceStringKeyedSimpleDictionaryTypeHandlerFlag,
        BigDictionaryTypeHandlerThresholdFlag,
        PerfHintLevelFlag,

        #if DBG
        InitializeInterpreterSlotsWithInvalidStackVarFlag,
        #endif

        #if DBG
            SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag,
        #endif

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
        Boolean Wasm;
        Boolean WasmI64;
        Boolean WasmFastArray;
        Boolean WasmSharedArrayVirtualBuffer;
        Boolean WasmMathExFilter;
        Boolean WasmCheckVersion;
        Boolean WasmAssignModuleID;
        Boolean WasmIgnoreResponse;
        Number WasmMaxTableSize;
        Boolean WasmThreads;
        Boolean WasmMultiValue;
        Boolean WasmSignExtends;
        Boolean WasmNontrapping;

        // WebAssembly Experimental Features

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        Boolean WasmSimd;

        Boolean BaselineMode;
            NumberPairSet BailOut;
            Boolean BailOutAtEveryByteCode;
            NumberSet BailOutByteCode;
        Boolean BgJit;
        Number BgJitDelay;

        Boolean CheckAlignment;

        Boolean ParserStateCache;
        // TODO (hanhossain): remove flag
        Boolean CompressParserStateCache;
        Boolean DeferTopLevelTillFirstCall;
        Number DeferParse;
        // TODO (hanhossain): remove flag
        Boolean DirectCallTelemetryStats;
        // TODO (hanhossain): remove flag
        Boolean DisableArrayBTree;
        // TODO (hanhossain): remove flag
        Boolean DisableRentalThreading;
        // TODO (hanhossain): remove flag
        Boolean DisableVTuneSourceLineInfo;
        // TODO (hanhossain): remove flag
        Boolean DisplayMemStats;
        Phases Dump;
        #ifdef DUMP_FRAGMENTATION_STATS
        // TODO (hanhossain): remove flag
            Boolean DumpFragmentationStats;
        #endif
        // TODO (hanhossain): remove flag
        Boolean DumpIRAddresses;
        // TODO (hanhossain): remove flag
        Boolean DumpLineNoInColor;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        // TODO (hanhossain): remove flag
            Boolean DumpObjectGraphOnExit;
        // TODO (hanhossain): remove flag
            Boolean DumpObjectGraphOnCollect;
        #endif
        // TODO (hanhossain): remove flag
        Boolean DumpEvalStringOnRemoval;
        // TODO (hanhossain): remove flag
        Boolean DumpObjectGraphOnEnum;
        #ifdef DYNAMIC_PROFILE_STORAGE
            String DynamicProfileCache;
        // TODO (hanhossain): remove flag
        String Dpc;
        // TODO (hanhossain): remove flag
            String DynamicProfileCacheDir;
            String DynamicProfileInput;
        // TODO (hanhossain): remove flag
        String Dpi;
        #endif
        #ifdef EDIT_AND_CONTINUE
        // TODO (hanhossain): remove flag
            Boolean EditTest;
        #endif
        // TODO (hanhossain): remove flag
        Boolean WininetProfileCache;
        // TODO (hanhossain): remove flag
        Boolean NoDynamicProfileInMemoryCache;
        // TODO (hanhossain): remove flag
        Boolean ProfileBasedSpeculativeJit;
        // TODO (hanhossain): remove flag
        Number ProfileBasedSpeculationCap;
        // TODO (hanhossain): remove flag
        Boolean ExecuteByteCodeBufferReturnsInvalidByteCode;
        // TODO (hanhossain): remove flag
        Number ExpirableCollectionGCCount;
        // TODO (hanhossain): remove flag
        Number ExpirableCollectionTriggerThreshold;
        Boolean SkipSplitOnNoResult;
        // TODO (hanhossain): remove flag
        Boolean Force32BitByteCode;

        Boolean CollectGarbage;

        Boolean Intl;
        // TODO (hanhossain): remove flag
        Boolean IntlBuiltIns;
        Boolean IntlPlatform;

        Boolean JsBuiltIn;
        // TODO (hanhossain): remove flag
        Boolean JitRepro;
        // TODO (hanhossain): remove flag
        Boolean EntryPointInfoRpcData;

        // TODO (hanhossain): remove flag
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
        // TODO (hanhossain): remove flag
        Boolean ES6DateParseFix;
        Boolean ES6FunctionNameFull;
        Boolean ES6Generators;
        Boolean ES7ExponentiationOperator;

        // TODO (hanhossain): remove flag
        Boolean ES7ValuesEntries;
        // TODO (hanhossain): remove flag
        Boolean ES7TrailingComma;
        // TODO (hanhossain): remove flag
        Boolean ES6IsConcatSpreadable;
        Boolean ES6Math;

        Boolean ESDynamicImport;

        Boolean ES6Module;
        // TODO (hanhossain): remove flag
        Boolean ES6Object;
        Boolean ES6Number;
        Boolean ES6ObjectLiterals;
        // TODO (hanhossain): remove flag
        Boolean ES6Proxy;
        Boolean ES6Rest;
        Boolean ES6Spread;
        Boolean ES6String;
        Boolean ES6StringPrototypeFixes;
        Boolean ES2018ObjectRestSpread;

        Boolean ES6PrototypeChain;
        // TODO (hanhossain): remove flag
        Boolean ES6ToPrimitive;
        Boolean ES6ToLength;
        Boolean ES6ToStringTag;
        Boolean ES6Unicode;
        // TODO (hanhossain): remove flag
        Boolean ES6UnicodeVerbose;
        // TODO (hanhossain): remove flag
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

        // TODO (hanhossain): remove flag
        Boolean ES6Verbose;
        Boolean ESObjectGetOwnPropertyDescriptors;

        Boolean ESSharedArrayBuffer;

        // Newer language feature flags

        // ES BigInt flag
        Boolean ESBigInt;

        // ES Numeric Separator support for numeric constants
        Boolean ESNumericSeparator;

        // ES Nullish coalescing operator support (??)
        // TODO (hanhossain): remove flag
        Boolean ESNullishCoalescingOperator;

        // ES Hashbang support for interpreter directive syntax
        Boolean ESHashbang;

        // ES Symbol.prototype.description flag
        Boolean ESSymbolDescription;

        // TODO (hanhossain): remove flag
        Boolean ESArrayFindFromLast;

        // ES Promise.any and AggregateError flag
        // TODO (hanhossain): remove flag
        Boolean ESPromiseAny;

        // ES import.meta keyword meta-property
        Boolean ESImportMeta;

        //ES globalThis flag
        Boolean ESGlobalThis;

        // This flag to be removed once JITing generator functions is stable
        Boolean JitES6Generators;

        // TODO (hanhossain): remove flag
        Boolean FastLineColumnCalculation;
        // TODO (hanhossain): remove flag
        String Filename;
        // TODO (hanhossain): remove flag
        Boolean FreeRejittedCode;
        // TODO (hanhossain): remove flag
        Boolean ForceGuardPages;
        // TODO (hanhossain): remove flag
        Boolean PrintGuardPageBounds;
        // TODO (hanhossain): remove flag
        Boolean ForceLegacyEngine;
        Phases Force;
        Phases Stress;
        Boolean ForceArrayBTree;
        // TODO (hanhossain): remove flag
        Boolean StrongArraySort;
        // TODO (hanhossain): remove flag
        Boolean ForceCleanPropertyOnCollect;
        // TODO (hanhossain): remove flag
        Boolean ForceCleanCacheOnCollect;
        Boolean ForceGCAfterJSONParse;
        // TODO (hanhossain): remove flag
        Boolean ForceDecommitOnCollect;
        Boolean ForceDeferParse;
        // TODO (hanhossain): remove flag
        Boolean ForceDiagnosticsMode;
        // TODO (hanhossain): remove flag
        Boolean ForceGetWriteWatchOOM;
        // TODO (hanhossain): remove flag
        Boolean ForcePostLowerGlobOptInstrString;
        // TODO (hanhossain): remove flag
        Boolean ForceSplitScope;
        // TODO (hanhossain): remove flag
        Boolean EnumerateSpecialPropertiesInDebugger;
        // TODO (hanhossain): remove flag
        Boolean EnableContinueAfterExceptionWrappersForHelpers;
        // TODO (hanhossain): remove flag
        Boolean EnableContinueAfterExceptionWrappersForBuiltIns;
        // TODO (hanhossain): remove flag
        Boolean EnableFunctionSourceReportForHeapEnum;
        // TODO (hanhossain): remove flag
        Number ForceFragmentAddressSpace;
        // TODO (hanhossain): remove flag
        Number ForceOOMOnEBCommit;
        // TODO (hanhossain): remove flag
        Boolean ForceDynamicProfile;
        Boolean ForceES5Array;
        Boolean ForceAsmJsLinkFail;
        // TODO (hanhossain): remove flag
        Boolean ForceExpireOnNonCacheCollect;
        // TODO (hanhossain): remove flag
        Boolean ForceFastPath;
        // TODO (hanhossain): remove flag
        Boolean ForceFloatPref;
        Boolean ForceJITLoopBody;
        Boolean ForceStaticInterpreterThunk;
        // TODO (hanhossain): remove flag
        Boolean DumpCommentsFromReferencedFiles;
        // TODO (hanhossain): remove flag
        Number DelayFullJITSmallFunc;
        Boolean EnableFatalErrorOnOOM;

        #if defined(_M_ARM32_OR_ARM64)
        // TODO (hanhossain): remove flag
            Boolean ForceLocalsPtr;
        #endif
        // TODO (hanhossain): remove flag
        Boolean DeferLoadingAvailableSource;
        Boolean ForceNative;
        Boolean ForceSerialized;
        // TODO (hanhossain): remove flag
        Number ForceSerializedBytecodeMajorVersion;
        // TODO (hanhossain): remove flag
        Number ForceSerializedBytecodeVersionSchema;
        Boolean ForceStrictMode;
        Boolean ForceUndoDefer;
        // TODO (hanhossain): remove flag
        Boolean ForceBlockingConcurrentCollect;
        // TODO (hanhossain): remove flag
        Boolean FreTestDiagMode;
        #ifdef BYTECODE_TESTING
        // TODO (hanhossain): remove flag
            Number ByteCodeBranchLimit;
        // TODO (hanhossain): remove flag
            Boolean MediumByteCodeLayout;
        // TODO (hanhossain): remove flag
            Boolean LargeByteCodeLayout;
        #endif
        // TODO (hanhossain): remove flag
        Number InduceCodeGenFailure;
        // TODO (hanhossain): remove flag
        Number InduceCodeGenFailureSeed;
        Number InjectPartiallyInitializedInterpreterFrameError;
        Number InjectPartiallyInitializedInterpreterFrameErrorType;
        // TODO (hanhossain): remove flag
        Boolean GenerateByteCodeBufferReturnsCantGenerate;
        Number GoptCleanupThreshold;
        // TODO (hanhossain): remove flag
        Number AsmGoptCleanupThreshold;
        Boolean HighPrecisionDate;
        // TODO (hanhossain): remove flag
        Number InlineCountMax;
        // TODO (hanhossain): remove flag
        Number InlineCountMaxInLoopBodies;
        // TODO (hanhossain): remove flag
        Number icminlb;
        Number InlineInLoopBodyScaleDownFactor;
        // TODO (hanhossain): remove flag
        Number iilbsdf;
        // TODO (hanhossain): remove flag
        Number InlineThreshold;
        // TODO (hanhossain): remove flag
        Number AggressiveInlineCountMax;
        // TODO (hanhossain): remove flag
        Number AggressiveInlineThreshold;
        // TODO (hanhossain): remove flag
        Number InlineThresholdAdjustCountInLargeFunction;
        // TODO (hanhossain): remove flag
        Number InlineThresholdAdjustCountInMediumSizedFunction;
        // TODO (hanhossain): remove flag
        Number InlineThresholdAdjustCountInSmallFunction;
        // TODO (hanhossain): remove flag
        Number AsmJsInlineAdjust;
        // TODO (hanhossain): remove flag
        String Interpret;
        // TODO (hanhossain): remove flag
        Phases Instrument;
        // TODO (hanhossain): remove flag
        Number JitQueueThreshold;
        // TODO (hanhossain): remove flag
        Number LoopInlineThreshold;
        // TODO (hanhossain): remove flag
        Number LeafInlineThreshold;
        // TODO (hanhossain): remove flag
        Number ConstantArgumentInlineThreshold;
        // TODO (hanhossain): remove flag
        Number RecursiveInlineThreshold;
        // TODO (hanhossain): remove flag
        Number RecursiveInlineDepthMax;
        // TODO (hanhossain): remove flag
        Number RecursiveInlineDepthMin;
        // TODO (hanhossain): remove flag
        Number RedeferralCap;
        // TODO (hanhossain): remove flag
        Number Loop;
        Number LoopInterpretCount;
        Number lic;
        // TODO (hanhossain): remove flag
        Number LoopProfileIterations;
        // TODO (hanhossain): remove flag
        Number OutsideLoopInlineThreshold;
        // TODO (hanhossain): remove flag
        Number MaxFuncInlineDepth;
        // TODO (hanhossain): remove flag
        Number MaxNumberOfInlineesWithLoop;
        #ifdef MEMSPECT_TRACKING
        // TODO (hanhossain): remove flag
            Phases Memspect;
        #endif
        // TODO (hanhossain): remove flag
        Number PolymorphicInlineThreshold;
        // TODO (hanhossain): remove flag
        Boolean PrimeRecycler;
        // TODO (hanhossain): remove flag
        Boolean TraceEngineRefcount;
        #if defined(CHECK_MEMORY_LEAK)
        // TODO (hanhossain): remove flag
            Boolean LeakStackTrace;
        // TODO (hanhossain): remove flag
            Boolean ForceMemoryLeak;
        #endif
        // TODO (hanhossain): remove flag
        Boolean DumpAfterFinalGC;
        Boolean ForceOldDateAPI;

        // TODO (hanhossain): remove flag
        Number JitLoopBodyHotLoopThreshold;
        // TODO (hanhossain): remove flag
        Number LoopBodySizeThresholdToDisableOpts;

        // TODO (hanhossain): remove flag
        Number MaxJitThreadCount;
        // TODO (hanhossain): remove flag
        Boolean ForceMaxJitThreadCount;

        // TODO (hanhossain): remove flag
        Boolean MitigateSpectre;

        // TODO (hanhossain): remove flag
        Boolean AddMaskingBlocks;

        // TODO (hanhossain): remove flag
        Boolean PoisonVarArrayLoad;
        // TODO (hanhossain): remove flag
        Boolean PoisonIntArrayLoad;
        // TODO (hanhossain): remove flag
        Boolean PoisonFloatArrayLoad;
        // TODO (hanhossain): remove flag
        Boolean PoisonTypedArrayLoad;
        // TODO (hanhossain): remove flag
        Boolean PoisonStringLoad;
        // TODO (hanhossain): remove flag
        Boolean PoisonObjectsForLoads;

        // TODO (hanhossain): remove flag
        Boolean PoisonVarArrayStore;
        // TODO (hanhossain): remove flag
        Boolean PoisonIntArrayStore;
        // TODO (hanhossain): remove flag
        Boolean PoisonFloatArrayStore;
        // TODO (hanhossain): remove flag
        Boolean PoisonTypedArrayStore;
        // TODO (hanhossain): remove flag
        Boolean PoisonStringStore;
        // TODO (hanhossain): remove flag
        Boolean PoisonObjectsForStores;

        Number MinInterpretCount;
        // TODO (hanhossain): remove flag
        Number MinSimpleJitRunCount;
        Number MaxInterpretCount;
        Number Mic;
        Number MaxSimpleJitRunCount; Number Msjrc;
        Number MinMemOpCount; Number Mmoc;

        #if ENABLE_COPYONACCESS_ARRAY
        // TODO (hanhossain): remove flag
            Number MaxCopyOnAccessArrayLength;
        // TODO (hanhossain): remove flag
            Number MinCopyOnAccessArrayLength;
        // TODO (hanhossain): remove flag
            Number CopyOnAccessArraySegmentCacheSize;
        #endif

        // TODO (hanhossain): remove flag
        Number MinTemplatizedJitRunCount;
        // TODO (hanhossain): remove flag
        Number MinAsmJsInterpreterRunCount;

        // TODO (hanhossain): remove flag
        Number MinTemplatizedJitLoopRunCount;
        Number MaxTemplatizedJitRunCount; Number Mtjrc;
        Number MaxAsmJsInterpreterRunCount; Number Maic;

        // TODO (hanhossain): remove flag
        Number AutoProfilingInterpreter0Limit;
        // TODO (hanhossain): remove flag
        Number ProfilingInterpreter0Limit;
        // TODO (hanhossain): remove flag
        Number AutoProfilingInterpreter1Limit;
        // TODO (hanhossain): remove flag
        Number SimpleJitLimit;
        // TODO (hanhossain): remove flag
        Number ProfilingInterpreter1Limit;

        // TODO (hanhossain): remove flag
        String ExecutionModeLimits; String Eml;
        // TODO (hanhossain): remove flag
        Boolean EnforceExecutionModeLimits; Boolean Eeml;

        Number SimpleJitAfter; Number Sja;
        Number FullJitAfter; Number Fja;

        // TODO (hanhossain): remove flag
        Boolean NewSimpleJit;

        Number MaxLinearIntCaseCount;
        // TODO (hanhossain): remove flag
        Number MaxSingleCharStrJumpTableSize;
        // TODO (hanhossain): remove flag
        Number MaxSingleCharStrJumpTableRatio;
        Number MinSwitchJumpTableSize;
        Number MaxLinearStringCaseCount;
        // TODO (hanhossain): remove flag
        Number MinDeferredFuncTokenCount;
        #if DBG
        // TODO (hanhossain): remove flag
            Number SkipFuncCountForBailOnNoProfile;
        #endif
        // TODO (hanhossain): remove flag
        Number MaxJITFunctionBytecodeByteLength;
        // TODO (hanhossain): remove flag
        Number MaxJITFunctionBytecodeCount;
        // TODO (hanhossain): remove flag
        Number MaxLoopsPerFunction;
        // TODO (hanhossain): remove flag
        Number FuncObjectInlineCacheThreshold;
        Boolean NoDeferParse;
        // TODO (hanhossain): remove flag
        Boolean NoLogo;
        Boolean OOPJITMissingOpts;
        // TODO (hanhossain): remove flag
        Boolean CrashOnOOPJITFailure;
        // TODO (hanhossain): remove flag
        Boolean OOPCFGRegistration;
        Boolean ForceJITCFGCheck;
        Boolean UseJITTrampoline;
        Boolean NoNative;
        // TODO (hanhossain): remove flag
        Number NopFrequency;
        // TODO (hanhossain): remove flag
        Boolean NoStrictMode;
        // TODO (hanhossain): remove flag
        Boolean NormalizeStats;
        Phases Off;
        // TODO (hanhossain): remove flag
        Phases OffProfiledByteCode;
        Phases On;
        #ifdef ENABLE_TRACE
        // TODO (hanhossain): remove flag
            Boolean InMemoryTrace;
        // TODO (hanhossain): remove flag
            Number InMemoryTraceBufferSize;
            #ifdef STACK_BACK_TRACE
        // TODO (hanhossain): remove flag
                Boolean TraceWithStack;
            #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        // TODO (hanhossain): remove flag
        Boolean PrintRunTimeDataCollectionTrace;
        #ifdef ENABLE_PREJIT
            Boolean Prejit;
        #endif
        // TODO (hanhossain): remove flag
        Boolean PrintSrcInDump;
        #if PROFILE_DICTIONARY
        // TODO (hanhossain): remove flag
            Number ProfileDictionary;
        #endif
        #ifdef PROFILE_EXEC
            Phases Profile;
        // TODO (hanhossain): remove flag
            Number ProfileThreshold;
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        // TODO (hanhossain): remove flag
            Boolean ProfileObjectLiteral;
        #endif
        #ifdef PROFILE_MEM
        // TODO (hanhossain): remove flag
            String ProfileMemory;
        #endif
        #ifdef PROFILE_STRINGS
        // TODO (hanhossain): remove flag
            Boolean ProfileStrings;
        #endif
        #ifdef PROFILE_TYPES
        // TODO (hanhossain): remove flag
            Boolean ProfileTypes;
        #endif
        #ifdef PROFILE_EVALMAP
        // TODO (hanhossain): remove flag
            Boolean ProfileEvalMap;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        // TODO (hanhossain): remove flag
            Boolean ProfileBailOutRecordMemory;
        #endif

        #if DBG
        // TODO (hanhossain): remove flag
            Boolean ValidateIntRanges;
        #endif
        // TODO (hanhossain): remove flag
        Number RejitMaxBailOutCount;
        // TODO (hanhossain): remove flag
        Number CallsToBailoutsRatioForRejit;
        // TODO (hanhossain): remove flag
        Number LoopIterationsToBailoutsRatioForRejit;
        Number MinBailOutsBeforeRejit;
        // TODO (hanhossain): remove flag
        Number MinBailOutsBeforeRejitForLoops;
        // TODO (hanhossain): remove flag
        Boolean LibraryStackFrame;
        // TODO (hanhossain): remove flag
        Boolean LibraryStackFrameDebugger;
        #ifdef RECYCLER_STRESS
            Boolean RecyclerStress;
        // TODO (hanhossain): remove flag
                Boolean RecyclerBackgroundStress;
        // TODO (hanhossain): remove flag
                Boolean RecyclerConcurrentStress;
        // TODO (hanhossain): remove flag
                Boolean RecyclerConcurrentRepeatStress;
        // TODO (hanhossain): remove flag
                Boolean RecyclerPartialStress;
        // TODO (hanhossain): remove flag
            Boolean RecyclerTrackStress;
        // TODO (hanhossain): remove flag
            Boolean RecyclerInduceFalsePositives;
        #endif // RECYCLER_STRESS
        // TODO (hanhossain): remove flag
        Boolean RecyclerForceMarkInterior;
        // TODO (hanhossain): remove flag
            Number RecyclerPriorityBoostTimeout;
        // TODO (hanhossain): remove flag
            Number RecyclerThreadCollectTimeout;
        // TODO (hanhossain): remove flag
            Boolean EnableConcurrentSweepAlloc; Boolean ecsa;
            Number PageHeap;
        // TODO (hanhossain): remove flag
            Boolean PageHeapAllocStack;
        // TODO (hanhossain): remove flag
            Boolean PageHeapFreeStack;
        // TODO (hanhossain): remove flag
            NumberRange PageHeapBucketNumber;
        // TODO (hanhossain): remove flag
            Number PageHeapBlockType;
        // TODO (hanhossain): remove flag
            Boolean PageHeapDecommitGuardPage;
        #ifdef RECYCLER_NO_PAGE_REUSE
            Boolean RecyclerNoPageReuse;
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
            Phases RecyclerVerify;
        // TODO (hanhossain): remove flag
            Number RecyclerVerifyPadSize;
        #endif
        // TODO (hanhossain): remove flag
        Boolean RecyclerTest;
        // TODO (hanhossain): remove flag
        Boolean RecyclerProtectPagesOnRescan;
        #ifdef RECYCLER_VERIFY_MARK
        // TODO (hanhossain): remove flag
            Boolean RecyclerVerifyMark;
        #endif
        // TODO (hanhossain): remove flag
        Number LowMemoryCap;
        // TODO (hanhossain): remove flag
        Number NewPagesCapDuringBGSweeping;
        // TODO (hanhossain): remove flag
        Number AllocPolicyLimit;
        // TODO (hanhossain): remove flag
        String RuntimeDataOutputFile;
        // TODO (hanhossain): remove flag
        Number SpeculationCap;
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        // TODO (hanhossain): remove flag
            Phases Stats;
        #endif
        #if EXCEPTION_RECOVERY
        // TODO (hanhossain): remove flag
            Boolean SwallowExceptions;
        #endif
        Boolean PrintSystemException;
        // TODO (hanhossain): remove flag
        Number SwitchOptHolesThreshold;
        // TODO (hanhossain): remove flag
        Number TempMin;
        // TODO (hanhossain): remove flag
        Number TempMax;
        Phases Trace;

        #if defined(_M_X64)
        // TODO (hanhossain): remove flag
            Number LoopAlignNopLimit;
        #endif

        #ifdef PROFILE_MEM
        // TODO (hanhossain): remove flag
            Phases TraceMemory;
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
            //TraceMetaDataParsing flag with optional levels:
            //    Level 1 = interfaces only
            //    Level 2 = interfaces and methods
            //    Level 3 = interfaces, methods and parameters
            //    Level 4 = interfaces and properties
            //    Level 5 (default) = ALL
        // TODO (hanhossain): remove flag
            Number TraceMetaDataParsing;
        // TODO (hanhossain): remove flag
            Boolean TraceWin8Allocations;
        // TODO (hanhossain): remove flag
            Boolean TraceWin8DeallocationsImmediate;
        // TODO (hanhossain): remove flag
            Boolean PrintWin8StatsDetailed;
        // TODO (hanhossain): remove flag
            Boolean TraceProtectPages;
        #endif
        // TODO (hanhossain): remove flag
        Boolean TraceAsyncDebugCalls;
        #ifdef TRACK_DISPATCH
        // TODO (hanhossain): remove flag
            Boolean TrackDispatch;
        #endif
        // TODO (hanhossain): remove flag
        Boolean Verbose;
        // TODO (hanhossain): remove flag
        Boolean UseFullName;
        // TODO (hanhossain): remove flag
        Boolean Utf8;
        Number Version;
        Boolean WERExceptionSupport;
        Boolean ExtendedErrorStackForTestHost;
        Boolean errorStackTrace;
        // TODO (hanhossain): remove flag
        Boolean DoHeapEnumOnEngineShutdown;
        #ifdef HEAP_ENUMERATION_VALIDATION
        // TODO (hanhossain): remove flag
            Boolean ValidateHeapEnum;
        #endif

        // todo (hanhossain): flag end
        #if ENABLE_REGEX_CONFIG_OPTIONS
            //
            // Regex flags
            //
            Boolean RegexDebug;
            Boolean RegexOptimize;
        #endif

        Number ArrayMutationTestSeed;
        Phases TestTrace;
        Number Sse;
        Boolean ForceStringKeyedSimpleDictionaryTypeHandler;
        Number BigDictionaryTypeHandlerThreshold;

        Number PerfHintLevel;

        #if DBG
            Boolean InitializeInterpreterSlotsWithInvalidStackVar;
        #endif

        #if DBG
            Number SimulatePolyCacheWithOneTypeForInlineCacheIndex;
        #endif

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

