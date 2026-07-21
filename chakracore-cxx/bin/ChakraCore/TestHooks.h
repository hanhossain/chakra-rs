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

#if DBG
bool (*pfIsEnabledArrayValidateFlag)();
int32_t (*pfGetArrayValidateFlag)(bool *flag);
int32_t (*pfSetArrayValidateFlag)(bool flag);
#endif
bool (*pfIsEnabledAsmJsFlag)();
int32_t (*pfGetAsmJsFlag)(bool *flag);
int32_t (*pfSetAsmJsFlag)(bool flag);
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
bool (*pfIsEnabledWasmMaxTableSizeFlag)();
int32_t (*pfGetWasmMaxTableSizeFlag)(int *flag);
int32_t (*pfSetWasmMaxTableSizeFlag)(int flag);
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
bool (*pfIsEnabledBailOnNoProfileLimitFlag)();
int32_t (*pfGetBailOnNoProfileLimitFlag)(int *flag);
int32_t (*pfSetBailOnNoProfileLimitFlag)(int flag);
bool (*pfIsEnabledBailOnNoProfileRejitLimitFlag)();
int32_t (*pfGetBailOnNoProfileRejitLimitFlag)(int *flag);
int32_t (*pfSetBailOnNoProfileRejitLimitFlag)(int flag);
bool (*pfIsEnabledBaselineModeFlag)();
int32_t (*pfGetBaselineModeFlag)(bool *flag);
int32_t (*pfSetBaselineModeFlag)(bool flag);
bool (*pfIsEnabledDumpOnCrashFlag)();
int32_t (*pfGetDumpOnCrashFlag)(BSTR *flag);
int32_t (*pfSetDumpOnCrashFlag)(BSTR flag);
bool (*pfIsEnabledFullMemoryDumpFlag)();
int32_t (*pfGetFullMemoryDumpFlag)(BSTR *flag);
int32_t (*pfSetFullMemoryDumpFlag)(BSTR flag);
bool (*pfIsEnabledBailOutAtEveryLineFlag)();
int32_t (*pfGetBailOutAtEveryLineFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryLineFlag)(bool flag);
bool (*pfIsEnabledBailOutAtEveryByteCodeFlag)();
int32_t (*pfGetBailOutAtEveryByteCodeFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryByteCodeFlag)(bool flag);
bool (*pfIsEnabledBailOutAtEveryImplicitCallFlag)();
int32_t (*pfGetBailOutAtEveryImplicitCallFlag)(bool *flag);
int32_t (*pfSetBailOutAtEveryImplicitCallFlag)(bool flag);
bool (*pfIsEnabledBenchmarkFlag)();
int32_t (*pfGetBenchmarkFlag)(bool *flag);
int32_t (*pfSetBenchmarkFlag)(bool flag);
bool (*pfIsEnabledBgJitFlag)();
int32_t (*pfGetBgJitFlag)(bool *flag);
int32_t (*pfSetBgJitFlag)(bool flag);
bool (*pfIsEnabledBgParseFlag)();
int32_t (*pfGetBgParseFlag)(bool *flag);
int32_t (*pfSetBgParseFlag)(bool flag);
bool (*pfIsEnabledBgJitDelayFlag)();
int32_t (*pfGetBgJitDelayFlag)(int *flag);
int32_t (*pfSetBgJitDelayFlag)(int flag);
bool (*pfIsEnabledBgJitDelayFgBufferFlag)();
int32_t (*pfGetBgJitDelayFgBufferFlag)(int *flag);
int32_t (*pfSetBgJitDelayFgBufferFlag)(int flag);
bool (*pfIsEnabledBgJitPendingFuncCapFlag)();
int32_t (*pfGetBgJitPendingFuncCapFlag)(int *flag);
int32_t (*pfSetBgJitPendingFuncCapFlag)(int flag);

bool (*pfIsEnabledCreateFunctionProxyFlag)();
int32_t (*pfGetCreateFunctionProxyFlag)(bool *flag);
int32_t (*pfSetCreateFunctionProxyFlag)(bool flag);
bool (*pfIsEnabledHybridFgJitFlag)();
int32_t (*pfGetHybridFgJitFlag)(bool *flag);
int32_t (*pfSetHybridFgJitFlag)(bool flag);
bool (*pfIsEnabledHybridFgJitBgQueueLengthThresholdFlag)();
int32_t (*pfGetHybridFgJitBgQueueLengthThresholdFlag)(int *flag);
int32_t (*pfSetHybridFgJitBgQueueLengthThresholdFlag)(int flag);
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
bool (*pfIsEnabledConstructorInlineThresholdFlag)();
int32_t (*pfGetConstructorInlineThresholdFlag)(int *flag);
int32_t (*pfSetConstructorInlineThresholdFlag)(int flag);
bool (*pfIsEnabledConstructorCallsRequiredToFinalizeCachedTypeFlag)();
int32_t (*pfGetConstructorCallsRequiredToFinalizeCachedTypeFlag)(int *flag);
int32_t (*pfSetConstructorCallsRequiredToFinalizeCachedTypeFlag)(int flag);
bool (*pfIsEnabledPropertyCacheMissPenaltyFlag)();
int32_t (*pfGetPropertyCacheMissPenaltyFlag)(int *flag);
int32_t (*pfSetPropertyCacheMissPenaltyFlag)(int flag);
bool (*pfIsEnabledPropertyCacheMissThresholdFlag)();
int32_t (*pfGetPropertyCacheMissThresholdFlag)(int *flag);
int32_t (*pfSetPropertyCacheMissThresholdFlag)(int flag);
bool (*pfIsEnabledPropertyCacheMissResetFlag)();
int32_t (*pfGetPropertyCacheMissResetFlag)(int *flag);
int32_t (*pfSetPropertyCacheMissResetFlag)(int flag);
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
bool (*pfIsEnabledDeferParseFlag)();
int32_t (*pfGetDeferParseFlag)(int *flag);
int32_t (*pfSetDeferParseFlag)(int flag);
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
bool (*pfIsEnabledProfileBasedSpeculationCapFlag)();
int32_t (*pfGetProfileBasedSpeculationCapFlag)(int *flag);
int32_t (*pfSetProfileBasedSpeculationCapFlag)(int flag);
bool (*pfIsEnabledExecuteByteCodeBufferReturnsInvalidByteCodeFlag)();
int32_t (*pfGetExecuteByteCodeBufferReturnsInvalidByteCodeFlag)(bool *flag);
int32_t (*pfSetExecuteByteCodeBufferReturnsInvalidByteCodeFlag)(bool flag);
bool (*pfIsEnabledExpirableCollectionGCCountFlag)();
int32_t (*pfGetExpirableCollectionGCCountFlag)(int *flag);
int32_t (*pfSetExpirableCollectionGCCountFlag)(int flag);
bool (*pfIsEnabledExpirableCollectionTriggerThresholdFlag)();
int32_t (*pfGetExpirableCollectionTriggerThresholdFlag)(int *flag);
int32_t (*pfSetExpirableCollectionTriggerThresholdFlag)(int flag);
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

bool (*pfIsEnabledES6RegExPrototypePropertiesFlag)();
int32_t (*pfGetES6RegExPrototypePropertiesFlag)(bool *flag);
int32_t (*pfSetES6RegExPrototypePropertiesFlag)(bool flag);

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
bool (*pfIsEnabledForceFragmentAddressSpaceFlag)();
int32_t (*pfGetForceFragmentAddressSpaceFlag)(int *flag);
int32_t (*pfSetForceFragmentAddressSpaceFlag)(int flag);
bool (*pfIsEnabledForceOOMOnEBCommitFlag)();
int32_t (*pfGetForceOOMOnEBCommitFlag)(int *flag);
int32_t (*pfSetForceOOMOnEBCommitFlag)(int flag);
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
bool (*pfIsEnabledDelayFullJITSmallFuncFlag)();
int32_t (*pfGetDelayFullJITSmallFuncFlag)(int *flag);
int32_t (*pfSetDelayFullJITSmallFuncFlag)(int flag);
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
bool (*pfIsEnabledForceSerializedBytecodeMajorVersionFlag)();
int32_t (*pfGetForceSerializedBytecodeMajorVersionFlag)(int *flag);
int32_t (*pfSetForceSerializedBytecodeMajorVersionFlag)(int flag);
bool (*pfIsEnabledForceSerializedBytecodeVersionSchemaFlag)();
int32_t (*pfGetForceSerializedBytecodeVersionSchemaFlag)(int *flag);
int32_t (*pfSetForceSerializedBytecodeVersionSchemaFlag)(int flag);
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
bool (*pfIsEnabledByteCodeBranchLimitFlag)();
int32_t (*pfGetByteCodeBranchLimitFlag)(int *flag);
int32_t (*pfSetByteCodeBranchLimitFlag)(int flag);
bool (*pfIsEnabledMediumByteCodeLayoutFlag)();
int32_t (*pfGetMediumByteCodeLayoutFlag)(bool *flag);
int32_t (*pfSetMediumByteCodeLayoutFlag)(bool flag);
bool (*pfIsEnabledLargeByteCodeLayoutFlag)();
int32_t (*pfGetLargeByteCodeLayoutFlag)(bool *flag);
int32_t (*pfSetLargeByteCodeLayoutFlag)(bool flag);
#endif
bool (*pfIsEnabledInduceCodeGenFailureFlag)();
int32_t (*pfGetInduceCodeGenFailureFlag)(int *flag);
int32_t (*pfSetInduceCodeGenFailureFlag)(int flag);
bool (*pfIsEnabledInduceCodeGenFailureSeedFlag)();
int32_t (*pfGetInduceCodeGenFailureSeedFlag)(int *flag);
int32_t (*pfSetInduceCodeGenFailureSeedFlag)(int flag);
bool (*pfIsEnabledInjectPartiallyInitializedInterpreterFrameErrorFlag)();
int32_t (*pfGetInjectPartiallyInitializedInterpreterFrameErrorFlag)(int *flag);
int32_t (*pfSetInjectPartiallyInitializedInterpreterFrameErrorFlag)(int flag);
bool (*pfIsEnabledInjectPartiallyInitializedInterpreterFrameErrorTypeFlag)();
int32_t (*pfGetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag)(int *flag);
int32_t (*pfSetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag)(int flag);
bool (*pfIsEnabledGenerateByteCodeBufferReturnsCantGenerateFlag)();
int32_t (*pfGetGenerateByteCodeBufferReturnsCantGenerateFlag)(bool *flag);
int32_t (*pfSetGenerateByteCodeBufferReturnsCantGenerateFlag)(bool flag);
bool (*pfIsEnabledGoptCleanupThresholdFlag)();
int32_t (*pfGetGoptCleanupThresholdFlag)(int *flag);
int32_t (*pfSetGoptCleanupThresholdFlag)(int flag);
bool (*pfIsEnabledAsmGoptCleanupThresholdFlag)();
int32_t (*pfGetAsmGoptCleanupThresholdFlag)(int *flag);
int32_t (*pfSetAsmGoptCleanupThresholdFlag)(int flag);
bool (*pfIsEnabledHighPrecisionDateFlag)();
int32_t (*pfGetHighPrecisionDateFlag)(bool *flag);
int32_t (*pfSetHighPrecisionDateFlag)(bool flag);
bool (*pfIsEnabledInlineCountMaxFlag)();
int32_t (*pfGetInlineCountMaxFlag)(int *flag);
int32_t (*pfSetInlineCountMaxFlag)(int flag);
bool (*pfIsEnabledInlineCountMaxInLoopBodiesFlag)();
int32_t (*pfGetInlineCountMaxInLoopBodiesFlag)(int *flag);
int32_t (*pfSetInlineCountMaxInLoopBodiesFlag)(int flag);
bool (*pfIsEnabledicminlbFlag)();
int32_t (*pfGeticminlbFlag)(int *flag);
int32_t (*pfSeticminlbFlag)(int flag);
bool (*pfIsEnabledInlineInLoopBodyScaleDownFactorFlag)();
int32_t (*pfGetInlineInLoopBodyScaleDownFactorFlag)(int *flag);
int32_t (*pfSetInlineInLoopBodyScaleDownFactorFlag)(int flag);
bool (*pfIsEnablediilbsdfFlag)();
int32_t (*pfGetiilbsdfFlag)(int *flag);
int32_t (*pfSetiilbsdfFlag)(int flag);
bool (*pfIsEnabledInlineThresholdFlag)();
int32_t (*pfGetInlineThresholdFlag)(int *flag);
int32_t (*pfSetInlineThresholdFlag)(int flag);
bool (*pfIsEnabledAggressiveInlineCountMaxFlag)();
int32_t (*pfGetAggressiveInlineCountMaxFlag)(int *flag);
int32_t (*pfSetAggressiveInlineCountMaxFlag)(int flag);
bool (*pfIsEnabledAggressiveInlineThresholdFlag)();
int32_t (*pfGetAggressiveInlineThresholdFlag)(int *flag);
int32_t (*pfSetAggressiveInlineThresholdFlag)(int flag);
bool (*pfIsEnabledInlineThresholdAdjustCountInLargeFunctionFlag)();
int32_t (*pfGetInlineThresholdAdjustCountInLargeFunctionFlag)(int *flag);
int32_t (*pfSetInlineThresholdAdjustCountInLargeFunctionFlag)(int flag);
bool (*pfIsEnabledInlineThresholdAdjustCountInMediumSizedFunctionFlag)();
int32_t (*pfGetInlineThresholdAdjustCountInMediumSizedFunctionFlag)(int *flag);
int32_t (*pfSetInlineThresholdAdjustCountInMediumSizedFunctionFlag)(int flag);
bool (*pfIsEnabledInlineThresholdAdjustCountInSmallFunctionFlag)();
int32_t (*pfGetInlineThresholdAdjustCountInSmallFunctionFlag)(int *flag);
int32_t (*pfSetInlineThresholdAdjustCountInSmallFunctionFlag)(int flag);
bool (*pfIsEnabledAsmJsInlineAdjustFlag)();
int32_t (*pfGetAsmJsInlineAdjustFlag)(int *flag);
int32_t (*pfSetAsmJsInlineAdjustFlag)(int flag);
bool (*pfIsEnabledInterpretFlag)();
int32_t (*pfGetInterpretFlag)(BSTR *flag);
int32_t (*pfSetInterpretFlag)(BSTR flag);
bool (*pfIsEnabledJitQueueThresholdFlag)();
int32_t (*pfGetJitQueueThresholdFlag)(int *flag);
int32_t (*pfSetJitQueueThresholdFlag)(int flag);
bool (*pfIsEnabledLoopInlineThresholdFlag)();
int32_t (*pfGetLoopInlineThresholdFlag)(int *flag);
int32_t (*pfSetLoopInlineThresholdFlag)(int flag);
bool (*pfIsEnabledLeafInlineThresholdFlag)();
int32_t (*pfGetLeafInlineThresholdFlag)(int *flag);
int32_t (*pfSetLeafInlineThresholdFlag)(int flag);
bool (*pfIsEnabledConstantArgumentInlineThresholdFlag)();
int32_t (*pfGetConstantArgumentInlineThresholdFlag)(int *flag);
int32_t (*pfSetConstantArgumentInlineThresholdFlag)(int flag);
bool (*pfIsEnabledRecursiveInlineThresholdFlag)();
int32_t (*pfGetRecursiveInlineThresholdFlag)(int *flag);
int32_t (*pfSetRecursiveInlineThresholdFlag)(int flag);
bool (*pfIsEnabledRecursiveInlineDepthMaxFlag)();
int32_t (*pfGetRecursiveInlineDepthMaxFlag)(int *flag);
int32_t (*pfSetRecursiveInlineDepthMaxFlag)(int flag);
bool (*pfIsEnabledRecursiveInlineDepthMinFlag)();
int32_t (*pfGetRecursiveInlineDepthMinFlag)(int *flag);
int32_t (*pfSetRecursiveInlineDepthMinFlag)(int flag);
bool (*pfIsEnabledRedeferralCapFlag)();
int32_t (*pfGetRedeferralCapFlag)(int *flag);
int32_t (*pfSetRedeferralCapFlag)(int flag);
bool (*pfIsEnabledLoopFlag)();
int32_t (*pfGetLoopFlag)(int *flag);
int32_t (*pfSetLoopFlag)(int flag);
bool (*pfIsEnabledLoopInterpretCountFlag)();
int32_t (*pfGetLoopInterpretCountFlag)(int *flag);
int32_t (*pfSetLoopInterpretCountFlag)(int flag);
bool (*pfIsEnabledlicFlag)();
int32_t (*pfGetlicFlag)(int *flag);
int32_t (*pfSetlicFlag)(int flag);
bool (*pfIsEnabledLoopProfileIterationsFlag)();
int32_t (*pfGetLoopProfileIterationsFlag)(int *flag);
int32_t (*pfSetLoopProfileIterationsFlag)(int flag);
bool (*pfIsEnabledOutsideLoopInlineThresholdFlag)();
int32_t (*pfGetOutsideLoopInlineThresholdFlag)(int *flag);
int32_t (*pfSetOutsideLoopInlineThresholdFlag)(int flag);
bool (*pfIsEnabledMaxFuncInlineDepthFlag)();
int32_t (*pfGetMaxFuncInlineDepthFlag)(int *flag);
int32_t (*pfSetMaxFuncInlineDepthFlag)(int flag);
bool (*pfIsEnabledMaxNumberOfInlineesWithLoopFlag)();
int32_t (*pfGetMaxNumberOfInlineesWithLoopFlag)(int *flag);
int32_t (*pfSetMaxNumberOfInlineesWithLoopFlag)(int flag);
#ifdef MEMSPECT_TRACKING
#endif
bool (*pfIsEnabledPolymorphicInlineThresholdFlag)();
int32_t (*pfGetPolymorphicInlineThresholdFlag)(int *flag);
int32_t (*pfSetPolymorphicInlineThresholdFlag)(int flag);
bool (*pfIsEnabledPrimeRecyclerFlag)();
int32_t (*pfGetPrimeRecyclerFlag)(bool *flag);
int32_t (*pfSetPrimeRecyclerFlag)(bool flag);
bool (*pfIsEnabledTraceEngineRefcountFlag)();
int32_t (*pfGetTraceEngineRefcountFlag)(bool *flag);
int32_t (*pfSetTraceEngineRefcountFlag)(bool flag);
#if defined(CHECK_MEMORY_LEAK)
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

bool (*pfIsEnabledJitLoopBodyHotLoopThresholdFlag)();
int32_t (*pfGetJitLoopBodyHotLoopThresholdFlag)(int *flag);
int32_t (*pfSetJitLoopBodyHotLoopThresholdFlag)(int flag);
bool (*pfIsEnabledLoopBodySizeThresholdToDisableOptsFlag)();
int32_t (*pfGetLoopBodySizeThresholdToDisableOptsFlag)(int *flag);
int32_t (*pfSetLoopBodySizeThresholdToDisableOptsFlag)(int flag);

bool (*pfIsEnabledMaxJitThreadCountFlag)();
int32_t (*pfGetMaxJitThreadCountFlag)(int *flag);
int32_t (*pfSetMaxJitThreadCountFlag)(int flag);
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

bool (*pfIsEnabledMinInterpretCountFlag)();
int32_t (*pfGetMinInterpretCountFlag)(int *flag);
int32_t (*pfSetMinInterpretCountFlag)(int flag);
bool (*pfIsEnabledMinSimpleJitRunCountFlag)();
int32_t (*pfGetMinSimpleJitRunCountFlag)(int *flag);
int32_t (*pfSetMinSimpleJitRunCountFlag)(int flag);
bool (*pfIsEnabledMaxInterpretCountFlag)();
int32_t (*pfGetMaxInterpretCountFlag)(int *flag);
int32_t (*pfSetMaxInterpretCountFlag)(int flag);
bool (*pfIsEnabledMicFlag)();
int32_t (*pfGetMicFlag)(int *flag);
int32_t (*pfSetMicFlag)(int flag);
bool (*pfIsEnabledMaxSimpleJitRunCountFlag)();
int32_t (*pfGetMaxSimpleJitRunCountFlag)(int *flag);
int32_t (*pfSetMaxSimpleJitRunCountFlag)(int flag);
bool (*pfIsEnabledMsjrcFlag)();
int32_t (*pfGetMsjrcFlag)(int *flag);
int32_t (*pfSetMsjrcFlag)(int flag);
bool (*pfIsEnabledMinMemOpCountFlag)();
int32_t (*pfGetMinMemOpCountFlag)(int *flag);
int32_t (*pfSetMinMemOpCountFlag)(int flag);
bool (*pfIsEnabledMmocFlag)();
int32_t (*pfGetMmocFlag)(int *flag);
int32_t (*pfSetMmocFlag)(int flag);

#if ENABLE_COPYONACCESS_ARRAY
bool (*pfIsEnabledMaxCopyOnAccessArrayLengthFlag)();
int32_t (*pfGetMaxCopyOnAccessArrayLengthFlag)(int *flag);
int32_t (*pfSetMaxCopyOnAccessArrayLengthFlag)(int flag);
bool (*pfIsEnabledMinCopyOnAccessArrayLengthFlag)();
int32_t (*pfGetMinCopyOnAccessArrayLengthFlag)(int *flag);
int32_t (*pfSetMinCopyOnAccessArrayLengthFlag)(int flag);
bool (*pfIsEnabledCopyOnAccessArraySegmentCacheSizeFlag)();
int32_t (*pfGetCopyOnAccessArraySegmentCacheSizeFlag)(int *flag);
int32_t (*pfSetCopyOnAccessArraySegmentCacheSizeFlag)(int flag);
#endif

bool (*pfIsEnabledMinTemplatizedJitRunCountFlag)();
int32_t (*pfGetMinTemplatizedJitRunCountFlag)(int *flag);
int32_t (*pfSetMinTemplatizedJitRunCountFlag)(int flag);
bool (*pfIsEnabledMinAsmJsInterpreterRunCountFlag)();
int32_t (*pfGetMinAsmJsInterpreterRunCountFlag)(int *flag);
int32_t (*pfSetMinAsmJsInterpreterRunCountFlag)(int flag);

bool (*pfIsEnabledMinTemplatizedJitLoopRunCountFlag)();
int32_t (*pfGetMinTemplatizedJitLoopRunCountFlag)(int *flag);
int32_t (*pfSetMinTemplatizedJitLoopRunCountFlag)(int flag);
bool (*pfIsEnabledMaxTemplatizedJitRunCountFlag)();
int32_t (*pfGetMaxTemplatizedJitRunCountFlag)(int *flag);
int32_t (*pfSetMaxTemplatizedJitRunCountFlag)(int flag);
bool (*pfIsEnabledMtjrcFlag)();
int32_t (*pfGetMtjrcFlag)(int *flag);
int32_t (*pfSetMtjrcFlag)(int flag);
bool (*pfIsEnabledMaxAsmJsInterpreterRunCountFlag)();
int32_t (*pfGetMaxAsmJsInterpreterRunCountFlag)(int *flag);
int32_t (*pfSetMaxAsmJsInterpreterRunCountFlag)(int flag);
bool (*pfIsEnabledMaicFlag)();
int32_t (*pfGetMaicFlag)(int *flag);
int32_t (*pfSetMaicFlag)(int flag);

bool (*pfIsEnabledAutoProfilingInterpreter0LimitFlag)();
int32_t (*pfGetAutoProfilingInterpreter0LimitFlag)(int *flag);
int32_t (*pfSetAutoProfilingInterpreter0LimitFlag)(int flag);
bool (*pfIsEnabledProfilingInterpreter0LimitFlag)();
int32_t (*pfGetProfilingInterpreter0LimitFlag)(int *flag);
int32_t (*pfSetProfilingInterpreter0LimitFlag)(int flag);
bool (*pfIsEnabledAutoProfilingInterpreter1LimitFlag)();
int32_t (*pfGetAutoProfilingInterpreter1LimitFlag)(int *flag);
int32_t (*pfSetAutoProfilingInterpreter1LimitFlag)(int flag);
bool (*pfIsEnabledSimpleJitLimitFlag)();
int32_t (*pfGetSimpleJitLimitFlag)(int *flag);
int32_t (*pfSetSimpleJitLimitFlag)(int flag);
bool (*pfIsEnabledProfilingInterpreter1LimitFlag)();
int32_t (*pfGetProfilingInterpreter1LimitFlag)(int *flag);
int32_t (*pfSetProfilingInterpreter1LimitFlag)(int flag);

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

bool (*pfIsEnabledSimpleJitAfterFlag)();
int32_t (*pfGetSimpleJitAfterFlag)(int *flag);
int32_t (*pfSetSimpleJitAfterFlag)(int flag);
bool (*pfIsEnabledSjaFlag)();
int32_t (*pfGetSjaFlag)(int *flag);
int32_t (*pfSetSjaFlag)(int flag);
bool (*pfIsEnabledFullJitAfterFlag)();
int32_t (*pfGetFullJitAfterFlag)(int *flag);
int32_t (*pfSetFullJitAfterFlag)(int flag);
bool (*pfIsEnabledFjaFlag)();
int32_t (*pfGetFjaFlag)(int *flag);
int32_t (*pfSetFjaFlag)(int flag);

bool (*pfIsEnabledNewSimpleJitFlag)();
int32_t (*pfGetNewSimpleJitFlag)(bool *flag);
int32_t (*pfSetNewSimpleJitFlag)(bool flag);

bool (*pfIsEnabledMaxLinearIntCaseCountFlag)();
int32_t (*pfGetMaxLinearIntCaseCountFlag)(int *flag);
int32_t (*pfSetMaxLinearIntCaseCountFlag)(int flag);
bool (*pfIsEnabledMaxSingleCharStrJumpTableSizeFlag)();
int32_t (*pfGetMaxSingleCharStrJumpTableSizeFlag)(int *flag);
int32_t (*pfSetMaxSingleCharStrJumpTableSizeFlag)(int flag);
bool (*pfIsEnabledMaxSingleCharStrJumpTableRatioFlag)();
int32_t (*pfGetMaxSingleCharStrJumpTableRatioFlag)(int *flag);
int32_t (*pfSetMaxSingleCharStrJumpTableRatioFlag)(int flag);
bool (*pfIsEnabledMinSwitchJumpTableSizeFlag)();
int32_t (*pfGetMinSwitchJumpTableSizeFlag)(int *flag);
int32_t (*pfSetMinSwitchJumpTableSizeFlag)(int flag);
bool (*pfIsEnabledMaxLinearStringCaseCountFlag)();
int32_t (*pfGetMaxLinearStringCaseCountFlag)(int *flag);
int32_t (*pfSetMaxLinearStringCaseCountFlag)(int flag);
bool (*pfIsEnabledMinDeferredFuncTokenCountFlag)();
int32_t (*pfGetMinDeferredFuncTokenCountFlag)(int *flag);
int32_t (*pfSetMinDeferredFuncTokenCountFlag)(int flag);
#if DBG
bool (*pfIsEnabledSkipFuncCountForBailOnNoProfileFlag)();
int32_t (*pfGetSkipFuncCountForBailOnNoProfileFlag)(int *flag);
int32_t (*pfSetSkipFuncCountForBailOnNoProfileFlag)(int flag);
#endif
bool (*pfIsEnabledMaxJITFunctionBytecodeByteLengthFlag)();
int32_t (*pfGetMaxJITFunctionBytecodeByteLengthFlag)(int *flag);
int32_t (*pfSetMaxJITFunctionBytecodeByteLengthFlag)(int flag);
bool (*pfIsEnabledMaxJITFunctionBytecodeCountFlag)();
int32_t (*pfGetMaxJITFunctionBytecodeCountFlag)(int *flag);
int32_t (*pfSetMaxJITFunctionBytecodeCountFlag)(int flag);
bool (*pfIsEnabledMaxLoopsPerFunctionFlag)();
int32_t (*pfGetMaxLoopsPerFunctionFlag)(int *flag);
int32_t (*pfSetMaxLoopsPerFunctionFlag)(int flag);
bool (*pfIsEnabledFuncObjectInlineCacheThresholdFlag)();
int32_t (*pfGetFuncObjectInlineCacheThresholdFlag)(int *flag);
int32_t (*pfSetFuncObjectInlineCacheThresholdFlag)(int flag);
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
bool (*pfIsEnabledNopFrequencyFlag)();
int32_t (*pfGetNopFrequencyFlag)(int *flag);
int32_t (*pfSetNopFrequencyFlag)(int flag);
bool (*pfIsEnabledNoStrictModeFlag)();
int32_t (*pfGetNoStrictModeFlag)(bool *flag);
int32_t (*pfSetNoStrictModeFlag)(bool flag);
bool (*pfIsEnabledNormalizeStatsFlag)();
int32_t (*pfGetNormalizeStatsFlag)(bool *flag);
int32_t (*pfSetNormalizeStatsFlag)(bool flag);
#ifdef ENABLE_TRACE
bool (*pfIsEnabledInMemoryTraceFlag)();
int32_t (*pfGetInMemoryTraceFlag)(bool *flag);
int32_t (*pfSetInMemoryTraceFlag)(bool flag);
bool (*pfIsEnabledInMemoryTraceBufferSizeFlag)();
int32_t (*pfGetInMemoryTraceBufferSizeFlag)(int *flag);
int32_t (*pfSetInMemoryTraceBufferSizeFlag)(int flag);
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
bool (*pfIsEnabledProfileDictionaryFlag)();
int32_t (*pfGetProfileDictionaryFlag)(int *flag);
int32_t (*pfSetProfileDictionaryFlag)(int flag);
#endif
#ifdef PROFILE_EXEC
bool (*pfIsEnabledProfileThresholdFlag)();
int32_t (*pfGetProfileThresholdFlag)(int *flag);
int32_t (*pfSetProfileThresholdFlag)(int flag);
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
bool (*pfIsEnabledRejitMaxBailOutCountFlag)();
int32_t (*pfGetRejitMaxBailOutCountFlag)(int *flag);
int32_t (*pfSetRejitMaxBailOutCountFlag)(int flag);
bool (*pfIsEnabledCallsToBailoutsRatioForRejitFlag)();
int32_t (*pfGetCallsToBailoutsRatioForRejitFlag)(int *flag);
int32_t (*pfSetCallsToBailoutsRatioForRejitFlag)(int flag);
bool (*pfIsEnabledLoopIterationsToBailoutsRatioForRejitFlag)();
int32_t (*pfGetLoopIterationsToBailoutsRatioForRejitFlag)(int *flag);
int32_t (*pfSetLoopIterationsToBailoutsRatioForRejitFlag)(int flag);
bool (*pfIsEnabledMinBailOutsBeforeRejitFlag)();
int32_t (*pfGetMinBailOutsBeforeRejitFlag)(int *flag);
int32_t (*pfSetMinBailOutsBeforeRejitFlag)(int flag);
bool (*pfIsEnabledMinBailOutsBeforeRejitForLoopsFlag)();
int32_t (*pfGetMinBailOutsBeforeRejitForLoopsFlag)(int *flag);
int32_t (*pfSetMinBailOutsBeforeRejitForLoopsFlag)(int flag);
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
bool (*pfIsEnabledRecyclerBackgroundStressFlag)();
int32_t (*pfGetRecyclerBackgroundStressFlag)(bool *flag);
int32_t (*pfSetRecyclerBackgroundStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerConcurrentStressFlag)();
int32_t (*pfGetRecyclerConcurrentStressFlag)(bool *flag);
int32_t (*pfSetRecyclerConcurrentStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerConcurrentRepeatStressFlag)();
int32_t (*pfGetRecyclerConcurrentRepeatStressFlag)(bool *flag);
int32_t (*pfSetRecyclerConcurrentRepeatStressFlag)(bool flag);
bool (*pfIsEnabledRecyclerPartialStressFlag)();
int32_t (*pfGetRecyclerPartialStressFlag)(bool *flag);
int32_t (*pfSetRecyclerPartialStressFlag)(bool flag);
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
bool (*pfIsEnabledRecyclerPriorityBoostTimeoutFlag)();
int32_t (*pfGetRecyclerPriorityBoostTimeoutFlag)(int *flag);
int32_t (*pfSetRecyclerPriorityBoostTimeoutFlag)(int flag);
bool (*pfIsEnabledRecyclerThreadCollectTimeoutFlag)();
int32_t (*pfGetRecyclerThreadCollectTimeoutFlag)(int *flag);
int32_t (*pfSetRecyclerThreadCollectTimeoutFlag)(int flag);
bool (*pfIsEnabledEnableConcurrentSweepAllocFlag)();
int32_t (*pfGetEnableConcurrentSweepAllocFlag)(bool *flag);
int32_t (*pfSetEnableConcurrentSweepAllocFlag)(bool flag);
bool (*pfIsEnabledecsaFlag)();
int32_t (*pfGetecsaFlag)(bool *flag);
int32_t (*pfSetecsaFlag)(bool flag);
bool (*pfIsEnabledPageHeapFlag)();
int32_t (*pfGetPageHeapFlag)(int *flag);
int32_t (*pfSetPageHeapFlag)(int flag);
bool (*pfIsEnabledPageHeapAllocStackFlag)();
int32_t (*pfGetPageHeapAllocStackFlag)(bool *flag);
int32_t (*pfSetPageHeapAllocStackFlag)(bool flag);
bool (*pfIsEnabledPageHeapFreeStackFlag)();
int32_t (*pfGetPageHeapFreeStackFlag)(bool *flag);
int32_t (*pfSetPageHeapFreeStackFlag)(bool flag);
bool (*pfIsEnabledPageHeapBlockTypeFlag)();
int32_t (*pfGetPageHeapBlockTypeFlag)(int *flag);
int32_t (*pfSetPageHeapBlockTypeFlag)(int flag);
bool (*pfIsEnabledPageHeapDecommitGuardPageFlag)();
int32_t (*pfGetPageHeapDecommitGuardPageFlag)(bool *flag);
int32_t (*pfSetPageHeapDecommitGuardPageFlag)(bool flag);
#ifdef RECYCLER_NO_PAGE_REUSE
bool (*pfIsEnabledRecyclerNoPageReuseFlag)();
int32_t (*pfGetRecyclerNoPageReuseFlag)(bool *flag);
int32_t (*pfSetRecyclerNoPageReuseFlag)(bool flag);
#endif
#ifdef RECYCLER_MEMORY_VERIFY
bool (*pfIsEnabledRecyclerVerifyPadSizeFlag)();
int32_t (*pfGetRecyclerVerifyPadSizeFlag)(int *flag);
int32_t (*pfSetRecyclerVerifyPadSizeFlag)(int flag);
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
bool (*pfIsEnabledLowMemoryCapFlag)();
int32_t (*pfGetLowMemoryCapFlag)(int *flag);
int32_t (*pfSetLowMemoryCapFlag)(int flag);
bool (*pfIsEnabledNewPagesCapDuringBGSweepingFlag)();
int32_t (*pfGetNewPagesCapDuringBGSweepingFlag)(int *flag);
int32_t (*pfSetNewPagesCapDuringBGSweepingFlag)(int flag);
bool (*pfIsEnabledAllocPolicyLimitFlag)();
int32_t (*pfGetAllocPolicyLimitFlag)(int *flag);
int32_t (*pfSetAllocPolicyLimitFlag)(int flag);
bool (*pfIsEnabledRuntimeDataOutputFileFlag)();
int32_t (*pfGetRuntimeDataOutputFileFlag)(BSTR *flag);
int32_t (*pfSetRuntimeDataOutputFileFlag)(BSTR flag);
bool (*pfIsEnabledSpeculationCapFlag)();
int32_t (*pfGetSpeculationCapFlag)(int *flag);
int32_t (*pfSetSpeculationCapFlag)(int flag);
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
bool (*pfIsEnabledSwitchOptHolesThresholdFlag)();
int32_t (*pfGetSwitchOptHolesThresholdFlag)(int *flag);
int32_t (*pfSetSwitchOptHolesThresholdFlag)(int flag);
bool (*pfIsEnabledTempMinFlag)();
int32_t (*pfGetTempMinFlag)(int *flag);
int32_t (*pfSetTempMinFlag)(int flag);
bool (*pfIsEnabledTempMaxFlag)();
int32_t (*pfGetTempMaxFlag)(int *flag);
int32_t (*pfSetTempMaxFlag)(int flag);

#if defined(_M_X64)
bool (*pfIsEnabledLoopAlignNopLimitFlag)();
int32_t (*pfGetLoopAlignNopLimitFlag)(int *flag);
int32_t (*pfSetLoopAlignNopLimitFlag)(int flag);
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
bool (*pfIsEnabledTraceMetaDataParsingFlag)();
int32_t (*pfGetTraceMetaDataParsingFlag)(int *flag);
int32_t (*pfSetTraceMetaDataParsingFlag)(int flag);
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
bool (*pfIsEnabledVersionFlag)();
int32_t (*pfGetVersionFlag)(int *flag);
int32_t (*pfSetVersionFlag)(int flag);
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
bool (*pfIsEnabledDynamicRegexMruListSizeFlag)();
int32_t (*pfGetDynamicRegexMruListSizeFlag)(int *flag);
int32_t (*pfSetDynamicRegexMruListSizeFlag)(int flag);
#endif

bool (*pfIsEnabledOptimizeForManyInstancesFlag)();
int32_t (*pfGetOptimizeForManyInstancesFlag)(bool *flag);
int32_t (*pfSetOptimizeForManyInstancesFlag)(bool flag);
bool (*pfIsEnabledEnableArrayTypeMutationFlag)();
int32_t (*pfGetEnableArrayTypeMutationFlag)(bool *flag);
int32_t (*pfSetEnableArrayTypeMutationFlag)(bool flag);
bool (*pfIsEnabledArrayMutationTestSeedFlag)();
int32_t (*pfGetArrayMutationTestSeedFlag)(int *flag);
int32_t (*pfSetArrayMutationTestSeedFlag)(int flag);
bool (*pfIsEnabledEnableEvalMapCleanupFlag)();
int32_t (*pfGetEnableEvalMapCleanupFlag)(bool *flag);
int32_t (*pfSetEnableEvalMapCleanupFlag)(bool flag);
#ifdef PROFILE_MEM
bool (*pfIsEnabledTraceObjectAllocationFlag)();
int32_t (*pfGetTraceObjectAllocationFlag)(bool *flag);
int32_t (*pfSetTraceObjectAllocationFlag)(bool flag);
#endif
bool (*pfIsEnabledSseFlag)();
int32_t (*pfGetSseFlag)(int *flag);
int32_t (*pfSetSseFlag)(int flag);
bool (*pfIsEnabledDeletedPropertyReuseThresholdFlag)();
int32_t (*pfGetDeletedPropertyReuseThresholdFlag)(int *flag);
int32_t (*pfSetDeletedPropertyReuseThresholdFlag)(int flag);
bool (*pfIsEnabledForceStringKeyedSimpleDictionaryTypeHandlerFlag)();
int32_t (*pfGetForceStringKeyedSimpleDictionaryTypeHandlerFlag)(bool *flag);
int32_t (*pfSetForceStringKeyedSimpleDictionaryTypeHandlerFlag)(bool flag);
bool (*pfIsEnabledBigDictionaryTypeHandlerThresholdFlag)();
int32_t (*pfGetBigDictionaryTypeHandlerThresholdFlag)(int *flag);
int32_t (*pfSetBigDictionaryTypeHandlerThresholdFlag)(int flag);
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
bool (*pfIsEnabledPerfHintLevelFlag)();
int32_t (*pfGetPerfHintLevelFlag)(int *flag);
int32_t (*pfSetPerfHintLevelFlag)(int flag);
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
bool (*pfIsEnabledMemProtectHeapFlag)();
int32_t (*pfGetMemProtectHeapFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapFlag)(bool flag);
#endif
#ifdef RECYCLER_STRESS
bool (*pfIsEnabledMemProtectHeapStressFlag)();
int32_t (*pfGetMemProtectHeapStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapBackgroundStressFlag)();
int32_t (*pfGetMemProtectHeapBackgroundStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapBackgroundStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapConcurrentStressFlag)();
int32_t (*pfGetMemProtectHeapConcurrentStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapConcurrentStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapConcurrentRepeatStressFlag)();
int32_t (*pfGetMemProtectHeapConcurrentRepeatStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapConcurrentRepeatStressFlag)(bool flag);
bool (*pfIsEnabledMemProtectHeapPartialStressFlag)();
int32_t (*pfGetMemProtectHeapPartialStressFlag)(bool *flag);
int32_t (*pfSetMemProtectHeapPartialStressFlag)(bool flag);
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
bool (*pfIsEnabledFixPropsOnPathTypesFlag)();
int32_t (*pfGetFixPropsOnPathTypesFlag)(bool *flag);
int32_t (*pfSetFixPropsOnPathTypesFlag)(bool flag);
#endif

// recycler heuristic flags
bool (*pfIsEnabledMaxBackgroundFinishMarkCountFlag)();
int32_t (*pfGetMaxBackgroundFinishMarkCountFlag)(int *flag);
int32_t (*pfSetMaxBackgroundFinishMarkCountFlag)(int flag);
bool (*pfIsEnabledBackgroundFinishMarkWaitTimeFlag)();
int32_t (*pfGetBackgroundFinishMarkWaitTimeFlag)(int *flag);
int32_t (*pfSetBackgroundFinishMarkWaitTimeFlag)(int flag);
bool (*pfIsEnabledMinBackgroundRepeatMarkRescanBytesFlag)();
int32_t (*pfGetMinBackgroundRepeatMarkRescanBytesFlag)(int *flag);
int32_t (*pfSetMinBackgroundRepeatMarkRescanBytesFlag)(int flag);

#if defined(_M_X64)
bool (*pfIsEnabledZeroMemoryWithNonTemporalStoreFlag)();
int32_t (*pfGetZeroMemoryWithNonTemporalStoreFlag)(bool *flag);
int32_t (*pfSetZeroMemoryWithNonTemporalStoreFlag)(bool flag);
#endif

// recycler memory restrict test flags
bool (*pfIsEnabledMaxMarkStackPageCountFlag)();
int32_t (*pfGetMaxMarkStackPageCountFlag)(int *flag);
int32_t (*pfSetMaxMarkStackPageCountFlag)(int flag);
bool (*pfIsEnabledMaxTrackedObjectListCountFlag)();
int32_t (*pfGetMaxTrackedObjectListCountFlag)(int *flag);
int32_t (*pfSetMaxTrackedObjectListCountFlag)(int flag);

// make the recycler page integration path easier to hit
bool (*pfIsEnabledNumberAllocPlusSizeFlag)();
int32_t (*pfGetNumberAllocPlusSizeFlag)(int *flag);
int32_t (*pfSetNumberAllocPlusSizeFlag)(int flag);

#if DBG
bool (*pfIsEnabledInitializeInterpreterSlotsWithInvalidStackVarFlag)();
int32_t (*pfGetInitializeInterpreterSlotsWithInvalidStackVarFlag)(bool *flag);
int32_t (*pfSetInitializeInterpreterSlotsWithInvalidStackVarFlag)(bool flag);
#endif

#if DBG
bool (*pfIsEnabledPRNGSeed0Flag)();
int32_t (*pfGetPRNGSeed0Flag)(int *flag);
int32_t (*pfSetPRNGSeed0Flag)(int flag);
bool (*pfIsEnabledPRNGSeed1Flag)();
int32_t (*pfGetPRNGSeed1Flag)(int *flag);
int32_t (*pfSetPRNGSeed1Flag)(int flag);
#endif

bool (*pfIsEnabledClearInlineCachesOnCollectFlag)();
int32_t (*pfGetClearInlineCachesOnCollectFlag)(bool *flag);
int32_t (*pfSetClearInlineCachesOnCollectFlag)(bool flag);
bool (*pfIsEnabledInlineCacheInvalidationListCompactionThresholdFlag)();
int32_t (*pfGetInlineCacheInvalidationListCompactionThresholdFlag)(int *flag);
int32_t (*pfSetInlineCacheInvalidationListCompactionThresholdFlag)(int flag);
bool (*pfIsEnabledConstructorCacheInvalidationThresholdFlag)();
int32_t (*pfGetConstructorCacheInvalidationThresholdFlag)(int *flag);
int32_t (*pfSetConstructorCacheInvalidationThresholdFlag)(int flag);

bool (*pfIsEnabledGCMemoryThresholdFlag)();
int32_t (*pfGetGCMemoryThresholdFlag)(int *flag);
int32_t (*pfSetGCMemoryThresholdFlag)(int flag);

#if DBG
    bool (*pfIsEnabledSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag)();
int32_t (*pfGetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag)(int *flag);
int32_t (*pfSetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag)(int flag);
#endif

bool (*pfIsEnabledJITServerIdleTimeoutFlag)();
int32_t (*pfGetJITServerIdleTimeoutFlag)(int *flag);
int32_t (*pfSetJITServerIdleTimeoutFlag)(int flag);
bool (*pfIsEnabledJITServerMaxInactivePageAllocatorCountFlag)();
int32_t (*pfGetJITServerMaxInactivePageAllocatorCountFlag)(int *flag);
int32_t (*pfSetJITServerMaxInactivePageAllocatorCountFlag)(int flag);

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

bool (*pfIsEnabledMaxSingleAllocSizeInMBFlag)();
int32_t (*pfGetMaxSingleAllocSizeInMBFlag)(int *flag);
int32_t (*pfSetMaxSingleAllocSizeInMBFlag)(int flag);
};

int32_t OnChakraCoreLoaded();

