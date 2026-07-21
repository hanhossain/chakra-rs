//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Entropy.h"
#include "DataStructures/Stack.h"
#include "DataStructures/Dictionary.h"
#include "Library/DelayFreeArrayBufferHelper.h"

namespace Js
{
    class ScriptContext;
    struct InlineCache;
    class CodeGenRecyclableData;
#ifdef ENABLE_SCRIPT_DEBUGGING
    class DebugManager;
    struct ReturnedValue;
    typedef JsUtil::List<ReturnedValue*> ReturnedValueList;
#endif
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
    void GetStatementCount(uint32_t *pluHi, uint32_t *pluLo);
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

#define BEGIN_SAFE_REENTRANT_REGION(threadContext) \
    { \
        AutoReentrancySafeRegion autoReentrancySafeRegion(threadContext);

#define END_SAFE_REENTRANT_REGION }
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

class AutoReentrancyHandler;

class ThreadContext :
    public DefaultRecyclerCollectionWrapper,
    public JsUtil::DoublyLinkedListElement<ThreadContext>,
    public ThreadContextInfo
{
public:
    static void GlobalInitialize();
    static const uint32_t NoThread = 0xFFFFFFFF;

    struct CollectCallBack
    {
        RecyclerCollectCallBackFunction callback;
        void * context;
    };

    struct WorkerThread
    {
        // Abstract notion to hold onto threadHandle of worker thread
        HANDLE threadHandle;
        WorkerThread(HANDLE handle = nullptr) :threadHandle(handle){};
    };

    struct AutoRestoreImplicitFlags
    {
        ThreadContext * threadContext;
        Js::ImplicitCallFlags savedImplicitCallFlags;
        DisableImplicitFlags savedDisableImplicitFlags;
        AutoRestoreImplicitFlags(ThreadContext *threadContext, Js::ImplicitCallFlags implicitCallFlags, DisableImplicitFlags disableImplicitFlags) :
            threadContext(threadContext),
            savedImplicitCallFlags(implicitCallFlags),
            savedDisableImplicitFlags(disableImplicitFlags)
        {
        }

        ~AutoRestoreImplicitFlags()
        {
            threadContext->SetImplicitCallFlags((Js::ImplicitCallFlags)(savedImplicitCallFlags));
            threadContext->SetDisableImplicitFlags((DisableImplicitFlags)savedDisableImplicitFlags);
        }
    };

    void SetCurrentThreadId(uint32_t threadId) { this->currentThreadId = threadId; }
    uint32_t GetCurrentThreadId() const { return this->currentThreadId; }
    void SetIsThreadBound()
    {
        if (this->recycler)
        {
            this->recycler->SetIsThreadBound();
        }
        this->isThreadBound = true;
    }
    bool IsJSRT() const { return !this->isThreadBound; }
    virtual bool IsThreadBound() const override { return this->isThreadBound; }
    void SetStackProber(StackProber * stackProber);
    static uint32_t GetStackLimitForCurrentThreadOffset() { return offsetof(ThreadContext, stackLimitForCurrentThread); }

    template <class Fn>
    Js::ImplicitCallFlags TryWithDisabledImplicitCall(Fn fn)
    {
        DisableImplicitFlags prevDisableImplicitFlags = this->GetDisableImplicitFlags();
        Js::ImplicitCallFlags savedImplicitCallFlags = this->GetImplicitCallFlags();

        this->DisableImplicitCall();
        this->SetImplicitCallFlags(Js::ImplicitCallFlags::ImplicitCall_None);
        fn();

        Js::ImplicitCallFlags curImplicitCallFlags = this->GetImplicitCallFlags();

        this->SetDisableImplicitFlags(prevDisableImplicitFlags);
        this->SetImplicitCallFlags(savedImplicitCallFlags);

        return curImplicitCallFlags;
    }

    void * GetAddressOfStackLimitForCurrentThread() const
    {
        return &this->stackLimitForCurrentThread;
    }
    void InitAvailableCommit();

    // This is always on for JSRT APIs.
    bool IsRentalThreadingEnabledInJSRT() const { return true; }

    IActiveScriptProfilerHeapEnum* GetHeapEnum();
    void SetHeapEnum(IActiveScriptProfilerHeapEnum* newHeapEnum);
    void ClearHeapEnum();

    Js::PropertyRecord const * GetPropertyRecord(Js::PropertyId propertyId);

    virtual bool IsNumericProperty(Js::PropertyId propertyId) override;

#ifdef ENABLE_WASM_SIMD
#if _M_AMD64
    // auxiliary SIMD values in memory to help JIT'ed code. E.g. used for Int8x16 shuffle.
    _x86_SIMDValue X86_TEMP_SIMD[SIMD_TEMP_SIZE];
    _x86_SIMDValue * GetSimdTempArea() { return X86_TEMP_SIMD; }
#endif
#endif

public:
    Js::PropertyRecord const * GetEmptyStringPropertyRecord()
    {
        if (!emptyStringPropertyRecord)
        {
            emptyStringPropertyRecord = propertyMap->LookupWithKey(Js::HashedCharacterBuffer<char16_t>(u"", 0));
            if (emptyStringPropertyRecord == nullptr)
            {
                emptyStringPropertyRecord = this->UncheckedAddPropertyId(u"", 0, true);
            }
        }
        return emptyStringPropertyRecord;
    }

    Js::PropertyId GetEmptyStringPropertyId()
    {
        return GetEmptyStringPropertyRecord()->GetPropertyId();
    }

private:
    const Js::PropertyRecord * emptyStringPropertyRecord;
    bool noScriptScope;

#ifdef ENABLE_SCRIPT_DEBUGGING
    Js::DebugManager * debugManager;
#endif

    static uint const MaxTemporaryArenaAllocators = 5;

    static std::recursive_mutex s_csThreadContext;

    StackProber * GetStackProber() const { return this->stackProber; }
    size_t GetStackLimitForCurrentThread() const;
    void SetStackLimitForCurrentThread(size_t limit);

    // The current heap enumeration object being used during enumeration.
    IActiveScriptProfilerHeapEnum* heapEnum;

    struct PropertyGuardEntry
    {
    public:
        typedef JsUtil::BaseHashSet<RecyclerWeakReference<Js::PropertyGuard>*, Recycler, PowerOf2SizePolicy> PropertyGuardHashSet;
        // we do not have WeaklyReferencedKeyHashSet - hence use uint8_t as a dummy value.
        typedef JsUtil::WeaklyReferencedKeyDictionary<Js::EntryPointInfo, uint8_t> EntryPointDictionary;
        // The sharedGuard is strongly referenced and will be kept alive by ThreadContext::propertyGuards until it's invalidated or
        // the property record itself is collected.  If the code using the guard needs access to it after it's been invalidated, it
        // (the code) is responsible for keeping it alive.
        // Each unique guard, is weakly referenced, such that it can be reclaimed if not referenced elsewhere even without being
        // invalidated.  The entry of a unique guard is removed from the table once the corresponding cache is invalidated.
        typename WriteBarrierFieldTypeTraits<Js::PropertyGuard*>::Type sharedGuard;
        typename WriteBarrierFieldTypeTraits<PropertyGuardHashSet>::Type uniqueGuards;
        typename WriteBarrierFieldTypeTraits<EntryPointDictionary*>::Type entryPoints;

        PropertyGuardEntry(Recycler* recycler) : sharedGuard(nullptr), uniqueGuards(recycler), entryPoints(nullptr) {}
    };

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
    PTHREADCONTEXT_HANDLE m_remoteThreadContextInfo;
    intptr_t m_prereservedRegionAddr;
    intptr_t m_jitThunkStartAddr;

#if ENABLE_NATIVE_CODEGEN
    BVSparse<HeapAllocator> * m_jitNumericProperties;
    bool m_jitNeedsPropertyUpdate;
public:
    intptr_t GetPreReservedRegionAddr()
    {
        return m_prereservedRegionAddr;
    }
    intptr_t GetJITThunkStartAddr()
    {
        return m_jitThunkStartAddr;
    }
    BVSparse<HeapAllocator> * GetJITNumericProperties() const
    {
        return m_jitNumericProperties;
    }
    bool JITNeedsPropUpdate() const
    {
        return m_jitNeedsPropertyUpdate;
    }
    void ResetJITNeedsPropUpdate()
    {
        m_jitNeedsPropertyUpdate = false;
    }

    static void SetJITConnectionInfo(HANDLE processHandle, void* serverSecurityDescriptor, UUID connectionId);
    bool EnsureJITThreadContext(bool allowPrereserveAlloc);

    PTHREADCONTEXT_HANDLE GetRemoteThreadContextAddr()
    {
        Assert(m_remoteThreadContextInfo);
        return m_remoteThreadContextInfo;
    }
#endif

private:
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

#ifdef ENABLE_SCRIPT_DEBUGGING
        // Just holding the reference to the returnedValueList of the stepController. This way that list will not get recycled prematurely.
        typename WriteBarrierFieldTypeTraits<Js::ReturnedValueList *>::Type returnedValueList;
#endif

        typename WriteBarrierFieldTypeTraits<uint>::Type constructorCacheInvalidationCount;

        // use for autoProxy called from Debug.setAutoProxyName. we need to keep the buffer from GetSz() alive.
        typename WriteBarrierFieldTypeTraits<const char16_t*>::Type autoProxyName;
    };

    static ThreadContext * globalListLast;

    ThreadContextFlags threadContextFlags;
    uint32_t currentThreadId;
    mutable size_t stackLimitForCurrentThread;
    StackProber * stackProber;
    bool isThreadBound;
    bool hasThrownPendingException;
    bool * hasBailedOutBitPtr;

#if ENABLE_JS_REENTRANCY_CHECK
    bool noJsReentrancy;
#endif
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

#if DBG_DUMP || defined(PROFILE_EXEC)
    ScriptSite* topLevelScriptSite;
#endif

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

    void* jsrtRuntime;

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

#ifdef _M_X64
    friend class Js::Amd64StackFrame;
    Js::Amd64ContextsManager amd64ContextsManager;
    Js::Amd64ContextsManager* GetAmd64ContextsManager() { return &amd64ContextsManager; }
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

    static uint GetScriptSiteHolderCount() { return activeScriptSiteCount; }
    static uint IncrementActiveScriptSiteCount() { return ++activeScriptSiteCount; }
    static uint DecrementActiveScriptSiteCount() { return --activeScriptSiteCount; }

    static ThreadContext * GetThreadContextList() { return globalListFirst; }
    void ValidateThreadContext();

    bool IsInScript() const { return callRootLevel != 0; }
    uint GetCallRootLevel() const { return callRootLevel; }

    PageAllocator * GetPageAllocator() { return &pageAllocator; }

    AllocationPolicyManager * GetAllocationPolicyManager() { return allocationPolicyManager; }

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
    PreReservedVirtualAllocWrapper * GetPreReservedVirtualAllocator() { return &preReservedVirtualAllocator; }
#if DYNAMIC_INTERPRETER_THUNK || defined(ASMJS_PLAT)
    CustomHeap::InProcCodePageAllocators * GetThunkPageAllocators() { return &thunkPageAllocators; }
#endif
    CustomHeap::InProcCodePageAllocators * GetCodePageAllocators() { return &codePageAllocators; }

#endif // ENABLE_NATIVE_CODEGEN

    std::recursive_mutex& GetFunctionBodyMutex() { return csFunctionBody; }

    Js::IsConcatSpreadableCache* GetIsConcatSpreadableCache() { return &isConcatSpreadableCache; }

    void SetAbnormalExceptionRecord(EXCEPTION_POINTERS *exceptionInfo) { this->exceptionInfo = *exceptionInfo; }
    void SetAbnormalExceptionCode(uint32_t exceptionInfo) { this->exceptionCode = exceptionInfo; }
    uint32_t GetAbnormalExceptionCode() const { return this->exceptionCode; }

    void *tridentLoadAddress;

    void* GetTridentLoadAddress() const { return tridentLoadAddress;  }
    void SetTridentLoadAddress(void *loadAddress) { tridentLoadAddress = loadAddress; }

    Js::NoSpecialPropertyThreadRegistry* GetNoSpecialPropertyRegistry() { return &this->noSpecialPropertyRegistry; }
    Js::OnlyWritablePropertyThreadRegistry* GetOnlyWritablePropertyRegistry() { return &this->onlyWritablePropertyRegistry; }

    Js::DelayedFreeArrayBuffer * GetScanStackCallback()
    {
        return &this->delayFreeCallback;
    }

#ifdef ENABLE_DIRECTCALL_TELEMETRY
    DirectCallTelemetry directCallTelemetry;
#endif

    BOOL HasPreviousHostScriptContext();
    HostScriptContext* GetPreviousHostScriptContext() ;
    void PushHostScriptContext(HostScriptContext* topProvider);
    HostScriptContext* PopHostScriptContext();

    void SetInterruptPoller(InterruptPoller *poller) { interruptPoller = poller; }
    InterruptPoller *GetInterruptPoller() const { return interruptPoller; }
    BOOL HasInterruptPoller() const { return interruptPoller != nullptr; }
    void CheckScriptInterrupt();
    void CheckInterruptPoll();

    bool DoInterruptProbe(Js::FunctionBody *const func) const
    {
        return
            (this->TestThreadContextFlag(ThreadContextFlagCanDisableExecution) &&
             !PHASE_OFF(Js::InterruptProbePhase, func)) ||
            PHASE_ON(Js::InterruptProbePhase, func);
    }

    bool DoInterruptProbe() const
    {
        return
            (this->TestThreadContextFlag(ThreadContextFlagCanDisableExecution) &&
             !PHASE_OFF1(Js::InterruptProbePhase)) ||
            PHASE_ON1(Js::InterruptProbePhase);
    }

    bool EvalDisabled() const
    {
        return this->TestThreadContextFlag(ThreadContextFlagEvalDisabled);
    }

    bool NoJIT() const
    {
        return this->TestThreadContextFlag(ThreadContextFlagNoJIT);
    }

    bool NoDynamicThunks() const
    {
        return this->TestThreadContextFlag(ThreadContextFlagNoDynamicThunks);
    }

    Js::Var GetMemoryStat(Js::ScriptContext* scriptContext);
    void SetAutoProxyName(const char16_t* objectName);
    const char16_t* GetAutoProxyName() const { return recyclableData->autoProxyName; }
    Js::PropertyId handlerPropertyId = Js::Constants::NoProperty;

#ifdef ENABLE_SCRIPT_DEBUGGING
    void SetReturnedValueList(Js::ReturnedValueList *returnedValueList)
    {
        Assert(this->recyclableData != nullptr);
        this->recyclableData->returnedValueList = returnedValueList;
    }
#if DBG
    void EnsureNoReturnedValueList()
    {
        Assert(this->recyclableData == nullptr || this->recyclableData->returnedValueList == nullptr);
    }
#endif
#endif

    uint GetScriptContextCount() const { return this->scriptContextCount; }
    Js::ScriptContext* GetScriptContextList() const { return this->scriptContextList; }
    bool WasAnyScriptContextEverRegistered() const { return this->scriptContextEverRegistered; }

#if DBG_DUMP || defined(PROFILE_EXEC)
    void SetTopLevelScriptSite(ScriptSite* topScriptSite) { this->topLevelScriptSite = topScriptSite; }
    ScriptSite* GetTopLevelScriptSite () { return this->topLevelScriptSite; }
#endif
#if DBG || defined(PROFILE_EXEC)
    virtual bool AsyncHostOperationStart(void *) override;
    virtual void AsyncHostOperationEnd(bool wasInAsync, void *) override;
#endif
#if DBG
    virtual void CheckJsReentrancyOnDispose() override;
    bool IsInAsyncHostOperation() const;
#endif

    BOOL ReserveStaticTypeIds(int first, int last);
    Js::TypeId ReserveTypeIds(int count);
    Js::TypeId CreateTypeId();
    Js::TypeId GetNextTypeId() { return nextTypeId; }

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

    // Register a well known type to a Js::TypeId.
    //  wellKnownType:  The well known type which we should register
    //  typeId:         The type id which matches to the well known type
    void SetWellKnownHostTypeId(WellKnownHostType wellKnownType, Js::TypeId typeId);

    uint32_t GetNextPolymorphicCacheState()
    {
        return ++polymorphicCacheState;
    };

    ~ThreadContext();
    void CloseForJSRT();

    //Call back is called for one or more handles
    //It does multiple callbacks (For example: separate call back for GC thread handle & JIT thread handles)
//    template<class Fn>
    //void ShutdownThreads(Fn callback);

    void ShutdownThreads()
    {
#if ENABLE_NATIVE_CODEGEN
        if (jobProcessor)
        {
            jobProcessor->Close();
        }

        if (JITManager::GetJITManager()->IsOOPJITEnabled() && JITManager::GetJITManager()->IsConnected() && m_remoteThreadContextInfo)
        {
            if (JITManager::GetJITManager()->CleanupThreadContext(&m_remoteThreadContextInfo) == S_OK)
            {
                Assert(m_remoteThreadContextInfo == nullptr);
            }
            m_remoteThreadContextInfo = nullptr;
        }
#endif
        if (this->recycler != nullptr)
        {
            this->recycler->ShutdownThread();
        }
    }

    DateTime::HiResTimer * GetHiResTimer() { return &hTimer; }
    ArenaAllocator* GetThreadAlloc() { return &threadAlloc; }
    static std::recursive_mutex& GetMutex() { return s_csThreadContext; }

    ThreadContext(AllocationPolicyManager * allocationPolicyManager = nullptr, JsUtil::ThreadService::ThreadServiceCallback threadServiceCallback = nullptr, bool enableExperimentalFeatures = false);
    static void Add(ThreadContext *threadContext);

    ThreadConfiguration const * GetConfig() const { return &configuration; }

public:
    static ThreadContext* GetContextForCurrentThread();

    Recycler* GetRecycler() { return recycler; }

    Recycler* EnsureRecycler();

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

    const Js::PropertyRecord * GetOrAddPropertyRecord(JsUtil::CharacterBuffer<char16_t> propertyName)
    {
        return GetOrAddPropertyRecordImpl(propertyName, false);
    }
    const Js::PropertyRecord * GetOrAddPropertyRecordBind(JsUtil::CharacterBuffer<char16_t> propertyName)
    {
        return GetOrAddPropertyRecordImpl(propertyName, true);
    }
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
    uint GetHighestPropertyNameIndex() const;

    void SetThreadServiceWrapper(ThreadServiceWrapper*);
    ThreadServiceWrapper* GetThreadServiceWrapper();

    uint NewFunctionNumber() { return ++functionCount; }
    uint PeekNewFunctionNumber() { return functionCount + 1; }

    uint NewSourceInfoNumber() { return ++sourceInfoCount; }

    void AddCodeSize(size_t newCode)
    {
        ::InterlockedExchangeAdd(&nativeCodeSize, newCode);
        ::InterlockedExchangeAdd(&processNativeCodeSize, newCode);
    }
    void AddSourceSize(size_t  newCode) { sourceCodeSize += newCode; }
    void SubCodeSize(size_t  deadCode)
    {
        Assert(nativeCodeSize >= deadCode);
        Assert(processNativeCodeSize >= deadCode);
        ::InterlockedExchangeSubtract(&nativeCodeSize, deadCode);
        ::InterlockedExchangeSubtract(&processNativeCodeSize, deadCode);
    }
    void SubSourceSize(size_t deadCode) { Assert(sourceCodeSize >= deadCode); sourceCodeSize -= deadCode; }
    size_t  GetCodeSize() { return nativeCodeSize; }
    static size_t  GetProcessCodeSize() { return processNativeCodeSize; }
    size_t GetSourceSize() { return sourceCodeSize; }

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

    Js::ScriptEntryExitRecord * GetScriptEntryExit() const { return entryExitRecord; }
    void RegisterCodeGenRecyclableData(Js::CodeGenRecyclableData *const codeGenRecyclableData);
    void UnregisterCodeGenRecyclableData(Js::CodeGenRecyclableData *const codeGenRecyclableData);
#if ENABLE_NATIVE_CODEGEN
    bool IsNativeAddressHelper(void * pCodeAddr, Js::ScriptContext* currentScriptContext);
    BOOL IsNativeAddress(void * pCodeAddr, Js::ScriptContext* currentScriptContext = nullptr);
    JsUtil::JobProcessor *GetJobProcessor();
    Js::Var * GetBailOutRegisterSaveSpace() const { return bailOutRegisterSaveSpace; }
    virtual intptr_t GetBailOutRegisterSaveSpaceAddr() const override { return (intptr_t)bailOutRegisterSaveSpace; }
#endif
    void ResetFunctionCount() { Assert(this->GetScriptSiteHolderCount() == 0); this->functionCount = 0; }
    void PushEntryExitRecord(Js::ScriptEntryExitRecord *);
    void PopEntryExitRecord(Js::ScriptEntryExitRecord *);
    uint EnterScriptStart(Js::ScriptEntryExitRecord *, bool doCleanup);
    void EnterScriptEnd(Js::ScriptEntryExitRecord *, bool doCleanup);

    void * GetTryHandlerAddrOfReturnAddr() { return this->tryHandlerAddrOfReturnAddr; }
    void SetTryHandlerAddrOfReturnAddr(void * addrOfReturnAddr) { this->tryHandlerAddrOfReturnAddr = addrOfReturnAddr; }

    template <bool leaveForHost>
    void LeaveScriptStart(void *);
    template <bool leaveForHost>
    void LeaveScriptEnd(void *);
    void DisposeOnLeaveScript();

    void PushInterpreterFrame(Js::InterpreterStackFrame *interpreterFrame);
    Js::InterpreterStackFrame *PopInterpreterFrame();
    Js::InterpreterStackFrame *GetLeafInterpreterFrame() const { return leafInterpreterFrame; }

    Js::TempArenaAllocatorObject * GetTemporaryAllocator(const char16_t* name);
    void ReleaseTemporaryAllocator(Js::TempArenaAllocatorObject * tempAllocator);

    Js::TempGuestArenaAllocatorObject * GetTemporaryGuestAllocator(const char16_t* name);
    void ReleaseTemporaryGuestAllocator(Js::TempGuestArenaAllocatorObject * tempAllocator);

#ifdef ENABLE_SCRIPT_DEBUGGING
    // Should be called from script context, at the time when construction for scriptcontext is just done.
    void EnsureDebugManager();

    // Should be called from script context 's destructor,
    void ReleaseDebugManager();
#endif

    void RegisterScriptContext(Js::ScriptContext *scriptContext);
    void UnregisterScriptContext(Js::ScriptContext *scriptContext);

    // NoScriptScope
    void SetNoScriptScope(bool noScriptScope) { this->noScriptScope = noScriptScope; }
    bool IsNoScriptScope() { return this->noScriptScope; }

    void SetPendingFinallyException(Js::JavascriptExceptionObject * exceptionObj)
    {
        recyclableData->pendingFinallyException = exceptionObj;
    }

    Js::JavascriptExceptionObject * GetPendingFinallyException()
    {
        return recyclableData->pendingFinallyException;
    }

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

    virtual size_t GetScriptStackLimit() const override;

    virtual HANDLE GetProcessHandle() const override;

    virtual intptr_t GetThreadStackLimitAddr() const override;

#if ENABLE_NATIVE_CODEGEN && defined(ENABLE_WASM_SIMD)
    virtual intptr_t GetSimdTempAreaAddr(uint8_t tempIndex) const override;
#endif

    virtual intptr_t GetDisableImplicitFlagsAddr() const override;
    virtual intptr_t GetImplicitCallFlagsAddr() const override;

    virtual ptrdiff_t GetChakraBaseAddressDifference() const override;
    virtual ptrdiff_t GetCRTBaseAddressDifference() const override;

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
        AutoDisableExpiration(ThreadContext* threadContext):
            _threadContext(threadContext),
            _oldExpirationDisabled(threadContext->disableExpiration)
        {
            _threadContext->disableExpiration = true;
        }

        ~AutoDisableExpiration()
        {
            _threadContext->disableExpiration = _oldExpirationDisabled;
        }

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
    void InvalidatePropertyGuards(Js::PropertyId propertyId);
    void InvalidateAllPropertyGuards();
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
    void ClearInvalidatedUniqueGuards();
    void ClearInlineCaches();
    void ClearIsInstInlineCaches();
    void ClearEnumeratorCaches();
    void ClearEquivalentTypeCaches();
    void ClearScriptContextCaches();

    void RegisterTypeWithProtoPropertyCache(const Js::PropertyId propertyId, Js::Type *const type);
    void InvalidateProtoTypePropertyCaches(const Js::PropertyId propertyId);
    void InternalInvalidateProtoTypePropertyCaches(const Js::PropertyId propertyId);
    void InvalidateAllProtoTypePropertyCaches();

    BOOL HasUnhandledException() const { return hasUnhandledException; }
    void SetHasUnhandledException() {hasUnhandledException = TRUE; }
    void ResetHasUnhandledException() {hasUnhandledException = FALSE; }
    void SetUnhandledExceptionObject(Js::JavascriptExceptionObject* exceptionObject) {recyclableData->unhandledExceptionObject  = exceptionObject; }
    Js::JavascriptExceptionObject* GetUnhandledExceptionObject() const  { return recyclableData->unhandledExceptionObject; };

    // To temporarily keep throwing exception object alive (thrown but not yet caught)
    void SaveTempUncaughtException(Js::JavascriptExceptionObject* exceptionObject)
    {
        Js::JavascriptExceptionObject::Insert(&recyclableData->tempUncaughtException, exceptionObject);
    }
    void ClearTempUncaughtException(Js::JavascriptExceptionObject* exceptionObject)
    {
        Js::JavascriptExceptionObject::Remove(&recyclableData->tempUncaughtException, exceptionObject);
    }

public:
    bool HasCatchHandler() const { return hasCatchHandler; }
    void SetHasCatchHandler(bool hasCatchHandler) { this->hasCatchHandler = hasCatchHandler; }

    bool IsUserCode() const { return this->hasCatchHandlerToUserCode; }
    void SetIsUserCode(bool set) { this->hasCatchHandlerToUserCode = set; }

    void QueueFreeOldEntryPointInfoIfInScript(Js::FunctionEntryPointInfo* oldEntryPointInfo)
    {
        if (this->IsInScript())
        {
            // Add it to the list only if it's not already in it
            if (oldEntryPointInfo->nextEntryPoint == nullptr && !oldEntryPointInfo->IsCleanedUp())
            {
                oldEntryPointInfo->nextEntryPoint = recyclableData->oldEntryPointInfo;
                recyclableData->oldEntryPointInfo = oldEntryPointInfo;
            }
        }
    }

    bool IsOldEntryPointInfo(Js::ProxyEntryPointInfo* entryPointInfo)
    {
        Js::FunctionEntryPointInfo* current = this->recyclableData->oldEntryPointInfo;
        while (current != nullptr)
        {
            if (current == entryPointInfo)
                return true;
            current = current->nextEntryPoint;
        }
        return false;
    }

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
    static void ProbeCurrentStack2(size_t size, Js::ScriptContext *scriptContext, uint32_t u1, uint32_t u2)
    {
        ProbeCurrentStack(size, scriptContext);
    }

#if ENABLE_PROFILE_INFO
    void EnsureSourceProfileManagersByUrlMap();
    Js::SourceDynamicProfileManager* GetSourceDynamicProfileManager(_In_z_ const char16_t* url, _In_ uint hash, _Inout_ bool* addref);
    uint ReleaseSourceDynamicProfileManagers(const char16_t* url);
#endif

    void EnsureSymbolRegistrationMap();
    const Js::PropertyRecord* GetSymbolFromRegistrationMap(const char16_t* stringKey, charcount_t stringLength);
    const Js::PropertyRecord* AddSymbolToRegistrationMap(const char16_t* stringKey, charcount_t stringLength);

    inline void ClearPendingSOError()
    {
        this->GetPendingSOErrorObject()->ClearError();
    }

    inline void ClearPendingOOMError()
    {
        this->GetPendingOOMErrorObject()->ClearError();
    }

    Js::JavascriptExceptionObject *GetPendingSOErrorObject()
    {
        Assert(recyclableData->soErrorObject.IsPendingExceptionObject());
        return &recyclableData->soErrorObject;
    }

    Js::JavascriptExceptionObject *GetPendingOOMErrorObject()
    {
        Assert(recyclableData->oomErrorObject.IsPendingExceptionObject());
        return &recyclableData->oomErrorObject;
    }

    Js::JavascriptExceptionObject *GetPendingTerminatedErrorObject()
    {
        return &recyclableData->terminatedErrorObject;
    }

    Js::JavascriptExceptionObject* GetRecordedException()
    {
        return recyclableData->exceptionObject;
    }

    bool GetPropagateException()
    {
        return recyclableData->propagateException;
    }

    void SetHasThrownPendingException()
    {
        Assert(this->IsInScript());
        this->hasThrownPendingException = true;
    }

    bool * GetHasBailedOutBitPtr()
    {
        return this->hasBailedOutBitPtr;
    }

    void SetHasBailedOutBitPtr(bool *setValue)
    {
        this->hasBailedOutBitPtr = setValue;
    }

    void SetRecordedException(Js::JavascriptExceptionObject* exceptionObject, bool propagateToDebugger = false)
    {
        this->recyclableData->exceptionObject = exceptionObject;
        this->recyclableData->propagateException = propagateToDebugger;
    }

#ifdef ENABLE_CUSTOM_ENTROPY
    Entropy& GetEntropy()
    {
        return entropy;
    }
#endif

    Js::ImplicitCallFlags * GetAddressOfImplicitCallFlags()
    {
        return &implicitCallFlags;
    }

    DisableImplicitFlags * GetAddressOfDisableImplicitFlags()
    {
        return &disableImplicitFlags;
    }

    Js::ImplicitCallFlags GetImplicitCallFlags()
    {
        return implicitCallFlags;
    }

    void SetImplicitCallFlags(Js::ImplicitCallFlags flags)
    {
        //Note: this action is inlined into JITed code in Lowerer::GenerateCallProfiling.
        //   if you change this, you might want to add it there too.
        implicitCallFlags = flags;
    }

    void ClearImplicitCallFlags();
    void ClearImplicitCallFlags(Js::ImplicitCallFlags flags);

    void AddImplicitCallFlags(Js::ImplicitCallFlags flags)
    {
        SetImplicitCallFlags((Js::ImplicitCallFlags)(implicitCallFlags | flags));
    }

    void CheckAndResetImplicitCallAccessorFlag();

    template <class Fn>
    inline Js::Var ExecuteImplicitCall(Js::RecyclableObject * function, Js::ImplicitCallFlags flags, Fn implicitCall)
    {
        AutoReentrancyHandler autoReentrancyHandler(this);

        Js::FunctionInfo::Attributes attributes = Js::FunctionInfo::GetAttributes(function);

        // we can hoist out const method if we know the function doesn't have side effect,
        // and the value can be hoisted.
        if (this->HasNoSideEffect(function, attributes))
        {
            // Has no side effect means the function does not change global value or
            // will check for implicit call flags
            Js::Var result = implicitCall();

            // If the value is on stack we need to bailout so that it can be boxed.
            // Instead of putting this in valueOf (or other builtins which have no side effect) adding
            // the check here to cover any other scenario we might miss.
            if (IsOnStack(result))
            {
                AddImplicitCallFlags(flags);
            }
            return result;
        }

        // Don't call the implicit call if disable implicit call
        if (IsDisableImplicitCall())
        {
            AddImplicitCallFlags(flags);
            // Return "undefined" just so we have a valid var, in case subsequent instructions are executed
            // before we bail out.
            return function->GetScriptContext()->GetLibrary()->GetUndefined();
        }

        if ((attributes & Js::FunctionInfo::HasNoSideEffect) != 0)
        {
            // Has no side effect means the function does not change global value or
            // will check for implicit call flags
            Js::Var result = implicitCall();

            // If the value is on stack we need to bailout so that it can be boxed.
            // Instead of putting this in valueOf (or other builtins which have no side effect) adding
            // the check here to cover any other scenario we might miss.
            if (IsOnStack(result))
            {
                AddImplicitCallFlags(flags);
            }
            return result;
        }

        // Save and restore implicit flags around the implicit call
        struct RestoreFlags
        {
            ThreadContext * const ctx;
            const Js::ImplicitCallFlags flags;
            const Js::ImplicitCallFlags savedFlags;

            RestoreFlags(ThreadContext *ctx, Js::ImplicitCallFlags flags) :
                ctx(ctx),
                flags(flags),
                savedFlags(ctx->GetImplicitCallFlags())
            {
            }

            ~RestoreFlags()
            {
                ctx->SetImplicitCallFlags(static_cast<Js::ImplicitCallFlags>(savedFlags | flags));
            }
        };

        RestoreFlags restoreFlags(this, flags);
        return implicitCall();
    }
    bool HasNoSideEffect(Js::RecyclableObject * function) const;
    bool HasNoSideEffect(Js::RecyclableObject * function, Js::FunctionInfo::Attributes attr) const;
    bool RecordImplicitException();
    DisableImplicitFlags GetDisableImplicitFlags() const { return disableImplicitFlags; }
    void SetDisableImplicitFlags(DisableImplicitFlags flags) { disableImplicitFlags = flags; }
    bool IsDisableImplicitCall() const { return (disableImplicitFlags & DisableImplicitCallFlag) != 0; }
    bool IsDisableImplicitException() const { return (disableImplicitFlags & DisableImplicitExceptionFlag) != 0; }
    void DisableImplicitCall() { disableImplicitFlags = (DisableImplicitFlags)(disableImplicitFlags | DisableImplicitCallFlag); }
    void ClearDisableImplicitFlags() { disableImplicitFlags = DisableImplicitNoFlag; }

    virtual uint GetRandomNumber() override;
    virtual bool DoSpecialMarkOnScanStack() override { return this->DoRedeferFunctionBodies(); }
    virtual void OnScanStackCallback(void ** stackTop, size_t byteCount, void ** registers, size_t registersByteCount) override;

    virtual void PostSweepRedeferralCallBack() override;

    // DefaultCollectWrapper
    virtual void PreCollectionCallBack(CollectionFlags flags) override;
    virtual void PreSweepCallback() override;
    virtual void PreRescanMarkCallback() override;
    virtual void WaitCollectionCallBack() override;
    virtual void PostCollectionCallBack() override;
    virtual BOOL ExecuteRecyclerCollectionFunction(Recycler * recycler, CollectionFunction function, CollectionFlags flags) override;
    virtual void DisposeObjects(Recycler * recycler) override;
    virtual void PreDisposeObjectsCallBack() override;

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
    bool IsScriptActive() const { return isScriptActive; }
    void SetIsScriptActive(bool isActive) { isScriptActive = isActive; }
    bool IsExecutionDisabled() const
    {
        return this->GetStackLimitForCurrentThread() == Js::Constants::StackLimitForScriptInterrupt;
    }
    void DisableExecution();
    void EnableExecution();
    bool TestThreadContextFlag(ThreadContextFlags threadContextFlag) const;
    void SetThreadContextFlag(ThreadContextFlags threadContextFlag);
    void ClearThreadContextFlag(ThreadContextFlags threadContextFlag);

    void SetForceOneIdleCollection();

    bool IsInThreadServiceCallback() const { return threadService.IsInCallback(); }

#ifdef ENABLE_SCRIPT_DEBUGGING
    Js::DebugManager * GetDebugManager() const { return this->debugManager; }
#endif

    const NativeLibraryEntryRecord::Entry* PeekNativeLibraryEntry() const { return this->nativeLibraryEntry.Peek(); }
    void PushNativeLibraryEntry(_In_ NativeLibraryEntryRecord::Entry* entry) { this->nativeLibraryEntry.Push(entry); }
    void PopNativeLibraryEntry() { this->nativeLibraryEntry.Pop(); }

    bool IsProfilingUserCode() const { return isProfilingUserCode; }
    void SetIsProfilingUserCode(bool value) { isProfilingUserCode = value; }

#if DBG_DUMP
    uint scriptSiteCount;
#endif

    uint bailOutByteCodeLocationCount;
    //
    // Regex helpers
    //
    UnifiedRegex::StandardChars<uint8_t>* GetStandardChars(__inout_opt uint8_t* dummy);
    UnifiedRegex::StandardChars<char16_t>* GetStandardChars(__inout_opt char16_t* dummy);

    bool IsOptimizedForManyInstances() const { return isOptimizedForManyInstances; }

    void OptimizeForManyInstances(const bool optimizeForManyInstances)
    {
        Assert(!recycler || optimizeForManyInstances == isOptimizedForManyInstances); // mode cannot be changed after recycler is created
        isOptimizedForManyInstances = optimizeForManyInstances;

    }

#if ENABLE_NATIVE_CODEGEN
    bool IsBgJitEnabled() const { return bgJit; }

    void EnableBgJit(const bool enableBgJit)
    {
        Assert(!jobProcessor || enableBgJit == bgJit);
        bgJit = enableBgJit;
    }
#endif

    void* GetJSRTRuntime() const { return jsrtRuntime; }
    void SetJSRTRuntime(void* runtime);

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

#if ENABLE_JS_REENTRANCY_CHECK
public:
    void SetNoJsReentrancy(bool val) { noJsReentrancy = val; }
    bool GetNoJsReentrancy() { return noJsReentrancy; }
    void AssertJsReentrancy()
    {
        if (GetNoJsReentrancy())
        {
            Js::Throw::FatalJsReentrancyError();
        }
    }
#else
    void AssertJsReentrancy() {}
#endif

public:
    void CheckAndResetReentrancySafeOrHandled()
    {
        AssertOrFailFast(reentrancySafeOrHandled || isInReentrancySafeRegion);
        SetReentrancySafeOrHandled(false);
    }

    void SetReentrancySafeOrHandled(bool val) { reentrancySafeOrHandled = val; }
    bool GetReentrancySafeOrHandled() { return reentrancySafeOrHandled; }
    void SetIsInReentrancySafeRegion(bool val) { isInReentrancySafeRegion = val; }
    bool GetIsInReentrancySafeRegion() { return isInReentrancySafeRegion; }

    template <typename Fn>
    Js::Var SafeReentrantCall(Fn fn)
    {
        AutoReentrancyHandler autoReentrancyHandler(this);
        return fn();
    }

    bool IsEntryPointToBuiltInOperationIdCacheInitialized()
    {
        return entryPointToBuiltInOperationIdCache.Count() != 0;
    }

    bool GetBuiltInOperationIdFromEntryPoint(Js::JavascriptMethod entryPoint, uint * id)
    {
        return entryPointToBuiltInOperationIdCache.TryGetValue(entryPoint, id);
    }

    void SetBuiltInOperationIdForEntryPoint(Js::JavascriptMethod entryPoint, uint id)
    {
        entryPointToBuiltInOperationIdCache.Add(entryPoint, id);
    }

    void ResetEntryPointToBuiltInOperationIdCache()
    {
        entryPointToBuiltInOperationIdCache.ResetNoDelete();
    }

    uint8_t LoopDepth() const
    {
        return loopDepth;
    }

    void SetLoopDepth(const uint8_t loopDepth)
    {
        this->loopDepth = loopDepth;
    }

    void IncrementLoopDepth()
    {
        if(loopDepth != UCHAR_MAX)
        {
            ++loopDepth;
        }
    }

    void DecrementLoopDepth()
    {
        if(loopDepth != 0)
        {
            --loopDepth;
        }
    }

private:
    class ThreadContextRecyclerTelemetryHostInterface : public RecyclerTelemetryHostInterface
    {
    public:
        ThreadContextRecyclerTelemetryHostInterface(ThreadContext* tc) :
            tc(tc)
        {
        }

        virtual LPFILETIME GetLastScriptExecutionEndTime() const;
        virtual bool TransmitGCTelemetryStats(RecyclerTelemetryInfo& rti);
        virtual bool TransmitTelemetryError(const RecyclerTelemetryInfo& rti, const char * msg);
        virtual bool TransmitHeapUsage(size_t totalHeapBytes, size_t usedHeapBytes, double heapUsedRatio);
        virtual bool IsThreadBound() const;
        virtual uint32_t GetCurrentScriptThreadID() const;
        virtual bool IsTelemetryProviderEnabled() const;
        virtual uint GetClosedContextCount() const;

    private:
        ThreadContext * tc;
    };
    ThreadContextRecyclerTelemetryHostInterface recyclerTelemetryHostInterface;
};

extern void(*InitializeAdditionalProperties)(ThreadContext *threadContext);

// This is for protecting a region of code, where we can't recover and be consistent upon failures (mainly due to OOM and SO).
// FailFast on that.
class AutoDisableInterrupt
{
public:
    AutoDisableInterrupt(ThreadContext *threadContext, bool explicitCompletion = true)
        : m_operationCompleted(false), m_interruptDisableState(false), m_threadContext(threadContext), m_explicitCompletion(explicitCompletion)
    {
        if (m_threadContext->HasInterruptPoller())
        {
            m_interruptDisableState = m_threadContext->GetInterruptPoller()->IsDisabled();
            m_threadContext->GetInterruptPoller()->SetDisabled(true);
        }
    }
    ~AutoDisableInterrupt()
    {
        if (m_threadContext->HasInterruptPoller())
        {
            m_threadContext->GetInterruptPoller()->SetDisabled(m_interruptDisableState);
        }

        if (m_explicitCompletion && !m_operationCompleted)
        {
            AssertOrFailFast(false);
        }
    }
    void RequireExplicitCompletion() { m_explicitCompletion = true; }
    void Completed() { m_operationCompleted = true; }

private:
    ThreadContext * m_threadContext;
    bool m_operationCompleted;
    bool m_interruptDisableState;
    bool m_explicitCompletion;
};

class AutoReentrancyHandler
{
    ThreadContext * m_threadContext;
    bool m_savedReentrancySafeOrHandled;

public:
    AutoReentrancyHandler(ThreadContext * threadContext)
    {
        m_threadContext = threadContext;
        m_savedReentrancySafeOrHandled = threadContext->GetReentrancySafeOrHandled();
        threadContext->SetReentrancySafeOrHandled(true);
    }

    ~AutoReentrancyHandler()
    {
        m_threadContext->SetReentrancySafeOrHandled(m_savedReentrancySafeOrHandled);
    }
};

class AutoReentrancySafeRegion
{
    ThreadContext * m_threadContext;
    bool m_savedIsInReentrancySafeRegion;

public:
    AutoReentrancySafeRegion(ThreadContext * threadContext)
    {
        m_threadContext = threadContext;
        m_savedIsInReentrancySafeRegion = threadContext->GetIsInReentrancySafeRegion();
        threadContext->SetIsInReentrancySafeRegion(true);
    }

    ~AutoReentrancySafeRegion()
    {
        m_threadContext->SetIsInReentrancySafeRegion(m_savedIsInReentrancySafeRegion);
    }
};

#if ENABLE_JS_REENTRANCY_CHECK
class JsReentLock
{
    ThreadContext *m_threadContext;
#if DBG
    Js::Var m_arrayObject;
    Js::Var m_arrayObject2; // This is for adding the second object in the mutation equation.
#endif

    bool m_savedNoJsReentrancy;

public:
    JsReentLock(ThreadContext *threadContext)
#if DBG
        : m_arrayObject(nullptr), m_arrayObject2(nullptr)
#endif
    {
        m_savedNoJsReentrancy = threadContext->GetNoJsReentrancy();
        threadContext->SetNoJsReentrancy(true);
        m_threadContext = threadContext;
    }

    void unlock() { m_threadContext->SetNoJsReentrancy(m_savedNoJsReentrancy); }
    void relock() { m_threadContext->SetNoJsReentrancy(true); }

#if DBG
    void setObjectForMutation(Js::Var object);
    void setSecondObjectForMutation(Js::Var object);
    void MutateArrayObject();
#endif

    ~JsReentLock()
    {
        m_threadContext->SetNoJsReentrancy(m_savedNoJsReentrancy);
    }
#if DBG
private:
    static void MutateArrayObject(Js::Var arrayObject);
#endif
};
#endif
