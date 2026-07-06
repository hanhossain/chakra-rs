//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "CommonCorePch.h"
#include "Memory/PageHeapBlockTypeFilter.h"
#include "Core/ConfigFlagsTable.h"

#include <chakracore-sys/src/str_helper.rs.h>
#include <rust/cxx.h>

#undef DebugBreak

// Initialization order
//  AB AutoSystemInfo
//  AD PerfCounter
//  AE PerfCounterSet
//  AM Output/Configuration
//  AN MemProtectHeap
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AR LeakReport
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAM")
namespace Js
{
    NumberSet::NumberSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberSet::Add(uint32_t x)
    {
        set.Item(x);
    }

    bool NumberSet::Contains(uint32_t x)
    {
        return set.Contains(x);
    }


    NumberPairSet::NumberPairSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberPairSet::Add(uint32_t x, uint32_t y)
    {
        set.Item(NumberPair(x, y));
    }

    bool NumberPairSet::Contains(uint32_t x, uint32_t y)
    {
        return set.Contains(NumberPair(x, y));
    }

    NumberTrioSet::NumberTrioSet() : set(&NoCheckHeapAllocator::Instance) {}

    void NumberTrioSet::Add(uint32_t x, uint32_t y, uint32_t z)
    {
        set.Item(NumberTrio(x, y, z));
    }

    bool NumberTrioSet::Contains(uint32_t x, uint32_t y, uint32_t z)
    {
        return set.Contains(NumberTrio(x, y, z));
    }

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class String
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    String::String()
    {
        this->pszValue = NULL;
    }

    String::String(__in_z_opt const char16_t* psz)
    {
        this->pszValue = NULL;
        Set(psz);
    }

    String::~String()
    {
        if(NULL != this->pszValue)
        {
            NoCheckHeapDeleteArray(std::u16string(this->pszValue).length() + 1, this->pszValue);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// String::Set
    ///
    /// Frees the existing string if any
    /// allocates a new buffer to copy the new string
    ///
    ///----------------------------------------------------------------------------

    void
    String::Set(__in_z_opt const char16_t* pszValue)
    {
        if(NULL != this->pszValue)
        {
            NoCheckHeapDeleteArray(std::u16string(this->pszValue).length() + 1, this->pszValue);
        }

        if(NULL != pszValue)
        {
            size_t size    = 1 + std::u16string(pszValue).length();
            this->pszValue  = NoCheckHeapNewArray(char16_t, size);
            wcscpy_s(this->pszValue, size, pszValue);
        }
        else
        {
            this->pszValue = NULL;
        }
    }

    template <>
    bool RangeUnitContains<SourceFunctionNode>(RangeUnit<SourceFunctionNode> unit, SourceFunctionNode n)
    {
        Assert(n.functionId != (uint32_t)-1);

        if ((n.sourceContextId >= unit.i.sourceContextId) &&
            (n.sourceContextId <= unit.j.sourceContextId)
            )
        {
            if ((n.sourceContextId == unit.j.sourceContextId && -2 == unit.j.functionId) ||  //#.#-#.* case
                (n.sourceContextId == unit.i.sourceContextId && -2 == unit.i.functionId)     //#.*-#.# case
                )
            {
                return true;
            }

            if ((n.sourceContextId == unit.j.sourceContextId && -1 == unit.j.functionId) || //#.#-#.+ case
                (n.sourceContextId == unit.i.sourceContextId && -1 == unit.i.functionId)     //#.+-#.# case
                )
            {
                return n.functionId != 0;
            }

            if ((n.sourceContextId == unit.i.sourceContextId && n.functionId < unit.i.functionId) || //excludes all values less than functionId LHS
                (n.sourceContextId == unit.j.sourceContextId && n.functionId > unit.j.functionId)) ////excludes all values greater than functionId RHS
            {
                return false;
            }

            return true;
        }

        return false;
    }

    template <>
    Js::RangeUnit<Js::SourceFunctionNode> GetFullRange()
    {
        RangeUnit<SourceFunctionNode> unit;
        unit.i.sourceContextId = 0;
        unit.j.sourceContextId = UINT_MAX;
        unit.i.functionId = 0;
        unit.j.functionId = (uint)-3;
        return unit;
    }

    template <>
    SourceFunctionNode GetPrevious(SourceFunctionNode unit)
    {
        SourceFunctionNode prevUnit = unit;
        prevUnit.functionId--;
        if (prevUnit.functionId == UINT_MAX)
        {
            prevUnit.sourceContextId--;
        }
        return prevUnit;
    }

    template <>
    SourceFunctionNode GetNext(SourceFunctionNode unit)
    {
        SourceFunctionNode nextUnit = unit;
        nextUnit.functionId++;
        if (nextUnit.functionId == 0)
        {
            nextUnit.sourceContextId++;
        }
        return nextUnit;
    }

    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class Phases
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------

    bool
    Phases::IsEnabled(Phase phase)
    {
        return this->phaseList[(int)phase].valid;
    }

    bool
    Phases::IsEnabled(Phase phase, uint sourceContextId, Js::LocalFunctionId functionId)
    {
        return  this->phaseList[(int)phase].valid &&
                this->phaseList[(int)phase].range.InRange(SourceFunctionNode(sourceContextId, functionId));
    }

    bool
    Phases::IsEnabledForAll(Phase phase)
    {
        return  this->phaseList[(int)phase].valid &&
                this->phaseList[(int)phase].range.ContainsAll();
    }

    Range *
    Phases::GetRange(Phase phase)
    {
        return &this->phaseList[(int)phase].range;
    }

    void
    Phases::Enable(Phase phase)
    {
        this->phaseList[(int)phase].valid = true;
    }

    void
    Phases::Disable(Phase phase)
    {
        this->phaseList[(int)phase].valid = false;
        this->phaseList[(int)phase].range.Clear();
    }

    Phase
    Phases::GetFirstPhase()
    {
        int i= -1;
        while(!this->phaseList[++i].valid)
        {
            if(i >= PhaseCount - 1)
            {
                return InvalidPhase;
            }
        }
        return Phase(i);
    }


    //
    // List of names of all the flags
    //

    const char16_t* const FlagNames[FlagCount + 1] =
    {
    // TODO (hanhossain): ConfigFlagsList start
// TODO (hanhossain): remove default values start
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

#define DEFAULT_CONFIG_PageHeap             ((Js::Number) PageHeapMode::PageHeapModeOff)
#define DEFAULT_CONFIG_PageHeapAllocStack   (false)
#define DEFAULT_CONFIG_PageHeapFreeStack    (false)
#define DEFAULT_CONFIG_PageHeapBlockType    ((Js::Number) PageHeapBlockTypeFilter::PageHeapBlockTypeFilterAll)

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

#define DEFAULT_CONFIG_ForceSoftwareWriteBarrier  (true)
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

// TODO (hanhossain): remove default values end

#if DBG
        u"ArrayValidate",
        u"MemOpMissingValueValidate",
        u"OOPJITFixupValidate",
#endif
#ifdef ARENA_MEMORY_VERIFY
        u"ArenaNoFreeList",
        u"ArenaNoPageReuse",
        u"ArenaUseHeapAlloc",
#endif
        u"ValidateInlineStack",
        u"AsmDiff",
        u"AsmDumpMode",
        u"AsmJs",
        u"AsmJsStopOnError",
        u"AsmJsEdge",
        u"Wasm",
        u"WasmI64",
        u"WasmFastArray",
        u"WasmSharedArrayVirtualBuffer",
        u"WasmMathExFilter",
        u"WasmCheckVersion",
        u"WasmAssignModuleID",
        u"WasmIgnoreLimits",
        u"WasmFold",
        u"WasmIgnoreResponse",
        u"WasmMaxTableSize",
        u"WasmThreads",
        u"WasmMultiValue",
        u"WasmSignExtends",
        u"WasmNontrapping",

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
        u"WasmExperimental",

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        u"WasmSimd",

        u"AssertBreak",
        u"AssertPopUp",
        u"AssertIgnore",
        u"AsyncDebugging",
        u"BailOnNoProfileLimit",
        u"BailOnNoProfileRejitLimit",
        u"BaselineMode",
        u"DumpOnCrash",
        u"FullMemoryDump",
        u"BailOut",
        u"BailOutAtEveryLine",
        u"BailOutAtEveryByteCode",
        u"BailOutAtEveryImplicitCall",
        u"BailOutByteCode",
        u"Benchmark",
        u"BgJit",
        u"BgParse",
        u"BgJitDelay",
        u"BgJitDelayFgBuffer",
        u"BgJitPendingFuncCap",

        u"CreateFunctionProxy",
        u"HybridFgJit",
        u"HybridFgJitBgQueueLengthThreshold",
        u"BytecodeHist",
        u"CurrentSourceInfo",
        u"CFGLog",
        u"CheckAlignment",
        u"CheckEmitBufferPermissions",
#ifdef CHECK_MEMORY_LEAK
        u"CheckMemoryLeak",
        u"DumpOnLeak",
#endif
        u"CheckOpHelpers",
        u"CloneInlinedPolymorphicCaches",
        u"ConcurrentRuntime",
        u"ConstructorInlineThreshold",
        u"ConstructorCallsRequiredToFinalizeCachedType",
        u"PropertyCacheMissPenalty",
        u"PropertyCacheMissThreshold",
        u"PropertyCacheMissReset",
        u"Debug",
        u"DebugBreak",
        u"StatementDebugBreak",
        u"DebugBreakOnPhaseBegin",

        u"DebugWindow",
        u"ParserStateCache",
        u"CompressParserStateCache",
        u"DeferTopLevelTillFirstCall",
        u"DeferParse",
        u"DirectCallTelemetryStats",
        u"DisableArrayBTree",
        u"DisableRentalThreading",
        u"DisableVTuneSourceLineInfo",
        u"DisplayMemStats",
        u"Dump",
#ifdef DUMP_FRAGMENTATION_STATS
        u"DumpFragmentationStats",
#endif
        u"DumpIRAddresses",
        u"DumpLineNoInColor",
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
        u"DumpObjectGraphOnExit",
        u"DumpObjectGraphOnCollect",
#endif
        u"DumpEvalStringOnRemoval",
        u"DumpObjectGraphOnEnum",
#ifdef DYNAMIC_PROFILE_STORAGE
        u"DynamicProfileCache",
        u"Dpc",
        u"DynamicProfileCacheDir",
        u"DynamicProfileInput",
        u"Dpi",
#endif
#ifdef EDIT_AND_CONTINUE
        u"EditTest",
#endif
        u"WininetProfileCache",
        u"NoDynamicProfileInMemoryCache",
        u"ProfileBasedSpeculativeJit",
        u"ProfileBasedSpeculationCap",
        u"ExecuteByteCodeBufferReturnsInvalidByteCode",
        u"ExpirableCollectionGCCount",
        u"ExpirableCollectionTriggerThreshold",
        u"SkipSplitOnNoResult",
        u"Force32BitByteCode",

        u"CollectGarbage",

        u"Intl",
        u"IntlBuiltIns",
        u"IntlPlatform",

        u"JsBuiltIn",
        u"JitRepro",
        u"EntryPointInfoRpcData",

        u"LdChakraLib",
        u"TestChakraLib",

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
        u"ES6",

// Master ES6 flag to enable ALL sub ES6 features/flags
        u"ES6All",

// Master ES6 flag to enable Threshold ES6 features/flags
        u"ES6Experimental",

// Per ES6 feature/flag

        u"ES7AsyncAwait",
        u"ES6DateParseFix",
        u"ES6FunctionNameFull",
        u"ES6Generators",
        u"ES7ExponentiationOperator",

        u"ES7ValuesEntries",
        u"ES7TrailingComma",
        u"ES6IsConcatSpreadable",
        u"ES6Math",

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
        u"ESDynamicImport",

        u"ES6Module",
        u"ES6Object",
        u"ES6Number",
        u"ES6ObjectLiterals",
        u"ES6Proxy",
        u"ES6Rest",
        u"ES6Spread",
        u"ES6String",
        u"ES6StringPrototypeFixes",
        u"ES2018ObjectRestSpread",

        u"ES6PrototypeChain",
        u"ES6ToPrimitive",
        u"ES6ToLength",
        u"ES6ToStringTag",
        u"ES6Unicode",
        u"ES6UnicodeVerbose",
        u"ES6Unscopables",
        u"ES6RegExSticky",
        u"ES2018RegExDotAll",
        u"ESExportNsAs",
        u"ES2018AsyncIteration",
        u"ESTopLevelAwait",

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
        u"ES6RegExPrototypeProperties",

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        u"ES6RegExSymbols",

        u"ES6Verbose",
        u"ESObjectGetOwnPropertyDescriptors",

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

        u"ESSharedArrayBuffer",

// Newer language feature flags

// ES BigInt flag
        u"ESBigInt",

// ES Numeric Separator support for numeric constants
        u"ESNumericSeparator",

// ES Nullish coalescing operator support (??)
        u"ESNullishCoalescingOperator",

// ES Hashbang support for interpreter directive syntax
        u"ESHashbang",

// ES Symbol.prototype.description flag
        u"ESSymbolDescription",

        u"ESArrayFindFromLast",

// ES Promise.any and AggregateError flag
        u"ESPromiseAny",

// ES import.meta keyword meta-property
        u"ESImportMeta",

//ES globalThis flag
        u"ESGlobalThis",

// This flag to be removed once JITing generator functions is stable
        u"JitES6Generators",

        u"FastLineColumnCalculation",
        u"Filename",
        u"FreeRejittedCode",
        u"ForceGuardPages",
        u"PrintGuardPageBounds",
        u"ForceLegacyEngine",
        u"Force",
        u"Stress",
        u"ForceArrayBTree",
        u"StrongArraySort",
        u"ForceCleanPropertyOnCollect",
        u"ForceCleanCacheOnCollect",
        u"ForceGCAfterJSONParse",
        u"ForceDecommitOnCollect",
        u"ForceDeferParse",
        u"ForceDiagnosticsMode",
        u"ForceGetWriteWatchOOM",
        u"ForcePostLowerGlobOptInstrString",
        u"ForceSplitScope",
        u"EnumerateSpecialPropertiesInDebugger",
        u"EnableContinueAfterExceptionWrappersForHelpers",
        u"EnableContinueAfterExceptionWrappersForBuiltIns",
        u"EnableFunctionSourceReportForHeapEnum",
        u"ForceFragmentAddressSpace",
        u"ForceOOMOnEBCommit",
        u"ForceDynamicProfile",
        u"ForceES5Array",
        u"ForceAsmJsLinkFail",
        u"ForceExpireOnNonCacheCollect",
        u"ForceFastPath",
        u"ForceFloatPref",
        u"ForceJITLoopBody",
        u"ForceStaticInterpreterThunk",
        u"DumpCommentsFromReferencedFiles",
        u"DelayFullJITSmallFunc",
        u"EnableFatalErrorOnOOM",

#if defined(_M_ARM32_OR_ARM64)
        u"ForceLocalsPtr",
#endif
        u"DeferLoadingAvailableSource",
        u"ForceNative",
        u"ForceSerialized",
        u"ForceSerializedBytecodeMajorVersion",
        u"ForceSerializedBytecodeVersionSchema",
        u"ForceStrictMode",
        u"ForceUndoDefer",
        u"ForceBlockingConcurrentCollect",
        u"FreTestDiagMode",
#ifdef BYTECODE_TESTING
        u"ByteCodeBranchLimit",
        u"MediumByteCodeLayout",
        u"LargeByteCodeLayout",
#endif
        u"InduceCodeGenFailure",
        u"InduceCodeGenFailureSeed",
        u"InjectPartiallyInitializedInterpreterFrameError",
        u"InjectPartiallyInitializedInterpreterFrameErrorType",
        u"GenerateByteCodeBufferReturnsCantGenerate",
        u"GoptCleanupThreshold",
        u"AsmGoptCleanupThreshold",
        u"HighPrecisionDate",
        u"InlineCountMax",
        u"InlineCountMaxInLoopBodies",
        u"icminlb",
        u"InlineInLoopBodyScaleDownFactor",
        u"iilbsdf",
        u"InlineThreshold",
        u"AggressiveInlineCountMax",
        u"AggressiveInlineThreshold",
        u"InlineThresholdAdjustCountInLargeFunction",
        u"InlineThresholdAdjustCountInMediumSizedFunction",
        u"InlineThresholdAdjustCountInSmallFunction",
        u"AsmJsInlineAdjust",
        u"Interpret",
        u"Instrument",
        u"JitQueueThreshold",
#ifdef LEAK_REPORT
        u"LeakReport",
#endif
        u"LoopInlineThreshold",
        u"LeafInlineThreshold",
        u"ConstantArgumentInlineThreshold",
        u"RecursiveInlineThreshold",
        u"RecursiveInlineDepthMax",
        u"RecursiveInlineDepthMin",
        u"RedeferralCap",
        u"Loop",
        u"LoopInterpretCount",
        u"lic",
        u"LoopProfileIterations",
        u"OutsideLoopInlineThreshold",
        u"MaxFuncInlineDepth",
        u"MaxNumberOfInlineesWithLoop",
#ifdef MEMSPECT_TRACKING
        u"Memspect",
#endif
        u"PolymorphicInlineThreshold",
        u"PrimeRecycler",
        u"TraceEngineRefcount",
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        u"LeakStackTrace",
        u"ForceMemoryLeak",
#endif
        u"DumpAfterFinalGC",
        u"ForceOldDateAPI",

        u"JitLoopBodyHotLoopThreshold",
        u"LoopBodySizeThresholdToDisableOpts",

        u"MaxJitThreadCount",
        u"ForceMaxJitThreadCount",

        u"MitigateSpectre",

        u"AddMaskingBlocks",

        u"PoisonVarArrayLoad",
        u"PoisonIntArrayLoad",
        u"PoisonFloatArrayLoad",
        u"PoisonTypedArrayLoad",
        u"PoisonStringLoad",
        u"PoisonObjectsForLoads",

        u"PoisonVarArrayStore",
        u"PoisonIntArrayStore",
        u"PoisonFloatArrayStore",
        u"PoisonTypedArrayStore",
        u"PoisonStringStore",
        u"PoisonObjectsForStores",

        u"MinInterpretCount",
        u"MinSimpleJitRunCount",
        u"MaxInterpretCount",
        u"Mic",
        u"MaxSimpleJitRunCount",
        u"Msjrc",
        u"MinMemOpCount",
        u"Mmoc",

#if ENABLE_COPYONACCESS_ARRAY
        u"MaxCopyOnAccessArrayLength",
        u"MinCopyOnAccessArrayLength",
        u"CopyOnAccessArraySegmentCacheSize",
#endif

        u"MinTemplatizedJitRunCount",
        u"MinAsmJsInterpreterRunCount",

        u"MinTemplatizedJitLoopRunCount",
        u"MaxTemplatizedJitRunCount",
        u"Mtjrc",
        u"MaxAsmJsInterpreterRunCount",
        u"Maic",

        u"AutoProfilingInterpreter0Limit",
        u"ProfilingInterpreter0Limit",
        u"AutoProfilingInterpreter1Limit",
        u"SimpleJitLimit",
        u"ProfilingInterpreter1Limit",

        u"ExecutionModeLimits",
        u"Eml",
        u"EnforceExecutionModeLimits",
        u"Eeml",

        u"SimpleJitAfter",
        u"Sja",
        u"FullJitAfter",
        u"Fja",

        u"NewSimpleJit",

        u"MaxLinearIntCaseCount",
        u"MaxSingleCharStrJumpTableSize",
        u"MaxSingleCharStrJumpTableRatio",
        u"MinSwitchJumpTableSize",
        u"MaxLinearStringCaseCount",
        u"MinDeferredFuncTokenCount",
#if DBG
        u"SkipFuncCountForBailOnNoProfile",
#endif
        u"MaxJITFunctionBytecodeByteLength",
        u"MaxJITFunctionBytecodeCount",
        u"MaxLoopsPerFunction",
        u"FuncObjectInlineCacheThreshold",
        u"NoDeferParse",
        u"NoLogo",
        u"OOPJITMissingOpts",
        u"CrashOnOOPJITFailure",
        u"OOPCFGRegistration",
        u"ForceJITCFGCheck",
        u"UseJITTrampoline",
        u"NoNative",
        u"NopFrequency",
        u"NoStrictMode",
        u"NormalizeStats",
        u"Off",
        u"OffProfiledByteCode",
        u"On",
        u"OutputFile",
        u"OutputFileOpenMode",
#ifdef ENABLE_TRACE
        u"InMemoryTrace",
        u"InMemoryTraceBufferSize",
#ifdef STACK_BACK_TRACE
        u"TraceWithStack",
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
        u"PrintRunTimeDataCollectionTrace",
#ifdef ENABLE_PREJIT
        u"Prejit",
#endif
        u"PrintSrcInDump",
#if PROFILE_DICTIONARY
        u"ProfileDictionary",
#endif
#ifdef PROFILE_EXEC
        u"Profile",
        u"ProfileThreshold",
#endif
#ifdef PROFILE_OBJECT_LITERALS
        u"ProfileObjectLiteral",
#endif
#ifdef PROFILE_MEM
        u"ProfileMemory",
#endif
#ifdef PROFILE_STRINGS
        u"ProfileStrings",
#endif
#ifdef PROFILE_TYPES
        u"ProfileTypes",
#endif
#ifdef PROFILE_EVALMAP
        u"ProfileEvalMap",
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
        u"ProfileBailOutRecordMemory",
#endif

#if DBG
        u"ValidateIntRanges",
#endif
        u"RejitMaxBailOutCount",
        u"CallsToBailoutsRatioForRejit",
        u"LoopIterationsToBailoutsRatioForRejit",
        u"MinBailOutsBeforeRejit",
        u"MinBailOutsBeforeRejitForLoops",
        u"LibraryStackFrame",
        u"LibraryStackFrameDebugger",
#ifdef RECYCLER_STRESS
        u"RecyclerStress",
        u"RecyclerBackgroundStress",
        u"RecyclerConcurrentStress",
        u"RecyclerConcurrentRepeatStress",
        u"RecyclerPartialStress",
        u"RecyclerTrackStress",
        u"RecyclerInduceFalsePositives",
#endif // RECYCLER_STRESS
        u"RecyclerForceMarkInterior",
        u"RecyclerPriorityBoostTimeout",
        u"RecyclerThreadCollectTimeout",
        u"EnableConcurrentSweepAlloc",
        u"ecsa",
        u"PageHeap",
        u"PageHeapAllocStack",
        u"PageHeapFreeStack",
        u"PageHeapBucketNumber",
        u"PageHeapBlockType",
        u"PageHeapDecommitGuardPage",
#ifdef RECYCLER_NO_PAGE_REUSE
        u"RecyclerNoPageReuse",
#endif
#ifdef RECYCLER_MEMORY_VERIFY
        u"RecyclerVerify",
        u"RecyclerVerifyPadSize",
#endif
        u"RecyclerTest",
        u"RecyclerProtectPagesOnRescan",
#ifdef RECYCLER_VERIFY_MARK
        u"RecyclerVerifyMark",
#endif
        u"LowMemoryCap",
        u"NewPagesCapDuringBGSweeping",
        u"AllocPolicyLimit",
        u"RuntimeDataOutputFile",
        u"SpeculationCap",
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        u"Stats",
#endif
#if EXCEPTION_RECOVERY
        u"SwallowExceptions",
#endif
        u"PrintSystemException",
        u"SwitchOptHolesThreshold",
        u"TempMin",
        u"TempMax",
        u"Trace",

#if defined(_M_X64)
        u"LoopAlignNopLimit",
#endif

#ifdef PROFILE_MEM
        u"TraceMemory",
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
        u"TraceMetaDataParsing",
        u"TraceWin8Allocations",
        u"TraceWin8DeallocationsImmediate",
        u"PrintWin8StatsDetailed",
        u"TraceProtectPages",
#endif
        u"TraceAsyncDebugCalls",
#ifdef TRACK_DISPATCH
        u"TrackDispatch",
#endif
        u"Verbose",
        u"UseFullName",
        u"Utf8",
        u"Version",
        u"WERExceptionSupport",
        u"ExtendedErrorStackForTestHost",
        u"errorStackTrace",
        u"DoHeapEnumOnEngineShutdown",
#ifdef HEAP_ENUMERATION_VALIDATION
        u"ValidateHeapEnum",
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
        u"RegexTracing",
        u"RegexProfile",
        u"RegexDebug",
        u"RegexDebugAST",
        u"RegexDebugAnnotatedAST",
        u"RegexBytecodeDebug",
        u"RegexOptimize",
        u"DynamicRegexMruListSize",
#endif

        u"OptimizeForManyInstances",
        u"EnableArrayTypeMutation",
        u"ArrayMutationTestSeed",
        u"TestTrace",
        u"EnableEvalMapCleanup",
#ifdef PROFILE_MEM
        u"TraceObjectAllocation",
#endif
        u"Sse",
        u"DeletedPropertyReuseThreshold",
        u"ForceStringKeyedSimpleDictionaryTypeHandler",
        u"BigDictionaryTypeHandlerThreshold",
        u"TypeSnapshotEnumeration",
        u"IsolatePrototypes",
        u"ChangeTypeOnProto",
        u"ShareInlineCaches",
        u"DisableDebugObject",
        u"DumpHeap",
        u"autoProxy",
        u"PerfHintLevel",
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        u"MemProtectHeap",
#endif
#ifdef RECYCLER_STRESS
        u"MemProtectHeapStress",
        u"MemProtectHeapBackgroundStress",
        u"MemProtectHeapConcurrentStress",
        u"MemProtectHeapConcurrentRepeatStress",
        u"MemProtectHeapPartialStress",
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        u"FixPropsOnPathTypes",
#endif
        u"BailoutTraceFilter",
        u"RejitTraceFilter",

// recycler heuristic flags
        u"MaxBackgroundFinishMarkCount",
        u"BackgroundFinishMarkWaitTime",
        u"MinBackgroundRepeatMarkRescanBytes",

#if defined(_M_X64)
        u"ZeroMemoryWithNonTemporalStore",
#endif

// recycler memory restrict test flags
        u"MaxMarkStackPageCount",
        u"MaxTrackedObjectListCount",

// make the recycler page integration path easier to hit
        u"NumberAllocPlusSize",

#if DBG
        u"InitializeInterpreterSlotsWithInvalidStackVar",
#endif

#if DBG
        u"PRNGSeed0",
        u"PRNGSeed1",
#endif

        u"ClearInlineCachesOnCollect",
        u"InlineCacheInvalidationListCompactionThreshold",
        u"ConstructorCacheInvalidationThreshold",

#ifdef IR_VIEWER
        u"IRViewer",
#endif /* IR_VIEWER */

        u"GCMemoryThreshold",

#if DBG
            u"SimulatePolyCacheWithOneTypeForInlineCacheIndex",
#endif

        u"JITServerIdleTimeout",
        u"JITServerMaxInactivePageAllocatorCount",

        u"StrictWriteBarrierCheck",
        u"WriteBarrierTest",
        u"ForceSoftwareWriteBarrier",
        u"VerifyBarrierBit",
        u"EnableBGFreeZero",
        u"KeepRecyclerTrackData",

        u"MaxSingleAllocSizeInMB",

// TODO (hanhossain): ConfigFlagsList end
        NULL
    };


    //
    // List of names of all the Phases
    //

    const char16_t* const PhaseNames[PhaseCount + 1] =
    {
    u"All",
        u"BGJit",
        u"Module",
        u"LibInit",
            u"JsLibInit",
        u"Parse",
            u"RegexCompile",
            u"DeferParse",
            u"Redeferral",
            u"DeferEventHandlers",
            u"FunctionSourceInfoParse",
            u"StringTemplateParse",
            u"CreateParserState",
            u"SkipNestedDeferred",
            u"CacheScopeInfoNames",
            u"ScanAhead",
            u"ParallelParse",
            u"EarlyReferenceErrors",
            u"EarlyErrorOnAssignToCall",
            u"BgParse",
        u"ByteCode",
            u"CachedScope",
            u"StackFunc",
            u"StackClosure",
            u"DisableStackFuncOnDeferredEscape",
            u"DelayCapture",
            u"DebuggerScope",
            u"ByteCodeSerialization",
                u"VariableIntEncoding",
            u"NativeCodeSerialization",
            u"OptimizeBlockScope",
        u"Delay",
            u"Speculation",
            u"GatherCodeGenData",
        u"Wasm",
            // Wasm frontend
            u"WasmBytecode", // Supports -off,-dump,-trace,-profile
            u"WasmReader", // Support -trace,-profile
            u"WasmSection", // Supports -trace
            u"WasmOpCodeDistribution", // Support -dump
            // Wasm features per functions
            u"WasmDeferred",
            u"WasmValidatePrejit",
            u"WasmInOut", // Trace input and output of wasm calls
            u"WasmMemWrites", // Trace memory writes
        u"Asmjs",
            u"AsmjsTmpRegisterAllocation",
            u"AsmjsEncoder",
            u"AsmjsInterpreter",
            u"AsmJsJITTemplate",
            u"AsmjsFunctionEntry",
            u"AsmjsInterpreterStack",
            u"AsmjsEntryPointInfo",
            u"AsmjsCallDebugBreak",
        u"BackEnd",
            u"IRBuilder",
                u"SwitchOpt",
                u"BailOnNoProfile",
                u"BackendConcatExprOpt",
                u"ClosureRangeCheck",
                u"ClosureRegCheck",
            u"Inline",
                u"InlineRecursive",
                u"InlineAtEveryCaller",      //Inlines a function, say, foo at every caller of foo. Doesn't guarantee all the calls within foo are inlined too.
                u"InlineTree",               //Inlines every function within a top function, say, foo (which needs to be top function) Note: -force:inline achieves the effect of both -force:InlineTree & -force:InlineAtEveryCaller
                u"TryAggressiveInlining",
                u"InlineConstructors",
                u"InlineBuiltIn",
                u"InlineInJitLoopBody",
                u"InlineAccessors",
                u"InlineGetters",
                u"InlineSetters",
                u"InlineApply",
                u"InlineApplyTarget",
                u"InlineApplyWithoutArrayArg",
                u"InlineAnyCallApplyTarget",
                u"BailOutOnNotStackArgs",
                u"InlineCall",
                u"InlineCallTarget",
                u"PartialPolymorphicInline",
                u"PolymorphicInline",
                u"PolymorphicInlineFixedMethods",
                u"InlineOutsideLoops",
                u"InlineFunctionsWithLoops",
                u"EliminateArgoutForInlinee",
                u"InlineBuiltInCaller",
                u"InlineArgsOpt",
                    u"RemoveInlineFrame",
                u"InlinerConstFold",
                u"InlineCallbacks",
        u"ExecBOIFastPath",
            u"FGBuild",
                u"OptimizeTryFinally",
                u"RemoveBreakBlock",
                u"TailDup",
            u"FGPeeps",
            u"GlobOpt",
                u"PathDepBranchFolding",
                u"OptimizeTryCatch",
                u"CaptureByteCodeRegUse",
                u"Backward",
                    u"TrackIntUsage",
                    u"TrackNegativeZero",
                    u"TypedArrayVirtual",
                    u"TrackIntOverflow",
                    u"TrackCompoundedIntOverflow",
                u"Forward",
                    u"ValueTable",
                    u"ValueNumbering",
                    u"AVTInPrePass",
                    u"PathDependentValues",
                        u"TrackRelativeIntBounds",
                            u"BoundCheckElimination",
                                u"BoundCheckHoist",
                                    u"LoopCountBasedBoundCheckHoist",
                    u"CopyProp",
                        u"ObjPtrCopyProp",
                    u"ConstProp",
                    u"Int64ConstProp",
                    u"ConstFold",
                    u"CSE",
                    u"HoistConstInt",
                    u"TypeSpec",
                    u"AggressiveIntTypeSpec",
                    u"AggressiveMulIntTypeSpec",
                    u"LossyIntTypeSpec",
                    u"FloatTypeSpec",
                    u"StringTypeSpec",
                    u"InductionVars",
                    u"Invariants",
                    u"FieldCopyProp",
                    u"FieldPRE",
                    u"MakeObjSymLiveInLandingPad",
                    u"HostOpt",
                    u"ObjTypeSpec",
                        u"ObjTypeSpecNewObj",
                        u"ObjTypeSpecIsolatedFldOps",
                        u"ObjTypeSpecIsolatedFldOpsWithBailOut",
                        u"ObjTypeSpecStore",
                        u"EquivObjTypeSpec",
                        u"EquivObjTypeSpecByDefault",
                        u"TraceObjTypeSpecTypeGuards",
                        u"TraceObjTypeSpecWriteGuards",
                        u"LiveOutFields",
                        u"DisabledObjTypeSpec",
                        u"DepolymorphizeInlinees",
                        u"ReuseAuxSlotPtr",
                        u"PolyEquivTypeGuard",
                        u"DeadStoreTypeChecksOnStores",
                        #if DBG
                            u"SimulatePolyCacheWithOneTypeForFunction",
                        #endif
                    u"CheckThis",
                    u"StackArgOpt",
                    u"StackArgFormalsOpt",
                    u"StackArgLenConstOpt",
                    u"IndirCopyProp",
                    u"ArrayCheckHoist",
                        u"ArrayMissingValueCheckHoist",
                        u"ArraySegmentHoist",
                            u"JsArraySegmentHoist",
                        u"ArrayLengthHoist",
                        u"EliminateArrayAccessHelperCall",
                    u"NativeArray",
                        u"NativeNewScArray",
                        u"NativeArrayConversion",
                        u"CopyOnAccessArray",
                        u"NativeArrayLeafSegment",
                    u"TypedArrayTypeSpec",
                    u"LdLenIntSpec",
                    u"FixDataProps",
                    u"FixMethodProps",
                    u"FixAccessorProps",
                    u"FixDataVarProps",
                    u"UseFixedDataProps",
                    u"UseFixedDataPropsInInliner",
                    u"LazyBailout",
                        u"LazyBailoutOnImplicitCalls",
                        u"LazyFixedDataBailout",
                        u"LazyFixedTypeBailout",
                    u"FixedMethods",
                        u"FEFixedMethods",
                        u"FixedFieldGuardCheck",
                        u"FixedNewObj",
                            u"JitAllocNewObj",
                        u"FixedCtorInlining",
                        u"FixedCtorCalls",
                        u"FixedScriptMethodInlining",
                        u"FixedScriptMethodCalls",
                        u"FixedBuiltInMethodInlining",
                        u"FixedBuiltInMethodCalls",
                        u"SplitNewScObject",
                    u"OptTagChecks",
                    u"MemOp",
                        u"MemSet",
                        u"MemCopy",
                    u"IncrementalBailout",
                u"DeadStore",
                    u"ReverseCopyProp",
                    u"MarkTemp",
                        u"MarkTempNumber",
                        u"MarkTempObject",
                        u"MarkTempNumberOnTempObject",
                    u"SpeculationPropagationAnalysis",
            u"DumpGlobOptInstr", // Print the Globopt instr string in post lower dumps
            u"Lowerer",
                u"FastPath",
                    u"LoopFastPath",
                    u"MathFastPath",
                    u"Atom",
                        u"MulStrengthReduction",
                        u"AgenPeeps",
                    u"BranchFastPath",
                    u"CallFastPath",
                    u"BitopsFastPath",
                    u"OtherFastPath",
                    u"ObjectFastPath",
                    u"ProfileBasedFldFastPath",
                    u"AddFldFastPath",
                    u"RootObjectFldFastPath",
                    u"ArrayLiteralFastPath",
                    u"ArrayCtorFastPath",
                    u"NewScopeSlotFastPath",
                    u"FrameDisplayFastPath",
                    u"HoistMarkTempInit",
                    u"HoistConstAddr",
                u"JitWriteBarrier",
                u"PreLowererPeeps",
                u"CFGInJit",
                u"TypedArray",
                u"TracePinnedTypes",
            u"InterruptProbe",
            u"EncodeConstants",
            u"RegAlloc",
                u"Liveness",
                    u"RegParams",
                u"LinearScan",
                    u"OpHelperRegOpt",
                    u"StackPack",
                    u"SecondChance",
                    u"RegionUseCount",
                    u"RegHoistLoads",
                    u"ClearRegLoopExit",
            u"Peeps",
            u"Layout",
            u"EHBailoutPatchUp",
            u"FinalLower",
            u"PrologEpilog",
            u"InsertNOPs",
            u"Encoder",
                u"Assembly",
                u"Emitter",
                u"DebugBreak",
    #if defined(_M_X64)
                u"BrShorten",
                    u"LoopAlign",
    #endif

    #if DBG_DUMP
        u"SWB",
    #endif
        u"Run",
            u"Interpreter",
            u"EvalCompile",
                u"FastIndirectEval",
            u"IdleDecommit",
            u"IdleCollect",
            u"Marshal",
            u"MemoryAllocation",
                u"PageHeap",
                u"LargeMemoryAllocation",
                u"PageAllocatorAlloc",
            u"Recycler",
                u"ThreadCollect",
                u"ExplicitFree",
                u"ExpirableCollect",
                u"GarbageCollect",
                u"ConcurrentCollect",
                    u"BackgroundResetMarks",
                    u"BackgroundFindRoots",
                    u"BackgroundRescan",
                    u"BackgroundRepeatMark",
                    u"BackgroundFinishMark",
                u"ConcurrentPartialCollect",
                u"ParallelMark",
                u"PartialCollect",
                    u"ResetMarks",
                    u"ResetWriteWatch",
                    u"FindRoot",
                        u"FindRootArena",
                        u"FindImplicitRoot",
                        u"FindRootExt",
                    u"ScanStack",
                    u"ConcurrentMark",
                    u"ConcurrentWait",
                    u"Rescan",
                    u"Mark",
                    u"Sweep",
                        u"SweepWeak",
                        u"SweepSmall",
                        u"SweepLarge",
                        u"SweepPartialReuse",
                    u"ConcurrentSweep",
                    u"Finalize",
                    u"Dispose",
                    u"FinishPartial",
            u"Host",
            u"BailOut",
            u"BailIn",
            u"GeneratorGlobOpt",
            u"RegexQc",
            u"RegexOptBT",
            u"InlineCache",
            u"PolymorphicInlineCache",
            u"MissingPropertyCache",
            u"PropertyCache", // Trace caching of property lookups using PropertyString and JavascriptSymbol
            u"CloneCacheInCollision",
            u"ConstructorCache",
            u"InlineCandidate",
            u"ScriptFunctionWithInlineCache",
            u"IsConcatSpreadableCache",
            u"Arena",
            u"ApplyUsage",
            u"ObjectHeaderInlining",
                u"ObjectHeaderInliningForConstructors",
                u"ObjectHeaderInliningForObjectLiterals",
                u"ObjectHeaderInliningForEmptyObjects",
            u"OptUnknownElementName",
            u"TypePropertyCache",
    #if DBG_DUMP
            u"InlineSlots",
    #endif
            u"DynamicProfile",
    #ifdef DYNAMIC_PROFILE_STORAGE
            u"DynamicProfileStorage",
    #endif
            u"JITLoopBody",
            u"JITLoopBodyInTryCatch",
            u"JITLoopBodyInTryFinally",
            u"ReJIT",
            u"ExecutionMode",
            u"SimpleJitDynamicProfile",
            u"SimpleJit",
            u"FullJit",
            u"FailNativeCodeInstall",
            u"PixelArray",
            u"Etw",
            u"Profiler",
            u"CustomHeap",
            u"XDataAllocator",
            u"PageAllocator",
            u"StringConcat",
    #if DBG_DUMP
            u"PRNG",
    #endif
            u"PreReservedHeapAlloc",
            u"CFG",
            u"ExceptionStackTrace",
            u"ExtendedExceptionInfoStackTrace",
            u"TypeHandlerTransition",
            u"Debugger",
                u"ENC",
            u"ConsoleScope",
            u"ScriptProfiler",
            u"JSON",
            u"Intl",
            u"RegexResultNotUsed",
            u"Error",
            u"PropertyRecord",
            u"TypePathDynamicSize",
            u"ObjectCopy",
            u"ShareTypesWithAttributes",
            u"ShareAccessorTypes",
            u"ShareFuncTypes",
            u"ShareCrossSiteFuncTypes",
            u"ConditionalCompilation",
            u"InterpreterProfile",
            u"InterpreterAutoProfile",
            u"ByteCodeConcatExprOpt",
            u"TraceInlineCacheInvalidation",
            u"TracePropertyGuards",
            u"PerfHint",
            u"TypeShareForChangePrototype",
            u"DeferSourceLoad",
            u"DataCache",
            u"ObjectMutationBreakpoint",
            u"NativeCodeData",
            u"XData",
        nullptr
    };


    //
    // Description of flags
    //
    const char16_t* const FlagDescriptions[FlagCount + 1] =
    {
#if DBG
        u"Validate each array for valid elements (default: false)",
        u"Validate Missing Value Tracking on memset/memcopy",
        u"Validate that all entries in fixup list are allocated as NativeCodeData and that all NativeCodeData gets fixed up",
#endif
#ifdef ARENA_MEMORY_VERIFY
        u"Do not free list in arena",
        u"Do not reuse page in arena",
        u"Arena use heap to allocate memory instead of page allocator",
#endif
        u"Does a stack walk on helper calls to validate inline stack is correctly restored",
        u"Dump the IR without memory locations and varying parameters.",
        u"Dump the final assembly to a file without memory locations and varying parameters\n\t\t\t\t\tThe 'filename' is the file where the assembly will be dumped. Dump to console if no file is specified",
        u"Enable Asmjs",
        u"Stop execution on any AsmJs validation errors",
        u"Enable asm.js features which may have backward incompatible changes or not validate on old demos",
        u"Enable WebAssembly",
        u"Enable Int64 testing for WebAssembly. ArgIns can be [number,string,{low:number,high:number}]. Return values will be {low:number,high:number}",
        u"Enable fast array implementation for WebAssembly",
        u"Use Virtual allocation for WebAssemblySharedArrayBuffer (Windows only)",
        u"Enable Math exception filter for WebAssembly",
        u"Check the binary version for WebAssembly",
        u"Assign an individual ID for WebAssembly module",
        u"Ignore the WebAssembly binary limits ",
        u"Enable i32/i64 const folding",
        u"Ignore the type of the Response object",
        u"Maximum size allowed to the WebAssembly.Table",
        u"Enable WebAssembly threads feature",
        u"Use new WebAssembly multi-value",
        u"Use new WebAssembly sign extension operators",
        u"Enable non-trapping float-to-int conversions in WebAssembly",

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
        u"Enable WebAssembly experimental features",

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        u"Enable SIMD in WebAssembly",

        u"Debug break on assert",
        u"Pop up asserts (default: false)",
        u"Ignores asserts if set",
        u"Enable async debugging feature (default: false)",
        u"The limit of bailout on no profile info before triggering a rejit",
        u"The limit of bailout on no profile info before we disable the bailouts",
        u"Dump only stable content that can be used for baseline comparison",
        u"generate heap dump on asserts or unhandled exception if set",
        u"Will perform a full memory dump when -DumpOnCrash is supplied.",
        u"Source location to insert BailOut",
        u"Inserts BailOut at every line of source (default: false)",
        u"Inserts BailOut at every Byte code (default: false)",
        u"Force generating implicit call bailout even when we don't need it",
        u"Byte code location to insert BailOut. Use with -prejit only",
        u"Disable security code which introduce variability in benchmarks",
        u"Background JIT. Disable to force heuristic-based foreground JITting. (default: true)",
        u"Background Parse. Disable to force all parsing to occur on UI thread. (default: true)",
        u"Delay to wait for speculative jitting before starting script execution",
        u"When speculatively jitting in the foreground thread, do so for (BgJitDelay - BgJitDelayBuffer) milliseconds",
        u"Disable delay if pending function count larger then cap",

        u"Create function proxies instead of full function bodies",
        u"When background JIT is enabled, enable jitting in the foreground based on heuristics. This flag is only effective when OptimizeForManyInstances is disabled (UI threads).",
        u"The background job queue length must exceed this threshold to consider jitting in the foreground",
        u"Provide a histogram of the bytecodes run by the script. (NoNative required).",
        u"Enable IASD get current script source info",
        u"Log CFG checks",
        u"Insert checks in the native code to verify 8-byte alignment of stack",
        u"Check JIT code buffers at commit and decommit time to ensure no PAGE_EXECUTE_READWRITE pages.",
#ifdef CHECK_MEMORY_LEAK
        u"Check for heap memory leak",
        u"Create a dump on failed memory leak check",
#endif
        u"Verify opHelper labels in the JIT are set properly",
        u"Clones polymorphic inline caches in inlined functions",
        u"Enable Concurrent GC and background JIT when creating runtime",
        u"Maximum size in bytecodes of a constructor inline candidate with monomorphic field access",
        u"Number of calls to a constructor required before the type cached in the constructor cache is finalized",
        u"Number of string or symbol cache hits per miss needed to be worth using cache",
        u"Point at which we disable string or symbol property cache",
        u"Point at which we try to start using string or symbol cache after giving up",
        u"Disable phases (layout, security code, etc) which makes JIT output harder to debug",
        u"Index of the function where you want to break",
        u"Index of the statement where you want to break",
        u"Break into debugger at the beginning of given phase for listed function",

        u"Send console output to debugger window",
        u"Enable creation of parser state cache",
        u"Enable compression of the parser state cache",
        u"Enable tracking of deferred top level functions in a script file, until the first function of the script context is parsed.",
        u"Minimum size of defer-parsed script (non-zero only: use /nodeferparse do disable",
        u"Enables logging stats for direct call telemetry",
        u"Disable creation of BTree for Arrays",
        u"Disable rental threading when creating runtime",
        u"Disable VTune Source line info for Dynamic JITted code",
        u"Display memory usage statistics",
        u"What All to dump",
#ifdef DUMP_FRAGMENTATION_STATS
        u"Dump bucket state after every GC",
#endif
        u"Print addresses in IR dumps",
        u"Print the source code in high intensity color for better readability",
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
        u"Dump object graph on recycler destructor",
        u"Dump object graph on recycler destructor",
#endif
        u"Dumps an eval string when its being removed from the eval map",
        u"Dump object graph on recycler heap enumeration",
#ifdef DYNAMIC_PROFILE_STORAGE
        u"File to cache dynamic profile information",
        u"File to cache dynamic profile information",
        u"Directory to cache dynamic profile information",
        u"Read only file containing dynamic profile information",
        u"Read only file containing dynamic profile information",
#endif
#ifdef EDIT_AND_CONTINUE
        u"Enable edit and continue test tools",
#endif
        u"Use the WININET cache to save the profile information",
        u"Enable in-memory cache for dynamic sources",
        u"Enable dynamic profile based speculative JIT",
        u"In the presence of dynamic profile speculative JIT is capped to this many bytecode instructions",
        u"Serialized byte code execution always returns SCRIPT_E_INVALID_BYTECODE",
        u"Number of GCs during which Expirable object profiling occurs",
        u"Threshold at which Expirable Object Collection is triggered (In Percentage)",
        u"If the result of Regex split isn't used, skip executing the regex. (Perf optimization)",
        u"Force CC to generate 32bit bytecode intended only for regenerating bytecode headers.",

        u"Enable CollectGarbage API",

        u"Intl object support",
        u"Intl built-in function support",
        u"Make the internal Intl native helper object visible to user code",

        u"JS Built-in function support",
        u"Add Function.invokeJit to execute codegen on an encoded rpc buffer",
        u"Keep encoded rpc buffer for jitted function on EntryPointInfo until cleanup",

        u"Access to the Chakra internal library with the __chakraLibrary keyword",
        u"Access to the Chakra internal library with the __chakraLibrary keyword without global access restriction",

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
        u"Enable ES6 stable features",

// Master ES6 flag to enable ALL sub ES6 features/flags
        u"Enable all ES6 features, both stable and unstable",

// Master ES6 flag to enable Threshold ES6 features/flags
        u"Enable all experimental features",

// Per ES6 feature/flag

        u"Enable ES7 'async' and 'await' keywords",
        u"Enable ES6 Date.parse fixes",
        u"Enable ES6 Full function.name",
        u"Enable ES6 generators",
        u"Enable ES7 exponentiation operator (**)",

        u"Enable ES7 Object.values and Object.entries",
        u"Enable ES7 trailing comma in function",
        u"Enable ES6 isConcatSpreadable Symbol",
        u"Enable ES6 Math extensions",

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
        u"Enable dynamic import",

        u"Enable ES6 Modules",
        u"Enable ES6 Object extensions",
        u"Enable ES6 Number extensions",
        u"Enable ES6 Object literal extensions",
        u"Enable ES6 Proxy feature",
        u"Enable ES6 Rest parameters",
        u"Enable ES6 Spread support",
        u"Enable ES6 String extensions",
        u"Enable ES6 String.prototype fixes",
        u"Enable ES2018 Object Rest/Spread",

        u"Enable ES6 prototypes (Example: Date prototype is object)",
        u"Enable ES6 ToPrimitive symbol",
        u"Enable ES6 ToLength fixes",
        u"Enable ES6 ToStringTag symbol",
        u"Enable ES6 Unicode 6.0 extensions",
        u"Enable ES6 Unicode 6.0 verbose failure output",
        u"Enable ES6 With Statement Unscopables",
        u"Enable ES6 RegEx sticky flag",
        u"Enable ES2018 RegEx dotAll flag",
        u"Enable ES experimental export * as name",
        u"Enable ES2018 Async Iteration",
        u"Enable Top Level Await in modules",

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
        u"Enable ES6 properties on the RegEx prototype",

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        u"Enable ES6 RegExp symbols",

        u"Enable ES6 verbose trace",
        u"Enable Object.getOwnPropertyDescriptors",

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

        u"Enable SharedArrayBuffer",

// Newer language feature flags

// ES BigInt flag
        u"Enable ESBigInt flag",

// ES Numeric Separator support for numeric constants
        u"Enable Numeric Separator flag",

// ES Nullish coalescing operator support (??)
        u"Enable nullish coalescing operator",

// ES Hashbang support for interpreter directive syntax
        u"Enable Hashbang syntax",

// ES Symbol.prototype.description flag
        u"Enable Symbol.prototype.description",

        u"Enable findLast, findLastIndex for Array.prototype and TypedArray.prorotype",

// ES Promise.any and AggregateError flag
        u"Enable Promise.any and AggregateError",

// ES import.meta keyword meta-property
        u"Enable import.meta keyword",

//ES globalThis flag
        u"Enable globalThis",

// This flag to be removed once JITing generator functions is stable
        u"Enable JITing of ES6 generators",

        u"Enable fast calculation of line/column numbers from the source.",
        u"Jscript source file",
        u"Free rejitted code",
        u"Force the addition of guard pages",
        u"Prints the bounds of a guard page",
        u"Force a jscript9 dll load",
        u"Force certain phase to run ignoring heuristics",
        u"Stress certain phases by making them kick in even if they normally would not.",
        u"Force enable creation of BTree for Arrays",
        u"Add secondary comparisons to the default array sort comparator to disambiguate sorts of equal-toString'd objects.",
        u"Force cleaning of property on collection",
        u"Force cleaning of dynamic caches on collection",
        u"Force GC to happen after JSON parsing",
        u"Force decommit collect",
        u"Defer parsing of all function bodies",
        u"Enable diagnostics mode and debug interpreter loop",
        u"Force GetWriteWatch to go into OOM codepath in HeapBlockMap rescan",
        u"Force tracking of globopt instr string post lower",
        u"All functions will have unmerged body and param scopes",
        u"Enable enumeration of special debug properties",
        u"Enable wrapper over helper methods in debugger, Fast F12 only",
        u"Enable wrapper over library calls in debugger, Fast F12 only",
        u"During HeapEnum, whether to report function source info (url/row/col)",
        u"Fragment the address space",
        u"Force CommitBuffer to return OOM",
        u"Force to always generate profiling byte code",
        u"Force using ES5Array",
        u"Force asm.js link time validation to fail",
        u"Allow expiration collect outside of cache collection cleanups",
        u"Force fast-paths in native codegen",
        u"Force float preferencing (JIT only)",
        u"Force jit loop body only",
        u"Force using static interpreter thunk",
        u"Allow printing comments of comment-table of the referenced file as well (use with -trace:CommentTable)",
        u"Scale Full JIT threshold for small functions which are going to be inlined soon. To provide fraction scale, the final scale is scale following this option divided by 10",
        u"Enabling failfast fatal error on OOM",

#if defined(_M_ARM32_OR_ARM64)
        u"Force use of alternative locals pointer (JIT only)",
#endif
        u"Treat available source code as a dummy defer-mappable object to go through that code path.",
        u"Force JIT everything that is called before running it, ignoring limits",
        u"Always serialize and deserialize byte codes before execution",
        u"Force the byte code serializer to write this major version number",
        u"Force the byte code serializer to write this kind of version. Decimal 10 is engineering, 20 is release mode, and 0 means use the default setting.",
        u"Force strict mode checks on all functions",
        u"Defer parsing of all function bodies, but undo deferral",
        u"Force doing in-thread GC on concurrent thread- this will skip doing concurrent collect",
        u"Enabled collection of diagnostic information on fretest builds",
#ifdef BYTECODE_TESTING
        u"Short branch limit before we use the branch island",
        u"Always use medium layout for bytecodes",
        u"Always use large layout for bytecodes",
#endif
        u"Probability of a codegen job failing.",
        u"Seed used while calculating codegen failure probability",
        u"The number of interpreter stack frame (with 1 being bottom-most) to inject error before the frame is initialized.",
        u"Type of error to inject: 0 - debug break, 1 - exception.",
        u"Serialized byte code generation always returns SCRIPT_E_CANT_GENERATE",
        u"Number of instructions seen before we cleanup the value table",
        u"Number of instructions seen before we cleanup the value table",
        u"Enable sub-millisecond resolution in Javascript Date for benchmark timing",
        u"Maximum count in bytecodes to inline in a given function",
        u"Maximum count in bytecodes to inline in a given function",
        u"Maximum count in bytecodes to inline in a given function",
        u"Maximum depth of a recursive inline call",
        u"Maximum depth of a recursive inline call",
        u"Maximum size in bytecodes of an inline candidate",
        u"Maximum count in bytecodes to inline in a given function",
        u"Maximum size in bytecodes of an inline candidate for aggressive inlining",
        u"Adjustment in the maximum size in bytecodes of an inline candidate in a large function",
        u"Adjustment in the maximum size in bytecodes of an inline candidate in a medium sized function",
        u"Adjustment in the maximum size in bytecodes of an inline candidate in a small function",
        u"Adjustment in the maximum size in bytecodes of an inline candidate for wasm function",
        u"List of functions to interpret",
        u"Instrument the generated code from the given phase",
        u"Max number of work items/script context in the jit queue",
#ifdef LEAK_REPORT
        u"File name for the leak report",
#endif
        u"Maximum size in bytecodes of an inline candidate with loops or not enough profile data",
        u"Maximum size in bytecodes of an inline candidate with loops or not enough profile data",
        u"Maximum size in bytecodes of an inline candidate with constant argument and the argument being used for a branch",
        u"Maximum size in bytecodes of an inline candidate to inline recursively",
        u"Maximum depth of a recursive inline call",
        u"Maximum depth of a recursive inline call",
        u"Number of compilations beyond which we stop redeferring a function",
        u"Number of times to execute the script (useful for profiling short benchmarks and finding leaks)",
        u"Number of times loop has to be interpreted before JIT Loop body",
        u"Number of times loop has to be interpreted before JIT Loop body",
        u"Number of iterations of a loop that must be profiled before jitting the loop body",
        u"Maximum size in bytecodes of an inline candidate outside a loop in inliner",
        u"Number of times to allow inlining a function recursively, plus one (min: 1, max: 255)",
        u"Number of times to allow inlinees with a loop in a top function",
#ifdef MEMSPECT_TRACKING
        u"Enables memspect tracking to perform memory investigations.",
#endif
        u"Maximum size in bytecodes of a polymorphic inline candidate",
        u"Prime the recycler first",
        u"Output traces for ScriptEngine AddRef/Release to debug lifetime management",
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        u"Include stack trace on leaked pinned object and heap objects",
        u"Fake leak some memory to test leak report and check memory leak",
#endif
        u"Collect a process dump after calling finalgc",
        u"Force Chakra to use old dates API regardless of availability of a new one",

        u"Number of times loop has to be iterated in jitloopbody before it is determined as hot",
        u"Minimum bytecode size of a loop body, above which we might consider switching off optimizations in jit loop body to avoid rejits",

        u"Number of maximum allowed parallel jit threads (actual number is factor of number of processors and other heuristics)",
        u"Force the number of parallel jit threads as specified by MaxJitThreadCount flag (creation guaranteed)",

        u"Use mitigations for Spectre",

        u"Optimize Spectre mitigations by masking on loop out edges",

        u"Poison loads from Var arrays",
        u"Poison loads from Int arrays",
        u"Poison loads from Float arrays",
        u"Poison loads from TypedArrays",
        u"Poison indexed loads from strings",
        u"Poison objects after type checks for loads",

        u"Poison stores to Var arrays",
        u"Poison stores to Int arrays",
        u"Poison stores to Float arrays",
        u"Poison stores to TypedArrays",
        u"Poison indexed stores to strings",
        u"Poison objects after type checks for stores",

        u"Minimum number of times a function must be interpreted",
        u"Minimum number of times a function must be run in simple jit",
        u"Maximum number of times a function can be interpreted",
        u"Maximum number of times a function can be interpreted",
        u"Maximum number of times a function will be run in SimpleJitted code",
        u"Maximum number of times a function will be run in SimpleJitted code",
        u"Minimum count of a loop to activate MemOp",
        u"Minimum count of a loop to activate MemOp",

#if ENABLE_COPYONACCESS_ARRAY
        u"Maximum length of copy-on-access array",
        u"Minimum length of copy-on-access array",
        u"Size of copy-on-access array segment cache (1-32)",
#endif

        u"Minimum number of times a function must be Templatized Jitted",
        u"Minimum number of times a function must be Asm Interpreted",

        u"Minimum LoopCount run of the Templatized Jit function to run FullJited",
        u"Maximum number of times a function must be templatized jit",
        u"Maximum number of times a function must be templatized jit",
        u"Maximum number of times a function must be interpreted in asmjs",
        u"Maximum number of times a function must be interpreted in asmjs",

        u"Limit after which to transition to the next execution mode",
        u"Limit after which to transition to the next execution mode",
        u"Limit after which to transition to the next execution mode",
        u"Limit after which to transition to the next execution mode",
        u"Limit after which to transition to the next execution mode",

        u"",
        u"",
        u"Enforces the execution mode limits such that they are never exceeded.",
        u"Enforces the execution mode limits such that they are never exceeded.",

        u"Number of calls to a function after which to simple-JIT the function",
        u"Number of calls to a function after which to simple-JIT the function",
        u"Number of calls to a function after which to full-JIT the function. The function will be profiled for every iteration.",
        u"Number of calls to a function after which to full-JIT the function. The function will be profiled for every iteration.",

        u"Uses the new simple JIT",

        u"Maximum number of cases(in switch statement) for which instructions can be generated linearly",
        u"Maximum single char string jump table size",
        u"Maximum single char string jump table size as multiples of the actual case arm",
        u"Minimum size of the jump table, that is created for consecutive integer case arms in a Switch Statement",
        u"Maximum number of string cases(in switch statement) for which instructions can be generated linearly",
        u"Minimum length in tokens of defer-parsed function",
#if DBG
        u"Initial Number of functions in a func body to be skipped from forcibly inserting BailOnNoProfile.",
#endif
        u"The biggest function we'll JIT (bytecode bytelength)",
        u"The biggest function we'll JIT (bytecode count)",
        u"Maximum number of loops in any function in the script",
        u"Maximum number of inline caches a function body may have to allow for inline caches to be allocated on the function object",
        u"Disable deferred parsing",
        u"No logo, which we don't display anyways",
        u"Use optimizations that are missing from OOP JIT",
        u"Crash runtime process if JIT process crashes",
        u"Do CFG registration OOP (under OOP JIT)",
        u"Have JIT code always do CFG check even if range check succeeded",
        u"Use trampoline for JIT entry points and emit range checks for it",
        u"Disable native codegen",
        u"Frequency of NOPs inserted by NOP insertion phase.  A NOP is guaranteed to be inserted within a range of (1<<n) instrs (default=8)",
        u"Disable strict mode checks on all functions",
        u"When dumping stats, do some normalization (used with -instrument:linearscan)",
        u"Turn off specific phases or feature.(Might not work for all phases)",
        u"Turn off specific byte code for phases or feature.(Might not work for all phases)",
        u"Turn on specific phases or feature.(Might not work for all phases)",
        u"output.log",
        u"wt",
#ifdef ENABLE_TRACE
        u"Enable in-memory trace (investigate crash using trace in dump file). Use !jd.dumptrace to print it.",
        u"The size of circular buffer for in-memory trace (the units used is: number of trace calls). ",
#ifdef STACK_BACK_TRACE
        u"Whether the trace need to include stack trace (for each trace entry).",
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
        u"Print traces needed for runtime data collection",
#ifdef ENABLE_PREJIT
        u"Prejit everything, including things that are not called, ignoring limits (default: false)",
#endif
        u"Print the lineno and the source code in the intermediate dumps",
#if PROFILE_DICTIONARY
        u"Profile dictionary usage. Only dictionaries with max depth of <number> or above are displayed (0=no filter).",
#endif
#ifdef PROFILE_EXEC
        u"Profile the given phase",
        u"A phase is displayed in the profiler report only if its contribution is more than this threshold",
#endif
#ifdef PROFILE_OBJECT_LITERALS
        u"Profile Object literal usage",
#endif
#ifdef PROFILE_MEM
        u"Profile memory usage",
#endif
#ifdef PROFILE_STRINGS
        u"Profile string statistics",
#endif
#ifdef PROFILE_TYPES
        u"Profile type statistics",
#endif
#ifdef PROFILE_EVALMAP
        u"Profile eval map statistics",
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
        u"Profile bailout record memory statistics",
#endif

#if DBG
        u"Validate at runtime int ranges/bounds determined by the globopt",
#endif
        u"Maximum number of bailouts for a bailout record after which rejit is forced",
        u"Ratio of function calls to bailouts above which a rejit is considered",
        u"Ratio of loop iteration count to bailouts above which a rejit of the loop body is considered",
        u"Minimum number of bailouts for a single bailout record after which a rejit is considered",
        u"Minimum number of bailouts for a single bailout record after which a rejit is considered",
        u"Display library stack frame",
        u"Assume debugger support for library stack frame",
#ifdef RECYCLER_STRESS
        u"Stress the recycler by collect on every allocation call",
        u"Stress the recycler by collect in the background thread on every allocation call",
        u"Stress the concurrent recycler by concurrent collect on every allocation call",
        u"Stress the concurrent recycler by concurrent collect on every allocation call and repeat mark and rescan in the background thread",
        u"Stress the partial recycler by partial collect on every allocation call",
        u"Stress tracked object handling by simulating tracked objects for regular allocations",
        u"Stress recycler by forcing false positive object marks",
#endif // RECYCLER_STRESS
        u"Force all the mark as interior",
        u"Adjust priority boost timeout",
        u"Adjust thread collect timeout",
        u"Turns off the feature to allow allocations during concurrent sweep.",
        u"Turns off the feature to allow allocations during concurrent sweep.",
        u"Use full page for heap allocations",
        u"Capture alloc stack under page heap mode",
        u"Capture free stack under page heap mode",
        u"Bucket numbers to be used for page heap allocations",
        u"Type of blocks to use page heap for",
        u"Decommit page heap guard page",
#ifdef RECYCLER_NO_PAGE_REUSE
        u"Do not reuse page in recycler",
#endif
#ifdef RECYCLER_MEMORY_VERIFY
        u"Verify recycler memory",
        u"Padding size to verify recycler memory",
#endif
        u"Run recycler tests instead of executing script",
        u"Temporarily switch all pages to read only during rescan",
#ifdef RECYCLER_VERIFY_MARK
        u"verify concurrent gc",
#endif
        u"Memory cap indicating a low-memory process",
        u"New pages count allowed to be allocated during background sweeping",
        u"Memory allocation policy limit in MB (default: -1, which means no allocation policy limit).",
        u"Filename to write the dynamic profile info",
        u"How much bytecode we'll speculatively JIT",
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        u"Stats the given phase",
#endif
#if EXCEPTION_RECOVERY
        u"Force a try/catch around every statement",
#endif
        u"Always print a message when there's OOM or OOS",
        u"Maximum percentage of holes (missing case values in a switch statement) with which a jump table can be created",
        u"Temp number switch which code can temporarily use for debugging",
        u"Temp number switch which code can temporarily use for debugging",
        u"Trace the given phase",

#if defined(_M_X64)
        u"Max number of nops for loop alignment",
#endif

#ifdef PROFILE_MEM
        u"Trace memory usage",
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
        u"Trace metadata parsing for generating JS projections. [Levels 1-5, with 5 corresponding to most detailed]",
        u"Trace the win8 memory allocations",
        u"Trace the win8 memory deallocations immediately",
        u"Print the detailed memory trace report",
        u"Trace calls to protecting pages of custom heap allocated pages",
#endif
        u"Trace calls to async debugging API (default: false)",
#ifdef TRACK_DISPATCH
        u"Save stack traces of where JavascriptDispatch/HostVariant are created",
#endif
        u"Dump details",
        u"Enable fully qualified name",
        u"Use UTF8 for file output",
        u"Version in which to run the jscript engine. [one of 1,2,3,4,5,6]. Default is latest for jc/jshost, 1 for IE",
        u"WER feature for extended exception support. Enabled when WinRT is enabled",
        u"Enable passing extended error stack string to test host.",
        u"error.StackTrace feature. Remove when feature complete",
        u"Perform a heap enumeration whenever shut a script engine down",
#ifdef HEAP_ENUMERATION_VALIDATION
        u"Validate that heap enumeration is reporting all Js::RecyclableObjects in the heap",
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
        u"Trace all Regex invocations to the output.",
        u"Collect usage statistics on all Regex invocations.",
        u"Trace compilation of UnifiedRegex expressions.",
        u"Display Regex AST (requires -RegexDebug to view). [default on]",
        u"Display Regex Annotated AST (requires -RegexDebug and -RegexDebugAST to view). [default on]",
        u"Display layout of UnifiedRegex bytecode (requires -RegexDebug to view).",
        u"Optimize regular expressions in the unified Regex system (default: true)",
        u"Size of the MRU list for dynamic regexes",
#endif

        u"Optimize script engine for many instances (low memory footprint per engine, assume low spare CPU cycles) (default: false)",
        u"Enable force array type mutation on re-entrant region",
        u"Seed used for the array mutation",
        u"Test trace for the given phase",
        u"Enable cleaning up the eval map",
#ifdef PROFILE_MEM
        u"Enable cleaning up the eval map",
#endif
        u"Virtually disables SSE-based optimizations above the specified SSE level in the Chakra JIT (does not affect CRT SSE usage)",
        u"Start reusing deleted property indexes after this many properties are deleted. Zero to disable reuse.",
        u"Force switch to string keyed version of SimpleDictionaryTypeHandler on first new property added to a SimpleDictionaryTypeHandler",
        u"Min Slot Capacity required to convert DictionaryTypeHandler to BigDictionaryTypeHandler.(Advisable to give more than 15 - to avoid false positive cases)",
        u"Create a true snapshot of the type of an object before enumeration and enumerate only those properties.",
        u"Should prototypes get unique types not shared with other objects (default: true)?",
        u"When becoming a prototype should the object switch to a new type (default: true)?",
        u"Determines whether inline caches are shared between all loads (or all stores) of the same property ID",
        u"Disable test only Debug object properties",
        u"enable Debug.dumpHeap even when DisableDebugObject is set",
        u"__msTestHandler",
        u"Specifies the perf-hint level (1,2) 1 == critical, 2 == only noisy",
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        u"Use the mem protect heap as the default heap",
#endif
#ifdef RECYCLER_STRESS
        u"Stress the recycler by collect on every allocation call",
        u"Stress the recycler by collect in the background thread on every allocation call",
        u"Stress the concurrent recycler by concurrent collect on every allocation call",
        u"Stress the concurrent recycler by concurrent collect on every allocation call and repeat mark and rescan in the background thread",
        u"Stress the partial recycler by partial collect on every allocation call",
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        u"Mark properties as fixed on path types (default: false).",
#endif
        u"Filter the bailout trace messages to specific bailout kinds.",
        u"Filter the rejit trace messages to specific bailout kinds.",

// recycler heuristic flags
        u"Maximum number of background finish mark",
        u"Millisecond to wait for background finish mark",
        u"Minimum number of bytes rescan to trigger background finish mark",

#if defined(_M_X64)
        u"Zero free memory with non-temporal stores to avoid evicting other content from processor cache",
#endif

// recycler memory restrict test flags
        u"Restrict recycler mark stack size (in pages)",
        u"Restrict recycler tracked object count during GC",

// make the recycler page integration path easier to hit
        u"Additional bytes to allocate with JavascriptNumber from number allocator (0~496)",

#if DBG
        u"Enable the initialization of the interpreter local slots with invalid stack vars",
#endif

#if DBG
        u"Override seed0 for Math.Random()",
        u"Override seed1 for Math.Random()",
#endif

        u"Clear all inline caches on every garbage collection",
        u"Compact inline cache invalidation lists if their utilization falls below this threshold",
        u"Clear uniquePropertyGuard entries from recyclableData if number of invalidations of constructor caches happened are more than the threshold.",

#ifdef IR_VIEWER
        u"Enable IRViewer functionality (improved UI for various stages of IR generation)",
#endif /* IR_VIEWER */

        u"Threshold for allocation-based GC initiation (in MB)",

#if DBG
            u"Use with SimulatePolyCacheWithOneTypeForFunction to simulate creating a polymorphic inline cache containing only one type due to a collision, for testing ObjTypeSpec",
#endif

        u"Idle timeout in milliseconds to do the cleanup in JIT server",
        u"Max inactive page allocators to keep before schedule a cleanup",

        u"Check write barrier setting on none write barrier pages",
        u"Always return true while checking barrier to test recycler regardless of annotation",
        u"Use to turn off write watch to test software write barrier on windows",
        u"Verify software write barrier bit is set while marking",
        u"Use to turn off background freeing and zeroing to simulate linux",
        u"Keep recycler track data after sweep until reuse",

        u"Max size(in MB) in single allocation",

        NULL
    };

    //
    // Parent flag categorization of flags
    //
    const Flag FlagParents[FlagCount + 1] =
    {
#if DBG
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
#ifdef ARENA_MEMORY_VERIFY
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
        NoParentFlag,

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        WasmExperimentalFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef CHECK_MEMORY_LEAK
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef DUMP_FRAGMENTATION_STATS
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
#ifdef DYNAMIC_PROFILE_STORAGE
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
#ifdef EDIT_AND_CONTINUE
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
        NoParentFlag,

// Master ES6 flag to enable ALL sub ES6 features/flags
        NoParentFlag,

// Master ES6 flag to enable Threshold ES6 features/flags
        NoParentFlag,

// Per ES6 feature/flag

        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,

        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
        ES6Flag,

        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,

        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,
        ES6Flag,

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
        ES6Flag,

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        ES6Flag,

        ES6Flag,
        ES6Flag,

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

        ES6Flag,

// Newer language feature flags

// ES BigInt flag
        NoParentFlag,

// ES Numeric Separator support for numeric constants
        NoParentFlag,

// ES Nullish coalescing operator support (??)
        NoParentFlag,

// ES Hashbang support for interpreter directive syntax
        NoParentFlag,

// ES Symbol.prototype.description flag
        NoParentFlag,

        NoParentFlag,

// ES Promise.any and AggregateError flag
        NoParentFlag,

// ES import.meta keyword meta-property
        NoParentFlag,

//ES globalThis flag
        NoParentFlag,

// This flag to be removed once JITing generator functions is stable
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

#if defined(_M_ARM32_OR_ARM64)
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef BYTECODE_TESTING
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef LEAK_REPORT
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef MEMSPECT_TRACKING
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,

        NoParentFlag,

        MitigateSpectreFlag,

        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,

        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,
        MitigateSpectreFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

#if ENABLE_COPYONACCESS_ARRAY
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif

        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#if DBG
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef ENABLE_TRACE
        NoParentFlag,
        NoParentFlag,
#ifdef STACK_BACK_TRACE
        NoParentFlag,
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
        NoParentFlag,
#ifdef ENABLE_PREJIT
        NoParentFlag,
#endif
        NoParentFlag,
#if PROFILE_DICTIONARY
        NoParentFlag,
#endif
#ifdef PROFILE_EXEC
        NoParentFlag,
        NoParentFlag,
#endif
#ifdef PROFILE_OBJECT_LITERALS
        NoParentFlag,
#endif
#ifdef PROFILE_MEM
        NoParentFlag,
#endif
#ifdef PROFILE_STRINGS
        NoParentFlag,
#endif
#ifdef PROFILE_TYPES
        NoParentFlag,
#endif
#ifdef PROFILE_EVALMAP
        NoParentFlag,
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
        NoParentFlag,
#endif

#if DBG
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef RECYCLER_STRESS
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif // RECYCLER_STRESS
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef RECYCLER_NO_PAGE_REUSE
        NoParentFlag,
#endif
#ifdef RECYCLER_MEMORY_VERIFY
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
#ifdef RECYCLER_VERIFY_MARK
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        NoParentFlag,
#endif
#if EXCEPTION_RECOVERY
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

#if defined(_M_X64)
        NoParentFlag,
#endif

#ifdef PROFILE_MEM
        NoParentFlag,
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
        NoParentFlag,
#ifdef TRACK_DISPATCH
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef HEAP_ENUMERATION_VALIDATION
        NoParentFlag,
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef PROFILE_MEM
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        NoParentFlag,
#endif
#ifdef RECYCLER_STRESS
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        NoParentFlag,
#endif
        NoParentFlag,
        NoParentFlag,

// recycler heuristic flags
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

#if defined(_M_X64)
        NoParentFlag,
#endif

// recycler memory restrict test flags
        NoParentFlag,
        NoParentFlag,

// make the recycler page integration path easier to hit
        NoParentFlag,

#if DBG
        NoParentFlag,
#endif

#if DBG
        NoParentFlag,
        NoParentFlag,
#endif

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

#ifdef IR_VIEWER
        NoParentFlag,
#endif /* IR_VIEWER */

        NoParentFlag,

#if DBG
            NoParentFlag,
#endif

        NoParentFlag,
        NoParentFlag,

        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,
        NoParentFlag,

        NoParentFlag,

        InvalidFlag
    };

    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------
    ///
    /// class ConfigFlagsTable
    ///
    ///----------------------------------------------------------------------------
    ///----------------------------------------------------------------------------


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::ConfigFlagsTable
    ///
    /// Constructor initializes all the flags with their default values. The nDummy
    /// variable is used to prevent the compiler error due to the trailing comma
    /// when we generate the list of flags.
    ///
    ///----------------------------------------------------------------------------

    ConfigFlagsTable::ConfigFlagsTable():
#if DBG
        ArrayValidate(false),
        MemOpMissingValueValidate(false),
        OOPJITFixupValidate(false),
#endif
#ifdef ARENA_MEMORY_VERIFY
        ArenaNoFreeList(false),
        ArenaNoPageReuse(false),
        ArenaUseHeapAlloc(false),
#endif
        ValidateInlineStack(false),
        AsmDiff(false),
        AsmDumpMode(nullptr),
        AsmJs(DEFAULT_CONFIG_AsmJs),
        AsmJsStopOnError(DEFAULT_CONFIG_AsmJsStopOnError),
        AsmJsEdge(DEFAULT_CONFIG_AsmJsEdge),
        Wasm(DEFAULT_CONFIG_Wasm),
        WasmI64(DEFAULT_CONFIG_WasmI64),
        WasmFastArray(DEFAULT_CONFIG_WasmFastArray),
        WasmSharedArrayVirtualBuffer(DEFAULT_CONFIG_WasmSharedArrayVirtualBuffer),
        WasmMathExFilter(DEFAULT_CONFIG_WasmMathExFilter),
        WasmCheckVersion(DEFAULT_CONFIG_WasmCheckVersion),
        WasmAssignModuleID(DEFAULT_CONFIG_WasmAssignModuleID),
        WasmIgnoreLimits(DEFAULT_CONFIG_WasmIgnoreLimits),
        WasmFold(DEFAULT_CONFIG_WasmFold),
        WasmIgnoreResponse(DEFAULT_CONFIG_WasmIgnoreResponse),
        WasmMaxTableSize(DEFAULT_CONFIG_WasmMaxTableSize),
        WasmThreads(DEFAULT_CONFIG_WasmThreads),
        WasmMultiValue(DEFAULT_CONFIG_WasmMultiValue),
        WasmSignExtends(DEFAULT_CONFIG_WasmSignExtends),
        WasmNontrapping(DEFAULT_CONFIG_WasmNontrapping),

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
        WasmExperimental(DEFAULT_CONFIG_WasmExperimental),

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        WasmSimd(true),

        AssertBreak(false),
        AssertPopUp(false),
        AssertIgnore(false),
        AsyncDebugging(DEFAULT_CONFIG_AsyncDebugging),
        BailOnNoProfileLimit(DEFAULT_CONFIG_BailOnNoProfileLimit),
        BailOnNoProfileRejitLimit(DEFAULT_CONFIG_BailOnNoProfileRejitLimit),
        BaselineMode(false),
        DumpOnCrash(nullptr),
        FullMemoryDump(nullptr),
        BailOut(),
        BailOutAtEveryLine(false),
        BailOutAtEveryByteCode(false),
        BailOutAtEveryImplicitCall(false),
        BailOutByteCode(),
        Benchmark(false),
        BgJit(true),
        BgParse(DEFAULT_CONFIG_BgParse),
        BgJitDelay(DEFAULT_CONFIG_BgJitDelay),
        BgJitDelayFgBuffer(DEFAULT_CONFIG_BgJitDelayFgBuffer),
        BgJitPendingFuncCap(DEFAULT_CONFIG_BgJitPendingFuncCap),

        CreateFunctionProxy(DEFAULT_CONFIG_CreateFunctionProxy),
        HybridFgJit(DEFAULT_CONFIG_HybridFgJit),
        HybridFgJitBgQueueLengthThreshold(DEFAULT_CONFIG_HybridFgJitBgQueueLengthThreshold),
        BytecodeHist(false),
        CurrentSourceInfo(DEFAULT_CONFIG_CurrentSourceInfo),
        CFGLog(false),
        CheckAlignment(false),
        CheckEmitBufferPermissions(false),
#ifdef CHECK_MEMORY_LEAK
        CheckMemoryLeak(false),
        DumpOnLeak(nullptr),
#endif
        CheckOpHelpers(false),
        CloneInlinedPolymorphicCaches(DEFAULT_CONFIG_CloneInlinedPolymorphicCaches),
        ConcurrentRuntime(DEFAULT_CONFIG_ConcurrentRuntime),
        ConstructorInlineThreshold(DEFAULT_CONFIG_ConstructorInlineThreshold),
        ConstructorCallsRequiredToFinalizeCachedType(DEFAULT_CONFIG_ConstructorCallsRequiredToFinalizeCachedType),
        PropertyCacheMissPenalty(DEFAULT_CONFIG_PropertyCacheMissPenalty),
        PropertyCacheMissThreshold(DEFAULT_CONFIG_PropertyCacheMissThreshold),
        PropertyCacheMissReset(DEFAULT_CONFIG_PropertyCacheMissReset),
        Debug(false),
        DebugBreak(),
        StatementDebugBreak(),
        DebugBreakOnPhaseBegin(),

        DebugWindow(false),
        ParserStateCache(DEFAULT_CONFIG_ParserStateCache),
        CompressParserStateCache(DEFAULT_CONFIG_CompressParserStateCache),
        DeferTopLevelTillFirstCall(DEFAULT_CONFIG_DeferTopLevelTillFirstCall),
        DeferParse(0),
        DirectCallTelemetryStats(DEFAULT_CONFIG_DirectCallTelemetryStats),
        DisableArrayBTree(false),
        DisableRentalThreading(DEFAULT_CONFIG_DisableRentalThreading),
        DisableVTuneSourceLineInfo(false),
        DisplayMemStats(false),
        Dump(),
#ifdef DUMP_FRAGMENTATION_STATS
        DumpFragmentationStats(false),
#endif
        DumpIRAddresses(false),
        DumpLineNoInColor(false),
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
        DumpObjectGraphOnExit(false),
        DumpObjectGraphOnCollect(false),
#endif
        DumpEvalStringOnRemoval(false),
        DumpObjectGraphOnEnum(false),
#ifdef DYNAMIC_PROFILE_STORAGE
        DynamicProfileCache(nullptr),
        Dpc(nullptr),
        DynamicProfileCacheDir(nullptr),
        DynamicProfileInput(nullptr),
        Dpi(nullptr),
#endif
#ifdef EDIT_AND_CONTINUE
        EditTest(false),
#endif
        WininetProfileCache(DEFAULT_CONFIG_WininetProfileCache),
        NoDynamicProfileInMemoryCache(false),
        ProfileBasedSpeculativeJit(DEFAULT_CONFIG_ProfileBasedSpeculativeJit),
        ProfileBasedSpeculationCap(DEFAULT_CONFIG_ProfileBasedSpeculationCap),
        ExecuteByteCodeBufferReturnsInvalidByteCode(false),
        ExpirableCollectionGCCount(DEFAULT_CONFIG_ExpirableCollectionGCCount),
        ExpirableCollectionTriggerThreshold(DEFAULT_CONFIG_ExpirableCollectionTriggerThreshold),
        SkipSplitOnNoResult(DEFAULT_CONFIG_SkipSplitWhenResultIgnored),
        Force32BitByteCode(false),

        CollectGarbage(DEFAULT_CONFIG_CollectGarbage),

        Intl(DEFAULT_CONFIG_Intl),
        IntlBuiltIns(DEFAULT_CONFIG_IntlBuiltIns),
        IntlPlatform(DEFAULT_CONFIG_IntlPlatform),

        JsBuiltIn(DEFAULT_CONFIG_JsBuiltIn),
        JitRepro(DEFAULT_CONFIG_JitRepro),
        EntryPointInfoRpcData(DEFAULT_CONFIG_EntryPointInfoRpcData),

        LdChakraLib(DEFAULT_CONFIG_LdChakraLib),
        TestChakraLib(DEFAULT_CONFIG_TestChakraLib),

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
        ES6(DEFAULT_CONFIG_ES6),

// Master ES6 flag to enable ALL sub ES6 features/flags
        ES6All(DEFAULT_CONFIG_ES6All),

// Master ES6 flag to enable Threshold ES6 features/flags
        ES6Experimental(DEFAULT_CONFIG_ES6All),

// Per ES6 feature/flag

        ES7AsyncAwait(DEFAULT_CONFIG_ES7AsyncAwait),
        ES6DateParseFix(DEFAULT_CONFIG_ES6DateParseFix),
        ES6FunctionNameFull(DEFAULT_CONFIG_ES6FunctionNameFull),
        ES6Generators(DEFAULT_CONFIG_ES6Generators),
        ES7ExponentiationOperator(DEFAULT_CONFIG_ES7ExponentionOperator),

        ES7ValuesEntries(DEFAULT_CONFIG_ES7ValuesEntries),
        ES7TrailingComma(DEFAULT_CONFIG_ES7TrailingComma),
        ES6IsConcatSpreadable(DEFAULT_CONFIG_ES6IsConcatSpreadable),
        ES6Math(DEFAULT_CONFIG_ES6Math),

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
        ESDynamicImport((true)),

        ES6Module(DEFAULT_CONFIG_ES6Module),
        ES6Object(DEFAULT_CONFIG_ES6Object),
        ES6Number(DEFAULT_CONFIG_ES6Number),
        ES6ObjectLiterals(DEFAULT_CONFIG_ES6ObjectLiterals),
        ES6Proxy(DEFAULT_CONFIG_ES6Proxy),
        ES6Rest(DEFAULT_CONFIG_ES6Rest),
        ES6Spread(DEFAULT_CONFIG_ES6Spread),
        ES6String(DEFAULT_CONFIG_ES6String),
        ES6StringPrototypeFixes(DEFAULT_CONFIG_ES6StringPrototypeFixes),
        ES2018ObjectRestSpread(DEFAULT_CONFIG_ES2018ObjectRestSpread),

        ES6PrototypeChain(DEFAULT_CONFIG_ES6PrototypeChain),
        ES6ToPrimitive(DEFAULT_CONFIG_ES6ToPrimitive),
        ES6ToLength(DEFAULT_CONFIG_ES6ToLength),
        ES6ToStringTag(DEFAULT_CONFIG_ES6ToStringTag),
        ES6Unicode(DEFAULT_CONFIG_ES6Unicode),
        ES6UnicodeVerbose(DEFAULT_CONFIG_ES6UnicodeVerbose),
        ES6Unscopables(DEFAULT_CONFIG_ES6Unscopables),
        ES6RegExSticky(DEFAULT_CONFIG_ES6RegExSticky),
        ES2018RegExDotAll(DEFAULT_CONFIG_ES2018RegExDotAll),
        ESExportNsAs(DEFAULT_CONFIG_ESExportNsAs),
        ES2018AsyncIteration(DEFAULT_CONFIG_ES2018AsyncIteration),
        ESTopLevelAwait(DEFAULT_CONFIG_ESTopLevelAwait),

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
        ES6RegExPrototypeProperties((false)),

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        ES6RegExSymbols((false)),

        ES6Verbose(DEFAULT_CONFIG_ES6Verbose),
        ESObjectGetOwnPropertyDescriptors(DEFAULT_CONFIG_ESObjectGetOwnPropertyDescriptors),

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

        ESSharedArrayBuffer((false)),

// Newer language feature flags

// ES BigInt flag
        ESBigInt(DEFAULT_CONFIG_ESBigInt),

// ES Numeric Separator support for numeric constants
        ESNumericSeparator(DEFAULT_CONFIG_ESNumericSeparator),

// ES Nullish coalescing operator support (??)
        ESNullishCoalescingOperator(DEFAULT_CONFIG_ESNullishCoalescingOperator),

// ES Hashbang support for interpreter directive syntax
        ESHashbang(DEFAULT_CONFIG_ESHashbang),

// ES Symbol.prototype.description flag
        ESSymbolDescription(DEFAULT_CONFIG_ESSymbolDescription),

        ESArrayFindFromLast(DEFAULT_CONFIG_ESArrayFindFromLast),

// ES Promise.any and AggregateError flag
        ESPromiseAny(DEFAULT_CONFIG_ESPromiseAny),

// ES import.meta keyword meta-property
        ESImportMeta(DEFAULT_CONFIG_ESImportMeta),

//ES globalThis flag
        ESGlobalThis(DEFAULT_CONFIG_ESGlobalThis),

// This flag to be removed once JITing generator functions is stable
        JitES6Generators(DEFAULT_CONFIG_JitES6Generators),

        FastLineColumnCalculation(DEFAULT_CONFIG_FastLineColumnCalculation),
        Filename(nullptr),
        FreeRejittedCode(true),
        ForceGuardPages(false),
        PrintGuardPageBounds(false),
        ForceLegacyEngine(false),
        Force(),
        Stress(),
        ForceArrayBTree(false),
        StrongArraySort(DEFAULT_CONFIG_StrongArraySort),
        ForceCleanPropertyOnCollect(DEFAULT_CONFIG_ForceCleanPropertyOnCollect),
        ForceCleanCacheOnCollect(DEFAULT_CONFIG_ForceCleanCacheOnCollect),
        ForceGCAfterJSONParse(DEFAULT_CONFIG_ForceGCAfterJSONParse),
        ForceDecommitOnCollect(DEFAULT_CONFIG_ForceDecommitOnCollect),
        ForceDeferParse(DEFAULT_CONFIG_ForceDeferParse),
        ForceDiagnosticsMode(false),
        ForceGetWriteWatchOOM(false),
        ForcePostLowerGlobOptInstrString(DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString),
        ForceSplitScope(DEFAULT_CONFIG_ForceSplitScope),
        EnumerateSpecialPropertiesInDebugger(DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger),
        EnableContinueAfterExceptionWrappersForHelpers(DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForHelpers),
        EnableContinueAfterExceptionWrappersForBuiltIns(DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForBuiltIns),
        EnableFunctionSourceReportForHeapEnum(DEFAULT_CONFIG_EnableFunctionSourceReportForHeapEnum),
        ForceFragmentAddressSpace(128 * 1024 * 1024),
        ForceOOMOnEBCommit(0),
        ForceDynamicProfile(DEFAULT_CONFIG_ForceDynamicProfile),
        ForceES5Array(DEFAULT_CONFIG_ForceES5Array),
        ForceAsmJsLinkFail(DEFAULT_CONFIG_ForceAsmJsLinkFail),
        ForceExpireOnNonCacheCollect(DEFAULT_CONFIG_ForceExpireOnNonCacheCollect),
        ForceFastPath(DEFAULT_CONFIG_ForceFastPath),
        ForceFloatPref(false),
        ForceJITLoopBody(DEFAULT_CONFIG_ForceJITLoopBody),
        ForceStaticInterpreterThunk(DEFAULT_CONFIG_ForceStaticInterpreterThunk),
        DumpCommentsFromReferencedFiles(DEFAULT_CONFIG_DumpCommentsFromReferencedFiles),
        DelayFullJITSmallFunc(DEFAULT_CONFIG_DelayFullJITSmallFunc),
        EnableFatalErrorOnOOM(DEFAULT_CONFIG_EnableFatalErrorOnOOM),

#if defined(_M_ARM32_OR_ARM64)
        ForceLocalsPtr(false),
#endif
        DeferLoadingAvailableSource(DEFAULT_CONFIG_DeferLoadingAvailableSource),
        ForceNative(false),
        ForceSerialized(DEFAULT_CONFIG_ForceSerialized),
        ForceSerializedBytecodeMajorVersion(0),
        ForceSerializedBytecodeVersionSchema(0),
        ForceStrictMode(false),
        ForceUndoDefer(false),
        ForceBlockingConcurrentCollect(false),
        FreTestDiagMode(false),
#ifdef BYTECODE_TESTING
        ByteCodeBranchLimit(128),
        MediumByteCodeLayout(false),
        LargeByteCodeLayout(false),
#endif
        InduceCodeGenFailure(DEFAULT_CONFIG_InduceCodeGenFailure),
        InduceCodeGenFailureSeed(0),
        InjectPartiallyInitializedInterpreterFrameError(DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameError),
        InjectPartiallyInitializedInterpreterFrameErrorType(DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameErrorType),
        GenerateByteCodeBufferReturnsCantGenerate(false),
        GoptCleanupThreshold(DEFAULT_CONFIG_GoptCleanupThreshold),
        AsmGoptCleanupThreshold(DEFAULT_CONFIG_AsmGoptCleanupThreshold),
        HighPrecisionDate(DEFAULT_CONFIG_HighPrecisionDate),
        InlineCountMax(DEFAULT_CONFIG_InlineCountMax),
        InlineCountMaxInLoopBodies(DEFAULT_CONFIG_InlineCountMaxInLoopBodies),
        icminlb(DEFAULT_CONFIG_InlineCountMaxInLoopBodies),
        InlineInLoopBodyScaleDownFactor(DEFAULT_CONFIG_InlineInLoopBodyScaleDownFactor),
        iilbsdf(DEFAULT_CONFIG_InlineInLoopBodyScaleDownFactor),
        InlineThreshold(DEFAULT_CONFIG_InlineThreshold),
        AggressiveInlineCountMax(DEFAULT_CONFIG_AggressiveInlineCountMax),
        AggressiveInlineThreshold(DEFAULT_CONFIG_AggressiveInlineThreshold),
        InlineThresholdAdjustCountInLargeFunction(DEFAULT_CONFIG_InlineThresholdAdjustCountInLargeFunction),
        InlineThresholdAdjustCountInMediumSizedFunction(DEFAULT_CONFIG_InlineThresholdAdjustCountInMediumSizedFunction),
        InlineThresholdAdjustCountInSmallFunction(DEFAULT_CONFIG_InlineThresholdAdjustCountInSmallFunction),
        AsmJsInlineAdjust(DEFAULT_CONFIG_AsmJsInlineAdjust),
        Interpret(nullptr),
        Instrument(),
        JitQueueThreshold(DEFAULT_CONFIG_JitQueueThreshold),
#ifdef LEAK_REPORT
        LeakReport(nullptr),
#endif
        LoopInlineThreshold(DEFAULT_CONFIG_LoopInlineThreshold),
        LeafInlineThreshold(DEFAULT_CONFIG_LeafInlineThreshold),
        ConstantArgumentInlineThreshold(DEFAULT_CONFIG_ConstantArgumentInlineThreshold),
        RecursiveInlineThreshold(DEFAULT_CONFIG_RecursiveInlineThreshold),
        RecursiveInlineDepthMax(DEFAULT_CONFIG_RecursiveInlineDepthMax),
        RecursiveInlineDepthMin(DEFAULT_CONFIG_RecursiveInlineDepthMin),
        RedeferralCap(DEFAULT_CONFIG_RedeferralCap),
        Loop(DEFAULT_CONFIG_Loop),
        LoopInterpretCount(DEFAULT_CONFIG_LoopInterpretCount),
        lic(DEFAULT_CONFIG_LoopInterpretCount),
        LoopProfileIterations(DEFAULT_CONFIG_LoopProfileIterations),
        OutsideLoopInlineThreshold(DEFAULT_CONFIG_OutsideLoopInlineThreshold),
        MaxFuncInlineDepth(DEFAULT_CONFIG_MaxFuncInlineDepth),
        MaxNumberOfInlineesWithLoop(DEFAULT_CONFIG_MaxNumberOfInlineesWithLoop),
#ifdef MEMSPECT_TRACKING
        Memspect(),
#endif
        PolymorphicInlineThreshold(DEFAULT_CONFIG_PolymorphicInlineThreshold),
        PrimeRecycler(DEFAULT_CONFIG_PrimeRecycler),
        TraceEngineRefcount(false),
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        LeakStackTrace(false),
        ForceMemoryLeak(false),
#endif
        DumpAfterFinalGC(false),
        ForceOldDateAPI(DEFAULT_CONFIG_ForceOldDateAPI),

        JitLoopBodyHotLoopThreshold(DEFAULT_CONFIG_JitLoopBodyHotLoopThreshold),
        LoopBodySizeThresholdToDisableOpts(DEFAULT_CONFIG_LoopBodySizeThresholdToDisableOpts),

        MaxJitThreadCount(DEFAULT_CONFIG_MaxJitThreadCount),
        ForceMaxJitThreadCount(DEFAULT_CONFIG_ForceMaxJitThreadCount),

        MitigateSpectre(DEFAULT_CONFIG_MitigateSpectre),

        AddMaskingBlocks(DEFAULT_CONFIG_AddMaskingBlocks),

        PoisonVarArrayLoad(DEFAULT_CONFIG_PoisonVarArrayLoad),
        PoisonIntArrayLoad(DEFAULT_CONFIG_PoisonIntArrayLoad),
        PoisonFloatArrayLoad(DEFAULT_CONFIG_PoisonFloatArrayLoad),
        PoisonTypedArrayLoad(DEFAULT_CONFIG_PoisonTypedArrayLoad),
        PoisonStringLoad(DEFAULT_CONFIG_PoisonStringLoad),
        PoisonObjectsForLoads(DEFAULT_CONFIG_PoisonObjectsForLoads),

        PoisonVarArrayStore(DEFAULT_CONFIG_PoisonVarArrayStore),
        PoisonIntArrayStore(DEFAULT_CONFIG_PoisonIntArrayStore),
        PoisonFloatArrayStore(DEFAULT_CONFIG_PoisonFloatArrayStore),
        PoisonTypedArrayStore(DEFAULT_CONFIG_PoisonTypedArrayStore),
        PoisonStringStore(DEFAULT_CONFIG_PoisonStringStore),
        PoisonObjectsForStores(DEFAULT_CONFIG_PoisonObjectsForStores),

        MinInterpretCount(0),
        MinSimpleJitRunCount(0),
        MaxInterpretCount(0),
        Mic(0),
        MaxSimpleJitRunCount(0),
        Msjrc(0),
        MinMemOpCount(DEFAULT_CONFIG_MinMemOpCount),
        Mmoc(DEFAULT_CONFIG_MinMemOpCount),

#if ENABLE_COPYONACCESS_ARRAY
        MaxCopyOnAccessArrayLength(DEFAULT_CONFIG_MaxCopyOnAccessArrayLength),
        MinCopyOnAccessArrayLength(DEFAULT_CONFIG_MinCopyOnAccessArrayLength),
        CopyOnAccessArraySegmentCacheSize(DEFAULT_CONFIG_CopyOnAccessArraySegmentCacheSize),
#endif

        MinTemplatizedJitRunCount(DEFAULT_CONFIG_MinTemplatizedJitRunCount),
        MinAsmJsInterpreterRunCount(DEFAULT_CONFIG_MinAsmJsInterpreterRunCount),

        MinTemplatizedJitLoopRunCount(DEFAULT_CONFIG_MinTemplatizedJitLoopRunCount),
        MaxTemplatizedJitRunCount(DEFAULT_CONFIG_MaxTemplatizedJitRunCount),
        Mtjrc(DEFAULT_CONFIG_MaxTemplatizedJitRunCount),
        MaxAsmJsInterpreterRunCount(DEFAULT_CONFIG_MaxAsmJsInterpreterRunCount),
        Maic(DEFAULT_CONFIG_MaxAsmJsInterpreterRunCount),

        AutoProfilingInterpreter0Limit(DEFAULT_CONFIG_AutoProfilingInterpreter0Limit),
        ProfilingInterpreter0Limit(DEFAULT_CONFIG_ProfilingInterpreter0Limit),
        AutoProfilingInterpreter1Limit(DEFAULT_CONFIG_AutoProfilingInterpreter1Limit),
        SimpleJitLimit(DEFAULT_CONFIG_SimpleJitLimit),
        ProfilingInterpreter1Limit(DEFAULT_CONFIG_ProfilingInterpreter1Limit),

        ExecutionModeLimits(u""),
        Eml(u""),
        EnforceExecutionModeLimits(false),
        Eeml(false),

        SimpleJitAfter(0),
        Sja(0),
        FullJitAfter(0),
        Fja(0),

        NewSimpleJit(DEFAULT_CONFIG_NewSimpleJit),

        MaxLinearIntCaseCount(DEFAULT_CONFIG_MaxLinearIntCaseCount),
        MaxSingleCharStrJumpTableSize(DEFAULT_CONFIG_MaxSingleCharStrJumpTableSize),
        MaxSingleCharStrJumpTableRatio(DEFAULT_CONFIG_MaxSingleCharStrJumpTableRatio),
        MinSwitchJumpTableSize(DEFAULT_CONFIG_MinSwitchJumpTableSize),
        MaxLinearStringCaseCount(DEFAULT_CONFIG_MaxLinearStringCaseCount),
        MinDeferredFuncTokenCount(DEFAULT_CONFIG_MinDeferredFuncTokenCount),
#if DBG
        SkipFuncCountForBailOnNoProfile(DEFAULT_CONFIG_SkipFuncCountForBailOnNoProfile),
#endif
        MaxJITFunctionBytecodeByteLength(DEFAULT_CONFIG_MaxJITFunctionBytecodeByteLength),
        MaxJITFunctionBytecodeCount(DEFAULT_CONFIG_MaxJITFunctionBytecodeCount),
        MaxLoopsPerFunction(DEFAULT_CONFIG_MaxLoopsPerFunction),
        FuncObjectInlineCacheThreshold(DEFAULT_CONFIG_FuncObjectInlineCacheThreshold),
        NoDeferParse(false),
        NoLogo(false),
        OOPJITMissingOpts(DEFAULT_CONFIG_OOPJITMissingOpts),
        CrashOnOOPJITFailure(DEFAULT_CONFIG_CrashOnOOPJITFailure),
        OOPCFGRegistration(DEFAULT_CONFIG_OOPCFGRegistration),
        ForceJITCFGCheck(DEFAULT_CONFIG_ForceJITCFGCheck),
        UseJITTrampoline(DEFAULT_CONFIG_UseJITTrampoline),
        NoNative(false),
        NopFrequency(DEFAULT_CONFIG_NopFrequency),
        NoStrictMode(false),
        NormalizeStats(false),
        Off(),
        OffProfiledByteCode(),
        On(),
        OutputFile(u"output.log"),
        OutputFileOpenMode(u"wt"),
#ifdef ENABLE_TRACE
        InMemoryTrace(DEFAULT_CONFIG_InMemoryTrace),
        InMemoryTraceBufferSize(DEFAULT_CONFIG_InMemoryTraceBufferSize),
#ifdef STACK_BACK_TRACE
        TraceWithStack(DEFAULT_CONFIG_TraceWithStack),
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
        PrintRunTimeDataCollectionTrace(false),
#ifdef ENABLE_PREJIT
        Prejit(DEFAULT_CONFIG_Prejit),
#endif
        PrintSrcInDump(true),
#if PROFILE_DICTIONARY
        ProfileDictionary(-1),
#endif
#ifdef PROFILE_EXEC
        Profile(),
        ProfileThreshold(0),
#endif
#ifdef PROFILE_OBJECT_LITERALS
        ProfileObjectLiteral(false),
#endif
#ifdef PROFILE_MEM
        ProfileMemory(),
#endif
#ifdef PROFILE_STRINGS
        ProfileStrings(false),
#endif
#ifdef PROFILE_TYPES
        ProfileTypes(false),
#endif
#ifdef PROFILE_EVALMAP
        ProfileEvalMap(false),
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
        ProfileBailOutRecordMemory(false),
#endif

#if DBG
        ValidateIntRanges(DEFAULT_CONFIG_ValidateIntRanges),
#endif
        RejitMaxBailOutCount(DEFAULT_CONFIG_RejitMaxBailOutCount),
        CallsToBailoutsRatioForRejit(DEFAULT_CONFIG_CallsToBailoutsRatioForRejit),
        LoopIterationsToBailoutsRatioForRejit(DEFAULT_CONFIG_LoopIterationsToBailoutsRatioForRejit),
        MinBailOutsBeforeRejit(DEFAULT_CONFIG_MinBailOutsBeforeRejit),
        MinBailOutsBeforeRejitForLoops(DEFAULT_CONFIG_MinBailOutsBeforeRejitForLoops),
        LibraryStackFrame(DEFAULT_CONFIG_LibraryStackFrame),
        LibraryStackFrameDebugger(DEFAULT_CONFIG_LibraryStackFrameDebugger),
#ifdef RECYCLER_STRESS
        RecyclerStress(false),
        RecyclerBackgroundStress(false),
        RecyclerConcurrentStress(false),
        RecyclerConcurrentRepeatStress(false),
        RecyclerPartialStress(false),
        RecyclerTrackStress(false),
        RecyclerInduceFalsePositives(false),
#endif // RECYCLER_STRESS
        RecyclerForceMarkInterior(DEFAULT_CONFIG_RecyclerForceMarkInterior),
        RecyclerPriorityBoostTimeout(5000),
        RecyclerThreadCollectTimeout(1000),
        EnableConcurrentSweepAlloc(true),
        ecsa(true),
        PageHeap(DEFAULT_CONFIG_PageHeap),
        PageHeapAllocStack(DEFAULT_CONFIG_PageHeapAllocStack),
        PageHeapFreeStack(DEFAULT_CONFIG_PageHeapFreeStack),
        PageHeapBucketNumber(),
        PageHeapBlockType(DEFAULT_CONFIG_PageHeapBlockType),
        PageHeapDecommitGuardPage(true),
#ifdef RECYCLER_NO_PAGE_REUSE
        RecyclerNoPageReuse(false),
#endif
#ifdef RECYCLER_MEMORY_VERIFY
        RecyclerVerify(),
        RecyclerVerifyPadSize(12),
#endif
        RecyclerTest(false),
        RecyclerProtectPagesOnRescan(false),
#ifdef RECYCLER_VERIFY_MARK
        RecyclerVerifyMark(false),
#endif
        LowMemoryCap(DEFAULT_CONFIG_LowMemoryCap),
        NewPagesCapDuringBGSweeping(DEFAULT_CONFIG_NewPagesCapDuringBGSweeping),
        AllocPolicyLimit(DEFAULT_CONFIG_AllocationPolicyLimit),
        RuntimeDataOutputFile(nullptr),
        SpeculationCap(DEFAULT_CONFIG_SpeculationCap),
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        Stats(),
#endif
#if EXCEPTION_RECOVERY
        SwallowExceptions(false),
#endif
        PrintSystemException(false),
        SwitchOptHolesThreshold(DEFAULT_CONFIG_SwitchOptHolesThreshold),
        TempMin(DEFAULT_CONFIG_TempMin),
        TempMax(DEFAULT_CONFIG_TempMax),
        Trace(),

#if defined(_M_X64)
        LoopAlignNopLimit(DEFAULT_CONFIG_LoopAlignNopLimit),
#endif

#ifdef PROFILE_MEM
        TraceMemory(),
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
        TraceMetaDataParsing(5),
        TraceWin8Allocations(false),
        TraceWin8DeallocationsImmediate(false),
        PrintWin8StatsDetailed(false),
        TraceProtectPages(false),
#endif
        TraceAsyncDebugCalls(DEFAULT_CONFIG_TraceAsyncDebugCalls),
#ifdef TRACK_DISPATCH
        TrackDispatch(false),
#endif
        Verbose(DEFAULT_CONFIG_Verbose),
        UseFullName(DEFAULT_CONFIG_UseFullName),
        Utf8(false),
        Version(6 ),
        WERExceptionSupport(false),
        ExtendedErrorStackForTestHost(DEFAULT_CONFIG_ExtendedErrorStackForTestHost),
        errorStackTrace(DEFAULT_CONFIG_errorStackTrace),
        DoHeapEnumOnEngineShutdown(false),
#ifdef HEAP_ENUMERATION_VALIDATION
        ValidateHeapEnum(false),
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
        RegexTracing(DEFAULT_CONFIG_RegexTracing),
        RegexProfile(DEFAULT_CONFIG_RegexProfile),
        RegexDebug(DEFAULT_CONFIG_RegexDebug),
        RegexDebugAST(DEFAULT_CONFIG_RegexDebugAST),
        RegexDebugAnnotatedAST(DEFAULT_CONFIG_RegexDebugAnnotatedAST),
        RegexBytecodeDebug(DEFAULT_CONFIG_RegexBytecodeDebug),
        RegexOptimize(DEFAULT_CONFIG_RegexOptimize),
        DynamicRegexMruListSize(DEFAULT_CONFIG_DynamicRegexMruListSize),
#endif

        OptimizeForManyInstances(DEFAULT_CONFIG_OptimizeForManyInstances),
        EnableArrayTypeMutation(DEFAULT_CONFIG_EnableArrayTypeMutation),
        ArrayMutationTestSeed(0),
        TestTrace(),
        EnableEvalMapCleanup(true),
#ifdef PROFILE_MEM
        TraceObjectAllocation(false),
#endif
        Sse(DEFAULT_CONFIG_Sse),
        DeletedPropertyReuseThreshold(DEFAULT_CONFIG_DeletedPropertyReuseThreshold),
        ForceStringKeyedSimpleDictionaryTypeHandler(DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler),
        BigDictionaryTypeHandlerThreshold(DEFAULT_CONFIG_BigDictionaryTypeHandlerThreshold),
        TypeSnapshotEnumeration(DEFAULT_CONFIG_TypeSnapshotEnumeration),
        IsolatePrototypes(DEFAULT_CONFIG_IsolatePrototypes),
        ChangeTypeOnProto(DEFAULT_CONFIG_ChangeTypeOnProto),
        ShareInlineCaches(DEFAULT_CONFIG_ShareInlineCaches),
        DisableDebugObject(DEFAULT_CONFIG_DisableDebugObject),
        DumpHeap(DEFAULT_CONFIG_DumpHeap),
        autoProxy(u"__msTestHandler"),
        PerfHintLevel(DEFAULT_CONFIG_PerfHintLevel),
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        MemProtectHeap(DEFAULT_CONFIG_MemProtectHeap),
#endif
#ifdef RECYCLER_STRESS
        MemProtectHeapStress(false),
        MemProtectHeapBackgroundStress(false),
        MemProtectHeapConcurrentStress(false),
        MemProtectHeapConcurrentRepeatStress(false),
        MemProtectHeapPartialStress(false),
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        FixPropsOnPathTypes(DEFAULT_CONFIG_FixPropsOnPathTypes),
#endif
        BailoutTraceFilter(),
        RejitTraceFilter(),

// recycler heuristic flags
        MaxBackgroundFinishMarkCount(1),
        BackgroundFinishMarkWaitTime(15),
        MinBackgroundRepeatMarkRescanBytes(-1),

#if defined(_M_X64)
        ZeroMemoryWithNonTemporalStore(DEFAULT_CONFIG_ZeroMemoryWithNonTemporalStore),
#endif

// recycler memory restrict test flags
        MaxMarkStackPageCount(-1),
        MaxTrackedObjectListCount(-1),

// make the recycler page integration path easier to hit
        NumberAllocPlusSize(0),

#if DBG
        InitializeInterpreterSlotsWithInvalidStackVar(false),
#endif

#if DBG
        PRNGSeed0(0),
        PRNGSeed1(0),
#endif

        ClearInlineCachesOnCollect(false),
        InlineCacheInvalidationListCompactionThreshold(DEFAULT_CONFIG_InlineCacheInvalidationListCompactionThreshold),
        ConstructorCacheInvalidationThreshold(DEFAULT_CONFIG_ConstructorCacheInvalidationThreshold),

#ifdef IR_VIEWER
        IRViewer(false),
#endif /* IR_VIEWER */

        GCMemoryThreshold(0),

#if DBG
            SimulatePolyCacheWithOneTypeForInlineCacheIndex(-1),
#endif

        JITServerIdleTimeout(500),
        JITServerMaxInactivePageAllocatorCount(10),

        StrictWriteBarrierCheck(DEFAULT_CONFIG_StrictWriteBarrierCheck),
        WriteBarrierTest(DEFAULT_CONFIG_WriteBarrierTest),
        ForceSoftwareWriteBarrier(DEFAULT_CONFIG_ForceSoftwareWriteBarrier),
        VerifyBarrierBit(DEFAULT_CONFIG_VerifyBarrierBit),
        EnableBGFreeZero(DEFAULT_CONFIG_EnableBGFreeZero),
        KeepRecyclerTrackData(DEFAULT_CONFIG_KeepRecyclerTrackData),

        MaxSingleAllocSizeInMB(DEFAULT_CONFIG_MaxSingleAllocSizeInMB),
        nDummy(0)
    {
        for(int i=0; i < FlagCount; flagPresent[i++] = false);

        // set mark for parent flags
        memset((this->flagIsParent),0,(sizeof(this->flagIsParent)));
        if ((int)NoParentFlag < FlagCount) this->flagIsParent[(int)NoParentFlag] = true;
        if ((int)WasmExperimentalFlag < FlagCount) this->flagIsParent[(int) WasmExperimentalFlag] = true;
        if ((int)ES6Flag < FlagCount) this->flagIsParent[(int)ES6Flag] = true;
        if ((int)MitigateSpectreFlag < FlagCount) this->flagIsParent[(int)MitigateSpectreFlag] = true;

        // set all parent flags to their default (setting all child flags to their right values)
        this->SetAllParentFlagsAsDefaultValue();

        rawInputFromConfigFileIndex = 0;
        memset(rawInputFromConfigFile, 0, sizeof(rawInputFromConfigFile));
    }


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::SetAllParentFlagsAsDefaultValue
    ///
    /// Iterate through all parent flags and set their default value
    ///
    /// Note: only Boolean type supported for now
    ///----------------------------------------------------------------------------

    String *
    ConfigFlagsTable::GetAsString(Flag flag) const
    {
        return reinterpret_cast<String* >(GetProperty(flag));
    }

    Phases *
    ConfigFlagsTable::GetAsPhase(Flag flag) const
    {
        return reinterpret_cast<Phases*>(GetProperty(flag));
    }

    Flag
    ConfigFlagsTable::GetOppositePhaseFlag(Flag flag) const
    {
        switch (flag)
        {
        case OnFlag: return OffFlag;
        case OffFlag: return OnFlag;
        }
        return InvalidFlag;
    }

    Boolean *
    ConfigFlagsTable::GetAsBoolean(Flag flag)  const
    {
        return reinterpret_cast<Boolean*>(GetProperty(flag));
    }

    Number *
    ConfigFlagsTable::GetAsNumber(Flag flag)  const
    {
        return reinterpret_cast<Number* >(GetProperty(flag));
    }

    NumberSet *
    ConfigFlagsTable::GetAsNumberSet(Flag flag)  const
    {
        return reinterpret_cast<NumberSet* >(GetProperty(flag));
    }

    NumberPairSet *
    ConfigFlagsTable::GetAsNumberPairSet(Flag flag)  const
    {
        return reinterpret_cast<NumberPairSet* >(GetProperty(flag));
    }

    NumberTrioSet *
    ConfigFlagsTable::GetAsNumberTrioSet(Flag flag) const
    {
        return reinterpret_cast<NumberTrioSet*>(GetProperty(flag));
    }

    NumberRange *
    ConfigFlagsTable::GetAsNumberRange(Flag flag)  const
    {
        return reinterpret_cast<NumberRange* >(GetProperty(flag));
    }

    void
    ConfigFlagsTable::Enable(Flag flag)
    {
        this->flagPresent[flag] = true;
    }

    void
    ConfigFlagsTable::Disable(Flag flag)
    {
        this->flagPresent[flag] = false;
    }

    bool
    ConfigFlagsTable::IsEnabled(Flag flag)
    {
        return this->flagPresent[flag];
    }

    bool
    ConfigFlagsTable::IsParentFlag(Flag flag) const
    {
        return this->flagIsParent[flag];
    }

    void
    ConfigFlagsTable::SetAllParentFlagsAsDefaultValue()
    {
        for (int i = 0; i < FlagCount; i++)
        {
            Flag currentFlag = (Flag) i;
            if (this->IsParentFlag(currentFlag))
            {
                // only supporting Boolean for now
                AssertMsg(this->GetFlagType(currentFlag) == FlagBoolean, "only supporting boolean flags as parent flags");

                Boolean defaultParentValue = this->GetDefaultValueAsBoolean(currentFlag);
                this->SetAsBoolean(currentFlag, defaultParentValue);
            }
        }
    }

    void ConfigFlagsTable::FinalizeConfiguration()
    {
        TransferAcronymFlagConfiguration();
        TranslateFlagConfiguration();
    }

    void ConfigFlagsTable::TransferAcronymFlagConfiguration()
    {
#ifdef DYNAMIC_PROFILE_STORAGE
        if(!IsEnabled(DynamicProfileCacheFlag) && IsEnabled(DpcFlag))
        {
            Enable(DynamicProfileCacheFlag);
            DynamicProfileCache = Dpc;
        }
        if(!IsEnabled(DynamicProfileInputFlag) && IsEnabled(DpiFlag))
        {
            Enable(DynamicProfileInputFlag);
            DynamicProfileInput = Dpi;
        }
#endif

        if(!IsEnabled(InlineCountMaxInLoopBodiesFlag) && IsEnabled(icminlbFlag))
        {
            Enable(InlineCountMaxInLoopBodiesFlag);
            InlineCountMaxInLoopBodies = icminlb;
        }
        if(!IsEnabled(InlineInLoopBodyScaleDownFactorFlag) && IsEnabled(iilbsdfFlag))
        {
            Enable(InlineInLoopBodyScaleDownFactorFlag);
            InlineInLoopBodyScaleDownFactor = iilbsdf;
        }
        if(!IsEnabled(LoopInterpretCountFlag) && IsEnabled(licFlag))
        {
            Enable(LoopInterpretCountFlag);
            LoopInterpretCount = lic;
        }

        if(!IsEnabled(MaxInterpretCountFlag) && IsEnabled(MicFlag))
        {
            Enable(MaxInterpretCountFlag);
            MaxInterpretCount = Mic;
        }
        if(!IsEnabled(MaxSimpleJitRunCountFlag) && IsEnabled(MsjrcFlag))
        {
            Enable(MaxSimpleJitRunCountFlag);
            MaxSimpleJitRunCount = Msjrc;
        }
        if(!IsEnabled(MinMemOpCountFlag) && IsEnabled(MmocFlag))
        {
            Enable(MinMemOpCountFlag);
            MinMemOpCount = Mmoc;
        }

        if(!IsEnabled(MaxTemplatizedJitRunCountFlag) && IsEnabled(MtjrcFlag))
        {
            Enable(MaxTemplatizedJitRunCountFlag);
            MaxTemplatizedJitRunCount = Mtjrc;
        }
        if(!IsEnabled(MaxAsmJsInterpreterRunCountFlag) && IsEnabled(MaicFlag))
        {
            Enable(MaxAsmJsInterpreterRunCountFlag);
            MaxAsmJsInterpreterRunCount = Maic;
        }

        if(!IsEnabled(ExecutionModeLimitsFlag) && IsEnabled(EmlFlag))
        {
            Enable(ExecutionModeLimitsFlag);
            ExecutionModeLimits = Eml;
        }
        if(!IsEnabled(EnforceExecutionModeLimitsFlag) && IsEnabled(EemlFlag))
        {
            Enable(EnforceExecutionModeLimitsFlag);
            EnforceExecutionModeLimits = Eeml;
        }

        if(!IsEnabled(SimpleJitAfterFlag) && IsEnabled(SjaFlag))
        {
            Enable(SimpleJitAfterFlag);
            SimpleJitAfter = Sja;
        }
        if(!IsEnabled(FullJitAfterFlag) && IsEnabled(FjaFlag))
        {
            Enable(FullJitAfterFlag);
            FullJitAfter = Fja;
        }

        if(!IsEnabled(EnableConcurrentSweepAllocFlag) && IsEnabled(ecsaFlag))
        {
            Enable(EnableConcurrentSweepAllocFlag);
            EnableConcurrentSweepAlloc = ecsa;
        }
    }

    void ConfigFlagsTable::TranslateFlagConfiguration()
    {
        const auto VerifyExecutionModeLimits = [this]()
        {
#ifdef DBG
            const Number zero = static_cast<Number>(0);
            const Number maxUint8 = static_cast<Number>(static_cast<uint8_t>(-1)); // entry point call count is uint8_t
            const Number maxUint16 = static_cast<Number>(static_cast<uint16>(-1));
#endif

            Assert(MinInterpretCount >= zero);
            Assert(MinInterpretCount <= maxUint16);
            Assert(MaxInterpretCount >= zero);
            Assert(MaxInterpretCount <= maxUint16);
            Assert(MinSimpleJitRunCount >= zero);
            Assert(MinSimpleJitRunCount <= maxUint8);
            Assert(MaxSimpleJitRunCount >= zero);
            Assert(MaxSimpleJitRunCount <= maxUint8);

            Assert(SimpleJitAfter >= zero);
            Assert(SimpleJitAfter <= maxUint8);
            Assert(FullJitAfter >= zero);
            Assert(FullJitAfter <= maxUint16);

            Assert(AutoProfilingInterpreter0Limit >= zero);
            Assert(AutoProfilingInterpreter0Limit <= maxUint16);
            Assert(ProfilingInterpreter0Limit >= zero);
            Assert(ProfilingInterpreter0Limit <= maxUint16);
            Assert(AutoProfilingInterpreter1Limit >= zero);
            Assert(AutoProfilingInterpreter1Limit <= maxUint16);
            Assert(SimpleJitLimit >= zero);
            Assert(SimpleJitLimit <= maxUint8);
            Assert(ProfilingInterpreter1Limit >= zero);
            Assert(ProfilingInterpreter1Limit <= maxUint16);
            Assert(
                (
                    AutoProfilingInterpreter0Limit +
                    ProfilingInterpreter0Limit +
                    AutoProfilingInterpreter1Limit +
                    SimpleJitLimit +
                    ProfilingInterpreter1Limit
                ) <= maxUint16);
        };
        VerifyExecutionModeLimits();

    #if !DISABLE_JIT
        if(ForceDynamicProfile)
        {
            Force.Enable(DynamicProfilePhase);
        }
        if(ForceJITLoopBody)
        {
            Force.Enable(JITLoopBodyPhase);
        }
    #endif
        if(NoDeferParse)
        {
            Off.Enable(DeferParsePhase);
        }

    #if !DISABLE_JIT
        [[maybe_unused]] bool dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = false;
        if((IsEnabled(MinInterpretCountFlag) || IsEnabled(MaxInterpretCountFlag)) &&
            !(IsEnabled(SimpleJitAfterFlag) || IsEnabled(FullJitAfterFlag)))
        {
            if(Off.IsEnabled(SimpleJitPhase))
            {
                Enable(FullJitAfterFlag);
                if(IsEnabled(MaxInterpretCountFlag))
                {
                    FullJitAfter = MaxInterpretCount;
                }
                else
                {
                    FullJitAfter = MinInterpretCount;
                    dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = true;
                }
            }
            else
            {
                Enable(SimpleJitAfterFlag);
                if(IsEnabled(MaxInterpretCountFlag))
                {
                    SimpleJitAfter = MaxInterpretCount;
                }
                else
                {
                    SimpleJitAfter = MinInterpretCount;
                    dontEnforceLimitsForSimpleJitAfterOrFullJitAfter = true;
                }
                if((IsEnabled(MinInterpretCountFlag) && IsEnabled(MinSimpleJitRunCountFlag)) ||
                    IsEnabled(MaxSimpleJitRunCountFlag))
                {
                    Enable(FullJitAfterFlag);
                    FullJitAfter = SimpleJitAfter;
                    if(IsEnabled(MaxSimpleJitRunCountFlag))
                    {
                        FullJitAfter += MaxSimpleJitRunCount;
                    }
                    else
                    {
                        FullJitAfter += MinSimpleJitRunCount;
                        Assert(dontEnforceLimitsForSimpleJitAfterOrFullJitAfter);
                    }
                }
            }
        }

        // Configure execution mode limits
        do
        {
            if(IsEnabled(AutoProfilingInterpreter0LimitFlag) ||
                IsEnabled(ProfilingInterpreter0LimitFlag) ||
                IsEnabled(AutoProfilingInterpreter1LimitFlag) ||
                IsEnabled(SimpleJitLimitFlag) ||
                IsEnabled(ProfilingInterpreter1LimitFlag))
            {
                break;
            }

            if(IsEnabled(ExecutionModeLimitsFlag))
            {
                uint autoProfilingInterpreter0Limit;
                uint profilingInterpreter0Limit;
                uint autoProfilingInterpreter1Limit;
                uint simpleJitLimit;
                uint profilingInterpreter1Limit;
                [[maybe_unused]] const int scannedCount =
                    PAL_swscanf(
                        static_cast<const char16_t*>(ExecutionModeLimits),
                        u"%u.%u.%u.%u.%u",
                        &autoProfilingInterpreter0Limit,
                        &profilingInterpreter0Limit,
                        &autoProfilingInterpreter1Limit,
                        &simpleJitLimit,
                        &profilingInterpreter1Limit);
                Assert(scannedCount == 5);

                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);

                AutoProfilingInterpreter0Limit = autoProfilingInterpreter0Limit;
                ProfilingInterpreter0Limit = profilingInterpreter0Limit;
                AutoProfilingInterpreter1Limit = autoProfilingInterpreter1Limit;
                SimpleJitLimit = simpleJitLimit;
                ProfilingInterpreter1Limit = profilingInterpreter1Limit;
                break;
            }

            if(!NewSimpleJit)
            {
                // Use the defaults for old simple JIT. The flags are not enabled here because the values can be changed later
                // based on other flags, only the defaults values are adjusted here.
                AutoProfilingInterpreter0Limit = DEFAULT_CONFIG_AutoProfilingInterpreter0Limit;
                ProfilingInterpreter0Limit = DEFAULT_CONFIG_ProfilingInterpreter0Limit;
                static_assert(
                    DEFAULT_CONFIG_AutoProfilingInterpreter0Limit <= DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit);
                AutoProfilingInterpreter1Limit =
                    DEFAULT_CONFIG_AutoProfilingInterpreterLimit_OldSimpleJit - DEFAULT_CONFIG_AutoProfilingInterpreter0Limit;
                static_assert(DEFAULT_CONFIG_ProfilingInterpreter0Limit <= DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit);
                SimpleJitLimit = DEFAULT_CONFIG_SimpleJitLimit_OldSimpleJit - DEFAULT_CONFIG_ProfilingInterpreter0Limit;
                ProfilingInterpreter1Limit = 0;
                VerifyExecutionModeLimits();
            }

            if (IsEnabled(SimpleJitAfterFlag))
            {
                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(EnforceExecutionModeLimitsFlag);

                {
                    Js::Number iterationsNeeded = SimpleJitAfter;
                    ProfilingInterpreter0Limit = min(ProfilingInterpreter0Limit, iterationsNeeded);
                    iterationsNeeded -= ProfilingInterpreter0Limit;
                    AutoProfilingInterpreter0Limit = iterationsNeeded;
                    AutoProfilingInterpreter1Limit = 0;
                }

                if(IsEnabled(FullJitAfterFlag))
                {
                    Enable(SimpleJitLimitFlag);
                    Enable(ProfilingInterpreter1LimitFlag);

                    Assert(SimpleJitAfter <= FullJitAfter);
                    Js::Number iterationsNeeded = FullJitAfter - SimpleJitAfter;
                    Js::Number profilingIterationsNeeded =
                        min(NewSimpleJit
                                ? DEFAULT_CONFIG_MinProfileIterations
                                : DEFAULT_CONFIG_MinProfileIterations_OldSimpleJit,
                            FullJitAfter) -
                        ProfilingInterpreter0Limit;
                    if(NewSimpleJit)
                    {
                        ProfilingInterpreter1Limit = min(ProfilingInterpreter1Limit, iterationsNeeded);
                        iterationsNeeded -= ProfilingInterpreter1Limit;
                        profilingIterationsNeeded -= ProfilingInterpreter1Limit;
                        SimpleJitLimit = iterationsNeeded;
                    }
                    else
                    {
                        SimpleJitLimit = iterationsNeeded;
                        profilingIterationsNeeded -= min(SimpleJitLimit, profilingIterationsNeeded);
                        ProfilingInterpreter1Limit = 0;
                    }

                    if(profilingIterationsNeeded != 0)
                    {
                        Js::Number iterationsToMove = min(AutoProfilingInterpreter1Limit, profilingIterationsNeeded);
                        AutoProfilingInterpreter1Limit -= iterationsToMove;
                        ProfilingInterpreter0Limit += iterationsToMove;
                        profilingIterationsNeeded -= iterationsToMove;

                        iterationsToMove = min(AutoProfilingInterpreter0Limit, profilingIterationsNeeded);
                        AutoProfilingInterpreter0Limit -= iterationsToMove;
                        ProfilingInterpreter0Limit += iterationsToMove;
                        profilingIterationsNeeded -= iterationsToMove;

                        Assert(profilingIterationsNeeded == 0);
                    }

                    Assert(
                        (
                            AutoProfilingInterpreter0Limit +
                            ProfilingInterpreter0Limit +
                            AutoProfilingInterpreter1Limit +
                            SimpleJitLimit +
                            ProfilingInterpreter1Limit
                        ) == FullJitAfter);
                }

                Assert(
                    (
                        AutoProfilingInterpreter0Limit +
                        ProfilingInterpreter0Limit +
                        AutoProfilingInterpreter1Limit
                    ) == SimpleJitAfter);
                EnforceExecutionModeLimits = true;
                break;
            }

            if(IsEnabled(FullJitAfterFlag))
            {
                Enable(AutoProfilingInterpreter0LimitFlag);
                Enable(ProfilingInterpreter0LimitFlag);
                Enable(AutoProfilingInterpreter1LimitFlag);
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);
                Enable(EnforceExecutionModeLimitsFlag);

                Js::Number iterationsNeeded = FullJitAfter;
                if(NewSimpleJit)
                {
                    ProfilingInterpreter1Limit = min(ProfilingInterpreter1Limit, iterationsNeeded);
                    iterationsNeeded -= ProfilingInterpreter1Limit;
                }
                else
                {
                    ProfilingInterpreter1Limit = 0;
                    SimpleJitLimit = min(SimpleJitLimit, iterationsNeeded);
                    iterationsNeeded -= SimpleJitLimit;
                }
                ProfilingInterpreter0Limit = min(ProfilingInterpreter0Limit, iterationsNeeded);
                iterationsNeeded -= ProfilingInterpreter0Limit;
                if(NewSimpleJit)
                {
                    SimpleJitLimit = min(SimpleJitLimit, iterationsNeeded);
                    iterationsNeeded -= SimpleJitLimit;
                }
                AutoProfilingInterpreter0Limit = min(AutoProfilingInterpreter0Limit, iterationsNeeded);
                iterationsNeeded -= AutoProfilingInterpreter0Limit;
                AutoProfilingInterpreter1Limit = iterationsNeeded;

                Assert(
                    (
                        AutoProfilingInterpreter0Limit +
                        ProfilingInterpreter0Limit +
                        AutoProfilingInterpreter1Limit +
                        SimpleJitLimit +
                        ProfilingInterpreter1Limit
                    ) == FullJitAfter);
                EnforceExecutionModeLimits = true;
                break;
            }
            if (IsEnabled(MaxTemplatizedJitRunCountFlag))
            {
                if (MaxTemplatizedJitRunCount >= 0)
                {
                    MinTemplatizedJitRunCount = MaxTemplatizedJitRunCount;
                }
            }
            if (IsEnabled(MaxAsmJsInterpreterRunCountFlag))
            {
                if (MaxAsmJsInterpreterRunCount >= 0)
                {
                    MinAsmJsInterpreterRunCount = MaxAsmJsInterpreterRunCount;
                }
            }

        } while(false);
    #endif

        if( (
            #ifdef ENABLE_PREJIT
                Prejit ||
            #endif
                ForceNative
            ) &&
            !NoNative)
        {
            Enable(AutoProfilingInterpreter0LimitFlag);
            Enable(ProfilingInterpreter0LimitFlag);
            Enable(AutoProfilingInterpreter1LimitFlag);
            Enable(EnforceExecutionModeLimitsFlag);

            // Override any relevant automatic configuration above
            AutoProfilingInterpreter0Limit = 0;
            ProfilingInterpreter0Limit = 0;
            AutoProfilingInterpreter1Limit = 0;
            if(Off.IsEnabled(SimpleJitPhase))
            {
                Enable(SimpleJitLimitFlag);
                Enable(ProfilingInterpreter1LimitFlag);

                SimpleJitLimit = 0;
                ProfilingInterpreter1Limit = 0;
            }

            EnforceExecutionModeLimits = true;
        }

        VerifyExecutionModeLimits();
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetFlag
    ///
    /// Given a string finds the corresponding enum Flag. The comparison is case
    /// in-sensitive
    ///
    ///----------------------------------------------------------------------------

    Flag
    ConfigFlagsTable::GetFlag(const char16_t* str)
    {
        const auto flagStr = chakra_rs::str_helper::to_lowercase(str);
        for(int i=0; i < FlagCount; i++)
        {
            if (flagStr == chakra_rs::str_helper::to_lowercase(FlagNames[i]))
            {
                return Flag(i);
            }
        }
        return InvalidFlag;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetPhase
    ///
    /// Given a string finds the corresponding enum Phase. The comparison is case
    /// in-sensitive
    ///
    ///----------------------------------------------------------------------------

    Phase
    ConfigFlagsTable::GetPhase(const char16_t* str)
    {
        const auto phaseStr = chakra_rs::str_helper::to_lowercase(str);
        for(int i=0; i < PhaseCount; i++)
        {
            if (phaseStr == chakra_rs::str_helper::to_lowercase(PhaseNames[i]))
            {
                return Phase(i);
            }
        }
        return InvalidPhase;
    }

    void
    ConfigFlagsTable::PrintUsageString()
    {
        Output::Print(u"List of Phases:\n");
        for(int i = 0; i < PhaseCount; i++)
        {
            if (i % 4 == 0)
            {
                Output::Print(u"\n  ");
            }
            Output::Print(u"%-40ls ", PhaseNames[i]);
        }

        Output::Print(u"\n\nList of flags:\n\n");
        for(int i = 0; i < FlagCount; i++)
        {
            Output::Print(u"%60ls ", FlagNames[i]);
            switch(GetFlagType(Flag(i)))
            {
            case InvalidFlagType:
                break;
            case FlagString:
                Output::Print(u"[:String]        ");
                break;
            case FlagPhases:
                Output::Print(u"[:Phase]         ");
                break;
            case FlagNumber:
                Output::Print(u"[:Number]        ");
                break;
            case FlagBoolean:
                Output::Print(u"                 ");
                break;
            case FlagNumberSet:
                Output::Print(u"[:NumberSet]     ");
                break;
            case FlagNumberPairSet:
                Output::Print(u"[:NumberPairSet] ");
                break;
            case FlagNumberTrioSet:
                Output::Print(u"[:NumberTrioSet] ");
                break;
            case FlagNumberRange:
                Output::Print(u"[:NumberRange]   ");
                break;
            default:
                Assert(false);
            }
            Output::Print(u"%ls\n", FlagDescriptions[i]);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetFlagType
    ///
    /// Given a flag it returns the type (PhaseFlag, StringFlag ...). This could
    /// easily have been a lookup table like FlagNames and PhaseNames but this
    /// seems more concise
    ///
    ///----------------------------------------------------------------------------


    FlagTypes
    ConfigFlagsTable::GetFlagType(Flag flag)
    {
        switch(flag)
        {
        #if DBG
        case ArrayValidateFlag:
            return FlagBoolean;
        case MemOpMissingValueValidateFlag:
            return FlagBoolean;
        case OOPJITFixupValidateFlag:
            return FlagBoolean;
        #endif
        #ifdef ARENA_MEMORY_VERIFY
        case ArenaNoFreeListFlag:
            return FlagBoolean;
        case ArenaNoPageReuseFlag:
            return FlagBoolean;
        case ArenaUseHeapAllocFlag:
            return FlagBoolean;
        #endif
        case ValidateInlineStackFlag:
            return FlagBoolean;
        case AsmDiffFlag:
            return FlagBoolean;
        case AsmDumpModeFlag:
            return FlagString;
        case AsmJsFlag:
            return FlagBoolean;
        case AsmJsStopOnErrorFlag:
            return FlagBoolean;
        case AsmJsEdgeFlag:
            return FlagBoolean;
        case WasmFlag:
            return FlagBoolean;
        case WasmI64Flag:
            return FlagBoolean;
        case WasmFastArrayFlag:
            return FlagBoolean;
        case WasmSharedArrayVirtualBufferFlag:
            return FlagBoolean;
        case WasmMathExFilterFlag:
            return FlagBoolean;
        case WasmCheckVersionFlag:
            return FlagBoolean;
        case WasmAssignModuleIDFlag:
            return FlagBoolean;
        case WasmIgnoreLimitsFlag:
            return FlagBoolean;
        case WasmFoldFlag:
            return FlagBoolean;
        case WasmIgnoreResponseFlag:
            return FlagBoolean;
        case WasmMaxTableSizeFlag:
            return FlagNumber;
        case WasmThreadsFlag:
            return FlagBoolean;
        case WasmMultiValueFlag:
            return FlagBoolean;
        case WasmSignExtendsFlag:
            return FlagBoolean;
        case WasmNontrappingFlag:
            return FlagBoolean;

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        case WasmExperimentalFlag:
            return FlagBoolean;

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        case WasmSimdFlag:
            return FlagBoolean;

        case AssertBreakFlag:
            return FlagBoolean;
        case AssertPopUpFlag:
            return FlagBoolean;
        case AssertIgnoreFlag:
            return FlagBoolean;
        case AsyncDebuggingFlag:
            return FlagBoolean;
        case BailOnNoProfileLimitFlag:
            return FlagNumber;
        case BailOnNoProfileRejitLimitFlag:
            return FlagNumber;
        case BaselineModeFlag:
            return FlagBoolean;
        case DumpOnCrashFlag:
            return FlagString;
        case FullMemoryDumpFlag:
            return FlagString;
        case BailOutFlag:
            return FlagNumberPairSet;
        case BailOutAtEveryLineFlag:
            return FlagBoolean;
        case BailOutAtEveryByteCodeFlag:
            return FlagBoolean;
        case BailOutAtEveryImplicitCallFlag:
            return FlagBoolean;
        case BailOutByteCodeFlag:
            return FlagNumberSet;
        case BenchmarkFlag:
            return FlagBoolean;
        case BgJitFlag:
            return FlagBoolean;
        case BgParseFlag:
            return FlagBoolean;
        case BgJitDelayFlag:
            return FlagNumber;
        case BgJitDelayFgBufferFlag:
            return FlagNumber;
        case BgJitPendingFuncCapFlag:
            return FlagNumber;

        case CreateFunctionProxyFlag:
            return FlagBoolean;
        case HybridFgJitFlag:
            return FlagBoolean;
        case HybridFgJitBgQueueLengthThresholdFlag:
            return FlagNumber;
        case BytecodeHistFlag:
            return FlagBoolean;
        case CurrentSourceInfoFlag:
            return FlagBoolean;
        case CFGLogFlag:
            return FlagBoolean;
        case CheckAlignmentFlag:
            return FlagBoolean;
        case CheckEmitBufferPermissionsFlag:
            return FlagBoolean;
        #ifdef CHECK_MEMORY_LEAK
        case CheckMemoryLeakFlag:
            return FlagBoolean;
        case DumpOnLeakFlag:
            return FlagString;
        #endif
        case CheckOpHelpersFlag:
            return FlagBoolean;
        case CloneInlinedPolymorphicCachesFlag:
            return FlagBoolean;
        case ConcurrentRuntimeFlag:
            return FlagBoolean;
        case ConstructorInlineThresholdFlag:
            return FlagNumber;
        case ConstructorCallsRequiredToFinalizeCachedTypeFlag:
            return FlagNumber;
        case PropertyCacheMissPenaltyFlag:
            return FlagNumber;
        case PropertyCacheMissThresholdFlag:
            return FlagNumber;
        case PropertyCacheMissResetFlag:
            return FlagNumber;
        case DebugFlag:
            return FlagBoolean;
        case DebugBreakFlag:
            return FlagNumberSet;
        case StatementDebugBreakFlag:
            return FlagNumberTrioSet;
        case DebugBreakOnPhaseBeginFlag:
            return FlagPhases;

        case DebugWindowFlag:
            return FlagBoolean;
        case ParserStateCacheFlag:
            return FlagBoolean;
        case CompressParserStateCacheFlag:
            return FlagBoolean;
        case DeferTopLevelTillFirstCallFlag:
            return FlagBoolean;
        case DeferParseFlag:
            return FlagNumber;
        case DirectCallTelemetryStatsFlag:
            return FlagBoolean;
        case DisableArrayBTreeFlag:
            return FlagBoolean;
        case DisableRentalThreadingFlag:
            return FlagBoolean;
        case DisableVTuneSourceLineInfoFlag:
            return FlagBoolean;
        case DisplayMemStatsFlag:
            return FlagBoolean;
        case DumpFlag:
            return FlagPhases;
        #ifdef DUMP_FRAGMENTATION_STATS
        case DumpFragmentationStatsFlag:
            return FlagBoolean;
        #endif
        case DumpIRAddressesFlag:
            return FlagBoolean;
        case DumpLineNoInColorFlag:
            return FlagBoolean;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        case DumpObjectGraphOnExitFlag:
            return FlagBoolean;
        case DumpObjectGraphOnCollectFlag:
            return FlagBoolean;
        #endif
        case DumpEvalStringOnRemovalFlag:
            return FlagBoolean;
        case DumpObjectGraphOnEnumFlag:
            return FlagBoolean;
        #ifdef DYNAMIC_PROFILE_STORAGE
        case DynamicProfileCacheFlag:
            return FlagString;
        case DpcFlag:
            return FlagString;
        case DynamicProfileCacheDirFlag:
            return FlagString;
        case DynamicProfileInputFlag:
            return FlagString;
        case DpiFlag:
            return FlagString;
        #endif
        #ifdef EDIT_AND_CONTINUE
        case EditTestFlag:
            return FlagBoolean;
        #endif
        case WininetProfileCacheFlag:
            return FlagBoolean;
        case NoDynamicProfileInMemoryCacheFlag:
            return FlagBoolean;
        case ProfileBasedSpeculativeJitFlag:
            return FlagBoolean;
        case ProfileBasedSpeculationCapFlag:
            return FlagNumber;
        case ExecuteByteCodeBufferReturnsInvalidByteCodeFlag:
            return FlagBoolean;
        case ExpirableCollectionGCCountFlag:
            return FlagNumber;
        case ExpirableCollectionTriggerThresholdFlag:
            return FlagNumber;
        case SkipSplitOnNoResultFlag:
            return FlagBoolean;
        case Force32BitByteCodeFlag:
            return FlagBoolean;

        case CollectGarbageFlag:
            return FlagBoolean;

        case IntlFlag:
            return FlagBoolean;
        case IntlBuiltInsFlag:
            return FlagBoolean;
        case IntlPlatformFlag:
            return FlagBoolean;

        case JsBuiltInFlag:
            return FlagBoolean;
        case JitReproFlag:
            return FlagBoolean;
        case EntryPointInfoRpcDataFlag:
            return FlagBoolean;

        case LdChakraLibFlag:
            return FlagBoolean;
        case TestChakraLibFlag:
            return FlagBoolean;

        case ES6Flag:
            return FlagBoolean;

        case ES6AllFlag:
            return FlagBoolean;

        case ES6ExperimentalFlag:
            return FlagBoolean;
        case ES7AsyncAwaitFlag:
            return FlagBoolean;
        case ES6DateParseFixFlag:
            return FlagBoolean;
        case ES6FunctionNameFullFlag:
            return FlagBoolean;
        case ES6GeneratorsFlag:
            return FlagBoolean;
        case ES7ExponentiationOperatorFlag:
            return FlagBoolean;
        case ES7ValuesEntriesFlag:
            return FlagBoolean;
        case ES7TrailingCommaFlag:
            return FlagBoolean;
        case ES6IsConcatSpreadableFlag:
            return FlagBoolean;
        case ES6MathFlag:
            return FlagBoolean;

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        case ESDynamicImportFlag:
            return FlagBoolean;

        case ES6ModuleFlag:
            return FlagBoolean;
        case ES6ObjectFlag:
            return FlagBoolean;
        case ES6NumberFlag:
            return FlagBoolean;
        case ES6ObjectLiteralsFlag:
            return FlagBoolean;
        case ES6ProxyFlag:
            return FlagBoolean;
        case ES6RestFlag:
            return FlagBoolean;
        case ES6SpreadFlag:
            return FlagBoolean;
        case ES6StringFlag:
            return FlagBoolean;
        case ES6StringPrototypeFixesFlag:
            return FlagBoolean;
        case ES2018ObjectRestSpreadFlag:
            return FlagBoolean;

        case ES6PrototypeChainFlag:
            return FlagBoolean;
        case ES6ToPrimitiveFlag:
            return FlagBoolean;
        case ES6ToLengthFlag:
            return FlagBoolean;
        case ES6ToStringTagFlag:
            return FlagBoolean;
        case ES6UnicodeFlag:
            return FlagBoolean;
        case ES6UnicodeVerboseFlag:
            return FlagBoolean;
        case ES6UnscopablesFlag:
            return FlagBoolean;
        case ES6RegExStickyFlag:
            return FlagBoolean;
        case ES2018RegExDotAllFlag:
            return FlagBoolean;
        case ESExportNsAsFlag:
            return FlagBoolean;
        case ES2018AsyncIterationFlag:
            return FlagBoolean;
        case ESTopLevelAwaitFlag:
            return FlagBoolean;

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        case ES6RegExPrototypePropertiesFlag:
            return FlagBoolean;

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        case ES6RegExSymbolsFlag:
            return FlagBoolean;

        case ES6VerboseFlag:
            return FlagBoolean;
        case ESObjectGetOwnPropertyDescriptorsFlag:
            return FlagBoolean;

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        case ESSharedArrayBufferFlag:
            return FlagBoolean;

        // Newer language feature flags

        // ES BigInt flag
        case ESBigIntFlag:
            return FlagBoolean;

        // ES Numeric Separator support for numeric constants
        case ESNumericSeparatorFlag:
            return FlagBoolean;

        // ES Nullish coalescing operator support (??)
        case ESNullishCoalescingOperatorFlag:
            return FlagBoolean;

        // ES Hashbang support for interpreter directive syntax
        case ESHashbangFlag:
            return FlagBoolean;

        // ES Symbol.prototype.description flag
        case ESSymbolDescriptionFlag:
            return FlagBoolean;

        case ESArrayFindFromLastFlag:
            return FlagBoolean;

        // ES Promise.any and AggregateError flag
        case ESPromiseAnyFlag:
            return FlagBoolean;

        // ES import.meta keyword meta-property
        case ESImportMetaFlag:
            return FlagBoolean;

        //ES globalThis flag
        case ESGlobalThisFlag:
            return FlagBoolean;

        // This flag to be removed once JITing generator functions is stable
        case JitES6GeneratorsFlag:
            return FlagBoolean;

        case FastLineColumnCalculationFlag:
            return FlagBoolean;
        case FilenameFlag:
            return FlagString;
        case FreeRejittedCodeFlag:
            return FlagBoolean;
        case ForceGuardPagesFlag:
            return FlagBoolean;
        case PrintGuardPageBoundsFlag:
            return FlagBoolean;
        case ForceLegacyEngineFlag:
            return FlagBoolean;
        case ForceFlag:
            return FlagPhases;
        case StressFlag:
            return FlagPhases;
        case ForceArrayBTreeFlag:
            return FlagBoolean;
        case StrongArraySortFlag:
            return FlagBoolean;
        case ForceCleanPropertyOnCollectFlag:
            return FlagBoolean;
        case ForceCleanCacheOnCollectFlag:
            return FlagBoolean;
        case ForceGCAfterJSONParseFlag:
            return FlagBoolean;
        case ForceDecommitOnCollectFlag:
            return FlagBoolean;
        case ForceDeferParseFlag:
            return FlagBoolean;
        case ForceDiagnosticsModeFlag:
            return FlagBoolean;
        case ForceGetWriteWatchOOMFlag:
            return FlagBoolean;
        case ForcePostLowerGlobOptInstrStringFlag:
            return FlagBoolean;
        case ForceSplitScopeFlag:
            return FlagBoolean;
        case EnumerateSpecialPropertiesInDebuggerFlag:
            return FlagBoolean;
        case EnableContinueAfterExceptionWrappersForHelpersFlag:
            return FlagBoolean;
        case EnableContinueAfterExceptionWrappersForBuiltInsFlag:
            return FlagBoolean;
        case EnableFunctionSourceReportForHeapEnumFlag:
            return FlagBoolean;
        case ForceFragmentAddressSpaceFlag:
            return FlagNumber;
        case ForceOOMOnEBCommitFlag:
            return FlagNumber;
        case ForceDynamicProfileFlag:
            return FlagBoolean;
        case ForceES5ArrayFlag:
            return FlagBoolean;
        case ForceAsmJsLinkFailFlag:
            return FlagBoolean;
        case ForceExpireOnNonCacheCollectFlag:
            return FlagBoolean;
        case ForceFastPathFlag:
            return FlagBoolean;
        case ForceFloatPrefFlag:
            return FlagBoolean;
        case ForceJITLoopBodyFlag:
            return FlagBoolean;
        case ForceStaticInterpreterThunkFlag:
            return FlagBoolean;
        case DumpCommentsFromReferencedFilesFlag:
            return FlagBoolean;
        case DelayFullJITSmallFuncFlag:
            return FlagNumber;
        case EnableFatalErrorOnOOMFlag:
            return FlagBoolean;

        #if defined(_M_ARM32_OR_ARM64)
        case ForceLocalsPtrFlag:
            return FlagBoolean;
        #endif
        case DeferLoadingAvailableSourceFlag:
            return FlagBoolean;
        case ForceNativeFlag:
            return FlagBoolean;
        case ForceSerializedFlag:
            return FlagBoolean;
        case ForceSerializedBytecodeMajorVersionFlag:
            return FlagNumber;
        case ForceSerializedBytecodeVersionSchemaFlag:
            return FlagNumber;
        case ForceStrictModeFlag:
            return FlagBoolean;
        case ForceUndoDeferFlag:
            return FlagBoolean;
        case ForceBlockingConcurrentCollectFlag:
            return FlagBoolean;
        case FreTestDiagModeFlag:
            return FlagBoolean;
        #ifdef BYTECODE_TESTING
        case ByteCodeBranchLimitFlag:
            return FlagNumber;
        case MediumByteCodeLayoutFlag:
            return FlagBoolean;
        case LargeByteCodeLayoutFlag:
            return FlagBoolean;
        #endif
        case InduceCodeGenFailureFlag:
            return FlagNumber;
        case InduceCodeGenFailureSeedFlag:
            return FlagNumber;
        case InjectPartiallyInitializedInterpreterFrameErrorFlag:
            return FlagNumber;
        case InjectPartiallyInitializedInterpreterFrameErrorTypeFlag:
            return FlagNumber;
        case GenerateByteCodeBufferReturnsCantGenerateFlag:
            return FlagBoolean;
        case GoptCleanupThresholdFlag:
            return FlagNumber;
        case AsmGoptCleanupThresholdFlag:
            return FlagNumber;
        case HighPrecisionDateFlag:
            return FlagBoolean;
        case InlineCountMaxFlag:
            return FlagNumber;
        case InlineCountMaxInLoopBodiesFlag:
            return FlagNumber;
        case icminlbFlag:
            return FlagNumber;
        case InlineInLoopBodyScaleDownFactorFlag:
            return FlagNumber;
        case iilbsdfFlag:
            return FlagNumber;
        case InlineThresholdFlag:
            return FlagNumber;
        case AggressiveInlineCountMaxFlag:
            return FlagNumber;
        case AggressiveInlineThresholdFlag:
            return FlagNumber;
        case InlineThresholdAdjustCountInLargeFunctionFlag:
            return FlagNumber;
        case InlineThresholdAdjustCountInMediumSizedFunctionFlag:
            return FlagNumber;
        case InlineThresholdAdjustCountInSmallFunctionFlag:
            return FlagNumber;
        case AsmJsInlineAdjustFlag:
            return FlagNumber;
        case InterpretFlag:
            return FlagString;
        case InstrumentFlag:
            return FlagPhases;
        case JitQueueThresholdFlag:
            return FlagNumber;
        #ifdef LEAK_REPORT
        case LeakReportFlag:
            return FlagString;
        #endif
        case LoopInlineThresholdFlag:
            return FlagNumber;
        case LeafInlineThresholdFlag:
            return FlagNumber;
        case ConstantArgumentInlineThresholdFlag:
            return FlagNumber;
        case RecursiveInlineThresholdFlag:
            return FlagNumber;
        case RecursiveInlineDepthMaxFlag:
            return FlagNumber;
        case RecursiveInlineDepthMinFlag:
            return FlagNumber;
        case RedeferralCapFlag:
            return FlagNumber;
        case LoopFlag:
            return FlagNumber;
        case LoopInterpretCountFlag:
            return FlagNumber;
        case licFlag:
            return FlagNumber;
        case LoopProfileIterationsFlag:
            return FlagNumber;
        case OutsideLoopInlineThresholdFlag:
            return FlagNumber;
        case MaxFuncInlineDepthFlag:
            return FlagNumber;
        case MaxNumberOfInlineesWithLoopFlag:
            return FlagNumber;
        #ifdef MEMSPECT_TRACKING
        case MemspectFlag:
            return FlagPhases;
        #endif
        case PolymorphicInlineThresholdFlag:
            return FlagNumber;
        case PrimeRecyclerFlag:
            return FlagBoolean;
        case TraceEngineRefcountFlag:
            return FlagBoolean;
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        case LeakStackTraceFlag:
            return FlagBoolean;
        case ForceMemoryLeakFlag:
            return FlagBoolean;
        #endif
        case DumpAfterFinalGCFlag:
            return FlagBoolean;
        case ForceOldDateAPIFlag:
            return FlagBoolean;

        case JitLoopBodyHotLoopThresholdFlag:
            return FlagNumber;
        case LoopBodySizeThresholdToDisableOptsFlag:
            return FlagNumber;

        case MaxJitThreadCountFlag:
            return FlagNumber;
        case ForceMaxJitThreadCountFlag:
            return FlagBoolean;

        case MitigateSpectreFlag:
            return FlagBoolean;

        case AddMaskingBlocksFlag:
            return FlagBoolean;

        case PoisonVarArrayLoadFlag:
            return FlagBoolean;
        case PoisonIntArrayLoadFlag:
            return FlagBoolean;
        case PoisonFloatArrayLoadFlag:
            return FlagBoolean;
        case PoisonTypedArrayLoadFlag:
            return FlagBoolean;
        case PoisonStringLoadFlag:
            return FlagBoolean;
        case PoisonObjectsForLoadsFlag:
            return FlagBoolean;

        case PoisonVarArrayStoreFlag:
            return FlagBoolean;
        case PoisonIntArrayStoreFlag:
            return FlagBoolean;
        case PoisonFloatArrayStoreFlag:
            return FlagBoolean;
        case PoisonTypedArrayStoreFlag:
            return FlagBoolean;
        case PoisonStringStoreFlag:
            return FlagBoolean;
        case PoisonObjectsForStoresFlag:
            return FlagBoolean;

        case MinInterpretCountFlag:
            return FlagNumber;
        case MinSimpleJitRunCountFlag:
            return FlagNumber;
        case MaxInterpretCountFlag:
            return FlagNumber;
        case MicFlag:
            return FlagNumber;
        case MaxSimpleJitRunCountFlag:
            return FlagNumber;
        case MsjrcFlag:
            return FlagNumber;
        case MinMemOpCountFlag:
            return FlagNumber;
        case MmocFlag:
            return FlagNumber;

        #if ENABLE_COPYONACCESS_ARRAY
        case MaxCopyOnAccessArrayLengthFlag:
            return FlagNumber;
        case MinCopyOnAccessArrayLengthFlag:
            return FlagNumber;
        case CopyOnAccessArraySegmentCacheSizeFlag:
            return FlagNumber;
        #endif

        case MinTemplatizedJitRunCountFlag:
            return FlagNumber;
        case MinAsmJsInterpreterRunCountFlag:
            return FlagNumber;

        case MinTemplatizedJitLoopRunCountFlag:
            return FlagNumber;
        case MaxTemplatizedJitRunCountFlag:
            return FlagNumber;
        case MtjrcFlag:
            return FlagNumber;
        case MaxAsmJsInterpreterRunCountFlag:
            return FlagNumber;
        case MaicFlag:
            return FlagNumber;

        case AutoProfilingInterpreter0LimitFlag:
            return FlagNumber;
        case ProfilingInterpreter0LimitFlag:
            return FlagNumber;
        case AutoProfilingInterpreter1LimitFlag:
            return FlagNumber;
        case SimpleJitLimitFlag:
            return FlagNumber;
        case ProfilingInterpreter1LimitFlag:
            return FlagNumber;

        case ExecutionModeLimitsFlag:
            return FlagString;
        case EmlFlag:
            return FlagString;
        case EnforceExecutionModeLimitsFlag:
            return FlagBoolean;
        case EemlFlag:
            return FlagBoolean;

        case SimpleJitAfterFlag:
            return FlagNumber;
        case SjaFlag:
            return FlagNumber;
        case FullJitAfterFlag:
            return FlagNumber;
        case FjaFlag:
            return FlagNumber;

        case NewSimpleJitFlag:
            return FlagBoolean;

        case MaxLinearIntCaseCountFlag:
            return FlagNumber;
        case MaxSingleCharStrJumpTableSizeFlag:
            return FlagNumber;
        case MaxSingleCharStrJumpTableRatioFlag:
            return FlagNumber;
        case MinSwitchJumpTableSizeFlag:
            return FlagNumber;
        case MaxLinearStringCaseCountFlag:
            return FlagNumber;
        case MinDeferredFuncTokenCountFlag:
            return FlagNumber;
        #if DBG
        case SkipFuncCountForBailOnNoProfileFlag:
            return FlagNumber;
        #endif
        case MaxJITFunctionBytecodeByteLengthFlag:
            return FlagNumber;
        case MaxJITFunctionBytecodeCountFlag:
            return FlagNumber;
        case MaxLoopsPerFunctionFlag:
            return FlagNumber;
        case FuncObjectInlineCacheThresholdFlag:
            return FlagNumber;
        case NoDeferParseFlag:
            return FlagBoolean;
        case NoLogoFlag:
            return FlagBoolean;
        case OOPJITMissingOptsFlag:
            return FlagBoolean;
        case CrashOnOOPJITFailureFlag:
            return FlagBoolean;
        case OOPCFGRegistrationFlag:
            return FlagBoolean;
        case ForceJITCFGCheckFlag:
            return FlagBoolean;
        case UseJITTrampolineFlag:
            return FlagBoolean;
        case NoNativeFlag:
            return FlagBoolean;
        case NopFrequencyFlag:
            return FlagNumber;
        case NoStrictModeFlag:
            return FlagBoolean;
        case NormalizeStatsFlag:
            return FlagBoolean;
        case OffFlag:
            return FlagPhases;
        case OffProfiledByteCodeFlag:
            return FlagPhases;
        case OnFlag:
            return FlagPhases;
        case OutputFileFlag:
            return FlagString;
        case OutputFileOpenModeFlag:
            return FlagString;
        #ifdef ENABLE_TRACE
        case InMemoryTraceFlag:
            return FlagBoolean;
        case InMemoryTraceBufferSizeFlag:
            return FlagNumber;
        #ifdef STACK_BACK_TRACE
        case TraceWithStackFlag:
            return FlagBoolean;
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        case PrintRunTimeDataCollectionTraceFlag:
            return FlagBoolean;
        #ifdef ENABLE_PREJIT
        case PrejitFlag:
            return FlagBoolean;
        #endif
        case PrintSrcInDumpFlag:
            return FlagBoolean;
        #if PROFILE_DICTIONARY
        case ProfileDictionaryFlag:
            return FlagNumber;
        #endif
        #ifdef PROFILE_EXEC
        case ProfileFlag:
            return FlagPhases;
        case ProfileThresholdFlag:
            return FlagNumber;
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        case ProfileObjectLiteralFlag:
            return FlagBoolean;
        #endif
        #ifdef PROFILE_MEM
        case ProfileMemoryFlag:
            return FlagString;
        #endif
        #ifdef PROFILE_STRINGS
        case ProfileStringsFlag:
            return FlagBoolean;
        #endif
        #ifdef PROFILE_TYPES
        case ProfileTypesFlag:
            return FlagBoolean;
        #endif
        #ifdef PROFILE_EVALMAP
        case ProfileEvalMapFlag:
            return FlagBoolean;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        case ProfileBailOutRecordMemoryFlag:
            return FlagBoolean;
        #endif

        #if DBG
        case ValidateIntRangesFlag:
            return FlagBoolean;
        #endif
        case RejitMaxBailOutCountFlag:
            return FlagNumber;
        case CallsToBailoutsRatioForRejitFlag:
            return FlagNumber;
        case LoopIterationsToBailoutsRatioForRejitFlag:
            return FlagNumber;
        case MinBailOutsBeforeRejitFlag:
            return FlagNumber;
        case MinBailOutsBeforeRejitForLoopsFlag:
            return FlagNumber;
        case LibraryStackFrameFlag:
            return FlagBoolean;
        case LibraryStackFrameDebuggerFlag:
            return FlagBoolean;
        #ifdef RECYCLER_STRESS
        case RecyclerStressFlag:
            return FlagBoolean;
        case RecyclerBackgroundStressFlag:
            return FlagBoolean;
        case RecyclerConcurrentStressFlag:
            return FlagBoolean;
        case RecyclerConcurrentRepeatStressFlag:
            return FlagBoolean;
        case RecyclerPartialStressFlag:
            return FlagBoolean;
        case RecyclerTrackStressFlag:
            return FlagBoolean;
        case RecyclerInduceFalsePositivesFlag:
            return FlagBoolean;
        #endif // RECYCLER_STRESS
        case RecyclerForceMarkInteriorFlag:
            return FlagBoolean;
        case RecyclerPriorityBoostTimeoutFlag:
            return FlagNumber;
        case RecyclerThreadCollectTimeoutFlag:
            return FlagNumber;
        case EnableConcurrentSweepAllocFlag:
            return FlagBoolean;
        case ecsaFlag:
            return FlagBoolean;
        case PageHeapFlag:
            return FlagNumber;
        case PageHeapAllocStackFlag:
            return FlagBoolean;
        case PageHeapFreeStackFlag:
            return FlagBoolean;
        case PageHeapBucketNumberFlag:
            return FlagNumberRange;
        case PageHeapBlockTypeFlag:
            return FlagNumber;
        case PageHeapDecommitGuardPageFlag:
            return FlagBoolean;
        #ifdef RECYCLER_NO_PAGE_REUSE
        case RecyclerNoPageReuseFlag:
            return FlagBoolean;
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
        case RecyclerVerifyFlag:
            return FlagPhases;
        case RecyclerVerifyPadSizeFlag:
            return FlagNumber;
        #endif
        case RecyclerTestFlag:
            return FlagBoolean;
        case RecyclerProtectPagesOnRescanFlag:
            return FlagBoolean;
        #ifdef RECYCLER_VERIFY_MARK
        case RecyclerVerifyMarkFlag:
            return FlagBoolean;
        #endif
        case LowMemoryCapFlag:
            return FlagNumber;
        case NewPagesCapDuringBGSweepingFlag:
            return FlagNumber;
        case AllocPolicyLimitFlag:
            return FlagNumber;
        case RuntimeDataOutputFileFlag:
            return FlagString;
        case SpeculationCapFlag:
            return FlagNumber;
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        case StatsFlag:
            return FlagPhases;
        #endif
        #if EXCEPTION_RECOVERY
        case SwallowExceptionsFlag:
            return FlagBoolean;
        #endif
        case PrintSystemExceptionFlag:
            return FlagBoolean;
        case SwitchOptHolesThresholdFlag:
            return FlagNumber;
        case TempMinFlag:
            return FlagNumber;
        case TempMaxFlag:
            return FlagNumber;
        case TraceFlag:
            return FlagPhases;

        #if defined(_M_X64)
        case LoopAlignNopLimitFlag:
            return FlagNumber;
        #endif

        #ifdef PROFILE_MEM
        case TraceMemoryFlag:
            return FlagPhases;
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        case TraceMetaDataParsingFlag:
            return FlagNumber;
        case TraceWin8AllocationsFlag:
            return FlagBoolean;
        case TraceWin8DeallocationsImmediateFlag:
            return FlagBoolean;
        case PrintWin8StatsDetailedFlag:
            return FlagBoolean;
        case TraceProtectPagesFlag:
            return FlagBoolean;
        #endif
        case TraceAsyncDebugCallsFlag:
            return FlagBoolean;
        #ifdef TRACK_DISPATCH
        case TrackDispatchFlag:
            return FlagBoolean;
        #endif
        case VerboseFlag:
            return FlagBoolean;
        case UseFullNameFlag:
            return FlagBoolean;
        case Utf8Flag:
            return FlagBoolean;
        case VersionFlag:
            return FlagNumber;
        case WERExceptionSupportFlag:
            return FlagBoolean;
        case ExtendedErrorStackForTestHostFlag:
            return FlagBoolean;
        case errorStackTraceFlag:
            return FlagBoolean;
        case DoHeapEnumOnEngineShutdownFlag:
            return FlagBoolean;
        #ifdef HEAP_ENUMERATION_VALIDATION
        case ValidateHeapEnumFlag:
            return FlagBoolean;
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        case RegexTracingFlag:
            return FlagBoolean;
        case RegexProfileFlag:
            return FlagBoolean;
        case RegexDebugFlag:
            return FlagBoolean;
        case RegexDebugASTFlag:
            return FlagBoolean;
        case RegexDebugAnnotatedASTFlag:
            return FlagBoolean;
        case RegexBytecodeDebugFlag:
            return FlagBoolean;
        case RegexOptimizeFlag:
            return FlagBoolean;
        case DynamicRegexMruListSizeFlag:
            return FlagNumber;
        #endif

        case OptimizeForManyInstancesFlag:
            return FlagBoolean;
        case EnableArrayTypeMutationFlag:
            return FlagBoolean;
        case ArrayMutationTestSeedFlag:
            return FlagNumber;
        case TestTraceFlag:
            return FlagPhases;
        case EnableEvalMapCleanupFlag:
            return FlagBoolean;
        #ifdef PROFILE_MEM
        case TraceObjectAllocationFlag:
            return FlagBoolean;
        #endif
        case SseFlag:
            return FlagNumber;
        case DeletedPropertyReuseThresholdFlag:
            return FlagNumber;
        case ForceStringKeyedSimpleDictionaryTypeHandlerFlag:
            return FlagBoolean;
        case BigDictionaryTypeHandlerThresholdFlag:
            return FlagNumber;
        case TypeSnapshotEnumerationFlag:
            return FlagBoolean;
        case IsolatePrototypesFlag:
            return FlagBoolean;
        case ChangeTypeOnProtoFlag:
            return FlagBoolean;
        case ShareInlineCachesFlag:
            return FlagBoolean;
        case DisableDebugObjectFlag:
            return FlagBoolean;
        case DumpHeapFlag:
            return FlagBoolean;
        case autoProxyFlag:
            return FlagString;
        case PerfHintLevelFlag:
            return FlagNumber;
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        case MemProtectHeapFlag:
            return FlagBoolean;
        #endif
        #ifdef RECYCLER_STRESS
        case MemProtectHeapStressFlag:
            return FlagBoolean;
        case MemProtectHeapBackgroundStressFlag:
            return FlagBoolean;
        case MemProtectHeapConcurrentStressFlag:
            return FlagBoolean;
        case MemProtectHeapConcurrentRepeatStressFlag:
            return FlagBoolean;
        case MemProtectHeapPartialStressFlag:
            return FlagBoolean;
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        case FixPropsOnPathTypesFlag:
            return FlagBoolean;
        #endif
        case BailoutTraceFilterFlag:
            return FlagNumberSet;
        case RejitTraceFilterFlag:
            return FlagNumberSet;

        // recycler heuristic flags
        case MaxBackgroundFinishMarkCountFlag:
            return FlagNumber;
        case BackgroundFinishMarkWaitTimeFlag:
            return FlagNumber;
        case MinBackgroundRepeatMarkRescanBytesFlag:
            return FlagNumber;

        #if defined(_M_X64)
        case ZeroMemoryWithNonTemporalStoreFlag:
            return FlagBoolean;
        #endif

        // recycler memory restrict test flags
        case MaxMarkStackPageCountFlag:
            return FlagNumber;
        case MaxTrackedObjectListCountFlag:
            return FlagNumber;

        // make the recycler page integration path easier to hit
        case NumberAllocPlusSizeFlag:
            return FlagNumber;

        #if DBG
        case InitializeInterpreterSlotsWithInvalidStackVarFlag:
            return FlagBoolean;
        #endif

        #if DBG
        case PRNGSeed0Flag:
            return FlagNumber;
        case PRNGSeed1Flag:
            return FlagNumber;
        #endif

        case ClearInlineCachesOnCollectFlag:
            return FlagBoolean;
        case InlineCacheInvalidationListCompactionThresholdFlag:
            return FlagNumber;
        case ConstructorCacheInvalidationThresholdFlag:
            return FlagNumber;

        #ifdef IR_VIEWER
        case IRViewerFlag:
            return FlagBoolean;
        #endif /* IR_VIEWER */

        case GCMemoryThresholdFlag:
            return FlagNumber;

        #if DBG
            case SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag:
            return FlagNumber;
        #endif

        case JITServerIdleTimeoutFlag:
            return FlagNumber;
        case JITServerMaxInactivePageAllocatorCountFlag:
            return FlagNumber;

        case StrictWriteBarrierCheckFlag:
            return FlagBoolean;
        case WriteBarrierTestFlag:
            return FlagBoolean;
        case ForceSoftwareWriteBarrierFlag:
            return FlagBoolean;
        case VerifyBarrierBitFlag:
            return FlagBoolean;
        case EnableBGFreeZeroFlag:
            return FlagBoolean;
        case KeepRecyclerTrackDataFlag:
            return FlagBoolean;

        case MaxSingleAllocSizeInMBFlag:
            return FlagNumber;

        default:
            return InvalidFlagType;
        }
    }


    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetProperty
    ///
    /// Get the field corresponding to the flag. used as an internal method for
    /// the various GetAs* methods.
    ///
    ///----------------------------------------------------------------------------


    void *
    ConfigFlagsTable::GetProperty(Flag flag) const
    {
        switch(flag)
        {
        #if DBG
        case ArrayValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArrayValidate));
        case MemOpMissingValueValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemOpMissingValueValidate));
        case OOPJITFixupValidateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPJITFixupValidate));
        #endif
        #ifdef ARENA_MEMORY_VERIFY
        case ArenaNoFreeListFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaNoFreeList));
        case ArenaNoPageReuseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaNoPageReuse));
        case ArenaUseHeapAllocFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ArenaUseHeapAlloc));
        #endif
        case ValidateInlineStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateInlineStack));
        case AsmDiffFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmDiff));
        case AsmDumpModeFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&AsmDumpMode));
        case AsmJsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJs));
        case AsmJsStopOnErrorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJsStopOnError));
        case AsmJsEdgeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsmJsEdge));
        case WasmFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Wasm));
        case WasmI64Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmI64));
        case WasmFastArrayFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmFastArray));
        case WasmSharedArrayVirtualBufferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSharedArrayVirtualBuffer));
        case WasmMathExFilterFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmMathExFilter));
        case WasmCheckVersionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmCheckVersion));
        case WasmAssignModuleIDFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmAssignModuleID));
        case WasmIgnoreLimitsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmIgnoreLimits));
        case WasmFoldFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmFold));
        case WasmIgnoreResponseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmIgnoreResponse));
        case WasmMaxTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&WasmMaxTableSize));
        case WasmThreadsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmThreads));
        case WasmMultiValueFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmMultiValue));
        case WasmSignExtendsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSignExtends));
        case WasmNontrappingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmNontrapping));

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        case WasmExperimentalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmExperimental));

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        case WasmSimdFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WasmSimd));

        case AssertBreakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertBreak));
        case AssertPopUpFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertPopUp));
        case AssertIgnoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AssertIgnore));
        case AsyncDebuggingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AsyncDebugging));
        case BailOnNoProfileLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BailOnNoProfileLimit));
        case BailOnNoProfileRejitLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BailOnNoProfileRejitLimit));
        case BaselineModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BaselineMode));
        case DumpOnCrashFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DumpOnCrash));
        case FullMemoryDumpFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&FullMemoryDump));
        case BailOutFlag:
            return reinterpret_cast<void*>(const_cast<NumberPairSet*>(&BailOut));
        case BailOutAtEveryLineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryLine));
        case BailOutAtEveryByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryByteCode));
        case BailOutAtEveryImplicitCallFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BailOutAtEveryImplicitCall));
        case BailOutByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&BailOutByteCode));
        case BenchmarkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Benchmark));
        case BgJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BgJit));
        case BgParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BgParse));
        case BgJitDelayFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitDelay));
        case BgJitDelayFgBufferFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitDelayFgBuffer));
        case BgJitPendingFuncCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BgJitPendingFuncCap));

        case CreateFunctionProxyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CreateFunctionProxy));
        case HybridFgJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&HybridFgJit));
        case HybridFgJitBgQueueLengthThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&HybridFgJitBgQueueLengthThreshold));
        case BytecodeHistFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&BytecodeHist));
        case CurrentSourceInfoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CurrentSourceInfo));
        case CFGLogFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CFGLog));
        case CheckAlignmentFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckAlignment));
        case CheckEmitBufferPermissionsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckEmitBufferPermissions));
        #ifdef CHECK_MEMORY_LEAK
        case CheckMemoryLeakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckMemoryLeak));
        case DumpOnLeakFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DumpOnLeak));
        #endif
        case CheckOpHelpersFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CheckOpHelpers));
        case CloneInlinedPolymorphicCachesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CloneInlinedPolymorphicCaches));
        case ConcurrentRuntimeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ConcurrentRuntime));
        case ConstructorInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorInlineThreshold));
        case ConstructorCallsRequiredToFinalizeCachedTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorCallsRequiredToFinalizeCachedType));
        case PropertyCacheMissPenaltyFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissPenalty));
        case PropertyCacheMissThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissThreshold));
        case PropertyCacheMissResetFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PropertyCacheMissReset));
        case DebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Debug));
        case DebugBreakFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&DebugBreak));
        case StatementDebugBreakFlag:
            return reinterpret_cast<void*>(const_cast<NumberTrioSet*>(&StatementDebugBreak));
        case DebugBreakOnPhaseBeginFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&DebugBreakOnPhaseBegin));

        case DebugWindowFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DebugWindow));
        case ParserStateCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ParserStateCache));
        case CompressParserStateCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CompressParserStateCache));
        case DeferTopLevelTillFirstCallFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DeferTopLevelTillFirstCall));
        case DeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DeferParse));
        case DirectCallTelemetryStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DirectCallTelemetryStats));
        case DisableArrayBTreeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableArrayBTree));
        case DisableRentalThreadingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableRentalThreading));
        case DisableVTuneSourceLineInfoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableVTuneSourceLineInfo));
        case DisplayMemStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisplayMemStats));
        case DumpFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Dump));
        #ifdef DUMP_FRAGMENTATION_STATS
        case DumpFragmentationStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpFragmentationStats));
        #endif
        case DumpIRAddressesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpIRAddresses));
        case DumpLineNoInColorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpLineNoInColor));
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        case DumpObjectGraphOnExitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnExit));
        case DumpObjectGraphOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnCollect));
        #endif
        case DumpEvalStringOnRemovalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpEvalStringOnRemoval));
        case DumpObjectGraphOnEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpObjectGraphOnEnum));
        #ifdef DYNAMIC_PROFILE_STORAGE
        case DynamicProfileCacheFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileCache));
        case DpcFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Dpc));
        case DynamicProfileCacheDirFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileCacheDir));
        case DynamicProfileInputFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&DynamicProfileInput));
        case DpiFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Dpi));
        #endif
        #ifdef EDIT_AND_CONTINUE
        case EditTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EditTest));
        #endif
        case WininetProfileCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WininetProfileCache));
        case NoDynamicProfileInMemoryCacheFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoDynamicProfileInMemoryCache));
        case ProfileBasedSpeculativeJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileBasedSpeculativeJit));
        case ProfileBasedSpeculationCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileBasedSpeculationCap));
        case ExecuteByteCodeBufferReturnsInvalidByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ExecuteByteCodeBufferReturnsInvalidByteCode));
        case ExpirableCollectionGCCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ExpirableCollectionGCCount));
        case ExpirableCollectionTriggerThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ExpirableCollectionTriggerThreshold));
        case SkipSplitOnNoResultFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&SkipSplitOnNoResult));
        case Force32BitByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Force32BitByteCode));

        case CollectGarbageFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CollectGarbage));

        case IntlFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Intl));
        case IntlBuiltInsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IntlBuiltIns));
        case IntlPlatformFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IntlPlatform));

        case JsBuiltInFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JsBuiltIn));
        case JitReproFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JitRepro));
        case EntryPointInfoRpcDataFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EntryPointInfoRpcData));

        case LdChakraLibFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LdChakraLib));
        case TestChakraLibFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TestChakraLib));

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        case ES6Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6));

        // Master ES6 flag to enable ALL sub ES6 features/flags
        case ES6AllFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6All));

        // Master ES6 flag to enable Threshold ES6 features/flags
        case ES6ExperimentalFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Experimental));

        // Per ES6 feature/flag

        case ES7AsyncAwaitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7AsyncAwait));
        case ES6DateParseFixFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6DateParseFix));
        case ES6FunctionNameFullFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6FunctionNameFull));
        case ES6GeneratorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Generators));
        case ES7ExponentiationOperatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7ExponentiationOperator));

        case ES7ValuesEntriesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7ValuesEntries));
        case ES7TrailingCommaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES7TrailingComma));
        case ES6IsConcatSpreadableFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6IsConcatSpreadable));
        case ES6MathFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Math));

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        case ESDynamicImportFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESDynamicImport));

        case ES6ModuleFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Module));
        case ES6ObjectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Object));
        case ES6NumberFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Number));
        case ES6ObjectLiteralsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ObjectLiterals));
        case ES6ProxyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Proxy));
        case ES6RestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Rest));
        case ES6SpreadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Spread));
        case ES6StringFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6String));
        case ES6StringPrototypeFixesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6StringPrototypeFixes));
        case ES2018ObjectRestSpreadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018ObjectRestSpread));

        case ES6PrototypeChainFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6PrototypeChain));
        case ES6ToPrimitiveFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToPrimitive));
        case ES6ToLengthFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToLength));
        case ES6ToStringTagFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6ToStringTag));
        case ES6UnicodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Unicode));
        case ES6UnicodeVerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6UnicodeVerbose));
        case ES6UnscopablesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Unscopables));
        case ES6RegExStickyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExSticky));
        case ES2018RegExDotAllFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018RegExDotAll));
        case ESExportNsAsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESExportNsAs));
        case ES2018AsyncIterationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES2018AsyncIteration));
        case ESTopLevelAwaitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESTopLevelAwait));

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        case ES6RegExPrototypePropertiesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExPrototypeProperties));

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        case ES6RegExSymbolsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6RegExSymbols));

        case ES6VerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ES6Verbose));
        case ESObjectGetOwnPropertyDescriptorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESObjectGetOwnPropertyDescriptors));

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        case ESSharedArrayBufferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESSharedArrayBuffer));

        // Newer language feature flags

        // ES BigInt flag
        case ESBigIntFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESBigInt));

        // ES Numeric Separator support for numeric constants
        case ESNumericSeparatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESNumericSeparator));

        // ES Nullish coalescing operator support (??)
        case ESNullishCoalescingOperatorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESNullishCoalescingOperator));

        // ES Hashbang support for interpreter directive syntax
        case ESHashbangFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESHashbang));

        // ES Symbol.prototype.description flag
        case ESSymbolDescriptionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESSymbolDescription));

        case ESArrayFindFromLastFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESArrayFindFromLast));

        // ES Promise.any and AggregateError flag
        case ESPromiseAnyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESPromiseAny));

        // ES import.meta keyword meta-property
        case ESImportMetaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESImportMeta));

        //ES globalThis flag
        case ESGlobalThisFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ESGlobalThis));

        // This flag to be removed once JITing generator functions is stable
        case JitES6GeneratorsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&JitES6Generators));

        case FastLineColumnCalculationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FastLineColumnCalculation));
        case FilenameFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Filename));
        case FreeRejittedCodeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FreeRejittedCode));
        case ForceGuardPagesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGuardPages));
        case PrintGuardPageBoundsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintGuardPageBounds));
        case ForceLegacyEngineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceLegacyEngine));
        case ForceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Force));
        case StressFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Stress));
        case ForceArrayBTreeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceArrayBTree));
        case StrongArraySortFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&StrongArraySort));
        case ForceCleanPropertyOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceCleanPropertyOnCollect));
        case ForceCleanCacheOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceCleanCacheOnCollect));
        case ForceGCAfterJSONParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGCAfterJSONParse));
        case ForceDecommitOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDecommitOnCollect));
        case ForceDeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDeferParse));
        case ForceDiagnosticsModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDiagnosticsMode));
        case ForceGetWriteWatchOOMFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceGetWriteWatchOOM));
        case ForcePostLowerGlobOptInstrStringFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForcePostLowerGlobOptInstrString));
        case ForceSplitScopeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSplitScope));
        case EnumerateSpecialPropertiesInDebuggerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnumerateSpecialPropertiesInDebugger));
        case EnableContinueAfterExceptionWrappersForHelpersFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableContinueAfterExceptionWrappersForHelpers));
        case EnableContinueAfterExceptionWrappersForBuiltInsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableContinueAfterExceptionWrappersForBuiltIns));
        case EnableFunctionSourceReportForHeapEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableFunctionSourceReportForHeapEnum));
        case ForceFragmentAddressSpaceFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceFragmentAddressSpace));
        case ForceOOMOnEBCommitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceOOMOnEBCommit));
        case ForceDynamicProfileFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceDynamicProfile));
        case ForceES5ArrayFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceES5Array));
        case ForceAsmJsLinkFailFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceAsmJsLinkFail));
        case ForceExpireOnNonCacheCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceExpireOnNonCacheCollect));
        case ForceFastPathFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceFastPath));
        case ForceFloatPrefFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceFloatPref));
        case ForceJITLoopBodyFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceJITLoopBody));
        case ForceStaticInterpreterThunkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStaticInterpreterThunk));
        case DumpCommentsFromReferencedFilesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpCommentsFromReferencedFiles));
        case DelayFullJITSmallFuncFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DelayFullJITSmallFunc));
        case EnableFatalErrorOnOOMFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableFatalErrorOnOOM));

        #if defined(_M_ARM32_OR_ARM64)
        case ForceLocalsPtrFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceLocalsPtr));
        #endif
        case DeferLoadingAvailableSourceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DeferLoadingAvailableSource));
        case ForceNativeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceNative));
        case ForceSerializedFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSerialized));
        case ForceSerializedBytecodeMajorVersionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceSerializedBytecodeMajorVersion));
        case ForceSerializedBytecodeVersionSchemaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ForceSerializedBytecodeVersionSchema));
        case ForceStrictModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStrictMode));
        case ForceUndoDeferFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceUndoDefer));
        case ForceBlockingConcurrentCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceBlockingConcurrentCollect));
        case FreTestDiagModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FreTestDiagMode));
        #ifdef BYTECODE_TESTING
        case ByteCodeBranchLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ByteCodeBranchLimit));
        case MediumByteCodeLayoutFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MediumByteCodeLayout));
        case LargeByteCodeLayoutFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LargeByteCodeLayout));
        #endif
        case InduceCodeGenFailureFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InduceCodeGenFailure));
        case InduceCodeGenFailureSeedFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InduceCodeGenFailureSeed));
        case InjectPartiallyInitializedInterpreterFrameErrorFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InjectPartiallyInitializedInterpreterFrameError));
        case InjectPartiallyInitializedInterpreterFrameErrorTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InjectPartiallyInitializedInterpreterFrameErrorType));
        case GenerateByteCodeBufferReturnsCantGenerateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&GenerateByteCodeBufferReturnsCantGenerate));
        case GoptCleanupThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&GoptCleanupThreshold));
        case AsmGoptCleanupThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AsmGoptCleanupThreshold));
        case HighPrecisionDateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&HighPrecisionDate));
        case InlineCountMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCountMax));
        case InlineCountMaxInLoopBodiesFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCountMaxInLoopBodies));
        case icminlbFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&icminlb));
        case InlineInLoopBodyScaleDownFactorFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineInLoopBodyScaleDownFactor));
        case iilbsdfFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&iilbsdf));
        case InlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThreshold));
        case AggressiveInlineCountMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AggressiveInlineCountMax));
        case AggressiveInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AggressiveInlineThreshold));
        case InlineThresholdAdjustCountInLargeFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInLargeFunction));
        case InlineThresholdAdjustCountInMediumSizedFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInMediumSizedFunction));
        case InlineThresholdAdjustCountInSmallFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineThresholdAdjustCountInSmallFunction));
        case AsmJsInlineAdjustFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AsmJsInlineAdjust));
        case InterpretFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Interpret));
        case InstrumentFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Instrument));
        case JitQueueThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JitQueueThreshold));
        #ifdef LEAK_REPORT
        case LeakReportFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&LeakReport));
        #endif
        case LoopInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopInlineThreshold));
        case LeafInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LeafInlineThreshold));
        case ConstantArgumentInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstantArgumentInlineThreshold));
        case RecursiveInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineThreshold));
        case RecursiveInlineDepthMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineDepthMax));
        case RecursiveInlineDepthMinFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecursiveInlineDepthMin));
        case RedeferralCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RedeferralCap));
        case LoopFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Loop));
        case LoopInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopInterpretCount));
        case licFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&lic));
        case LoopProfileIterationsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopProfileIterations));
        case OutsideLoopInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&OutsideLoopInlineThreshold));
        case MaxFuncInlineDepthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxFuncInlineDepth));
        case MaxNumberOfInlineesWithLoopFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxNumberOfInlineesWithLoop));
        #ifdef MEMSPECT_TRACKING
        case MemspectFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Memspect));
        #endif
        case PolymorphicInlineThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PolymorphicInlineThreshold));
        case PrimeRecyclerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrimeRecycler));
        case TraceEngineRefcountFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceEngineRefcount));
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        case LeakStackTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LeakStackTrace));
        case ForceMemoryLeakFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceMemoryLeak));
        #endif
        case DumpAfterFinalGCFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpAfterFinalGC));
        case ForceOldDateAPIFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceOldDateAPI));

        case JitLoopBodyHotLoopThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JitLoopBodyHotLoopThreshold));
        case LoopBodySizeThresholdToDisableOptsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopBodySizeThresholdToDisableOpts));

        case MaxJitThreadCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJitThreadCount));
        case ForceMaxJitThreadCountFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceMaxJitThreadCount));

        case MitigateSpectreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MitigateSpectre));

        case AddMaskingBlocksFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&AddMaskingBlocks));

        case PoisonVarArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonVarArrayLoad));
        case PoisonIntArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonIntArrayLoad));
        case PoisonFloatArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonFloatArrayLoad));
        case PoisonTypedArrayLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonTypedArrayLoad));
        case PoisonStringLoadFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonStringLoad));
        case PoisonObjectsForLoadsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonObjectsForLoads));

        case PoisonVarArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonVarArrayStore));
        case PoisonIntArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonIntArrayStore));
        case PoisonFloatArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonFloatArrayStore));
        case PoisonTypedArrayStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonTypedArrayStore));
        case PoisonStringStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonStringStore));
        case PoisonObjectsForStoresFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PoisonObjectsForStores));

        case MinInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinInterpretCount));
        case MinSimpleJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinSimpleJitRunCount));
        case MaxInterpretCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxInterpretCount));
        case MicFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mic));
        case MaxSimpleJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSimpleJitRunCount));
        case MsjrcFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Msjrc));
        case MinMemOpCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinMemOpCount));
        case MmocFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mmoc));

        #if ENABLE_COPYONACCESS_ARRAY
        case MaxCopyOnAccessArrayLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxCopyOnAccessArrayLength));
        case MinCopyOnAccessArrayLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinCopyOnAccessArrayLength));
        case CopyOnAccessArraySegmentCacheSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&CopyOnAccessArraySegmentCacheSize));
        #endif

        case MinTemplatizedJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinTemplatizedJitRunCount));
        case MinAsmJsInterpreterRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinAsmJsInterpreterRunCount));

        case MinTemplatizedJitLoopRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinTemplatizedJitLoopRunCount));
        case MaxTemplatizedJitRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxTemplatizedJitRunCount));
        case MtjrcFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Mtjrc));
        case MaxAsmJsInterpreterRunCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxAsmJsInterpreterRunCount));
        case MaicFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Maic));

        case AutoProfilingInterpreter0LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AutoProfilingInterpreter0Limit));
        case ProfilingInterpreter0LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfilingInterpreter0Limit));
        case AutoProfilingInterpreter1LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AutoProfilingInterpreter1Limit));
        case SimpleJitLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimpleJitLimit));
        case ProfilingInterpreter1LimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfilingInterpreter1Limit));

        case ExecutionModeLimitsFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&ExecutionModeLimits));
        case EmlFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&Eml));
        case EnforceExecutionModeLimitsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnforceExecutionModeLimits));
        case EemlFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Eeml));

        case SimpleJitAfterFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimpleJitAfter));
        case SjaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Sja));
        case FullJitAfterFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&FullJitAfter));
        case FjaFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Fja));

        case NewSimpleJitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NewSimpleJit));

        case MaxLinearIntCaseCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLinearIntCaseCount));
        case MaxSingleCharStrJumpTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleCharStrJumpTableSize));
        case MaxSingleCharStrJumpTableRatioFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleCharStrJumpTableRatio));
        case MinSwitchJumpTableSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinSwitchJumpTableSize));
        case MaxLinearStringCaseCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLinearStringCaseCount));
        case MinDeferredFuncTokenCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinDeferredFuncTokenCount));
        #if DBG
        case SkipFuncCountForBailOnNoProfileFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SkipFuncCountForBailOnNoProfile));
        #endif
        case MaxJITFunctionBytecodeByteLengthFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJITFunctionBytecodeByteLength));
        case MaxJITFunctionBytecodeCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxJITFunctionBytecodeCount));
        case MaxLoopsPerFunctionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxLoopsPerFunction));
        case FuncObjectInlineCacheThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&FuncObjectInlineCacheThreshold));
        case NoDeferParseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoDeferParse));
        case NoLogoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoLogo));
        case OOPJITMissingOptsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPJITMissingOpts));
        case CrashOnOOPJITFailureFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&CrashOnOOPJITFailure));
        case OOPCFGRegistrationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OOPCFGRegistration));
        case ForceJITCFGCheckFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceJITCFGCheck));
        case UseJITTrampolineFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&UseJITTrampoline));
        case NoNativeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoNative));
        case NopFrequencyFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NopFrequency));
        case NoStrictModeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NoStrictMode));
        case NormalizeStatsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&NormalizeStats));
        case OffFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Off));
        case OffProfiledByteCodeFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&OffProfiledByteCode));
        case OnFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&On));
        case OutputFileFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&OutputFile));
        case OutputFileOpenModeFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&OutputFileOpenMode));
        #ifdef ENABLE_TRACE
        case InMemoryTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&InMemoryTrace));
        case InMemoryTraceBufferSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InMemoryTraceBufferSize));
        #ifdef STACK_BACK_TRACE
        case TraceWithStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWithStack));
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        case PrintRunTimeDataCollectionTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintRunTimeDataCollectionTrace));
        #ifdef ENABLE_PREJIT
        case PrejitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Prejit));
        #endif
        case PrintSrcInDumpFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintSrcInDump));
        #if PROFILE_DICTIONARY
        case ProfileDictionaryFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileDictionary));
        #endif
        #ifdef PROFILE_EXEC
        case ProfileFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Profile));
        case ProfileThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ProfileThreshold));
        #endif
        #ifdef PROFILE_OBJECT_LITERALS
        case ProfileObjectLiteralFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileObjectLiteral));
        #endif
        #ifdef PROFILE_MEM
        case ProfileMemoryFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&ProfileMemory));
        #endif
        #ifdef PROFILE_STRINGS
        case ProfileStringsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileStrings));
        #endif
        #ifdef PROFILE_TYPES
        case ProfileTypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileTypes));
        #endif
        #ifdef PROFILE_EVALMAP
        case ProfileEvalMapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileEvalMap));
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        case ProfileBailOutRecordMemoryFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ProfileBailOutRecordMemory));
        #endif

        #if DBG
        case ValidateIntRangesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateIntRanges));
        #endif
        case RejitMaxBailOutCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RejitMaxBailOutCount));
        case CallsToBailoutsRatioForRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&CallsToBailoutsRatioForRejit));
        case LoopIterationsToBailoutsRatioForRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopIterationsToBailoutsRatioForRejit));
        case MinBailOutsBeforeRejitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBailOutsBeforeRejit));
        case MinBailOutsBeforeRejitForLoopsFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBailOutsBeforeRejitForLoops));
        case LibraryStackFrameFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LibraryStackFrame));
        case LibraryStackFrameDebuggerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&LibraryStackFrameDebugger));
        #ifdef RECYCLER_STRESS
        case RecyclerStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerStress));
        case RecyclerBackgroundStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerBackgroundStress));
        case RecyclerConcurrentStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerConcurrentStress));
        case RecyclerConcurrentRepeatStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerConcurrentRepeatStress));
        case RecyclerPartialStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerPartialStress));
        case RecyclerTrackStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerTrackStress));
        case RecyclerInduceFalsePositivesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerInduceFalsePositives));
        #endif // RECYCLER_STRESS
        case RecyclerForceMarkInteriorFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerForceMarkInterior));
        case RecyclerPriorityBoostTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerPriorityBoostTimeout));
        case RecyclerThreadCollectTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerThreadCollectTimeout));
        case EnableConcurrentSweepAllocFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableConcurrentSweepAlloc));
        case ecsaFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ecsa));
        case PageHeapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PageHeap));
        case PageHeapAllocStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapAllocStack));
        case PageHeapFreeStackFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapFreeStack));
        case PageHeapBucketNumberFlag:
            return reinterpret_cast<void*>(const_cast<NumberRange*>(&PageHeapBucketNumber));
        case PageHeapBlockTypeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PageHeapBlockType));
        case PageHeapDecommitGuardPageFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PageHeapDecommitGuardPage));
        #ifdef RECYCLER_NO_PAGE_REUSE
        case RecyclerNoPageReuseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerNoPageReuse));
        #endif
        #ifdef RECYCLER_MEMORY_VERIFY
        case RecyclerVerifyFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&RecyclerVerify));
        case RecyclerVerifyPadSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&RecyclerVerifyPadSize));
        #endif
        case RecyclerTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerTest));
        case RecyclerProtectPagesOnRescanFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerProtectPagesOnRescan));
        #ifdef RECYCLER_VERIFY_MARK
        case RecyclerVerifyMarkFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RecyclerVerifyMark));
        #endif
        case LowMemoryCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LowMemoryCap));
        case NewPagesCapDuringBGSweepingFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NewPagesCapDuringBGSweeping));
        case AllocPolicyLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&AllocPolicyLimit));
        case RuntimeDataOutputFileFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&RuntimeDataOutputFile));
        case SpeculationCapFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SpeculationCap));
        #if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
        case StatsFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Stats));
        #endif
        #if EXCEPTION_RECOVERY
        case SwallowExceptionsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&SwallowExceptions));
        #endif
        case PrintSystemExceptionFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintSystemException));
        case SwitchOptHolesThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SwitchOptHolesThreshold));
        case TempMinFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TempMin));
        case TempMaxFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TempMax));
        case TraceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&Trace));

        #if defined(_M_X64)
        case LoopAlignNopLimitFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&LoopAlignNopLimit));
        #endif

        #ifdef PROFILE_MEM
        case TraceMemoryFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&TraceMemory));
        #endif
        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        case TraceMetaDataParsingFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&TraceMetaDataParsing));
        case TraceWin8AllocationsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWin8Allocations));
        case TraceWin8DeallocationsImmediateFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceWin8DeallocationsImmediate));
        case PrintWin8StatsDetailedFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&PrintWin8StatsDetailed));
        case TraceProtectPagesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceProtectPages));
        #endif
        case TraceAsyncDebugCallsFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceAsyncDebugCalls));
        #ifdef TRACK_DISPATCH
        case TrackDispatchFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TrackDispatch));
        #endif
        case VerboseFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Verbose));
        case UseFullNameFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&UseFullName));
        case Utf8Flag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&Utf8));
        case VersionFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Version));
        case WERExceptionSupportFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WERExceptionSupport));
        case ExtendedErrorStackForTestHostFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ExtendedErrorStackForTestHost));
        case errorStackTraceFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&errorStackTrace));
        case DoHeapEnumOnEngineShutdownFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DoHeapEnumOnEngineShutdown));
        #ifdef HEAP_ENUMERATION_VALIDATION
        case ValidateHeapEnumFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ValidateHeapEnum));
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        case RegexTracingFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexTracing));
        case RegexProfileFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexProfile));
        case RegexDebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebug));
        case RegexDebugASTFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebugAST));
        case RegexDebugAnnotatedASTFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexDebugAnnotatedAST));
        case RegexBytecodeDebugFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexBytecodeDebug));
        case RegexOptimizeFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&RegexOptimize));
        case DynamicRegexMruListSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DynamicRegexMruListSize));
        #endif

        case OptimizeForManyInstancesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&OptimizeForManyInstances));
        case EnableArrayTypeMutationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableArrayTypeMutation));
        case ArrayMutationTestSeedFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ArrayMutationTestSeed));
        case TestTraceFlag:
            return reinterpret_cast<void*>(const_cast<Phases*>(&TestTrace));
        case EnableEvalMapCleanupFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableEvalMapCleanup));
        #ifdef PROFILE_MEM
        case TraceObjectAllocationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TraceObjectAllocation));
        #endif
        case SseFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&Sse));
        case DeletedPropertyReuseThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&DeletedPropertyReuseThreshold));
        case ForceStringKeyedSimpleDictionaryTypeHandlerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceStringKeyedSimpleDictionaryTypeHandler));
        case BigDictionaryTypeHandlerThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BigDictionaryTypeHandlerThreshold));
        case TypeSnapshotEnumerationFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&TypeSnapshotEnumeration));
        case IsolatePrototypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IsolatePrototypes));
        case ChangeTypeOnProtoFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ChangeTypeOnProto));
        case ShareInlineCachesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ShareInlineCaches));
        case DisableDebugObjectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DisableDebugObject));
        case DumpHeapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&DumpHeap));
        case autoProxyFlag:
            return reinterpret_cast<void*>(const_cast<String*>(&autoProxy));
        case PerfHintLevelFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PerfHintLevel));
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        case MemProtectHeapFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeap));
        #endif
        #ifdef RECYCLER_STRESS
        case MemProtectHeapStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapStress));
        case MemProtectHeapBackgroundStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapBackgroundStress));
        case MemProtectHeapConcurrentStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapConcurrentStress));
        case MemProtectHeapConcurrentRepeatStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapConcurrentRepeatStress));
        case MemProtectHeapPartialStressFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&MemProtectHeapPartialStress));
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        case FixPropsOnPathTypesFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&FixPropsOnPathTypes));
        #endif
        case BailoutTraceFilterFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&BailoutTraceFilter));
        case RejitTraceFilterFlag:
            return reinterpret_cast<void*>(const_cast<NumberSet*>(&RejitTraceFilter));

        // recycler heuristic flags
        case MaxBackgroundFinishMarkCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxBackgroundFinishMarkCount));
        case BackgroundFinishMarkWaitTimeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&BackgroundFinishMarkWaitTime));
        case MinBackgroundRepeatMarkRescanBytesFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MinBackgroundRepeatMarkRescanBytes));

        #if defined(_M_X64)
        case ZeroMemoryWithNonTemporalStoreFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ZeroMemoryWithNonTemporalStore));
        #endif

        // recycler memory restrict test flags
        case MaxMarkStackPageCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxMarkStackPageCount));
        case MaxTrackedObjectListCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxTrackedObjectListCount));

        // make the recycler page integration path easier to hit
        case NumberAllocPlusSizeFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&NumberAllocPlusSize));

        #if DBG
        case InitializeInterpreterSlotsWithInvalidStackVarFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&InitializeInterpreterSlotsWithInvalidStackVar));
        #endif

        #if DBG
        case PRNGSeed0Flag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PRNGSeed0));
        case PRNGSeed1Flag:
            return reinterpret_cast<void*>(const_cast<Number*>(&PRNGSeed1));
        #endif

        case ClearInlineCachesOnCollectFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ClearInlineCachesOnCollect));
        case InlineCacheInvalidationListCompactionThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&InlineCacheInvalidationListCompactionThreshold));
        case ConstructorCacheInvalidationThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&ConstructorCacheInvalidationThreshold));

        #ifdef IR_VIEWER
        case IRViewerFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&IRViewer));
        #endif /* IR_VIEWER */

        case GCMemoryThresholdFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&GCMemoryThreshold));

        #if DBG
            case SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&SimulatePolyCacheWithOneTypeForInlineCacheIndex));
        #endif

        case JITServerIdleTimeoutFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JITServerIdleTimeout));
        case JITServerMaxInactivePageAllocatorCountFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&JITServerMaxInactivePageAllocatorCount));

        case StrictWriteBarrierCheckFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&StrictWriteBarrierCheck));
        case WriteBarrierTestFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&WriteBarrierTest));
        case ForceSoftwareWriteBarrierFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&ForceSoftwareWriteBarrier));
        case VerifyBarrierBitFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&VerifyBarrierBit));
        case EnableBGFreeZeroFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&EnableBGFreeZero));
        case KeepRecyclerTrackDataFlag:
            return reinterpret_cast<void*>(const_cast<Boolean*>(&KeepRecyclerTrackData));

        case MaxSingleAllocSizeInMBFlag:
            return reinterpret_cast<void*>(const_cast<Number*>(&MaxSingleAllocSizeInMB));
        default:
            return NULL;
        }
    }


    void
    ConfigFlagsTable::VerboseDump()
    {

//  types:
//      String
//      Phases
//      Number
//      Boolean
//
// If the default value is not required it should be left empty
// For Phases, there is no default value. it should always be empty
// Default values for stings must be prefixed with 'L'. See AsmDumpMode
// Scroll till the extreme right to see the default values

#if DBG
if (IsEnabled(ArrayValidateFlag))
{
    Output::Print(u"-%s", u"ArrayValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArrayValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArrayValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArrayValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArrayValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemOpMissingValueValidateFlag))
{
    Output::Print(u"-%s", u"MemOpMissingValueValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemOpMissingValueValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemOpMissingValueValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemOpMissingValueValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemOpMissingValueValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPJITFixupValidateFlag))
{
    Output::Print(u"-%s", u"OOPJITFixupValidate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPJITFixupValidateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPJITFixupValidateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPJITFixupValidateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPJITFixupValidateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef ARENA_MEMORY_VERIFY
if (IsEnabled(ArenaNoFreeListFlag))
{
    Output::Print(u"-%s", u"ArenaNoFreeList");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaNoFreeListFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaNoFreeListFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaNoFreeListFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaNoFreeListFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArenaNoPageReuseFlag))
{
    Output::Print(u"-%s", u"ArenaNoPageReuse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaNoPageReuseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaNoPageReuseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaNoPageReuseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaNoPageReuseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArenaUseHeapAllocFlag))
{
    Output::Print(u"-%s", u"ArenaUseHeapAlloc");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArenaUseHeapAllocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArenaUseHeapAllocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArenaUseHeapAllocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArenaUseHeapAllocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(ValidateInlineStackFlag))
{
    Output::Print(u"-%s", u"ValidateInlineStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateInlineStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateInlineStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateInlineStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateInlineStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmDiffFlag))
{
    Output::Print(u"-%s", u"AsmDiff");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmDiffFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmDiffFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmDiffFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmDiffFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmDumpModeFlag))
{
    Output::Print(u"-%s", u"AsmDumpMode");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmDumpModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmDumpModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmDumpModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmDumpModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsFlag))
{
    Output::Print(u"-%s", u"AsmJs");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsStopOnErrorFlag))
{
    Output::Print(u"-%s", u"AsmJsStopOnError");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsStopOnErrorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsStopOnErrorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsStopOnErrorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsStopOnErrorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsEdgeFlag))
{
    Output::Print(u"-%s", u"AsmJsEdge");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsEdgeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsEdgeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsEdgeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsEdgeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFlag))
{
    Output::Print(u"-%s", u"Wasm");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmI64Flag))
{
    Output::Print(u"-%s", u"WasmI64");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmI64Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmI64Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmI64Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmI64Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFastArrayFlag))
{
    Output::Print(u"-%s", u"WasmFastArray");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFastArrayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFastArrayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFastArrayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFastArrayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmSharedArrayVirtualBufferFlag))
{
    Output::Print(u"-%s", u"WasmSharedArrayVirtualBuffer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSharedArrayVirtualBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSharedArrayVirtualBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSharedArrayVirtualBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSharedArrayVirtualBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMathExFilterFlag))
{
    Output::Print(u"-%s", u"WasmMathExFilter");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMathExFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMathExFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMathExFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMathExFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmCheckVersionFlag))
{
    Output::Print(u"-%s", u"WasmCheckVersion");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmCheckVersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmCheckVersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmCheckVersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmCheckVersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmAssignModuleIDFlag))
{
    Output::Print(u"-%s", u"WasmAssignModuleID");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmAssignModuleIDFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmAssignModuleIDFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmAssignModuleIDFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmAssignModuleIDFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmIgnoreLimitsFlag))
{
    Output::Print(u"-%s", u"WasmIgnoreLimits");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmIgnoreLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmIgnoreLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmIgnoreLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmIgnoreLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmFoldFlag))
{
    Output::Print(u"-%s", u"WasmFold");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmFoldFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmFoldFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmFoldFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmFoldFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmIgnoreResponseFlag))
{
    Output::Print(u"-%s", u"WasmIgnoreResponse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmIgnoreResponseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmIgnoreResponseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmIgnoreResponseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmIgnoreResponseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMaxTableSizeFlag))
{
    Output::Print(u"-%s", u"WasmMaxTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMaxTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMaxTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMaxTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMaxTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmThreadsFlag))
{
    Output::Print(u"-%s", u"WasmThreads");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmThreadsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmThreadsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmThreadsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmThreadsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmMultiValueFlag))
{
    Output::Print(u"-%s", u"WasmMultiValue");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmMultiValueFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmMultiValueFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmMultiValueFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmMultiValueFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmSignExtendsFlag))
{
    Output::Print(u"-%s", u"WasmSignExtends");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSignExtendsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSignExtendsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSignExtendsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSignExtendsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WasmNontrappingFlag))
{
    Output::Print(u"-%s", u"WasmNontrapping");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmNontrappingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmNontrappingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmNontrappingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmNontrappingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
if (IsEnabled(WasmExperimentalFlag))
{
    Output::Print(u"-%s", u"WasmExperimental");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmExperimentalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmExperimentalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmExperimentalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmExperimentalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
if (IsEnabled(WasmSimdFlag))
{
    Output::Print(u"-%s", u"WasmSimd");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WasmSimdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WasmSimdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WasmSimdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WasmSimdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AssertBreakFlag))
{
    Output::Print(u"-%s", u"AssertBreak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AssertPopUpFlag))
{
    Output::Print(u"-%s", u"AssertPopUp");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertPopUpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertPopUpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertPopUpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertPopUpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AssertIgnoreFlag))
{
    Output::Print(u"-%s", u"AssertIgnore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AssertIgnoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AssertIgnoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AssertIgnoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AssertIgnoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsyncDebuggingFlag))
{
    Output::Print(u"-%s", u"AsyncDebugging");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsyncDebuggingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsyncDebuggingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsyncDebuggingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsyncDebuggingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOnNoProfileLimitFlag))
{
    Output::Print(u"-%s", u"BailOnNoProfileLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOnNoProfileLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOnNoProfileLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOnNoProfileLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOnNoProfileLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOnNoProfileRejitLimitFlag))
{
    Output::Print(u"-%s", u"BailOnNoProfileRejitLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOnNoProfileRejitLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOnNoProfileRejitLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOnNoProfileRejitLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOnNoProfileRejitLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BaselineModeFlag))
{
    Output::Print(u"-%s", u"BaselineMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BaselineModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BaselineModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BaselineModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BaselineModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpOnCrashFlag))
{
    Output::Print(u"-%s", u"DumpOnCrash");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpOnCrashFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpOnCrashFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpOnCrashFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpOnCrashFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FullMemoryDumpFlag))
{
    Output::Print(u"-%s", u"FullMemoryDump");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FullMemoryDumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FullMemoryDumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FullMemoryDumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FullMemoryDumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutFlag))
{
    Output::Print(u"-%s", u"BailOut");
    switch (FlagNumberPairSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryLineFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryLine");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryLineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryLineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryLineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryLineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryByteCodeFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutAtEveryImplicitCallFlag))
{
    Output::Print(u"-%s", u"BailOutAtEveryImplicitCall");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutAtEveryImplicitCallFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutAtEveryImplicitCallFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutAtEveryImplicitCallFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutAtEveryImplicitCallFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BailOutByteCodeFlag))
{
    Output::Print(u"-%s", u"BailOutByteCode");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailOutByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailOutByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailOutByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailOutByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BenchmarkFlag))
{
    Output::Print(u"-%s", u"Benchmark");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BenchmarkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BenchmarkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BenchmarkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BenchmarkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitFlag))
{
    Output::Print(u"-%s", u"BgJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgParseFlag))
{
    Output::Print(u"-%s", u"BgParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitDelayFlag))
{
    Output::Print(u"-%s", u"BgJitDelay");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitDelayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitDelayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitDelayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitDelayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitDelayFgBufferFlag))
{
    Output::Print(u"-%s", u"BgJitDelayFgBuffer");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitDelayFgBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitDelayFgBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitDelayFgBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitDelayFgBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BgJitPendingFuncCapFlag))
{
    Output::Print(u"-%s", u"BgJitPendingFuncCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BgJitPendingFuncCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BgJitPendingFuncCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BgJitPendingFuncCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BgJitPendingFuncCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(CreateFunctionProxyFlag))
{
    Output::Print(u"-%s", u"CreateFunctionProxy");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CreateFunctionProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CreateFunctionProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CreateFunctionProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CreateFunctionProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HybridFgJitFlag))
{
    Output::Print(u"-%s", u"HybridFgJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HybridFgJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HybridFgJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HybridFgJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HybridFgJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HybridFgJitBgQueueLengthThresholdFlag))
{
    Output::Print(u"-%s", u"HybridFgJitBgQueueLengthThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HybridFgJitBgQueueLengthThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HybridFgJitBgQueueLengthThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HybridFgJitBgQueueLengthThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HybridFgJitBgQueueLengthThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BytecodeHistFlag))
{
    Output::Print(u"-%s", u"BytecodeHist");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BytecodeHistFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BytecodeHistFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BytecodeHistFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BytecodeHistFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CurrentSourceInfoFlag))
{
    Output::Print(u"-%s", u"CurrentSourceInfo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CurrentSourceInfoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CurrentSourceInfoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CurrentSourceInfoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CurrentSourceInfoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CFGLogFlag))
{
    Output::Print(u"-%s", u"CFGLog");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CFGLogFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CFGLogFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CFGLogFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CFGLogFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CheckAlignmentFlag))
{
    Output::Print(u"-%s", u"CheckAlignment");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckAlignmentFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckAlignmentFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckAlignmentFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckAlignmentFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CheckEmitBufferPermissionsFlag))
{
    Output::Print(u"-%s", u"CheckEmitBufferPermissions");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckEmitBufferPermissionsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckEmitBufferPermissionsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckEmitBufferPermissionsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckEmitBufferPermissionsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef CHECK_MEMORY_LEAK
if (IsEnabled(CheckMemoryLeakFlag))
{
    Output::Print(u"-%s", u"CheckMemoryLeak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckMemoryLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckMemoryLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckMemoryLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckMemoryLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpOnLeakFlag))
{
    Output::Print(u"-%s", u"DumpOnLeak");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpOnLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpOnLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpOnLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpOnLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(CheckOpHelpersFlag))
{
    Output::Print(u"-%s", u"CheckOpHelpers");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CheckOpHelpersFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CheckOpHelpersFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CheckOpHelpersFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CheckOpHelpersFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CloneInlinedPolymorphicCachesFlag))
{
    Output::Print(u"-%s", u"CloneInlinedPolymorphicCaches");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CloneInlinedPolymorphicCachesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CloneInlinedPolymorphicCachesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CloneInlinedPolymorphicCachesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CloneInlinedPolymorphicCachesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConcurrentRuntimeFlag))
{
    Output::Print(u"-%s", u"ConcurrentRuntime");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConcurrentRuntimeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConcurrentRuntimeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConcurrentRuntimeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConcurrentRuntimeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorInlineThresholdFlag))
{
    Output::Print(u"-%s", u"ConstructorInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorCallsRequiredToFinalizeCachedTypeFlag))
{
    Output::Print(u"-%s", u"ConstructorCallsRequiredToFinalizeCachedType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorCallsRequiredToFinalizeCachedTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorCallsRequiredToFinalizeCachedTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorCallsRequiredToFinalizeCachedTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorCallsRequiredToFinalizeCachedTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissPenaltyFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissPenalty");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissPenaltyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissPenaltyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissPenaltyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissPenaltyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissThresholdFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PropertyCacheMissResetFlag))
{
    Output::Print(u"-%s", u"PropertyCacheMissReset");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PropertyCacheMissResetFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PropertyCacheMissResetFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PropertyCacheMissResetFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PropertyCacheMissResetFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugFlag))
{
    Output::Print(u"-%s", u"Debug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugBreakFlag))
{
    Output::Print(u"-%s", u"DebugBreak");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StatementDebugBreakFlag))
{
    Output::Print(u"-%s", u"StatementDebugBreak");
    switch (FlagNumberTrioSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StatementDebugBreakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StatementDebugBreakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StatementDebugBreakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StatementDebugBreakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DebugBreakOnPhaseBeginFlag))
{
    Output::Print(u"-%s", u"DebugBreakOnPhaseBegin");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugBreakOnPhaseBeginFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugBreakOnPhaseBeginFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugBreakOnPhaseBeginFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugBreakOnPhaseBeginFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(DebugWindowFlag))
{
    Output::Print(u"-%s", u"DebugWindow");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DebugWindowFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DebugWindowFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DebugWindowFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DebugWindowFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ParserStateCacheFlag))
{
    Output::Print(u"-%s", u"ParserStateCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ParserStateCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ParserStateCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ParserStateCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ParserStateCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CompressParserStateCacheFlag))
{
    Output::Print(u"-%s", u"CompressParserStateCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CompressParserStateCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CompressParserStateCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CompressParserStateCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CompressParserStateCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeferTopLevelTillFirstCallFlag))
{
    Output::Print(u"-%s", u"DeferTopLevelTillFirstCall");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferTopLevelTillFirstCallFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferTopLevelTillFirstCallFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferTopLevelTillFirstCallFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferTopLevelTillFirstCallFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeferParseFlag))
{
    Output::Print(u"-%s", u"DeferParse");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DirectCallTelemetryStatsFlag))
{
    Output::Print(u"-%s", u"DirectCallTelemetryStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DirectCallTelemetryStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DirectCallTelemetryStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DirectCallTelemetryStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DirectCallTelemetryStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableArrayBTreeFlag))
{
    Output::Print(u"-%s", u"DisableArrayBTree");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableArrayBTreeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableArrayBTreeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableArrayBTreeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableArrayBTreeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableRentalThreadingFlag))
{
    Output::Print(u"-%s", u"DisableRentalThreading");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableRentalThreadingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableRentalThreadingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableRentalThreadingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableRentalThreadingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableVTuneSourceLineInfoFlag))
{
    Output::Print(u"-%s", u"DisableVTuneSourceLineInfo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableVTuneSourceLineInfoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableVTuneSourceLineInfoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableVTuneSourceLineInfoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableVTuneSourceLineInfoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisplayMemStatsFlag))
{
    Output::Print(u"-%s", u"DisplayMemStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisplayMemStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisplayMemStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisplayMemStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisplayMemStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpFlag))
{
    Output::Print(u"-%s", u"Dump");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef DUMP_FRAGMENTATION_STATS
if (IsEnabled(DumpFragmentationStatsFlag))
{
    Output::Print(u"-%s", u"DumpFragmentationStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpFragmentationStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpFragmentationStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpFragmentationStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpFragmentationStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpIRAddressesFlag))
{
    Output::Print(u"-%s", u"DumpIRAddresses");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpIRAddressesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpIRAddressesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpIRAddressesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpIRAddressesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpLineNoInColorFlag))
{
    Output::Print(u"-%s", u"DumpLineNoInColor");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpLineNoInColorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpLineNoInColorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpLineNoInColorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpLineNoInColorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
if (IsEnabled(DumpObjectGraphOnExitFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnExit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnExitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnExitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnExitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnExitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpObjectGraphOnCollectFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpEvalStringOnRemovalFlag))
{
    Output::Print(u"-%s", u"DumpEvalStringOnRemoval");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpEvalStringOnRemovalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpEvalStringOnRemovalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpEvalStringOnRemovalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpEvalStringOnRemovalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpObjectGraphOnEnumFlag))
{
    Output::Print(u"-%s", u"DumpObjectGraphOnEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpObjectGraphOnEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpObjectGraphOnEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpObjectGraphOnEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpObjectGraphOnEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef DYNAMIC_PROFILE_STORAGE
if (IsEnabled(DynamicProfileCacheFlag))
{
    Output::Print(u"-%s", u"DynamicProfileCache");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DpcFlag))
{
    Output::Print(u"-%s", u"Dpc");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DpcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DpcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DpcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DpcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicProfileCacheDirFlag))
{
    Output::Print(u"-%s", u"DynamicProfileCacheDir");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileCacheDirFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileCacheDirFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileCacheDirFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileCacheDirFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicProfileInputFlag))
{
    Output::Print(u"-%s", u"DynamicProfileInput");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicProfileInputFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicProfileInputFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicProfileInputFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicProfileInputFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DpiFlag))
{
    Output::Print(u"-%s", u"Dpi");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DpiFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DpiFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DpiFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DpiFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef EDIT_AND_CONTINUE
if (IsEnabled(EditTestFlag))
{
    Output::Print(u"-%s", u"EditTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EditTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EditTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EditTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EditTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(WininetProfileCacheFlag))
{
    Output::Print(u"-%s", u"WininetProfileCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WininetProfileCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WininetProfileCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WininetProfileCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WininetProfileCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoDynamicProfileInMemoryCacheFlag))
{
    Output::Print(u"-%s", u"NoDynamicProfileInMemoryCache");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoDynamicProfileInMemoryCacheFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoDynamicProfileInMemoryCacheFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoDynamicProfileInMemoryCacheFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoDynamicProfileInMemoryCacheFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileBasedSpeculativeJitFlag))
{
    Output::Print(u"-%s", u"ProfileBasedSpeculativeJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBasedSpeculativeJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBasedSpeculativeJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBasedSpeculativeJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBasedSpeculativeJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileBasedSpeculationCapFlag))
{
    Output::Print(u"-%s", u"ProfileBasedSpeculationCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBasedSpeculationCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBasedSpeculationCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBasedSpeculationCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBasedSpeculationCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag))
{
    Output::Print(u"-%s", u"ExecuteByteCodeBufferReturnsInvalidByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExecuteByteCodeBufferReturnsInvalidByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExpirableCollectionGCCountFlag))
{
    Output::Print(u"-%s", u"ExpirableCollectionGCCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExpirableCollectionGCCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExpirableCollectionGCCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExpirableCollectionGCCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExpirableCollectionGCCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExpirableCollectionTriggerThresholdFlag))
{
    Output::Print(u"-%s", u"ExpirableCollectionTriggerThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExpirableCollectionTriggerThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExpirableCollectionTriggerThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExpirableCollectionTriggerThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExpirableCollectionTriggerThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SkipSplitOnNoResultFlag))
{
    Output::Print(u"-%s", u"SkipSplitOnNoResult");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SkipSplitOnNoResultFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SkipSplitOnNoResultFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SkipSplitOnNoResultFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SkipSplitOnNoResultFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(Force32BitByteCodeFlag))
{
    Output::Print(u"-%s", u"Force32BitByteCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(Force32BitByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(Force32BitByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(Force32BitByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(Force32BitByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(CollectGarbageFlag))
{
    Output::Print(u"-%s", u"CollectGarbage");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CollectGarbageFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CollectGarbageFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CollectGarbageFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CollectGarbageFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(IntlFlag))
{
    Output::Print(u"-%s", u"Intl");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IntlBuiltInsFlag))
{
    Output::Print(u"-%s", u"IntlBuiltIns");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlBuiltInsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlBuiltInsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlBuiltInsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlBuiltInsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IntlPlatformFlag))
{
    Output::Print(u"-%s", u"IntlPlatform");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IntlPlatformFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IntlPlatformFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IntlPlatformFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IntlPlatformFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(JsBuiltInFlag))
{
    Output::Print(u"-%s", u"JsBuiltIn");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JsBuiltInFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JsBuiltInFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JsBuiltInFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JsBuiltInFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JitReproFlag))
{
    Output::Print(u"-%s", u"JitRepro");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitReproFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitReproFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitReproFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitReproFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EntryPointInfoRpcDataFlag))
{
    Output::Print(u"-%s", u"EntryPointInfoRpcData");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EntryPointInfoRpcDataFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EntryPointInfoRpcDataFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EntryPointInfoRpcDataFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EntryPointInfoRpcDataFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(LdChakraLibFlag))
{
    Output::Print(u"-%s", u"LdChakraLib");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LdChakraLibFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LdChakraLibFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LdChakraLibFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LdChakraLibFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TestChakraLibFlag))
{
    Output::Print(u"-%s", u"TestChakraLib");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TestChakraLibFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TestChakraLibFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TestChakraLibFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TestChakraLibFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
if (IsEnabled(ES6Flag))
{
    Output::Print(u"-%s", u"ES6");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Master ES6 flag to enable ALL sub ES6 features/flags
if (IsEnabled(ES6AllFlag))
{
    Output::Print(u"-%s", u"ES6All");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6AllFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6AllFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6AllFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6AllFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Master ES6 flag to enable Threshold ES6 features/flags
if (IsEnabled(ES6ExperimentalFlag))
{
    Output::Print(u"-%s", u"ES6Experimental");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ExperimentalFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ExperimentalFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ExperimentalFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ExperimentalFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Per ES6 feature/flag

if (IsEnabled(ES7AsyncAwaitFlag))
{
    Output::Print(u"-%s", u"ES7AsyncAwait");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7AsyncAwaitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7AsyncAwaitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7AsyncAwaitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7AsyncAwaitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6DateParseFixFlag))
{
    Output::Print(u"-%s", u"ES6DateParseFix");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6DateParseFixFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6DateParseFixFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6DateParseFixFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6DateParseFixFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6FunctionNameFullFlag))
{
    Output::Print(u"-%s", u"ES6FunctionNameFull");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6FunctionNameFullFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6FunctionNameFullFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6FunctionNameFullFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6FunctionNameFullFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6GeneratorsFlag))
{
    Output::Print(u"-%s", u"ES6Generators");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6GeneratorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6GeneratorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6GeneratorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6GeneratorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES7ExponentiationOperatorFlag))
{
    Output::Print(u"-%s", u"ES7ExponentiationOperator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7ExponentiationOperatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7ExponentiationOperatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7ExponentiationOperatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7ExponentiationOperatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES7ValuesEntriesFlag))
{
    Output::Print(u"-%s", u"ES7ValuesEntries");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7ValuesEntriesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7ValuesEntriesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7ValuesEntriesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7ValuesEntriesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES7TrailingCommaFlag))
{
    Output::Print(u"-%s", u"ES7TrailingComma");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES7TrailingCommaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES7TrailingCommaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES7TrailingCommaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES7TrailingCommaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6IsConcatSpreadableFlag))
{
    Output::Print(u"-%s", u"ES6IsConcatSpreadable");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6IsConcatSpreadableFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6IsConcatSpreadableFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6IsConcatSpreadableFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6IsConcatSpreadableFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6MathFlag))
{
    Output::Print(u"-%s", u"ES6Math");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6MathFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6MathFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6MathFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6MathFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
if (IsEnabled(ESDynamicImportFlag))
{
    Output::Print(u"-%s", u"ESDynamicImport");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESDynamicImportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESDynamicImportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESDynamicImportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESDynamicImportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6ModuleFlag))
{
    Output::Print(u"-%s", u"ES6Module");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ModuleFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ModuleFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ModuleFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ModuleFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ObjectFlag))
{
    Output::Print(u"-%s", u"ES6Object");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ObjectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ObjectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ObjectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ObjectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6NumberFlag))
{
    Output::Print(u"-%s", u"ES6Number");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6NumberFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6NumberFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6NumberFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6NumberFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ObjectLiteralsFlag))
{
    Output::Print(u"-%s", u"ES6ObjectLiterals");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ObjectLiteralsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ObjectLiteralsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ObjectLiteralsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ObjectLiteralsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ProxyFlag))
{
    Output::Print(u"-%s", u"ES6Proxy");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6RestFlag))
{
    Output::Print(u"-%s", u"ES6Rest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6SpreadFlag))
{
    Output::Print(u"-%s", u"ES6Spread");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6SpreadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6SpreadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6SpreadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6SpreadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6StringFlag))
{
    Output::Print(u"-%s", u"ES6String");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6StringFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6StringFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6StringFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6StringFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6StringPrototypeFixesFlag))
{
    Output::Print(u"-%s", u"ES6StringPrototypeFixes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6StringPrototypeFixesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6StringPrototypeFixesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6StringPrototypeFixesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6StringPrototypeFixesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018ObjectRestSpreadFlag))
{
    Output::Print(u"-%s", u"ES2018ObjectRestSpread");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018ObjectRestSpreadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018ObjectRestSpreadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018ObjectRestSpreadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018ObjectRestSpreadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6PrototypeChainFlag))
{
    Output::Print(u"-%s", u"ES6PrototypeChain");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6PrototypeChainFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6PrototypeChainFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6PrototypeChainFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6PrototypeChainFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToPrimitiveFlag))
{
    Output::Print(u"-%s", u"ES6ToPrimitive");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToPrimitiveFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToPrimitiveFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToPrimitiveFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToPrimitiveFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToLengthFlag))
{
    Output::Print(u"-%s", u"ES6ToLength");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6ToStringTagFlag))
{
    Output::Print(u"-%s", u"ES6ToStringTag");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6ToStringTagFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6ToStringTagFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6ToStringTagFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6ToStringTagFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnicodeFlag))
{
    Output::Print(u"-%s", u"ES6Unicode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnicodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnicodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnicodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnicodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnicodeVerboseFlag))
{
    Output::Print(u"-%s", u"ES6UnicodeVerbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnicodeVerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnicodeVerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnicodeVerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnicodeVerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6UnscopablesFlag))
{
    Output::Print(u"-%s", u"ES6Unscopables");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6UnscopablesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6UnscopablesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6UnscopablesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6UnscopablesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES6RegExStickyFlag))
{
    Output::Print(u"-%s", u"ES6RegExSticky");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExStickyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExStickyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExStickyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExStickyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018RegExDotAllFlag))
{
    Output::Print(u"-%s", u"ES2018RegExDotAll");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018RegExDotAllFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018RegExDotAllFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018RegExDotAllFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018RegExDotAllFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESExportNsAsFlag))
{
    Output::Print(u"-%s", u"ESExportNsAs");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESExportNsAsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESExportNsAsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESExportNsAsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESExportNsAsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ES2018AsyncIterationFlag))
{
    Output::Print(u"-%s", u"ES2018AsyncIteration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES2018AsyncIterationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES2018AsyncIterationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES2018AsyncIterationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES2018AsyncIterationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESTopLevelAwaitFlag))
{
    Output::Print(u"-%s", u"ESTopLevelAwait");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESTopLevelAwaitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESTopLevelAwaitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESTopLevelAwaitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESTopLevelAwaitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
if (IsEnabled(ES6RegExPrototypePropertiesFlag))
{
    Output::Print(u"-%s", u"ES6RegExPrototypeProperties");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExPrototypePropertiesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExPrototypePropertiesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExPrototypePropertiesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExPrototypePropertiesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
if (IsEnabled(ES6RegExSymbolsFlag))
{
    Output::Print(u"-%s", u"ES6RegExSymbols");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6RegExSymbolsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6RegExSymbolsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6RegExSymbolsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6RegExSymbolsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ES6VerboseFlag))
{
    Output::Print(u"-%s", u"ES6Verbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ES6VerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ES6VerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ES6VerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ES6VerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ESObjectGetOwnPropertyDescriptorsFlag))
{
    Output::Print(u"-%s", u"ESObjectGetOwnPropertyDescriptors");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESObjectGetOwnPropertyDescriptorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESObjectGetOwnPropertyDescriptorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESObjectGetOwnPropertyDescriptorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESObjectGetOwnPropertyDescriptorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif

if (IsEnabled(ESSharedArrayBufferFlag))
{
    Output::Print(u"-%s", u"ESSharedArrayBuffer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESSharedArrayBufferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESSharedArrayBufferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESSharedArrayBufferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESSharedArrayBufferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// Newer language feature flags

// ES BigInt flag
if (IsEnabled(ESBigIntFlag))
{
    Output::Print(u"-%s", u"ESBigInt");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESBigIntFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESBigIntFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESBigIntFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESBigIntFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Numeric Separator support for numeric constants
if (IsEnabled(ESNumericSeparatorFlag))
{
    Output::Print(u"-%s", u"ESNumericSeparator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESNumericSeparatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESNumericSeparatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESNumericSeparatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESNumericSeparatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Nullish coalescing operator support (??)
if (IsEnabled(ESNullishCoalescingOperatorFlag))
{
    Output::Print(u"-%s", u"ESNullishCoalescingOperator");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESNullishCoalescingOperatorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESNullishCoalescingOperatorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESNullishCoalescingOperatorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESNullishCoalescingOperatorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Hashbang support for interpreter directive syntax
if (IsEnabled(ESHashbangFlag))
{
    Output::Print(u"-%s", u"ESHashbang");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESHashbangFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESHashbangFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESHashbangFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESHashbangFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Symbol.prototype.description flag
if (IsEnabled(ESSymbolDescriptionFlag))
{
    Output::Print(u"-%s", u"ESSymbolDescription");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESSymbolDescriptionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESSymbolDescriptionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESSymbolDescriptionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESSymbolDescriptionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ESArrayFindFromLastFlag))
{
    Output::Print(u"-%s", u"ESArrayFindFromLast");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESArrayFindFromLastFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESArrayFindFromLastFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESArrayFindFromLastFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESArrayFindFromLastFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES Promise.any and AggregateError flag
if (IsEnabled(ESPromiseAnyFlag))
{
    Output::Print(u"-%s", u"ESPromiseAny");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESPromiseAnyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESPromiseAnyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESPromiseAnyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESPromiseAnyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// ES import.meta keyword meta-property
if (IsEnabled(ESImportMetaFlag))
{
    Output::Print(u"-%s", u"ESImportMeta");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESImportMetaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESImportMetaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESImportMetaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESImportMetaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

//ES globalThis flag
if (IsEnabled(ESGlobalThisFlag))
{
    Output::Print(u"-%s", u"ESGlobalThis");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ESGlobalThisFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ESGlobalThisFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ESGlobalThisFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ESGlobalThisFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// This flag to be removed once JITing generator functions is stable
if (IsEnabled(JitES6GeneratorsFlag))
{
    Output::Print(u"-%s", u"JitES6Generators");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitES6GeneratorsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitES6GeneratorsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitES6GeneratorsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitES6GeneratorsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(FastLineColumnCalculationFlag))
{
    Output::Print(u"-%s", u"FastLineColumnCalculation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FastLineColumnCalculationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FastLineColumnCalculationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FastLineColumnCalculationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FastLineColumnCalculationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FilenameFlag))
{
    Output::Print(u"-%s", u"Filename");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FilenameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FilenameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FilenameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FilenameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FreeRejittedCodeFlag))
{
    Output::Print(u"-%s", u"FreeRejittedCode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FreeRejittedCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FreeRejittedCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FreeRejittedCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FreeRejittedCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGuardPagesFlag))
{
    Output::Print(u"-%s", u"ForceGuardPages");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGuardPagesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGuardPagesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGuardPagesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGuardPagesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrintGuardPageBoundsFlag))
{
    Output::Print(u"-%s", u"PrintGuardPageBounds");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintGuardPageBoundsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintGuardPageBoundsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintGuardPageBoundsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintGuardPageBoundsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceLegacyEngineFlag))
{
    Output::Print(u"-%s", u"ForceLegacyEngine");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceLegacyEngineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceLegacyEngineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceLegacyEngineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceLegacyEngineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFlag))
{
    Output::Print(u"-%s", u"Force");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StressFlag))
{
    Output::Print(u"-%s", u"Stress");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceArrayBTreeFlag))
{
    Output::Print(u"-%s", u"ForceArrayBTree");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceArrayBTreeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceArrayBTreeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceArrayBTreeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceArrayBTreeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(StrongArraySortFlag))
{
    Output::Print(u"-%s", u"StrongArraySort");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StrongArraySortFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StrongArraySortFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StrongArraySortFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StrongArraySortFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceCleanPropertyOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceCleanPropertyOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceCleanPropertyOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceCleanPropertyOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceCleanPropertyOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceCleanPropertyOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceCleanCacheOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceCleanCacheOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceCleanCacheOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceCleanCacheOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceCleanCacheOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceCleanCacheOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGCAfterJSONParseFlag))
{
    Output::Print(u"-%s", u"ForceGCAfterJSONParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGCAfterJSONParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGCAfterJSONParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGCAfterJSONParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGCAfterJSONParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDecommitOnCollectFlag))
{
    Output::Print(u"-%s", u"ForceDecommitOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDecommitOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDecommitOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDecommitOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDecommitOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDeferParseFlag))
{
    Output::Print(u"-%s", u"ForceDeferParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDiagnosticsModeFlag))
{
    Output::Print(u"-%s", u"ForceDiagnosticsMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDiagnosticsModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDiagnosticsModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDiagnosticsModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDiagnosticsModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceGetWriteWatchOOMFlag))
{
    Output::Print(u"-%s", u"ForceGetWriteWatchOOM");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceGetWriteWatchOOMFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceGetWriteWatchOOMFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceGetWriteWatchOOMFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceGetWriteWatchOOMFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForcePostLowerGlobOptInstrStringFlag))
{
    Output::Print(u"-%s", u"ForcePostLowerGlobOptInstrString");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForcePostLowerGlobOptInstrStringFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForcePostLowerGlobOptInstrStringFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForcePostLowerGlobOptInstrStringFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForcePostLowerGlobOptInstrStringFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSplitScopeFlag))
{
    Output::Print(u"-%s", u"ForceSplitScope");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSplitScopeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSplitScopeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSplitScopeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSplitScopeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnumerateSpecialPropertiesInDebuggerFlag))
{
    Output::Print(u"-%s", u"EnumerateSpecialPropertiesInDebugger");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnumerateSpecialPropertiesInDebuggerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnumerateSpecialPropertiesInDebuggerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnumerateSpecialPropertiesInDebuggerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnumerateSpecialPropertiesInDebuggerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableContinueAfterExceptionWrappersForHelpersFlag))
{
    Output::Print(u"-%s", u"EnableContinueAfterExceptionWrappersForHelpers");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableContinueAfterExceptionWrappersForHelpersFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableContinueAfterExceptionWrappersForHelpersFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableContinueAfterExceptionWrappersForHelpersFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableContinueAfterExceptionWrappersForHelpersFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableContinueAfterExceptionWrappersForBuiltInsFlag))
{
    Output::Print(u"-%s", u"EnableContinueAfterExceptionWrappersForBuiltIns");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableContinueAfterExceptionWrappersForBuiltInsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableContinueAfterExceptionWrappersForBuiltInsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableContinueAfterExceptionWrappersForBuiltInsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableContinueAfterExceptionWrappersForBuiltInsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableFunctionSourceReportForHeapEnumFlag))
{
    Output::Print(u"-%s", u"EnableFunctionSourceReportForHeapEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableFunctionSourceReportForHeapEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableFunctionSourceReportForHeapEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableFunctionSourceReportForHeapEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableFunctionSourceReportForHeapEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFragmentAddressSpaceFlag))
{
    Output::Print(u"-%s", u"ForceFragmentAddressSpace");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFragmentAddressSpaceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFragmentAddressSpaceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFragmentAddressSpaceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFragmentAddressSpaceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceOOMOnEBCommitFlag))
{
    Output::Print(u"-%s", u"ForceOOMOnEBCommit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceOOMOnEBCommitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceOOMOnEBCommitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceOOMOnEBCommitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceOOMOnEBCommitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceDynamicProfileFlag))
{
    Output::Print(u"-%s", u"ForceDynamicProfile");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceDynamicProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceDynamicProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceDynamicProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceDynamicProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceES5ArrayFlag))
{
    Output::Print(u"-%s", u"ForceES5Array");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceES5ArrayFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceES5ArrayFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceES5ArrayFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceES5ArrayFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceAsmJsLinkFailFlag))
{
    Output::Print(u"-%s", u"ForceAsmJsLinkFail");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceAsmJsLinkFailFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceAsmJsLinkFailFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceAsmJsLinkFailFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceAsmJsLinkFailFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceExpireOnNonCacheCollectFlag))
{
    Output::Print(u"-%s", u"ForceExpireOnNonCacheCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceExpireOnNonCacheCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceExpireOnNonCacheCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceExpireOnNonCacheCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceExpireOnNonCacheCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFastPathFlag))
{
    Output::Print(u"-%s", u"ForceFastPath");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFastPathFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFastPathFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFastPathFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFastPathFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceFloatPrefFlag))
{
    Output::Print(u"-%s", u"ForceFloatPref");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceFloatPrefFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceFloatPrefFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceFloatPrefFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceFloatPrefFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceJITLoopBodyFlag))
{
    Output::Print(u"-%s", u"ForceJITLoopBody");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceJITLoopBodyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceJITLoopBodyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceJITLoopBodyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceJITLoopBodyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStaticInterpreterThunkFlag))
{
    Output::Print(u"-%s", u"ForceStaticInterpreterThunk");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStaticInterpreterThunkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStaticInterpreterThunkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStaticInterpreterThunkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStaticInterpreterThunkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpCommentsFromReferencedFilesFlag))
{
    Output::Print(u"-%s", u"DumpCommentsFromReferencedFiles");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpCommentsFromReferencedFilesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpCommentsFromReferencedFilesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpCommentsFromReferencedFilesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpCommentsFromReferencedFilesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DelayFullJITSmallFuncFlag))
{
    Output::Print(u"-%s", u"DelayFullJITSmallFunc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DelayFullJITSmallFuncFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DelayFullJITSmallFuncFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DelayFullJITSmallFuncFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DelayFullJITSmallFuncFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableFatalErrorOnOOMFlag))
{
    Output::Print(u"-%s", u"EnableFatalErrorOnOOM");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableFatalErrorOnOOMFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableFatalErrorOnOOMFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableFatalErrorOnOOMFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableFatalErrorOnOOMFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_ARM32_OR_ARM64)
if (IsEnabled(ForceLocalsPtrFlag))
{
    Output::Print(u"-%s", u"ForceLocalsPtr");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceLocalsPtrFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceLocalsPtrFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceLocalsPtrFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceLocalsPtrFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DeferLoadingAvailableSourceFlag))
{
    Output::Print(u"-%s", u"DeferLoadingAvailableSource");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeferLoadingAvailableSourceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeferLoadingAvailableSourceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeferLoadingAvailableSourceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeferLoadingAvailableSourceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceNativeFlag))
{
    Output::Print(u"-%s", u"ForceNative");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceNativeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceNativeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceNativeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceNativeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedFlag))
{
    Output::Print(u"-%s", u"ForceSerialized");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedBytecodeMajorVersionFlag))
{
    Output::Print(u"-%s", u"ForceSerializedBytecodeMajorVersion");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedBytecodeMajorVersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedBytecodeMajorVersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedBytecodeMajorVersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedBytecodeMajorVersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSerializedBytecodeVersionSchemaFlag))
{
    Output::Print(u"-%s", u"ForceSerializedBytecodeVersionSchema");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSerializedBytecodeVersionSchemaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSerializedBytecodeVersionSchemaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSerializedBytecodeVersionSchemaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSerializedBytecodeVersionSchemaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStrictModeFlag))
{
    Output::Print(u"-%s", u"ForceStrictMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStrictModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStrictModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStrictModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStrictModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceUndoDeferFlag))
{
    Output::Print(u"-%s", u"ForceUndoDefer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceUndoDeferFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceUndoDeferFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceUndoDeferFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceUndoDeferFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceBlockingConcurrentCollectFlag))
{
    Output::Print(u"-%s", u"ForceBlockingConcurrentCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceBlockingConcurrentCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceBlockingConcurrentCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceBlockingConcurrentCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceBlockingConcurrentCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FreTestDiagModeFlag))
{
    Output::Print(u"-%s", u"FreTestDiagMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FreTestDiagModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FreTestDiagModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FreTestDiagModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FreTestDiagModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef BYTECODE_TESTING
if (IsEnabled(ByteCodeBranchLimitFlag))
{
    Output::Print(u"-%s", u"ByteCodeBranchLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ByteCodeBranchLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ByteCodeBranchLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ByteCodeBranchLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ByteCodeBranchLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MediumByteCodeLayoutFlag))
{
    Output::Print(u"-%s", u"MediumByteCodeLayout");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MediumByteCodeLayoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MediumByteCodeLayoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MediumByteCodeLayoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MediumByteCodeLayoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LargeByteCodeLayoutFlag))
{
    Output::Print(u"-%s", u"LargeByteCodeLayout");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LargeByteCodeLayoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LargeByteCodeLayoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LargeByteCodeLayoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LargeByteCodeLayoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(InduceCodeGenFailureFlag))
{
    Output::Print(u"-%s", u"InduceCodeGenFailure");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InduceCodeGenFailureFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InduceCodeGenFailureFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InduceCodeGenFailureFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InduceCodeGenFailureFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InduceCodeGenFailureSeedFlag))
{
    Output::Print(u"-%s", u"InduceCodeGenFailureSeed");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InduceCodeGenFailureSeedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InduceCodeGenFailureSeedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InduceCodeGenFailureSeedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InduceCodeGenFailureSeedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InjectPartiallyInitializedInterpreterFrameErrorFlag))
{
    Output::Print(u"-%s", u"InjectPartiallyInitializedInterpreterFrameError");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InjectPartiallyInitializedInterpreterFrameErrorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InjectPartiallyInitializedInterpreterFrameErrorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InjectPartiallyInitializedInterpreterFrameErrorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InjectPartiallyInitializedInterpreterFrameErrorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag))
{
    Output::Print(u"-%s", u"InjectPartiallyInitializedInterpreterFrameErrorType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InjectPartiallyInitializedInterpreterFrameErrorTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(GenerateByteCodeBufferReturnsCantGenerateFlag))
{
    Output::Print(u"-%s", u"GenerateByteCodeBufferReturnsCantGenerate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GenerateByteCodeBufferReturnsCantGenerateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GenerateByteCodeBufferReturnsCantGenerateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GenerateByteCodeBufferReturnsCantGenerateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GenerateByteCodeBufferReturnsCantGenerateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(GoptCleanupThresholdFlag))
{
    Output::Print(u"-%s", u"GoptCleanupThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GoptCleanupThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GoptCleanupThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GoptCleanupThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GoptCleanupThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmGoptCleanupThresholdFlag))
{
    Output::Print(u"-%s", u"AsmGoptCleanupThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmGoptCleanupThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmGoptCleanupThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmGoptCleanupThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmGoptCleanupThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(HighPrecisionDateFlag))
{
    Output::Print(u"-%s", u"HighPrecisionDate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(HighPrecisionDateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(HighPrecisionDateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(HighPrecisionDateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(HighPrecisionDateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCountMaxFlag))
{
    Output::Print(u"-%s", u"InlineCountMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCountMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCountMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCountMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCountMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCountMaxInLoopBodiesFlag))
{
    Output::Print(u"-%s", u"InlineCountMaxInLoopBodies");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCountMaxInLoopBodiesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCountMaxInLoopBodiesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCountMaxInLoopBodiesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCountMaxInLoopBodiesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(icminlbFlag))
{
    Output::Print(u"-%s", u"icminlb");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(icminlbFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(icminlbFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(icminlbFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(icminlbFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineInLoopBodyScaleDownFactorFlag))
{
    Output::Print(u"-%s", u"InlineInLoopBodyScaleDownFactor");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineInLoopBodyScaleDownFactorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineInLoopBodyScaleDownFactorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineInLoopBodyScaleDownFactorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineInLoopBodyScaleDownFactorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(iilbsdfFlag))
{
    Output::Print(u"-%s", u"iilbsdf");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(iilbsdfFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(iilbsdfFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(iilbsdfFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(iilbsdfFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdFlag))
{
    Output::Print(u"-%s", u"InlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AggressiveInlineCountMaxFlag))
{
    Output::Print(u"-%s", u"AggressiveInlineCountMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AggressiveInlineCountMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AggressiveInlineCountMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AggressiveInlineCountMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AggressiveInlineCountMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AggressiveInlineThresholdFlag))
{
    Output::Print(u"-%s", u"AggressiveInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AggressiveInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AggressiveInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AggressiveInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AggressiveInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInLargeFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInLargeFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInLargeFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInLargeFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInLargeFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInLargeFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInMediumSizedFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInMediumSizedFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInMediumSizedFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInMediumSizedFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInMediumSizedFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInMediumSizedFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineThresholdAdjustCountInSmallFunctionFlag))
{
    Output::Print(u"-%s", u"InlineThresholdAdjustCountInSmallFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineThresholdAdjustCountInSmallFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineThresholdAdjustCountInSmallFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineThresholdAdjustCountInSmallFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineThresholdAdjustCountInSmallFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AsmJsInlineAdjustFlag))
{
    Output::Print(u"-%s", u"AsmJsInlineAdjust");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AsmJsInlineAdjustFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AsmJsInlineAdjustFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AsmJsInlineAdjustFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AsmJsInlineAdjustFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InterpretFlag))
{
    Output::Print(u"-%s", u"Interpret");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InterpretFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InterpretFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InterpretFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InterpretFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InstrumentFlag))
{
    Output::Print(u"-%s", u"Instrument");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InstrumentFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InstrumentFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InstrumentFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InstrumentFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JitQueueThresholdFlag))
{
    Output::Print(u"-%s", u"JitQueueThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitQueueThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitQueueThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitQueueThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitQueueThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef LEAK_REPORT
if (IsEnabled(LeakReportFlag))
{
    Output::Print(u"-%s", u"LeakReport");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeakReportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeakReportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeakReportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeakReportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(LoopInlineThresholdFlag))
{
    Output::Print(u"-%s", u"LoopInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LeafInlineThresholdFlag))
{
    Output::Print(u"-%s", u"LeafInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeafInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeafInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeafInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeafInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstantArgumentInlineThresholdFlag))
{
    Output::Print(u"-%s", u"ConstantArgumentInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstantArgumentInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstantArgumentInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstantArgumentInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstantArgumentInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineThresholdFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineDepthMaxFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineDepthMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineDepthMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineDepthMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineDepthMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineDepthMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecursiveInlineDepthMinFlag))
{
    Output::Print(u"-%s", u"RecursiveInlineDepthMin");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecursiveInlineDepthMinFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecursiveInlineDepthMinFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecursiveInlineDepthMinFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecursiveInlineDepthMinFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RedeferralCapFlag))
{
    Output::Print(u"-%s", u"RedeferralCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RedeferralCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RedeferralCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RedeferralCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RedeferralCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopFlag))
{
    Output::Print(u"-%s", u"Loop");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopInterpretCountFlag))
{
    Output::Print(u"-%s", u"LoopInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(licFlag))
{
    Output::Print(u"-%s", u"lic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(licFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(licFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(licFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(licFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopProfileIterationsFlag))
{
    Output::Print(u"-%s", u"LoopProfileIterations");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopProfileIterationsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopProfileIterationsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopProfileIterationsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopProfileIterationsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutsideLoopInlineThresholdFlag))
{
    Output::Print(u"-%s", u"OutsideLoopInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutsideLoopInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutsideLoopInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutsideLoopInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutsideLoopInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxFuncInlineDepthFlag))
{
    Output::Print(u"-%s", u"MaxFuncInlineDepth");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxFuncInlineDepthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxFuncInlineDepthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxFuncInlineDepthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxFuncInlineDepthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxNumberOfInlineesWithLoopFlag))
{
    Output::Print(u"-%s", u"MaxNumberOfInlineesWithLoop");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxNumberOfInlineesWithLoopFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxNumberOfInlineesWithLoopFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxNumberOfInlineesWithLoopFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxNumberOfInlineesWithLoopFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef MEMSPECT_TRACKING
if (IsEnabled(MemspectFlag))
{
    Output::Print(u"-%s", u"Memspect");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemspectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemspectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemspectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemspectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PolymorphicInlineThresholdFlag))
{
    Output::Print(u"-%s", u"PolymorphicInlineThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PolymorphicInlineThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PolymorphicInlineThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PolymorphicInlineThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PolymorphicInlineThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrimeRecyclerFlag))
{
    Output::Print(u"-%s", u"PrimeRecycler");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrimeRecyclerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrimeRecyclerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrimeRecyclerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrimeRecyclerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceEngineRefcountFlag))
{
    Output::Print(u"-%s", u"TraceEngineRefcount");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceEngineRefcountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceEngineRefcountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceEngineRefcountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceEngineRefcountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
if (IsEnabled(LeakStackTraceFlag))
{
    Output::Print(u"-%s", u"LeakStackTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LeakStackTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LeakStackTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LeakStackTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LeakStackTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceMemoryLeakFlag))
{
    Output::Print(u"-%s", u"ForceMemoryLeak");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceMemoryLeakFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceMemoryLeakFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceMemoryLeakFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceMemoryLeakFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(DumpAfterFinalGCFlag))
{
    Output::Print(u"-%s", u"DumpAfterFinalGC");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpAfterFinalGCFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpAfterFinalGCFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpAfterFinalGCFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpAfterFinalGCFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceOldDateAPIFlag))
{
    Output::Print(u"-%s", u"ForceOldDateAPI");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceOldDateAPIFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceOldDateAPIFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceOldDateAPIFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceOldDateAPIFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(JitLoopBodyHotLoopThresholdFlag))
{
    Output::Print(u"-%s", u"JitLoopBodyHotLoopThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JitLoopBodyHotLoopThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JitLoopBodyHotLoopThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JitLoopBodyHotLoopThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JitLoopBodyHotLoopThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopBodySizeThresholdToDisableOptsFlag))
{
    Output::Print(u"-%s", u"LoopBodySizeThresholdToDisableOpts");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopBodySizeThresholdToDisableOptsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopBodySizeThresholdToDisableOptsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopBodySizeThresholdToDisableOptsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopBodySizeThresholdToDisableOptsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxJitThreadCountFlag))
{
    Output::Print(u"-%s", u"MaxJitThreadCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJitThreadCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJitThreadCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJitThreadCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJitThreadCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceMaxJitThreadCountFlag))
{
    Output::Print(u"-%s", u"ForceMaxJitThreadCount");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceMaxJitThreadCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceMaxJitThreadCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceMaxJitThreadCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceMaxJitThreadCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MitigateSpectreFlag))
{
    Output::Print(u"-%s", u"MitigateSpectre");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MitigateSpectreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MitigateSpectreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MitigateSpectreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MitigateSpectreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AddMaskingBlocksFlag))
{
    Output::Print(u"-%s", u"AddMaskingBlocks");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AddMaskingBlocksFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AddMaskingBlocksFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AddMaskingBlocksFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AddMaskingBlocksFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(PoisonVarArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonVarArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonVarArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonVarArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonVarArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonVarArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonIntArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonIntArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonIntArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonIntArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonIntArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonIntArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonFloatArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonFloatArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonFloatArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonFloatArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonFloatArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonFloatArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonTypedArrayLoadFlag))
{
    Output::Print(u"-%s", u"PoisonTypedArrayLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonTypedArrayLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonTypedArrayLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonTypedArrayLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonTypedArrayLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonStringLoadFlag))
{
    Output::Print(u"-%s", u"PoisonStringLoad");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonStringLoadFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonStringLoadFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonStringLoadFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonStringLoadFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonObjectsForLoadsFlag))
{
    Output::Print(u"-%s", u"PoisonObjectsForLoads");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonObjectsForLoadsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonObjectsForLoadsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonObjectsForLoadsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonObjectsForLoadsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(PoisonVarArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonVarArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonVarArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonVarArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonVarArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonVarArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonIntArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonIntArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonIntArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonIntArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonIntArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonIntArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonFloatArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonFloatArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonFloatArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonFloatArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonFloatArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonFloatArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonTypedArrayStoreFlag))
{
    Output::Print(u"-%s", u"PoisonTypedArrayStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonTypedArrayStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonTypedArrayStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonTypedArrayStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonTypedArrayStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonStringStoreFlag))
{
    Output::Print(u"-%s", u"PoisonStringStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonStringStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonStringStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonStringStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonStringStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PoisonObjectsForStoresFlag))
{
    Output::Print(u"-%s", u"PoisonObjectsForStores");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PoisonObjectsForStoresFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PoisonObjectsForStoresFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PoisonObjectsForStoresFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PoisonObjectsForStoresFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MinInterpretCountFlag))
{
    Output::Print(u"-%s", u"MinInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinSimpleJitRunCountFlag))
{
    Output::Print(u"-%s", u"MinSimpleJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinSimpleJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinSimpleJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinSimpleJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinSimpleJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxInterpretCountFlag))
{
    Output::Print(u"-%s", u"MaxInterpretCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxInterpretCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxInterpretCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxInterpretCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxInterpretCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MicFlag))
{
    Output::Print(u"-%s", u"Mic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MicFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MicFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MicFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MicFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSimpleJitRunCountFlag))
{
    Output::Print(u"-%s", u"MaxSimpleJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSimpleJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSimpleJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSimpleJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSimpleJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MsjrcFlag))
{
    Output::Print(u"-%s", u"Msjrc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MsjrcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MsjrcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MsjrcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MsjrcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinMemOpCountFlag))
{
    Output::Print(u"-%s", u"MinMemOpCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinMemOpCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinMemOpCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinMemOpCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinMemOpCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MmocFlag))
{
    Output::Print(u"-%s", u"Mmoc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MmocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MmocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MmocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MmocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if ENABLE_COPYONACCESS_ARRAY
if (IsEnabled(MaxCopyOnAccessArrayLengthFlag))
{
    Output::Print(u"-%s", u"MaxCopyOnAccessArrayLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxCopyOnAccessArrayLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxCopyOnAccessArrayLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxCopyOnAccessArrayLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxCopyOnAccessArrayLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinCopyOnAccessArrayLengthFlag))
{
    Output::Print(u"-%s", u"MinCopyOnAccessArrayLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinCopyOnAccessArrayLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinCopyOnAccessArrayLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinCopyOnAccessArrayLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinCopyOnAccessArrayLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CopyOnAccessArraySegmentCacheSizeFlag))
{
    Output::Print(u"-%s", u"CopyOnAccessArraySegmentCacheSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CopyOnAccessArraySegmentCacheSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CopyOnAccessArraySegmentCacheSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CopyOnAccessArraySegmentCacheSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CopyOnAccessArraySegmentCacheSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(MinTemplatizedJitRunCountFlag))
{
    Output::Print(u"-%s", u"MinTemplatizedJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinTemplatizedJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinTemplatizedJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinTemplatizedJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinTemplatizedJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinAsmJsInterpreterRunCountFlag))
{
    Output::Print(u"-%s", u"MinAsmJsInterpreterRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinAsmJsInterpreterRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinAsmJsInterpreterRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinAsmJsInterpreterRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinAsmJsInterpreterRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MinTemplatizedJitLoopRunCountFlag))
{
    Output::Print(u"-%s", u"MinTemplatizedJitLoopRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinTemplatizedJitLoopRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinTemplatizedJitLoopRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinTemplatizedJitLoopRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinTemplatizedJitLoopRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxTemplatizedJitRunCountFlag))
{
    Output::Print(u"-%s", u"MaxTemplatizedJitRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxTemplatizedJitRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxTemplatizedJitRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxTemplatizedJitRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxTemplatizedJitRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MtjrcFlag))
{
    Output::Print(u"-%s", u"Mtjrc");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MtjrcFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MtjrcFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MtjrcFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MtjrcFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxAsmJsInterpreterRunCountFlag))
{
    Output::Print(u"-%s", u"MaxAsmJsInterpreterRunCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxAsmJsInterpreterRunCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxAsmJsInterpreterRunCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxAsmJsInterpreterRunCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxAsmJsInterpreterRunCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaicFlag))
{
    Output::Print(u"-%s", u"Maic");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaicFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaicFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaicFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaicFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(AutoProfilingInterpreter0LimitFlag))
{
    Output::Print(u"-%s", u"AutoProfilingInterpreter0Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AutoProfilingInterpreter0LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AutoProfilingInterpreter0LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AutoProfilingInterpreter0LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AutoProfilingInterpreter0LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfilingInterpreter0LimitFlag))
{
    Output::Print(u"-%s", u"ProfilingInterpreter0Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfilingInterpreter0LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfilingInterpreter0LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfilingInterpreter0LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfilingInterpreter0LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AutoProfilingInterpreter1LimitFlag))
{
    Output::Print(u"-%s", u"AutoProfilingInterpreter1Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AutoProfilingInterpreter1LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AutoProfilingInterpreter1LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AutoProfilingInterpreter1LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AutoProfilingInterpreter1LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SimpleJitLimitFlag))
{
    Output::Print(u"-%s", u"SimpleJitLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimpleJitLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimpleJitLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimpleJitLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimpleJitLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfilingInterpreter1LimitFlag))
{
    Output::Print(u"-%s", u"ProfilingInterpreter1Limit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfilingInterpreter1LimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfilingInterpreter1LimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfilingInterpreter1LimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfilingInterpreter1LimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(ExecutionModeLimitsFlag))
{
    Output::Print(u"-%s", u"ExecutionModeLimits");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExecutionModeLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExecutionModeLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExecutionModeLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExecutionModeLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EmlFlag))
{
    Output::Print(u"-%s", u"Eml");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EmlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EmlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EmlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EmlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnforceExecutionModeLimitsFlag))
{
    Output::Print(u"-%s", u"EnforceExecutionModeLimits");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnforceExecutionModeLimitsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnforceExecutionModeLimitsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnforceExecutionModeLimitsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnforceExecutionModeLimitsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EemlFlag))
{
    Output::Print(u"-%s", u"Eeml");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EemlFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EemlFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EemlFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EemlFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(SimpleJitAfterFlag))
{
    Output::Print(u"-%s", u"SimpleJitAfter");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimpleJitAfterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimpleJitAfterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimpleJitAfterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimpleJitAfterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SjaFlag))
{
    Output::Print(u"-%s", u"Sja");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SjaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SjaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SjaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SjaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FullJitAfterFlag))
{
    Output::Print(u"-%s", u"FullJitAfter");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FullJitAfterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FullJitAfterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FullJitAfterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FullJitAfterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FjaFlag))
{
    Output::Print(u"-%s", u"Fja");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FjaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FjaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FjaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FjaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(NewSimpleJitFlag))
{
    Output::Print(u"-%s", u"NewSimpleJit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NewSimpleJitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NewSimpleJitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NewSimpleJitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NewSimpleJitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxLinearIntCaseCountFlag))
{
    Output::Print(u"-%s", u"MaxLinearIntCaseCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLinearIntCaseCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLinearIntCaseCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLinearIntCaseCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLinearIntCaseCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSingleCharStrJumpTableSizeFlag))
{
    Output::Print(u"-%s", u"MaxSingleCharStrJumpTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleCharStrJumpTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleCharStrJumpTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleCharStrJumpTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleCharStrJumpTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxSingleCharStrJumpTableRatioFlag))
{
    Output::Print(u"-%s", u"MaxSingleCharStrJumpTableRatio");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleCharStrJumpTableRatioFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleCharStrJumpTableRatioFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleCharStrJumpTableRatioFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleCharStrJumpTableRatioFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinSwitchJumpTableSizeFlag))
{
    Output::Print(u"-%s", u"MinSwitchJumpTableSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinSwitchJumpTableSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinSwitchJumpTableSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinSwitchJumpTableSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinSwitchJumpTableSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxLinearStringCaseCountFlag))
{
    Output::Print(u"-%s", u"MaxLinearStringCaseCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLinearStringCaseCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLinearStringCaseCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLinearStringCaseCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLinearStringCaseCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinDeferredFuncTokenCountFlag))
{
    Output::Print(u"-%s", u"MinDeferredFuncTokenCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinDeferredFuncTokenCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinDeferredFuncTokenCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinDeferredFuncTokenCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinDeferredFuncTokenCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if DBG
if (IsEnabled(SkipFuncCountForBailOnNoProfileFlag))
{
    Output::Print(u"-%s", u"SkipFuncCountForBailOnNoProfile");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SkipFuncCountForBailOnNoProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SkipFuncCountForBailOnNoProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SkipFuncCountForBailOnNoProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SkipFuncCountForBailOnNoProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(MaxJITFunctionBytecodeByteLengthFlag))
{
    Output::Print(u"-%s", u"MaxJITFunctionBytecodeByteLength");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJITFunctionBytecodeByteLengthFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJITFunctionBytecodeByteLengthFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJITFunctionBytecodeByteLengthFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJITFunctionBytecodeByteLengthFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxJITFunctionBytecodeCountFlag))
{
    Output::Print(u"-%s", u"MaxJITFunctionBytecodeCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxJITFunctionBytecodeCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxJITFunctionBytecodeCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxJITFunctionBytecodeCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxJITFunctionBytecodeCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxLoopsPerFunctionFlag))
{
    Output::Print(u"-%s", u"MaxLoopsPerFunction");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxLoopsPerFunctionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxLoopsPerFunctionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxLoopsPerFunctionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxLoopsPerFunctionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(FuncObjectInlineCacheThresholdFlag))
{
    Output::Print(u"-%s", u"FuncObjectInlineCacheThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FuncObjectInlineCacheThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FuncObjectInlineCacheThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FuncObjectInlineCacheThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FuncObjectInlineCacheThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoDeferParseFlag))
{
    Output::Print(u"-%s", u"NoDeferParse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoDeferParseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoDeferParseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoDeferParseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoDeferParseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoLogoFlag))
{
    Output::Print(u"-%s", u"NoLogo");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoLogoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoLogoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoLogoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoLogoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPJITMissingOptsFlag))
{
    Output::Print(u"-%s", u"OOPJITMissingOpts");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPJITMissingOptsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPJITMissingOptsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPJITMissingOptsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPJITMissingOptsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CrashOnOOPJITFailureFlag))
{
    Output::Print(u"-%s", u"CrashOnOOPJITFailure");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CrashOnOOPJITFailureFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CrashOnOOPJITFailureFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CrashOnOOPJITFailureFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CrashOnOOPJITFailureFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OOPCFGRegistrationFlag))
{
    Output::Print(u"-%s", u"OOPCFGRegistration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OOPCFGRegistrationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OOPCFGRegistrationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OOPCFGRegistrationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OOPCFGRegistrationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceJITCFGCheckFlag))
{
    Output::Print(u"-%s", u"ForceJITCFGCheck");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceJITCFGCheckFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceJITCFGCheckFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceJITCFGCheckFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceJITCFGCheckFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(UseJITTrampolineFlag))
{
    Output::Print(u"-%s", u"UseJITTrampoline");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(UseJITTrampolineFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(UseJITTrampolineFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(UseJITTrampolineFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(UseJITTrampolineFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoNativeFlag))
{
    Output::Print(u"-%s", u"NoNative");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoNativeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoNativeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoNativeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoNativeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NopFrequencyFlag))
{
    Output::Print(u"-%s", u"NopFrequency");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NopFrequencyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NopFrequencyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NopFrequencyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NopFrequencyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NoStrictModeFlag))
{
    Output::Print(u"-%s", u"NoStrictMode");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NoStrictModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NoStrictModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NoStrictModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NoStrictModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NormalizeStatsFlag))
{
    Output::Print(u"-%s", u"NormalizeStats");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NormalizeStatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NormalizeStatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NormalizeStatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NormalizeStatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OffFlag))
{
    Output::Print(u"-%s", u"Off");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OffFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OffFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OffFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OffFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OffProfiledByteCodeFlag))
{
    Output::Print(u"-%s", u"OffProfiledByteCode");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OffProfiledByteCodeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OffProfiledByteCodeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OffProfiledByteCodeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OffProfiledByteCodeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OnFlag))
{
    Output::Print(u"-%s", u"On");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OnFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OnFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OnFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OnFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutputFileFlag))
{
    Output::Print(u"-%s", u"OutputFile");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutputFileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutputFileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutputFileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutputFileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(OutputFileOpenModeFlag))
{
    Output::Print(u"-%s", u"OutputFileOpenMode");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OutputFileOpenModeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OutputFileOpenModeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OutputFileOpenModeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OutputFileOpenModeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef ENABLE_TRACE
if (IsEnabled(InMemoryTraceFlag))
{
    Output::Print(u"-%s", u"InMemoryTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InMemoryTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InMemoryTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InMemoryTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InMemoryTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InMemoryTraceBufferSizeFlag))
{
    Output::Print(u"-%s", u"InMemoryTraceBufferSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InMemoryTraceBufferSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InMemoryTraceBufferSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InMemoryTraceBufferSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InMemoryTraceBufferSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef STACK_BACK_TRACE
if (IsEnabled(TraceWithStackFlag))
{
    Output::Print(u"-%s", u"TraceWithStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWithStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWithStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWithStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWithStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
if (IsEnabled(PrintRunTimeDataCollectionTraceFlag))
{
    Output::Print(u"-%s", u"PrintRunTimeDataCollectionTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintRunTimeDataCollectionTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintRunTimeDataCollectionTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintRunTimeDataCollectionTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintRunTimeDataCollectionTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef ENABLE_PREJIT
if (IsEnabled(PrejitFlag))
{
    Output::Print(u"-%s", u"Prejit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PrintSrcInDumpFlag))
{
    Output::Print(u"-%s", u"PrintSrcInDump");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintSrcInDumpFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintSrcInDumpFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintSrcInDumpFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintSrcInDumpFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if PROFILE_DICTIONARY
if (IsEnabled(ProfileDictionaryFlag))
{
    Output::Print(u"-%s", u"ProfileDictionary");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileDictionaryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileDictionaryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileDictionaryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileDictionaryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_EXEC
if (IsEnabled(ProfileFlag))
{
    Output::Print(u"-%s", u"Profile");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ProfileThresholdFlag))
{
    Output::Print(u"-%s", u"ProfileThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_OBJECT_LITERALS
if (IsEnabled(ProfileObjectLiteralFlag))
{
    Output::Print(u"-%s", u"ProfileObjectLiteral");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileObjectLiteralFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileObjectLiteralFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileObjectLiteralFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileObjectLiteralFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_MEM
if (IsEnabled(ProfileMemoryFlag))
{
    Output::Print(u"-%s", u"ProfileMemory");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_STRINGS
if (IsEnabled(ProfileStringsFlag))
{
    Output::Print(u"-%s", u"ProfileStrings");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileStringsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileStringsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileStringsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileStringsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_TYPES
if (IsEnabled(ProfileTypesFlag))
{
    Output::Print(u"-%s", u"ProfileTypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileTypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileTypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileTypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileTypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef PROFILE_EVALMAP
if (IsEnabled(ProfileEvalMapFlag))
{
    Output::Print(u"-%s", u"ProfileEvalMap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileEvalMapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileEvalMapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileEvalMapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileEvalMapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
if (IsEnabled(ProfileBailOutRecordMemoryFlag))
{
    Output::Print(u"-%s", u"ProfileBailOutRecordMemory");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ProfileBailOutRecordMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ProfileBailOutRecordMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ProfileBailOutRecordMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ProfileBailOutRecordMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if DBG
if (IsEnabled(ValidateIntRangesFlag))
{
    Output::Print(u"-%s", u"ValidateIntRanges");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateIntRangesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateIntRangesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateIntRangesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateIntRangesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(RejitMaxBailOutCountFlag))
{
    Output::Print(u"-%s", u"RejitMaxBailOutCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RejitMaxBailOutCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RejitMaxBailOutCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RejitMaxBailOutCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RejitMaxBailOutCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(CallsToBailoutsRatioForRejitFlag))
{
    Output::Print(u"-%s", u"CallsToBailoutsRatioForRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(CallsToBailoutsRatioForRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(CallsToBailoutsRatioForRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(CallsToBailoutsRatioForRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(CallsToBailoutsRatioForRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LoopIterationsToBailoutsRatioForRejitFlag))
{
    Output::Print(u"-%s", u"LoopIterationsToBailoutsRatioForRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopIterationsToBailoutsRatioForRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopIterationsToBailoutsRatioForRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopIterationsToBailoutsRatioForRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopIterationsToBailoutsRatioForRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBailOutsBeforeRejitFlag))
{
    Output::Print(u"-%s", u"MinBailOutsBeforeRejit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBailOutsBeforeRejitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBailOutsBeforeRejitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBailOutsBeforeRejitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBailOutsBeforeRejitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBailOutsBeforeRejitForLoopsFlag))
{
    Output::Print(u"-%s", u"MinBailOutsBeforeRejitForLoops");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBailOutsBeforeRejitForLoopsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBailOutsBeforeRejitForLoopsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBailOutsBeforeRejitForLoopsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBailOutsBeforeRejitForLoopsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LibraryStackFrameFlag))
{
    Output::Print(u"-%s", u"LibraryStackFrame");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LibraryStackFrameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LibraryStackFrameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LibraryStackFrameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LibraryStackFrameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(LibraryStackFrameDebuggerFlag))
{
    Output::Print(u"-%s", u"LibraryStackFrameDebugger");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LibraryStackFrameDebuggerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LibraryStackFrameDebuggerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LibraryStackFrameDebuggerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LibraryStackFrameDebuggerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_STRESS
if (IsEnabled(RecyclerStressFlag))
{
    Output::Print(u"-%s", u"RecyclerStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerBackgroundStressFlag))
{
    Output::Print(u"-%s", u"RecyclerBackgroundStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerBackgroundStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerBackgroundStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerBackgroundStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerBackgroundStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerConcurrentStressFlag))
{
    Output::Print(u"-%s", u"RecyclerConcurrentStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerConcurrentStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerConcurrentStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerConcurrentStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerConcurrentStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerConcurrentRepeatStressFlag))
{
    Output::Print(u"-%s", u"RecyclerConcurrentRepeatStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerConcurrentRepeatStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerConcurrentRepeatStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerConcurrentRepeatStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerConcurrentRepeatStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerPartialStressFlag))
{
    Output::Print(u"-%s", u"RecyclerPartialStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerPartialStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerPartialStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerPartialStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerPartialStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerTrackStressFlag))
{
    Output::Print(u"-%s", u"RecyclerTrackStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerTrackStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerTrackStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerTrackStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerTrackStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerInduceFalsePositivesFlag))
{
    Output::Print(u"-%s", u"RecyclerInduceFalsePositives");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerInduceFalsePositivesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerInduceFalsePositivesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerInduceFalsePositivesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerInduceFalsePositivesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif // RECYCLER_STRESS
if (IsEnabled(RecyclerForceMarkInteriorFlag))
{
    Output::Print(u"-%s", u"RecyclerForceMarkInterior");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerForceMarkInteriorFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerForceMarkInteriorFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerForceMarkInteriorFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerForceMarkInteriorFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerPriorityBoostTimeoutFlag))
{
    Output::Print(u"-%s", u"RecyclerPriorityBoostTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerPriorityBoostTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerPriorityBoostTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerPriorityBoostTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerPriorityBoostTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerThreadCollectTimeoutFlag))
{
    Output::Print(u"-%s", u"RecyclerThreadCollectTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerThreadCollectTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerThreadCollectTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerThreadCollectTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerThreadCollectTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableConcurrentSweepAllocFlag))
{
    Output::Print(u"-%s", u"EnableConcurrentSweepAlloc");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableConcurrentSweepAllocFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableConcurrentSweepAllocFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableConcurrentSweepAllocFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableConcurrentSweepAllocFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ecsaFlag))
{
    Output::Print(u"-%s", u"ecsa");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ecsaFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ecsaFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ecsaFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ecsaFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapFlag))
{
    Output::Print(u"-%s", u"PageHeap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapAllocStackFlag))
{
    Output::Print(u"-%s", u"PageHeapAllocStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapAllocStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapAllocStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapAllocStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapAllocStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapFreeStackFlag))
{
    Output::Print(u"-%s", u"PageHeapFreeStack");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapFreeStackFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapFreeStackFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapFreeStackFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapFreeStackFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapBucketNumberFlag))
{
    Output::Print(u"-%s", u"PageHeapBucketNumber");
    switch (FlagNumberRange)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapBucketNumberFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapBucketNumberFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapBucketNumberFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapBucketNumberFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapBlockTypeFlag))
{
    Output::Print(u"-%s", u"PageHeapBlockType");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapBlockTypeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapBlockTypeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapBlockTypeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapBlockTypeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PageHeapDecommitGuardPageFlag))
{
    Output::Print(u"-%s", u"PageHeapDecommitGuardPage");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PageHeapDecommitGuardPageFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PageHeapDecommitGuardPageFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PageHeapDecommitGuardPageFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PageHeapDecommitGuardPageFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_NO_PAGE_REUSE
if (IsEnabled(RecyclerNoPageReuseFlag))
{
    Output::Print(u"-%s", u"RecyclerNoPageReuse");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerNoPageReuseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerNoPageReuseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerNoPageReuseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerNoPageReuseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_MEMORY_VERIFY
if (IsEnabled(RecyclerVerifyFlag))
{
    Output::Print(u"-%s", u"RecyclerVerify");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerVerifyPadSizeFlag))
{
    Output::Print(u"-%s", u"RecyclerVerifyPadSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyPadSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyPadSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyPadSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyPadSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(RecyclerTestFlag))
{
    Output::Print(u"-%s", u"RecyclerTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RecyclerProtectPagesOnRescanFlag))
{
    Output::Print(u"-%s", u"RecyclerProtectPagesOnRescan");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerProtectPagesOnRescanFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerProtectPagesOnRescanFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerProtectPagesOnRescanFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerProtectPagesOnRescanFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef RECYCLER_VERIFY_MARK
if (IsEnabled(RecyclerVerifyMarkFlag))
{
    Output::Print(u"-%s", u"RecyclerVerifyMark");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RecyclerVerifyMarkFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RecyclerVerifyMarkFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RecyclerVerifyMarkFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RecyclerVerifyMarkFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(LowMemoryCapFlag))
{
    Output::Print(u"-%s", u"LowMemoryCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LowMemoryCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LowMemoryCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LowMemoryCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LowMemoryCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(NewPagesCapDuringBGSweepingFlag))
{
    Output::Print(u"-%s", u"NewPagesCapDuringBGSweeping");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NewPagesCapDuringBGSweepingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NewPagesCapDuringBGSweepingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NewPagesCapDuringBGSweepingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NewPagesCapDuringBGSweepingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(AllocPolicyLimitFlag))
{
    Output::Print(u"-%s", u"AllocPolicyLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(AllocPolicyLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(AllocPolicyLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(AllocPolicyLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(AllocPolicyLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RuntimeDataOutputFileFlag))
{
    Output::Print(u"-%s", u"RuntimeDataOutputFile");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RuntimeDataOutputFileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RuntimeDataOutputFileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RuntimeDataOutputFileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RuntimeDataOutputFileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SpeculationCapFlag))
{
    Output::Print(u"-%s", u"SpeculationCap");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SpeculationCapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SpeculationCapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SpeculationCapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SpeculationCapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#if DBG_DUMP || defined(BGJIT_STATS) || defined(RECYCLER_STATS)
if (IsEnabled(StatsFlag))
{
    Output::Print(u"-%s", u"Stats");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StatsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StatsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StatsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StatsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if EXCEPTION_RECOVERY
if (IsEnabled(SwallowExceptionsFlag))
{
    Output::Print(u"-%s", u"SwallowExceptions");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SwallowExceptionsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SwallowExceptionsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SwallowExceptionsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SwallowExceptionsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(PrintSystemExceptionFlag))
{
    Output::Print(u"-%s", u"PrintSystemException");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintSystemExceptionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintSystemExceptionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintSystemExceptionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintSystemExceptionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(SwitchOptHolesThresholdFlag))
{
    Output::Print(u"-%s", u"SwitchOptHolesThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SwitchOptHolesThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SwitchOptHolesThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SwitchOptHolesThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SwitchOptHolesThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TempMinFlag))
{
    Output::Print(u"-%s", u"TempMin");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TempMinFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TempMinFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TempMinFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TempMinFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TempMaxFlag))
{
    Output::Print(u"-%s", u"TempMax");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TempMaxFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TempMaxFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TempMaxFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TempMaxFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceFlag))
{
    Output::Print(u"-%s", u"Trace");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_X64)
if (IsEnabled(LoopAlignNopLimitFlag))
{
    Output::Print(u"-%s", u"LoopAlignNopLimit");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(LoopAlignNopLimitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(LoopAlignNopLimitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(LoopAlignNopLimitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(LoopAlignNopLimitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#ifdef PROFILE_MEM
if (IsEnabled(TraceMemoryFlag))
{
    Output::Print(u"-%s", u"TraceMemory");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceMemoryFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceMemoryFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceMemoryFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceMemoryFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
if (IsEnabled(TraceMetaDataParsingFlag))
{
    Output::Print(u"-%s", u"TraceMetaDataParsing");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceMetaDataParsingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceMetaDataParsingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceMetaDataParsingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceMetaDataParsingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceWin8AllocationsFlag))
{
    Output::Print(u"-%s", u"TraceWin8Allocations");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWin8AllocationsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWin8AllocationsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWin8AllocationsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWin8AllocationsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceWin8DeallocationsImmediateFlag))
{
    Output::Print(u"-%s", u"TraceWin8DeallocationsImmediate");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceWin8DeallocationsImmediateFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceWin8DeallocationsImmediateFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceWin8DeallocationsImmediateFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceWin8DeallocationsImmediateFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PrintWin8StatsDetailedFlag))
{
    Output::Print(u"-%s", u"PrintWin8StatsDetailed");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PrintWin8StatsDetailedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PrintWin8StatsDetailedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PrintWin8StatsDetailedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PrintWin8StatsDetailedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TraceProtectPagesFlag))
{
    Output::Print(u"-%s", u"TraceProtectPages");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceProtectPagesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceProtectPagesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceProtectPagesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceProtectPagesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(TraceAsyncDebugCallsFlag))
{
    Output::Print(u"-%s", u"TraceAsyncDebugCalls");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceAsyncDebugCallsFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceAsyncDebugCallsFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceAsyncDebugCallsFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceAsyncDebugCallsFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef TRACK_DISPATCH
if (IsEnabled(TrackDispatchFlag))
{
    Output::Print(u"-%s", u"TrackDispatch");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TrackDispatchFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TrackDispatchFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TrackDispatchFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TrackDispatchFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(VerboseFlag))
{
    Output::Print(u"-%s", u"Verbose");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VerboseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VerboseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VerboseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VerboseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(UseFullNameFlag))
{
    Output::Print(u"-%s", u"UseFullName");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(UseFullNameFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(UseFullNameFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(UseFullNameFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(UseFullNameFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(Utf8Flag))
{
    Output::Print(u"-%s", u"Utf8");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(Utf8Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(Utf8Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(Utf8Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(Utf8Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(VersionFlag))
{
    Output::Print(u"-%s", u"Version");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VersionFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VersionFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VersionFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VersionFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WERExceptionSupportFlag))
{
    Output::Print(u"-%s", u"WERExceptionSupport");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WERExceptionSupportFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WERExceptionSupportFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WERExceptionSupportFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WERExceptionSupportFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ExtendedErrorStackForTestHostFlag))
{
    Output::Print(u"-%s", u"ExtendedErrorStackForTestHost");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ExtendedErrorStackForTestHostFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ExtendedErrorStackForTestHostFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ExtendedErrorStackForTestHostFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ExtendedErrorStackForTestHostFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(errorStackTraceFlag))
{
    Output::Print(u"-%s", u"errorStackTrace");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(errorStackTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(errorStackTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(errorStackTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(errorStackTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DoHeapEnumOnEngineShutdownFlag))
{
    Output::Print(u"-%s", u"DoHeapEnumOnEngineShutdown");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DoHeapEnumOnEngineShutdownFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DoHeapEnumOnEngineShutdownFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DoHeapEnumOnEngineShutdownFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DoHeapEnumOnEngineShutdownFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef HEAP_ENUMERATION_VALIDATION
if (IsEnabled(ValidateHeapEnumFlag))
{
    Output::Print(u"-%s", u"ValidateHeapEnum");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ValidateHeapEnumFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ValidateHeapEnumFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ValidateHeapEnumFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ValidateHeapEnumFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
if (IsEnabled(RegexTracingFlag))
{
    Output::Print(u"-%s", u"RegexTracing");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexTracingFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexTracingFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexTracingFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexTracingFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexProfileFlag))
{
    Output::Print(u"-%s", u"RegexProfile");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexProfileFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexProfileFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexProfileFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexProfileFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugFlag))
{
    Output::Print(u"-%s", u"RegexDebug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugASTFlag))
{
    Output::Print(u"-%s", u"RegexDebugAST");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugASTFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugASTFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugASTFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugASTFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexDebugAnnotatedASTFlag))
{
    Output::Print(u"-%s", u"RegexDebugAnnotatedAST");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexDebugAnnotatedASTFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexDebugAnnotatedASTFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexDebugAnnotatedASTFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexDebugAnnotatedASTFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexBytecodeDebugFlag))
{
    Output::Print(u"-%s", u"RegexBytecodeDebug");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexBytecodeDebugFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexBytecodeDebugFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexBytecodeDebugFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexBytecodeDebugFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RegexOptimizeFlag))
{
    Output::Print(u"-%s", u"RegexOptimize");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RegexOptimizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RegexOptimizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RegexOptimizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RegexOptimizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DynamicRegexMruListSizeFlag))
{
    Output::Print(u"-%s", u"DynamicRegexMruListSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DynamicRegexMruListSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DynamicRegexMruListSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DynamicRegexMruListSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DynamicRegexMruListSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(OptimizeForManyInstancesFlag))
{
    Output::Print(u"-%s", u"OptimizeForManyInstances");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(OptimizeForManyInstancesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(OptimizeForManyInstancesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(OptimizeForManyInstancesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(OptimizeForManyInstancesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableArrayTypeMutationFlag))
{
    Output::Print(u"-%s", u"EnableArrayTypeMutation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableArrayTypeMutationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableArrayTypeMutationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableArrayTypeMutationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableArrayTypeMutationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ArrayMutationTestSeedFlag))
{
    Output::Print(u"-%s", u"ArrayMutationTestSeed");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ArrayMutationTestSeedFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ArrayMutationTestSeedFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ArrayMutationTestSeedFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ArrayMutationTestSeedFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TestTraceFlag))
{
    Output::Print(u"-%s", u"TestTrace");
    switch (FlagPhases)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TestTraceFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TestTraceFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TestTraceFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TestTraceFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableEvalMapCleanupFlag))
{
    Output::Print(u"-%s", u"EnableEvalMapCleanup");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableEvalMapCleanupFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableEvalMapCleanupFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableEvalMapCleanupFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableEvalMapCleanupFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef PROFILE_MEM
if (IsEnabled(TraceObjectAllocationFlag))
{
    Output::Print(u"-%s", u"TraceObjectAllocation");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TraceObjectAllocationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TraceObjectAllocationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TraceObjectAllocationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TraceObjectAllocationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(SseFlag))
{
    Output::Print(u"-%s", u"Sse");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SseFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SseFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SseFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SseFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DeletedPropertyReuseThresholdFlag))
{
    Output::Print(u"-%s", u"DeletedPropertyReuseThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DeletedPropertyReuseThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DeletedPropertyReuseThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DeletedPropertyReuseThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DeletedPropertyReuseThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceStringKeyedSimpleDictionaryTypeHandlerFlag))
{
    Output::Print(u"-%s", u"ForceStringKeyedSimpleDictionaryTypeHandler");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceStringKeyedSimpleDictionaryTypeHandlerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceStringKeyedSimpleDictionaryTypeHandlerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceStringKeyedSimpleDictionaryTypeHandlerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceStringKeyedSimpleDictionaryTypeHandlerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BigDictionaryTypeHandlerThresholdFlag))
{
    Output::Print(u"-%s", u"BigDictionaryTypeHandlerThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BigDictionaryTypeHandlerThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BigDictionaryTypeHandlerThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BigDictionaryTypeHandlerThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BigDictionaryTypeHandlerThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(TypeSnapshotEnumerationFlag))
{
    Output::Print(u"-%s", u"TypeSnapshotEnumeration");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(TypeSnapshotEnumerationFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(TypeSnapshotEnumerationFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(TypeSnapshotEnumerationFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(TypeSnapshotEnumerationFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(IsolatePrototypesFlag))
{
    Output::Print(u"-%s", u"IsolatePrototypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IsolatePrototypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IsolatePrototypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IsolatePrototypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IsolatePrototypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ChangeTypeOnProtoFlag))
{
    Output::Print(u"-%s", u"ChangeTypeOnProto");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ChangeTypeOnProtoFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ChangeTypeOnProtoFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ChangeTypeOnProtoFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ChangeTypeOnProtoFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ShareInlineCachesFlag))
{
    Output::Print(u"-%s", u"ShareInlineCaches");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ShareInlineCachesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ShareInlineCachesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ShareInlineCachesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ShareInlineCachesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DisableDebugObjectFlag))
{
    Output::Print(u"-%s", u"DisableDebugObject");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DisableDebugObjectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DisableDebugObjectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DisableDebugObjectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DisableDebugObjectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(DumpHeapFlag))
{
    Output::Print(u"-%s", u"DumpHeap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(DumpHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(DumpHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(DumpHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(DumpHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(autoProxyFlag))
{
    Output::Print(u"-%s", u"autoProxy");
    switch (FlagString)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(autoProxyFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(autoProxyFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(autoProxyFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(autoProxyFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PerfHintLevelFlag))
{
    Output::Print(u"-%s", u"PerfHintLevel");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PerfHintLevelFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PerfHintLevelFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PerfHintLevelFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PerfHintLevelFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
if (IsEnabled(MemProtectHeapFlag))
{
    Output::Print(u"-%s", u"MemProtectHeap");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef RECYCLER_STRESS
if (IsEnabled(MemProtectHeapStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapBackgroundStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapBackgroundStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapBackgroundStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapBackgroundStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapBackgroundStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapBackgroundStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapConcurrentStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapConcurrentStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapConcurrentStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapConcurrentStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapConcurrentStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapConcurrentStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapConcurrentRepeatStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapConcurrentRepeatStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapConcurrentRepeatStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapConcurrentRepeatStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapConcurrentRepeatStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapConcurrentRepeatStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MemProtectHeapPartialStressFlag))
{
    Output::Print(u"-%s", u"MemProtectHeapPartialStress");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MemProtectHeapPartialStressFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MemProtectHeapPartialStressFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MemProtectHeapPartialStressFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MemProtectHeapPartialStressFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
if (IsEnabled(FixPropsOnPathTypesFlag))
{
    Output::Print(u"-%s", u"FixPropsOnPathTypes");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(FixPropsOnPathTypesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(FixPropsOnPathTypesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(FixPropsOnPathTypesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(FixPropsOnPathTypesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif
if (IsEnabled(BailoutTraceFilterFlag))
{
    Output::Print(u"-%s", u"BailoutTraceFilter");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BailoutTraceFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BailoutTraceFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BailoutTraceFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BailoutTraceFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(RejitTraceFilterFlag))
{
    Output::Print(u"-%s", u"RejitTraceFilter");
    switch (FlagNumberSet)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(RejitTraceFilterFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(RejitTraceFilterFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(RejitTraceFilterFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(RejitTraceFilterFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// recycler heuristic flags
if (IsEnabled(MaxBackgroundFinishMarkCountFlag))
{
    Output::Print(u"-%s", u"MaxBackgroundFinishMarkCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxBackgroundFinishMarkCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxBackgroundFinishMarkCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxBackgroundFinishMarkCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxBackgroundFinishMarkCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(BackgroundFinishMarkWaitTimeFlag))
{
    Output::Print(u"-%s", u"BackgroundFinishMarkWaitTime");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(BackgroundFinishMarkWaitTimeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(BackgroundFinishMarkWaitTimeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(BackgroundFinishMarkWaitTimeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(BackgroundFinishMarkWaitTimeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MinBackgroundRepeatMarkRescanBytesFlag))
{
    Output::Print(u"-%s", u"MinBackgroundRepeatMarkRescanBytes");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MinBackgroundRepeatMarkRescanBytesFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MinBackgroundRepeatMarkRescanBytesFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MinBackgroundRepeatMarkRescanBytesFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MinBackgroundRepeatMarkRescanBytesFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if defined(_M_X64)
if (IsEnabled(ZeroMemoryWithNonTemporalStoreFlag))
{
    Output::Print(u"-%s", u"ZeroMemoryWithNonTemporalStore");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ZeroMemoryWithNonTemporalStoreFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ZeroMemoryWithNonTemporalStoreFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ZeroMemoryWithNonTemporalStoreFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ZeroMemoryWithNonTemporalStoreFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

// recycler memory restrict test flags
if (IsEnabled(MaxMarkStackPageCountFlag))
{
    Output::Print(u"-%s", u"MaxMarkStackPageCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxMarkStackPageCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxMarkStackPageCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxMarkStackPageCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxMarkStackPageCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(MaxTrackedObjectListCountFlag))
{
    Output::Print(u"-%s", u"MaxTrackedObjectListCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxTrackedObjectListCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxTrackedObjectListCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxTrackedObjectListCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxTrackedObjectListCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

// make the recycler page integration path easier to hit
if (IsEnabled(NumberAllocPlusSizeFlag))
{
    Output::Print(u"-%s", u"NumberAllocPlusSize");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(NumberAllocPlusSizeFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(NumberAllocPlusSizeFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(NumberAllocPlusSizeFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(NumberAllocPlusSizeFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if DBG
if (IsEnabled(InitializeInterpreterSlotsWithInvalidStackVarFlag))
{
    Output::Print(u"-%s", u"InitializeInterpreterSlotsWithInvalidStackVar");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InitializeInterpreterSlotsWithInvalidStackVarFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InitializeInterpreterSlotsWithInvalidStackVarFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InitializeInterpreterSlotsWithInvalidStackVarFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InitializeInterpreterSlotsWithInvalidStackVarFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

#if DBG
if (IsEnabled(PRNGSeed0Flag))
{
    Output::Print(u"-%s", u"PRNGSeed0");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PRNGSeed0Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PRNGSeed0Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PRNGSeed0Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PRNGSeed0Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(PRNGSeed1Flag))
{
    Output::Print(u"-%s", u"PRNGSeed1");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(PRNGSeed1Flag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(PRNGSeed1Flag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(PRNGSeed1Flag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(PRNGSeed1Flag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(ClearInlineCachesOnCollectFlag))
{
    Output::Print(u"-%s", u"ClearInlineCachesOnCollect");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ClearInlineCachesOnCollectFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ClearInlineCachesOnCollectFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ClearInlineCachesOnCollectFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ClearInlineCachesOnCollectFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(InlineCacheInvalidationListCompactionThresholdFlag))
{
    Output::Print(u"-%s", u"InlineCacheInvalidationListCompactionThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(InlineCacheInvalidationListCompactionThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(InlineCacheInvalidationListCompactionThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(InlineCacheInvalidationListCompactionThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(InlineCacheInvalidationListCompactionThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ConstructorCacheInvalidationThresholdFlag))
{
    Output::Print(u"-%s", u"ConstructorCacheInvalidationThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ConstructorCacheInvalidationThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ConstructorCacheInvalidationThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ConstructorCacheInvalidationThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ConstructorCacheInvalidationThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#ifdef IR_VIEWER
if (IsEnabled(IRViewerFlag))
{
    Output::Print(u"-%s", u"IRViewer");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(IRViewerFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(IRViewerFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(IRViewerFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(IRViewerFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif /* IR_VIEWER */

if (IsEnabled(GCMemoryThresholdFlag))
{
    Output::Print(u"-%s", u"GCMemoryThreshold");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(GCMemoryThresholdFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(GCMemoryThresholdFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(GCMemoryThresholdFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(GCMemoryThresholdFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

#if DBG
    if (IsEnabled(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag))
{
    Output::Print(u"-%s", u"SimulatePolyCacheWithOneTypeForInlineCacheIndex");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
#endif

if (IsEnabled(JITServerIdleTimeoutFlag))
{
    Output::Print(u"-%s", u"JITServerIdleTimeout");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JITServerIdleTimeoutFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JITServerIdleTimeoutFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JITServerIdleTimeoutFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JITServerIdleTimeoutFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(JITServerMaxInactivePageAllocatorCountFlag))
{
    Output::Print(u"-%s", u"JITServerMaxInactivePageAllocatorCount");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(JITServerMaxInactivePageAllocatorCountFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(JITServerMaxInactivePageAllocatorCountFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(JITServerMaxInactivePageAllocatorCountFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(JITServerMaxInactivePageAllocatorCountFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(StrictWriteBarrierCheckFlag))
{
    Output::Print(u"-%s", u"StrictWriteBarrierCheck");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(StrictWriteBarrierCheckFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(StrictWriteBarrierCheckFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(StrictWriteBarrierCheckFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(StrictWriteBarrierCheckFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(WriteBarrierTestFlag))
{
    Output::Print(u"-%s", u"WriteBarrierTest");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(WriteBarrierTestFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(WriteBarrierTestFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(WriteBarrierTestFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(WriteBarrierTestFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(ForceSoftwareWriteBarrierFlag))
{
    Output::Print(u"-%s", u"ForceSoftwareWriteBarrier");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(ForceSoftwareWriteBarrierFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(ForceSoftwareWriteBarrierFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(ForceSoftwareWriteBarrierFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(ForceSoftwareWriteBarrierFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(VerifyBarrierBitFlag))
{
    Output::Print(u"-%s", u"VerifyBarrierBit");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(VerifyBarrierBitFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(VerifyBarrierBitFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(VerifyBarrierBitFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(VerifyBarrierBitFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(EnableBGFreeZeroFlag))
{
    Output::Print(u"-%s", u"EnableBGFreeZero");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(EnableBGFreeZeroFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(EnableBGFreeZeroFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(EnableBGFreeZeroFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(EnableBGFreeZeroFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
if (IsEnabled(KeepRecyclerTrackDataFlag))
{
    Output::Print(u"-%s", u"KeepRecyclerTrackData");
    switch (FlagBoolean)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(KeepRecyclerTrackDataFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(KeepRecyclerTrackDataFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(KeepRecyclerTrackDataFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(KeepRecyclerTrackDataFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}

if (IsEnabled(MaxSingleAllocSizeInMBFlag))
{
    Output::Print(u"-%s", u"MaxSingleAllocSizeInMB");
    switch (FlagNumber)
    {
    case FlagBoolean:
        if (!*GetAsBoolean(MaxSingleAllocSizeInMBFlag))
        {
            Output::Print(u"-");
        }
        break;
    case FlagString:
        if (GetAsString(MaxSingleAllocSizeInMBFlag) != nullptr)
        {
            Output::Print(u":%s", (const char16_t*)*GetAsString(MaxSingleAllocSizeInMBFlag));
        }
        break;
    case FlagNumber:
        Output::Print(u":%d", *GetAsNumber(MaxSingleAllocSizeInMBFlag));
        break;
    default:
        break;
    };
    Output::Print(u"\n");
}
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetDefaultValueAsBoolean
    ///
    /// Get the default value of a given boolean flag. If the flag is not of boolean
    /// type, will assert on CHK or return FALSE on FRE.
    ///
    ///----------------------------------------------------------------------------
    Boolean
    ConfigFlagsTable::GetDefaultValueAsBoolean(Flag flag) const
    {
        Boolean retValue = FALSE;

        switch (flag)
        {
        #if DBG
        case ArrayValidateFlag:
            retValue = (Boolean) false;
            break;
        case MemOpMissingValueValidateFlag:
            retValue = (Boolean) false;
            break;
        case OOPJITFixupValidateFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef ARENA_MEMORY_VERIFY
        case ArenaNoFreeListFlag:
            retValue = (Boolean) false;
            break;
        case ArenaNoPageReuseFlag:
            retValue = (Boolean) false;
            break;
        case ArenaUseHeapAllocFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case ValidateInlineStackFlag:
            retValue = (Boolean) false;
            break;
        case AsmDiffFlag:
            retValue = (Boolean) false;
            break;
        case AsmJsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJs;
            break;
        case AsmJsStopOnErrorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJsStopOnError;
            break;
        case AsmJsEdgeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsmJsEdge;
            break;
        case WasmFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Wasm;
            break;
        case WasmI64Flag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmI64;
            break;
        case WasmFastArrayFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmFastArray;
            break;
        case WasmSharedArrayVirtualBufferFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmSharedArrayVirtualBuffer;
            break;
        case WasmMathExFilterFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmMathExFilter;
            break;
        case WasmCheckVersionFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmCheckVersion;
            break;
        case WasmAssignModuleIDFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmAssignModuleID;
            break;
        case WasmIgnoreLimitsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmIgnoreLimits;
            break;
        case WasmFoldFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmFold;
            break;
        case WasmIgnoreResponseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmIgnoreResponse;
            break;
        case WasmThreadsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmThreads;
            break;
        case WasmMultiValueFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmMultiValue;
            break;
        case WasmSignExtendsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmSignExtends;
            break;
        case WasmNontrappingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmNontrapping;
            break;

        // WebAssembly Experimental Features
        // Master WasmExperimental flag to activate WebAssembly experimental features
        case WasmExperimentalFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WasmExperimental;
            break;

        // The default value of the experimental features will be off because the parent is off
        // Turning on the parent causes the child flag to take on their default value (aka on)
        // In Edge, we manually turn on the individual child flags
        // Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
        case WasmSimdFlag:
            retValue = (Boolean) true;
            break;

        case AssertBreakFlag:
            retValue = (Boolean) false;
            break;
        case AssertPopUpFlag:
            retValue = (Boolean) false;
            break;
        case AssertIgnoreFlag:
            retValue = (Boolean) false;
            break;
        case AsyncDebuggingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AsyncDebugging;
            break;
        case BaselineModeFlag:
            retValue = (Boolean) false;
            break;
        case BailOutAtEveryLineFlag:
            retValue = (Boolean) false;
            break;
        case BailOutAtEveryByteCodeFlag:
            retValue = (Boolean) false;
            break;
        case BailOutAtEveryImplicitCallFlag:
            retValue = (Boolean) false;
            break;
        case BenchmarkFlag:
            retValue = (Boolean) false;
            break;
        case BgJitFlag:
            retValue = (Boolean) true;
            break;
        case BgParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_BgParse;
            break;

        case CreateFunctionProxyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CreateFunctionProxy;
            break;
        case HybridFgJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_HybridFgJit;
            break;
        case BytecodeHistFlag:
            retValue = (Boolean) false;
            break;
        case CurrentSourceInfoFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CurrentSourceInfo;
            break;
        case CFGLogFlag:
            retValue = (Boolean) false;
            break;
        case CheckAlignmentFlag:
            retValue = (Boolean) false;
            break;
        case CheckEmitBufferPermissionsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef CHECK_MEMORY_LEAK
        case CheckMemoryLeakFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case CheckOpHelpersFlag:
            retValue = (Boolean) false;
            break;
        case CloneInlinedPolymorphicCachesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CloneInlinedPolymorphicCaches;
            break;
        case ConcurrentRuntimeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ConcurrentRuntime;
            break;
        case DebugFlag:
            retValue = (Boolean) false;
            break;

        case DebugWindowFlag:
            retValue = (Boolean) false;
            break;
        case ParserStateCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ParserStateCache;
            break;
        case CompressParserStateCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CompressParserStateCache;
            break;
        case DeferTopLevelTillFirstCallFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DeferTopLevelTillFirstCall;
            break;
        case DirectCallTelemetryStatsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DirectCallTelemetryStats;
            break;
        case DisableArrayBTreeFlag:
            retValue = (Boolean) false;
            break;
        case DisableRentalThreadingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DisableRentalThreading;
            break;
        case DisableVTuneSourceLineInfoFlag:
            retValue = (Boolean) false;
            break;
        case DisplayMemStatsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef DUMP_FRAGMENTATION_STATS
        case DumpFragmentationStatsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpIRAddressesFlag:
            retValue = (Boolean) false;
            break;
        case DumpLineNoInColorFlag:
            retValue = (Boolean) false;
            break;
        #ifdef RECYCLER_DUMP_OBJECT_GRAPH
        case DumpObjectGraphOnExitFlag:
            retValue = (Boolean) false;
            break;
        case DumpObjectGraphOnCollectFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpEvalStringOnRemovalFlag:
            retValue = (Boolean) false;
            break;
        case DumpObjectGraphOnEnumFlag:
            retValue = (Boolean) false;
            break;
        #ifdef EDIT_AND_CONTINUE
        case EditTestFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case WininetProfileCacheFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WininetProfileCache;
            break;
        case NoDynamicProfileInMemoryCacheFlag:
            retValue = (Boolean) false;
            break;
        case ProfileBasedSpeculativeJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ProfileBasedSpeculativeJit;
            break;
        case ExecuteByteCodeBufferReturnsInvalidByteCodeFlag:
            retValue = (Boolean) false;
            break;
        case SkipSplitOnNoResultFlag:
            retValue = (Boolean) DEFAULT_CONFIG_SkipSplitWhenResultIgnored;
            break;
        case Force32BitByteCodeFlag:
            retValue = (Boolean) false;
            break;

        case CollectGarbageFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CollectGarbage;
            break;

        case IntlFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Intl;
            break;
        case IntlBuiltInsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IntlBuiltIns;
            break;
        case IntlPlatformFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IntlPlatform;
            break;

        case JsBuiltInFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JsBuiltIn;
            break;
        case JitReproFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JitRepro;
            break;
        case EntryPointInfoRpcDataFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EntryPointInfoRpcData;
            break;

        case LdChakraLibFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LdChakraLib;
            break;
        case TestChakraLibFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TestChakraLib;
            break;

        // ES6 (BLUE+1) features/flags

        // Master ES6 flag to enable STABLE ES6 features/flags
        case ES6Flag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6;
            break;

        // Master ES6 flag to enable ALL sub ES6 features/flags
        case ES6AllFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6All;
            break;

        // Master ES6 flag to enable Threshold ES6 features/flags
        case ES6ExperimentalFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6All;
            break;

        // Per ES6 feature/flag

        case ES7AsyncAwaitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7AsyncAwait;
            break;
        case ES6DateParseFixFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6DateParseFix;
            break;
        case ES6FunctionNameFullFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6FunctionNameFull;
            break;
        case ES6GeneratorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Generators;
            break;
        case ES7ExponentiationOperatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7ExponentionOperator;
            break;

        case ES7ValuesEntriesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7ValuesEntries;
            break;
        case ES7TrailingCommaFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES7TrailingComma;
            break;
        case ES6IsConcatSpreadableFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6IsConcatSpreadable;
            break;
        case ES6MathFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Math;
            break;

        #ifndef COMPILE_DISABLE_ESDynamicImport
        #define COMPILE_DISABLE_ESDynamicImport 0
        #endif
        case ESDynamicImportFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESDynamicImport;
            break;

        case ES6ModuleFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Module;
            break;
        case ES6ObjectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Object;
            break;
        case ES6NumberFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Number;
            break;
        case ES6ObjectLiteralsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ObjectLiterals;
            break;
        case ES6ProxyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Proxy;
            break;
        case ES6RestFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Rest;
            break;
        case ES6SpreadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Spread;
            break;
        case ES6StringFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6String;
            break;
        case ES6StringPrototypeFixesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6StringPrototypeFixes;
            break;
        case ES2018ObjectRestSpreadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018ObjectRestSpread;
            break;

        case ES6PrototypeChainFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6PrototypeChain;
            break;
        case ES6ToPrimitiveFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToPrimitive;
            break;
        case ES6ToLengthFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToLength;
            break;
        case ES6ToStringTagFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6ToStringTag;
            break;
        case ES6UnicodeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Unicode;
            break;
        case ES6UnicodeVerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6UnicodeVerbose;
            break;
        case ES6UnscopablesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Unscopables;
            break;
        case ES6RegExStickyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExSticky;
            break;
        case ES2018RegExDotAllFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018RegExDotAll;
            break;
        case ESExportNsAsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESExportNsAs;
            break;
        case ES2018AsyncIterationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES2018AsyncIteration;
            break;
        case ESTopLevelAwaitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESTopLevelAwait;
            break;

        #ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
            #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
        #endif
        case ES6RegExPrototypePropertiesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExPrototypeProperties;
            break;

        #ifndef COMPILE_DISABLE_ES6RegExSymbols
            #define COMPILE_DISABLE_ES6RegExSymbols 0
        #endif

        // When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
        // sets implicit call flag before calling into script
        // Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
        case ES6RegExSymbolsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6RegExSymbols;
            break;

        case ES6VerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ES6Verbose;
            break;
        case ESObjectGetOwnPropertyDescriptorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESObjectGetOwnPropertyDescriptors;
            break;

        #ifndef COMPILE_DISABLE_ESSharedArrayBuffer
            #define COMPILE_DISABLE_ESSharedArrayBuffer 0
        #endif

        case ESSharedArrayBufferFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESSharedArrayBuffer;
            break;

        // ES BigInt flag
        case ESBigIntFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESBigInt;
            break;

        // ES Numeric Separator support for numeric constants
        case ESNumericSeparatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESNumericSeparator;
            break;

        // ES Nullish coalescing operator support (??)
        case ESNullishCoalescingOperatorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESNullishCoalescingOperator;
            break;

        // ES Hashbang support for interpreter directive syntax
        case ESHashbangFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESHashbang;
            break;

        // ES Symbol.prototype.description flag
        case ESSymbolDescriptionFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESSymbolDescription;
            break;

        case ESArrayFindFromLastFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESArrayFindFromLast;
            break;

        // ES Promise.any and AggregateError flag
        case ESPromiseAnyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESPromiseAny;
            break;

        // ES import.meta keyword meta-property
        case ESImportMetaFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESImportMeta;
            break;

        //ES globalThis flag
        case ESGlobalThisFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ESGlobalThis;
            break;

        // This flag to be removed once JITing generator functions is stable
        case JitES6GeneratorsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_JitES6Generators;
            break;

        case FastLineColumnCalculationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_FastLineColumnCalculation;
            break;
        case FreeRejittedCodeFlag:
            retValue = (Boolean) true;
            break;
        case ForceGuardPagesFlag:
            retValue = (Boolean) false;
            break;
        case PrintGuardPageBoundsFlag:
            retValue = (Boolean) false;
            break;
        case ForceLegacyEngineFlag:
            retValue = (Boolean) false;
            break;
        case ForceArrayBTreeFlag:
            retValue = (Boolean) false;
            break;
        case StrongArraySortFlag:
            retValue = (Boolean) DEFAULT_CONFIG_StrongArraySort;
            break;
        case ForceCleanPropertyOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceCleanPropertyOnCollect;
            break;
        case ForceCleanCacheOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceCleanCacheOnCollect;
            break;
        case ForceGCAfterJSONParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceGCAfterJSONParse;
            break;
        case ForceDecommitOnCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDecommitOnCollect;
            break;
        case ForceDeferParseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDeferParse;
            break;
        case ForceDiagnosticsModeFlag:
            retValue = (Boolean) false;
            break;
        case ForceGetWriteWatchOOMFlag:
            retValue = (Boolean) false;
            break;
        case ForcePostLowerGlobOptInstrStringFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString;
            break;
        case ForceSplitScopeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSplitScope;
            break;
        case EnumerateSpecialPropertiesInDebuggerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger;
            break;
        case EnableContinueAfterExceptionWrappersForHelpersFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForHelpers;
            break;
        case EnableContinueAfterExceptionWrappersForBuiltInsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableContinueAfterExceptionWrappersForBuiltIns;
            break;
        case EnableFunctionSourceReportForHeapEnumFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableFunctionSourceReportForHeapEnum;
            break;
        case ForceDynamicProfileFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceDynamicProfile;
            break;
        case ForceES5ArrayFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceES5Array;
            break;
        case ForceAsmJsLinkFailFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceAsmJsLinkFail;
            break;
        case ForceExpireOnNonCacheCollectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceExpireOnNonCacheCollect;
            break;
        case ForceFastPathFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceFastPath;
            break;
        case ForceFloatPrefFlag:
            retValue = (Boolean) false;
            break;
        case ForceJITLoopBodyFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceJITLoopBody;
            break;
        case ForceStaticInterpreterThunkFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceStaticInterpreterThunk;
            break;
        case DumpCommentsFromReferencedFilesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DumpCommentsFromReferencedFiles;
            break;
        case EnableFatalErrorOnOOMFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableFatalErrorOnOOM;
            break;

        #if defined(_M_ARM32_OR_ARM64)
        case ForceLocalsPtrFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DeferLoadingAvailableSourceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DeferLoadingAvailableSource;
            break;
        case ForceNativeFlag:
            retValue = (Boolean) false;
            break;
        case ForceSerializedFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSerialized;
            break;
        case ForceStrictModeFlag:
            retValue = (Boolean) false;
            break;
        case ForceUndoDeferFlag:
            retValue = (Boolean) false;
            break;
        case ForceBlockingConcurrentCollectFlag:
            retValue = (Boolean) false;
            break;
        case FreTestDiagModeFlag:
            retValue = (Boolean) false;
            break;
        #ifdef BYTECODE_TESTING
        case MediumByteCodeLayoutFlag:
            retValue = (Boolean) false;
            break;
        case LargeByteCodeLayoutFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case GenerateByteCodeBufferReturnsCantGenerateFlag:
            retValue = (Boolean) false;
            break;
        case HighPrecisionDateFlag:
            retValue = (Boolean) DEFAULT_CONFIG_HighPrecisionDate;
            break;

        case PrimeRecyclerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PrimeRecycler;
            break;
        case TraceEngineRefcountFlag:
            retValue = (Boolean) false;
            break;
        #if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
        case LeakStackTraceFlag:
            retValue = (Boolean) false;
            break;
        case ForceMemoryLeakFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case DumpAfterFinalGCFlag:
            retValue = (Boolean) false;
            break;
        case ForceOldDateAPIFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceOldDateAPI;
            break;

        case ForceMaxJitThreadCountFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceMaxJitThreadCount;
            break;

        case MitigateSpectreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_MitigateSpectre;
            break;

        case AddMaskingBlocksFlag:
            retValue = (Boolean) DEFAULT_CONFIG_AddMaskingBlocks;
            break;

        case PoisonVarArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonVarArrayLoad;
            break;
        case PoisonIntArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonIntArrayLoad;
            break;
        case PoisonFloatArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonFloatArrayLoad;
            break;
        case PoisonTypedArrayLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonTypedArrayLoad;
            break;
        case PoisonStringLoadFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonStringLoad;
            break;
        case PoisonObjectsForLoadsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonObjectsForLoads;
            break;

        case PoisonVarArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonVarArrayStore;
            break;
        case PoisonIntArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonIntArrayStore;
            break;
        case PoisonFloatArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonFloatArrayStore;
            break;
        case PoisonTypedArrayStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonTypedArrayStore;
            break;
        case PoisonStringStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonStringStore;
            break;
        case PoisonObjectsForStoresFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PoisonObjectsForStores;
            break;

        case EnforceExecutionModeLimitsFlag:
            retValue = (Boolean) false;
            break;
        case EemlFlag:
            retValue = (Boolean) false;
            break;

        case NewSimpleJitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_NewSimpleJit;
            break;

        case NoDeferParseFlag:
            retValue = (Boolean) false;
            break;
        case NoLogoFlag:
            retValue = (Boolean) false;
            break;
        case OOPJITMissingOptsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OOPJITMissingOpts;
            break;
        case CrashOnOOPJITFailureFlag:
            retValue = (Boolean) DEFAULT_CONFIG_CrashOnOOPJITFailure;
            break;
        case OOPCFGRegistrationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OOPCFGRegistration;
            break;
        case ForceJITCFGCheckFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceJITCFGCheck;
            break;
        case UseJITTrampolineFlag:
            retValue = (Boolean) DEFAULT_CONFIG_UseJITTrampoline;
            break;
        case NoNativeFlag:
            retValue = (Boolean) false;
            break;
        case NoStrictModeFlag:
            retValue = (Boolean) false;
            break;
        case NormalizeStatsFlag:
            retValue = (Boolean) false;
            break;
        #ifdef ENABLE_TRACE
        case InMemoryTraceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_InMemoryTrace;
            break;
        #ifdef STACK_BACK_TRACE
        case TraceWithStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TraceWithStack;
            break;
        #endif // STACK_BACK_TRACE
        #endif // ENABLE_TRACE
        case PrintRunTimeDataCollectionTraceFlag:
            retValue = (Boolean) false;
            break;
        #ifdef ENABLE_PREJIT
        case PrejitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Prejit;
            break;
        #endif
        case PrintSrcInDumpFlag:
            retValue = (Boolean) true;
            break;
        #ifdef PROFILE_OBJECT_LITERALS
        case ProfileObjectLiteralFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_STRINGS
        case ProfileStringsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_TYPES
        case ProfileTypesFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef PROFILE_EVALMAP
        case ProfileEvalMapFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #ifdef PROFILE_BAILOUT_RECORD_MEMORY
        case ProfileBailOutRecordMemoryFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #if DBG
        case ValidateIntRangesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ValidateIntRanges;
            break;
        #endif
        case LibraryStackFrameFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LibraryStackFrame;
            break;
        case LibraryStackFrameDebuggerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_LibraryStackFrameDebugger;
            break;
        #ifdef RECYCLER_STRESS
        case RecyclerStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerBackgroundStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerConcurrentStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerConcurrentRepeatStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerPartialStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerTrackStressFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerInduceFalsePositivesFlag:
            retValue = (Boolean) false;
            break;
        #endif // RECYCLER_STRESS
        case RecyclerForceMarkInteriorFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RecyclerForceMarkInterior;
            break;
        case EnableConcurrentSweepAllocFlag:
            retValue = (Boolean) true;
            break;
        case ecsaFlag:
            retValue = (Boolean) true;
            break;
        case PageHeapAllocStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PageHeapAllocStack;
            break;
        case PageHeapFreeStackFlag:
            retValue = (Boolean) DEFAULT_CONFIG_PageHeapFreeStack;
            break;
        case PageHeapDecommitGuardPageFlag:
            retValue = (Boolean) true;
            break;
        #ifdef RECYCLER_NO_PAGE_REUSE
        case RecyclerNoPageReuseFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case RecyclerTestFlag:
            retValue = (Boolean) false;
            break;
        case RecyclerProtectPagesOnRescanFlag:
            retValue = (Boolean) false;
            break;
        #ifdef RECYCLER_VERIFY_MARK
        case RecyclerVerifyMarkFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #if EXCEPTION_RECOVERY
        case SwallowExceptionsFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case PrintSystemExceptionFlag:
            retValue = (Boolean) false;
            break;


        #if DBG_DUMP || defined(RECYCLER_TRACE)
        //TraceMetaDataParsing flag with optional levels:
        //    Level 1 = interfaces only
        //    Level 2 = interfaces and methods
        //    Level 3 = interfaces, methods and parameters
        //    Level 4 = interfaces and properties
        //    Level 5 (default) = ALL
        case TraceWin8AllocationsFlag:
            retValue = (Boolean) false;
            break;
        case TraceWin8DeallocationsImmediateFlag:
            retValue = (Boolean) false;
            break;
        case PrintWin8StatsDetailedFlag:
            retValue = (Boolean) false;
            break;
        case TraceProtectPagesFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case TraceAsyncDebugCallsFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TraceAsyncDebugCalls;
            break;
        #ifdef TRACK_DISPATCH
        case TrackDispatchFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case VerboseFlag:
            retValue = (Boolean) DEFAULT_CONFIG_Verbose;
            break;
        case UseFullNameFlag:
            retValue = (Boolean) DEFAULT_CONFIG_UseFullName;
            break;
        case Utf8Flag:
            retValue = (Boolean) false;
            break;
        case WERExceptionSupportFlag:
            retValue = (Boolean) false;
            break;
        case ExtendedErrorStackForTestHostFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ExtendedErrorStackForTestHost;
            break;
        case errorStackTraceFlag:
            retValue = (Boolean) DEFAULT_CONFIG_errorStackTrace;
            break;
        case DoHeapEnumOnEngineShutdownFlag:
            retValue = (Boolean) false;
            break;
        #ifdef HEAP_ENUMERATION_VALIDATION
        case ValidateHeapEnumFlag:
            retValue = (Boolean) false;
            break;
        #endif

        #if ENABLE_REGEX_CONFIG_OPTIONS
        //
        // Regex flags
        //
        case RegexTracingFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexTracing;
            break;
        case RegexProfileFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexProfile;
            break;
        case RegexDebugFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebug;
            break;
        case RegexDebugASTFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebugAST;
            break;
        case RegexDebugAnnotatedASTFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexDebugAnnotatedAST;
            break;
        case RegexBytecodeDebugFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexBytecodeDebug;
            break;
        case RegexOptimizeFlag:
            retValue = (Boolean) DEFAULT_CONFIG_RegexOptimize;
            break;
        #endif

        case OptimizeForManyInstancesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_OptimizeForManyInstances;
            break;
        case EnableArrayTypeMutationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableArrayTypeMutation;
            break;
        case EnableEvalMapCleanupFlag:
            retValue = (Boolean) true;
            break;
        #ifdef PROFILE_MEM
        case TraceObjectAllocationFlag:
            retValue = (Boolean) false;
            break;
        #endif
        case ForceStringKeyedSimpleDictionaryTypeHandlerFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler;
            break;
        case TypeSnapshotEnumerationFlag:
            retValue = (Boolean) DEFAULT_CONFIG_TypeSnapshotEnumeration;
            break;
        case IsolatePrototypesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_IsolatePrototypes;
            break;
        case ChangeTypeOnProtoFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ChangeTypeOnProto;
            break;
        case ShareInlineCachesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ShareInlineCaches;
            break;
        case DisableDebugObjectFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DisableDebugObject;
            break;
        case DumpHeapFlag:
            retValue = (Boolean) DEFAULT_CONFIG_DumpHeap;
            break;
        #ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
        case MemProtectHeapFlag:
            retValue = (Boolean) DEFAULT_CONFIG_MemProtectHeap;
            break;
        #endif
        #ifdef RECYCLER_STRESS
        case MemProtectHeapStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapBackgroundStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapConcurrentStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapConcurrentRepeatStressFlag:
            retValue = (Boolean) false;
            break;
        case MemProtectHeapPartialStressFlag:
            retValue = (Boolean) false;
            break;
        #endif
        #ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
        case FixPropsOnPathTypesFlag:
            retValue = (Boolean) DEFAULT_CONFIG_FixPropsOnPathTypes;
            break;
        #endif

        #if defined(_M_X64)
        case ZeroMemoryWithNonTemporalStoreFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ZeroMemoryWithNonTemporalStore;
            break;
        #endif

        #if DBG
        case InitializeInterpreterSlotsWithInvalidStackVarFlag:
            retValue = (Boolean) false;
            break;
        #endif

        case ClearInlineCachesOnCollectFlag:
            retValue = (Boolean) false;
            break;

        #ifdef IR_VIEWER
        case IRViewerFlag:
            retValue = (Boolean) false;
            break;
        #endif /* IR_VIEWER */

        case StrictWriteBarrierCheckFlag:
            retValue = (Boolean) DEFAULT_CONFIG_StrictWriteBarrierCheck;
            break;
        case WriteBarrierTestFlag:
            retValue = (Boolean) DEFAULT_CONFIG_WriteBarrierTest;
            break;
        case ForceSoftwareWriteBarrierFlag:
            retValue = (Boolean) DEFAULT_CONFIG_ForceSoftwareWriteBarrier;
            break;
        case VerifyBarrierBitFlag:
            retValue = (Boolean) DEFAULT_CONFIG_VerifyBarrierBit;
            break;
        case EnableBGFreeZeroFlag:
            retValue = (Boolean) DEFAULT_CONFIG_EnableBGFreeZero;
            break;
        case KeepRecyclerTrackDataFlag:
            retValue = (Boolean) DEFAULT_CONFIG_KeepRecyclerTrackData;
            break;

        default:
            // not found - or not a boolean flag
            Assert(false);
        }

        return retValue;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::SetAsBoolean
    ///
    /// Set the value of a boolean flag. If the flag is a parent flag, all children flag
    //  will be set accordingly.
    ///
    ///----------------------------------------------------------------------------
    void
    ConfigFlagsTable::SetAsBoolean(Flag flag, Boolean value)
    {
        AssertMsg(this->GetFlagType(flag) == FlagBoolean, "flag not a boolean type");

        Boolean* settingAsBoolean = this->GetAsBoolean(flag);
        Assert(settingAsBoolean != nullptr);

        Output::VerboseNote(u"FLAG %s = %d\n", FlagNames[(int) flag], value);
        *settingAsBoolean = value;

        // check if parent flag
        if (this->IsParentFlag(flag))
        {
            // parent flag, will iterate through all child flags
            Flag childFlag = GetNextChildFlag(flag, /* no currentChildFlag */ InvalidFlag);
            while (childFlag != InvalidFlag)
            {
                Boolean childDefaultValue = GetDefaultValueAsBoolean(childFlag);

                // if the parent flag is TRUE, the children flag values are based on their default values
                // if the parent flag is FALSE, the children flag values are FALSE (always - as disabled)
                Boolean childValue = value == TRUE ? childDefaultValue : FALSE;

                Output::VerboseNote(u"FLAG %s = %d - setting child flag %s = %d\n", FlagNames[(int) flag], value, FlagNames[(int) childFlag], childValue);
                this->SetAsBoolean(childFlag, childValue);

                // get next child flag
                childFlag = GetNextChildFlag(flag, /* currentChildFlag */ childFlag);
            }
        }

        // Master ES6 flag to enable ALL sub ES6 features/flags
        if( flag == ES6AllFlag )
        {
            this->FlagSetCallback_ES6All(value);
        }

        // Master ES6 flag to enable Threshold ES6 features/flags
        if( flag == ES6ExperimentalFlag )
        {
            this->FlagSetCallback_ES6Experimental(value);
        }
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetParentFlag
    ///
    /// Get the parent flag corresponding to the flag, if any, otherwise returns NoParentFlag
    ///
    ///----------------------------------------------------------------------------
    Flag
    ConfigFlagsTable::GetParentFlag(Flag flag) const
    {
        Flag parentFlag = FlagParents[(int)flag];

        return parentFlag;
    }

    ///----------------------------------------------------------------------------
    ///
    /// ConfigFlagsTable::GetNextChildFlag
    ///
    /// Get the next child flag for a given parent flag. If no currentChildFlag, use
    /// InvalidFlag or NoParentFlag as start iterator.
    ///
    ///----------------------------------------------------------------------------
    Flag
    ConfigFlagsTable::GetNextChildFlag(Flag parentFlag, Flag currentChildFlag)  const
    {
        // start at the current+1
        int startIndex = (int)currentChildFlag + 1;

        // otherwise start from beginning
        if (currentChildFlag == InvalidFlag || currentChildFlag == NoParentFlag)
        {
            // reset the start index
            startIndex = 0;
        }

        for(int i=startIndex; i < FlagCount; i++)
        {
            Flag currentFlag = (Flag)i;
            Flag parentFlagForCurrentFlag = GetParentFlag(currentFlag);

            if(parentFlagForCurrentFlag == parentFlag)
            {
                // found a match
                return currentFlag;
            }
        }

        // no more
        return InvalidFlag;
    }

    //
    // Special overrides for flags being set
    //
    void
    ConfigFlagsTable::FlagSetCallback_ES6All(Boolean value)
    {
        // iterate through all ES6 flags - and set them explicitly (except ES6Verbose)
        Flag parentFlag = ES6Flag;

        // parent ES6 flag, will iterate through all child ES6 flags
        Flag childFlag = GetNextChildFlag(parentFlag, /* no currentChildFlag */ InvalidFlag);
        while (childFlag != InvalidFlag)
        {
            // skip verbose
            if (childFlag != ES6VerboseFlag)
            {
                Boolean childValue = value;

                Output::VerboseNote(u"FLAG %s = %d - setting child flag %s = %d\n", FlagNames[(int) parentFlag], value, FlagNames[(int) childFlag], childValue);
                this->SetAsBoolean(childFlag, childValue);
            }

            // get next child flag
            childFlag = GetNextChildFlag(parentFlag, /* currentChildFlag */ childFlag);
        }
    }

    void
    ConfigFlagsTable::FlagSetCallback_ES6Experimental(Boolean value)
    {
        if (value)
        {
            EnableExperimentalFlag();
        }
    }


    void
    ConfigFlagsTable::EnableExperimentalFlag()
    {
        std::unique_lock autocs(csExperimentalFlags);
        this->SetAsBoolean(Js::Flag::WasmSimdFlag, true);
        this->SetAsBoolean(Js::Flag::ESDynamicImportFlag, true);
        this->SetAsBoolean(Js::Flag::ES6RegExPrototypePropertiesFlag, true);
        this->SetAsBoolean(Js::Flag::ES6RegExSymbolsFlag, true);
        this->SetAsBoolean(Js::Flag::ESSharedArrayBufferFlag, true);
    }

    //
    // Configuration options
    //

    Configuration::Configuration()
    {
    }

    Configuration        Configuration::Global;


} //namespace Js
