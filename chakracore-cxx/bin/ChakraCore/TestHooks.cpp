//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Runtime.h"

#include "Library/DelayFreeArrayBufferHelper.h"
#include "Base/ThreadContext.h"

#include "Base/StackProber.h"
#include "Base/ScriptContextProfiler.h"

#include "Language/JavascriptConversion.h"

#include "Base/ScriptContextOptimizationOverrideInfo.h"
#include "Base/ScriptContextBase.h"
#include "Base/ScriptContextInfo.h"
#include "Base/ScriptContext.h"
#include "Base/LeaveScriptObject.h"
#include "Base/PropertyRecord.h"

#include "ByteCode/ByteCodeReader.h"
#include "Language/TaggedInt.h"

#include "Library/RootObjectBase.h"
#include "Library/GlobalObject.h"

#include "Library/LiteralString.h"
#include "Library/ConcatString.h"
#include "Library/CompoundString.h"
#include "Library/PropertyRecordUsageCache.h"
#include "Library/PropertyString.h"
#include "Library/SingleCharString.h"

#include "Library/JavascriptTypedNumber.h"
#include "Library/SparseArraySegment.h"
#include "Library/JavascriptError.h"
#include "Library/JavascriptArray.h"
#include "Library/JavascriptSymbol.h"

#include "Library/AtomicsObject.h"
#include "Library/DetachedStateBase.h"
#include "Library/ArrayBuffer.h"
#include "Library/SharedArrayBuffer.h"
#include "Library/TypedArray.h"
#include "Library/JavascriptBoolean.h"
#include "Library/JavascriptBigInt.h"
#include "Library/WebAssemblyEnvironment.h"
#include "Library/WebAssemblyTable.h"
#include "Library/WebAssemblyMemory.h"
#include "Library/WebAssemblyModule.h"
#include "Library/WebAssembly.h"
#include "Core/ConfigParser.h"
#include "TestHooks.h"

namespace PlatformAgnostic
{
namespace UnicodeText
{
namespace Internal
{
// this is in place of including PlatformAgnostic/UnicodeTextInternal.h, which has template
// instantiations that upset Clang on macOS and Linux
int LogicalStringCompareImpl(const char16_t* p1, int p1size, const char16_t* p2, int p2size);
}
}
}

int32_t SetConfigFlags(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags)
{
    CmdLineArgsParser parser(customConfigFlags);
    if (parser.Parse(vargs) != 0)
    {
        return E_FAIL;
    }

    return S_OK;
}

int32_t SetConfigFile(char16_t* strConfigFile)
{
    CmdLineArgsParser parser;
    ConfigParser::ParseCustomConfigFile(parser, strConfigFile);
    return S_OK;
}

int32_t PrintConfigFlagsUsageString()
{
    Js::ConfigFlagsTable::PrintUsageString();
    return S_OK;
}

int32_t SetAssertToConsoleFlag(bool flag)
{
#ifdef DBG
    AssertsToConsole = flag;
#endif
    return S_OK;
}

int32_t SetEnableCheckMemoryLeakOutput(bool flag)
{
#if defined(CHECK_MEMORY_LEAK)
    MemoryLeakCheck::SetEnableOutput(flag);
#endif
    return S_OK;
}

void NotifyUnhandledException(PEXCEPTION_POINTERS exceptionInfo)
{
}

#define FLAG(type, name, description, defaultValue, ...) FLAG_##type##(name)
#define FLAG_String(name) \
    bool IsEnabled##name##Flag() \
    { \
        return Js::Configuration::Global.flags.IsEnabled(Js::##name##Flag); \
    } \
    int32_t Get##name##Flag(BSTR *flag) \
    { \
        *flag = SysAllocString(Js::Configuration::Global.flags.##name##); \
        return (*flag == NULL ? E_OUTOFMEMORY : S_OK); \
    } \
    int32_t Set##name##Flag(BSTR flag) \
    { \
        Js::Configuration::Global.flags.##name = flag; \
        return S_OK; \
    }
#define FLAG_Boolean(name) \
    bool IsEnabled##name##Flag() \
    { \
        return Js::Configuration::Global.flags.IsEnabled(Js::##name##Flag); \
    } \
    int32_t Get##name##Flag(bool *flag) \
    { \
        *flag = Js::Configuration::Global.flags.##name##; \
        return S_OK; \
    } \
    int32_t Set##name##Flag(bool flag) \
    { \
        Js::Configuration::Global.flags.##name = flag; \
        return S_OK; \
    }
#define FLAG_Number(name) \
    bool IsEnabled##name##Flag() \
    { \
        return Js::Configuration::Global.flags.IsEnabled(Js::##name##Flag); \
    } \
    int32_t Get##name##Flag(int *flag) \
    { \
        *flag = Js::Configuration::Global.flags.##name##; \
        return S_OK; \
    } \
    int32_t Set##name##Flag(int flag) \
    { \
        Js::Configuration::Global.flags.##name = flag; \
        return S_OK; \
    }
// skipping other types
#define FLAG_Phases(name)
#define FLAG_NumberSet(name)
#define FLAG_NumberPairSet(name)
#define FLAG_NumberTrioSet(name)
#define FLAG_NumberRange(name)
#include "Interface/ConfigFlagsList.h"
#undef FLAG
#undef FLAG_String
#undef FLAG_Boolean
#undef FLAG_Number
#undef FLAG_Phases
#undef FLAG_NumberSet
#undef FLAG_NumberPairSet
#undef FLAG_NumberTrioSet
#undef FLAG_NumberRange

int32_t OnChakraCoreLoaded()
{
    TestHooks testHooks =
    {
        SetConfigFlags,
        SetConfigFile,
        PrintConfigFlagsUsageString,
        SetAssertToConsoleFlag,
        SetEnableCheckMemoryLeakOutput,
        PlatformAgnostic::UnicodeText::Internal::LogicalStringCompareImpl,

        //BigInt hooks
        Js::JavascriptBigInt::AddDigit,
        Js::JavascriptBigInt::SubDigit,
        Js::JavascriptBigInt::MulDigit,

#if DBG
IsEnabledArrayValidateFlag, GetArrayValidateFlag, SetArrayValidateFlag,
IsEnabledMemOpMissingValueValidateFlag, GetMemOpMissingValueValidateFlag, SetMemOpMissingValueValidateFlag,
IsEnabledOOPJITFixupValidateFlag, GetOOPJITFixupValidateFlag, SetOOPJITFixupValidateFlag,
#endif
#ifdef ARENA_MEMORY_VERIFY
IsEnabledArenaNoFreeListFlag, GetArenaNoFreeListFlag, SetArenaNoFreeListFlag,
IsEnabledArenaNoPageReuseFlag, GetArenaNoPageReuseFlag, SetArenaNoPageReuseFlag,
IsEnabledArenaUseHeapAllocFlag, GetArenaUseHeapAllocFlag, SetArenaUseHeapAllocFlag,
#endif
IsEnabledValidateInlineStackFlag, GetValidateInlineStackFlag, SetValidateInlineStackFlag,
IsEnabledAsmDiffFlag, GetAsmDiffFlag, SetAsmDiffFlag,
IsEnabledAsmDumpModeFlag, GetAsmDumpModeFlag, SetAsmDumpModeFlag,
IsEnabledAsmJsFlag, GetAsmJsFlag, SetAsmJsFlag,
IsEnabledAsmJsStopOnErrorFlag, GetAsmJsStopOnErrorFlag, SetAsmJsStopOnErrorFlag,
IsEnabledAsmJsEdgeFlag, GetAsmJsEdgeFlag, SetAsmJsEdgeFlag,
IsEnabledWasmFlag, GetWasmFlag, SetWasmFlag,
IsEnabledWasmI64Flag, GetWasmI64Flag, SetWasmI64Flag,
IsEnabledWasmFastArrayFlag, GetWasmFastArrayFlag, SetWasmFastArrayFlag,
IsEnabledWasmSharedArrayVirtualBufferFlag, GetWasmSharedArrayVirtualBufferFlag, SetWasmSharedArrayVirtualBufferFlag,
IsEnabledWasmMathExFilterFlag, GetWasmMathExFilterFlag, SetWasmMathExFilterFlag,
IsEnabledWasmCheckVersionFlag, GetWasmCheckVersionFlag, SetWasmCheckVersionFlag,
IsEnabledWasmAssignModuleIDFlag, GetWasmAssignModuleIDFlag, SetWasmAssignModuleIDFlag,
IsEnabledWasmIgnoreLimitsFlag, GetWasmIgnoreLimitsFlag, SetWasmIgnoreLimitsFlag,
IsEnabledWasmFoldFlag, GetWasmFoldFlag, SetWasmFoldFlag,
IsEnabledWasmIgnoreResponseFlag, GetWasmIgnoreResponseFlag, SetWasmIgnoreResponseFlag,
IsEnabledWasmMaxTableSizeFlag, GetWasmMaxTableSizeFlag, SetWasmMaxTableSizeFlag,
IsEnabledWasmThreadsFlag, GetWasmThreadsFlag, SetWasmThreadsFlag,
IsEnabledWasmMultiValueFlag, GetWasmMultiValueFlag, SetWasmMultiValueFlag,
IsEnabledWasmSignExtendsFlag, GetWasmSignExtendsFlag, SetWasmSignExtendsFlag,
IsEnabledWasmNontrappingFlag, GetWasmNontrappingFlag, SetWasmNontrappingFlag,

// WebAssembly Experimental Features
// Master WasmExperimental flag to activate WebAssembly experimental features
IsEnabledWasmExperimentalFlag, GetWasmExperimentalFlag, SetWasmExperimentalFlag,

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
IsEnabledWasmSimdFlag, GetWasmSimdFlag, SetWasmSimdFlag,

IsEnabledAssertBreakFlag, GetAssertBreakFlag, SetAssertBreakFlag,
IsEnabledAssertPopUpFlag, GetAssertPopUpFlag, SetAssertPopUpFlag,
IsEnabledAssertIgnoreFlag, GetAssertIgnoreFlag, SetAssertIgnoreFlag,
IsEnabledAsyncDebuggingFlag, GetAsyncDebuggingFlag, SetAsyncDebuggingFlag,
IsEnabledBailOnNoProfileLimitFlag, GetBailOnNoProfileLimitFlag, SetBailOnNoProfileLimitFlag,
IsEnabledBailOnNoProfileRejitLimitFlag, GetBailOnNoProfileRejitLimitFlag, SetBailOnNoProfileRejitLimitFlag,
IsEnabledBaselineModeFlag, GetBaselineModeFlag, SetBaselineModeFlag,
IsEnabledDumpOnCrashFlag, GetDumpOnCrashFlag, SetDumpOnCrashFlag,
IsEnabledFullMemoryDumpFlag, GetFullMemoryDumpFlag, SetFullMemoryDumpFlag,
#ifdef BAILOUT_INJECTION

IsEnabledBailOutAtEveryLineFlag, GetBailOutAtEveryLineFlag, SetBailOutAtEveryLineFlag,
IsEnabledBailOutAtEveryByteCodeFlag, GetBailOutAtEveryByteCodeFlag, SetBailOutAtEveryByteCodeFlag,
IsEnabledBailOutAtEveryImplicitCallFlag, GetBailOutAtEveryImplicitCallFlag, SetBailOutAtEveryImplicitCallFlag,

#endif
IsEnabledBenchmarkFlag, GetBenchmarkFlag, SetBenchmarkFlag,
IsEnabledBgJitFlag, GetBgJitFlag, SetBgJitFlag,
IsEnabledBgParseFlag, GetBgParseFlag, SetBgParseFlag,
IsEnabledBgJitDelayFlag, GetBgJitDelayFlag, SetBgJitDelayFlag,
IsEnabledBgJitDelayFgBufferFlag, GetBgJitDelayFgBufferFlag, SetBgJitDelayFgBufferFlag,
IsEnabledBgJitPendingFuncCapFlag, GetBgJitPendingFuncCapFlag, SetBgJitPendingFuncCapFlag,

IsEnabledCreateFunctionProxyFlag, GetCreateFunctionProxyFlag, SetCreateFunctionProxyFlag,
IsEnabledHybridFgJitFlag, GetHybridFgJitFlag, SetHybridFgJitFlag,
IsEnabledHybridFgJitBgQueueLengthThresholdFlag, GetHybridFgJitBgQueueLengthThresholdFlag, SetHybridFgJitBgQueueLengthThresholdFlag,
IsEnabledBytecodeHistFlag, GetBytecodeHistFlag, SetBytecodeHistFlag,
IsEnabledCurrentSourceInfoFlag, GetCurrentSourceInfoFlag, SetCurrentSourceInfoFlag,
IsEnabledCFGLogFlag, GetCFGLogFlag, SetCFGLogFlag,
IsEnabledCheckAlignmentFlag, GetCheckAlignmentFlag, SetCheckAlignmentFlag,
IsEnabledCheckEmitBufferPermissionsFlag, GetCheckEmitBufferPermissionsFlag, SetCheckEmitBufferPermissionsFlag,
#ifdef CHECK_MEMORY_LEAK
IsEnabledCheckMemoryLeakFlag, GetCheckMemoryLeakFlag, SetCheckMemoryLeakFlag,
IsEnabledDumpOnLeakFlag, GetDumpOnLeakFlag, SetDumpOnLeakFlag,
#endif
IsEnabledCheckOpHelpersFlag, GetCheckOpHelpersFlag, SetCheckOpHelpersFlag,
IsEnabledCloneInlinedPolymorphicCachesFlag, GetCloneInlinedPolymorphicCachesFlag, SetCloneInlinedPolymorphicCachesFlag,
IsEnabledConcurrentRuntimeFlag, GetConcurrentRuntimeFlag, SetConcurrentRuntimeFlag,
IsEnabledConstructorInlineThresholdFlag, GetConstructorInlineThresholdFlag, SetConstructorInlineThresholdFlag,
IsEnabledConstructorCallsRequiredToFinalizeCachedTypeFlag, GetConstructorCallsRequiredToFinalizeCachedTypeFlag, SetConstructorCallsRequiredToFinalizeCachedTypeFlag,
IsEnabledPropertyCacheMissPenaltyFlag, GetPropertyCacheMissPenaltyFlag, SetPropertyCacheMissPenaltyFlag,
IsEnabledPropertyCacheMissThresholdFlag, GetPropertyCacheMissThresholdFlag, SetPropertyCacheMissThresholdFlag,
IsEnabledPropertyCacheMissResetFlag, GetPropertyCacheMissResetFlag, SetPropertyCacheMissResetFlag,
IsEnabledDebugFlag, GetDebugFlag, SetDebugFlag,



IsEnabledDebugWindowFlag, GetDebugWindowFlag, SetDebugWindowFlag,
IsEnabledParserStateCacheFlag, GetParserStateCacheFlag, SetParserStateCacheFlag,
IsEnabledCompressParserStateCacheFlag, GetCompressParserStateCacheFlag, SetCompressParserStateCacheFlag,
IsEnabledDeferTopLevelTillFirstCallFlag, GetDeferTopLevelTillFirstCallFlag, SetDeferTopLevelTillFirstCallFlag,
IsEnabledDeferParseFlag, GetDeferParseFlag, SetDeferParseFlag,
IsEnabledDirectCallTelemetryStatsFlag, GetDirectCallTelemetryStatsFlag, SetDirectCallTelemetryStatsFlag,
IsEnabledDisableArrayBTreeFlag, GetDisableArrayBTreeFlag, SetDisableArrayBTreeFlag,
IsEnabledDisableRentalThreadingFlag, GetDisableRentalThreadingFlag, SetDisableRentalThreadingFlag,
IsEnabledDisableVTuneSourceLineInfoFlag, GetDisableVTuneSourceLineInfoFlag, SetDisableVTuneSourceLineInfoFlag,
IsEnabledDisplayMemStatsFlag, GetDisplayMemStatsFlag, SetDisplayMemStatsFlag,
#ifdef DUMP_FRAGMENTATION_STATS
IsEnabledDumpFragmentationStatsFlag, GetDumpFragmentationStatsFlag, SetDumpFragmentationStatsFlag,
#endif
IsEnabledDumpIRAddressesFlag, GetDumpIRAddressesFlag, SetDumpIRAddressesFlag,
IsEnabledDumpLineNoInColorFlag, GetDumpLineNoInColorFlag, SetDumpLineNoInColorFlag,
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
IsEnabledDumpObjectGraphOnExitFlag, GetDumpObjectGraphOnExitFlag, SetDumpObjectGraphOnExitFlag,
IsEnabledDumpObjectGraphOnCollectFlag, GetDumpObjectGraphOnCollectFlag, SetDumpObjectGraphOnCollectFlag,
#endif
IsEnabledDumpEvalStringOnRemovalFlag, GetDumpEvalStringOnRemovalFlag, SetDumpEvalStringOnRemovalFlag,
IsEnabledDumpObjectGraphOnEnumFlag, GetDumpObjectGraphOnEnumFlag, SetDumpObjectGraphOnEnumFlag,
#ifdef DYNAMIC_PROFILE_STORAGE
IsEnabledDynamicProfileCacheFlag, GetDynamicProfileCacheFlag, SetDynamicProfileCacheFlag, IsEnabledDpcFlag, GetDpcFlag, SetDpcFlag,
IsEnabledDynamicProfileCacheDirFlag, GetDynamicProfileCacheDirFlag, SetDynamicProfileCacheDirFlag,
IsEnabledDynamicProfileInputFlag, GetDynamicProfileInputFlag, SetDynamicProfileInputFlag, IsEnabledDpiFlag, GetDpiFlag, SetDpiFlag,
#endif
#ifdef EDIT_AND_CONTINUE
IsEnabledEditTestFlag, GetEditTestFlag, SetEditTestFlag,
#endif
IsEnabledWininetProfileCacheFlag, GetWininetProfileCacheFlag, SetWininetProfileCacheFlag,
IsEnabledNoDynamicProfileInMemoryCacheFlag, GetNoDynamicProfileInMemoryCacheFlag, SetNoDynamicProfileInMemoryCacheFlag,
IsEnabledProfileBasedSpeculativeJitFlag, GetProfileBasedSpeculativeJitFlag, SetProfileBasedSpeculativeJitFlag,
IsEnabledProfileBasedSpeculationCapFlag, GetProfileBasedSpeculationCapFlag, SetProfileBasedSpeculationCapFlag,
IsEnabledExecuteByteCodeBufferReturnsInvalidByteCodeFlag, GetExecuteByteCodeBufferReturnsInvalidByteCodeFlag, SetExecuteByteCodeBufferReturnsInvalidByteCodeFlag,
IsEnabledExpirableCollectionGCCountFlag, GetExpirableCollectionGCCountFlag, SetExpirableCollectionGCCountFlag,
IsEnabledExpirableCollectionTriggerThresholdFlag, GetExpirableCollectionTriggerThresholdFlag, SetExpirableCollectionTriggerThresholdFlag,
IsEnabledSkipSplitOnNoResultFlag, GetSkipSplitOnNoResultFlag, SetSkipSplitOnNoResultFlag,
IsEnabledForce32BitByteCodeFlag, GetForce32BitByteCodeFlag, SetForce32BitByteCodeFlag,

IsEnabledCollectGarbageFlag, GetCollectGarbageFlag, SetCollectGarbageFlag,

IsEnabledIntlFlag, GetIntlFlag, SetIntlFlag,
IsEnabledIntlBuiltInsFlag, GetIntlBuiltInsFlag, SetIntlBuiltInsFlag,
IsEnabledIntlPlatformFlag, GetIntlPlatformFlag, SetIntlPlatformFlag,

IsEnabledJsBuiltInFlag, GetJsBuiltInFlag, SetJsBuiltInFlag,
IsEnabledJitReproFlag, GetJitReproFlag, SetJitReproFlag,
IsEnabledEntryPointInfoRpcDataFlag, GetEntryPointInfoRpcDataFlag, SetEntryPointInfoRpcDataFlag,

IsEnabledLdChakraLibFlag, GetLdChakraLibFlag, SetLdChakraLibFlag,
IsEnabledTestChakraLibFlag, GetTestChakraLibFlag, SetTestChakraLibFlag,

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
IsEnabledES6Flag, GetES6Flag, SetES6Flag,

// Master ES6 flag to enable ALL sub ES6 features/flags
IsEnabledES6AllFlag, GetES6AllFlag, SetES6AllFlag,

// Master ES6 flag to enable Threshold ES6 features/flags
IsEnabledES6ExperimentalFlag, GetES6ExperimentalFlag, SetES6ExperimentalFlag,

// Per ES6 feature/flag

IsEnabledES7AsyncAwaitFlag, GetES7AsyncAwaitFlag, SetES7AsyncAwaitFlag,
IsEnabledES6DateParseFixFlag, GetES6DateParseFixFlag, SetES6DateParseFixFlag,
IsEnabledES6FunctionNameFullFlag, GetES6FunctionNameFullFlag, SetES6FunctionNameFullFlag,
IsEnabledES6GeneratorsFlag, GetES6GeneratorsFlag, SetES6GeneratorsFlag,
IsEnabledES7ExponentiationOperatorFlag, GetES7ExponentiationOperatorFlag, SetES7ExponentiationOperatorFlag,

IsEnabledES7ValuesEntriesFlag, GetES7ValuesEntriesFlag, SetES7ValuesEntriesFlag,
IsEnabledES7TrailingCommaFlag, GetES7TrailingCommaFlag, SetES7TrailingCommaFlag,
IsEnabledES6IsConcatSpreadableFlag, GetES6IsConcatSpreadableFlag, SetES6IsConcatSpreadableFlag,
IsEnabledES6MathFlag, GetES6MathFlag, SetES6MathFlag,

IsEnabledESDynamicImportFlag, GetESDynamicImportFlag, SetESDynamicImportFlag,

IsEnabledES6ModuleFlag, GetES6ModuleFlag, SetES6ModuleFlag,
IsEnabledES6ObjectFlag, GetES6ObjectFlag, SetES6ObjectFlag,
IsEnabledES6NumberFlag, GetES6NumberFlag, SetES6NumberFlag,
IsEnabledES6ObjectLiteralsFlag, GetES6ObjectLiteralsFlag, SetES6ObjectLiteralsFlag,
IsEnabledES6ProxyFlag, GetES6ProxyFlag, SetES6ProxyFlag,
IsEnabledES6RestFlag, GetES6RestFlag, SetES6RestFlag,
IsEnabledES6SpreadFlag, GetES6SpreadFlag, SetES6SpreadFlag,
IsEnabledES6StringFlag, GetES6StringFlag, SetES6StringFlag,
IsEnabledES6StringPrototypeFixesFlag, GetES6StringPrototypeFixesFlag, SetES6StringPrototypeFixesFlag,
IsEnabledES2018ObjectRestSpreadFlag, GetES2018ObjectRestSpreadFlag, SetES2018ObjectRestSpreadFlag,

IsEnabledES6PrototypeChainFlag, GetES6PrototypeChainFlag, SetES6PrototypeChainFlag,
IsEnabledES6ToPrimitiveFlag, GetES6ToPrimitiveFlag, SetES6ToPrimitiveFlag,
IsEnabledES6ToLengthFlag, GetES6ToLengthFlag, SetES6ToLengthFlag,
IsEnabledES6ToStringTagFlag, GetES6ToStringTagFlag, SetES6ToStringTagFlag,
IsEnabledES6UnicodeFlag, GetES6UnicodeFlag, SetES6UnicodeFlag,
IsEnabledES6UnicodeVerboseFlag, GetES6UnicodeVerboseFlag, SetES6UnicodeVerboseFlag,
IsEnabledES6UnscopablesFlag, GetES6UnscopablesFlag, SetES6UnscopablesFlag,
IsEnabledES6RegExStickyFlag, GetES6RegExStickyFlag, SetES6RegExStickyFlag,
IsEnabledES2018RegExDotAllFlag, GetES2018RegExDotAllFlag, SetES2018RegExDotAllFlag,
IsEnabledESExportNsAsFlag, GetESExportNsAsFlag, SetESExportNsAsFlag,
IsEnabledES2018AsyncIterationFlag, GetES2018AsyncIterationFlag, SetES2018AsyncIterationFlag,
IsEnabledESTopLevelAwaitFlag, GetESTopLevelAwaitFlag, SetESTopLevelAwaitFlag,
IsEnabledES6RegExPrototypePropertiesFlag, GetES6RegExPrototypePropertiesFlag, SetES6RegExPrototypePropertiesFlag,

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
IsEnabledES6RegExSymbolsFlag, GetES6RegExSymbolsFlag, SetES6RegExSymbolsFlag,

IsEnabledES6VerboseFlag, GetES6VerboseFlag, SetES6VerboseFlag,
IsEnabledESObjectGetOwnPropertyDescriptorsFlag, GetESObjectGetOwnPropertyDescriptorsFlag, SetESObjectGetOwnPropertyDescriptorsFlag,

IsEnabledESSharedArrayBufferFlag, GetESSharedArrayBufferFlag, SetESSharedArrayBufferFlag,

// Newer language feature flags

// ES BigInt flag
IsEnabledESBigIntFlag, GetESBigIntFlag, SetESBigIntFlag,

// ES Numeric Separator support for numeric constants
IsEnabledESNumericSeparatorFlag, GetESNumericSeparatorFlag, SetESNumericSeparatorFlag,

// ES Nullish coalescing operator support (??)
IsEnabledESNullishCoalescingOperatorFlag, GetESNullishCoalescingOperatorFlag, SetESNullishCoalescingOperatorFlag,

// ES Hashbang support for interpreter directive syntax
IsEnabledESHashbangFlag, GetESHashbangFlag, SetESHashbangFlag,

// ES Symbol.prototype.description flag
IsEnabledESSymbolDescriptionFlag, GetESSymbolDescriptionFlag, SetESSymbolDescriptionFlag,

IsEnabledESArrayFindFromLastFlag, GetESArrayFindFromLastFlag, SetESArrayFindFromLastFlag,

// ES Promise.any and AggregateError flag
IsEnabledESPromiseAnyFlag, GetESPromiseAnyFlag, SetESPromiseAnyFlag,

// ES import.meta keyword meta-property
IsEnabledESImportMetaFlag, GetESImportMetaFlag, SetESImportMetaFlag,

//ES globalThis flag
IsEnabledESGlobalThisFlag, GetESGlobalThisFlag, SetESGlobalThisFlag,

// This flag to be removed once JITing generator functions is stable
IsEnabledJitES6GeneratorsFlag, GetJitES6GeneratorsFlag, SetJitES6GeneratorsFlag,

IsEnabledFastLineColumnCalculationFlag, GetFastLineColumnCalculationFlag, SetFastLineColumnCalculationFlag,
IsEnabledFilenameFlag, GetFilenameFlag, SetFilenameFlag,
IsEnabledFreeRejittedCodeFlag, GetFreeRejittedCodeFlag, SetFreeRejittedCodeFlag,
IsEnabledForceGuardPagesFlag, GetForceGuardPagesFlag, SetForceGuardPagesFlag,
IsEnabledPrintGuardPageBoundsFlag, GetPrintGuardPageBoundsFlag, SetPrintGuardPageBoundsFlag,
IsEnabledForceLegacyEngineFlag, GetForceLegacyEngineFlag, SetForceLegacyEngineFlag,
IsEnabledForceArrayBTreeFlag, GetForceArrayBTreeFlag, SetForceArrayBTreeFlag,
IsEnabledStrongArraySortFlag, GetStrongArraySortFlag, SetStrongArraySortFlag,
IsEnabledForceCleanPropertyOnCollectFlag, GetForceCleanPropertyOnCollectFlag, SetForceCleanPropertyOnCollectFlag,
IsEnabledForceCleanCacheOnCollectFlag, GetForceCleanCacheOnCollectFlag, SetForceCleanCacheOnCollectFlag,
IsEnabledForceGCAfterJSONParseFlag, GetForceGCAfterJSONParseFlag, SetForceGCAfterJSONParseFlag,
IsEnabledForceDecommitOnCollectFlag, GetForceDecommitOnCollectFlag, SetForceDecommitOnCollectFlag,
IsEnabledForceDeferParseFlag, GetForceDeferParseFlag, SetForceDeferParseFlag,
IsEnabledForceDiagnosticsModeFlag, GetForceDiagnosticsModeFlag, SetForceDiagnosticsModeFlag,
IsEnabledForceGetWriteWatchOOMFlag, GetForceGetWriteWatchOOMFlag, SetForceGetWriteWatchOOMFlag,
IsEnabledForcePostLowerGlobOptInstrStringFlag, GetForcePostLowerGlobOptInstrStringFlag, SetForcePostLowerGlobOptInstrStringFlag,
IsEnabledForceSplitScopeFlag, GetForceSplitScopeFlag, SetForceSplitScopeFlag,
IsEnabledEnumerateSpecialPropertiesInDebuggerFlag, GetEnumerateSpecialPropertiesInDebuggerFlag, SetEnumerateSpecialPropertiesInDebuggerFlag,
IsEnabledEnableContinueAfterExceptionWrappersForHelpersFlag, GetEnableContinueAfterExceptionWrappersForHelpersFlag, SetEnableContinueAfterExceptionWrappersForHelpersFlag,
IsEnabledEnableContinueAfterExceptionWrappersForBuiltInsFlag, GetEnableContinueAfterExceptionWrappersForBuiltInsFlag, SetEnableContinueAfterExceptionWrappersForBuiltInsFlag,
IsEnabledEnableFunctionSourceReportForHeapEnumFlag, GetEnableFunctionSourceReportForHeapEnumFlag, SetEnableFunctionSourceReportForHeapEnumFlag,
IsEnabledForceFragmentAddressSpaceFlag, GetForceFragmentAddressSpaceFlag, SetForceFragmentAddressSpaceFlag,
IsEnabledForceOOMOnEBCommitFlag, GetForceOOMOnEBCommitFlag, SetForceOOMOnEBCommitFlag,
IsEnabledForceDynamicProfileFlag, GetForceDynamicProfileFlag, SetForceDynamicProfileFlag,
IsEnabledForceES5ArrayFlag, GetForceES5ArrayFlag, SetForceES5ArrayFlag,
IsEnabledForceAsmJsLinkFailFlag, GetForceAsmJsLinkFailFlag, SetForceAsmJsLinkFailFlag,
IsEnabledForceExpireOnNonCacheCollectFlag, GetForceExpireOnNonCacheCollectFlag, SetForceExpireOnNonCacheCollectFlag,
IsEnabledForceFastPathFlag, GetForceFastPathFlag, SetForceFastPathFlag,
IsEnabledForceFloatPrefFlag, GetForceFloatPrefFlag, SetForceFloatPrefFlag,
IsEnabledForceJITLoopBodyFlag, GetForceJITLoopBodyFlag, SetForceJITLoopBodyFlag,
IsEnabledForceStaticInterpreterThunkFlag, GetForceStaticInterpreterThunkFlag, SetForceStaticInterpreterThunkFlag,
IsEnabledDumpCommentsFromReferencedFilesFlag, GetDumpCommentsFromReferencedFilesFlag, SetDumpCommentsFromReferencedFilesFlag,
IsEnabledDelayFullJITSmallFuncFlag, GetDelayFullJITSmallFuncFlag, SetDelayFullJITSmallFuncFlag,
IsEnabledEnableFatalErrorOnOOMFlag, GetEnableFatalErrorOnOOMFlag, SetEnableFatalErrorOnOOMFlag,

#if defined(_M_ARM32_OR_ARM64)
IsEnabledForceLocalsPtrFlag, GetForceLocalsPtrFlag, SetForceLocalsPtrFlag,
#endif
IsEnabledDeferLoadingAvailableSourceFlag, GetDeferLoadingAvailableSourceFlag, SetDeferLoadingAvailableSourceFlag,
IsEnabledForceNativeFlag, GetForceNativeFlag, SetForceNativeFlag,
IsEnabledForceSerializedFlag, GetForceSerializedFlag, SetForceSerializedFlag,
IsEnabledForceSerializedBytecodeMajorVersionFlag, GetForceSerializedBytecodeMajorVersionFlag, SetForceSerializedBytecodeMajorVersionFlag,
IsEnabledForceSerializedBytecodeVersionSchemaFlag, GetForceSerializedBytecodeVersionSchemaFlag, SetForceSerializedBytecodeVersionSchemaFlag,
IsEnabledForceStrictModeFlag, GetForceStrictModeFlag, SetForceStrictModeFlag,
IsEnabledForceUndoDeferFlag, GetForceUndoDeferFlag, SetForceUndoDeferFlag,
IsEnabledForceBlockingConcurrentCollectFlag, GetForceBlockingConcurrentCollectFlag, SetForceBlockingConcurrentCollectFlag,
IsEnabledFreTestDiagModeFlag, GetFreTestDiagModeFlag, SetFreTestDiagModeFlag,
#ifdef BYTECODE_TESTING
IsEnabledByteCodeBranchLimitFlag, GetByteCodeBranchLimitFlag, SetByteCodeBranchLimitFlag,
IsEnabledMediumByteCodeLayoutFlag, GetMediumByteCodeLayoutFlag, SetMediumByteCodeLayoutFlag,
IsEnabledLargeByteCodeLayoutFlag, GetLargeByteCodeLayoutFlag, SetLargeByteCodeLayoutFlag,
#endif
IsEnabledInduceCodeGenFailureFlag, GetInduceCodeGenFailureFlag, SetInduceCodeGenFailureFlag,
IsEnabledInduceCodeGenFailureSeedFlag, GetInduceCodeGenFailureSeedFlag, SetInduceCodeGenFailureSeedFlag,
IsEnabledInjectPartiallyInitializedInterpreterFrameErrorFlag, GetInjectPartiallyInitializedInterpreterFrameErrorFlag, SetInjectPartiallyInitializedInterpreterFrameErrorFlag,
IsEnabledInjectPartiallyInitializedInterpreterFrameErrorTypeFlag, GetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag, SetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag,
IsEnabledGenerateByteCodeBufferReturnsCantGenerateFlag, GetGenerateByteCodeBufferReturnsCantGenerateFlag, SetGenerateByteCodeBufferReturnsCantGenerateFlag,
IsEnabledGoptCleanupThresholdFlag, GetGoptCleanupThresholdFlag, SetGoptCleanupThresholdFlag,
IsEnabledAsmGoptCleanupThresholdFlag, GetAsmGoptCleanupThresholdFlag, SetAsmGoptCleanupThresholdFlag,
IsEnabledHighPrecisionDateFlag, GetHighPrecisionDateFlag, SetHighPrecisionDateFlag,
IsEnabledInlineCountMaxFlag, GetInlineCountMaxFlag, SetInlineCountMaxFlag,
IsEnabledInlineCountMaxInLoopBodiesFlag, GetInlineCountMaxInLoopBodiesFlag, SetInlineCountMaxInLoopBodiesFlag, IsEnabledicminlbFlag, GeticminlbFlag, SeticminlbFlag,
IsEnabledInlineInLoopBodyScaleDownFactorFlag, GetInlineInLoopBodyScaleDownFactorFlag, SetInlineInLoopBodyScaleDownFactorFlag, IsEnablediilbsdfFlag, GetiilbsdfFlag, SetiilbsdfFlag,
IsEnabledInlineThresholdFlag, GetInlineThresholdFlag, SetInlineThresholdFlag,
IsEnabledAggressiveInlineCountMaxFlag, GetAggressiveInlineCountMaxFlag, SetAggressiveInlineCountMaxFlag,
IsEnabledAggressiveInlineThresholdFlag, GetAggressiveInlineThresholdFlag, SetAggressiveInlineThresholdFlag,
IsEnabledInlineThresholdAdjustCountInLargeFunctionFlag, GetInlineThresholdAdjustCountInLargeFunctionFlag, SetInlineThresholdAdjustCountInLargeFunctionFlag,
IsEnabledInlineThresholdAdjustCountInMediumSizedFunctionFlag, GetInlineThresholdAdjustCountInMediumSizedFunctionFlag, SetInlineThresholdAdjustCountInMediumSizedFunctionFlag,
IsEnabledInlineThresholdAdjustCountInSmallFunctionFlag, GetInlineThresholdAdjustCountInSmallFunctionFlag, SetInlineThresholdAdjustCountInSmallFunctionFlag,
IsEnabledAsmJsInlineAdjustFlag, GetAsmJsInlineAdjustFlag, SetAsmJsInlineAdjustFlag,
IsEnabledInterpretFlag, GetInterpretFlag, SetInterpretFlag,
IsEnabledJitQueueThresholdFlag, GetJitQueueThresholdFlag, SetJitQueueThresholdFlag,
#ifdef LEAK_REPORT
IsEnabledLeakReportFlag, GetLeakReportFlag, SetLeakReportFlag,
#endif
IsEnabledLoopInlineThresholdFlag, GetLoopInlineThresholdFlag, SetLoopInlineThresholdFlag,
IsEnabledLeafInlineThresholdFlag, GetLeafInlineThresholdFlag, SetLeafInlineThresholdFlag,
IsEnabledConstantArgumentInlineThresholdFlag, GetConstantArgumentInlineThresholdFlag, SetConstantArgumentInlineThresholdFlag,
IsEnabledRecursiveInlineThresholdFlag, GetRecursiveInlineThresholdFlag, SetRecursiveInlineThresholdFlag,
IsEnabledRecursiveInlineDepthMaxFlag, GetRecursiveInlineDepthMaxFlag, SetRecursiveInlineDepthMaxFlag,
IsEnabledRecursiveInlineDepthMinFlag, GetRecursiveInlineDepthMinFlag, SetRecursiveInlineDepthMinFlag,
IsEnabledRedeferralCapFlag, GetRedeferralCapFlag, SetRedeferralCapFlag,
IsEnabledLoopFlag, GetLoopFlag, SetLoopFlag,
IsEnabledLoopInterpretCountFlag, GetLoopInterpretCountFlag, SetLoopInterpretCountFlag, IsEnabledlicFlag, GetlicFlag, SetlicFlag,
IsEnabledLoopProfileIterationsFlag, GetLoopProfileIterationsFlag, SetLoopProfileIterationsFlag,
IsEnabledOutsideLoopInlineThresholdFlag, GetOutsideLoopInlineThresholdFlag, SetOutsideLoopInlineThresholdFlag,
IsEnabledMaxFuncInlineDepthFlag, GetMaxFuncInlineDepthFlag, SetMaxFuncInlineDepthFlag,
IsEnabledMaxNumberOfInlineesWithLoopFlag, GetMaxNumberOfInlineesWithLoopFlag, SetMaxNumberOfInlineesWithLoopFlag,
IsEnabledPolymorphicInlineThresholdFlag, GetPolymorphicInlineThresholdFlag, SetPolymorphicInlineThresholdFlag,
IsEnabledPrimeRecyclerFlag, GetPrimeRecyclerFlag, SetPrimeRecyclerFlag,
IsEnabledTraceEngineRefcountFlag, GetTraceEngineRefcountFlag, SetTraceEngineRefcountFlag,
#if defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
IsEnabledLeakStackTraceFlag, GetLeakStackTraceFlag, SetLeakStackTraceFlag,
IsEnabledForceMemoryLeakFlag, GetForceMemoryLeakFlag, SetForceMemoryLeakFlag,
#endif
IsEnabledDumpAfterFinalGCFlag, GetDumpAfterFinalGCFlag, SetDumpAfterFinalGCFlag,
IsEnabledForceOldDateAPIFlag, GetForceOldDateAPIFlag, SetForceOldDateAPIFlag,

IsEnabledJitLoopBodyHotLoopThresholdFlag, GetJitLoopBodyHotLoopThresholdFlag, SetJitLoopBodyHotLoopThresholdFlag,
IsEnabledLoopBodySizeThresholdToDisableOptsFlag, GetLoopBodySizeThresholdToDisableOptsFlag, SetLoopBodySizeThresholdToDisableOptsFlag,

IsEnabledMaxJitThreadCountFlag, GetMaxJitThreadCountFlag, SetMaxJitThreadCountFlag,
IsEnabledForceMaxJitThreadCountFlag, GetForceMaxJitThreadCountFlag, SetForceMaxJitThreadCountFlag,

IsEnabledMitigateSpectreFlag, GetMitigateSpectreFlag, SetMitigateSpectreFlag,

IsEnabledAddMaskingBlocksFlag, GetAddMaskingBlocksFlag, SetAddMaskingBlocksFlag,

IsEnabledPoisonVarArrayLoadFlag, GetPoisonVarArrayLoadFlag, SetPoisonVarArrayLoadFlag,
IsEnabledPoisonIntArrayLoadFlag, GetPoisonIntArrayLoadFlag, SetPoisonIntArrayLoadFlag,
IsEnabledPoisonFloatArrayLoadFlag, GetPoisonFloatArrayLoadFlag, SetPoisonFloatArrayLoadFlag,
IsEnabledPoisonTypedArrayLoadFlag, GetPoisonTypedArrayLoadFlag, SetPoisonTypedArrayLoadFlag,
IsEnabledPoisonStringLoadFlag, GetPoisonStringLoadFlag, SetPoisonStringLoadFlag,
IsEnabledPoisonObjectsForLoadsFlag, GetPoisonObjectsForLoadsFlag, SetPoisonObjectsForLoadsFlag,

IsEnabledPoisonVarArrayStoreFlag, GetPoisonVarArrayStoreFlag, SetPoisonVarArrayStoreFlag,
IsEnabledPoisonIntArrayStoreFlag, GetPoisonIntArrayStoreFlag, SetPoisonIntArrayStoreFlag,
IsEnabledPoisonFloatArrayStoreFlag, GetPoisonFloatArrayStoreFlag, SetPoisonFloatArrayStoreFlag,
IsEnabledPoisonTypedArrayStoreFlag, GetPoisonTypedArrayStoreFlag, SetPoisonTypedArrayStoreFlag,
IsEnabledPoisonStringStoreFlag, GetPoisonStringStoreFlag, SetPoisonStringStoreFlag,
IsEnabledPoisonObjectsForStoresFlag, GetPoisonObjectsForStoresFlag, SetPoisonObjectsForStoresFlag,

IsEnabledMinInterpretCountFlag, GetMinInterpretCountFlag, SetMinInterpretCountFlag,
IsEnabledMinSimpleJitRunCountFlag, GetMinSimpleJitRunCountFlag, SetMinSimpleJitRunCountFlag,
IsEnabledMaxInterpretCountFlag, GetMaxInterpretCountFlag, SetMaxInterpretCountFlag, IsEnabledMicFlag, GetMicFlag, SetMicFlag,
IsEnabledMaxSimpleJitRunCountFlag, GetMaxSimpleJitRunCountFlag, SetMaxSimpleJitRunCountFlag, IsEnabledMsjrcFlag, GetMsjrcFlag, SetMsjrcFlag,
IsEnabledMinMemOpCountFlag, GetMinMemOpCountFlag, SetMinMemOpCountFlag, IsEnabledMmocFlag, GetMmocFlag, SetMmocFlag,

#if ENABLE_COPYONACCESS_ARRAY
IsEnabledMaxCopyOnAccessArrayLengthFlag, GetMaxCopyOnAccessArrayLengthFlag, SetMaxCopyOnAccessArrayLengthFlag,
IsEnabledMinCopyOnAccessArrayLengthFlag, GetMinCopyOnAccessArrayLengthFlag, SetMinCopyOnAccessArrayLengthFlag,
IsEnabledCopyOnAccessArraySegmentCacheSizeFlag, GetCopyOnAccessArraySegmentCacheSizeFlag, SetCopyOnAccessArraySegmentCacheSizeFlag,
#endif

IsEnabledMinTemplatizedJitRunCountFlag, GetMinTemplatizedJitRunCountFlag, SetMinTemplatizedJitRunCountFlag,
IsEnabledMinAsmJsInterpreterRunCountFlag, GetMinAsmJsInterpreterRunCountFlag, SetMinAsmJsInterpreterRunCountFlag,

IsEnabledMinTemplatizedJitLoopRunCountFlag, GetMinTemplatizedJitLoopRunCountFlag, SetMinTemplatizedJitLoopRunCountFlag,
IsEnabledMaxTemplatizedJitRunCountFlag, GetMaxTemplatizedJitRunCountFlag, SetMaxTemplatizedJitRunCountFlag, IsEnabledMtjrcFlag, GetMtjrcFlag, SetMtjrcFlag,
IsEnabledMaxAsmJsInterpreterRunCountFlag, GetMaxAsmJsInterpreterRunCountFlag, SetMaxAsmJsInterpreterRunCountFlag, IsEnabledMaicFlag, GetMaicFlag, SetMaicFlag,

IsEnabledAutoProfilingInterpreter0LimitFlag, GetAutoProfilingInterpreter0LimitFlag, SetAutoProfilingInterpreter0LimitFlag,
IsEnabledProfilingInterpreter0LimitFlag, GetProfilingInterpreter0LimitFlag, SetProfilingInterpreter0LimitFlag,
IsEnabledAutoProfilingInterpreter1LimitFlag, GetAutoProfilingInterpreter1LimitFlag, SetAutoProfilingInterpreter1LimitFlag,
IsEnabledSimpleJitLimitFlag, GetSimpleJitLimitFlag, SetSimpleJitLimitFlag,
IsEnabledProfilingInterpreter1LimitFlag, GetProfilingInterpreter1LimitFlag, SetProfilingInterpreter1LimitFlag,

IsEnabledExecutionModeLimitsFlag, GetExecutionModeLimitsFlag, SetExecutionModeLimitsFlag, IsEnabledEmlFlag, GetEmlFlag, SetEmlFlag,
IsEnabledEnforceExecutionModeLimitsFlag, GetEnforceExecutionModeLimitsFlag, SetEnforceExecutionModeLimitsFlag, IsEnabledEemlFlag, GetEemlFlag, SetEemlFlag,

IsEnabledSimpleJitAfterFlag, GetSimpleJitAfterFlag, SetSimpleJitAfterFlag, IsEnabledSjaFlag, GetSjaFlag, SetSjaFlag,
IsEnabledFullJitAfterFlag, GetFullJitAfterFlag, SetFullJitAfterFlag, IsEnabledFjaFlag, GetFjaFlag, SetFjaFlag,

IsEnabledNewSimpleJitFlag, GetNewSimpleJitFlag, SetNewSimpleJitFlag,

IsEnabledMaxLinearIntCaseCountFlag, GetMaxLinearIntCaseCountFlag, SetMaxLinearIntCaseCountFlag,
IsEnabledMaxSingleCharStrJumpTableSizeFlag, GetMaxSingleCharStrJumpTableSizeFlag, SetMaxSingleCharStrJumpTableSizeFlag,
IsEnabledMaxSingleCharStrJumpTableRatioFlag, GetMaxSingleCharStrJumpTableRatioFlag, SetMaxSingleCharStrJumpTableRatioFlag,
IsEnabledMinSwitchJumpTableSizeFlag, GetMinSwitchJumpTableSizeFlag, SetMinSwitchJumpTableSizeFlag,
IsEnabledMaxLinearStringCaseCountFlag, GetMaxLinearStringCaseCountFlag, SetMaxLinearStringCaseCountFlag,
IsEnabledMinDeferredFuncTokenCountFlag, GetMinDeferredFuncTokenCountFlag, SetMinDeferredFuncTokenCountFlag,
#if DBG
IsEnabledSkipFuncCountForBailOnNoProfileFlag, GetSkipFuncCountForBailOnNoProfileFlag, SetSkipFuncCountForBailOnNoProfileFlag,
#endif
IsEnabledMaxJITFunctionBytecodeByteLengthFlag, GetMaxJITFunctionBytecodeByteLengthFlag, SetMaxJITFunctionBytecodeByteLengthFlag,
IsEnabledMaxJITFunctionBytecodeCountFlag, GetMaxJITFunctionBytecodeCountFlag, SetMaxJITFunctionBytecodeCountFlag,
IsEnabledMaxLoopsPerFunctionFlag, GetMaxLoopsPerFunctionFlag, SetMaxLoopsPerFunctionFlag,
IsEnabledFuncObjectInlineCacheThresholdFlag, GetFuncObjectInlineCacheThresholdFlag, SetFuncObjectInlineCacheThresholdFlag,
IsEnabledNoDeferParseFlag, GetNoDeferParseFlag, SetNoDeferParseFlag,
IsEnabledNoLogoFlag, GetNoLogoFlag, SetNoLogoFlag,
IsEnabledOOPJITMissingOptsFlag, GetOOPJITMissingOptsFlag, SetOOPJITMissingOptsFlag,
IsEnabledCrashOnOOPJITFailureFlag, GetCrashOnOOPJITFailureFlag, SetCrashOnOOPJITFailureFlag,
IsEnabledOOPCFGRegistrationFlag, GetOOPCFGRegistrationFlag, SetOOPCFGRegistrationFlag,
IsEnabledForceJITCFGCheckFlag, GetForceJITCFGCheckFlag, SetForceJITCFGCheckFlag,
IsEnabledUseJITTrampolineFlag, GetUseJITTrampolineFlag, SetUseJITTrampolineFlag,
IsEnabledNoNativeFlag, GetNoNativeFlag, SetNoNativeFlag,
IsEnabledNopFrequencyFlag, GetNopFrequencyFlag, SetNopFrequencyFlag,
IsEnabledNoStrictModeFlag, GetNoStrictModeFlag, SetNoStrictModeFlag,
IsEnabledNormalizeStatsFlag, GetNormalizeStatsFlag, SetNormalizeStatsFlag,
IsEnabledOutputFileFlag, GetOutputFileFlag, SetOutputFileFlag,
IsEnabledOutputFileOpenModeFlag, GetOutputFileOpenModeFlag, SetOutputFileOpenModeFlag,
#ifdef ENABLE_TRACE
IsEnabledInMemoryTraceFlag, GetInMemoryTraceFlag, SetInMemoryTraceFlag,
IsEnabledInMemoryTraceBufferSizeFlag, GetInMemoryTraceBufferSizeFlag, SetInMemoryTraceBufferSizeFlag,
#ifdef STACK_BACK_TRACE
IsEnabledTraceWithStackFlag, GetTraceWithStackFlag, SetTraceWithStackFlag,
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
IsEnabledPrintRunTimeDataCollectionTraceFlag, GetPrintRunTimeDataCollectionTraceFlag, SetPrintRunTimeDataCollectionTraceFlag,
#ifdef ENABLE_PREJIT
IsEnabledPrejitFlag, GetPrejitFlag, SetPrejitFlag,
#endif
IsEnabledPrintSrcInDumpFlag, GetPrintSrcInDumpFlag, SetPrintSrcInDumpFlag,
#if PROFILE_DICTIONARY
IsEnabledProfileDictionaryFlag, GetProfileDictionaryFlag, SetProfileDictionaryFlag,
#endif
#ifdef PROFILE_EXEC
IsEnabledProfileThresholdFlag, GetProfileThresholdFlag, SetProfileThresholdFlag,
#endif
#ifdef PROFILE_OBJECT_LITERALS
IsEnabledProfileObjectLiteralFlag, GetProfileObjectLiteralFlag, SetProfileObjectLiteralFlag,
#endif
#ifdef PROFILE_MEM
IsEnabledProfileMemoryFlag, GetProfileMemoryFlag, SetProfileMemoryFlag,
#endif
#ifdef PROFILE_STRINGS
IsEnabledProfileStringsFlag, GetProfileStringsFlag, SetProfileStringsFlag,
#endif
#ifdef PROFILE_TYPES
IsEnabledProfileTypesFlag, GetProfileTypesFlag, SetProfileTypesFlag,
#endif
#ifdef PROFILE_EVALMAP
IsEnabledProfileEvalMapFlag, GetProfileEvalMapFlag, SetProfileEvalMapFlag,
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
IsEnabledProfileBailOutRecordMemoryFlag, GetProfileBailOutRecordMemoryFlag, SetProfileBailOutRecordMemoryFlag,
#endif

#if DBG
IsEnabledValidateIntRangesFlag, GetValidateIntRangesFlag, SetValidateIntRangesFlag,
#endif
IsEnabledRejitMaxBailOutCountFlag, GetRejitMaxBailOutCountFlag, SetRejitMaxBailOutCountFlag,
IsEnabledCallsToBailoutsRatioForRejitFlag, GetCallsToBailoutsRatioForRejitFlag, SetCallsToBailoutsRatioForRejitFlag,
IsEnabledLoopIterationsToBailoutsRatioForRejitFlag, GetLoopIterationsToBailoutsRatioForRejitFlag, SetLoopIterationsToBailoutsRatioForRejitFlag,
IsEnabledMinBailOutsBeforeRejitFlag, GetMinBailOutsBeforeRejitFlag, SetMinBailOutsBeforeRejitFlag,
IsEnabledMinBailOutsBeforeRejitForLoopsFlag, GetMinBailOutsBeforeRejitForLoopsFlag, SetMinBailOutsBeforeRejitForLoopsFlag,
IsEnabledLibraryStackFrameFlag, GetLibraryStackFrameFlag, SetLibraryStackFrameFlag,
IsEnabledLibraryStackFrameDebuggerFlag, GetLibraryStackFrameDebuggerFlag, SetLibraryStackFrameDebuggerFlag,
#ifdef RECYCLER_STRESS
IsEnabledRecyclerStressFlag, GetRecyclerStressFlag, SetRecyclerStressFlag,
#if ENABLE_CONCURRENT_GC
IsEnabledRecyclerBackgroundStressFlag, GetRecyclerBackgroundStressFlag, SetRecyclerBackgroundStressFlag,
IsEnabledRecyclerConcurrentStressFlag, GetRecyclerConcurrentStressFlag, SetRecyclerConcurrentStressFlag,
IsEnabledRecyclerConcurrentRepeatStressFlag, GetRecyclerConcurrentRepeatStressFlag, SetRecyclerConcurrentRepeatStressFlag,
#endif
#if ENABLE_PARTIAL_GC
IsEnabledRecyclerPartialStressFlag, GetRecyclerPartialStressFlag, SetRecyclerPartialStressFlag,
#endif
IsEnabledRecyclerTrackStressFlag, GetRecyclerTrackStressFlag, SetRecyclerTrackStressFlag,
IsEnabledRecyclerInduceFalsePositivesFlag, GetRecyclerInduceFalsePositivesFlag, SetRecyclerInduceFalsePositivesFlag,
#endif // RECYCLER_STRESS
IsEnabledRecyclerForceMarkInteriorFlag, GetRecyclerForceMarkInteriorFlag, SetRecyclerForceMarkInteriorFlag,
#if ENABLE_CONCURRENT_GC
IsEnabledRecyclerPriorityBoostTimeoutFlag, GetRecyclerPriorityBoostTimeoutFlag, SetRecyclerPriorityBoostTimeoutFlag,
IsEnabledRecyclerThreadCollectTimeoutFlag, GetRecyclerThreadCollectTimeoutFlag, SetRecyclerThreadCollectTimeoutFlag,
IsEnabledEnableConcurrentSweepAllocFlag, GetEnableConcurrentSweepAllocFlag, SetEnableConcurrentSweepAllocFlag, IsEnabledecsaFlag, GetecsaFlag, SetecsaFlag,
#endif
#ifdef RECYCLER_PAGE_HEAP
IsEnabledPageHeapFlag, GetPageHeapFlag, SetPageHeapFlag,
IsEnabledPageHeapAllocStackFlag, GetPageHeapAllocStackFlag, SetPageHeapAllocStackFlag,
IsEnabledPageHeapFreeStackFlag, GetPageHeapFreeStackFlag, SetPageHeapFreeStackFlag,

IsEnabledPageHeapBlockTypeFlag, GetPageHeapBlockTypeFlag, SetPageHeapBlockTypeFlag,
IsEnabledPageHeapDecommitGuardPageFlag, GetPageHeapDecommitGuardPageFlag, SetPageHeapDecommitGuardPageFlag,
#endif
#ifdef RECYCLER_NO_PAGE_REUSE
IsEnabledRecyclerNoPageReuseFlag, GetRecyclerNoPageReuseFlag, SetRecyclerNoPageReuseFlag,
#endif
#ifdef RECYCLER_MEMORY_VERIFY
IsEnabledRecyclerVerifyPadSizeFlag, GetRecyclerVerifyPadSizeFlag, SetRecyclerVerifyPadSizeFlag,
#endif
IsEnabledRecyclerTestFlag, GetRecyclerTestFlag, SetRecyclerTestFlag,
IsEnabledRecyclerProtectPagesOnRescanFlag, GetRecyclerProtectPagesOnRescanFlag, SetRecyclerProtectPagesOnRescanFlag,
#ifdef RECYCLER_VERIFY_MARK
IsEnabledRecyclerVerifyMarkFlag, GetRecyclerVerifyMarkFlag, SetRecyclerVerifyMarkFlag,
#endif
IsEnabledLowMemoryCapFlag, GetLowMemoryCapFlag, SetLowMemoryCapFlag,
IsEnabledNewPagesCapDuringBGSweepingFlag, GetNewPagesCapDuringBGSweepingFlag, SetNewPagesCapDuringBGSweepingFlag,
IsEnabledAllocPolicyLimitFlag, GetAllocPolicyLimitFlag, SetAllocPolicyLimitFlag,
#ifdef RUNTIME_DATA_COLLECTION
IsEnabledRuntimeDataOutputFileFlag, GetRuntimeDataOutputFileFlag, SetRuntimeDataOutputFileFlag,
#endif
IsEnabledSpeculationCapFlag, GetSpeculationCapFlag, SetSpeculationCapFlag,
#if EXCEPTION_RECOVERY
IsEnabledSwallowExceptionsFlag, GetSwallowExceptionsFlag, SetSwallowExceptionsFlag,
#endif
IsEnabledPrintSystemExceptionFlag, GetPrintSystemExceptionFlag, SetPrintSystemExceptionFlag,
IsEnabledSwitchOptHolesThresholdFlag, GetSwitchOptHolesThresholdFlag, SetSwitchOptHolesThresholdFlag,
IsEnabledTempMinFlag, GetTempMinFlag, SetTempMinFlag,
IsEnabledTempMaxFlag, GetTempMaxFlag, SetTempMaxFlag,

#if defined(_M_X64)
IsEnabledLoopAlignNopLimitFlag, GetLoopAlignNopLimitFlag, SetLoopAlignNopLimitFlag,
#endif

#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
IsEnabledTraceMetaDataParsingFlag, GetTraceMetaDataParsingFlag, SetTraceMetaDataParsingFlag,
IsEnabledTraceWin8AllocationsFlag, GetTraceWin8AllocationsFlag, SetTraceWin8AllocationsFlag,
IsEnabledTraceWin8DeallocationsImmediateFlag, GetTraceWin8DeallocationsImmediateFlag, SetTraceWin8DeallocationsImmediateFlag,
IsEnabledPrintWin8StatsDetailedFlag, GetPrintWin8StatsDetailedFlag, SetPrintWin8StatsDetailedFlag,
IsEnabledTraceProtectPagesFlag, GetTraceProtectPagesFlag, SetTraceProtectPagesFlag,
#endif
IsEnabledTraceAsyncDebugCallsFlag, GetTraceAsyncDebugCallsFlag, SetTraceAsyncDebugCallsFlag,
#ifdef TRACK_DISPATCH
IsEnabledTrackDispatchFlag, GetTrackDispatchFlag, SetTrackDispatchFlag,
#endif
IsEnabledVerboseFlag, GetVerboseFlag, SetVerboseFlag,
IsEnabledUseFullNameFlag, GetUseFullNameFlag, SetUseFullNameFlag,
IsEnabledUtf8Flag, GetUtf8Flag, SetUtf8Flag,
IsEnabledVersionFlag, GetVersionFlag, SetVersionFlag,
IsEnabledWERExceptionSupportFlag, GetWERExceptionSupportFlag, SetWERExceptionSupportFlag,
IsEnabledExtendedErrorStackForTestHostFlag, GetExtendedErrorStackForTestHostFlag, SetExtendedErrorStackForTestHostFlag,
IsEnablederrorStackTraceFlag, GeterrorStackTraceFlag, SeterrorStackTraceFlag,
IsEnabledDoHeapEnumOnEngineShutdownFlag, GetDoHeapEnumOnEngineShutdownFlag, SetDoHeapEnumOnEngineShutdownFlag,
#ifdef HEAP_ENUMERATION_VALIDATION
IsEnabledValidateHeapEnumFlag, GetValidateHeapEnumFlag, SetValidateHeapEnumFlag,
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
IsEnabledRegexTracingFlag, GetRegexTracingFlag, SetRegexTracingFlag,
IsEnabledRegexProfileFlag, GetRegexProfileFlag, SetRegexProfileFlag,
IsEnabledRegexDebugFlag, GetRegexDebugFlag, SetRegexDebugFlag,
IsEnabledRegexDebugASTFlag, GetRegexDebugASTFlag, SetRegexDebugASTFlag,
IsEnabledRegexDebugAnnotatedASTFlag, GetRegexDebugAnnotatedASTFlag, SetRegexDebugAnnotatedASTFlag,
IsEnabledRegexBytecodeDebugFlag, GetRegexBytecodeDebugFlag, SetRegexBytecodeDebugFlag,
IsEnabledRegexOptimizeFlag, GetRegexOptimizeFlag, SetRegexOptimizeFlag,
IsEnabledDynamicRegexMruListSizeFlag, GetDynamicRegexMruListSizeFlag, SetDynamicRegexMruListSizeFlag,
#endif

IsEnabledOptimizeForManyInstancesFlag, GetOptimizeForManyInstancesFlag, SetOptimizeForManyInstancesFlag,
IsEnabledEnableArrayTypeMutationFlag, GetEnableArrayTypeMutationFlag, SetEnableArrayTypeMutationFlag,
IsEnabledArrayMutationTestSeedFlag, GetArrayMutationTestSeedFlag, SetArrayMutationTestSeedFlag,
IsEnabledEnableEvalMapCleanupFlag, GetEnableEvalMapCleanupFlag, SetEnableEvalMapCleanupFlag,
#ifdef PROFILE_MEM
IsEnabledTraceObjectAllocationFlag, GetTraceObjectAllocationFlag, SetTraceObjectAllocationFlag,
#endif
IsEnabledSseFlag, GetSseFlag, SetSseFlag,
IsEnabledDeletedPropertyReuseThresholdFlag, GetDeletedPropertyReuseThresholdFlag, SetDeletedPropertyReuseThresholdFlag,
IsEnabledForceStringKeyedSimpleDictionaryTypeHandlerFlag, GetForceStringKeyedSimpleDictionaryTypeHandlerFlag, SetForceStringKeyedSimpleDictionaryTypeHandlerFlag,
IsEnabledBigDictionaryTypeHandlerThresholdFlag, GetBigDictionaryTypeHandlerThresholdFlag, SetBigDictionaryTypeHandlerThresholdFlag,
IsEnabledTypeSnapshotEnumerationFlag, GetTypeSnapshotEnumerationFlag, SetTypeSnapshotEnumerationFlag,
IsEnabledIsolatePrototypesFlag, GetIsolatePrototypesFlag, SetIsolatePrototypesFlag,
IsEnabledChangeTypeOnProtoFlag, GetChangeTypeOnProtoFlag, SetChangeTypeOnProtoFlag,
IsEnabledShareInlineCachesFlag, GetShareInlineCachesFlag, SetShareInlineCachesFlag,
IsEnabledDisableDebugObjectFlag, GetDisableDebugObjectFlag, SetDisableDebugObjectFlag,
IsEnabledDumpHeapFlag, GetDumpHeapFlag, SetDumpHeapFlag,
IsEnabledautoProxyFlag, GetautoProxyFlag, SetautoProxyFlag,
IsEnabledPerfHintLevelFlag, GetPerfHintLevelFlag, SetPerfHintLevelFlag,
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
IsEnabledMemProtectHeapFlag, GetMemProtectHeapFlag, SetMemProtectHeapFlag,
#endif
#ifdef RECYCLER_STRESS
IsEnabledMemProtectHeapStressFlag, GetMemProtectHeapStressFlag, SetMemProtectHeapStressFlag,
#if ENABLE_CONCURRENT_GC
IsEnabledMemProtectHeapBackgroundStressFlag, GetMemProtectHeapBackgroundStressFlag, SetMemProtectHeapBackgroundStressFlag,
IsEnabledMemProtectHeapConcurrentStressFlag, GetMemProtectHeapConcurrentStressFlag, SetMemProtectHeapConcurrentStressFlag,
IsEnabledMemProtectHeapConcurrentRepeatStressFlag, GetMemProtectHeapConcurrentRepeatStressFlag, SetMemProtectHeapConcurrentRepeatStressFlag,
#endif
#if ENABLE_PARTIAL_GC
IsEnabledMemProtectHeapPartialStressFlag, GetMemProtectHeapPartialStressFlag, SetMemProtectHeapPartialStressFlag,
#endif
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
IsEnabledFixPropsOnPathTypesFlag, GetFixPropsOnPathTypesFlag, SetFixPropsOnPathTypesFlag,
#endif



// recycler heuristic flags
IsEnabledMaxBackgroundFinishMarkCountFlag, GetMaxBackgroundFinishMarkCountFlag, SetMaxBackgroundFinishMarkCountFlag,
IsEnabledBackgroundFinishMarkWaitTimeFlag, GetBackgroundFinishMarkWaitTimeFlag, SetBackgroundFinishMarkWaitTimeFlag,
IsEnabledMinBackgroundRepeatMarkRescanBytesFlag, GetMinBackgroundRepeatMarkRescanBytesFlag, SetMinBackgroundRepeatMarkRescanBytesFlag,

#if defined(_M_X64)
IsEnabledZeroMemoryWithNonTemporalStoreFlag, GetZeroMemoryWithNonTemporalStoreFlag, SetZeroMemoryWithNonTemporalStoreFlag,
#endif

// recycler memory restrict test flags
IsEnabledMaxMarkStackPageCountFlag, GetMaxMarkStackPageCountFlag, SetMaxMarkStackPageCountFlag,
IsEnabledMaxTrackedObjectListCountFlag, GetMaxTrackedObjectListCountFlag, SetMaxTrackedObjectListCountFlag,

// make the recycler page integration path easier to hit
IsEnabledNumberAllocPlusSizeFlag, GetNumberAllocPlusSizeFlag, SetNumberAllocPlusSizeFlag,

#if DBG
IsEnabledInitializeInterpreterSlotsWithInvalidStackVarFlag, GetInitializeInterpreterSlotsWithInvalidStackVarFlag, SetInitializeInterpreterSlotsWithInvalidStackVarFlag,
#endif

#if DBG
IsEnabledPRNGSeed0Flag, GetPRNGSeed0Flag, SetPRNGSeed0Flag,
IsEnabledPRNGSeed1Flag, GetPRNGSeed1Flag, SetPRNGSeed1Flag,
#endif

IsEnabledClearInlineCachesOnCollectFlag, GetClearInlineCachesOnCollectFlag, SetClearInlineCachesOnCollectFlag,
IsEnabledInlineCacheInvalidationListCompactionThresholdFlag, GetInlineCacheInvalidationListCompactionThresholdFlag, SetInlineCacheInvalidationListCompactionThresholdFlag,
IsEnabledConstructorCacheInvalidationThresholdFlag, GetConstructorCacheInvalidationThresholdFlag, SetConstructorCacheInvalidationThresholdFlag,

#ifdef IR_VIEWER
IsEnabledIRViewerFlag, GetIRViewerFlag, SetIRViewerFlag,
#endif /* IR_VIEWER */

IsEnabledGCMemoryThresholdFlag, GetGCMemoryThresholdFlag, SetGCMemoryThresholdFlag,

#if DBG
    IsEnabledSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag, GetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag, SetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag,
#endif

IsEnabledJITServerIdleTimeoutFlag, GetJITServerIdleTimeoutFlag, SetJITServerIdleTimeoutFlag,
IsEnabledJITServerMaxInactivePageAllocatorCountFlag, GetJITServerMaxInactivePageAllocatorCountFlag, SetJITServerMaxInactivePageAllocatorCountFlag,

IsEnabledStrictWriteBarrierCheckFlag, GetStrictWriteBarrierCheckFlag, SetStrictWriteBarrierCheckFlag,
IsEnabledWriteBarrierTestFlag, GetWriteBarrierTestFlag, SetWriteBarrierTestFlag,
IsEnabledForceSoftwareWriteBarrierFlag, GetForceSoftwareWriteBarrierFlag, SetForceSoftwareWriteBarrierFlag,
IsEnabledVerifyBarrierBitFlag, GetVerifyBarrierBitFlag, SetVerifyBarrierBitFlag,
IsEnabledEnableBGFreeZeroFlag, GetEnableBGFreeZeroFlag, SetEnableBGFreeZeroFlag,
IsEnabledKeepRecyclerTrackDataFlag, GetKeepRecyclerTrackDataFlag, SetKeepRecyclerTrackDataFlag,

IsEnabledMaxSingleAllocSizeInMBFlag, GetMaxSingleAllocSizeInMBFlag, SetMaxSingleAllocSizeInMBFlag,
        NotifyUnhandledException
    };
    return ChakraRTInterface::InitializeTestHooks(testHooks);
}

