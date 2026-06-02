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
bool (*pfIsEnabledArrayValidateFlag)();
int32_t (*pfGetArrayValidateFlag)(bool *flag);
int32_t (*pfSetArrayValidateFlag)(bool flag);
bool (*pfIsEnabledMemOpMissingValueValidateFlag)();
int32_t (*pfGetMemOpMissingValueValidateFlag)(bool *flag);
int32_t (*pfSetMemOpMissingValueValidateFlag)(bool flag);
bool (*pfIsEnabledOOPJITFixupValidateFlag)();
int32_t (*pfGetOOPJITFixupValidateFlag)(bool *flag);
int32_t (*pfSetOOPJITFixupValidateFlag)(bool flag);
#endif
#ifdef ARENA_MEMORY_VERIFY
bool (*pfIsEnabledArenaNoFreeListFlag)();
int32_t (*pfGetArenaNoFreeListFlag)(bool *flag);
int32_t (*pfSetArenaNoFreeListFlag)(bool flag);
bool (*pfIsEnabledArenaNoPageReuseFlag)();
int32_t (*pfGetArenaNoPageReuseFlag)(bool *flag);
int32_t (*pfSetArenaNoPageReuseFlag)(bool flag);
bool (*pfIsEnabledArenaUseHeapAllocFlag)();
int32_t (*pfGetArenaUseHeapAllocFlag)(bool *flag);
int32_t (*pfSetArenaUseHeapAllocFlag)(bool flag);
#endif
bool (*pfIsEnabledValidateInlineStackFlag)();
int32_t (*pfGetValidateInlineStackFlag)(bool *flag);
int32_t (*pfSetValidateInlineStackFlag)(bool flag);
bool (*pfIsEnabledAsmDiffFlag)();
int32_t (*pfGetAsmDiffFlag)(bool *flag);
int32_t (*pfSetAsmDiffFlag)(bool flag);
bool (*pfIsEnabledAsmDumpModeFlag)();
int32_t (*pfGetAsmDumpModeFlag)(BSTR *flag);
int32_t (*pfSetAsmDumpModeFlag)(BSTR flag);
bool (*pfIsEnabledAsmJsFlag)();
int32_t (*pfGetAsmJsFlag)(bool *flag);
int32_t (*pfSetAsmJsFlag)(bool flag);
bool (*pfIsEnabledAsmJsStopOnErrorFlag)();
int32_t (*pfGetAsmJsStopOnErrorFlag)(bool *flag);
int32_t (*pfSetAsmJsStopOnErrorFlag)(bool flag);
bool (*pfIsEnabledAsmJsEdgeFlag)();
int32_t (*pfGetAsmJsEdgeFlag)(bool *flag);
int32_t (*pfSetAsmJsEdgeFlag)(bool flag);
bool (*pfIsEnabledWasmFlag)();
int32_t (*pfGetWasmFlag)(bool *flag);
int32_t (*pfSetWasmFlag)(bool flag);
bool (*pfIsEnabledWasmI64Flag)();
int32_t (*pfGetWasmI64Flag)(bool *flag);
int32_t (*pfSetWasmI64Flag)(bool flag);
bool (*pfIsEnabledWasmFastArrayFlag)();
int32_t (*pfGetWasmFastArrayFlag)(bool *flag);
int32_t (*pfSetWasmFastArrayFlag)(bool flag);
bool (*pfIsEnabledWasmSharedArrayVirtualBufferFlag)();
int32_t (*pfGetWasmSharedArrayVirtualBufferFlag)(bool *flag);
int32_t (*pfSetWasmSharedArrayVirtualBufferFlag)(bool flag);
bool (*pfIsEnabledWasmMathExFilterFlag)();
int32_t (*pfGetWasmMathExFilterFlag)(bool *flag);
int32_t (*pfSetWasmMathExFilterFlag)(bool flag);
bool (*pfIsEnabledWasmCheckVersionFlag)();
int32_t (*pfGetWasmCheckVersionFlag)(bool *flag);
int32_t (*pfSetWasmCheckVersionFlag)(bool flag);
bool (*pfIsEnabledWasmAssignModuleIDFlag)();
int32_t (*pfGetWasmAssignModuleIDFlag)(bool *flag);
int32_t (*pfSetWasmAssignModuleIDFlag)(bool flag);
bool (*pfIsEnabledWasmIgnoreLimitsFlag)();
int32_t (*pfGetWasmIgnoreLimitsFlag)(bool *flag);
int32_t (*pfSetWasmIgnoreLimitsFlag)(bool flag);
bool (*pfIsEnabledWasmFoldFlag)();
int32_t (*pfGetWasmFoldFlag)(bool *flag);
int32_t (*pfSetWasmFoldFlag)(bool flag);
bool (*pfIsEnabledWasmIgnoreResponseFlag)();
int32_t (*pfGetWasmIgnoreResponseFlag)(bool *flag);
int32_t (*pfSetWasmIgnoreResponseFlag)(bool flag);
FLAG_Number(WasmMaxTableSize)
bool (*pfIsEnabledWasmThreadsFlag)();
int32_t (*pfGetWasmThreadsFlag)(bool *flag);
int32_t (*pfSetWasmThreadsFlag)(bool flag);
bool (*pfIsEnabledWasmMultiValueFlag)();
int32_t (*pfGetWasmMultiValueFlag)(bool *flag);
int32_t (*pfSetWasmMultiValueFlag)(bool flag);
bool (*pfIsEnabledWasmSignExtendsFlag)();
int32_t (*pfGetWasmSignExtendsFlag)(bool *flag);
int32_t (*pfSetWasmSignExtendsFlag)(bool flag);
bool (*pfIsEnabledWasmNontrappingFlag)();
int32_t (*pfGetWasmNontrappingFlag)(bool *flag);
int32_t (*pfSetWasmNontrappingFlag)(bool flag);

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
bool (*pfIsEnabledWasmExperimentalFlag)();
int32_t (*pfGetWasmExperimentalFlag)(bool *flag);
int32_t (*pfSetWasmExperimentalFlag)(bool flag);

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
bool (*pfIsEnabledWasmSimdFlag)();
int32_t (*pfGetWasmSimdFlag)(bool *flag);
int32_t (*pfSetWasmSimdFlag)(bool flag);

bool (*pfIsEnabledAssertBreakFlag)();
int32_t (*pfGetAssertBreakFlag)(bool *flag);
int32_t (*pfSetAssertBreakFlag)(bool flag);
bool (*pfIsEnabledAssertPopUpFlag)();
int32_t (*pfGetAssertPopUpFlag)(bool *flag);
int32_t (*pfSetAssertPopUpFlag)(bool flag);
bool (*pfIsEnabledAssertIgnoreFlag)();
int32_t (*pfGetAssertIgnoreFlag)(bool *flag);
int32_t (*pfSetAssertIgnoreFlag)(bool flag);
bool (*pfIsEnabledAsyncDebuggingFlag)();
int32_t (*pfGetAsyncDebuggingFlag)(bool *flag);
int32_t (*pfSetAsyncDebuggingFlag)(bool flag);
FLAG_Number(BailOnNoProfileLimit)
FLAG_Number(BailOnNoProfileRejitLimit)
bool (*pfIsEnabledBaselineModeFlag)();
int32_t (*pfGetBaselineModeFlag)(bool *flag);
int32_t (*pfSetBaselineModeFlag)(bool flag);
bool (*pfIsEnabledDumpOnCrashFlag)();
int32_t (*pfGetDumpOnCrashFlag)(BSTR *flag);
int32_t (*pfSetDumpOnCrashFlag)(BSTR flag);
bool (*pfIsEnabledFullMemoryDumpFlag)();
int32_t (*pfGetFullMemoryDumpFlag)(BSTR *flag);
int32_t (*pfSetFullMemoryDumpFlag)(BSTR flag);
#ifdef BAILOUT_INJECTION
bool (*pfIsEnabledBailOutAtEveryLineFlag)();
int32_t (*pfGetBailOutAtEveryLineFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryLineFlag)(bool flag);
bool (*pfIsEnabledBailOutAtEveryByteCodeFlag)();
int32_t (*pfGetBailOutAtEveryByteCodeFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryByteCodeFlag)(bool flag);
bool (*pfIsEnabledBailOutAtEveryImplicitCallFlag)();
int32_t (*pfGetBailOutAtEveryImplicitCallFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryImplicitCallFlag)(bool flag);
#endif
bool (*pfIsEnabledBenchmarkFlag)();
int32_t (*pfGetBenchmarkFlag)(bool *flag);
int32_t (*pfSetBenchmarkFlag)(bool flag);
bool (*pfIsEnabledBgJitFlag)();
int32_t (*pfGetBgJitFlag)(bool *flag);
int32_t (*pfSetBgJitFlag)(bool flag);
bool (*pfIsEnabledBgParseFlag)();
int32_t (*pfGetBgParseFlag)(bool *flag);
int32_t (*pfSetBgParseFlag)(bool flag);
FLAG_Number(BgJitDelay)
FLAG_Number(BgJitDelayFgBuffer)
FLAG_Number(BgJitPendingFuncCap)

bool (*pfIsEnabledCreateFunctionProxyFlag)();
int32_t (*pfGetCreateFunctionProxyFlag)(bool *flag);
int32_t (*pfSetCreateFunctionProxyFlag)(bool flag);
bool (*pfIsEnabledHybridFgJitFlag)();
int32_t (*pfGetHybridFgJitFlag)(bool *flag);
int32_t (*pfSetHybridFgJitFlag)(bool flag);
FLAG_Number(HybridFgJitBgQueueLengthThreshold)
bool (*pfIsEnabledBytecodeHistFlag)();
int32_t (*pfGetBytecodeHistFlag)(bool *flag);
int32_t (*pfSetBytecodeHistFlag)(bool flag);
bool (*pfIsEnabledCurrentSourceInfoFlag)();
int32_t (*pfGetCurrentSourceInfoFlag)(bool *flag);
int32_t (*pfSetCurrentSourceInfoFlag)(bool flag);
bool (*pfIsEnabledCFGLogFlag)();
int32_t (*pfGetCFGLogFlag)(bool *flag);
int32_t (*pfSetCFGLogFlag)(bool flag);
bool (*pfIsEnabledCheckAlignmentFlag)();
int32_t (*pfGetCheckAlignmentFlag)(bool *flag);
int32_t (*pfSetCheckAlignmentFlag)(bool flag);
bool (*pfIsEnabledCheckEmitBufferPermissionsFlag)();
int32_t (*pfGetCheckEmitBufferPermissionsFlag)(bool *flag);
int32_t (*pfSetCheckEmitBufferPermissionsFlag)(bool flag);
#ifdef CHECK_MEMORY_LEAK
bool (*pfIsEnabledCheckMemoryLeakFlag)();
int32_t (*pfGetCheckMemoryLeakFlag)(bool *flag);
int32_t (*pfSetCheckMemoryLeakFlag)(bool flag);
bool (*pfIsEnabledDumpOnLeakFlag)();
int32_t (*pfGetDumpOnLeakFlag)(BSTR *flag);
int32_t (*pfSetDumpOnLeakFlag)(BSTR flag);
#endif
bool (*pfIsEnabledCheckOpHelpersFlag)();
int32_t (*pfGetCheckOpHelpersFlag)(bool *flag);
int32_t (*pfSetCheckOpHelpersFlag)(bool flag);
bool (*pfIsEnabledCloneInlinedPolymorphicCachesFlag)();
int32_t (*pfGetCloneInlinedPolymorphicCachesFlag)(bool *flag);
int32_t (*pfSetCloneInlinedPolymorphicCachesFlag)(bool flag);
bool (*pfIsEnabledConcurrentRuntimeFlag)();
int32_t (*pfGetConcurrentRuntimeFlag)(bool *flag);
int32_t (*pfSetConcurrentRuntimeFlag)(bool flag);
FLAG_Number(ConstructorInlineThreshold)
FLAG_Number(ConstructorCallsRequiredToFinalizeCachedType)
FLAG_Number(PropertyCacheMissPenalty)
FLAG_Number(PropertyCacheMissThreshold)
FLAG_Number(PropertyCacheMissReset)
bool (*pfIsEnabledDebugFlag)();
int32_t (*pfGetDebugFlag)(bool *flag);
int32_t (*pfSetDebugFlag)(bool flag);

bool (*pfIsEnabledDebugWindowFlag)();
int32_t (*pfGetDebugWindowFlag)(bool *flag);
int32_t (*pfSetDebugWindowFlag)(bool flag);
bool (*pfIsEnabledParserStateCacheFlag)();
int32_t (*pfGetParserStateCacheFlag)(bool *flag);
int32_t (*pfSetParserStateCacheFlag)(bool flag);
bool (*pfIsEnabledCompressParserStateCacheFlag)();
int32_t (*pfGetCompressParserStateCacheFlag)(bool *flag);
int32_t (*pfSetCompressParserStateCacheFlag)(bool flag);
bool (*pfIsEnabledDeferTopLevelTillFirstCallFlag)();
int32_t (*pfGetDeferTopLevelTillFirstCallFlag)(bool *flag);
int32_t (*pfSetDeferTopLevelTillFirstCallFlag)(bool flag);
FLAG_Number(DeferParse)
bool (*pfIsEnabledDirectCallTelemetryStatsFlag)();
int32_t (*pfGetDirectCallTelemetryStatsFlag)(bool *flag);
int32_t (*pfSetDirectCallTelemetryStatsFlag)(bool flag);
bool (*pfIsEnabledDisableArrayBTreeFlag)();
int32_t (*pfGetDisableArrayBTreeFlag)(bool *flag);
int32_t (*pfSetDisableArrayBTreeFlag)(bool flag);
bool (*pfIsEnabledDisableRentalThreadingFlag)();
int32_t (*pfGetDisableRentalThreadingFlag)(bool *flag);
int32_t (*pfSetDisableRentalThreadingFlag)(bool flag);
bool (*pfIsEnabledDisableVTuneSourceLineInfoFlag)();
int32_t (*pfGetDisableVTuneSourceLineInfoFlag)(bool *flag);
int32_t (*pfSetDisableVTuneSourceLineInfoFlag)(bool flag);
bool (*pfIsEnabledDisplayMemStatsFlag)();
int32_t (*pfGetDisplayMemStatsFlag)(bool *flag);
int32_t (*pfSetDisplayMemStatsFlag)(bool flag);
#ifdef DUMP_FRAGMENTATION_STATS
bool (*pfIsEnabledDumpFragmentationStatsFlag)();
int32_t (*pfGetDumpFragmentationStatsFlag)(bool *flag);
int32_t (*pfSetDumpFragmentationStatsFlag)(bool flag);
#endif
bool (*pfIsEnabledDumpIRAddressesFlag)();
int32_t (*pfGetDumpIRAddressesFlag)(bool *flag);
int32_t (*pfSetDumpIRAddressesFlag)(bool flag);
bool (*pfIsEnabledDumpLineNoInColorFlag)();
int32_t (*pfGetDumpLineNoInColorFlag)(bool *flag);
int32_t (*pfSetDumpLineNoInColorFlag)(bool flag);
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
bool (*pfIsEnabledDumpObjectGraphOnExitFlag)();
int32_t (*pfGetDumpObjectGraphOnExitFlag)(bool *flag);
int32_t (*pfSetDumpObjectGraphOnExitFlag)(bool flag);
bool (*pfIsEnabledDumpObjectGraphOnCollectFlag)();
int32_t (*pfGetDumpObjectGraphOnCollectFlag)(bool *flag);
int32_t (*pfSetDumpObjectGraphOnCollectFlag)(bool flag);
#endif
bool (*pfIsEnabledDumpEvalStringOnRemovalFlag)();
int32_t (*pfGetDumpEvalStringOnRemovalFlag)(bool *flag);
int32_t (*pfSetDumpEvalStringOnRemovalFlag)(bool flag);
bool (*pfIsEnabledDumpObjectGraphOnEnumFlag)();
int32_t (*pfGetDumpObjectGraphOnEnumFlag)(bool *flag);
int32_t (*pfSetDumpObjectGraphOnEnumFlag)(bool flag);
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
bool (*pfIsEnabledEditTestFlag)();
int32_t (*pfGetEditTestFlag)(bool *flag);
int32_t (*pfSetEditTestFlag)(bool flag);
#endif
bool (*pfIsEnabledWininetProfileCacheFlag)();
int32_t (*pfGetWininetProfileCacheFlag)(bool *flag);
int32_t (*pfSetWininetProfileCacheFlag)(bool flag);
bool (*pfIsEnabledNoDynamicProfileInMemoryCacheFlag)();
int32_t (*pfGetNoDynamicProfileInMemoryCacheFlag)(bool *flag);
int32_t (*pfSetNoDynamicProfileInMemoryCacheFlag)(bool flag);
bool (*pfIsEnabledProfileBasedSpeculativeJitFlag)();
int32_t (*pfGetProfileBasedSpeculativeJitFlag)(bool *flag);
int32_t (*pfSetProfileBasedSpeculativeJitFlag)(bool flag);
FLAG_Number(ProfileBasedSpeculationCap)
bool (*pfIsEnabledExecuteByteCodeBufferReturnsInvalidByteCodeFlag)();
int32_t (*pfGetExecuteByteCodeBufferReturnsInvalidByteCodeFlag)(bool *flag);
int32_t (*pfSetExecuteByteCodeBufferReturnsInvalidByteCodeFlag)(bool flag);
FLAG_Number(ExpirableCollectionGCCount)
FLAG_Number(ExpirableCollectionTriggerThreshold)
bool (*pfIsEnabledSkipSplitOnNoResultFlag)();
int32_t (*pfGetSkipSplitOnNoResultFlag)(bool *flag);
int32_t (*pfSetSkipSplitOnNoResultFlag)(bool flag);
bool (*pfIsEnabledForce32BitByteCodeFlag)();
int32_t (*pfGetForce32BitByteCodeFlag)(bool *flag);
int32_t (*pfSetForce32BitByteCodeFlag)(bool flag);

bool (*pfIsEnabledCollectGarbageFlag)();
int32_t (*pfGetCollectGarbageFlag)(bool *flag);
int32_t (*pfSetCollectGarbageFlag)(bool flag);

bool (*pfIsEnabledIntlFlag)();
int32_t (*pfGetIntlFlag)(bool *flag);
int32_t (*pfSetIntlFlag)(bool flag);
bool (*pfIsEnabledIntlBuiltInsFlag)();
int32_t (*pfGetIntlBuiltInsFlag)(bool *flag);
int32_t (*pfSetIntlBuiltInsFlag)(bool flag);
bool (*pfIsEnabledIntlPlatformFlag)();
int32_t (*pfGetIntlPlatformFlag)(bool *flag);
int32_t (*pfSetIntlPlatformFlag)(bool flag);

bool (*pfIsEnabledJsBuiltInFlag)();
int32_t (*pfGetJsBuiltInFlag)(bool *flag);
int32_t (*pfSetJsBuiltInFlag)(bool flag);
bool (*pfIsEnabledJitReproFlag)();
int32_t (*pfGetJitReproFlag)(bool *flag);
int32_t (*pfSetJitReproFlag)(bool flag);
bool (*pfIsEnabledEntryPointInfoRpcDataFlag)();
int32_t (*pfGetEntryPointInfoRpcDataFlag)(bool *flag);
int32_t (*pfSetEntryPointInfoRpcDataFlag)(bool flag);

bool (*pfIsEnabledLdChakraLibFlag)();
int32_t (*pfGetLdChakraLibFlag)(bool *flag);
int32_t (*pfSetLdChakraLibFlag)(bool flag);
bool (*pfIsEnabledTestChakraLibFlag)();
int32_t (*pfGetTestChakraLibFlag)(bool *flag);
int32_t (*pfSetTestChakraLibFlag)(bool flag);

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
bool (*pfIsEnabledES6Flag)();
int32_t (*pfGetES6Flag)(bool *flag);
int32_t (*pfSetES6Flag)(bool flag);

// Master ES6 flag to enable ALL sub ES6 features/flags
bool (*pfIsEnabledES6AllFlag)();
int32_t (*pfGetES6AllFlag)(bool *flag);
int32_t (*pfSetES6AllFlag)(bool flag);

// Master ES6 flag to enable Threshold ES6 features/flags
bool (*pfIsEnabledES6ExperimentalFlag)();
int32_t (*pfGetES6ExperimentalFlag)(bool *flag);
int32_t (*pfSetES6ExperimentalFlag)(bool flag);

// Per ES6 feature/flag

bool (*pfIsEnabledES7AsyncAwaitFlag)();
int32_t (*pfGetES7AsyncAwaitFlag)(bool *flag);
int32_t (*pfSetES7AsyncAwaitFlag)(bool flag);
bool (*pfIsEnabledES6DateParseFixFlag)();
int32_t (*pfGetES6DateParseFixFlag)(bool *flag);
int32_t (*pfSetES6DateParseFixFlag)(bool flag);
bool (*pfIsEnabledES6FunctionNameFullFlag)();
int32_t (*pfGetES6FunctionNameFullFlag)(bool *flag);
int32_t (*pfSetES6FunctionNameFullFlag)(bool flag);
bool (*pfIsEnabledES6GeneratorsFlag)();
int32_t (*pfGetES6GeneratorsFlag)(bool *flag);
int32_t (*pfSetES6GeneratorsFlag)(bool flag);
bool (*pfIsEnabledES7ExponentiationOperatorFlag)();
int32_t (*pfGetES7ExponentiationOperatorFlag)(bool *flag);
int32_t (*pfSetES7ExponentiationOperatorFlag)(bool flag);

bool (*pfIsEnabledES7ValuesEntriesFlag)();
int32_t (*pfGetES7ValuesEntriesFlag)(bool *flag);
int32_t (*pfSetES7ValuesEntriesFlag)(bool flag);
bool (*pfIsEnabledES7TrailingCommaFlag)();
int32_t (*pfGetES7TrailingCommaFlag)(bool *flag);
int32_t (*pfSetES7TrailingCommaFlag)(bool flag);
bool (*pfIsEnabledES6IsConcatSpreadableFlag)();
int32_t (*pfGetES6IsConcatSpreadableFlag)(bool *flag);
int32_t (*pfSetES6IsConcatSpreadableFlag)(bool flag);
bool (*pfIsEnabledES6MathFlag)();
int32_t (*pfGetES6MathFlag)(bool *flag);
int32_t (*pfSetES6MathFlag)(bool flag);

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
bool (*pfIsEnabledESDynamicImportFlag)();
int32_t (*pfGetESDynamicImportFlag)(bool *flag);
int32_t (*pfSetESDynamicImportFlag)(bool flag);

bool (*pfIsEnabledES6ModuleFlag)();
int32_t (*pfGetES6ModuleFlag)(bool *flag);
int32_t (*pfSetES6ModuleFlag)(bool flag);
bool (*pfIsEnabledES6ObjectFlag)();
int32_t (*pfGetES6ObjectFlag)(bool *flag);
int32_t (*pfSetES6ObjectFlag)(bool flag);
bool (*pfIsEnabledES6NumberFlag)();
int32_t (*pfGetES6NumberFlag)(bool *flag);
int32_t (*pfSetES6NumberFlag)(bool flag);
bool (*pfIsEnabledES6ObjectLiteralsFlag)();
int32_t (*pfGetES6ObjectLiteralsFlag)(bool *flag);
int32_t (*pfSetES6ObjectLiteralsFlag)(bool flag);
bool (*pfIsEnabledES6ProxyFlag)();
int32_t (*pfGetES6ProxyFlag)(bool *flag);
int32_t (*pfSetES6ProxyFlag)(bool flag);
bool (*pfIsEnabledES6RestFlag)();
int32_t (*pfGetES6RestFlag)(bool *flag);
int32_t (*pfSetES6RestFlag)(bool flag);
bool (*pfIsEnabledES6SpreadFlag)();
int32_t (*pfGetES6SpreadFlag)(bool *flag);
int32_t (*pfSetES6SpreadFlag)(bool flag);
bool (*pfIsEnabledES6StringFlag)();
int32_t (*pfGetES6StringFlag)(bool *flag);
int32_t (*pfSetES6StringFlag)(bool flag);
bool (*pfIsEnabledES6StringPrototypeFixesFlag)();
int32_t (*pfGetES6StringPrototypeFixesFlag)(bool *flag);
int32_t (*pfSetES6StringPrototypeFixesFlag)(bool flag);
bool (*pfIsEnabledES2018ObjectRestSpreadFlag)();
int32_t (*pfGetES2018ObjectRestSpreadFlag)(bool *flag);
int32_t (*pfSetES2018ObjectRestSpreadFlag)(bool flag);

bool (*pfIsEnabledES6PrototypeChainFlag)();
int32_t (*pfGetES6PrototypeChainFlag)(bool *flag);
int32_t (*pfSetES6PrototypeChainFlag)(bool flag);
bool (*pfIsEnabledES6ToPrimitiveFlag)();
int32_t (*pfGetES6ToPrimitiveFlag)(bool *flag);
int32_t (*pfSetES6ToPrimitiveFlag)(bool flag);
bool (*pfIsEnabledES6ToLengthFlag)();
int32_t (*pfGetES6ToLengthFlag)(bool *flag);
int32_t (*pfSetES6ToLengthFlag)(bool flag);
bool (*pfIsEnabledES6ToStringTagFlag)();
int32_t (*pfGetES6ToStringTagFlag)(bool *flag);
int32_t (*pfSetES6ToStringTagFlag)(bool flag);
bool (*pfIsEnabledES6UnicodeFlag)();
int32_t (*pfGetES6UnicodeFlag)(bool *flag);
int32_t (*pfSetES6UnicodeFlag)(bool flag);
bool (*pfIsEnabledES6UnicodeVerboseFlag)();
int32_t (*pfGetES6UnicodeVerboseFlag)(bool *flag);
int32_t (*pfSetES6UnicodeVerboseFlag)(bool flag);
bool (*pfIsEnabledES6UnscopablesFlag)();
int32_t (*pfGetES6UnscopablesFlag)(bool *flag);
int32_t (*pfSetES6UnscopablesFlag)(bool flag);
bool (*pfIsEnabledES6RegExStickyFlag)();
int32_t (*pfGetES6RegExStickyFlag)(bool *flag);
int32_t (*pfSetES6RegExStickyFlag)(bool flag);
bool (*pfIsEnabledES2018RegExDotAllFlag)();
int32_t (*pfGetES2018RegExDotAllFlag)(bool *flag);
int32_t (*pfSetES2018RegExDotAllFlag)(bool flag);
bool (*pfIsEnabledESExportNsAsFlag)();
int32_t (*pfGetESExportNsAsFlag)(bool *flag);
int32_t (*pfSetESExportNsAsFlag)(bool flag);
bool (*pfIsEnabledES2018AsyncIterationFlag)();
int32_t (*pfGetES2018AsyncIterationFlag)(bool *flag);
int32_t (*pfSetES2018AsyncIterationFlag)(bool flag);
bool (*pfIsEnabledESTopLevelAwaitFlag)();
int32_t (*pfGetESTopLevelAwaitFlag)(bool *flag);
int32_t (*pfSetESTopLevelAwaitFlag)(bool flag);

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
bool (*pfIsEnabledES6RegExPrototypePropertiesFlag)();
int32_t (*pfGetES6RegExPrototypePropertiesFlag)(bool *flag);
int32_t (*pfSetES6RegExPrototypePropertiesFlag)(bool flag);

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
bool (*pfIsEnabledES6RegExSymbolsFlag)();
int32_t (*pfGetES6RegExSymbolsFlag)(bool *flag);
int32_t (*pfSetES6RegExSymbolsFlag)(bool flag);

bool (*pfIsEnabledES6VerboseFlag)();
int32_t (*pfGetES6VerboseFlag)(bool *flag);
int32_t (*pfSetES6VerboseFlag)(bool flag);
bool (*pfIsEnabledESObjectGetOwnPropertyDescriptorsFlag)();
int32_t (*pfGetESObjectGetOwnPropertyDescriptorsFlag)(bool *flag);
int32_t (*pfSetESObjectGetOwnPropertyDescriptorsFlag)(bool flag);

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

bool (*pfIsEnabledESSharedArrayBufferFlag)();
int32_t (*pfGetESSharedArrayBufferFlag)(bool *flag);
int32_t (*pfSetESSharedArrayBufferFlag)(bool flag);

// Newer language feature flags

// ES BigInt flag
bool (*pfIsEnabledESBigIntFlag)();
int32_t (*pfGetESBigIntFlag)(bool *flag);
int32_t (*pfSetESBigIntFlag)(bool flag);

// ES Numeric Separator support for numeric constants
bool (*pfIsEnabledESNumericSeparatorFlag)();
int32_t (*pfGetESNumericSeparatorFlag)(bool *flag);
int32_t (*pfSetESNumericSeparatorFlag)(bool flag);

// ES Nullish coalescing operator support (??)
bool (*pfIsEnabledESNullishCoalescingOperatorFlag)();
int32_t (*pfGetESNullishCoalescingOperatorFlag)(bool *flag);
int32_t (*pfSetESNullishCoalescingOperatorFlag)(bool flag);

// ES Hashbang support for interpreter directive syntax
bool (*pfIsEnabledESHashbangFlag)();
int32_t (*pfGetESHashbangFlag)(bool *flag);
int32_t (*pfSetESHashbangFlag)(bool flag);

// ES Symbol.prototype.description flag
bool (*pfIsEnabledESSymbolDescriptionFlag)();
int32_t (*pfGetESSymbolDescriptionFlag)(bool *flag);
int32_t (*pfSetESSymbolDescriptionFlag)(bool flag);

bool (*pfIsEnabledESArrayFindFromLastFlag)();
int32_t (*pfGetESArrayFindFromLastFlag)(bool *flag);
int32_t (*pfSetESArrayFindFromLastFlag)(bool flag);

// ES Promise.any and AggregateError flag
bool (*pfIsEnabledESPromiseAnyFlag)();
int32_t (*pfGetESPromiseAnyFlag)(bool *flag);
int32_t (*pfSetESPromiseAnyFlag)(bool flag);

// ES import.meta keyword meta-property
bool (*pfIsEnabledESImportMetaFlag)();
int32_t (*pfGetESImportMetaFlag)(bool *flag);
int32_t (*pfSetESImportMetaFlag)(bool flag);

//ES globalThis flag
bool (*pfIsEnabledESGlobalThisFlag)();
int32_t (*pfGetESGlobalThisFlag)(bool *flag);
int32_t (*pfSetESGlobalThisFlag)(bool flag);

// This flag to be removed once JITing generator functions is stable
bool (*pfIsEnabledJitES6GeneratorsFlag)();
int32_t (*pfGetJitES6GeneratorsFlag)(bool *flag);
int32_t (*pfSetJitES6GeneratorsFlag)(bool flag);

bool (*pfIsEnabledFastLineColumnCalculationFlag)();
int32_t (*pfGetFastLineColumnCalculationFlag)(bool *flag);
int32_t (*pfSetFastLineColumnCalculationFlag)(bool flag);
bool (*pfIsEnabledFilenameFlag)();
int32_t (*pfGetFilenameFlag)(BSTR *flag);
int32_t (*pfSetFilenameFlag)(BSTR flag);
bool (*pfIsEnabledFreeRejittedCodeFlag)();
int32_t (*pfGetFreeRejittedCodeFlag)(bool *flag);
int32_t (*pfSetFreeRejittedCodeFlag)(bool flag);
bool (*pfIsEnabledForceGuardPagesFlag)();
int32_t (*pfGetForceGuardPagesFlag)(bool *flag);
int32_t (*pfSetForceGuardPagesFlag)(bool flag);
bool (*pfIsEnabledPrintGuardPageBoundsFlag)();
int32_t (*pfGetPrintGuardPageBoundsFlag)(bool *flag);
int32_t (*pfSetPrintGuardPageBoundsFlag)(bool flag);
bool (*pfIsEnabledForceLegacyEngineFlag)();
int32_t (*pfGetForceLegacyEngineFlag)(bool *flag);
int32_t (*pfSetForceLegacyEngineFlag)(bool flag);
bool (*pfIsEnabledForceArrayBTreeFlag)();
int32_t (*pfGetForceArrayBTreeFlag)(bool *flag);
int32_t (*pfSetForceArrayBTreeFlag)(bool flag);
bool (*pfIsEnabledStrongArraySortFlag)();
int32_t (*pfGetStrongArraySortFlag)(bool *flag);
int32_t (*pfSetStrongArraySortFlag)(bool flag);
bool (*pfIsEnabledForceCleanPropertyOnCollectFlag)();
int32_t (*pfGetForceCleanPropertyOnCollectFlag)(bool *flag);
int32_t (*pfSetForceCleanPropertyOnCollectFlag)(bool flag);
bool (*pfIsEnabledForceCleanCacheOnCollectFlag)();
int32_t (*pfGetForceCleanCacheOnCollectFlag)(bool *flag);
int32_t (*pfSetForceCleanCacheOnCollectFlag)(bool flag);
bool (*pfIsEnabledForceGCAfterJSONParseFlag)();
int32_t (*pfGetForceGCAfterJSONParseFlag)(bool *flag);
int32_t (*pfSetForceGCAfterJSONParseFlag)(bool flag);
bool (*pfIsEnabledForceDecommitOnCollectFlag)();
int32_t (*pfGetForceDecommitOnCollectFlag)(bool *flag);
int32_t (*pfSetForceDecommitOnCollectFlag)(bool flag);
bool (*pfIsEnabledForceDeferParseFlag)();
int32_t (*pfGetForceDeferParseFlag)(bool *flag);
int32_t (*pfSetForceDeferParseFlag)(bool flag);
bool (*pfIsEnabledForceDiagnosticsModeFlag)();
int32_t (*pfGetForceDiagnosticsModeFlag)(bool *flag);
int32_t (*pfSetForceDiagnosticsModeFlag)(bool flag);
bool (*pfIsEnabledForceGetWriteWatchOOMFlag)();
int32_t (*pfGetForceGetWriteWatchOOMFlag)(bool *flag);
int32_t (*pfSetForceGetWriteWatchOOMFlag)(bool flag);
bool (*pfIsEnabledForcePostLowerGlobOptInstrStringFlag)();
int32_t (*pfGetForcePostLowerGlobOptInstrStringFlag)(bool *flag);
int32_t (*pfSetForcePostLowerGlobOptInstrStringFlag)(bool flag);
bool (*pfIsEnabledForceSplitScopeFlag)();
int32_t (*pfGetForceSplitScopeFlag)(bool *flag);
int32_t (*pfSetForceSplitScopeFlag)(bool flag);
bool (*pfIsEnabledEnumerateSpecialPropertiesInDebuggerFlag)();
int32_t (*pfGetEnumerateSpecialPropertiesInDebuggerFlag)(bool *flag);
int32_t (*pfSetEnumerateSpecialPropertiesInDebuggerFlag)(bool flag);
bool (*pfIsEnabledEnableContinueAfterExceptionWrappersForHelpersFlag)();
int32_t (*pfGetEnableContinueAfterExceptionWrappersForHelpersFlag)(bool *flag);
int32_t (*pfSetEnableContinueAfterExceptionWrappersForHelpersFlag)(bool flag);
bool (*pfIsEnabledEnableContinueAfterExceptionWrappersForBuiltInsFlag)();
int32_t (*pfGetEnableContinueAfterExceptionWrappersForBuiltInsFlag)(bool *flag);
int32_t (*pfSetEnableContinueAfterExceptionWrappersForBuiltInsFlag)(bool flag);
bool (*pfIsEnabledEnableFunctionSourceReportForHeapEnumFlag)();
int32_t (*pfGetEnableFunctionSourceReportForHeapEnumFlag)(bool *flag);
int32_t (*pfSetEnableFunctionSourceReportForHeapEnumFlag)(bool flag);
FLAG_Number(ForceFragmentAddressSpace)
FLAG_Number(ForceOOMOnEBCommit)
bool (*pfIsEnabledForceDynamicProfileFlag)();
int32_t (*pfGetForceDynamicProfileFlag)(bool *flag);
int32_t (*pfSetForceDynamicProfileFlag)(bool flag);
bool (*pfIsEnabledForceES5ArrayFlag)();
int32_t (*pfGetForceES5ArrayFlag)(bool *flag);
int32_t (*pfSetForceES5ArrayFlag)(bool flag);
bool (*pfIsEnabledForceAsmJsLinkFailFlag)();
int32_t (*pfGetForceAsmJsLinkFailFlag)(bool *flag);
int32_t (*pfSetForceAsmJsLinkFailFlag)(bool flag);
bool (*pfIsEnabledForceExpireOnNonCacheCollectFlag)();
int32_t (*pfGetForceExpireOnNonCacheCollectFlag)(bool *flag);
int32_t (*pfSetForceExpireOnNonCacheCollectFlag)(bool flag);
bool (*pfIsEnabledForceFastPathFlag)();
int32_t (*pfGetForceFastPathFlag)(bool *flag);
int32_t (*pfSetForceFastPathFlag)(bool flag);
bool (*pfIsEnabledForceFloatPrefFlag)();
int32_t (*pfGetForceFloatPrefFlag)(bool *flag);
int32_t (*pfSetForceFloatPrefFlag)(bool flag);
bool (*pfIsEnabledForceJITLoopBodyFlag)();
int32_t (*pfGetForceJITLoopBodyFlag)(bool *flag);
int32_t (*pfSetForceJITLoopBodyFlag)(bool flag);
bool (*pfIsEnabledForceStaticInterpreterThunkFlag)();
int32_t (*pfGetForceStaticInterpreterThunkFlag)(bool *flag);
int32_t (*pfSetForceStaticInterpreterThunkFlag)(bool flag);
bool (*pfIsEnabledDumpCommentsFromReferencedFilesFlag)();
int32_t (*pfGetDumpCommentsFromReferencedFilesFlag)(bool *flag);
int32_t (*pfSetDumpCommentsFromReferencedFilesFlag)(bool flag);
FLAG_Number(DelayFullJITSmallFunc)
bool (*pfIsEnabledEnableFatalErrorOnOOMFlag)();
int32_t (*pfGetEnableFatalErrorOnOOMFlag)(bool *flag);
int32_t (*pfSetEnableFatalErrorOnOOMFlag)(bool flag);

#if defined(_M_ARM32_OR_ARM64)
bool (*pfIsEnabledForceLocalsPtrFlag)();
int32_t (*pfGetForceLocalsPtrFlag)(bool *flag);
int32_t (*pfSetForceLocalsPtrFlag)(bool flag);
#endif
bool (*pfIsEnabledDeferLoadingAvailableSourceFlag)();
int32_t (*pfGetDeferLoadingAvailableSourceFlag)(bool *flag);
int32_t (*pfSetDeferLoadingAvailableSourceFlag)(bool flag);
bool (*pfIsEnabledForceNativeFlag)();
int32_t (*pfGetForceNativeFlag)(bool *flag);
int32_t (*pfSetForceNativeFlag)(bool flag);
bool (*pfIsEnabledForceSerializedFlag)();
int32_t (*pfGetForceSerializedFlag)(bool *flag);
int32_t (*pfSetForceSerializedFlag)(bool flag);
FLAG_Number(ForceSerializedBytecodeMajorVersion)
FLAG_Number(ForceSerializedBytecodeVersionSchema)
bool (*pfIsEnabledForceStrictModeFlag)();
int32_t (*pfGetForceStrictModeFlag)(bool *flag);
int32_t (*pfSetForceStrictModeFlag)(bool flag);
bool (*pfIsEnabledForceUndoDeferFlag)();
int32_t (*pfGetForceUndoDeferFlag)(bool *flag);
int32_t (*pfSetForceUndoDeferFlag)(bool flag);
bool (*pfIsEnabledForceBlockingConcurrentCollectFlag)();
int32_t (*pfGetForceBlockingConcurrentCollectFlag)(bool *flag);
int32_t (*pfSetForceBlockingConcurrentCollectFlag)(bool flag);
bool (*pfIsEnabledFreTestDiagModeFlag)();
int32_t (*pfGetFreTestDiagModeFlag)(bool *flag);
int32_t (*pfSetFreTestDiagModeFlag)(bool flag);
#ifdef BYTECODE_TESTING
FLAG_Number(ByteCodeBranchLimit)
bool (*pfIsEnabledMediumByteCodeLayoutFlag)();
int32_t (*pfGetMediumByteCodeLayoutFlag)(bool *flag);
int32_t (*pfSetMediumByteCodeLayoutFlag)(bool flag);
bool (*pfIsEnabledLargeByteCodeLayoutFlag)();
int32_t (*pfGetLargeByteCodeLayoutFlag)(bool *flag);
int32_t (*pfSetLargeByteCodeLayoutFlag)(bool flag);
#endif
FLAG_Number(InduceCodeGenFailure)
FLAG_Number(InduceCodeGenFailureSeed)
FLAG_Number(InjectPartiallyInitializedInterpreterFrameError)
FLAG_Number(InjectPartiallyInitializedInterpreterFrameErrorType)
bool (*pfIsEnabledGenerateByteCodeBufferReturnsCantGenerateFlag)();
int32_t (*pfGetGenerateByteCodeBufferReturnsCantGenerateFlag)(bool *flag);
int32_t (*pfSetGenerateByteCodeBufferReturnsCantGenerateFlag)(bool flag);
FLAG_Number(GoptCleanupThreshold)
FLAG_Number(AsmGoptCleanupThreshold)
bool (*pfIsEnabledHighPrecisionDateFlag)();
int32_t (*pfGetHighPrecisionDateFlag)(bool *flag);
int32_t (*pfSetHighPrecisionDateFlag)(bool flag);
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
bool (*pfIsEnabledPrimeRecyclerFlag)();
int32_t (*pfGetPrimeRecyclerFlag)(bool *flag);
int32_t (*pfSetPrimeRecyclerFlag)(bool flag);
bool (*pfIsEnabledTraceEngineRefcountFlag)();
int32_t (*pfGetTraceEngineRefcountFlag)(bool *flag);
int32_t (*pfSetTraceEngineRefcountFlag)(bool flag);
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
bool (*pfIsEnabledLeakStackTraceFlag)();
int32_t (*pfGetLeakStackTraceFlag)(bool *flag);
int32_t (*pfSetLeakStackTraceFlag)(bool flag);
bool (*pfIsEnabledForceMemoryLeakFlag)();
int32_t (*pfGetForceMemoryLeakFlag)(bool *flag);
int32_t (*pfSetForceMemoryLeakFlag)(bool flag);
#endif
bool (*pfIsEnabledDumpAfterFinalGCFlag)();
int32_t (*pfGetDumpAfterFinalGCFlag)(bool *flag);
int32_t (*pfSetDumpAfterFinalGCFlag)(bool flag);
bool (*pfIsEnabledForceOldDateAPIFlag)();
int32_t (*pfGetForceOldDateAPIFlag)(bool *flag);
int32_t (*pfSetForceOldDateAPIFlag)(bool flag);

FLAG_Number(JitLoopBodyHotLoopThreshold)
FLAG_Number(LoopBodySizeThresholdToDisableOpts)

FLAG_Number(MaxJitThreadCount)
bool (*pfIsEnabledForceMaxJitThreadCountFlag)();
int32_t (*pfGetForceMaxJitThreadCountFlag)(bool *flag);
int32_t (*pfSetForceMaxJitThreadCountFlag)(bool flag);

bool (*pfIsEnabledMitigateSpectreFlag)();
int32_t (*pfGetMitigateSpectreFlag)(bool *flag);
int32_t (*pfSetMitigateSpectreFlag)(bool flag);

bool (*pfIsEnabledAddMaskingBlocksFlag)();
int32_t (*pfGetAddMaskingBlocksFlag)(bool *flag);
int32_t (*pfSetAddMaskingBlocksFlag)(bool flag);

bool (*pfIsEnabledPoisonVarArrayLoadFlag)();
int32_t (*pfGetPoisonVarArrayLoadFlag)(bool *flag);
int32_t (*pfSetPoisonVarArrayLoadFlag)(bool flag);
bool (*pfIsEnabledPoisonIntArrayLoadFlag)();
int32_t (*pfGetPoisonIntArrayLoadFlag)(bool *flag);
int32_t (*pfSetPoisonIntArrayLoadFlag)(bool flag);
bool (*pfIsEnabledPoisonFloatArrayLoadFlag)();
int32_t (*pfGetPoisonFloatArrayLoadFlag)(bool *flag);
int32_t (*pfSetPoisonFloatArrayLoadFlag)(bool flag);
bool (*pfIsEnabledPoisonTypedArrayLoadFlag)();
int32_t (*pfGetPoisonTypedArrayLoadFlag)(bool *flag);
int32_t (*pfSetPoisonTypedArrayLoadFlag)(bool flag);
bool (*pfIsEnabledPoisonStringLoadFlag)();
int32_t (*pfGetPoisonStringLoadFlag)(bool *flag);
int32_t (*pfSetPoisonStringLoadFlag)(bool flag);
bool (*pfIsEnabledPoisonObjectsForLoadsFlag)();
int32_t (*pfGetPoisonObjectsForLoadsFlag)(bool *flag);
int32_t (*pfSetPoisonObjectsForLoadsFlag)(bool flag);

bool (*pfIsEnabledPoisonVarArrayStoreFlag)();
int32_t (*pfGetPoisonVarArrayStoreFlag)(bool *flag);
int32_t (*pfSetPoisonVarArrayStoreFlag)(bool flag);
bool (*pfIsEnabledPoisonIntArrayStoreFlag)();
int32_t (*pfGetPoisonIntArrayStoreFlag)(bool *flag);
int32_t (*pfSetPoisonIntArrayStoreFlag)(bool flag);
bool (*pfIsEnabledPoisonFloatArrayStoreFlag)();
int32_t (*pfGetPoisonFloatArrayStoreFlag)(bool *flag);
int32_t (*pfSetPoisonFloatArrayStoreFlag)(bool flag);
bool (*pfIsEnabledPoisonTypedArrayStoreFlag)();
int32_t (*pfGetPoisonTypedArrayStoreFlag)(bool *flag);
int32_t (*pfSetPoisonTypedArrayStoreFlag)(bool flag);
bool (*pfIsEnabledPoisonStringStoreFlag)();
int32_t (*pfGetPoisonStringStoreFlag)(bool *flag);
int32_t (*pfSetPoisonStringStoreFlag)(bool flag);
bool (*pfIsEnabledPoisonObjectsForStoresFlag)();
int32_t (*pfGetPoisonObjectsForStoresFlag)(bool *flag);
int32_t (*pfSetPoisonObjectsForStoresFlag)(bool flag);

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
bool (*pfIsEnabledEnforceExecutionModeLimitsFlag)();
int32_t (*pfGetEnforceExecutionModeLimitsFlag)(bool *flag);
int32_t (*pfSetEnforceExecutionModeLimitsFlag)(bool flag);
bool (*pfIsEnabledEemlFlag)();
int32_t (*pfGetEemlFlag)(bool *flag);
int32_t (*pfSetEemlFlag)(bool flag);

FLAG_Number(SimpleJitAfter)
FLAG_Number(Sja)
FLAG_Number(FullJitAfter)
FLAG_Number(Fja)

bool (*pfIsEnabledNewSimpleJitFlag)();
int32_t (*pfGetNewSimpleJitFlag)(bool *flag);
int32_t (*pfSetNewSimpleJitFlag)(bool flag);

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
bool (*pfIsEnabledNoDeferParseFlag)();
int32_t (*pfGetNoDeferParseFlag)(bool *flag);
int32_t (*pfSetNoDeferParseFlag)(bool flag);
bool (*pfIsEnabledNoLogoFlag)();
int32_t (*pfGetNoLogoFlag)(bool *flag);
int32_t (*pfSetNoLogoFlag)(bool flag);
bool (*pfIsEnabledOOPJITMissingOptsFlag)();
int32_t (*pfGetOOPJITMissingOptsFlag)(bool *flag);
int32_t (*pfSetOOPJITMissingOptsFlag)(bool flag);
bool (*pfIsEnabledCrashOnOOPJITFailureFlag)();
int32_t (*pfGetCrashOnOOPJITFailureFlag)(bool *flag);
int32_t (*pfSetCrashOnOOPJITFailureFlag)(bool flag);
bool (*pfIsEnabledOOPCFGRegistrationFlag)();
int32_t (*pfGetOOPCFGRegistrationFlag)(bool *flag);
int32_t (*pfSetOOPCFGRegistrationFlag)(bool flag);
bool (*pfIsEnabledForceJITCFGCheckFlag)();
int32_t (*pfGetForceJITCFGCheckFlag)(bool *flag);
int32_t (*pfSetForceJITCFGCheckFlag)(bool flag);
bool (*pfIsEnabledUseJITTrampolineFlag)();
int32_t (*pfGetUseJITTrampolineFlag)(bool *flag);
int32_t (*pfSetUseJITTrampolineFlag)(bool flag);
bool (*pfIsEnabledNoNativeFlag)();
int32_t (*pfGetNoNativeFlag)(bool *flag);
int32_t (*pfSetNoNativeFlag)(bool flag);
FLAG_Number(NopFrequency)
bool (*pfIsEnabledNoStrictModeFlag)();
int32_t (*pfGetNoStrictModeFlag)(bool *flag);
int32_t (*pfSetNoStrictModeFlag)(bool flag);
bool (*pfIsEnabledNormalizeStatsFlag)();
int32_t (*pfGetNormalizeStatsFlag)(bool *flag);
int32_t (*pfSetNormalizeStatsFlag)(bool flag);
bool (*pfIsEnabledOutputFileFlag)();
int32_t (*pfGetOutputFileFlag)(BSTR *flag);
int32_t (*pfSetOutputFileFlag)(BSTR flag);
bool (*pfIsEnabledOutputFileOpenModeFlag)();
int32_t (*pfGetOutputFileOpenModeFlag)(BSTR *flag);
int32_t (*pfSetOutputFileOpenModeFlag)(BSTR flag);
#ifdef ENABLE_TRACE
bool (*pfIsEnabledInMemoryTraceFlag)();
int32_t (*pfGetInMemoryTraceFlag)(bool *flag);
int32_t (*pfSetInMemoryTraceFlag)(bool flag);
FLAG_Number(InMemoryTraceBufferSize)
#ifdef STACK_BACK_TRACE
bool (*pfIsEnabledTraceWithStackFlag)();
int32_t (*pfGetTraceWithStackFlag)(bool *flag);
int32_t (*pfSetTraceWithStackFlag)(bool flag);
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
bool (*pfIsEnabledPrintRunTimeDataCollectionTraceFlag)();
int32_t (*pfGetPrintRunTimeDataCollectionTraceFlag)(bool *flag);
int32_t (*pfSetPrintRunTimeDataCollectionTraceFlag)(bool flag);
#ifdef ENABLE_PREJIT
bool (*pfIsEnabledPrejitFlag)();
int32_t (*pfGetPrejitFlag)(bool *flag);
int32_t (*pfSetPrejitFlag)(bool flag);
#endif
bool (*pfIsEnabledPrintSrcInDumpFlag)();
int32_t (*pfGetPrintSrcInDumpFlag)(bool *flag);
int32_t (*pfSetPrintSrcInDumpFlag)(bool flag);
#if PROFILE_DICTIONARY
FLAG_Number(ProfileDictionary)
#endif
#ifdef PROFILE_EXEC
FLAG_Number(ProfileThreshold)
#endif
#ifdef PROFILE_OBJECT_LITERALS
bool (*pfIsEnabledProfileObjectLiteralFlag)();
int32_t (*pfGetProfileObjectLiteralFlag)(bool *flag);
int32_t (*pfSetProfileObjectLiteralFlag)(bool flag);
#endif
#ifdef PROFILE_MEM
bool (*pfIsEnabledProfileMemoryFlag)();
int32_t (*pfGetProfileMemoryFlag)(BSTR *flag);
int32_t (*pfSetProfileMemoryFlag)(BSTR flag);
#endif
#ifdef PROFILE_STRINGS
bool (*pfIsEnabledProfileStringsFlag)();
int32_t (*pfGetProfileStringsFlag)(bool *flag);
int32_t (*pfSetProfileStringsFlag)(bool flag);
#endif
#ifdef PROFILE_TYPES
bool (*pfIsEnabledProfileTypesFlag)();
int32_t (*pfGetProfileTypesFlag)(bool *flag);
int32_t (*pfSetProfileTypesFlag)(bool flag);
#endif
#ifdef PROFILE_EVALMAP
bool (*pfIsEnabledProfileEvalMapFlag)();
int32_t (*pfGetProfileEvalMapFlag)(bool *flag);
int32_t (*pfSetProfileEvalMapFlag)(bool flag);
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
bool (*pfIsEnabledProfileBailOutRecordMemoryFlag)();
int32_t (*pfGetProfileBailOutRecordMemoryFlag)(bool *flag);
int32_t (*pfSetProfileBailOutRecordMemoryFlag)(bool flag);
#endif

#if DBG
bool (*pfIsEnabledValidateIntRangesFlag)();
int32_t (*pfGetValidateIntRangesFlag)(bool *flag);
int32_t (*pfSetValidateIntRangesFlag)(bool flag);
#endif
FLAG_Number(RejitMaxBailOutCount)
FLAG_Number(CallsToBailoutsRatioForRejit)
FLAG_Number(LoopIterationsToBailoutsRatioForRejit)
FLAG_Number(MinBailOutsBeforeRejit)
FLAG_Number(MinBailOutsBeforeRejitForLoops)
bool (*pfIsEnabledLibraryStackFrameFlag)();
int32_t (*pfGetLibraryStackFrameFlag)(bool *flag);
int32_t (*pfSetLibraryStackFrameFlag)(bool flag);
bool (*pfIsEnabledLibraryStackFrameDebuggerFlag)();
int32_t (*pfGetLibraryStackFrameDebuggerFlag)(bool *flag);
int32_t (*pfSetLibraryStackFrameDebuggerFlag)(bool flag);
#ifdef RECYCLER_STRESS
bool (*pfIsEnabledRecyclerStressFlag)();
int32_t (*pfGetRecyclerStressFlag)(bool *flag);
int32_t (*pfSetRecyclerStressFlag)(bool flag);
#if ENABLE_CONCURRENT_GC
bool (*pfIsEnabledRecyclerBackgroundStressFlag)();
int32_t (*pfGetRecyclerBackgroundStressFlag)(bool *flag);
int32_t (*pfSetRecyclerBackgroundStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerConcurrentStressFlag)();
int32_t (*pfGetRecyclerConcurrentStressFlag)(bool *flag);
int32_t (*pfSetRecyclerConcurrentStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerConcurrentRepeatStressFlag)();
int32_t (*pfGetRecyclerConcurrentRepeatStressFlag)(bool *flag);
int32_t (*pfSetRecyclerConcurrentRepeatStressFlag)(bool flag);
#endif
#if ENABLE_PARTIAL_GC
bool (*pfIsEnabledRecyclerPartialStressFlag)();
int32_t (*pfGetRecyclerPartialStressFlag)(bool *flag);
int32_t (*pfSetRecyclerPartialStressFlag)(bool flag);
#endif
bool (*pfIsEnabledRecyclerTrackStressFlag)();
int32_t (*pfGetRecyclerTrackStressFlag)(bool *flag);
int32_t (*pfSetRecyclerTrackStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerInduceFalsePositivesFlag)();
int32_t (*pfGetRecyclerInduceFalsePositivesFlag)(bool *flag);
int32_t (*pfSetRecyclerInduceFalsePositivesFlag)(bool flag);
#endif // RECYCLER_STRESS
bool (*pfIsEnabledRecyclerForceMarkInteriorFlag)();
int32_t (*pfGetRecyclerForceMarkInteriorFlag)(bool *flag);
int32_t (*pfSetRecyclerForceMarkInteriorFlag)(bool flag);
#if ENABLE_CONCURRENT_GC
FLAG_Number(RecyclerPriorityBoostTimeout)
FLAG_Number(RecyclerThreadCollectTimeout)
bool (*pfIsEnabledEnableConcurrentSweepAllocFlag)();
int32_t (*pfGetEnableConcurrentSweepAllocFlag)(bool *flag);
int32_t (*pfSetEnableConcurrentSweepAllocFlag)(bool flag);
bool (*pfIsEnabledecsaFlag)();
int32_t (*pfGetecsaFlag)(bool *flag);
int32_t (*pfSetecsaFlag)(bool flag);
#endif
#ifdef RECYCLER_PAGE_HEAP
FLAG_Number(PageHeap)
bool (*pfIsEnabledPageHeapAllocStackFlag)();
int32_t (*pfGetPageHeapAllocStackFlag)(bool *flag);
int32_t (*pfSetPageHeapAllocStackFlag)(bool flag);
bool (*pfIsEnabledPageHeapFreeStackFlag)();
int32_t (*pfGetPageHeapFreeStackFlag)(bool *flag);
int32_t (*pfSetPageHeapFreeStackFlag)(bool flag);
FLAG_Number(PageHeapBlockType)
bool (*pfIsEnabledPageHeapDecommitGuardPageFlag)();
int32_t (*pfGetPageHeapDecommitGuardPageFlag)(bool *flag);
int32_t (*pfSetPageHeapDecommitGuardPageFlag)(bool flag);
#endif
#ifdef RECYCLER_NO_PAGE_REUSE
bool (*pfIsEnabledRecyclerNoPageReuseFlag)();
int32_t (*pfGetRecyclerNoPageReuseFlag)(bool *flag);
int32_t (*pfSetRecyclerNoPageReuseFlag)(bool flag);
#endif
#ifdef RECYCLER_MEMORY_VERIFY
FLAG_Number(RecyclerVerifyPadSize)
#endif
bool (*pfIsEnabledRecyclerTestFlag)();
int32_t (*pfGetRecyclerTestFlag)(bool *flag);
int32_t (*pfSetRecyclerTestFlag)(bool flag);
bool (*pfIsEnabledRecyclerProtectPagesOnRescanFlag)();
int32_t (*pfGetRecyclerProtectPagesOnRescanFlag)(bool *flag);
int32_t (*pfSetRecyclerProtectPagesOnRescanFlag)(bool flag);
#ifdef RECYCLER_VERIFY_MARK
bool (*pfIsEnabledRecyclerVerifyMarkFlag)();
int32_t (*pfGetRecyclerVerifyMarkFlag)(bool *flag);
int32_t (*pfSetRecyclerVerifyMarkFlag)(bool flag);
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
bool (*pfIsEnabledSwallowExceptionsFlag)();
int32_t (*pfGetSwallowExceptionsFlag)(bool *flag);
int32_t (*pfSetSwallowExceptionsFlag)(bool flag);
#endif
bool (*pfIsEnabledPrintSystemExceptionFlag)();
int32_t (*pfGetPrintSystemExceptionFlag)(bool *flag);
int32_t (*pfSetPrintSystemExceptionFlag)(bool flag);
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
bool (*pfIsEnabledTraceWin8AllocationsFlag)();
int32_t (*pfGetTraceWin8AllocationsFlag)(bool *flag);
int32_t (*pfSetTraceWin8AllocationsFlag)(bool flag);
bool (*pfIsEnabledTraceWin8DeallocationsImmediateFlag)();
int32_t (*pfGetTraceWin8DeallocationsImmediateFlag)(bool *flag);
int32_t (*pfSetTraceWin8DeallocationsImmediateFlag)(bool flag);
bool (*pfIsEnabledPrintWin8StatsDetailedFlag)();
int32_t (*pfGetPrintWin8StatsDetailedFlag)(bool *flag);
int32_t (*pfSetPrintWin8StatsDetailedFlag)(bool flag);
bool (*pfIsEnabledTraceProtectPagesFlag)();
int32_t (*pfGetTraceProtectPagesFlag)(bool *flag);
int32_t (*pfSetTraceProtectPagesFlag)(bool flag);
#endif
bool (*pfIsEnabledTraceAsyncDebugCallsFlag)();
int32_t (*pfGetTraceAsyncDebugCallsFlag)(bool *flag);
int32_t (*pfSetTraceAsyncDebugCallsFlag)(bool flag);
#ifdef TRACK_DISPATCH
bool (*pfIsEnabledTrackDispatchFlag)();
int32_t (*pfGetTrackDispatchFlag)(bool *flag);
int32_t (*pfSetTrackDispatchFlag)(bool flag);
#endif
bool (*pfIsEnabledVerboseFlag)();
int32_t (*pfGetVerboseFlag)(bool *flag);
int32_t (*pfSetVerboseFlag)(bool flag);
bool (*pfIsEnabledUseFullNameFlag)();
int32_t (*pfGetUseFullNameFlag)(bool *flag);
int32_t (*pfSetUseFullNameFlag)(bool flag);
bool (*pfIsEnabledUtf8Flag)();
int32_t (*pfGetUtf8Flag)(bool *flag);
int32_t (*pfSetUtf8Flag)(bool flag);
FLAG_Number(Version)
bool (*pfIsEnabledWERExceptionSupportFlag)();
int32_t (*pfGetWERExceptionSupportFlag)(bool *flag);
int32_t (*pfSetWERExceptionSupportFlag)(bool flag);
bool (*pfIsEnabledExtendedErrorStackForTestHostFlag)();
int32_t (*pfGetExtendedErrorStackForTestHostFlag)(bool *flag);
int32_t (*pfSetExtendedErrorStackForTestHostFlag)(bool flag);
bool (*pfIsEnablederrorStackTraceFlag)();
int32_t (*pfGeterrorStackTraceFlag)(bool *flag);
int32_t (*pfSeterrorStackTraceFlag)(bool flag);
bool (*pfIsEnabledDoHeapEnumOnEngineShutdownFlag)();
int32_t (*pfGetDoHeapEnumOnEngineShutdownFlag)(bool *flag);
int32_t (*pfSetDoHeapEnumOnEngineShutdownFlag)(bool flag);
#ifdef HEAP_ENUMERATION_VALIDATION
bool (*pfIsEnabledValidateHeapEnumFlag)();
int32_t (*pfGetValidateHeapEnumFlag)(bool *flag);
int32_t (*pfSetValidateHeapEnumFlag)(bool flag);
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
bool (*pfIsEnabledRegexTracingFlag)();
int32_t (*pfGetRegexTracingFlag)(bool *flag);
int32_t (*pfSetRegexTracingFlag)(bool flag);
bool (*pfIsEnabledRegexProfileFlag)();
int32_t (*pfGetRegexProfileFlag)(bool *flag);
int32_t (*pfSetRegexProfileFlag)(bool flag);
bool (*pfIsEnabledRegexDebugFlag)();
int32_t (*pfGetRegexDebugFlag)(bool *flag);
int32_t (*pfSetRegexDebugFlag)(bool flag);
bool (*pfIsEnabledRegexDebugASTFlag)();
int32_t (*pfGetRegexDebugASTFlag)(bool *flag);
int32_t (*pfSetRegexDebugASTFlag)(bool flag);
bool (*pfIsEnabledRegexDebugAnnotatedASTFlag)();
int32_t (*pfGetRegexDebugAnnotatedASTFlag)(bool *flag);
int32_t (*pfSetRegexDebugAnnotatedASTFlag)(bool flag);
bool (*pfIsEnabledRegexBytecodeDebugFlag)();
int32_t (*pfGetRegexBytecodeDebugFlag)(bool *flag);
int32_t (*pfSetRegexBytecodeDebugFlag)(bool flag);
bool (*pfIsEnabledRegexOptimizeFlag)();
int32_t (*pfGetRegexOptimizeFlag)(bool *flag);
int32_t (*pfSetRegexOptimizeFlag)(bool flag);
FLAG_Number(DynamicRegexMruListSize)
#endif

bool (*pfIsEnabledOptimizeForManyInstancesFlag)();
int32_t (*pfGetOptimizeForManyInstancesFlag)(bool *flag);
int32_t (*pfSetOptimizeForManyInstancesFlag)(bool flag);
bool (*pfIsEnabledEnableArrayTypeMutationFlag)();
int32_t (*pfGetEnableArrayTypeMutationFlag)(bool *flag);
int32_t (*pfSetEnableArrayTypeMutationFlag)(bool flag);
FLAG_Number(ArrayMutationTestSeed)
bool (*pfIsEnabledEnableEvalMapCleanupFlag)();
int32_t (*pfGetEnableEvalMapCleanupFlag)(bool *flag);
int32_t (*pfSetEnableEvalMapCleanupFlag)(bool flag);
#ifdef PROFILE_MEM
bool (*pfIsEnabledTraceObjectAllocationFlag)();
int32_t (*pfGetTraceObjectAllocationFlag)(bool *flag);
int32_t (*pfSetTraceObjectAllocationFlag)(bool flag);
#endif
FLAG_Number(Sse)
FLAG_Number(DeletedPropertyReuseThreshold)
bool (*pfIsEnabledForceStringKeyedSimpleDictionaryTypeHandlerFlag)();
int32_t (*pfGetForceStringKeyedSimpleDictionaryTypeHandlerFlag)(bool *flag);
int32_t (*pfSetForceStringKeyedSimpleDictionaryTypeHandlerFlag)(bool flag);
FLAG_Number(BigDictionaryTypeHandlerThreshold)
bool (*pfIsEnabledTypeSnapshotEnumerationFlag)();
int32_t (*pfGetTypeSnapshotEnumerationFlag)(bool *flag);
int32_t (*pfSetTypeSnapshotEnumerationFlag)(bool flag);
bool (*pfIsEnabledIsolatePrototypesFlag)();
int32_t (*pfGetIsolatePrototypesFlag)(bool *flag);
int32_t (*pfSetIsolatePrototypesFlag)(bool flag);
bool (*pfIsEnabledChangeTypeOnProtoFlag)();
int32_t (*pfGetChangeTypeOnProtoFlag)(bool *flag);
int32_t (*pfSetChangeTypeOnProtoFlag)(bool flag);
bool (*pfIsEnabledShareInlineCachesFlag)();
int32_t (*pfGetShareInlineCachesFlag)(bool *flag);
int32_t (*pfSetShareInlineCachesFlag)(bool flag);
bool (*pfIsEnabledDisableDebugObjectFlag)();
int32_t (*pfGetDisableDebugObjectFlag)(bool *flag);
int32_t (*pfSetDisableDebugObjectFlag)(bool flag);
bool (*pfIsEnabledDumpHeapFlag)();
int32_t (*pfGetDumpHeapFlag)(bool *flag);
int32_t (*pfSetDumpHeapFlag)(bool flag);
bool (*pfIsEnabledautoProxyFlag)();
int32_t (*pfGetautoProxyFlag)(BSTR *flag);
int32_t (*pfSetautoProxyFlag)(BSTR flag);
FLAG_Number(PerfHintLevel)
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
bool (*pfIsEnabledMemProtectHeapFlag)();
int32_t (*pfGetMemProtectHeapFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapFlag)(bool flag);
#endif
#ifdef RECYCLER_STRESS
bool (*pfIsEnabledMemProtectHeapStressFlag)();
int32_t (*pfGetMemProtectHeapStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapStressFlag)(bool flag);
#if ENABLE_CONCURRENT_GC
bool (*pfIsEnabledMemProtectHeapBackgroundStressFlag)();
int32_t (*pfGetMemProtectHeapBackgroundStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapBackgroundStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapConcurrentStressFlag)();
int32_t (*pfGetMemProtectHeapConcurrentStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapConcurrentStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapConcurrentRepeatStressFlag)();
int32_t (*pfGetMemProtectHeapConcurrentRepeatStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapConcurrentRepeatStressFlag)(bool flag);
#endif
#if ENABLE_PARTIAL_GC
bool (*pfIsEnabledMemProtectHeapPartialStressFlag)();
int32_t (*pfGetMemProtectHeapPartialStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapPartialStressFlag)(bool flag);
#endif
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
bool (*pfIsEnabledFixPropsOnPathTypesFlag)();
int32_t (*pfGetFixPropsOnPathTypesFlag)(bool *flag);
int32_t (*pfSetFixPropsOnPathTypesFlag)(bool flag);
#endif

// recycler heuristic flags
FLAG_Number(MaxBackgroundFinishMarkCount)
FLAG_Number(BackgroundFinishMarkWaitTime)
FLAG_Number(MinBackgroundRepeatMarkRescanBytes)

#if defined(_M_X64)
bool (*pfIsEnabledZeroMemoryWithNonTemporalStoreFlag)();
int32_t (*pfGetZeroMemoryWithNonTemporalStoreFlag)(bool *flag);
int32_t (*pfSetZeroMemoryWithNonTemporalStoreFlag)(bool flag);
#endif

// recycler memory restrict test flags
FLAG_Number(MaxMarkStackPageCount)
FLAG_Number(MaxTrackedObjectListCount)

// make the recycler page integration path easier to hit
FLAG_Number(NumberAllocPlusSize)

#if DBG
bool (*pfIsEnabledInitializeInterpreterSlotsWithInvalidStackVarFlag)();
int32_t (*pfGetInitializeInterpreterSlotsWithInvalidStackVarFlag)(bool *flag);
int32_t (*pfSetInitializeInterpreterSlotsWithInvalidStackVarFlag)(bool flag);
#endif

#if DBG
FLAG_Number(PRNGSeed0)
FLAG_Number(PRNGSeed1)
#endif

bool (*pfIsEnabledClearInlineCachesOnCollectFlag)();
int32_t (*pfGetClearInlineCachesOnCollectFlag)(bool *flag);
int32_t (*pfSetClearInlineCachesOnCollectFlag)(bool flag);
FLAG_Number(InlineCacheInvalidationListCompactionThreshold)
FLAG_Number(ConstructorCacheInvalidationThreshold)

#ifdef IR_VIEWER
bool (*pfIsEnabledIRViewerFlag)();
int32_t (*pfGetIRViewerFlag)(bool *flag);
int32_t (*pfSetIRViewerFlag)(bool flag);
#endif /* IR_VIEWER */

FLAG_Number(GCMemoryThreshold)

#if DBG
    FLAG_Number(SimulatePolyCacheWithOneTypeForInlineCacheIndex)
#endif

FLAG_Number(JITServerIdleTimeout)
FLAG_Number(JITServerMaxInactivePageAllocatorCount)

bool (*pfIsEnabledStrictWriteBarrierCheckFlag)();
int32_t (*pfGetStrictWriteBarrierCheckFlag)(bool *flag);
int32_t (*pfSetStrictWriteBarrierCheckFlag)(bool flag);
bool (*pfIsEnabledWriteBarrierTestFlag)();
int32_t (*pfGetWriteBarrierTestFlag)(bool *flag);
int32_t (*pfSetWriteBarrierTestFlag)(bool flag);
bool (*pfIsEnabledForceSoftwareWriteBarrierFlag)();
int32_t (*pfGetForceSoftwareWriteBarrierFlag)(bool *flag);
int32_t (*pfSetForceSoftwareWriteBarrierFlag)(bool flag);
bool (*pfIsEnabledVerifyBarrierBitFlag)();
int32_t (*pfGetVerifyBarrierBitFlag)(bool *flag);
int32_t (*pfSetVerifyBarrierBitFlag)(bool flag);
bool (*pfIsEnabledEnableBGFreeZeroFlag)();
int32_t (*pfGetEnableBGFreeZeroFlag)(bool *flag);
int32_t (*pfSetEnableBGFreeZeroFlag)(bool flag);
bool (*pfIsEnabledKeepRecyclerTrackDataFlag)();
int32_t (*pfGetKeepRecyclerTrackDataFlag)(bool *flag);
int32_t (*pfSetKeepRecyclerTrackDataFlag)(bool flag);

FLAG_Number(MaxSingleAllocSizeInMB)

// TODO (hanhossain): ConfigFlagsList end
#undef FLAG_Number

    NotifyUnhandledExceptionPtr pfnNotifyUnhandledException;
};

int32_t OnChakraCoreLoaded();

