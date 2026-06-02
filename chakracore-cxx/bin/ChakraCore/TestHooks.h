//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

#include <oaidl.h>
interface ICustomConfigFlags;

struct TestHooks
{
    using SetConfigFlagsPtr = int32_t(*)(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags);
    typedef int32_t(*SetConfigFilePtr)(char16_t* strConfigFile);
    typedef int32_t(*PrintConfigFlagsUsageStringPtr)(void);
    typedef int32_t(*SetAssertToConsoleFlagPtr)(bool flag);
    typedef int32_t(*SetEnableCheckMemoryLeakOutputPtr)(bool flag);
    typedef void(* NotifyUnhandledExceptionPtr)(PEXCEPTION_POINTERS exceptionInfo);
    typedef int(*LogicalStringCompareImpl)(const char16_t* p1, int p1size, const char16_t* p2, int p2size);

    SetConfigFlagsPtr pfSetConfigFlags;
    SetConfigFilePtr  pfSetConfigFile;
    PrintConfigFlagsUsageStringPtr pfPrintConfigFlagsUsageString;
    SetAssertToConsoleFlagPtr pfSetAssertToConsoleFlag;
    SetEnableCheckMemoryLeakOutputPtr pfSetEnableCheckMemoryLeakOutput;
    LogicalStringCompareImpl pfLogicalCompareStringImpl;

    // Javasscript Bigint hooks
    typedef digit_t(*AddDigit)(digit_t a, digit_t b, digit_t* carry);
    typedef digit_t(*SubDigit)(digit_t a, digit_t b, digit_t* borrow);
    typedef digit_t(*MulDigit)(digit_t a, digit_t b, digit_t* high);
    AddDigit pfAddDigit;
    SubDigit pfSubDigit;
    MulDigit pfMulDigit;

#define FLAG_Boolean(name) \
    bool (*pfIsEnabled##name##Flag)(); \
    int32_t (*pfGet##name##Flag)(bool *flag); \
    int32_t (*pfSet##name##Flag)(bool flag);
#define FLAG_Number(name) \
    bool (*pfIsEnabled##name##Flag)(); \
    int32_t (*pfGet##name##Flag)(int *flag); \
    int32_t (*pfSet##name##Flag)(int flag);
// TODO (hanhossain): ConfigFlagsList start
#ifndef DEFAULT_CONFIG_BgJitDelay
#if _M_ARM
#define DEFAULT_CONFIG_BgJitDelay           (70)
#else
#define DEFAULT_CONFIG_BgJitDelay           (30)
#endif
#endif // DEFAULT_CONFIG_BgJitDelay
#define DEFAULT_CONFIG_AsmJs                (true)
#define DEFAULT_CONFIG_AsmJsEdge            (false)
#define DEFAULT_CONFIG_AsmJsStopOnError     (false)

#define DEFAULT_CONFIG_Wasm               (true)
#define DEFAULT_CONFIG_WasmI64            (false)
#if ENABLE_FAST_ARRAYBUFFER
    #define DEFAULT_CONFIG_WasmFastArray    (true)
#else
    #define DEFAULT_CONFIG_WasmFastArray    (false)
#endif
#define DEFAULT_CONFIG_WasmSharedArrayVirtualBuffer (true)
#define DEFAULT_CONFIG_WasmCheckVersion     (true)
#define DEFAULT_CONFIG_WasmAssignModuleID   (false)
#define DEFAULT_CONFIG_WasmIgnoreLimits     (false)
#define DEFAULT_CONFIG_WasmFold             (true)
#define DEFAULT_CONFIG_WasmMathExFilter     (false)
#define DEFAULT_CONFIG_WasmIgnoreResponse   (false)
#define DEFAULT_CONFIG_WasmMaxTableSize     (10000000)
#define DEFAULT_CONFIG_WasmThreads          (false)
#define DEFAULT_CONFIG_WasmMultiValue       (false)
#define DEFAULT_CONFIG_WasmSignExtends      (true)
#define DEFAULT_CONFIG_WasmNontrapping      (true)
#define DEFAULT_CONFIG_WasmExperimental     (false)
#define DEFAULT_CONFIG_BgParse              (false)
#define DEFAULT_CONFIG_BgJitDelayFgBuffer   (0)
#define DEFAULT_CONFIG_BgJitPendingFuncCap  (31)
#define DEFAULT_CONFIG_CurrentSourceInfo    (true)
#define DEFAULT_CONFIG_CreateFunctionProxy  (true)
#define DEFAULT_CONFIG_HybridFgJit          (false)
#define DEFAULT_CONFIG_HybridFgJitBgQueueLengthThreshold (32)
#define DEFAULT_CONFIG_Prejit               (false)
#define DEFAULT_CONFIG_ParserStateCache     (true)
#define DEFAULT_CONFIG_CompressParserStateCache (false)
#define DEFAULT_CONFIG_DeferTopLevelTillFirstCall (true)
#define DEFAULT_CONFIG_DirectCallTelemetryStats (false)
#define DEFAULT_CONFIG_errorStackTrace      (true)
#define DEFAULT_CONFIG_FastLineColumnCalculation (true)
#define DEFAULT_CONFIG_PrintLineColumnInfo (false)
#define DEFAULT_CONFIG_ForceDecommitOnCollect (false)
#define DEFAULT_CONFIG_ForceDeferParse      (false)
#define DEFAULT_CONFIG_NoDeferParse         (false)
#define DEFAULT_CONFIG_ForceDynamicProfile  (false)
#define DEFAULT_CONFIG_ForceExpireOnNonCacheCollect (false)
#define DEFAULT_CONFIG_ForceFastPath        (false)
#define DEFAULT_CONFIG_ForceJITLoopBody     (false)
#define DEFAULT_CONFIG_ForceStaticInterpreterThunk (false)
#define DEFAULT_CONFIG_ForceCleanPropertyOnCollect (false)
#define DEFAULT_CONFIG_ForceCleanCacheOnCollect (false)
#define DEFAULT_CONFIG_ForceGCAfterJSONParse (false)
#define DEFAULT_CONFIG_ForceSerialized      (false)
#define DEFAULT_CONFIG_ForceES5Array        (false)
#define DEFAULT_CONFIG_ForceAsmJsLinkFail   (false)
#define DEFAULT_CONFIG_StrongArraySort      (false)
#define DEFAULT_CONFIG_DumpCommentsFromReferencedFiles (false)
#define DEFAULT_CONFIG_ExtendedErrorStackForTestHost (false)
#define DEFAULT_CONFIG_ForceSplitScope      (false)
#define DEFAULT_CONFIG_DelayFullJITSmallFunc (0)
#define DEFAULT_CONFIG_EnableFatalErrorOnOOM (true)
#define DEFAULT_CONFIG_RedeferralCap         (3)

//Following determines inline thresholds
#define DEFAULT_CONFIG_InlineThreshold      (35)            //Default start
#define DEFAULT_CONFIG_AggressiveInlineThreshold  (80)      //Limit for aggressive inlining.
#define DEFAULT_CONFIG_InlineThresholdAdjustCountInLargeFunction  (20)
#define DEFAULT_CONFIG_InlineThresholdAdjustCountInMediumSizedFunction  (6)
#define DEFAULT_CONFIG_InlineThresholdAdjustCountInSmallFunction  (10)
#define DEFAULT_CONFIG_ConstructorInlineThreshold (21)      //Monomorphic constructor threshold
#define DEFAULT_CONFIG_AsmJsInlineAdjust (35)                // wasm functions are cheaper to inline, so worth being more aggressive
#define DEFAULT_CONFIG_ConstructorCallsRequiredToFinalizeCachedType (2)
#define DEFAULT_CONFIG_OutsideLoopInlineThreshold (16)      //Threshold to inline outside loops
#define DEFAULT_CONFIG_LeafInlineThreshold  (60)            //Inlinee threshold for function which is leaf (irrespective of it has loops or not)
#define DEFAULT_CONFIG_LoopInlineThreshold  (25)            //Inlinee threshold for function with loops
#define DEFAULT_CONFIG_PolymorphicInlineThreshold  (35)     //Polymorphic inline threshold
#define DEFAULT_CONFIG_InlineCountMax       (1200)          //Max sum of bytecodes of inlinees inlined into a function (excluding built-ins)
#define DEFAULT_CONFIG_InlineCountMaxInLoopBodies (500)     // Max sum of bytecodes of inlinees that can be inlined into a jitted loop body (excluding built-ins)
#define DEFAULT_CONFIG_AggressiveInlineCountMax       (8000)          //Max sum of bytecodes of inlinees inlined into a function (excluding built-ins) when inlined aggressively
#define DEFAULT_CONFIG_MaxFuncInlineDepth   (2)             //Maximum number of times a function can be inlined within a top function
#define DEFAULT_CONFIG_MaxNumberOfInlineesWithLoop   (40) //Inlinee with a loop is controlled by LoopInlineThreshold, though we don't want to inline lot of inlinees with loop, this ensures a limit.
#define DEFAULT_CONFIG_ConstantArgumentInlineThreshold   (157)      // Bytecode threshold for functions with constant arguments which are used for branching
#define DEFAULT_CONFIG_RecursiveInlineThreshold     (2000)      // Bytecode threshold recursive call at a call site
#define DEFAULT_CONFIG_RecursiveInlineDepthMax      (8)      // Maximum inline depth for recursive calls
#define DEFAULT_CONFIG_RecursiveInlineDepthMin      (2)      // Minimum inline depth for recursive call
#define DEFAULT_CONFIG_InlineInLoopBodyScaleDownFactor    (4)
#define DEFAULT_CONFIG_PropertyCacheMissPenalty (10)
#define DEFAULT_CONFIG_PropertyCacheMissThreshold (-100)
#define DEFAULT_CONFIG_PropertyCacheMissReset (-5000)

#define DEFAULT_CONFIG_CloneInlinedPolymorphicCaches (true)
#define DEFAULT_CONFIG_HighPrecisionDate    (false)
#define DEFAULT_CONFIG_ForceOldDateAPI      (false)
#define DEFAULT_CONFIG_Loop                 (1)
#define DEFAULT_CONFIG_ForceDiagnosticsMode (false)
#define DEFAULT_CONFIG_UseFullName          (true)
#define DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForHelpers  (true)
#define DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForBuiltIns  (true)
#define DEFAULT_CONFIG_EnableFunctionSourceReportForHeapEnum (true)
#define DEFAULT_CONFIG_LoopInterpretCount   (150)
#define DEFAULT_CONFIG_LoopProfileIterations (25)
#define DEFAULT_CONFIG_JitLoopBodyHotLoopThreshold (20000)
#define DEFAULT_CONFIG_LoopBodySizeThresholdToDisableOpts (255)

#define DEFAULT_CONFIG_MaxJitThreadCount        (2)
#define DEFAULT_CONFIG_ForceMaxJitThreadCount   (false)

#ifdef ENABLE_SPECTRE_RUNTIME_MITIGATIONS
#define DEFAULT_CONFIG_MitigateSpectre (true)

#define DEFAULT_CONFIG_AddMaskingBlocks (true)

#define DEFAULT_CONFIG_PoisonVarArrayLoad (true)
#define DEFAULT_CONFIG_PoisonIntArrayLoad (true)
#define DEFAULT_CONFIG_PoisonFloatArrayLoad (true)
#define DEFAULT_CONFIG_PoisonTypedArrayLoad (true)
#define DEFAULT_CONFIG_PoisonStringLoad (true)
#define DEFAULT_CONFIG_PoisonObjectsForLoads (true)

#define DEFAULT_CONFIG_PoisonVarArrayStore (true)
#define DEFAULT_CONFIG_PoisonIntArrayStore (true)
#define DEFAULT_CONFIG_PoisonFloatArrayStore (true)
#define DEFAULT_CONFIG_PoisonTypedArrayStore (true)
#define DEFAULT_CONFIG_PoisonStringStore (true)
#define DEFAULT_CONFIG_PoisonObjectsForStores (true)
#else
#define DEFAULT_CONFIG_MitigateSpectre (false)

#define DEFAULT_CONFIG_AddMaskingBlocks (false)

#define DEFAULT_CONFIG_PoisonVarArrayLoad (false)
#define DEFAULT_CONFIG_PoisonIntArrayLoad (false)
#define DEFAULT_CONFIG_PoisonFloatArrayLoad (false)
#define DEFAULT_CONFIG_PoisonTypedArrayLoad (false)
#define DEFAULT_CONFIG_PoisonStringLoad (false)
#define DEFAULT_CONFIG_PoisonObjectsForLoads (false)

#define DEFAULT_CONFIG_PoisonVarArrayStore (false)
#define DEFAULT_CONFIG_PoisonIntArrayStore (false)
#define DEFAULT_CONFIG_PoisonFloatArrayStore (false)
#define DEFAULT_CONFIG_PoisonTypedArrayStore (false)
#define DEFAULT_CONFIG_PoisonStringStore (false)
#define DEFAULT_CONFIG_PoisonObjectsForStores (false)
#endif

#ifdef RECYCLER_PAGE_HEAP
#define DEFAULT_CONFIG_PageHeap             ((Js::Number) PageHeapMode::PageHeapModeOff)
#define DEFAULT_CONFIG_PageHeapAllocStack   (false)
#define DEFAULT_CONFIG_PageHeapFreeStack    (false)
#define DEFAULT_CONFIG_PageHeapBlockType    ((Js::Number) PageHeapBlockTypeFilter::PageHeapBlockTypeFilterAll)
#endif

#define DEFAULT_CONFIG_LowMemoryCap         (0xB900000) // 185 MB - based on memory cap for process on low-capacity device
#define DEFAULT_CONFIG_NewPagesCapDuringBGSweeping    (15000 * 4)
#define DEFAULT_CONFIG_MaxSingleAllocSizeInMB  (2048)
#define DEFAULT_CONFIG_AllocationPolicyLimit    (-1)

#define DEFAULT_CONFIG_MaxCodeFill          (500)
#define DEFAULT_CONFIG_MaxLoopsPerFunction  (10)
#define DEFAULT_CONFIG_NopFrequency         (8)
#define DEFAULT_CONFIG_SpeculationCap       (1)         // Needs to be 1 and not 0 since the compiler complains about a condition being always false
#define DEFAULT_CONFIG_ProfileBasedSpeculationCap (1600)
#define DEFAULT_CONFIG_Verbose              (false)
#define DEFAULT_CONFIG_ForceStrictMode      (false)
#define DEFAULT_CONFIG_EnableEvalMapCleanup (true)
#define DEFAULT_CONFIG_ExpirableCollectionGCCount (5)  // Number of GCs during which entry point profiling occurs
#define DEFAULT_CONFIG_ExpirableCollectionTriggerThreshold (50)  // Threshold at which Entry Point Collection is triggered
#define DEFAULT_CONFIG_RegexTracing         (false)
#define DEFAULT_CONFIG_RegexProfile         (false)
#define DEFAULT_CONFIG_RegexDebug           (false)
#define DEFAULT_CONFIG_RegexDebugAST        (true)
#define DEFAULT_CONFIG_RegexDebugAnnotatedAST (true)
#define DEFAULT_CONFIG_RegexBytecodeDebug   (false)
#define DEFAULT_CONFIG_RegexOptimize        (true)
#define DEFAULT_CONFIG_DynamicRegexMruListSize (16)
#define DEFAULT_CONFIG_GoptCleanupThreshold  (25)
#define DEFAULT_CONFIG_AsmGoptCleanupThreshold  (500)
#define DEFAULT_CONFIG_OptimizeForManyInstances (false)
#define DEFAULT_CONFIG_EnableArrayTypeMutation (false)

#define DEFAULT_CONFIG_DeferParseThreshold             (4 * 1024) // Unit is number of characters
#define DEFAULT_CONFIG_ProfileBasedDeferParseThreshold (100)      // Unit is number of characters

#define DEFAULT_CONFIG_ProfileBasedSpeculativeJit (true)
#define DEFAULT_CONFIG_WininetProfileCache        (true)
#define DEFAULT_CONFIG_MinProfileCacheSize        (5)   // Minimum number of functions before profile is saved.
#define DEFAULT_CONFIG_ProfileDifferencePercent   (15)  // If 15% of the functions have different profile we will trigger a save.

#define DEFAULT_CONFIG_Intl                    (true)
#define DEFAULT_CONFIG_IntlBuiltIns            (true)
#define DEFAULT_CONFIG_IntlPlatform            (false) // Makes the EngineExtension.Intl object visible to user code as Intl.platform, meant for testing

#ifdef ENABLE_JS_BUILTINS
    #define DEFAULT_CONFIG_JsBuiltIn             (true)
#else
    #define DEFAULT_CONFIG_JsBuiltIn             (false)
#endif
#define DEFAULT_CONFIG_JitRepro                (false)
#define DEFAULT_CONFIG_LdChakraLib             (false)
#define DEFAULT_CONFIG_TestChakraLib           (false)
#define DEFAULT_CONFIG_EntryPointInfoRpcData   (false)

// ES6 DEFAULT BEHAVIOR
#define DEFAULT_CONFIG_ES6                     (true)  // master flag to gate all P0-spec-test compliant ES6 features

//CollectGarbage is legacy IE specific global function disabled in Microsoft Edge.
#define DEFAULT_CONFIG_CollectGarbage          (false)

// ES6 sub-feature gate - to enable-disable ES6 sub-feature when ES6 flag is enabled
#define DEFAULT_CONFIG_ES6DateParseFix         (true)
#define DEFAULT_CONFIG_ES6FunctionNameFull     (true)
#define DEFAULT_CONFIG_ES6Generators           (true)
#define DEFAULT_CONFIG_ES6IsConcatSpreadable   (true)
#define DEFAULT_CONFIG_ES6Math                 (true)
#ifdef COMPILE_DISABLE_ES6Module
    // If ES6Module needs to be disabled by compile flag, DEFAULT_CONFIG_ES6Module should be false
    #define DEFAULT_CONFIG_ES6Module               (false)
#else
    #define DEFAULT_CONFIG_ES6Module               (true)
#endif
#define DEFAULT_CONFIG_ES6Object               (true)
#define DEFAULT_CONFIG_ES6Number               (true)
#define DEFAULT_CONFIG_ES6ObjectLiterals       (true)
#define DEFAULT_CONFIG_ES6Proxy                (true)
#define DEFAULT_CONFIG_ES6Rest                 (true)
#define DEFAULT_CONFIG_ES6Spread               (true)
#define DEFAULT_CONFIG_ES6String               (true)
#define DEFAULT_CONFIG_ES6StringPrototypeFixes (true)
#define DEFAULT_CONFIG_ES2018ObjectRestSpread  (true)

#ifndef DEFAULT_CONFIG_ES6PrototypeChain
#ifdef COMPILE_DISABLE_ES6PrototypeChain
    // If ES6PrototypeChain needs to be disabled by compile flag, DEFAULT_CONFIG_ES6PrototypeChain should be false
    #define DEFAULT_CONFIG_ES6PrototypeChain       (false)
#else
    #define DEFAULT_CONFIG_ES6PrototypeChain       (true)
#endif
#endif

#define DEFAULT_CONFIG_ES6ToPrimitive          (true)
#define DEFAULT_CONFIG_ES6ToLength             (true)
#define DEFAULT_CONFIG_ES6ToStringTag          (true)
#define DEFAULT_CONFIG_ES6Unicode              (true)
#define DEFAULT_CONFIG_ES6UnicodeVerbose       (true)
#define DEFAULT_CONFIG_ES6Unscopables          (true)
#define DEFAULT_CONFIG_ES6RegExSticky          (true)
#define DEFAULT_CONFIG_ES2018RegExDotAll       (true)
#define DEFAULT_CONFIG_ESBigInt                (false)
#define DEFAULT_CONFIG_ESNumericSeparator      (true)
#define DEFAULT_CONFIG_ESHashbang              (true)
#define DEFAULT_CONFIG_ESSymbolDescription     (true)
#define DEFAULT_CONFIG_ESArrayFindFromLast     (true)
#define DEFAULT_CONFIG_ESPromiseAny            (true)
#define DEFAULT_CONFIG_ESNullishCoalescingOperator (true)
#define DEFAULT_CONFIG_ESGlobalThis            (true)

// Jitting generator functions is not functional on ARM
// Also still contains significant bugs on x86/x64 hence disabled
#ifdef _M_ARM32_OR_ARM64
    #define DEFAULT_CONFIG_JitES6Generators            (false)
#else
    #define DEFAULT_CONFIG_JitES6Generators            (false)
#endif

#ifdef COMPILE_DISABLE_ES6RegExPrototypeProperties
    // If ES6RegExPrototypeProperties needs to be disabled by compile flag, DEFAULT_CONFIG_ES6RegExPrototypeProperties should be false
    #define DEFAULT_CONFIG_ES6RegExPrototypeProperties (false)
#else
    #define DEFAULT_CONFIG_ES6RegExPrototypeProperties (false)
#endif
#ifdef COMPILE_DISABLE_ES6RegExSymbols
    // If ES6RegExSymbols needs to be disabled by compile flag, DEFAULT_CONFIG_ES6RegExSymbols should be false
    #define DEFAULT_CONFIG_ES6RegExSymbols         (false)
#else
    #define DEFAULT_CONFIG_ES6RegExSymbols         (false)
#endif
#define DEFAULT_CONFIG_ES7AsyncAwait           (true)
#define DEFAULT_CONFIG_ES7ExponentionOperator  (true)
#define DEFAULT_CONFIG_ES7TrailingComma        (true)
#define DEFAULT_CONFIG_ES7ValuesEntries        (true)
#define DEFAULT_CONFIG_ESObjectGetOwnPropertyDescriptors (true)
#define DEFAULT_CONFIG_ESDynamicImport         (true)
#define DEFAULT_CONFIG_ESImportMeta            (true)
#define DEFAULT_CONFIG_ESExportNsAs            (true)
#define DEFAULT_CONFIG_ES2018AsyncIteration    (true)
#define DEFAULT_CONFIG_ESTopLevelAwait         (true)

#define DEFAULT_CONFIG_ESSharedArrayBuffer     (false)

#define DEFAULT_CONFIG_ES6Verbose              (false)
#define DEFAULT_CONFIG_ES6All                  (false)
// ES6 DEFAULT BEHAVIOR

#define DEFAULT_CONFIG_AsyncDebugging           (true)
#define DEFAULT_CONFIG_TraceAsyncDebugCalls     (false)
#define DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString (false)
#define DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger (true)

#define DEFAULT_CONFIG_MaxJITFunctionBytecodeByteLength (4800000)
#define DEFAULT_CONFIG_MaxJITFunctionBytecodeCount (120000)

#define DEFAULT_CONFIG_JitQueueThreshold      (6)

#define DEFAULT_CONFIG_FullJitRequeueThreshold (25)     // Minimum number of times a function needs to be executed before it is re-added to the jit queue

#define DEFAULT_CONFIG_MinTemplatizedJitRunCount      (100)     // Minimum number of times a function needs to be interpreted before it is jitted
#define DEFAULT_CONFIG_MinAsmJsInterpreterRunCount      (10)     // Minimum number of times a function needs to be Asm interpreted before it is jitted
#define DEFAULT_CONFIG_MinTemplatizedJitLoopRunCount      (500)     // Minimum number of times a function needs to be interpreted before it is jitted
#define DEFAULT_CONFIG_MaxTemplatizedJitRunCount      (-1)     // Maximum number of times a function can be TJ before it is jitted
#define DEFAULT_CONFIG_MaxAsmJsInterpreterRunCount      (-1)     // Maximum number of times a function can be Asm interpreted before it is jitted

// Note: The following defaults only apply when the NewSimpleJit is on. The defaults for when it's off are computed in
// ConfigFlagsTable::TranslateFlagConfiguration.
#define DEFAULT_CONFIG_AutoProfilingInterpreter0Limit (12)
#define DEFAULT_CONFIG_ProfilingInterpreter0Limit (4)
#define DEFAULT_CONFIG_AutoProfilingInterpreter1Limit (0)
#define DEFAULT_CONFIG_SimpleJitLimit (132)
#define DEFAULT_CONFIG_ProfilingInterpreter1Limit (12)

// These are used to compute the above defaults for when NewSimpleJit is off
#define DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit (80)
#define DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit (25)

#define DEFAULT_CONFIG_MinProfileIterations (16)
#define DEFAULT_CONFIG_MinProfileIterations_OldSimpleJit (25)
#define DEFAULT_CONFIG_MinSimpleJitIterations (16)
#define DEFAULT_CONFIG_NewSimpleJit (false)

#define DEFAULT_CONFIG_MaxLinearIntCaseCount     (3)       // Maximum number of cases (in switch statement) for which instructions can be generated linearly.
#define DEFAULT_CONFIG_MaxSingleCharStrJumpTableRatio  (2)       // Maximum single char string jump table size as multiples of the actual case arm
#define DEFAULT_CONFIG_MaxSingleCharStrJumpTableSize  (128)       // Maximum single char string jump table size
#define DEFAULT_CONFIG_MinSwitchJumpTableSize   (9)     // Minimum number of case target entries in the jump table(this may also include values that are missing in the consecutive set of integer case arms)
#define DEFAULT_CONFIG_SwitchOptHolesThreshold  (50)     // Maximum percentage of holes (missing case values in a switch statement) with which a jump table can be created
#define DEFAULT_CONFIG_MaxLinearStringCaseCount (4)     // Maximum number of String cases (in switch statement) for which instructions can be generated linearly.

#define DEFAULT_CONFIG_MinDeferredFuncTokenCount (20)   // Minimum size in tokens of a defer-parsed function

#if DBG
#define DEFAULT_CONFIG_SkipFuncCountForBailOnNoProfile (0) //Initial Number of functions in a func body to be skipped from forcibly inserting BailOnNoProfile.
#endif
#define DEFAULT_CONFIG_BailOnNoProfileLimit    200      // The limit of bailout on no profile info before triggering a rejit
#define DEFAULT_CONFIG_BailOnNoProfileRejitLimit (50)   // The limit of bailout on no profile info before disable all the no profile bailouts
#define DEFAULT_CONFIG_CallsToBailoutsRatioForRejit 10   // Ratio of function calls to bailouts above which a rejit is considered
#define DEFAULT_CONFIG_LoopIterationsToBailoutsRatioForRejit 50 // Ratio of loop iteration count to bailouts above which a rejit of the loop body is considered
#define DEFAULT_CONFIG_MinBailOutsBeforeRejit 2         // Minimum number of bailouts for a single bailout record after which a rejit is considered
#define DEFAULT_CONFIG_MinBailOutsBeforeRejitForLoops 2         // Minimum number of bailouts for a single bailout record after which a rejit is considered
#define DEFAULT_CONFIG_RejitMaxBailOutCount 500         // Maximum number of bailouts for a single bailout record after which rejit is forced.

#if DBG
#define DEFAULT_CONFIG_ValidateIntRanges (false)
#endif

#define DEFAULT_CONFIG_Sse                  (-1)

#define DEFAULT_CONFIG_DeletedPropertyReuseThreshold (32)
#define DEFAULT_CONFIG_BigDictionaryTypeHandlerThreshold (0xffff)
#define DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler (false)
#define DEFAULT_CONFIG_TypeSnapshotEnumeration (true)
#define DEFAULT_CONFIG_ConcurrentRuntime (false)
#define DEFAULT_CONFIG_PrimeRecycler     (false)
#define DEFAULT_CONFIG_DisableRentalThreading (false)
#define DEFAULT_CONFIG_DisableDebugObject (false)
#define DEFAULT_CONFIG_DumpHeap (false)
#define DEFAULT_CONFIG_PerfHintLevel (1)
#define DEFAULT_CONFIG_OOPJITMissingOpts (false)
#define DEFAULT_CONFIG_OOPCFGRegistration (true)
#define DEFAULT_CONFIG_CrashOnOOPJITFailure (false)
#define DEFAULT_CONFIG_ForceJITCFGCheck (false)
#define DEFAULT_CONFIG_UseJITTrampoline (true)

#define DEFAULT_CONFIG_IsolatePrototypes    (true)
#define DEFAULT_CONFIG_ChangeTypeOnProto    (true)
#define DEFAULT_CONFIG_FixPropsOnPathTypes    (true)
#define DEFAULT_CONFIG_BailoutTraceFilter (-1)
#define DEFAULT_CONFIG_TempMin    (0)
#define DEFAULT_CONFIG_TempMax    (INT_MAX)

#define DEFAULT_CONFIG_LibraryStackFrame            (true)
#define DEFAULT_CONFIG_LibraryStackFrameDebugger    (false)

#define DEFAULT_CONFIG_FuncObjectInlineCacheThreshold   (2) // Maximum number of inline caches a function body may have to allow for inline caches to be allocated on the function object.
#define DEFAULT_CONFIG_ShareInlineCaches (false)
#define DEFAULT_CONFIG_InlineCacheInvalidationListCompactionThreshold (4)
#define DEFAULT_CONFIG_ConstructorCacheInvalidationThreshold (500)

#define DEFAULT_CONFIG_InMemoryTrace                (false)
#define DEFAULT_CONFIG_InMemoryTraceBufferSize      (1024)
#define DEFAULT_CONFIG_RichTraceFormat              (false)
#define DEFAULT_CONFIG_TraceWithStack               (false)

#define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameError (0)
#define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameErrorType (0)

#define DEFAULT_CONFIG_DeferLoadingAvailableSource  (false)

#define DEFAULT_CONFIG_RecyclerForceMarkInterior (false)

#define DEFAULT_CONFIG_MemProtectHeap (false)

#define DEFAULT_CONFIG_InduceCodeGenFailure (30) // When -InduceCodeGenFailure is passed in, 30% of JIT allocations will fail

#define DEFAULT_CONFIG_SkipSplitWhenResultIgnored (false)

#define DEFAULT_CONFIG_MinMemOpCount (16U)

#if ENABLE_COPYONACCESS_ARRAY
#define DEFAULT_CONFIG_MaxCopyOnAccessArrayLength (32U)
#define DEFAULT_CONFIG_MinCopyOnAccessArrayLength (5U)
#define DEFAULT_CONFIG_CopyOnAccessArraySegmentCacheSize (16U)
#endif

#if defined(_M_X64)
#define DEFAULT_CONFIG_LoopAlignNopLimit (6)
#endif

#if defined(_M_X64)
#define DEFAULT_CONFIG_ZeroMemoryWithNonTemporalStore (true)
#endif

#define DEFAULT_CONFIG_StrictWriteBarrierCheck  (false)
#define DEFAULT_CONFIG_KeepRecyclerTrackData  (false)
#define DEFAULT_CONFIG_EnableBGFreeZero (true)

#if !GLOBAL_ENABLE_WRITE_BARRIER
#define DEFAULT_CONFIG_ForceSoftwareWriteBarrier  (false)
#else
#define DEFAULT_CONFIG_ForceSoftwareWriteBarrier  (true)
#endif
#define DEFAULT_CONFIG_WriteBarrierTest (false)
#define DEFAULT_CONFIG_VerifyBarrierBit  (false)

#define TraceLevel_Error        (1)
#define TraceLevel_Warning      (2)
#define TraceLevel_Info         (3)

#define TEMP_ENABLE_FLAG_FOR_APPX_BETA_ONLY 1

#define INMEMORY_CACHE_MAX_URL                    (5)             // This is the max number of URLs that the in-memory profile cache can hold.
#define INMEMORY_CACHE_MAX_PROFILE_MANAGER        (50)            // This is the max number of dynamic scripts that the in-memory profile cache can have

#ifdef SUPPORT_INTRUSIVE_TESTTRACES
#define INTRUSIVE_TESTTRACE_PolymorphicInlineCache (1)
#endif

#if DBG
FLAG_Boolean(ArrayValidate)
FLAG_Boolean(MemOpMissingValueValidate)
FLAG_Boolean(OOPJITFixupValidate)
#endif
#ifdef ARENA_MEMORY_VERIFY
FLAG_Boolean(ArenaNoFreeList)
FLAG_Boolean(ArenaNoPageReuse)
FLAG_Boolean(ArenaUseHeapAlloc)
#endif
FLAG_Boolean(ValidateInlineStack)
FLAG_Boolean(AsmDiff)
bool (*pfIsEnabledAsmDumpModeFlag)();
int32_t (*pfGetAsmDumpModeFlag)(BSTR *flag);
int32_t (*pfSetAsmDumpModeFlag)(BSTR flag);
FLAG_Boolean(AsmJs)
FLAG_Boolean(AsmJsStopOnError)
FLAG_Boolean(AsmJsEdge)
FLAG_Boolean(Wasm)
FLAG_Boolean(WasmI64)
FLAG_Boolean(WasmFastArray)
FLAG_Boolean(WasmSharedArrayVirtualBuffer)
FLAG_Boolean(WasmMathExFilter)
FLAG_Boolean(WasmCheckVersion)
FLAG_Boolean(WasmAssignModuleID)
FLAG_Boolean(WasmIgnoreLimits)
FLAG_Boolean(WasmFold)
FLAG_Boolean(WasmIgnoreResponse)
FLAG_Number(WasmMaxTableSize)
FLAG_Boolean(WasmThreads)
FLAG_Boolean(WasmMultiValue)
FLAG_Boolean(WasmSignExtends)
FLAG_Boolean(WasmNontrapping)

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
FLAG_Boolean(WasmExperimental)

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
FLAG_Boolean(WasmSimd)

FLAG_Boolean(AssertBreak)
FLAG_Boolean(AssertPopUp)
FLAG_Boolean(AssertIgnore)
FLAG_Boolean(AsyncDebugging)
FLAG_Number(BailOnNoProfileLimit)
FLAG_Number(BailOnNoProfileRejitLimit)
FLAG_Boolean(BaselineMode)
bool (*pfIsEnabledDumpOnCrashFlag)();
int32_t (*pfGetDumpOnCrashFlag)(BSTR *flag);
int32_t (*pfSetDumpOnCrashFlag)(BSTR flag);
bool (*pfIsEnabledFullMemoryDumpFlag)();
int32_t (*pfGetFullMemoryDumpFlag)(BSTR *flag);
int32_t (*pfSetFullMemoryDumpFlag)(BSTR flag);
#ifdef BAILOUT_INJECTION
FLAG_Boolean(BailOutAtEveryLine)
FLAG_Boolean(BailOutAtEveryByteCode)
FLAG_Boolean(BailOutAtEveryImplicitCall)
#endif
FLAG_Boolean(Benchmark)
FLAG_Boolean(BgJit)
FLAG_Boolean(BgParse)
FLAG_Number(BgJitDelay)
FLAG_Number(BgJitDelayFgBuffer)
FLAG_Number(BgJitPendingFuncCap)

FLAG_Boolean(CreateFunctionProxy)
FLAG_Boolean(HybridFgJit)
FLAG_Number(HybridFgJitBgQueueLengthThreshold)
FLAG_Boolean(BytecodeHist)
FLAG_Boolean(CurrentSourceInfo)
FLAG_Boolean(CFGLog)
FLAG_Boolean(CheckAlignment)
FLAG_Boolean(CheckEmitBufferPermissions)
#ifdef CHECK_MEMORY_LEAK
FLAG_Boolean(CheckMemoryLeak)
bool (*pfIsEnabledDumpOnLeakFlag)();
int32_t (*pfGetDumpOnLeakFlag)(BSTR *flag);
int32_t (*pfSetDumpOnLeakFlag)(BSTR flag);
#endif
FLAG_Boolean(CheckOpHelpers)
FLAG_Boolean(CloneInlinedPolymorphicCaches)
FLAG_Boolean(ConcurrentRuntime)
FLAG_Number(ConstructorInlineThreshold)
FLAG_Number(ConstructorCallsRequiredToFinalizeCachedType)
FLAG_Number(PropertyCacheMissPenalty)
FLAG_Number(PropertyCacheMissThreshold)
FLAG_Number(PropertyCacheMissReset)
FLAG_Boolean(Debug)

FLAG_Boolean(DebugWindow)
FLAG_Boolean(ParserStateCache)
FLAG_Boolean(CompressParserStateCache)
FLAG_Boolean(DeferTopLevelTillFirstCall)
FLAG_Number(DeferParse)
FLAG_Boolean(DirectCallTelemetryStats)
FLAG_Boolean(DisableArrayBTree)
FLAG_Boolean(DisableRentalThreading)
FLAG_Boolean(DisableVTuneSourceLineInfo)
FLAG_Boolean(DisplayMemStats)
#ifdef DUMP_FRAGMENTATION_STATS
FLAG_Boolean(DumpFragmentationStats)
#endif
FLAG_Boolean(DumpIRAddresses)
FLAG_Boolean(DumpLineNoInColor)
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
FLAG_Boolean(DumpObjectGraphOnExit)
FLAG_Boolean(DumpObjectGraphOnCollect)
#endif
FLAG_Boolean(DumpEvalStringOnRemoval)
FLAG_Boolean(DumpObjectGraphOnEnum)
#ifdef DYNAMIC_PROFILE_STORAGE
bool (*pfIsEnabledDynamicProfileCacheFlag)();
int32_t (*pfGetDynamicProfileCacheFlag)(BSTR *flag);
int32_t (*pfSetDynamicProfileCacheFlag)(BSTR flag);
bool (*pfIsEnabledDpcFlag)();
int32_t (*pfGetDpcFlag)(BSTR *flag);
int32_t (*pfSetDpcFlag)(BSTR flag);
bool (*pfIsEnabledDynamicProfileCacheDirFlag)();
int32_t (*pfGetDynamicProfileCacheDirFlag)(BSTR *flag);
int32_t (*pfSetDynamicProfileCacheDirFlag)(BSTR flag);
bool (*pfIsEnabledDynamicProfileInputFlag)();
int32_t (*pfGetDynamicProfileInputFlag)(BSTR *flag);
int32_t (*pfSetDynamicProfileInputFlag)(BSTR flag);
bool (*pfIsEnabledDpiFlag)();
int32_t (*pfGetDpiFlag)(BSTR *flag);
int32_t (*pfSetDpiFlag)(BSTR flag);
#endif
#ifdef EDIT_AND_CONTINUE
FLAG_Boolean(EditTest)
#endif
FLAG_Boolean(WininetProfileCache)
FLAG_Boolean(NoDynamicProfileInMemoryCache)
FLAG_Boolean(ProfileBasedSpeculativeJit)
FLAG_Number(ProfileBasedSpeculationCap)
FLAG_Boolean(ExecuteByteCodeBufferReturnsInvalidByteCode)
FLAG_Number(ExpirableCollectionGCCount)
FLAG_Number(ExpirableCollectionTriggerThreshold)
FLAG_Boolean(SkipSplitOnNoResult)
FLAG_Boolean(Force32BitByteCode)

FLAG_Boolean(CollectGarbage)

FLAG_Boolean(Intl)
FLAG_Boolean(IntlBuiltIns)
FLAG_Boolean(IntlPlatform)

FLAG_Boolean(JsBuiltIn)
FLAG_Boolean(JitRepro)
FLAG_Boolean(EntryPointInfoRpcData)

FLAG_Boolean(LdChakraLib)
FLAG_Boolean(TestChakraLib)

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
FLAG_Boolean(ES6)

// Master ES6 flag to enable ALL sub ES6 features/flags
FLAG_Boolean(ES6All)

// Master ES6 flag to enable Threshold ES6 features/flags
FLAG_Boolean(ES6Experimental)

// Per ES6 feature/flag

FLAG_Boolean(ES7AsyncAwait)
FLAG_Boolean(ES6DateParseFix)
FLAG_Boolean(ES6FunctionNameFull)
FLAG_Boolean(ES6Generators)
FLAG_Boolean(ES7ExponentiationOperator)

FLAG_Boolean(ES7ValuesEntries)
FLAG_Boolean(ES7TrailingComma)
FLAG_Boolean(ES6IsConcatSpreadable)
FLAG_Boolean(ES6Math)

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
FLAG_Boolean(ESDynamicImport)

FLAG_Boolean(ES6Module)
FLAG_Boolean(ES6Object)
FLAG_Boolean(ES6Number)
FLAG_Boolean(ES6ObjectLiterals)
FLAG_Boolean(ES6Proxy)
FLAG_Boolean(ES6Rest)
FLAG_Boolean(ES6Spread)
FLAG_Boolean(ES6String)
FLAG_Boolean(ES6StringPrototypeFixes)
FLAG_Boolean(ES2018ObjectRestSpread)

FLAG_Boolean(ES6PrototypeChain)
FLAG_Boolean(ES6ToPrimitive)
FLAG_Boolean(ES6ToLength)
FLAG_Boolean(ES6ToStringTag)
FLAG_Boolean(ES6Unicode)
FLAG_Boolean(ES6UnicodeVerbose)
FLAG_Boolean(ES6Unscopables)
FLAG_Boolean(ES6RegExSticky)
FLAG_Boolean(ES2018RegExDotAll)
FLAG_Boolean(ESExportNsAs)
FLAG_Boolean(ES2018AsyncIteration)
FLAG_Boolean(ESTopLevelAwait)

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
FLAG_Boolean(ES6RegExPrototypeProperties)

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
FLAG_Boolean(ES6RegExSymbols)

FLAG_Boolean(ES6Verbose)
FLAG_Boolean(ESObjectGetOwnPropertyDescriptors)

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

FLAG_Boolean(ESSharedArrayBuffer)

// Newer language feature flags

// ES BigInt flag
FLAG_Boolean(ESBigInt)

// ES Numeric Separator support for numeric constants
FLAG_Boolean(ESNumericSeparator)

// ES Nullish coalescing operator support (??)
FLAG_Boolean(ESNullishCoalescingOperator)

// ES Hashbang support for interpreter directive syntax
FLAG_Boolean(ESHashbang)

// ES Symbol.prototype.description flag
FLAG_Boolean(ESSymbolDescription)

FLAG_Boolean(ESArrayFindFromLast)

// ES Promise.any and AggregateError flag
FLAG_Boolean(ESPromiseAny)

// ES import.meta keyword meta-property
FLAG_Boolean(ESImportMeta)

//ES globalThis flag
FLAG_Boolean(ESGlobalThis)

// This flag to be removed once JITing generator functions is stable
FLAG_Boolean(JitES6Generators)

FLAG_Boolean(FastLineColumnCalculation)
bool (*pfIsEnabledFilenameFlag)();
int32_t (*pfGetFilenameFlag)(BSTR *flag);
int32_t (*pfSetFilenameFlag)(BSTR flag);
FLAG_Boolean(FreeRejittedCode)
FLAG_Boolean(ForceGuardPages)
FLAG_Boolean(PrintGuardPageBounds)
FLAG_Boolean(ForceLegacyEngine)
FLAG_Boolean(ForceArrayBTree)
FLAG_Boolean(StrongArraySort)
FLAG_Boolean(ForceCleanPropertyOnCollect)
FLAG_Boolean(ForceCleanCacheOnCollect)
FLAG_Boolean(ForceGCAfterJSONParse)
FLAG_Boolean(ForceDecommitOnCollect)
FLAG_Boolean(ForceDeferParse)
FLAG_Boolean(ForceDiagnosticsMode)
FLAG_Boolean(ForceGetWriteWatchOOM)
FLAG_Boolean(ForcePostLowerGlobOptInstrString)
FLAG_Boolean(ForceSplitScope)
FLAG_Boolean(EnumerateSpecialPropertiesInDebugger)
FLAG_Boolean(EnableContinueAfterExceptionWrappersForHelpers)
FLAG_Boolean(EnableContinueAfterExceptionWrappersForBuiltIns)
FLAG_Boolean(EnableFunctionSourceReportForHeapEnum)
FLAG_Number(ForceFragmentAddressSpace)
FLAG_Number(ForceOOMOnEBCommit)
FLAG_Boolean(ForceDynamicProfile)
FLAG_Boolean(ForceES5Array)
FLAG_Boolean(ForceAsmJsLinkFail)
FLAG_Boolean(ForceExpireOnNonCacheCollect)
FLAG_Boolean(ForceFastPath)
FLAG_Boolean(ForceFloatPref)
FLAG_Boolean(ForceJITLoopBody)
FLAG_Boolean(ForceStaticInterpreterThunk)
FLAG_Boolean(DumpCommentsFromReferencedFiles)
FLAG_Number(DelayFullJITSmallFunc)
FLAG_Boolean(EnableFatalErrorOnOOM)

#if defined(_M_ARM32_OR_ARM64)
FLAG_Boolean(ForceLocalsPtr)
#endif
FLAG_Boolean(DeferLoadingAvailableSource)
FLAG_Boolean(ForceNative)
FLAG_Boolean(ForceSerialized)
FLAG_Number(ForceSerializedBytecodeMajorVersion)
FLAG_Number(ForceSerializedBytecodeVersionSchema)
FLAG_Boolean(ForceStrictMode)
FLAG_Boolean(ForceUndoDefer)
FLAG_Boolean(ForceBlockingConcurrentCollect)
FLAG_Boolean(FreTestDiagMode)
#ifdef BYTECODE_TESTING
FLAG_Number(ByteCodeBranchLimit)
FLAG_Boolean(MediumByteCodeLayout)
FLAG_Boolean(LargeByteCodeLayout)
#endif
FLAG_Number(InduceCodeGenFailure)
FLAG_Number(InduceCodeGenFailureSeed)
FLAG_Number(InjectPartiallyInitializedInterpreterFrameError)
FLAG_Number(InjectPartiallyInitializedInterpreterFrameErrorType)
FLAG_Boolean(GenerateByteCodeBufferReturnsCantGenerate)
FLAG_Number(GoptCleanupThreshold)
FLAG_Number(AsmGoptCleanupThreshold)
FLAG_Boolean(HighPrecisionDate)
FLAG_Number(InlineCountMax)
FLAG_Number(InlineCountMaxInLoopBodies)
FLAG_Number(icminlb)
FLAG_Number(InlineInLoopBodyScaleDownFactor)
FLAG_Number(iilbsdf)
FLAG_Number(InlineThreshold)
FLAG_Number(AggressiveInlineCountMax)
FLAG_Number(AggressiveInlineThreshold)
FLAG_Number(InlineThresholdAdjustCountInLargeFunction)
FLAG_Number(InlineThresholdAdjustCountInMediumSizedFunction)
FLAG_Number(InlineThresholdAdjustCountInSmallFunction)
FLAG_Number(AsmJsInlineAdjust)
bool (*pfIsEnabledInterpretFlag)();
int32_t (*pfGetInterpretFlag)(BSTR *flag);
int32_t (*pfSetInterpretFlag)(BSTR flag);
FLAG_Number(JitQueueThreshold)
#ifdef LEAK_REPORT
bool (*pfIsEnabledLeakReportFlag)();
int32_t (*pfGetLeakReportFlag)(BSTR *flag);
int32_t (*pfSetLeakReportFlag)(BSTR flag);
#endif
FLAG_Number(LoopInlineThreshold)
FLAG_Number(LeafInlineThreshold)
FLAG_Number(ConstantArgumentInlineThreshold)
FLAG_Number(RecursiveInlineThreshold)
FLAG_Number(RecursiveInlineDepthMax)
FLAG_Number(RecursiveInlineDepthMin)
FLAG_Number(RedeferralCap)
FLAG_Number(Loop)
FLAG_Number(LoopInterpretCount)
FLAG_Number(lic)
FLAG_Number(LoopProfileIterations)
FLAG_Number(OutsideLoopInlineThreshold)
FLAG_Number(MaxFuncInlineDepth)
FLAG_Number(MaxNumberOfInlineesWithLoop)
#ifdef MEMSPECT_TRACKING
#endif
FLAG_Number(PolymorphicInlineThreshold)
FLAG_Boolean(PrimeRecycler)
FLAG_Boolean(TraceEngineRefcount)
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
FLAG_Boolean(LeakStackTrace)
FLAG_Boolean(ForceMemoryLeak)
#endif
FLAG_Boolean(DumpAfterFinalGC)
FLAG_Boolean(ForceOldDateAPI)

FLAG_Number(JitLoopBodyHotLoopThreshold)
FLAG_Number(LoopBodySizeThresholdToDisableOpts)

FLAG_Number(MaxJitThreadCount)
FLAG_Boolean(ForceMaxJitThreadCount)

FLAG_Boolean(MitigateSpectre)

FLAG_Boolean(AddMaskingBlocks)

FLAG_Boolean(PoisonVarArrayLoad)
FLAG_Boolean(PoisonIntArrayLoad)
FLAG_Boolean(PoisonFloatArrayLoad)
FLAG_Boolean(PoisonTypedArrayLoad)
FLAG_Boolean(PoisonStringLoad)
FLAG_Boolean(PoisonObjectsForLoads)

FLAG_Boolean(PoisonVarArrayStore)
FLAG_Boolean(PoisonIntArrayStore)
FLAG_Boolean(PoisonFloatArrayStore)
FLAG_Boolean(PoisonTypedArrayStore)
FLAG_Boolean(PoisonStringStore)
FLAG_Boolean(PoisonObjectsForStores)

FLAG_Number(MinInterpretCount)
FLAG_Number(MinSimpleJitRunCount)
FLAG_Number(MaxInterpretCount)
FLAG_Number(Mic)
FLAG_Number(MaxSimpleJitRunCount)
FLAG_Number(Msjrc)
FLAG_Number(MinMemOpCount)
FLAG_Number(Mmoc)

#if ENABLE_COPYONACCESS_ARRAY
FLAG_Number(MaxCopyOnAccessArrayLength)
FLAG_Number(MinCopyOnAccessArrayLength)
FLAG_Number(CopyOnAccessArraySegmentCacheSize)
#endif

FLAG_Number(MinTemplatizedJitRunCount)
FLAG_Number(MinAsmJsInterpreterRunCount)

FLAG_Number(MinTemplatizedJitLoopRunCount)
FLAG_Number(MaxTemplatizedJitRunCount)
FLAG_Number(Mtjrc)
FLAG_Number(MaxAsmJsInterpreterRunCount)
FLAG_Number(Maic)

FLAG_Number(AutoProfilingInterpreter0Limit)
FLAG_Number(ProfilingInterpreter0Limit)
FLAG_Number(AutoProfilingInterpreter1Limit)
FLAG_Number(SimpleJitLimit)
FLAG_Number(ProfilingInterpreter1Limit)

bool (*pfIsEnabledExecutionModeLimitsFlag)();
int32_t (*pfGetExecutionModeLimitsFlag)(BSTR *flag);
int32_t (*pfSetExecutionModeLimitsFlag)(BSTR flag);
bool (*pfIsEnabledEmlFlag)();
int32_t (*pfGetEmlFlag)(BSTR *flag);
int32_t (*pfSetEmlFlag)(BSTR flag);
FLAG_Boolean(EnforceExecutionModeLimits)
FLAG_Boolean(Eeml)

FLAG_Number(SimpleJitAfter)
FLAG_Number(Sja)
FLAG_Number(FullJitAfter)
FLAG_Number(Fja)

FLAG_Boolean(NewSimpleJit)

FLAG_Number(MaxLinearIntCaseCount)
FLAG_Number(MaxSingleCharStrJumpTableSize)
FLAG_Number(MaxSingleCharStrJumpTableRatio)
FLAG_Number(MinSwitchJumpTableSize)
FLAG_Number(MaxLinearStringCaseCount)
FLAG_Number(MinDeferredFuncTokenCount)
#if DBG
FLAG_Number(SkipFuncCountForBailOnNoProfile)
#endif
FLAG_Number(MaxJITFunctionBytecodeByteLength)
FLAG_Number(MaxJITFunctionBytecodeCount)
FLAG_Number(MaxLoopsPerFunction)
FLAG_Number(FuncObjectInlineCacheThreshold)
FLAG_Boolean(NoDeferParse)
FLAG_Boolean(NoLogo)
FLAG_Boolean(OOPJITMissingOpts)
FLAG_Boolean(CrashOnOOPJITFailure)
FLAG_Boolean(OOPCFGRegistration)
FLAG_Boolean(ForceJITCFGCheck)
FLAG_Boolean(UseJITTrampoline)
FLAG_Boolean(NoNative)
FLAG_Number(NopFrequency)
FLAG_Boolean(NoStrictMode)
FLAG_Boolean(NormalizeStats)
bool (*pfIsEnabledOutputFileFlag)();
int32_t (*pfGetOutputFileFlag)(BSTR *flag);
int32_t (*pfSetOutputFileFlag)(BSTR flag);
bool (*pfIsEnabledOutputFileOpenModeFlag)();
int32_t (*pfGetOutputFileOpenModeFlag)(BSTR *flag);
int32_t (*pfSetOutputFileOpenModeFlag)(BSTR flag);
#ifdef ENABLE_TRACE
FLAG_Boolean(InMemoryTrace)
FLAG_Number(InMemoryTraceBufferSize)
#ifdef STACK_BACK_TRACE
FLAG_Boolean(TraceWithStack)
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
FLAG_Boolean(PrintRunTimeDataCollectionTrace)
#ifdef ENABLE_PREJIT
FLAG_Boolean(Prejit)
#endif
FLAG_Boolean(PrintSrcInDump)
#if PROFILE_DICTIONARY
FLAG_Number(ProfileDictionary)
#endif
#ifdef PROFILE_EXEC
FLAG_Number(ProfileThreshold)
#endif
#ifdef PROFILE_OBJECT_LITERALS
FLAG_Boolean(ProfileObjectLiteral)
#endif
#ifdef PROFILE_MEM
bool (*pfIsEnabledProfileMemoryFlag)();
int32_t (*pfGetProfileMemoryFlag)(BSTR *flag);
int32_t (*pfSetProfileMemoryFlag)(BSTR flag);
#endif
#ifdef PROFILE_STRINGS
FLAG_Boolean(ProfileStrings)
#endif
#ifdef PROFILE_TYPES
FLAG_Boolean(ProfileTypes)
#endif
#ifdef PROFILE_EVALMAP
FLAG_Boolean(ProfileEvalMap)
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
FLAG_Boolean(ProfileBailOutRecordMemory)
#endif

#if DBG
FLAG_Boolean(ValidateIntRanges)
#endif
FLAG_Number(RejitMaxBailOutCount)
FLAG_Number(CallsToBailoutsRatioForRejit)
FLAG_Number(LoopIterationsToBailoutsRatioForRejit)
FLAG_Number(MinBailOutsBeforeRejit)
FLAG_Number(MinBailOutsBeforeRejitForLoops)
FLAG_Boolean(LibraryStackFrame)
FLAG_Boolean(LibraryStackFrameDebugger)
#ifdef RECYCLER_STRESS
FLAG_Boolean(RecyclerStress)
#if ENABLE_CONCURRENT_GC
FLAG_Boolean(RecyclerBackgroundStress)
FLAG_Boolean(RecyclerConcurrentStress)
FLAG_Boolean(RecyclerConcurrentRepeatStress)
#endif
#if ENABLE_PARTIAL_GC
FLAG_Boolean(RecyclerPartialStress)
#endif
FLAG_Boolean(RecyclerTrackStress)
FLAG_Boolean(RecyclerInduceFalsePositives)
#endif // RECYCLER_STRESS
FLAG_Boolean(RecyclerForceMarkInterior)
#if ENABLE_CONCURRENT_GC
FLAG_Number(RecyclerPriorityBoostTimeout)
FLAG_Number(RecyclerThreadCollectTimeout)
FLAG_Boolean(EnableConcurrentSweepAlloc)
FLAG_Boolean(ecsa)
#endif
#ifdef RECYCLER_PAGE_HEAP
FLAG_Number(PageHeap)
FLAG_Boolean(PageHeapAllocStack)
FLAG_Boolean(PageHeapFreeStack)
FLAG_Number(PageHeapBlockType)
FLAG_Boolean(PageHeapDecommitGuardPage)
#endif
#ifdef RECYCLER_NO_PAGE_REUSE
FLAG_Boolean(RecyclerNoPageReuse)
#endif
#ifdef RECYCLER_MEMORY_VERIFY
FLAG_Number(RecyclerVerifyPadSize)
#endif
FLAG_Boolean(RecyclerTest)
FLAG_Boolean(RecyclerProtectPagesOnRescan)
#ifdef RECYCLER_VERIFY_MARK
FLAG_Boolean(RecyclerVerifyMark)
#endif
FLAG_Number(LowMemoryCap)
FLAG_Number(NewPagesCapDuringBGSweeping)
FLAG_Number(AllocPolicyLimit)
#ifdef RUNTIME_DATA_COLLECTION
bool (*pfIsEnabledRuntimeDataOutputFileFlag)();
int32_t (*pfGetRuntimeDataOutputFileFlag)(BSTR *flag);
int32_t (*pfSetRuntimeDataOutputFileFlag)(BSTR flag);
#endif
FLAG_Number(SpeculationCap)
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
#endif
#if EXCEPTION_RECOVERY
FLAG_Boolean(SwallowExceptions)
#endif
FLAG_Boolean(PrintSystemException)
FLAG_Number(SwitchOptHolesThreshold)
FLAG_Number(TempMin)
FLAG_Number(TempMax)

#if defined(_M_X64)
FLAG_Number(LoopAlignNopLimit)
#endif

#ifdef PROFILE_MEM
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
FLAG_Number(TraceMetaDataParsing)
FLAG_Boolean(TraceWin8Allocations)
FLAG_Boolean(TraceWin8DeallocationsImmediate)
FLAG_Boolean(PrintWin8StatsDetailed)
FLAG_Boolean(TraceProtectPages)
#endif
FLAG_Boolean(TraceAsyncDebugCalls)
#ifdef TRACK_DISPATCH
FLAG_Boolean(TrackDispatch)
#endif
FLAG_Boolean(Verbose)
FLAG_Boolean(UseFullName)
FLAG_Boolean(Utf8)
FLAG_Number(Version)
FLAG_Boolean(WERExceptionSupport)
FLAG_Boolean(ExtendedErrorStackForTestHost)
FLAG_Boolean(errorStackTrace)
FLAG_Boolean(DoHeapEnumOnEngineShutdown)
#ifdef HEAP_ENUMERATION_VALIDATION
FLAG_Boolean(ValidateHeapEnum)
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
FLAG_Boolean(RegexTracing)
FLAG_Boolean(RegexProfile)
FLAG_Boolean(RegexDebug)
FLAG_Boolean(RegexDebugAST)
FLAG_Boolean(RegexDebugAnnotatedAST)
FLAG_Boolean(RegexBytecodeDebug)
FLAG_Boolean(RegexOptimize)
FLAG_Number(DynamicRegexMruListSize)
#endif

FLAG_Boolean(OptimizeForManyInstances)
FLAG_Boolean(EnableArrayTypeMutation)
FLAG_Number(ArrayMutationTestSeed)
FLAG_Boolean(EnableEvalMapCleanup)
#ifdef PROFILE_MEM
FLAG_Boolean(TraceObjectAllocation)
#endif
FLAG_Number(Sse)
FLAG_Number(DeletedPropertyReuseThreshold)
FLAG_Boolean(ForceStringKeyedSimpleDictionaryTypeHandler)
FLAG_Number(BigDictionaryTypeHandlerThreshold)
FLAG_Boolean(TypeSnapshotEnumeration)
FLAG_Boolean(IsolatePrototypes)
FLAG_Boolean(ChangeTypeOnProto)
FLAG_Boolean(ShareInlineCaches)
FLAG_Boolean(DisableDebugObject)
FLAG_Boolean(DumpHeap)
bool (*pfIsEnabledautoProxyFlag)();
int32_t (*pfGetautoProxyFlag)(BSTR *flag);
int32_t (*pfSetautoProxyFlag)(BSTR flag);
FLAG_Number(PerfHintLevel)
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
FLAG_Boolean(MemProtectHeap)
#endif
#ifdef RECYCLER_STRESS
FLAG_Boolean(MemProtectHeapStress)
#if ENABLE_CONCURRENT_GC
FLAG_Boolean(MemProtectHeapBackgroundStress)
FLAG_Boolean(MemProtectHeapConcurrentStress)
FLAG_Boolean(MemProtectHeapConcurrentRepeatStress)
#endif
#if ENABLE_PARTIAL_GC
FLAG_Boolean(MemProtectHeapPartialStress)
#endif
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
FLAG_Boolean(FixPropsOnPathTypes)
#endif

// recycler heuristic flags
FLAG_Number(MaxBackgroundFinishMarkCount)
FLAG_Number(BackgroundFinishMarkWaitTime)
FLAG_Number(MinBackgroundRepeatMarkRescanBytes)

#if defined(_M_X64)
FLAG_Boolean(ZeroMemoryWithNonTemporalStore)
#endif

// recycler memory restrict test flags
FLAG_Number(MaxMarkStackPageCount)
FLAG_Number(MaxTrackedObjectListCount)

// make the recycler page integration path easier to hit
FLAG_Number(NumberAllocPlusSize)

#if DBG
FLAG_Boolean(InitializeInterpreterSlotsWithInvalidStackVar)
#endif

#if DBG
FLAG_Number(PRNGSeed0)
FLAG_Number(PRNGSeed1)
#endif

FLAG_Boolean(ClearInlineCachesOnCollect)
FLAG_Number(InlineCacheInvalidationListCompactionThreshold)
FLAG_Number(ConstructorCacheInvalidationThreshold)

#ifdef IR_VIEWER
FLAG_Boolean(IRViewer)
#endif /* IR_VIEWER */

FLAG_Number(GCMemoryThreshold)

#if DBG
    FLAG_Number(SimulatePolyCacheWithOneTypeForInlineCacheIndex)
#endif

FLAG_Number(JITServerIdleTimeout)
FLAG_Number(JITServerMaxInactivePageAllocatorCount)

FLAG_Boolean(StrictWriteBarrierCheck)
FLAG_Boolean(WriteBarrierTest)
FLAG_Boolean(ForceSoftwareWriteBarrier)
FLAG_Boolean(VerifyBarrierBit)
FLAG_Boolean(EnableBGFreeZero)
FLAG_Boolean(KeepRecyclerTrackData)

FLAG_Number(MaxSingleAllocSizeInMB)

// TODO (hanhossain): ConfigFlagsList end
#undef FLAG_Boolean
#undef FLAG_Number

    NotifyUnhandledExceptionPtr pfnNotifyUnhandledException;
};

int32_t OnChakraCoreLoaded();

