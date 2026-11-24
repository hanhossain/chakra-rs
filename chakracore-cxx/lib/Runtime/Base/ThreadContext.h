//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Entropy.h"
#include "DataStructures/Stack.h"
#include "DataStructures/Dictionary.h"
#include "Library/DelayFreeArrayBufferHelper.h"
#include "Base/AutoReentrancyHandler.h"

#include <memory>

namespace Js
{
    class ScriptContext;
    struct InlineCache;
    class CodeGenRecyclableData;
    class DelayedFreeArrayBuffer;
}

typedef BVSparse<ArenaAllocator> ActiveFunctionSet;

using namespace PlatformAgnostic;

struct IAuthorFileContext;

class HostScriptContext;
class ScriptSite;
class ThreadServiceWrapper;
struct IActiveScriptProfilerHeapEnum;
class StackProber;

enum DisableImplicitFlags : uint8_t
{
    DisableImplicitNoFlag               = 0x00,
    DisableImplicitCallFlag             = 0x01,
    DisableImplicitExceptionFlag        = 0x02,
    DisableImplicitCallAndExceptionFlag = DisableImplicitCallFlag | DisableImplicitExceptionFlag
};

enum ThreadContextFlags
{
    ThreadContextFlagNoFlag                        = 0x00000000,
    ThreadContextFlagCanDisableExecution           = 0x00000001,
    ThreadContextFlagEvalDisabled                  = 0x00000002,
    ThreadContextFlagNoJIT                         = 0x00000004,
    ThreadContextFlagDisableFatalOnOOM             = 0x00000008,
    ThreadContextFlagNoDynamicThunks               = 0x00000010,
};

const int LS_MAX_STACK_SIZE_KB = 300;

class ThreadContext;

class InterruptPoller
{
    // Interface with a polling object located in the hosting layer.

public:
    InterruptPoller(ThreadContext *tc);

    virtual ~InterruptPoller() { }

    void CheckInterruptPoll();
    void ResetStatementCount() { lastResetTick = lastPollTick; }
    void StartScript() { lastResetTick = lastPollTick = ::GetTickCount(); }
    void EndScript() { lastResetTick = lastPollTick = 0;}
    bool IsDisabled() const { return isDisabled; }
    void SetDisabled(bool disable) { isDisabled = disable; }

    virtual void TryInterruptPoll(Js::ScriptContext *scriptContext) = 0;

    // Default: throw up QC dialog after 5M statements == 2 minutes
    static const uint32_t TicksToStatements = (5000000 / 120000);

protected:
    ThreadContext *threadContext;
    uint32_t lastPollTick;
    uint32_t lastResetTick;
    bool isDisabled;
};

#define PROBE_STACK(scriptContext, size) ((scriptContext)->GetThreadContext()->ProbeStack(size, scriptContext))
#define PROBE_STACK_NO_DISPOSE(scriptContext, size) ((scriptContext)->GetThreadContext()->ProbeStackNoDispose(size, scriptContext))
#define PROBE_STACK_PARTIAL_INITIALIZED_INTERPRETER_FRAME(scriptContext, size) ((scriptContext)->GetThreadContext()->ProbeStack(size, scriptContext, __builtin_return_address(0)))
#define PROBE_STACK_PARTIAL_INITIALIZED_BAILOUT_FRAME(scriptContext, size, returnAddress) ((scriptContext)->GetThreadContext()->ProbeStack(size, scriptContext, returnAddress))
#define PROBE_STACK_CALL(scriptContext, obj, size) ((scriptContext)->GetThreadContext()->ProbeStack(size, obj, scriptContext))
#define AssertInScript() Assert(ThreadContext::GetContextForCurrentThread()->IsScriptActive());
#define AssertNotInScript() Assert(!ThreadContext::GetContextForCurrentThread()->IsScriptActive());

#define LEAVE_SCRIPT_START_EX(scriptContext, stackProbe, leaveForHost, isFPUControlRestoreNeeded) \
        { \
            void * __frameAddr = nullptr; \
            GET_CURRENT_FRAME_ID(__frameAddr); \
            Js::LeaveScriptObject<stackProbe, leaveForHost, isFPUControlRestoreNeeded> __leaveScriptObject(scriptContext, __frameAddr); \
            AutoReentrancyHandler autoReentrancyHandler(scriptContext->GetThreadContext());
#define LEAVE_SCRIPT_END_EX(scriptContext) \
            if (scriptContext != nullptr) \
                {   \
                    scriptContext->GetThreadContext()->DisposeOnLeaveScript(); \
                }\
        }

#define LEAVE_SCRIPT_IF_ACTIVE(scriptContext, externalCall) \
        if (scriptContext->GetThreadContext()->IsScriptActive()) \
        { \
            BEGIN_LEAVE_SCRIPT(scriptContext); \
            externalCall \
            END_LEAVE_SCRIPT(scriptContext); \
        } \
        else \
        { \
            DECLARE_EXCEPTION_CHECK_DATA \
            SAVE_EXCEPTION_CHECK \
            externalCall \
            RESTORE_EXCEPTION_CHECK \
        }

#define ENTER_SCRIPT_IF(scriptContext, doCleanup, isCallRoot, hasCaller, condition, block) \
        if (condition) \
        { \
            BEGIN_ENTER_SCRIPT(scriptContext, doCleanup, isCallRoot, hasCaller); \
            block \
            END_ENTER_SCRIPT \
        } \
        else \
        { \
            block \
        }

#define BEGIN_LEAVE_SCRIPT(scriptContext) \
        LEAVE_SCRIPT_START_EX(scriptContext, /* stackProbe */ true, /* leaveForHost */ true, /* isFPUControlRestoreNeeded */ false)

#define BEGIN_LEAVE_SCRIPT_SAVE_FPU_CONTROL(scriptContext) \
        LEAVE_SCRIPT_START_EX(scriptContext, /* stackProbe */ true, /* leaveForHost */ true, /* isFPUControlRestoreNeeded */ true)

// BEGIN_LEAVE_SCRIPT_INTERNAL is used when there are no explicit external call after leave script,
// but we might have external call when allocation memory doing QC or GC Dispose, which may enter script again.
// This will record the reentry as an implicit call (ImplicitCall_AsyncHostOperation)
#define BEGIN_LEAVE_SCRIPT_INTERNAL(scriptContext) \
        LEAVE_SCRIPT_START_EX(scriptContext, /* stackProbe */ true, /* leaveForHost */ false, /* isFPUControlRestoreNeeded */ false)

#define BEGIN_LEAVE_SCRIPT_NO_STACK_PROBE(scriptContext) \
        LEAVE_SCRIPT_START_EX(scriptContext, /* stackProbe */ false, /* leaveForHost */ true, /* isFPUControlRestoreNeeded */ false)

#define END_LEAVE_SCRIPT(scriptContext) \
        LEAVE_SCRIPT_END_EX(scriptContext)

#define END_LEAVE_SCRIPT_RESTORE_FPU_CONTROL(scriptContext) \
        LEAVE_SCRIPT_END_EX(scriptContext)

#define END_LEAVE_SCRIPT_INTERNAL(scriptContext) \
        LEAVE_SCRIPT_END_EX(scriptContext)

#define END_LEAVE_SCRIPT_NO_STACK_PROBE(scriptContext) \
        LEAVE_SCRIPT_END_EX(scriptContext)

#define BEGIN_LEAVE_SCRIPT_WITH_EXCEPTION(scriptContext) \
        BEGIN_LEAVE_SCRIPT(scriptContext)

#define END_LEAVE_SCRIPT_WITH_EXCEPTION(scriptContext) \
        Assert(!scriptContext->HasRecordedException()); \
        END_LEAVE_SCRIPT(scriptContext)

#define BEGIN_SAFE_REENTRANT_CALL(threadContext) \
    { \
        AutoReentrancyHandler autoReentrancyHandler(threadContext);

#define END_SAFE_REENTRANT_CALL }

// Keep in sync with CollectGarbageCallBackFlags in scriptdirect.idl

enum RecyclerCollectCallBackFlags
{
    Collect_Begin                    = 0x01,
    Collect_Begin_Concurrent         = 0x11,
    Collect_Begin_Partial            = 0x21,
    Collect_Begin_Concurrent_Partial = Collect_Begin_Concurrent | Collect_Begin_Partial,
    Collect_End                      = 0x02,
    Collect_Wait                     = 0x04,     // callback can be from another thread
    Collect_Begin_Sweep              = 0x08
};
typedef void (*RecyclerCollectCallBackFunction)(void * context, RecyclerCollectCallBackFlags flags);

class NativeLibraryEntryRecord
{
public:
    struct Entry
    {
        Js::RecyclableObject* function;
        Js::CallInfo callInfo;
        const char16_t * name;
        void * addr;
        Entry* next;
    };

private:
    Entry* head;

public:
    NativeLibraryEntryRecord() : head(nullptr)
    {
    }

    const Entry* Peek() const
    {
        return head;
    }

    void Push(_In_ Entry* e)
    {
        e->next = head;
        head = e;
    }

    void Pop()
    {
        head = head->next;
    }
};

class AutoTagNativeLibraryEntry
{
private:
    NativeLibraryEntryRecord::Entry entry;

public:
    AutoTagNativeLibraryEntry(Js::RecyclableObject* function, Js::CallInfo callInfo, const char16_t * name, void* addr);
    ~AutoTagNativeLibraryEntry();
};

#define AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, name) \
    AutoTagNativeLibraryEntry __tag(function, callInfo, name, _AddressOfReturnAddress())

class ThreadConfiguration
{
public:
    ThreadConfiguration(bool enableExperimentalFeatures)
    {
        CopyGlobalFlags();
        if (enableExperimentalFeatures)
        {
            EnableExperimentalFeatures();
            ResetExperimentalFeaturesFromConfig();
        }
    }

#define DEFINE_FLAG(threadFlag, globalFlag) \
    public: \
        inline bool threadFlag() const { return m_##globalFlag; } \
    \
    private: \
        bool m_##globalFlag;
#define FLAG(threadFlag, globalFlag) DEFINE_FLAG(threadFlag, globalFlag)
#define FLAG_RELEASE(threadFlag, globalFlag) DEFINE_FLAG(threadFlag, globalFlag)
#include "ThreadConfigFlagsList.h"
#undef FLAG_RELEASE
#undef FLAG
#undef DEFINE_FLAG

private:
    void CopyGlobalFlags()
    {
        std::unique_lock autocs(Js::Configuration::Global.flags.csExperimentalFlags);

#define FLAG(threadFlag, globalFlag) m_##globalFlag = CONFIG_FLAG(globalFlag);
#define FLAG_RELEASE(threadFlag, globalFlag) m_##globalFlag = CONFIG_FLAG_RELEASE(globalFlag);
#include "ThreadConfigFlagsList.h"
#undef FLAG_RELEASE
#undef FLAG
    }

    void EnableExperimentalFeatures()
    {
        // If a ES6 flag is disabled using compile flag don't enable it
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
#define DEFAULT_CONFIG_WasmMathExFilter     (false)
#define DEFAULT_CONFIG_WasmIgnoreResponse   (false)
#define DEFAULT_CONFIG_WasmMaxTableSize     (10000000)
#define DEFAULT_CONFIG_WasmThreads          (false)
#define DEFAULT_CONFIG_WasmMultiValue       (false)
#define DEFAULT_CONFIG_WasmSignExtends      (true)
#define DEFAULT_CONFIG_WasmNontrapping      (true)
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
#define DEFAULT_CONFIG_AsmJsInlineAdjust (35)                // wasm functions are cheaper to inline, so worth being more aggressive
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

#define DEFAULT_CONFIG_HighPrecisionDate    (false)
#define DEFAULT_CONFIG_ForceOldDateAPI      (false)
#define DEFAULT_CONFIG_Loop                 (1)
#define DEFAULT_CONFIG_ForceDiagnosticsMode (false)
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

#define DEFAULT_CONFIG_MaxCodeFill          (500)
#define DEFAULT_CONFIG_ProfileBasedSpeculationCap (1600)
#define DEFAULT_CONFIG_ForceStrictMode      (false)
#define DEFAULT_CONFIG_ExpirableCollectionGCCount (5)  // Number of GCs during which entry point profiling occurs
#define DEFAULT_CONFIG_ExpirableCollectionTriggerThreshold (50)  // Threshold at which Entry Point Collection is triggered
#define DEFAULT_CONFIG_RegexDebug           (false)
#define DEFAULT_CONFIG_RegexOptimize        (true)
#define DEFAULT_CONFIG_GoptCleanupThreshold  (25)
#define DEFAULT_CONFIG_AsmGoptCleanupThreshold  (500)

#define DEFAULT_CONFIG_DeferParseThreshold             (4 * 1024) // Unit is number of characters
#define DEFAULT_CONFIG_ProfileBasedDeferParseThreshold (100)      // Unit is number of characters

#define DEFAULT_CONFIG_ProfileBasedSpeculativeJit (true)
#define DEFAULT_CONFIG_WininetProfileCache        (true)
#define DEFAULT_CONFIG_MinProfileCacheSize        (5)   // Minimum number of functions before profile is saved.
#define DEFAULT_CONFIG_ProfileDifferencePercent   (15)  // If 15% of the functions have different profile we will trigger a save.

#define DEFAULT_CONFIG_Intl                    (true)
#define DEFAULT_CONFIG_IntlBuiltIns            (true)
#define DEFAULT_CONFIG_IntlPlatform            (false) // Makes the EngineExtension.Intl object visible to user code as Intl.platform, meant for testing

#define DEFAULT_CONFIG_JsBuiltIn             (true)
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

#define DEFAULT_CONFIG_ForcePostLowerGlobOptInstrString (false)
#define DEFAULT_CONFIG_EnumerateSpecialPropertiesInDebugger (true)

#define DEFAULT_CONFIG_JitQueueThreshold      (6)

#define DEFAULT_CONFIG_FullJitRequeueThreshold (25)     // Minimum number of times a function needs to be executed before it is re-added to the jit queue

#define DEFAULT_CONFIG_MinTemplatizedJitRunCount      (100)     // Minimum number of times a function needs to be interpreted before it is jitted
#define DEFAULT_CONFIG_MinAsmJsInterpreterRunCount      (10)     // Minimum number of times a function needs to be Asm interpreted before it is jitted
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

#define DEFAULT_CONFIG_MaxLinearIntCaseCount     (3)       // Maximum number of cases (in switch statement) for which instructions can be generated linearly.
#define DEFAULT_CONFIG_MinSwitchJumpTableSize   (9)     // Minimum number of case target entries in the jump table(this may also include values that are missing in the consecutive set of integer case arms)
#define DEFAULT_CONFIG_MaxLinearStringCaseCount (4)     // Maximum number of String cases (in switch statement) for which instructions can be generated linearly.

#define DEFAULT_CONFIG_MinBailOutsBeforeRejit 2         // Minimum number of bailouts for a single bailout record after which a rejit is considered

#define DEFAULT_CONFIG_Sse                  (-1)

#define DEFAULT_CONFIG_BigDictionaryTypeHandlerThreshold (0xffff)
#define DEFAULT_CONFIG_ForceStringKeyedSimpleDictionaryTypeHandler (false)
#define DEFAULT_CONFIG_PrimeRecycler     (false)
#define DEFAULT_CONFIG_DisableRentalThreading (false)
#define DEFAULT_CONFIG_PerfHintLevel (1)
#define DEFAULT_CONFIG_OOPJITMissingOpts (false)
#define DEFAULT_CONFIG_ForceJITCFGCheck (false)
#define DEFAULT_CONFIG_UseJITTrampoline (true)

#define DEFAULT_CONFIG_RichTraceFormat              (false)

#define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameError (0)
#define DEFAULT_CONFIG_InjectPartiallyInitializedInterpreterFrameErrorType (0)

#define DEFAULT_CONFIG_DeferLoadingAvailableSource  (false)

#define DEFAULT_CONFIG_InduceCodeGenFailure (30) // When -InduceCodeGenFailure is passed in, 30% of JIT allocations will fail

#define DEFAULT_CONFIG_SkipSplitWhenResultIgnored (false)

#define DEFAULT_CONFIG_MinMemOpCount (16U)

#if ENABLE_COPYONACCESS_ARRAY
#define DEFAULT_CONFIG_MaxCopyOnAccessArrayLength (32U)
#define DEFAULT_CONFIG_MinCopyOnAccessArrayLength (5U)
#define DEFAULT_CONFIG_CopyOnAccessArraySegmentCacheSize (16U)
#endif

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

#ifndef COMPILE_DISABLE_ESDynamicImport
#define COMPILE_DISABLE_ESDynamicImport 0
#endif
m_ESDynamicImport= COMPILE_DISABLE_ESDynamicImport? false : true;

#ifndef COMPILE_DISABLE_ES6RegExPrototypeProperties
    #define COMPILE_DISABLE_ES6RegExPrototypeProperties 0
#endif
m_ES6RegExPrototypeProperties= COMPILE_DISABLE_ES6RegExPrototypeProperties? false : true;

#ifndef COMPILE_DISABLE_ES6RegExSymbols
    #define COMPILE_DISABLE_ES6RegExSymbols 0
#endif
m_ES6RegExSymbols= COMPILE_DISABLE_ES6RegExSymbols? false : true;

#ifndef COMPILE_DISABLE_ESSharedArrayBuffer
    #define COMPILE_DISABLE_ESSharedArrayBuffer 0
#endif
m_ESSharedArrayBuffer= COMPILE_DISABLE_ESSharedArrayBuffer? false : true;

// TODO (hanhossain): ConfigFlagsList end
    }

    void ResetExperimentalFeaturesFromConfig()
    {
        // If a flag was overridden using config/command line it should take precedence
        if (Js::Configuration::Global.flags.IsEnabled(Js::Flag::ESDynamicImportFlag))
        {
            m_ESDynamicImport = (Js::Configuration::Global.flags.ESDynamicImport);
        }
        if (Js::Configuration::Global.flags.IsEnabled(Js::Flag::ES6RegExPrototypePropertiesFlag))
        {
            m_ES6RegExPrototypeProperties = (Js::Configuration::Global.flags.ES6RegExPrototypeProperties);
        }
        if (Js::Configuration::Global.flags.IsEnabled(Js::Flag::ES6RegExSymbolsFlag))
        {
            m_ES6RegExSymbols = (Js::Configuration::Global.flags.ES6RegExSymbols);
        }
        if (Js::Configuration::Global.flags.IsEnabled(Js::Flag::ESSharedArrayBufferFlag))
        {
            m_ESSharedArrayBuffer = (Js::Configuration::Global.flags.ESSharedArrayBuffer);
        }
    }
};

class ThreadContext :
    public DefaultRecyclerCollectionWrapper,
    public JsUtil::DoublyLinkedListElement<ThreadContext>,
    public ThreadContextInfo
{
    struct PropertyGuardEntry;
public:
    static void GlobalInitialize();
    static const uint32_t NoThread = 0xFFFFFFFF;

    struct CollectCallBack
    {
        RecyclerCollectCallBackFunction callback;
        void * context;
    };

    struct AutoRestoreImplicitFlags
    {
        ThreadContext * threadContext;
        Js::ImplicitCallFlags savedImplicitCallFlags;
        DisableImplicitFlags savedDisableImplicitFlags;
        AutoRestoreImplicitFlags(ThreadContext *threadContext, Js::ImplicitCallFlags implicitCallFlags, DisableImplicitFlags disableImplicitFlags);
        ~AutoRestoreImplicitFlags();
    };

    void SetCurrentThreadId(uint32_t threadId);
    uint32_t GetCurrentThreadId() const;
    // Rental threading is always enabled for JSRT (i.e. not bound to a specific thread)
    void SetIsThreadBound();
    bool IsJSRT() const;
    bool IsThreadBound() const override;
    void SetStackProber(StackProber * stackProber);

    Js::ImplicitCallFlags TryWithDisabledImplicitCall(std::function<void()> fn);

    void * GetAddressOfStackLimitForCurrentThread() const;

    IActiveScriptProfilerHeapEnum * GetHeapEnum();
    void SetHeapEnum(IActiveScriptProfilerHeapEnum* newHeapEnum);
    void ClearHeapEnum();

    Js::PropertyRecord const *GetPropertyRecord(Js::PropertyId propertyId);

    bool IsNumericProperty(Js::PropertyId propertyId) override;

#ifdef ENABLE_WASM_SIMD
#if _M_AMD64
    // auxiliary SIMD values in memory to help JIT'ed code. E.g. used for Int8x16 shuffle.
    _x86_SIMDValue X86_TEMP_SIMD[SIMD_TEMP_SIZE];
    _x86_SIMDValue * GetSimdTempArea();
#endif
#endif

public:
    Js::PropertyRecord const * GetEmptyStringPropertyRecord();
    Js::PropertyId GetEmptyStringPropertyId();

private:
    const Js::PropertyRecord * emptyStringPropertyRecord;
    bool noScriptScope;

    static uint const MaxTemporaryArenaAllocators = 5;

    static std::recursive_mutex s_csThreadContext;

    StackProber * GetStackProber() const;
    size_t GetStackLimitForCurrentThread() const;
    void SetStackLimitForCurrentThread(size_t limit);

    // The current heap enumeration object being used during enumeration.
    IActiveScriptProfilerHeapEnum* heapEnum;

public:
    typedef JsUtil::BaseHashSet<const Js::PropertyRecord *, HeapAllocator, PowerOf2SizePolicy, const Js::PropertyRecord *,
        Js::PropertyRecordStringHashComparer, JsUtil::SimpleHashedEntry, JsUtil::AsymetricResizeLock> PropertyMap;
    PropertyMap * propertyMap;

    typedef JsUtil::BaseHashSet<Js::CaseInvariantPropertyListWithHashCode*, Recycler, PowerOf2SizePolicy, Js::CaseInvariantPropertyListWithHashCode*, JsUtil::NoCaseComparer, JsUtil::SimpleDictionaryEntry>
        PropertyNoCaseSetType;
    typedef JsUtil::WeaklyReferencedKeyDictionary<Js::Type, bool> TypeHashSet;
    typedef JsUtil::BaseDictionary<Js::PropertyId, TypeHashSet *, Recycler, PowerOf2SizePolicy> PropertyIdToTypeHashSetDictionary;
    typedef JsUtil::WeaklyReferencedKeyDictionary<const Js::PropertyRecord, PropertyGuardEntry*, Js::PropertyRecordPointerComparer> PropertyGuardDictionary;

private:
#if ENABLE_NATIVE_CODEGEN
    BVSparse<HeapAllocator> * m_jitNumericProperties;
#endif
    typedef JsUtil::BaseDictionary<uint, Js::SourceDynamicProfileManager*, Recycler, PowerOf2SizePolicy> SourceDynamicProfileManagerMap;
    typedef JsUtil::BaseDictionary<Js::HashedCharacterBuffer<char16_t>*, const Js::PropertyRecord*, Recycler, PowerOf2SizePolicy, Js::PropertyRecordStringHashComparer> SymbolRegistrationMap;

    class SourceDynamicProfileManagerCache
    {
    public:
        SourceDynamicProfileManagerCache() : refCount(0), sourceProfileManagerMap(nullptr) {}

        typename WriteBarrierFieldTypeTraits<SourceDynamicProfileManagerMap*>::Type sourceProfileManagerMap;
        void AddRef() { refCount++; }
        uint Release() { Assert(refCount > 0); return --refCount; }
    private:
        typename WriteBarrierFieldTypeTraits<uint>::Type refCount;              // For every script context using this cache, there is a ref count added.
    };

    typedef JsUtil::BaseDictionary<const char16_t*, SourceDynamicProfileManagerCache*, Recycler, PowerOf2SizePolicy> SourceProfileManagersByUrlMap;

    struct RecyclableData
    {
        RecyclableData(Recycler *const recycler);
        typename WriteBarrierFieldTypeTraits<Js::TempArenaAllocatorObject *>::Type temporaryArenaAllocators[MaxTemporaryArenaAllocators];
        typename WriteBarrierFieldTypeTraits<Js::TempGuestArenaAllocatorObject *>::Type temporaryGuestArenaAllocators[MaxTemporaryArenaAllocators];

        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject *>::Type pendingFinallyException;

        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject *>::Type exceptionObject;
        typename WriteBarrierFieldTypeTraits<bool>::Type propagateException;

        // We throw a JS catchable SO exception if we detect we might overflow the stack. Allocating this (JS)
        // object though might really overflow the stack. So use this thread global to identify them from the throw point
        // to where they are caught; where the stack has been unwound and it is safer to allocate the real exception
        // object and throw.
        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject>::Type soErrorObject;

        // We can't allocate an out of memory object...  So use this static as a way to identify
        // them from the throw point to where they are caught.
        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject>::Type oomErrorObject;

        // This is for JsRT scenario where a runtime is not usable after a suspend request, before a resume runtime call is made
        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject>::Type terminatedErrorObject;

        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject*>::Type unhandledExceptionObject;

        // Used to temporarily keep throwing exception object alive (thrown but not yet caught)
        typename WriteBarrierFieldTypeTraits<Js::JavascriptExceptionObject*>::Type tempUncaughtException;

        // Contains types that have property caches that need to be tracked, as the caches may need to be cleared. Types that
        // contain a property cache for a property that is on a prototype object will be tracked in this map since those caches
        // need to be cleared if for instance, the property is deleted from the prototype object.
        //
        // It is expected that over time, types that are deleted will eventually be removed by the weak reference hash sets when
        // they're searching through a bucket while registering a type or enumerating types to invalidate, or when a property ID
        // is reclaimed. If none of those happen, then this collection may contain weak reference handles to deleted objects
        // that would not get removed, but it would also not get any bigger.
        typename WriteBarrierFieldTypeTraits<PropertyIdToTypeHashSetDictionary>::Type typesWithProtoPropertyCache;

#if ENABLE_NATIVE_CODEGEN
        // The property guard dictionary contains property guards which need to be invalidated in response to properties changing
        // from writable to read-only and vice versa, properties being shadowed or unshadowed on prototypes, etc.  The dictionary
        // holds only weak references to property guards and their lifetimes are controlled by their creators (typically entry points).
        // When a guard is no longer needed it is garbage collected, but the weak references and dictionary entries remain, until
        // the guards for a given property get invalidated.
        // TODO: Create and use a self-cleaning weak reference dictionary, which would periodically remove any unused weak references.
        typename WriteBarrierFieldTypeTraits<PropertyGuardDictionary>::Type propertyGuards;
#endif

        typename WriteBarrierFieldTypeTraits<PropertyNoCaseSetType *>::Type caseInvariantPropertySet;

        typename WriteBarrierFieldTypeTraits<JsUtil::List<Js::PropertyRecord const*>*>::Type boundPropertyStrings; // Recycler allocated list of property strings that we need to strongly reference so that they're not reclaimed

        typename WriteBarrierFieldTypeTraits<SourceProfileManagersByUrlMap*>::Type sourceProfileManagersByUrl;

        // Used to register recyclable data that needs to be kept alive while jitting
        typedef JsUtil::DoublyLinkedList<Js::CodeGenRecyclableData, Recycler> CodeGenRecyclableDataList;
        typename WriteBarrierFieldTypeTraits<CodeGenRecyclableDataList>::Type codeGenRecyclableDatas;

        // Used to root old entry points so that they're not prematurely collected
        typename WriteBarrierFieldTypeTraits<Js::FunctionEntryPointInfo*>::Type oldEntryPointInfo;

        // Used to store a mapping of string to Symbol for cross-realm Symbol registration
        // See ES6 (draft 22) 19.4.2.2
        typename WriteBarrierFieldTypeTraits<SymbolRegistrationMap*>::Type symbolRegistrationMap;

        typename WriteBarrierFieldTypeTraits<uint>::Type constructorCacheInvalidationCount;
    };

    static ThreadContext * globalListLast;

    ThreadContextFlags threadContextFlags;
    uint32_t currentThreadId;
    mutable size_t stackLimitForCurrentThread;
    StackProber * stackProber;
    // Rental threading is always enabled for JSRT (i.e. not bound to a specific thread)
    bool isThreadBound;
    bool hasThrownPendingException;
    bool * hasBailedOutBitPtr;

    bool noJsReentrancy;
private:
    bool reentrancySafeOrHandled;
    bool isInReentrancySafeRegion;

    AllocationPolicyManager * allocationPolicyManager;

    JsUtil::ThreadService threadService;
#if ENABLE_NATIVE_CODEGEN
    PreReservedVirtualAllocWrapper preReservedVirtualAllocator;
#endif

    uint callRootLevel;

    // The thread page allocator is used by the recycler and need the background page queue
    PageAllocator::BackgroundPageQueue backgroundPageQueue;
    IdleDecommitPageAllocator pageAllocator;
    Recycler* recycler;

    // This instance holds list of delay-free array buffer - this will be used in 
    // scanning the stack in order to release any delay-free buffer.
    Js::DelayedFreeArrayBuffer delayFreeCallback;

    // Fake RecyclerWeakReference for built-in properties
    class StaticPropertyRecordReference : public RecyclerWeakReference<const Js::PropertyRecord>
    {
    public:
        StaticPropertyRecordReference(const Js::PropertyRecord* propertyRecord)
        {
            strongRef = (char*)propertyRecord;
            strongRefHeapBlock = &CollectedRecyclerWeakRefHeapBlock::Instance;
        }
    };

    static const Js::PropertyRecord * const builtInPropertyRecords[];

    PropertyNoCaseSetType * caseInvariantPropertySet;

    Js::ScriptContext * rootPendingClose;
    Js::ScriptEntryExitRecord * entryExitRecord;
    Js::InterpreterStackFrame* leafInterpreterFrame;
    const Js::PropertyRecord * propertyNamesDirect[128];
    ArenaAllocator threadAlloc;
    ThreadServiceWrapper* threadServiceWrapper;
    uint functionCount;
    uint sourceInfoCount;
    void * tryHandlerAddrOfReturnAddr;
    enum RedeferralState
    {
        InitialRedeferralState,
        StartupRedeferralState,
        MainRedeferralState
    };
    RedeferralState redeferralState;
    uint gcSinceLastRedeferral;
    uint gcSinceCallCountsCollected;

    static const uint InitialRedeferralDelay = 5;
    static const uint StartupRedeferralCheckInterval = 10;
    static const uint StartupRedeferralInactiveThreshold = 5;
    static const uint MainRedeferralCheckInterval = 20;
    static const uint MainRedeferralInactiveThreshold = 10;

    Js::TypeId nextTypeId;
    uint32_t polymorphicCacheState;

#if ENABLE_NATIVE_CODEGEN
    JsUtil::JobProcessor *jobProcessor;
    Js::Var * bailOutRegisterSaveSpace;
#if DYNAMIC_INTERPRETER_THUNK || defined(ASMJS_PLAT)
    CustomHeap::InProcCodePageAllocators thunkPageAllocators;
#endif
    CustomHeap::InProcCodePageAllocators codePageAllocators;
#endif

    RecyclerRootPtr<RecyclableData> recyclableData;
    uint temporaryArenaAllocatorCount;
    uint temporaryGuestArenaAllocatorCount;

    ScriptSite* topLevelScriptSite;

    Js::ScriptContext *scriptContextList;
    bool scriptContextEverRegistered;
    static size_t processNativeCodeSize;
    size_t nativeCodeSize;
    size_t sourceCodeSize;

    DateTime::HiResTimer hTimer;

    int stackProbeCount;
    // Count stack probes and poll for continuation every n probes
    static const int StackProbePollThreshold = 1000;
    EXCEPTION_POINTERS exceptionInfo;
    uint32_t exceptionCode;

    ArenaAllocator inlineCacheThreadInfoAllocator;
    ArenaAllocator isInstInlineCacheThreadInfoAllocator;
    ArenaAllocator equivalentTypeCacheInfoAllocator;
    DListBase<Js::EntryPointInfo *> equivalentTypeCacheEntryPoints;

    typedef SList<Js::InlineCache*> InlineCacheList;
    typedef JsUtil::BaseDictionary<Js::PropertyId, InlineCacheList*, ArenaAllocator, PrimeSizePolicy> InlineCacheListMapByPropertyId;
    InlineCacheListMapByPropertyId protoInlineCacheByPropId;
    InlineCacheListMapByPropertyId storeFieldInlineCacheByPropId;

    uint registeredInlineCacheCount;
    uint unregisteredInlineCacheCount;
#if DBG
    uint totalUnregisteredCacheCount;
    uint arrayMutationSeed; // This is mostly to aid in debugging.
#endif

    typedef JsUtil::BaseDictionary<Js::Var, Js::IsInstInlineCache*, ArenaAllocator, PrimeSizePolicy> IsInstInlineCacheListMapByFunction;
    IsInstInlineCacheListMapByFunction isInstInlineCacheByFunction;

    Js::IsConcatSpreadableCache isConcatSpreadableCache;

    Js::NoSpecialPropertyThreadRegistry noSpecialPropertyRegistry;
    Js::OnlyWritablePropertyThreadRegistry onlyWritablePropertyRegistry;

    DListBase<CollectCallBack> collectCallBackList;
    std::recursive_mutex csCollectionCallBack;
    bool hasCollectionCallBack;
    bool isOptimizedForManyInstances;
    bool bgJit;

    // We report library code to profiler only if called directly by user code. Not if called by library implementation.
    bool isProfilingUserCode;

    bool isJsrtRuntime_;

    bool hasUnhandledException;
    bool hasCatchHandler;
    DisableImplicitFlags disableImplicitFlags;

    // Used for identifying that any particular time, the caller chain has try/catch blocks belong to the user code.
    // If all try/catch blocks in the current stack marked as non-user code then this member will remain false.
    bool hasCatchHandlerToUserCode;

    // Number of script context attached with probe manager.
    // This counter will be used as addref when the script context is created, this way we maintain the life of diagnostic object.
    // Once no script context available , diagnostic will go away.
    int32_t crefSContextForDiag;

    Entropy entropy;

    JsUtil::Stack<HostScriptContext*>* hostScriptContextStack;

    //
    // Regex globals
    //
    UnifiedRegex::StandardChars<uint8_t>* standardUTF8Chars;
    UnifiedRegex::StandardChars<char16_t>* standardUnicodeChars;

    Js::ImplicitCallFlags implicitCallFlags;

    thread_local static uint activeScriptSiteCount;
    bool isScriptActive;

    // When ETW rundown in background thread which needs to walk scriptContext/functionBody/entryPoint lists,
    // or when JIT thread is getting auxPtrs from function body, we should not be modifying the list of 
    // functionBody/entrypoints, or expanding the auxPtrs
    std::recursive_mutex csFunctionBody;

#ifdef __x86_64__
    friend class Js::Amd64StackFrame;
    Js::Amd64ContextsManager amd64ContextsManager;
    Js::Amd64ContextsManager* GetAmd64ContextsManager();
#endif

    typedef JsUtil::BaseDictionary<Js::DynamicType const *, void *, HeapAllocator, PowerOf2SizePolicy> DynamicObjectEnumeratorCacheMap;
    DynamicObjectEnumeratorCacheMap dynamicObjectEnumeratorCacheMap;

    NativeLibraryEntryRecord nativeLibraryEntry;

    // Indicates the current loop depth as observed by the interpreter. The interpreter causes this value to be updated upon
    // entering and leaving a loop.
    uint8_t loopDepth;

    const ThreadConfiguration configuration;

public:
    static ThreadContext * globalListFirst;

    static uint GetScriptSiteHolderCount();
    static uint IncrementActiveScriptSiteCount();
    static uint DecrementActiveScriptSiteCount();

    static ThreadContext *GetThreadContextList();
    void ValidateThreadContext();

    bool IsInScript() const;
    uint GetCallRootLevel() const;

    PageAllocator *GetPageAllocator();
    AllocationPolicyManager *GetAllocationPolicyManager() const;

    // used for diagnosing abnormally high number of closed, but still formally reachable script contexts
    // at the time of failfast due to allocation limits.
    // high number may indicate that context leaks have occured.
    uint closedScriptContextCount;

    enum VisibilityState : uint8_t
    {
        Undefined = 0,
        Visible = 1,
        NotVisible = 2
    };

    // indicates the visibility state of the hosting application/window/tab if known.
    VisibilityState visibilityState;

#if ENABLE_NATIVE_CODEGEN
    PreReservedVirtualAllocWrapper *GetPreReservedVirtualAllocator();
#if DYNAMIC_INTERPRETER_THUNK || defined(ASMJS_PLAT)
    CustomHeap::InProcCodePageAllocators *GetThunkPageAllocators();
#endif
    CustomHeap::InProcCodePageAllocators *GetCodePageAllocators();
#endif // ENABLE_NATIVE_CODEGEN

    std::recursive_mutex& GetFunctionBodyMutex();
    Js::IsConcatSpreadableCache* GetIsConcatSpreadableCache();

    void SetAbnormalExceptionRecord(EXCEPTION_POINTERS *exceptionInfo);
    void SetAbnormalExceptionCode(uint32_t exceptionInfo);
    uint32_t GetAbnormalExceptionCode() const;

    void *tridentLoadAddress;

    void* GetTridentLoadAddress() const;
    void SetTridentLoadAddress(void *loadAddress);

    Js::NoSpecialPropertyThreadRegistry* GetNoSpecialPropertyRegistry();
    Js::OnlyWritablePropertyThreadRegistry* GetOnlyWritablePropertyRegistry();

    Js::DelayedFreeArrayBuffer *GetScanStackCallback();

    BOOL HasPreviousHostScriptContext();
    HostScriptContext* GetPreviousHostScriptContext() ;
    void PushHostScriptContext(HostScriptContext* topProvider);
    HostScriptContext* PopHostScriptContext();

    void SetInterruptPoller(InterruptPoller *poller);
    InterruptPoller *GetInterruptPoller() const;
    BOOL HasInterruptPoller() const;
    void CheckScriptInterrupt();
    void CheckInterruptPoll();

    bool DoInterruptProbe(Js::FunctionBody *const func) const;
    bool DoInterruptProbe() const;
    bool EvalDisabled() const;
    bool NoJIT() const;
    bool NoDynamicThunks() const;

    Js::Var GetMemoryStat(Js::ScriptContext* scriptContext);
    Js::PropertyId handlerPropertyId = Js::Constants::NoProperty;

    uint GetScriptContextCount() const;
    Js::ScriptContext* GetScriptContextList() const;
    bool WasAnyScriptContextEverRegistered() const;

    void SetTopLevelScriptSite(ScriptSite* topScriptSite);
    ScriptSite * GetTopLevelScriptSite () const;

    bool AsyncHostOperationStart(void *) override;
    void AsyncHostOperationEnd(bool wasInAsync, void *) override;
#if DBG
    void CheckJsReentrancyOnDispose() override;
    bool IsInAsyncHostOperation() const;
#endif

    BOOL ReserveStaticTypeIds(int first, int last);
    Js::TypeId ReserveTypeIds(int count);
    Js::TypeId CreateTypeId();
    Js::TypeId GetNextTypeId() const;

    // Lookup the well known type registered with a Js::TypeId.
    //  wellKnownType:  The well known type which we should register
    //  typeId:   The type id to match
    //  returns:  true if the typeid is the wellKnownType
    template<WellKnownHostType wellKnownType>
    bool IsWellKnownHostType(Js::TypeId typeId)
    {
        static_assert(wellKnownType <= WellKnownHostType_Last);
        return wellKnownHostTypeIds[wellKnownType] == typeId;
    }

    uint32_t GetNextPolymorphicCacheState();

    ~ThreadContext();
    void MarkContextsForClose();
    void CloseForJSRT();

    void ShutdownThreads();
    DateTime::HiResTimer * GetHiResTimer();
    ArenaAllocator * GetThreadAlloc();
    static std::recursive_mutex& GetMutex();

    ThreadContext(AllocationPolicyManager * allocationPolicyManager = nullptr, JsUtil::ThreadService::ThreadServiceCallback threadServiceCallback = nullptr, bool enableExperimentalFeatures = false);
    ThreadContext(std::unique_ptr<AllocationPolicyManager> allocationPolicyManager, bool enableExperimentalFeatures);
    static void Add(ThreadContext *threadContext);

    ThreadConfiguration const * GetConfig() const;

public:
    static ThreadContext* GetContextForCurrentThread();
    Recycler* GetRecycler();
    Recycler * EnsureRecycler();

    ThreadContext::CollectCallBack * AddRecyclerCollectCallBack(RecyclerCollectCallBackFunction callback, void * context);
    void RemoveRecyclerCollectCallBack(ThreadContext::CollectCallBack * collectCallBack);

    void AddToPendingScriptContextCloseList(Js::ScriptContext * scriptContext);
    void RemoveFromPendingClose(Js::ScriptContext * scriptContext);
    void ClosePendingScriptContexts();

    Js::PropertyRecord const * GetPropertyName(Js::PropertyId propertyId);
    Js::PropertyRecord const * GetPropertyNameLocked(Js::PropertyId propertyId);

private:
    template <bool locked> Js::PropertyRecord const * GetPropertyNameImpl(Js::PropertyId propertyId);
public:
    void FindPropertyRecord(Js::JavascriptString *pstName, Js::PropertyRecord const ** propertyRecord);
    void FindPropertyRecord(const char16_t * propertyName, int propertyNameLength, Js::PropertyRecord const ** propertyRecord);
    const Js::PropertyRecord * FindPropertyRecord(const char16_t * propertyName, int propertyNameLength);

    JsUtil::List<const RecyclerWeakReference<Js::PropertyRecord const>*>* FindPropertyIdNoCase(Js::ScriptContext * scriptContext, const char16_t* propertyName, int propertyNameLength);
    JsUtil::List<const RecyclerWeakReference<Js::PropertyRecord const>*>* FindPropertyIdNoCase(Js::ScriptContext * scriptContext, JsUtil::CharacterBuffer<char16_t> const& propertyName);
    bool FindExistingPropertyRecord(_In_ JsUtil::CharacterBuffer<char16_t> const& propertyName, Js::CaseInvariantPropertyListWithHashCode** propertyRecord);
    void CleanNoCasePropertyMap();
    void ForceCleanPropertyMap();

    const Js::PropertyRecord *GetOrAddPropertyRecord(JsUtil::CharacterBuffer<char16_t> propertyName);
    const Js::PropertyRecord *GetOrAddPropertyRecordBind(JsUtil::CharacterBuffer<char16_t> propertyName);
    void AddBuiltInPropertyRecord(const Js::PropertyRecord *propertyRecord);

    void GetOrAddPropertyId(_In_ const char16_t* propertyName, _In_ int propertyNameLength, _Out_ Js::PropertyRecord const** propertyRecord);
    void GetOrAddPropertyId(_In_ JsUtil::CharacterBuffer<char16_t> const& propertyName, _Out_ Js::PropertyRecord const** propertyRecord);
    Js::PropertyRecord const * UncheckedAddPropertyId(JsUtil::CharacterBuffer<char16_t> const& propertyName, bool bind, bool isSymbol = false);
    Js::PropertyRecord const * UncheckedAddPropertyId(const char16_t* propertyName, int propertyNameLength, bool bind = false, bool isSymbol = false);

private:
    const Js::PropertyRecord * GetOrAddPropertyRecordImpl(JsUtil::CharacterBuffer<char16_t> propertyName, bool bind);
    void AddPropertyRecordInternal(const Js::PropertyRecord * propertyRecord);
    void BindPropertyRecord(const Js::PropertyRecord * propertyRecord);
    bool IsDirectPropertyName(const char16_t * propertyName, int propertyNameLength);

    RecyclerWeakReference<const Js::PropertyRecord> * CreatePropertyRecordWeakRef(const Js::PropertyRecord * propertyRecord);
    void AddCaseInvariantPropertyRecord(const Js::PropertyRecord * propertyRecord);

    uint scriptContextCount;

public:
    void UncheckedAddBuiltInPropertyId();

    BOOL IsNumericPropertyId(Js::PropertyId propertyId, uint32_t* value);
    bool IsActivePropertyId(Js::PropertyId pid);
    void InvalidatePropertyRecord(const Js::PropertyRecord * propertyRecord);
    Js::PropertyId GetNextPropertyId();
    Js::PropertyId GetMaxPropertyId();

    void SetThreadServiceWrapper(ThreadServiceWrapper*);
    ThreadServiceWrapper* GetThreadServiceWrapper();

    uint NewFunctionNumber();
    uint PeekNewFunctionNumber();
    uint NewSourceInfoNumber();

    void AddCodeSize(size_t newCode);
    void AddSourceSize(size_t  newCode);
    void SubCodeSize(size_t  deadCode);
    void SubSourceSize(size_t deadCode);
    size_t GetCodeSize() const;
    static size_t GetProcessCodeSize();
    size_t GetSourceSize() const;

    bool DoTryRedeferral() const;
    void TryRedeferral();
    bool DoRedeferFunctionBodies() const;
    void UpdateRedeferralState();
    uint GetRedeferralCollectionInterval() const;
    uint GetRedeferralInactiveThreshold() const;
    void GetActiveFunctions(ActiveFunctionSet * pActive);
#if DBG
    uint redeferredFunctions;
    uint recoveredBytes;
#endif

    Js::ScriptEntryExitRecord * GetScriptEntryExit() const;
    void RegisterCodeGenRecyclableData(Js::CodeGenRecyclableData *const codeGenRecyclableData);
    void UnregisterCodeGenRecyclableData(Js::CodeGenRecyclableData *const codeGenRecyclableData);
#if ENABLE_NATIVE_CODEGEN
    bool IsNativeAddressHelper(void * pCodeAddr, Js::ScriptContext* currentScriptContext);
    BOOL IsNativeAddress(void * pCodeAddr, Js::ScriptContext* currentScriptContext = nullptr);
    JsUtil::JobProcessor *GetJobProcessor();
    Js::Var * GetBailOutRegisterSaveSpace() const;
    intptr_t GetBailOutRegisterSaveSpaceAddr() const override;
#endif
    void ResetFunctionCount();
    void PushEntryExitRecord(Js::ScriptEntryExitRecord *);
    void PopEntryExitRecord(Js::ScriptEntryExitRecord *);
    uint EnterScriptStart(Js::ScriptEntryExitRecord *, bool doCleanup);
    void EnterScriptEnd(Js::ScriptEntryExitRecord *, bool doCleanup);

    void * GetTryHandlerAddrOfReturnAddr() const;
    void SetTryHandlerAddrOfReturnAddr(void * addrOfReturnAddr);

    template <bool leaveForHost>
    void LeaveScriptStart(void *);
    template <bool leaveForHost>
    void LeaveScriptEnd(void *);
    void DisposeOnLeaveScript();

    void PushInterpreterFrame(Js::InterpreterStackFrame *interpreterFrame);
    Js::InterpreterStackFrame *PopInterpreterFrame();
    Js::InterpreterStackFrame *GetLeafInterpreterFrame() const;

    Js::TempArenaAllocatorObject * GetTemporaryAllocator(const char16_t* name);
    void ReleaseTemporaryAllocator(Js::TempArenaAllocatorObject * tempAllocator);

    Js::TempGuestArenaAllocatorObject * GetTemporaryGuestAllocator(const char16_t* name);
    void ReleaseTemporaryGuestAllocator(Js::TempGuestArenaAllocatorObject * tempAllocator);

    void RegisterScriptContext(Js::ScriptContext *scriptContext);
    void UnregisterScriptContext(Js::ScriptContext *scriptContext);

    // NoScriptScope
    void SetNoScriptScope(bool noScriptScope);
    bool IsNoScriptScope() const;

    void SetPendingFinallyException(Js::JavascriptExceptionObject * exceptionObj);
    Js::JavascriptExceptionObject * GetPendingFinallyException() const;

    Js::EntryPointInfo ** RegisterEquivalentTypeCacheEntryPoint(Js::EntryPointInfo * entryPoint);
    void UnregisterEquivalentTypeCacheEntryPoint(Js::EntryPointInfo ** entryPoint);
    void RegisterProtoInlineCache(Js::InlineCache * inlineCache, Js::PropertyId propertyId);
    void RegisterStoreFieldInlineCache(Js::InlineCache * inlineCache, Js::PropertyId propertyId);
    void NotifyInlineCacheBatchUnregistered(uint count);

#if DBG
    bool IsProtoInlineCacheRegistered(const Js::InlineCache * inlineCache, Js::PropertyId propertyId);
    bool IsStoreFieldInlineCacheRegistered(const Js::InlineCache * inlineCache, Js::PropertyId propertyId);
#endif

#if ENABLE_NATIVE_CODEGEN
    Js::PropertyGuard* RegisterSharedPropertyGuard(Js::PropertyId propertyId);
    void RegisterLazyBailout(Js::PropertyId propertyId, Js::EntryPointInfo* entryPoint);
    void RegisterUniquePropertyGuard(Js::PropertyId propertyId, Js::PropertyGuard* guard);
    void RegisterUniquePropertyGuard(Js::PropertyId propertyId, RecyclerWeakReference<Js::PropertyGuard>* guardWeakRef);
    void RegisterConstructorCache(Js::PropertyId propertyId, Js::ConstructorCache* cache);
#endif

    size_t GetScriptStackLimit() const override;

    intptr_t GetThreadStackLimitAddr() const override;

#if ENABLE_NATIVE_CODEGEN && defined(ENABLE_WASM_SIMD)
    virtual intptr_t GetSimdTempAreaAddr(uint8_t tempIndex) const override;
#endif

    intptr_t GetDisableImplicitFlagsAddr() const override;
    intptr_t GetImplicitCallFlagsAddr() const override;

    ptrdiff_t GetChakraBaseAddressDifference() const override;
    ptrdiff_t GetCRTBaseAddressDifference() const override;

private:
    void RegisterInlineCache(InlineCacheListMapByPropertyId& inlineCacheMap, Js::InlineCache* inlineCache, Js::PropertyId propertyId);
    static bool IsInlineCacheRegistered(InlineCacheListMapByPropertyId& inlineCacheMap, const Js::InlineCache* inlineCache, Js::PropertyId propertyId);
    void InvalidateAndDeleteInlineCacheList(InlineCacheList *inlineCacheList);
    void CompactInlineCacheList(InlineCacheList *inlineCacheList);
    void CompactInlineCacheInvalidationLists();
    void CompactProtoInlineCaches();
    void CompactStoreFieldInlineCaches();

#if DBG
    static bool IsInlineCacheInList(const Js::InlineCache* inlineCache, const InlineCacheList* inlineCacheChain);
#endif

#if ENABLE_NATIVE_CODEGEN
    void InvalidateFixedFieldGuard(Js::PropertyGuard* guard);
    PropertyGuardEntry* EnsurePropertyGuardEntry(const Js::PropertyRecord* propertyRecord, bool& foundExistingEntry);
    void InvalidatePropertyGuardEntry(const Js::PropertyRecord* propertyRecord, PropertyGuardEntry* entry, bool isAllPropertyGuardsInvalidation);
#endif

public:
    class AutoDisableExpiration
    {
    public:
        AutoDisableExpiration(ThreadContext* threadContext);
        ~AutoDisableExpiration();

    private:
        ThreadContext* _threadContext;
        bool _oldExpirationDisabled;
    };

    void InvalidateProtoInlineCaches(Js::PropertyId propertyId);
    void InvalidateStoreFieldInlineCaches(Js::PropertyId propertyId);
    void InvalidateAllProtoInlineCaches();
#if DBG
    bool IsObjectRegisteredInProtoInlineCaches(Js::DynamicObject * object);
    bool IsObjectRegisteredInStoreFieldInlineCaches(Js::DynamicObject * object);
#endif
    bool AreAllProtoInlineCachesInvalidated();
    void InvalidateAllStoreFieldInlineCaches();
    bool AreAllStoreFieldInlineCachesInvalidated();
#if ENABLE_NATIVE_CODEGEN
    void InvalidatePropertyGuards(Js::PropertyId propertyId);
    void InvalidateAllPropertyGuards();
#endif
    void RegisterIsInstInlineCache(Js::IsInstInlineCache * inlineCache, Js::Var function);
    void UnregisterIsInstInlineCache(Js::IsInstInlineCache * inlineCache, Js::Var function);
#if DBG
    bool IsIsInstInlineCacheRegistered(Js::IsInstInlineCache * inlineCache, Js::Var function);
#endif

private:
    void InvalidateIsInstInlineCacheList(Js::IsInstInlineCache* inlineCacheList);
#if DBG
    bool IsIsInstInlineCacheInList(const Js::IsInstInlineCache* inlineCache, const Js::IsInstInlineCache* inlineCacheList);
#endif

public:
    void InvalidateIsInstInlineCachesForFunction(Js::Var function);
    void InvalidateAllIsInstInlineCaches();
    bool AreAllIsInstInlineCachesInvalidated() const;
    void ClearInlineCachesWithDeadWeakRefs();
#if ENABLE_NATIVE_CODEGEN
    void ClearInvalidatedUniqueGuards();
#endif
    void ClearInlineCaches();
    void ClearIsInstInlineCaches();
    void ClearEnumeratorCaches();
    void ClearEquivalentTypeCaches();
    void ClearScriptContextCaches();

    void RegisterTypeWithProtoPropertyCache(const Js::PropertyId propertyId, Js::Type *const type);
    void InvalidateProtoTypePropertyCaches(const Js::PropertyId propertyId);
    void InternalInvalidateProtoTypePropertyCaches(const Js::PropertyId propertyId);
    void InvalidateAllProtoTypePropertyCaches();

    bool HasUnhandledException() const;
    void SetHasUnhandledException();
    void ResetHasUnhandledException();
    void SetUnhandledExceptionObject(Js::JavascriptExceptionObject* exceptionObject);
    Js::JavascriptExceptionObject* GetUnhandledExceptionObject() const;

    // To temporarily keep throwing exception object alive (thrown but not yet caught)
    void SaveTempUncaughtException(Js::JavascriptExceptionObject* exceptionObject);
    void ClearTempUncaughtException(Js::JavascriptExceptionObject* exceptionObject);

public:
    bool HasCatchHandler() const;
    void SetHasCatchHandler(bool hasCatchHandler);

    bool IsUserCode() const;
    void SetIsUserCode(bool set);

    void QueueFreeOldEntryPointInfoIfInScript(Js::FunctionEntryPointInfo* oldEntryPointInfo);
    bool IsOldEntryPointInfo(Js::ProxyEntryPointInfo* entryPointInfo);

    static bool IsOnStack(void const *ptr);
    bool IsStackAvailable(size_t size, bool* isInterrupt = nullptr);
    bool IsStackAvailableNoThrow(size_t size = Js::Constants::MinStackDefault);
    static bool IsCurrentStackAvailable(size_t size);
    void ProbeStackNoDispose(size_t size, Js::ScriptContext *scriptContext, void * returnAddress = nullptr);
    void ProbeStack(size_t size, Js::ScriptContext *scriptContext, void * returnAddress = nullptr);
    void ProbeStack(size_t size, Js::RecyclableObject * obj, Js::ScriptContext *scriptContext);
    void ProbeStack(size_t size);
    static void ProbeCurrentStackNoDispose(size_t size, Js::ScriptContext *scriptContext);
    static void ProbeCurrentStack(size_t size, Js::ScriptContext *scriptContext);
    static void ProbeCurrentStack2(size_t size, Js::ScriptContext *scriptContext, uint32_t u1, uint32_t u2);

#if ENABLE_PROFILE_INFO
    void EnsureSourceProfileManagersByUrlMap();
    Js::SourceDynamicProfileManager* GetSourceDynamicProfileManager(_In_z_ const char16_t* url, _In_ uint hash, _Inout_ bool* addref);
    uint ReleaseSourceDynamicProfileManagers(const char16_t* url);
#endif

    void EnsureSymbolRegistrationMap();
    const Js::PropertyRecord* GetSymbolFromRegistrationMap(const char16_t* stringKey, charcount_t stringLength);
    const Js::PropertyRecord* AddSymbolToRegistrationMap(const char16_t* stringKey, charcount_t stringLength);

    void ClearPendingSOError();
    void ClearPendingOOMError();
    Js::JavascriptExceptionObject *GetPendingSOErrorObject();
    Js::JavascriptExceptionObject *GetPendingOOMErrorObject();
    Js::JavascriptExceptionObject *GetPendingTerminatedErrorObject();
    Js::JavascriptExceptionObject* GetRecordedException();

    bool GetPropagateException() const;
    void SetHasThrownPendingException();
    bool * GetHasBailedOutBitPtr();
    void SetHasBailedOutBitPtr(bool *setValue);
    void SetRecordedException(Js::JavascriptExceptionObject* exceptionObject, bool propagateToDebugger = false);

    Js::ImplicitCallFlags * GetAddressOfImplicitCallFlags();
    DisableImplicitFlags * GetAddressOfDisableImplicitFlags();
    Js::ImplicitCallFlags GetImplicitCallFlags() const;
    void SetImplicitCallFlags(Js::ImplicitCallFlags flags);
    void ClearImplicitCallFlags();
    void ClearImplicitCallFlags(Js::ImplicitCallFlags flags);
    void AddImplicitCallFlags(Js::ImplicitCallFlags flags);
    void CheckAndResetImplicitCallAccessorFlag();

    Js::Var ExecuteImplicitCall(Js::RecyclableObject *function, Js::ImplicitCallFlags flags, std::function<Js::Var()> implicitCall);
    bool HasNoSideEffect(Js::RecyclableObject * function) const;
    bool HasNoSideEffect(Js::RecyclableObject * function, Js::FunctionInfo::Attributes attr) const;
    bool RecordImplicitException();
    DisableImplicitFlags GetDisableImplicitFlags() const;
    void SetDisableImplicitFlags(DisableImplicitFlags flags);
    bool IsDisableImplicitCall() const;
    bool IsDisableImplicitException() const;
    void DisableImplicitCall();
    void ClearDisableImplicitFlags();

    uint GetRandomNumber() override;
    bool DoSpecialMarkOnScanStack() override;
    void OnScanStackCallback(void ** stackTop, size_t byteCount, void ** registers, size_t registersByteCount) override;

    void PostSweepRedeferralCallBack() override;

    // DefaultCollectWrapper
    void PreCollectionCallBack(CollectionFlags flags) override;
    void PreSweepCallback() override;
    void PreRescanMarkCallback() override;
    void WaitCollectionCallBack() override;
    void PostCollectionCallBack() override;
    BOOL ExecuteRecyclerCollectionFunction(Recycler * recycler, CollectionFunction function, CollectionFlags flags) override;
    void DisposeObjects(Recycler * recycler) override;
    void PreDisposeObjectsCallBack() override;

    void DoExpirableCollectModeStackWalk();

    typedef DList<ExpirableObject*, ArenaAllocator> ExpirableObjectList;
    ExpirableObjectList* expirableObjectList;
    ExpirableObjectList* expirableObjectDisposeList;
    int numExpirableObjects;
    int expirableCollectModeGcCount;
    bool disableExpiration;
    bool callDispose;

    bool InExpirableCollectMode();
    void TryEnterExpirableCollectMode();
    void TryExitExpirableCollectMode();
    void RegisterExpirableObject(ExpirableObject* object);
    void UnregisterExpirableObject(ExpirableObject* object);

    void * GetDynamicObjectEnumeratorCache(Js::DynamicType const * dynamicType);
    void AddDynamicObjectEnumeratorCache(Js::DynamicType const * dynamicType, void * cache);
public:
    bool IsScriptActive() const;
    void SetIsScriptActive(bool isActive);
    bool IsExecutionDisabled() const;
    bool TestThreadContextFlag(ThreadContextFlags threadContextFlag) const;
    void SetThreadContextFlag(ThreadContextFlags threadContextFlag);

    void SetForceOneIdleCollection();

    bool IsInThreadServiceCallback() const;

    const NativeLibraryEntryRecord::Entry* PeekNativeLibraryEntry() const;
    void PushNativeLibraryEntry(_In_ NativeLibraryEntryRecord::Entry* entry);
    void PopNativeLibraryEntry();

    bool IsProfilingUserCode() const;
    void SetIsProfilingUserCode(bool value);

#if DBG_DUMP
    uint scriptSiteCount;
#endif

    uint bailOutByteCodeLocationCount;
    //
    // Regex helpers
    //
    UnifiedRegex::StandardChars<uint8_t>* GetStandardChars(uint8_t* dummy);
    UnifiedRegex::StandardChars<char16_t>* GetStandardChars(char16_t* dummy);

    bool IsOptimizedForManyInstances() const;
    void OptimizeForManyInstances(bool optimizeForManyInstances);

#if ENABLE_NATIVE_CODEGEN
    bool IsBgJitEnabled() const;
    void EnableBgJit(bool enableBgJit);
#endif

    void SetJSRTRuntime(bool isJsrtRuntime);

private:
    BOOL ExecuteRecyclerCollectionFunctionCommon(Recycler * recycler, CollectionFunction function, CollectionFlags flags);

    void DoInvalidateProtoTypePropertyCaches(const Js::PropertyId propertyId, TypeHashSet *const typeHashSet);
    void InitializePropertyMaps();
    void CreateNoCasePropertyMap();

    InterruptPoller *interruptPoller;

    void CollectionCallBack(RecyclerCollectCallBackFlags flags);

    // Cache used by HostDispatch::GetBuiltInOperationFromEntryPoint
private:
    JsUtil::BaseDictionary<Js::JavascriptMethod, uint, ArenaAllocator, PowerOf2SizePolicy> entryPointToBuiltInOperationIdCache;

public:
    void SetNoJsReentrancy(bool val);
    bool GetNoJsReentrancy() const;
    void AssertJsReentrancy();

    void CheckAndResetReentrancySafeOrHandled();
    void SetReentrancySafeOrHandled(bool val);
    bool GetReentrancySafeOrHandled() const;
    void SetIsInReentrancySafeRegion(bool val);
    bool GetIsInReentrancySafeRegion() const;

    bool IsEntryPointToBuiltInOperationIdCacheInitialized() const;
    bool GetBuiltInOperationIdFromEntryPoint(Js::JavascriptMethod entryPoint, uint * id) const;
    void SetBuiltInOperationIdForEntryPoint(Js::JavascriptMethod entryPoint, uint id);
    void ResetEntryPointToBuiltInOperationIdCache();

    uint8_t LoopDepth() const;
    void SetLoopDepth(uint8_t loopDepth);
    void IncrementLoopDepth();
    void DecrementLoopDepth();
};
