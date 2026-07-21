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

#if DBG
bool IsEnabledArrayValidateFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ArrayValidateFlag);
}
int32_t GetArrayValidateFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ArrayValidate;
    return S_OK;
}
int32_t SetArrayValidateFlag(bool flag)
{
    Js::Configuration::Global.flags.ArrayValidate = flag;
    return S_OK;
}

#endif
bool IsEnabledAsmJsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AsmJsFlag);
}
int32_t GetAsmJsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.AsmJs;
    return S_OK;
}
int32_t SetAsmJsFlag(bool flag)
{
    Js::Configuration::Global.flags.AsmJs = flag;
    return S_OK;
}
bool IsEnabledWasmFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmFlag);
}
int32_t GetWasmFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Wasm;
    return S_OK;
}
int32_t SetWasmFlag(bool flag)
{
    Js::Configuration::Global.flags.Wasm = flag;
    return S_OK;
}
bool IsEnabledWasmI64Flag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmI64Flag);
}
int32_t GetWasmI64Flag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmI64;
    return S_OK;
}
int32_t SetWasmI64Flag(bool flag)
{
    Js::Configuration::Global.flags.WasmI64 = flag;
    return S_OK;
}
bool IsEnabledWasmFastArrayFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmFastArrayFlag);
}
int32_t GetWasmFastArrayFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmFastArray;
    return S_OK;
}
int32_t SetWasmFastArrayFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmFastArray = flag;
    return S_OK;
}
bool IsEnabledWasmSharedArrayVirtualBufferFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmSharedArrayVirtualBufferFlag);
}
int32_t GetWasmSharedArrayVirtualBufferFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmSharedArrayVirtualBuffer;
    return S_OK;
}
int32_t SetWasmSharedArrayVirtualBufferFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmSharedArrayVirtualBuffer = flag;
    return S_OK;
}
bool IsEnabledWasmMathExFilterFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmMathExFilterFlag);
}
int32_t GetWasmMathExFilterFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmMathExFilter;
    return S_OK;
}
int32_t SetWasmMathExFilterFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmMathExFilter = flag;
    return S_OK;
}
bool IsEnabledWasmCheckVersionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmCheckVersionFlag);
}
int32_t GetWasmCheckVersionFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmCheckVersion;
    return S_OK;
}
int32_t SetWasmCheckVersionFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmCheckVersion = flag;
    return S_OK;
}
bool IsEnabledWasmAssignModuleIDFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmAssignModuleIDFlag);
}
int32_t GetWasmAssignModuleIDFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmAssignModuleID;
    return S_OK;
}
int32_t SetWasmAssignModuleIDFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmAssignModuleID = flag;
    return S_OK;
}
bool IsEnabledWasmIgnoreResponseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmIgnoreResponseFlag);
}
int32_t GetWasmIgnoreResponseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmIgnoreResponse;
    return S_OK;
}
int32_t SetWasmIgnoreResponseFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmIgnoreResponse = flag;
    return S_OK;
}
bool IsEnabledWasmMaxTableSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmMaxTableSizeFlag);
}
int32_t GetWasmMaxTableSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.WasmMaxTableSize;
    return S_OK;
}
int32_t SetWasmMaxTableSizeFlag(int flag)
{
    Js::Configuration::Global.flags.WasmMaxTableSize = flag;
    return S_OK;
}
bool IsEnabledWasmThreadsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmThreadsFlag);
}
int32_t GetWasmThreadsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmThreads;
    return S_OK;
}
int32_t SetWasmThreadsFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmThreads = flag;
    return S_OK;
}
bool IsEnabledWasmMultiValueFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmMultiValueFlag);
}
int32_t GetWasmMultiValueFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmMultiValue;
    return S_OK;
}
int32_t SetWasmMultiValueFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmMultiValue = flag;
    return S_OK;
}
bool IsEnabledWasmSignExtendsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmSignExtendsFlag);
}
int32_t GetWasmSignExtendsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmSignExtends;
    return S_OK;
}
int32_t SetWasmSignExtendsFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmSignExtends = flag;
    return S_OK;
}
bool IsEnabledWasmNontrappingFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmNontrappingFlag);
}
int32_t GetWasmNontrappingFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmNontrapping;
    return S_OK;
}
int32_t SetWasmNontrappingFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmNontrapping = flag;
    return S_OK;
}

// WebAssembly Experimental Features

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
bool IsEnabledWasmSimdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WasmSimdFlag);
}
int32_t GetWasmSimdFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WasmSimd;
    return S_OK;
}
int32_t SetWasmSimdFlag(bool flag)
{
    Js::Configuration::Global.flags.WasmSimd = flag;
    return S_OK;
}

bool IsEnabledBailOnNoProfileLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BailOnNoProfileLimitFlag);
}
int32_t GetBailOnNoProfileLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BailOnNoProfileLimit;
    return S_OK;
}
int32_t SetBailOnNoProfileLimitFlag(int flag)
{
    Js::Configuration::Global.flags.BailOnNoProfileLimit = flag;
    return S_OK;
}
bool IsEnabledBailOnNoProfileRejitLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BailOnNoProfileRejitLimitFlag);
}
int32_t GetBailOnNoProfileRejitLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BailOnNoProfileRejitLimit;
    return S_OK;
}
int32_t SetBailOnNoProfileRejitLimitFlag(int flag)
{
    Js::Configuration::Global.flags.BailOnNoProfileRejitLimit = flag;
    return S_OK;
}
bool IsEnabledBaselineModeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BaselineModeFlag);
}
int32_t GetBaselineModeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BaselineMode;
    return S_OK;
}
int32_t SetBaselineModeFlag(bool flag)
{
    Js::Configuration::Global.flags.BaselineMode = flag;
    return S_OK;
}
bool IsEnabledDumpOnCrashFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpOnCrashFlag);
}

int32_t GetDumpOnCrashFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.DumpOnCrash);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDumpOnCrashFlag(BSTR flag)
{
    Js::Configuration::Global.flags.DumpOnCrash = flag;
    return S_OK;
}
bool IsEnabledFullMemoryDumpFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FullMemoryDumpFlag);
}

int32_t GetFullMemoryDumpFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.FullMemoryDump);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetFullMemoryDumpFlag(BSTR flag)
{
    Js::Configuration::Global.flags.FullMemoryDump = flag;
    return S_OK;
}
bool IsEnabledBailOutAtEveryLineFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BailOutAtEveryLineFlag);
}
int32_t GetBailOutAtEveryLineFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BailOutAtEveryLine;
    return S_OK;
}
int32_t SetBailOutAtEveryLineFlag(bool flag)
{
    Js::Configuration::Global.flags.BailOutAtEveryLine = flag;
    return S_OK;
}
bool IsEnabledBailOutAtEveryByteCodeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BailOutAtEveryByteCodeFlag);
}
int32_t GetBailOutAtEveryByteCodeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BailOutAtEveryByteCode;
    return S_OK;
}
int32_t SetBailOutAtEveryByteCodeFlag(bool flag)
{
    Js::Configuration::Global.flags.BailOutAtEveryByteCode = flag;
    return S_OK;
}
bool IsEnabledBailOutAtEveryImplicitCallFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BailOutAtEveryImplicitCallFlag);
}
int32_t GetBailOutAtEveryImplicitCallFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BailOutAtEveryImplicitCall;
    return S_OK;
}
int32_t SetBailOutAtEveryImplicitCallFlag(bool flag)
{
    Js::Configuration::Global.flags.BailOutAtEveryImplicitCall = flag;
    return S_OK;
}
bool IsEnabledBenchmarkFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BenchmarkFlag);
}
int32_t GetBenchmarkFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Benchmark;
    return S_OK;
}
int32_t SetBenchmarkFlag(bool flag)
{
    Js::Configuration::Global.flags.Benchmark = flag;
    return S_OK;
}
bool IsEnabledBgJitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BgJitFlag);
}
int32_t GetBgJitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BgJit;
    return S_OK;
}
int32_t SetBgJitFlag(bool flag)
{
    Js::Configuration::Global.flags.BgJit = flag;
    return S_OK;
}
bool IsEnabledBgParseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BgParseFlag);
}
int32_t GetBgParseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BgParse;
    return S_OK;
}
int32_t SetBgParseFlag(bool flag)
{
    Js::Configuration::Global.flags.BgParse = flag;
    return S_OK;
}
bool IsEnabledBgJitDelayFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BgJitDelayFlag);
}
int32_t GetBgJitDelayFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BgJitDelay;
    return S_OK;
}
int32_t SetBgJitDelayFlag(int flag)
{
    Js::Configuration::Global.flags.BgJitDelay = flag;
    return S_OK;
}
bool IsEnabledBgJitDelayFgBufferFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BgJitDelayFgBufferFlag);
}
int32_t GetBgJitDelayFgBufferFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BgJitDelayFgBuffer;
    return S_OK;
}
int32_t SetBgJitDelayFgBufferFlag(int flag)
{
    Js::Configuration::Global.flags.BgJitDelayFgBuffer = flag;
    return S_OK;
}
bool IsEnabledBgJitPendingFuncCapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BgJitPendingFuncCapFlag);
}
int32_t GetBgJitPendingFuncCapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BgJitPendingFuncCap;
    return S_OK;
}
int32_t SetBgJitPendingFuncCapFlag(int flag)
{
    Js::Configuration::Global.flags.BgJitPendingFuncCap = flag;
    return S_OK;
}

bool IsEnabledCreateFunctionProxyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CreateFunctionProxyFlag);
}
int32_t GetCreateFunctionProxyFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CreateFunctionProxy;
    return S_OK;
}
int32_t SetCreateFunctionProxyFlag(bool flag)
{
    Js::Configuration::Global.flags.CreateFunctionProxy = flag;
    return S_OK;
}
bool IsEnabledHybridFgJitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::HybridFgJitFlag);
}
int32_t GetHybridFgJitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.HybridFgJit;
    return S_OK;
}
int32_t SetHybridFgJitFlag(bool flag)
{
    Js::Configuration::Global.flags.HybridFgJit = flag;
    return S_OK;
}
bool IsEnabledHybridFgJitBgQueueLengthThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::HybridFgJitBgQueueLengthThresholdFlag);
}
int32_t GetHybridFgJitBgQueueLengthThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.HybridFgJitBgQueueLengthThreshold;
    return S_OK;
}
int32_t SetHybridFgJitBgQueueLengthThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.HybridFgJitBgQueueLengthThreshold = flag;
    return S_OK;
}
bool IsEnabledBytecodeHistFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BytecodeHistFlag);
}
int32_t GetBytecodeHistFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.BytecodeHist;
    return S_OK;
}
int32_t SetBytecodeHistFlag(bool flag)
{
    Js::Configuration::Global.flags.BytecodeHist = flag;
    return S_OK;
}
bool IsEnabledCurrentSourceInfoFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CurrentSourceInfoFlag);
}
int32_t GetCurrentSourceInfoFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CurrentSourceInfo;
    return S_OK;
}
int32_t SetCurrentSourceInfoFlag(bool flag)
{
    Js::Configuration::Global.flags.CurrentSourceInfo = flag;
    return S_OK;
}
bool IsEnabledCFGLogFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CFGLogFlag);
}
int32_t GetCFGLogFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CFGLog;
    return S_OK;
}
int32_t SetCFGLogFlag(bool flag)
{
    Js::Configuration::Global.flags.CFGLog = flag;
    return S_OK;
}
bool IsEnabledCheckAlignmentFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CheckAlignmentFlag);
}
int32_t GetCheckAlignmentFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CheckAlignment;
    return S_OK;
}
int32_t SetCheckAlignmentFlag(bool flag)
{
    Js::Configuration::Global.flags.CheckAlignment = flag;
    return S_OK;
}
bool IsEnabledCheckEmitBufferPermissionsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CheckEmitBufferPermissionsFlag);
}
int32_t GetCheckEmitBufferPermissionsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CheckEmitBufferPermissions;
    return S_OK;
}
int32_t SetCheckEmitBufferPermissionsFlag(bool flag)
{
    Js::Configuration::Global.flags.CheckEmitBufferPermissions = flag;
    return S_OK;
}
#ifdef CHECK_MEMORY_LEAK
bool IsEnabledCheckMemoryLeakFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CheckMemoryLeakFlag);
}
int32_t GetCheckMemoryLeakFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CheckMemoryLeak;
    return S_OK;
}
int32_t SetCheckMemoryLeakFlag(bool flag)
{
    Js::Configuration::Global.flags.CheckMemoryLeak = flag;
    return S_OK;
}
bool IsEnabledDumpOnLeakFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpOnLeakFlag);
}

int32_t GetDumpOnLeakFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.DumpOnLeak);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDumpOnLeakFlag(BSTR flag)
{
    Js::Configuration::Global.flags.DumpOnLeak = flag;
    return S_OK;
}
#endif
bool IsEnabledCheckOpHelpersFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CheckOpHelpersFlag);
}
int32_t GetCheckOpHelpersFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CheckOpHelpers;
    return S_OK;
}
int32_t SetCheckOpHelpersFlag(bool flag)
{
    Js::Configuration::Global.flags.CheckOpHelpers = flag;
    return S_OK;
}
bool IsEnabledCloneInlinedPolymorphicCachesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CloneInlinedPolymorphicCachesFlag);
}
int32_t GetCloneInlinedPolymorphicCachesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CloneInlinedPolymorphicCaches;
    return S_OK;
}
int32_t SetCloneInlinedPolymorphicCachesFlag(bool flag)
{
    Js::Configuration::Global.flags.CloneInlinedPolymorphicCaches = flag;
    return S_OK;
}
bool IsEnabledConcurrentRuntimeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ConcurrentRuntimeFlag);
}
int32_t GetConcurrentRuntimeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ConcurrentRuntime;
    return S_OK;
}
int32_t SetConcurrentRuntimeFlag(bool flag)
{
    Js::Configuration::Global.flags.ConcurrentRuntime = flag;
    return S_OK;
}
bool IsEnabledConstructorInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ConstructorInlineThresholdFlag);
}
int32_t GetConstructorInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ConstructorInlineThreshold;
    return S_OK;
}
int32_t SetConstructorInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.ConstructorInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledConstructorCallsRequiredToFinalizeCachedTypeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ConstructorCallsRequiredToFinalizeCachedTypeFlag);
}
int32_t GetConstructorCallsRequiredToFinalizeCachedTypeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ConstructorCallsRequiredToFinalizeCachedType;
    return S_OK;
}
int32_t SetConstructorCallsRequiredToFinalizeCachedTypeFlag(int flag)
{
    Js::Configuration::Global.flags.ConstructorCallsRequiredToFinalizeCachedType = flag;
    return S_OK;
}
bool IsEnabledPropertyCacheMissPenaltyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PropertyCacheMissPenaltyFlag);
}
int32_t GetPropertyCacheMissPenaltyFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PropertyCacheMissPenalty;
    return S_OK;
}
int32_t SetPropertyCacheMissPenaltyFlag(int flag)
{
    Js::Configuration::Global.flags.PropertyCacheMissPenalty = flag;
    return S_OK;
}
bool IsEnabledPropertyCacheMissThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PropertyCacheMissThresholdFlag);
}
int32_t GetPropertyCacheMissThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PropertyCacheMissThreshold;
    return S_OK;
}
int32_t SetPropertyCacheMissThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.PropertyCacheMissThreshold = flag;
    return S_OK;
}
bool IsEnabledPropertyCacheMissResetFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PropertyCacheMissResetFlag);
}
int32_t GetPropertyCacheMissResetFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PropertyCacheMissReset;
    return S_OK;
}
int32_t SetPropertyCacheMissResetFlag(int flag)
{
    Js::Configuration::Global.flags.PropertyCacheMissReset = flag;
    return S_OK;
}
bool IsEnabledDebugFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DebugFlag);
}
int32_t GetDebugFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Debug;
    return S_OK;
}
int32_t SetDebugFlag(bool flag)
{
    Js::Configuration::Global.flags.Debug = flag;
    return S_OK;
}

bool IsEnabledDebugWindowFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DebugWindowFlag);
}
int32_t GetDebugWindowFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DebugWindow;
    return S_OK;
}
int32_t SetDebugWindowFlag(bool flag)
{
    Js::Configuration::Global.flags.DebugWindow = flag;
    return S_OK;
}
bool IsEnabledParserStateCacheFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ParserStateCacheFlag);
}
int32_t GetParserStateCacheFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ParserStateCache;
    return S_OK;
}
int32_t SetParserStateCacheFlag(bool flag)
{
    Js::Configuration::Global.flags.ParserStateCache = flag;
    return S_OK;
}
bool IsEnabledCompressParserStateCacheFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CompressParserStateCacheFlag);
}
int32_t GetCompressParserStateCacheFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CompressParserStateCache;
    return S_OK;
}
int32_t SetCompressParserStateCacheFlag(bool flag)
{
    Js::Configuration::Global.flags.CompressParserStateCache = flag;
    return S_OK;
}
bool IsEnabledDeferTopLevelTillFirstCallFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DeferTopLevelTillFirstCallFlag);
}
int32_t GetDeferTopLevelTillFirstCallFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DeferTopLevelTillFirstCall;
    return S_OK;
}
int32_t SetDeferTopLevelTillFirstCallFlag(bool flag)
{
    Js::Configuration::Global.flags.DeferTopLevelTillFirstCall = flag;
    return S_OK;
}
bool IsEnabledDeferParseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DeferParseFlag);
}
int32_t GetDeferParseFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.DeferParse;
    return S_OK;
}
int32_t SetDeferParseFlag(int flag)
{
    Js::Configuration::Global.flags.DeferParse = flag;
    return S_OK;
}
bool IsEnabledDirectCallTelemetryStatsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DirectCallTelemetryStatsFlag);
}
int32_t GetDirectCallTelemetryStatsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DirectCallTelemetryStats;
    return S_OK;
}
int32_t SetDirectCallTelemetryStatsFlag(bool flag)
{
    Js::Configuration::Global.flags.DirectCallTelemetryStats = flag;
    return S_OK;
}
bool IsEnabledDisableArrayBTreeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DisableArrayBTreeFlag);
}
int32_t GetDisableArrayBTreeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DisableArrayBTree;
    return S_OK;
}
int32_t SetDisableArrayBTreeFlag(bool flag)
{
    Js::Configuration::Global.flags.DisableArrayBTree = flag;
    return S_OK;
}
bool IsEnabledDisableRentalThreadingFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DisableRentalThreadingFlag);
}
int32_t GetDisableRentalThreadingFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DisableRentalThreading;
    return S_OK;
}
int32_t SetDisableRentalThreadingFlag(bool flag)
{
    Js::Configuration::Global.flags.DisableRentalThreading = flag;
    return S_OK;
}
bool IsEnabledDisableVTuneSourceLineInfoFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DisableVTuneSourceLineInfoFlag);
}
int32_t GetDisableVTuneSourceLineInfoFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DisableVTuneSourceLineInfo;
    return S_OK;
}
int32_t SetDisableVTuneSourceLineInfoFlag(bool flag)
{
    Js::Configuration::Global.flags.DisableVTuneSourceLineInfo = flag;
    return S_OK;
}
bool IsEnabledDisplayMemStatsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DisplayMemStatsFlag);
}
int32_t GetDisplayMemStatsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DisplayMemStats;
    return S_OK;
}
int32_t SetDisplayMemStatsFlag(bool flag)
{
    Js::Configuration::Global.flags.DisplayMemStats = flag;
    return S_OK;
}
#ifdef DUMP_FRAGMENTATION_STATS
bool IsEnabledDumpFragmentationStatsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpFragmentationStatsFlag);
}
int32_t GetDumpFragmentationStatsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpFragmentationStats;
    return S_OK;
}
int32_t SetDumpFragmentationStatsFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpFragmentationStats = flag;
    return S_OK;
}
#endif
bool IsEnabledDumpIRAddressesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpIRAddressesFlag);
}
int32_t GetDumpIRAddressesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpIRAddresses;
    return S_OK;
}
int32_t SetDumpIRAddressesFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpIRAddresses = flag;
    return S_OK;
}
bool IsEnabledDumpLineNoInColorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpLineNoInColorFlag);
}
int32_t GetDumpLineNoInColorFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpLineNoInColor;
    return S_OK;
}
int32_t SetDumpLineNoInColorFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpLineNoInColor = flag;
    return S_OK;
}
#ifdef RECYCLER_DUMP_OBJECT_GRAPH
bool IsEnabledDumpObjectGraphOnExitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpObjectGraphOnExitFlag);
}
int32_t GetDumpObjectGraphOnExitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpObjectGraphOnExit;
    return S_OK;
}
int32_t SetDumpObjectGraphOnExitFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpObjectGraphOnExit = flag;
    return S_OK;
}
bool IsEnabledDumpObjectGraphOnCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpObjectGraphOnCollectFlag);
}
int32_t GetDumpObjectGraphOnCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpObjectGraphOnCollect;
    return S_OK;
}
int32_t SetDumpObjectGraphOnCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpObjectGraphOnCollect = flag;
    return S_OK;
}
#endif
bool IsEnabledDumpEvalStringOnRemovalFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpEvalStringOnRemovalFlag);
}
int32_t GetDumpEvalStringOnRemovalFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpEvalStringOnRemoval;
    return S_OK;
}
int32_t SetDumpEvalStringOnRemovalFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpEvalStringOnRemoval = flag;
    return S_OK;
}
bool IsEnabledDumpObjectGraphOnEnumFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpObjectGraphOnEnumFlag);
}
int32_t GetDumpObjectGraphOnEnumFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpObjectGraphOnEnum;
    return S_OK;
}
int32_t SetDumpObjectGraphOnEnumFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpObjectGraphOnEnum = flag;
    return S_OK;
}
#ifdef DYNAMIC_PROFILE_STORAGE
bool IsEnabledDynamicProfileCacheFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DynamicProfileCacheFlag);
}

int32_t GetDynamicProfileCacheFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.DynamicProfileCache);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDynamicProfileCacheFlag(BSTR flag)
{
    Js::Configuration::Global.flags.DynamicProfileCache = flag;
    return S_OK;
}
bool IsEnabledDpcFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DpcFlag);
}

int32_t GetDpcFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.Dpc);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDpcFlag(BSTR flag)
{
    Js::Configuration::Global.flags.Dpc = flag;
    return S_OK;
}
bool IsEnabledDynamicProfileCacheDirFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DynamicProfileCacheDirFlag);
}

int32_t GetDynamicProfileCacheDirFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.DynamicProfileCacheDir);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDynamicProfileCacheDirFlag(BSTR flag)
{
    Js::Configuration::Global.flags.DynamicProfileCacheDir = flag;
    return S_OK;
}
bool IsEnabledDynamicProfileInputFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DynamicProfileInputFlag);
}

int32_t GetDynamicProfileInputFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.DynamicProfileInput);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDynamicProfileInputFlag(BSTR flag)
{
    Js::Configuration::Global.flags.DynamicProfileInput = flag;
    return S_OK;
}
bool IsEnabledDpiFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DpiFlag);
}

int32_t GetDpiFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.Dpi);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetDpiFlag(BSTR flag)
{
    Js::Configuration::Global.flags.Dpi = flag;
    return S_OK;
}
#endif
#ifdef EDIT_AND_CONTINUE
bool IsEnabledEditTestFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EditTestFlag);
}
int32_t GetEditTestFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EditTest;
    return S_OK;
}
int32_t SetEditTestFlag(bool flag)
{
    Js::Configuration::Global.flags.EditTest = flag;
    return S_OK;
}
#endif
bool IsEnabledWininetProfileCacheFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WininetProfileCacheFlag);
}
int32_t GetWininetProfileCacheFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WininetProfileCache;
    return S_OK;
}
int32_t SetWininetProfileCacheFlag(bool flag)
{
    Js::Configuration::Global.flags.WininetProfileCache = flag;
    return S_OK;
}
bool IsEnabledNoDynamicProfileInMemoryCacheFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NoDynamicProfileInMemoryCacheFlag);
}
int32_t GetNoDynamicProfileInMemoryCacheFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NoDynamicProfileInMemoryCache;
    return S_OK;
}
int32_t SetNoDynamicProfileInMemoryCacheFlag(bool flag)
{
    Js::Configuration::Global.flags.NoDynamicProfileInMemoryCache = flag;
    return S_OK;
}
bool IsEnabledProfileBasedSpeculativeJitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileBasedSpeculativeJitFlag);
}
int32_t GetProfileBasedSpeculativeJitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileBasedSpeculativeJit;
    return S_OK;
}
int32_t SetProfileBasedSpeculativeJitFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileBasedSpeculativeJit = flag;
    return S_OK;
}
bool IsEnabledProfileBasedSpeculationCapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileBasedSpeculationCapFlag);
}
int32_t GetProfileBasedSpeculationCapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileBasedSpeculationCap;
    return S_OK;
}
int32_t SetProfileBasedSpeculationCapFlag(int flag)
{
    Js::Configuration::Global.flags.ProfileBasedSpeculationCap = flag;
    return S_OK;
}
bool IsEnabledExecuteByteCodeBufferReturnsInvalidByteCodeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ExecuteByteCodeBufferReturnsInvalidByteCodeFlag);
}
int32_t GetExecuteByteCodeBufferReturnsInvalidByteCodeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ExecuteByteCodeBufferReturnsInvalidByteCode;
    return S_OK;
}
int32_t SetExecuteByteCodeBufferReturnsInvalidByteCodeFlag(bool flag)
{
    Js::Configuration::Global.flags.ExecuteByteCodeBufferReturnsInvalidByteCode = flag;
    return S_OK;
}
bool IsEnabledExpirableCollectionGCCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ExpirableCollectionGCCountFlag);
}
int32_t GetExpirableCollectionGCCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ExpirableCollectionGCCount;
    return S_OK;
}
int32_t SetExpirableCollectionGCCountFlag(int flag)
{
    Js::Configuration::Global.flags.ExpirableCollectionGCCount = flag;
    return S_OK;
}
bool IsEnabledExpirableCollectionTriggerThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ExpirableCollectionTriggerThresholdFlag);
}
int32_t GetExpirableCollectionTriggerThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ExpirableCollectionTriggerThreshold;
    return S_OK;
}
int32_t SetExpirableCollectionTriggerThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.ExpirableCollectionTriggerThreshold = flag;
    return S_OK;
}
bool IsEnabledSkipSplitOnNoResultFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SkipSplitOnNoResultFlag);
}
int32_t GetSkipSplitOnNoResultFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.SkipSplitOnNoResult;
    return S_OK;
}
int32_t SetSkipSplitOnNoResultFlag(bool flag)
{
    Js::Configuration::Global.flags.SkipSplitOnNoResult = flag;
    return S_OK;
}
bool IsEnabledForce32BitByteCodeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::Force32BitByteCodeFlag);
}
int32_t GetForce32BitByteCodeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Force32BitByteCode;
    return S_OK;
}
int32_t SetForce32BitByteCodeFlag(bool flag)
{
    Js::Configuration::Global.flags.Force32BitByteCode = flag;
    return S_OK;
}

bool IsEnabledCollectGarbageFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CollectGarbageFlag);
}
int32_t GetCollectGarbageFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CollectGarbage;
    return S_OK;
}
int32_t SetCollectGarbageFlag(bool flag)
{
    Js::Configuration::Global.flags.CollectGarbage = flag;
    return S_OK;
}

bool IsEnabledIntlFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::IntlFlag);
}
int32_t GetIntlFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Intl;
    return S_OK;
}
int32_t SetIntlFlag(bool flag)
{
    Js::Configuration::Global.flags.Intl = flag;
    return S_OK;
}
bool IsEnabledIntlBuiltInsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::IntlBuiltInsFlag);
}
int32_t GetIntlBuiltInsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.IntlBuiltIns;
    return S_OK;
}
int32_t SetIntlBuiltInsFlag(bool flag)
{
    Js::Configuration::Global.flags.IntlBuiltIns = flag;
    return S_OK;
}
bool IsEnabledIntlPlatformFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::IntlPlatformFlag);
}
int32_t GetIntlPlatformFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.IntlPlatform;
    return S_OK;
}
int32_t SetIntlPlatformFlag(bool flag)
{
    Js::Configuration::Global.flags.IntlPlatform = flag;
    return S_OK;
}

bool IsEnabledJsBuiltInFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JsBuiltInFlag);
}
int32_t GetJsBuiltInFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.JsBuiltIn;
    return S_OK;
}
int32_t SetJsBuiltInFlag(bool flag)
{
    Js::Configuration::Global.flags.JsBuiltIn = flag;
    return S_OK;
}
bool IsEnabledJitReproFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JitReproFlag);
}
int32_t GetJitReproFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.JitRepro;
    return S_OK;
}
int32_t SetJitReproFlag(bool flag)
{
    Js::Configuration::Global.flags.JitRepro = flag;
    return S_OK;
}
bool IsEnabledEntryPointInfoRpcDataFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EntryPointInfoRpcDataFlag);
}
int32_t GetEntryPointInfoRpcDataFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EntryPointInfoRpcData;
    return S_OK;
}
int32_t SetEntryPointInfoRpcDataFlag(bool flag)
{
    Js::Configuration::Global.flags.EntryPointInfoRpcData = flag;
    return S_OK;
}

bool IsEnabledLdChakraLibFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LdChakraLibFlag);
}
int32_t GetLdChakraLibFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.LdChakraLib;
    return S_OK;
}
int32_t SetLdChakraLibFlag(bool flag)
{
    Js::Configuration::Global.flags.LdChakraLib = flag;
    return S_OK;
}
bool IsEnabledTestChakraLibFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TestChakraLibFlag);
}
int32_t GetTestChakraLibFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TestChakraLib;
    return S_OK;
}
int32_t SetTestChakraLibFlag(bool flag)
{
    Js::Configuration::Global.flags.TestChakraLib = flag;
    return S_OK;
}

// ES6 (BLUE+1) features/flags

// Master ES6 flag to enable STABLE ES6 features/flags
bool IsEnabledES6Flag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6Flag);
}
int32_t GetES6Flag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6;
    return S_OK;
}
int32_t SetES6Flag(bool flag)
{
    Js::Configuration::Global.flags.ES6 = flag;
    return S_OK;
}

// Master ES6 flag to enable ALL sub ES6 features/flags
bool IsEnabledES6AllFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6AllFlag);
}
int32_t GetES6AllFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6All;
    return S_OK;
}
int32_t SetES6AllFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6All = flag;
    return S_OK;
}

// Master ES6 flag to enable Threshold ES6 features/flags
bool IsEnabledES6ExperimentalFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ExperimentalFlag);
}
int32_t GetES6ExperimentalFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Experimental;
    return S_OK;
}
int32_t SetES6ExperimentalFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Experimental = flag;
    return S_OK;
}

// Per ES6 feature/flag

bool IsEnabledES7AsyncAwaitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES7AsyncAwaitFlag);
}
int32_t GetES7AsyncAwaitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES7AsyncAwait;
    return S_OK;
}
int32_t SetES7AsyncAwaitFlag(bool flag)
{
    Js::Configuration::Global.flags.ES7AsyncAwait = flag;
    return S_OK;
}
bool IsEnabledES6DateParseFixFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6DateParseFixFlag);
}
int32_t GetES6DateParseFixFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6DateParseFix;
    return S_OK;
}
int32_t SetES6DateParseFixFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6DateParseFix = flag;
    return S_OK;
}
bool IsEnabledES6FunctionNameFullFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6FunctionNameFullFlag);
}
int32_t GetES6FunctionNameFullFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6FunctionNameFull;
    return S_OK;
}
int32_t SetES6FunctionNameFullFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6FunctionNameFull = flag;
    return S_OK;
}
bool IsEnabledES6GeneratorsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6GeneratorsFlag);
}
int32_t GetES6GeneratorsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Generators;
    return S_OK;
}
int32_t SetES6GeneratorsFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Generators = flag;
    return S_OK;
}
bool IsEnabledES7ExponentiationOperatorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES7ExponentiationOperatorFlag);
}
int32_t GetES7ExponentiationOperatorFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES7ExponentiationOperator;
    return S_OK;
}
int32_t SetES7ExponentiationOperatorFlag(bool flag)
{
    Js::Configuration::Global.flags.ES7ExponentiationOperator = flag;
    return S_OK;
}

bool IsEnabledES7ValuesEntriesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES7ValuesEntriesFlag);
}
int32_t GetES7ValuesEntriesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES7ValuesEntries;
    return S_OK;
}
int32_t SetES7ValuesEntriesFlag(bool flag)
{
    Js::Configuration::Global.flags.ES7ValuesEntries = flag;
    return S_OK;
}
bool IsEnabledES7TrailingCommaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES7TrailingCommaFlag);
}
int32_t GetES7TrailingCommaFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES7TrailingComma;
    return S_OK;
}
int32_t SetES7TrailingCommaFlag(bool flag)
{
    Js::Configuration::Global.flags.ES7TrailingComma = flag;
    return S_OK;
}
bool IsEnabledES6IsConcatSpreadableFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6IsConcatSpreadableFlag);
}
int32_t GetES6IsConcatSpreadableFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6IsConcatSpreadable;
    return S_OK;
}
int32_t SetES6IsConcatSpreadableFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6IsConcatSpreadable = flag;
    return S_OK;
}
bool IsEnabledES6MathFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6MathFlag);
}
int32_t GetES6MathFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Math;
    return S_OK;
}
int32_t SetES6MathFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Math = flag;
    return S_OK;
}

bool IsEnabledESDynamicImportFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESDynamicImportFlag);
}
int32_t GetESDynamicImportFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESDynamicImport;
    return S_OK;
}
int32_t SetESDynamicImportFlag(bool flag)
{
    Js::Configuration::Global.flags.ESDynamicImport = flag;
    return S_OK;
}

bool IsEnabledES6ModuleFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ModuleFlag);
}
int32_t GetES6ModuleFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Module;
    return S_OK;
}
int32_t SetES6ModuleFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Module = flag;
    return S_OK;
}
bool IsEnabledES6ObjectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ObjectFlag);
}
int32_t GetES6ObjectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Object;
    return S_OK;
}
int32_t SetES6ObjectFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Object = flag;
    return S_OK;
}
bool IsEnabledES6NumberFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6NumberFlag);
}
int32_t GetES6NumberFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Number;
    return S_OK;
}
int32_t SetES6NumberFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Number = flag;
    return S_OK;
}
bool IsEnabledES6ObjectLiteralsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ObjectLiteralsFlag);
}
int32_t GetES6ObjectLiteralsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6ObjectLiterals;
    return S_OK;
}
int32_t SetES6ObjectLiteralsFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6ObjectLiterals = flag;
    return S_OK;
}
bool IsEnabledES6ProxyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ProxyFlag);
}
int32_t GetES6ProxyFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Proxy;
    return S_OK;
}
int32_t SetES6ProxyFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Proxy = flag;
    return S_OK;
}
bool IsEnabledES6RestFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6RestFlag);
}
int32_t GetES6RestFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Rest;
    return S_OK;
}
int32_t SetES6RestFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Rest = flag;
    return S_OK;
}
bool IsEnabledES6SpreadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6SpreadFlag);
}
int32_t GetES6SpreadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Spread;
    return S_OK;
}
int32_t SetES6SpreadFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Spread = flag;
    return S_OK;
}
bool IsEnabledES6StringFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6StringFlag);
}
int32_t GetES6StringFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6String;
    return S_OK;
}
int32_t SetES6StringFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6String = flag;
    return S_OK;
}
bool IsEnabledES6StringPrototypeFixesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6StringPrototypeFixesFlag);
}
int32_t GetES6StringPrototypeFixesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6StringPrototypeFixes;
    return S_OK;
}
int32_t SetES6StringPrototypeFixesFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6StringPrototypeFixes = flag;
    return S_OK;
}
bool IsEnabledES2018ObjectRestSpreadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES2018ObjectRestSpreadFlag);
}
int32_t GetES2018ObjectRestSpreadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES2018ObjectRestSpread;
    return S_OK;
}
int32_t SetES2018ObjectRestSpreadFlag(bool flag)
{
    Js::Configuration::Global.flags.ES2018ObjectRestSpread = flag;
    return S_OK;
}

bool IsEnabledES6PrototypeChainFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6PrototypeChainFlag);
}
int32_t GetES6PrototypeChainFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6PrototypeChain;
    return S_OK;
}
int32_t SetES6PrototypeChainFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6PrototypeChain = flag;
    return S_OK;
}
bool IsEnabledES6ToPrimitiveFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ToPrimitiveFlag);
}
int32_t GetES6ToPrimitiveFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6ToPrimitive;
    return S_OK;
}
int32_t SetES6ToPrimitiveFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6ToPrimitive = flag;
    return S_OK;
}
bool IsEnabledES6ToLengthFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ToLengthFlag);
}
int32_t GetES6ToLengthFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6ToLength;
    return S_OK;
}
int32_t SetES6ToLengthFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6ToLength = flag;
    return S_OK;
}
bool IsEnabledES6ToStringTagFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6ToStringTagFlag);
}
int32_t GetES6ToStringTagFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6ToStringTag;
    return S_OK;
}
int32_t SetES6ToStringTagFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6ToStringTag = flag;
    return S_OK;
}
bool IsEnabledES6UnicodeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6UnicodeFlag);
}
int32_t GetES6UnicodeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Unicode;
    return S_OK;
}
int32_t SetES6UnicodeFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Unicode = flag;
    return S_OK;
}
bool IsEnabledES6UnicodeVerboseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6UnicodeVerboseFlag);
}
int32_t GetES6UnicodeVerboseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6UnicodeVerbose;
    return S_OK;
}
int32_t SetES6UnicodeVerboseFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6UnicodeVerbose = flag;
    return S_OK;
}
bool IsEnabledES6UnscopablesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6UnscopablesFlag);
}
int32_t GetES6UnscopablesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Unscopables;
    return S_OK;
}
int32_t SetES6UnscopablesFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Unscopables = flag;
    return S_OK;
}
bool IsEnabledES6RegExStickyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6RegExStickyFlag);
}
int32_t GetES6RegExStickyFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6RegExSticky;
    return S_OK;
}
int32_t SetES6RegExStickyFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6RegExSticky = flag;
    return S_OK;
}
bool IsEnabledES2018RegExDotAllFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES2018RegExDotAllFlag);
}
int32_t GetES2018RegExDotAllFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES2018RegExDotAll;
    return S_OK;
}
int32_t SetES2018RegExDotAllFlag(bool flag)
{
    Js::Configuration::Global.flags.ES2018RegExDotAll = flag;
    return S_OK;
}
bool IsEnabledESExportNsAsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESExportNsAsFlag);
}
int32_t GetESExportNsAsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESExportNsAs;
    return S_OK;
}
int32_t SetESExportNsAsFlag(bool flag)
{
    Js::Configuration::Global.flags.ESExportNsAs = flag;
    return S_OK;
}
bool IsEnabledES2018AsyncIterationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES2018AsyncIterationFlag);
}
int32_t GetES2018AsyncIterationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES2018AsyncIteration;
    return S_OK;
}
int32_t SetES2018AsyncIterationFlag(bool flag)
{
    Js::Configuration::Global.flags.ES2018AsyncIteration = flag;
    return S_OK;
}
bool IsEnabledESTopLevelAwaitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESTopLevelAwaitFlag);
}
int32_t GetESTopLevelAwaitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESTopLevelAwait;
    return S_OK;
}
int32_t SetESTopLevelAwaitFlag(bool flag)
{
    Js::Configuration::Global.flags.ESTopLevelAwait = flag;
    return S_OK;
}

bool IsEnabledES6RegExPrototypePropertiesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6RegExPrototypePropertiesFlag);
}
int32_t GetES6RegExPrototypePropertiesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6RegExPrototypeProperties;
    return S_OK;
}
int32_t SetES6RegExPrototypePropertiesFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6RegExPrototypeProperties = flag;
    return S_OK;
}

// When we enable ES6RegExSymbols check all String and Regex built-ins which are inlined in JIT and make sure the helper
// sets implicit call flag before calling into script
// Also, the corresponding helpers in JnHelperMethodList.h should be marked as being reentrant
bool IsEnabledES6RegExSymbolsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6RegExSymbolsFlag);
}
int32_t GetES6RegExSymbolsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6RegExSymbols;
    return S_OK;
}
int32_t SetES6RegExSymbolsFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6RegExSymbols = flag;
    return S_OK;
}

bool IsEnabledES6VerboseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ES6VerboseFlag);
}
int32_t GetES6VerboseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ES6Verbose;
    return S_OK;
}
int32_t SetES6VerboseFlag(bool flag)
{
    Js::Configuration::Global.flags.ES6Verbose = flag;
    return S_OK;
}
bool IsEnabledESObjectGetOwnPropertyDescriptorsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESObjectGetOwnPropertyDescriptorsFlag);
}
int32_t GetESObjectGetOwnPropertyDescriptorsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESObjectGetOwnPropertyDescriptors;
    return S_OK;
}
int32_t SetESObjectGetOwnPropertyDescriptorsFlag(bool flag)
{
    Js::Configuration::Global.flags.ESObjectGetOwnPropertyDescriptors = flag;
    return S_OK;
}

bool IsEnabledESSharedArrayBufferFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESSharedArrayBufferFlag);
}
int32_t GetESSharedArrayBufferFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESSharedArrayBuffer;
    return S_OK;
}
int32_t SetESSharedArrayBufferFlag(bool flag)
{
    Js::Configuration::Global.flags.ESSharedArrayBuffer = flag;
    return S_OK;
}

// Newer language feature flags

// ES BigInt flag
bool IsEnabledESBigIntFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESBigIntFlag);
}
int32_t GetESBigIntFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESBigInt;
    return S_OK;
}
int32_t SetESBigIntFlag(bool flag)
{
    Js::Configuration::Global.flags.ESBigInt = flag;
    return S_OK;
}

// ES Numeric Separator support for numeric constants
bool IsEnabledESNumericSeparatorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESNumericSeparatorFlag);
}
int32_t GetESNumericSeparatorFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESNumericSeparator;
    return S_OK;
}
int32_t SetESNumericSeparatorFlag(bool flag)
{
    Js::Configuration::Global.flags.ESNumericSeparator = flag;
    return S_OK;
}

// ES Nullish coalescing operator support (??)
bool IsEnabledESNullishCoalescingOperatorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESNullishCoalescingOperatorFlag);
}
int32_t GetESNullishCoalescingOperatorFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESNullishCoalescingOperator;
    return S_OK;
}
int32_t SetESNullishCoalescingOperatorFlag(bool flag)
{
    Js::Configuration::Global.flags.ESNullishCoalescingOperator = flag;
    return S_OK;
}

// ES Hashbang support for interpreter directive syntax
bool IsEnabledESHashbangFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESHashbangFlag);
}
int32_t GetESHashbangFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESHashbang;
    return S_OK;
}
int32_t SetESHashbangFlag(bool flag)
{
    Js::Configuration::Global.flags.ESHashbang = flag;
    return S_OK;
}

// ES Symbol.prototype.description flag
bool IsEnabledESSymbolDescriptionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESSymbolDescriptionFlag);
}
int32_t GetESSymbolDescriptionFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESSymbolDescription;
    return S_OK;
}
int32_t SetESSymbolDescriptionFlag(bool flag)
{
    Js::Configuration::Global.flags.ESSymbolDescription = flag;
    return S_OK;
}

bool IsEnabledESArrayFindFromLastFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESArrayFindFromLastFlag);
}
int32_t GetESArrayFindFromLastFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESArrayFindFromLast;
    return S_OK;
}
int32_t SetESArrayFindFromLastFlag(bool flag)
{
    Js::Configuration::Global.flags.ESArrayFindFromLast = flag;
    return S_OK;
}

// ES Promise.any and AggregateError flag
bool IsEnabledESPromiseAnyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESPromiseAnyFlag);
}
int32_t GetESPromiseAnyFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESPromiseAny;
    return S_OK;
}
int32_t SetESPromiseAnyFlag(bool flag)
{
    Js::Configuration::Global.flags.ESPromiseAny = flag;
    return S_OK;
}

// ES import.meta keyword meta-property
bool IsEnabledESImportMetaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESImportMetaFlag);
}
int32_t GetESImportMetaFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESImportMeta;
    return S_OK;
}
int32_t SetESImportMetaFlag(bool flag)
{
    Js::Configuration::Global.flags.ESImportMeta = flag;
    return S_OK;
}

//ES globalThis flag
bool IsEnabledESGlobalThisFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ESGlobalThisFlag);
}
int32_t GetESGlobalThisFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ESGlobalThis;
    return S_OK;
}
int32_t SetESGlobalThisFlag(bool flag)
{
    Js::Configuration::Global.flags.ESGlobalThis = flag;
    return S_OK;
}

// This flag to be removed once JITing generator functions is stable
bool IsEnabledJitES6GeneratorsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JitES6GeneratorsFlag);
}
int32_t GetJitES6GeneratorsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.JitES6Generators;
    return S_OK;
}
int32_t SetJitES6GeneratorsFlag(bool flag)
{
    Js::Configuration::Global.flags.JitES6Generators = flag;
    return S_OK;
}

bool IsEnabledFastLineColumnCalculationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FastLineColumnCalculationFlag);
}
int32_t GetFastLineColumnCalculationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.FastLineColumnCalculation;
    return S_OK;
}
int32_t SetFastLineColumnCalculationFlag(bool flag)
{
    Js::Configuration::Global.flags.FastLineColumnCalculation = flag;
    return S_OK;
}
bool IsEnabledFilenameFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FilenameFlag);
}

int32_t GetFilenameFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.Filename);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetFilenameFlag(BSTR flag)
{
    Js::Configuration::Global.flags.Filename = flag;
    return S_OK;
}
bool IsEnabledFreeRejittedCodeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FreeRejittedCodeFlag);
}
int32_t GetFreeRejittedCodeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.FreeRejittedCode;
    return S_OK;
}
int32_t SetFreeRejittedCodeFlag(bool flag)
{
    Js::Configuration::Global.flags.FreeRejittedCode = flag;
    return S_OK;
}
bool IsEnabledForceGuardPagesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceGuardPagesFlag);
}
int32_t GetForceGuardPagesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceGuardPages;
    return S_OK;
}
int32_t SetForceGuardPagesFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceGuardPages = flag;
    return S_OK;
}
bool IsEnabledPrintGuardPageBoundsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrintGuardPageBoundsFlag);
}
int32_t GetPrintGuardPageBoundsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrintGuardPageBounds;
    return S_OK;
}
int32_t SetPrintGuardPageBoundsFlag(bool flag)
{
    Js::Configuration::Global.flags.PrintGuardPageBounds = flag;
    return S_OK;
}
bool IsEnabledForceLegacyEngineFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceLegacyEngineFlag);
}
int32_t GetForceLegacyEngineFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceLegacyEngine;
    return S_OK;
}
int32_t SetForceLegacyEngineFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceLegacyEngine = flag;
    return S_OK;
}
bool IsEnabledForceArrayBTreeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceArrayBTreeFlag);
}
int32_t GetForceArrayBTreeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceArrayBTree;
    return S_OK;
}
int32_t SetForceArrayBTreeFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceArrayBTree = flag;
    return S_OK;
}
bool IsEnabledStrongArraySortFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::StrongArraySortFlag);
}
int32_t GetStrongArraySortFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.StrongArraySort;
    return S_OK;
}
int32_t SetStrongArraySortFlag(bool flag)
{
    Js::Configuration::Global.flags.StrongArraySort = flag;
    return S_OK;
}
bool IsEnabledForceCleanPropertyOnCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceCleanPropertyOnCollectFlag);
}
int32_t GetForceCleanPropertyOnCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceCleanPropertyOnCollect;
    return S_OK;
}
int32_t SetForceCleanPropertyOnCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceCleanPropertyOnCollect = flag;
    return S_OK;
}
bool IsEnabledForceCleanCacheOnCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceCleanCacheOnCollectFlag);
}
int32_t GetForceCleanCacheOnCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceCleanCacheOnCollect;
    return S_OK;
}
int32_t SetForceCleanCacheOnCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceCleanCacheOnCollect = flag;
    return S_OK;
}
bool IsEnabledForceGCAfterJSONParseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceGCAfterJSONParseFlag);
}
int32_t GetForceGCAfterJSONParseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceGCAfterJSONParse;
    return S_OK;
}
int32_t SetForceGCAfterJSONParseFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceGCAfterJSONParse = flag;
    return S_OK;
}
bool IsEnabledForceDecommitOnCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceDecommitOnCollectFlag);
}
int32_t GetForceDecommitOnCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceDecommitOnCollect;
    return S_OK;
}
int32_t SetForceDecommitOnCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceDecommitOnCollect = flag;
    return S_OK;
}
bool IsEnabledForceDeferParseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceDeferParseFlag);
}
int32_t GetForceDeferParseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceDeferParse;
    return S_OK;
}
int32_t SetForceDeferParseFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceDeferParse = flag;
    return S_OK;
}
bool IsEnabledForceDiagnosticsModeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceDiagnosticsModeFlag);
}
int32_t GetForceDiagnosticsModeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceDiagnosticsMode;
    return S_OK;
}
int32_t SetForceDiagnosticsModeFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceDiagnosticsMode = flag;
    return S_OK;
}
bool IsEnabledForceGetWriteWatchOOMFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceGetWriteWatchOOMFlag);
}
int32_t GetForceGetWriteWatchOOMFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceGetWriteWatchOOM;
    return S_OK;
}
int32_t SetForceGetWriteWatchOOMFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceGetWriteWatchOOM = flag;
    return S_OK;
}
bool IsEnabledForcePostLowerGlobOptInstrStringFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForcePostLowerGlobOptInstrStringFlag);
}
int32_t GetForcePostLowerGlobOptInstrStringFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForcePostLowerGlobOptInstrString;
    return S_OK;
}
int32_t SetForcePostLowerGlobOptInstrStringFlag(bool flag)
{
    Js::Configuration::Global.flags.ForcePostLowerGlobOptInstrString = flag;
    return S_OK;
}
bool IsEnabledForceSplitScopeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceSplitScopeFlag);
}
int32_t GetForceSplitScopeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceSplitScope;
    return S_OK;
}
int32_t SetForceSplitScopeFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceSplitScope = flag;
    return S_OK;
}
bool IsEnabledEnumerateSpecialPropertiesInDebuggerFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnumerateSpecialPropertiesInDebuggerFlag);
}
int32_t GetEnumerateSpecialPropertiesInDebuggerFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnumerateSpecialPropertiesInDebugger;
    return S_OK;
}
int32_t SetEnumerateSpecialPropertiesInDebuggerFlag(bool flag)
{
    Js::Configuration::Global.flags.EnumerateSpecialPropertiesInDebugger = flag;
    return S_OK;
}
bool IsEnabledEnableContinueAfterExceptionWrappersForHelpersFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableContinueAfterExceptionWrappersForHelpersFlag);
}
int32_t GetEnableContinueAfterExceptionWrappersForHelpersFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableContinueAfterExceptionWrappersForHelpers;
    return S_OK;
}
int32_t SetEnableContinueAfterExceptionWrappersForHelpersFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableContinueAfterExceptionWrappersForHelpers = flag;
    return S_OK;
}
bool IsEnabledEnableContinueAfterExceptionWrappersForBuiltInsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableContinueAfterExceptionWrappersForBuiltInsFlag);
}
int32_t GetEnableContinueAfterExceptionWrappersForBuiltInsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableContinueAfterExceptionWrappersForBuiltIns;
    return S_OK;
}
int32_t SetEnableContinueAfterExceptionWrappersForBuiltInsFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableContinueAfterExceptionWrappersForBuiltIns = flag;
    return S_OK;
}
bool IsEnabledEnableFunctionSourceReportForHeapEnumFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableFunctionSourceReportForHeapEnumFlag);
}
int32_t GetEnableFunctionSourceReportForHeapEnumFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableFunctionSourceReportForHeapEnum;
    return S_OK;
}
int32_t SetEnableFunctionSourceReportForHeapEnumFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableFunctionSourceReportForHeapEnum = flag;
    return S_OK;
}
bool IsEnabledForceFragmentAddressSpaceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceFragmentAddressSpaceFlag);
}
int32_t GetForceFragmentAddressSpaceFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ForceFragmentAddressSpace;
    return S_OK;
}
int32_t SetForceFragmentAddressSpaceFlag(int flag)
{
    Js::Configuration::Global.flags.ForceFragmentAddressSpace = flag;
    return S_OK;
}
bool IsEnabledForceOOMOnEBCommitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceOOMOnEBCommitFlag);
}
int32_t GetForceOOMOnEBCommitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ForceOOMOnEBCommit;
    return S_OK;
}
int32_t SetForceOOMOnEBCommitFlag(int flag)
{
    Js::Configuration::Global.flags.ForceOOMOnEBCommit = flag;
    return S_OK;
}
bool IsEnabledForceDynamicProfileFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceDynamicProfileFlag);
}
int32_t GetForceDynamicProfileFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceDynamicProfile;
    return S_OK;
}
int32_t SetForceDynamicProfileFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceDynamicProfile = flag;
    return S_OK;
}
bool IsEnabledForceES5ArrayFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceES5ArrayFlag);
}
int32_t GetForceES5ArrayFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceES5Array;
    return S_OK;
}
int32_t SetForceES5ArrayFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceES5Array = flag;
    return S_OK;
}
bool IsEnabledForceAsmJsLinkFailFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceAsmJsLinkFailFlag);
}
int32_t GetForceAsmJsLinkFailFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceAsmJsLinkFail;
    return S_OK;
}
int32_t SetForceAsmJsLinkFailFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceAsmJsLinkFail = flag;
    return S_OK;
}
bool IsEnabledForceExpireOnNonCacheCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceExpireOnNonCacheCollectFlag);
}
int32_t GetForceExpireOnNonCacheCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceExpireOnNonCacheCollect;
    return S_OK;
}
int32_t SetForceExpireOnNonCacheCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceExpireOnNonCacheCollect = flag;
    return S_OK;
}
bool IsEnabledForceFastPathFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceFastPathFlag);
}
int32_t GetForceFastPathFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceFastPath;
    return S_OK;
}
int32_t SetForceFastPathFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceFastPath = flag;
    return S_OK;
}
bool IsEnabledForceFloatPrefFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceFloatPrefFlag);
}
int32_t GetForceFloatPrefFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceFloatPref;
    return S_OK;
}
int32_t SetForceFloatPrefFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceFloatPref = flag;
    return S_OK;
}
bool IsEnabledForceJITLoopBodyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceJITLoopBodyFlag);
}
int32_t GetForceJITLoopBodyFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceJITLoopBody;
    return S_OK;
}
int32_t SetForceJITLoopBodyFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceJITLoopBody = flag;
    return S_OK;
}
bool IsEnabledForceStaticInterpreterThunkFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceStaticInterpreterThunkFlag);
}
int32_t GetForceStaticInterpreterThunkFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceStaticInterpreterThunk;
    return S_OK;
}
int32_t SetForceStaticInterpreterThunkFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceStaticInterpreterThunk = flag;
    return S_OK;
}
bool IsEnabledDumpCommentsFromReferencedFilesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpCommentsFromReferencedFilesFlag);
}
int32_t GetDumpCommentsFromReferencedFilesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpCommentsFromReferencedFiles;
    return S_OK;
}
int32_t SetDumpCommentsFromReferencedFilesFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpCommentsFromReferencedFiles = flag;
    return S_OK;
}
bool IsEnabledDelayFullJITSmallFuncFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DelayFullJITSmallFuncFlag);
}
int32_t GetDelayFullJITSmallFuncFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.DelayFullJITSmallFunc;
    return S_OK;
}
int32_t SetDelayFullJITSmallFuncFlag(int flag)
{
    Js::Configuration::Global.flags.DelayFullJITSmallFunc = flag;
    return S_OK;
}
bool IsEnabledEnableFatalErrorOnOOMFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableFatalErrorOnOOMFlag);
}
int32_t GetEnableFatalErrorOnOOMFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableFatalErrorOnOOM;
    return S_OK;
}
int32_t SetEnableFatalErrorOnOOMFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableFatalErrorOnOOM = flag;
    return S_OK;
}

#if defined(_M_ARM32_OR_ARM64)
bool IsEnabledForceLocalsPtrFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceLocalsPtrFlag);
}
int32_t GetForceLocalsPtrFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceLocalsPtr;
    return S_OK;
}
int32_t SetForceLocalsPtrFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceLocalsPtr = flag;
    return S_OK;
}
#endif
bool IsEnabledDeferLoadingAvailableSourceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DeferLoadingAvailableSourceFlag);
}
int32_t GetDeferLoadingAvailableSourceFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DeferLoadingAvailableSource;
    return S_OK;
}
int32_t SetDeferLoadingAvailableSourceFlag(bool flag)
{
    Js::Configuration::Global.flags.DeferLoadingAvailableSource = flag;
    return S_OK;
}
bool IsEnabledForceNativeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceNativeFlag);
}
int32_t GetForceNativeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceNative;
    return S_OK;
}
int32_t SetForceNativeFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceNative = flag;
    return S_OK;
}
bool IsEnabledForceSerializedFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceSerializedFlag);
}
int32_t GetForceSerializedFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceSerialized;
    return S_OK;
}
int32_t SetForceSerializedFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceSerialized = flag;
    return S_OK;
}
bool IsEnabledForceSerializedBytecodeMajorVersionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceSerializedBytecodeMajorVersionFlag);
}
int32_t GetForceSerializedBytecodeMajorVersionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ForceSerializedBytecodeMajorVersion;
    return S_OK;
}
int32_t SetForceSerializedBytecodeMajorVersionFlag(int flag)
{
    Js::Configuration::Global.flags.ForceSerializedBytecodeMajorVersion = flag;
    return S_OK;
}
bool IsEnabledForceSerializedBytecodeVersionSchemaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceSerializedBytecodeVersionSchemaFlag);
}
int32_t GetForceSerializedBytecodeVersionSchemaFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ForceSerializedBytecodeVersionSchema;
    return S_OK;
}
int32_t SetForceSerializedBytecodeVersionSchemaFlag(int flag)
{
    Js::Configuration::Global.flags.ForceSerializedBytecodeVersionSchema = flag;
    return S_OK;
}
bool IsEnabledForceStrictModeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceStrictModeFlag);
}
int32_t GetForceStrictModeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceStrictMode;
    return S_OK;
}
int32_t SetForceStrictModeFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceStrictMode = flag;
    return S_OK;
}
bool IsEnabledForceUndoDeferFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceUndoDeferFlag);
}
int32_t GetForceUndoDeferFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceUndoDefer;
    return S_OK;
}
int32_t SetForceUndoDeferFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceUndoDefer = flag;
    return S_OK;
}
bool IsEnabledForceBlockingConcurrentCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceBlockingConcurrentCollectFlag);
}
int32_t GetForceBlockingConcurrentCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceBlockingConcurrentCollect;
    return S_OK;
}
int32_t SetForceBlockingConcurrentCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceBlockingConcurrentCollect = flag;
    return S_OK;
}
bool IsEnabledFreTestDiagModeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FreTestDiagModeFlag);
}
int32_t GetFreTestDiagModeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.FreTestDiagMode;
    return S_OK;
}
int32_t SetFreTestDiagModeFlag(bool flag)
{
    Js::Configuration::Global.flags.FreTestDiagMode = flag;
    return S_OK;
}
#ifdef BYTECODE_TESTING
bool IsEnabledByteCodeBranchLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ByteCodeBranchLimitFlag);
}
int32_t GetByteCodeBranchLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ByteCodeBranchLimit;
    return S_OK;
}
int32_t SetByteCodeBranchLimitFlag(int flag)
{
    Js::Configuration::Global.flags.ByteCodeBranchLimit = flag;
    return S_OK;
}
bool IsEnabledMediumByteCodeLayoutFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MediumByteCodeLayoutFlag);
}
int32_t GetMediumByteCodeLayoutFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MediumByteCodeLayout;
    return S_OK;
}
int32_t SetMediumByteCodeLayoutFlag(bool flag)
{
    Js::Configuration::Global.flags.MediumByteCodeLayout = flag;
    return S_OK;
}
bool IsEnabledLargeByteCodeLayoutFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LargeByteCodeLayoutFlag);
}
int32_t GetLargeByteCodeLayoutFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.LargeByteCodeLayout;
    return S_OK;
}
int32_t SetLargeByteCodeLayoutFlag(bool flag)
{
    Js::Configuration::Global.flags.LargeByteCodeLayout = flag;
    return S_OK;
}
#endif
bool IsEnabledInduceCodeGenFailureFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InduceCodeGenFailureFlag);
}
int32_t GetInduceCodeGenFailureFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InduceCodeGenFailure;
    return S_OK;
}
int32_t SetInduceCodeGenFailureFlag(int flag)
{
    Js::Configuration::Global.flags.InduceCodeGenFailure = flag;
    return S_OK;
}
bool IsEnabledInduceCodeGenFailureSeedFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InduceCodeGenFailureSeedFlag);
}
int32_t GetInduceCodeGenFailureSeedFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InduceCodeGenFailureSeed;
    return S_OK;
}
int32_t SetInduceCodeGenFailureSeedFlag(int flag)
{
    Js::Configuration::Global.flags.InduceCodeGenFailureSeed = flag;
    return S_OK;
}
bool IsEnabledInjectPartiallyInitializedInterpreterFrameErrorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InjectPartiallyInitializedInterpreterFrameErrorFlag);
}
int32_t GetInjectPartiallyInitializedInterpreterFrameErrorFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InjectPartiallyInitializedInterpreterFrameError;
    return S_OK;
}
int32_t SetInjectPartiallyInitializedInterpreterFrameErrorFlag(int flag)
{
    Js::Configuration::Global.flags.InjectPartiallyInitializedInterpreterFrameError = flag;
    return S_OK;
}
bool IsEnabledInjectPartiallyInitializedInterpreterFrameErrorTypeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InjectPartiallyInitializedInterpreterFrameErrorTypeFlag);
}
int32_t GetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InjectPartiallyInitializedInterpreterFrameErrorType;
    return S_OK;
}
int32_t SetInjectPartiallyInitializedInterpreterFrameErrorTypeFlag(int flag)
{
    Js::Configuration::Global.flags.InjectPartiallyInitializedInterpreterFrameErrorType = flag;
    return S_OK;
}
bool IsEnabledGenerateByteCodeBufferReturnsCantGenerateFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::GenerateByteCodeBufferReturnsCantGenerateFlag);
}
int32_t GetGenerateByteCodeBufferReturnsCantGenerateFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.GenerateByteCodeBufferReturnsCantGenerate;
    return S_OK;
}
int32_t SetGenerateByteCodeBufferReturnsCantGenerateFlag(bool flag)
{
    Js::Configuration::Global.flags.GenerateByteCodeBufferReturnsCantGenerate = flag;
    return S_OK;
}
bool IsEnabledGoptCleanupThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::GoptCleanupThresholdFlag);
}
int32_t GetGoptCleanupThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.GoptCleanupThreshold;
    return S_OK;
}
int32_t SetGoptCleanupThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.GoptCleanupThreshold = flag;
    return S_OK;
}
bool IsEnabledAsmGoptCleanupThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AsmGoptCleanupThresholdFlag);
}
int32_t GetAsmGoptCleanupThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AsmGoptCleanupThreshold;
    return S_OK;
}
int32_t SetAsmGoptCleanupThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.AsmGoptCleanupThreshold = flag;
    return S_OK;
}
bool IsEnabledHighPrecisionDateFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::HighPrecisionDateFlag);
}
int32_t GetHighPrecisionDateFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.HighPrecisionDate;
    return S_OK;
}
int32_t SetHighPrecisionDateFlag(bool flag)
{
    Js::Configuration::Global.flags.HighPrecisionDate = flag;
    return S_OK;
}
bool IsEnabledInlineCountMaxFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineCountMaxFlag);
}
int32_t GetInlineCountMaxFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineCountMax;
    return S_OK;
}
int32_t SetInlineCountMaxFlag(int flag)
{
    Js::Configuration::Global.flags.InlineCountMax = flag;
    return S_OK;
}
bool IsEnabledInlineCountMaxInLoopBodiesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineCountMaxInLoopBodiesFlag);
}
int32_t GetInlineCountMaxInLoopBodiesFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineCountMaxInLoopBodies;
    return S_OK;
}
int32_t SetInlineCountMaxInLoopBodiesFlag(int flag)
{
    Js::Configuration::Global.flags.InlineCountMaxInLoopBodies = flag;
    return S_OK;
}
bool IsEnabledicminlbFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::icminlbFlag);
}
int32_t GeticminlbFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.icminlb;
    return S_OK;
}
int32_t SeticminlbFlag(int flag)
{
    Js::Configuration::Global.flags.icminlb = flag;
    return S_OK;
}
bool IsEnabledInlineInLoopBodyScaleDownFactorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineInLoopBodyScaleDownFactorFlag);
}
int32_t GetInlineInLoopBodyScaleDownFactorFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineInLoopBodyScaleDownFactor;
    return S_OK;
}
int32_t SetInlineInLoopBodyScaleDownFactorFlag(int flag)
{
    Js::Configuration::Global.flags.InlineInLoopBodyScaleDownFactor = flag;
    return S_OK;
}
bool IsEnablediilbsdfFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::iilbsdfFlag);
}
int32_t GetiilbsdfFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.iilbsdf;
    return S_OK;
}
int32_t SetiilbsdfFlag(int flag)
{
    Js::Configuration::Global.flags.iilbsdf = flag;
    return S_OK;
}
bool IsEnabledInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineThresholdFlag);
}
int32_t GetInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineThreshold;
    return S_OK;
}
int32_t SetInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.InlineThreshold = flag;
    return S_OK;
}
bool IsEnabledAggressiveInlineCountMaxFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AggressiveInlineCountMaxFlag);
}
int32_t GetAggressiveInlineCountMaxFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AggressiveInlineCountMax;
    return S_OK;
}
int32_t SetAggressiveInlineCountMaxFlag(int flag)
{
    Js::Configuration::Global.flags.AggressiveInlineCountMax = flag;
    return S_OK;
}
bool IsEnabledAggressiveInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AggressiveInlineThresholdFlag);
}
int32_t GetAggressiveInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AggressiveInlineThreshold;
    return S_OK;
}
int32_t SetAggressiveInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.AggressiveInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledInlineThresholdAdjustCountInLargeFunctionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineThresholdAdjustCountInLargeFunctionFlag);
}
int32_t GetInlineThresholdAdjustCountInLargeFunctionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineThresholdAdjustCountInLargeFunction;
    return S_OK;
}
int32_t SetInlineThresholdAdjustCountInLargeFunctionFlag(int flag)
{
    Js::Configuration::Global.flags.InlineThresholdAdjustCountInLargeFunction = flag;
    return S_OK;
}
bool IsEnabledInlineThresholdAdjustCountInMediumSizedFunctionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineThresholdAdjustCountInMediumSizedFunctionFlag);
}
int32_t GetInlineThresholdAdjustCountInMediumSizedFunctionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineThresholdAdjustCountInMediumSizedFunction;
    return S_OK;
}
int32_t SetInlineThresholdAdjustCountInMediumSizedFunctionFlag(int flag)
{
    Js::Configuration::Global.flags.InlineThresholdAdjustCountInMediumSizedFunction = flag;
    return S_OK;
}
bool IsEnabledInlineThresholdAdjustCountInSmallFunctionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineThresholdAdjustCountInSmallFunctionFlag);
}
int32_t GetInlineThresholdAdjustCountInSmallFunctionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineThresholdAdjustCountInSmallFunction;
    return S_OK;
}
int32_t SetInlineThresholdAdjustCountInSmallFunctionFlag(int flag)
{
    Js::Configuration::Global.flags.InlineThresholdAdjustCountInSmallFunction = flag;
    return S_OK;
}
bool IsEnabledAsmJsInlineAdjustFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AsmJsInlineAdjustFlag);
}
int32_t GetAsmJsInlineAdjustFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AsmJsInlineAdjust;
    return S_OK;
}
int32_t SetAsmJsInlineAdjustFlag(int flag)
{
    Js::Configuration::Global.flags.AsmJsInlineAdjust = flag;
    return S_OK;
}
bool IsEnabledInterpretFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InterpretFlag);
}

int32_t GetInterpretFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.Interpret);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetInterpretFlag(BSTR flag)
{
    Js::Configuration::Global.flags.Interpret = flag;
    return S_OK;
}
bool IsEnabledJitQueueThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JitQueueThresholdFlag);
}
int32_t GetJitQueueThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.JitQueueThreshold;
    return S_OK;
}
int32_t SetJitQueueThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.JitQueueThreshold = flag;
    return S_OK;
}

bool IsEnabledLoopInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopInlineThresholdFlag);
}
int32_t GetLoopInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopInlineThreshold;
    return S_OK;
}
int32_t SetLoopInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.LoopInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledLeafInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LeafInlineThresholdFlag);
}
int32_t GetLeafInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LeafInlineThreshold;
    return S_OK;
}
int32_t SetLeafInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.LeafInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledConstantArgumentInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ConstantArgumentInlineThresholdFlag);
}
int32_t GetConstantArgumentInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ConstantArgumentInlineThreshold;
    return S_OK;
}
int32_t SetConstantArgumentInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.ConstantArgumentInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledRecursiveInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecursiveInlineThresholdFlag);
}
int32_t GetRecursiveInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecursiveInlineThreshold;
    return S_OK;
}
int32_t SetRecursiveInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.RecursiveInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledRecursiveInlineDepthMaxFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecursiveInlineDepthMaxFlag);
}
int32_t GetRecursiveInlineDepthMaxFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecursiveInlineDepthMax;
    return S_OK;
}
int32_t SetRecursiveInlineDepthMaxFlag(int flag)
{
    Js::Configuration::Global.flags.RecursiveInlineDepthMax = flag;
    return S_OK;
}
bool IsEnabledRecursiveInlineDepthMinFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecursiveInlineDepthMinFlag);
}
int32_t GetRecursiveInlineDepthMinFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecursiveInlineDepthMin;
    return S_OK;
}
int32_t SetRecursiveInlineDepthMinFlag(int flag)
{
    Js::Configuration::Global.flags.RecursiveInlineDepthMin = flag;
    return S_OK;
}
bool IsEnabledRedeferralCapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RedeferralCapFlag);
}
int32_t GetRedeferralCapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RedeferralCap;
    return S_OK;
}
int32_t SetRedeferralCapFlag(int flag)
{
    Js::Configuration::Global.flags.RedeferralCap = flag;
    return S_OK;
}
bool IsEnabledLoopFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopFlag);
}
int32_t GetLoopFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Loop;
    return S_OK;
}
int32_t SetLoopFlag(int flag)
{
    Js::Configuration::Global.flags.Loop = flag;
    return S_OK;
}
bool IsEnabledLoopInterpretCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopInterpretCountFlag);
}
int32_t GetLoopInterpretCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopInterpretCount;
    return S_OK;
}
int32_t SetLoopInterpretCountFlag(int flag)
{
    Js::Configuration::Global.flags.LoopInterpretCount = flag;
    return S_OK;
}
bool IsEnabledlicFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::licFlag);
}
int32_t GetlicFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.lic;
    return S_OK;
}
int32_t SetlicFlag(int flag)
{
    Js::Configuration::Global.flags.lic = flag;
    return S_OK;
}
bool IsEnabledLoopProfileIterationsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopProfileIterationsFlag);
}
int32_t GetLoopProfileIterationsFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopProfileIterations;
    return S_OK;
}
int32_t SetLoopProfileIterationsFlag(int flag)
{
    Js::Configuration::Global.flags.LoopProfileIterations = flag;
    return S_OK;
}
bool IsEnabledOutsideLoopInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::OutsideLoopInlineThresholdFlag);
}
int32_t GetOutsideLoopInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.OutsideLoopInlineThreshold;
    return S_OK;
}
int32_t SetOutsideLoopInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.OutsideLoopInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledMaxFuncInlineDepthFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxFuncInlineDepthFlag);
}
int32_t GetMaxFuncInlineDepthFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxFuncInlineDepth;
    return S_OK;
}
int32_t SetMaxFuncInlineDepthFlag(int flag)
{
    Js::Configuration::Global.flags.MaxFuncInlineDepth = flag;
    return S_OK;
}
bool IsEnabledMaxNumberOfInlineesWithLoopFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxNumberOfInlineesWithLoopFlag);
}
int32_t GetMaxNumberOfInlineesWithLoopFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxNumberOfInlineesWithLoop;
    return S_OK;
}
int32_t SetMaxNumberOfInlineesWithLoopFlag(int flag)
{
    Js::Configuration::Global.flags.MaxNumberOfInlineesWithLoop = flag;
    return S_OK;
}
bool IsEnabledPolymorphicInlineThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PolymorphicInlineThresholdFlag);
}
int32_t GetPolymorphicInlineThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PolymorphicInlineThreshold;
    return S_OK;
}
int32_t SetPolymorphicInlineThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.PolymorphicInlineThreshold = flag;
    return S_OK;
}
bool IsEnabledPrimeRecyclerFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrimeRecyclerFlag);
}
int32_t GetPrimeRecyclerFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrimeRecycler;
    return S_OK;
}
int32_t SetPrimeRecyclerFlag(bool flag)
{
    Js::Configuration::Global.flags.PrimeRecycler = flag;
    return S_OK;
}
bool IsEnabledTraceEngineRefcountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceEngineRefcountFlag);
}
int32_t GetTraceEngineRefcountFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceEngineRefcount;
    return S_OK;
}
int32_t SetTraceEngineRefcountFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceEngineRefcount = flag;
    return S_OK;
}
#if defined(CHECK_MEMORY_LEAK)
bool IsEnabledLeakStackTraceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LeakStackTraceFlag);
}
int32_t GetLeakStackTraceFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.LeakStackTrace;
    return S_OK;
}
int32_t SetLeakStackTraceFlag(bool flag)
{
    Js::Configuration::Global.flags.LeakStackTrace = flag;
    return S_OK;
}
bool IsEnabledForceMemoryLeakFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceMemoryLeakFlag);
}
int32_t GetForceMemoryLeakFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceMemoryLeak;
    return S_OK;
}
int32_t SetForceMemoryLeakFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceMemoryLeak = flag;
    return S_OK;
}
#endif
bool IsEnabledDumpAfterFinalGCFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpAfterFinalGCFlag);
}
int32_t GetDumpAfterFinalGCFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpAfterFinalGC;
    return S_OK;
}
int32_t SetDumpAfterFinalGCFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpAfterFinalGC = flag;
    return S_OK;
}
bool IsEnabledForceOldDateAPIFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceOldDateAPIFlag);
}
int32_t GetForceOldDateAPIFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceOldDateAPI;
    return S_OK;
}
int32_t SetForceOldDateAPIFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceOldDateAPI = flag;
    return S_OK;
}

bool IsEnabledJitLoopBodyHotLoopThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JitLoopBodyHotLoopThresholdFlag);
}
int32_t GetJitLoopBodyHotLoopThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.JitLoopBodyHotLoopThreshold;
    return S_OK;
}
int32_t SetJitLoopBodyHotLoopThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.JitLoopBodyHotLoopThreshold = flag;
    return S_OK;
}
bool IsEnabledLoopBodySizeThresholdToDisableOptsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopBodySizeThresholdToDisableOptsFlag);
}
int32_t GetLoopBodySizeThresholdToDisableOptsFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopBodySizeThresholdToDisableOpts;
    return S_OK;
}
int32_t SetLoopBodySizeThresholdToDisableOptsFlag(int flag)
{
    Js::Configuration::Global.flags.LoopBodySizeThresholdToDisableOpts = flag;
    return S_OK;
}

bool IsEnabledMaxJitThreadCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxJitThreadCountFlag);
}
int32_t GetMaxJitThreadCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxJitThreadCount;
    return S_OK;
}
int32_t SetMaxJitThreadCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxJitThreadCount = flag;
    return S_OK;
}
bool IsEnabledForceMaxJitThreadCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceMaxJitThreadCountFlag);
}
int32_t GetForceMaxJitThreadCountFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceMaxJitThreadCount;
    return S_OK;
}
int32_t SetForceMaxJitThreadCountFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceMaxJitThreadCount = flag;
    return S_OK;
}

bool IsEnabledMitigateSpectreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MitigateSpectreFlag);
}
int32_t GetMitigateSpectreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MitigateSpectre;
    return S_OK;
}
int32_t SetMitigateSpectreFlag(bool flag)
{
    Js::Configuration::Global.flags.MitigateSpectre = flag;
    return S_OK;
}

bool IsEnabledAddMaskingBlocksFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AddMaskingBlocksFlag);
}
int32_t GetAddMaskingBlocksFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.AddMaskingBlocks;
    return S_OK;
}
int32_t SetAddMaskingBlocksFlag(bool flag)
{
    Js::Configuration::Global.flags.AddMaskingBlocks = flag;
    return S_OK;
}

bool IsEnabledPoisonVarArrayLoadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonVarArrayLoadFlag);
}
int32_t GetPoisonVarArrayLoadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonVarArrayLoad;
    return S_OK;
}
int32_t SetPoisonVarArrayLoadFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonVarArrayLoad = flag;
    return S_OK;
}
bool IsEnabledPoisonIntArrayLoadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonIntArrayLoadFlag);
}
int32_t GetPoisonIntArrayLoadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonIntArrayLoad;
    return S_OK;
}
int32_t SetPoisonIntArrayLoadFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonIntArrayLoad = flag;
    return S_OK;
}
bool IsEnabledPoisonFloatArrayLoadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonFloatArrayLoadFlag);
}
int32_t GetPoisonFloatArrayLoadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonFloatArrayLoad;
    return S_OK;
}
int32_t SetPoisonFloatArrayLoadFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonFloatArrayLoad = flag;
    return S_OK;
}
bool IsEnabledPoisonTypedArrayLoadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonTypedArrayLoadFlag);
}
int32_t GetPoisonTypedArrayLoadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonTypedArrayLoad;
    return S_OK;
}
int32_t SetPoisonTypedArrayLoadFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonTypedArrayLoad = flag;
    return S_OK;
}
bool IsEnabledPoisonStringLoadFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonStringLoadFlag);
}
int32_t GetPoisonStringLoadFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonStringLoad;
    return S_OK;
}
int32_t SetPoisonStringLoadFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonStringLoad = flag;
    return S_OK;
}
bool IsEnabledPoisonObjectsForLoadsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonObjectsForLoadsFlag);
}
int32_t GetPoisonObjectsForLoadsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonObjectsForLoads;
    return S_OK;
}
int32_t SetPoisonObjectsForLoadsFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonObjectsForLoads = flag;
    return S_OK;
}

bool IsEnabledPoisonVarArrayStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonVarArrayStoreFlag);
}
int32_t GetPoisonVarArrayStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonVarArrayStore;
    return S_OK;
}
int32_t SetPoisonVarArrayStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonVarArrayStore = flag;
    return S_OK;
}
bool IsEnabledPoisonIntArrayStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonIntArrayStoreFlag);
}
int32_t GetPoisonIntArrayStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonIntArrayStore;
    return S_OK;
}
int32_t SetPoisonIntArrayStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonIntArrayStore = flag;
    return S_OK;
}
bool IsEnabledPoisonFloatArrayStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonFloatArrayStoreFlag);
}
int32_t GetPoisonFloatArrayStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonFloatArrayStore;
    return S_OK;
}
int32_t SetPoisonFloatArrayStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonFloatArrayStore = flag;
    return S_OK;
}
bool IsEnabledPoisonTypedArrayStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonTypedArrayStoreFlag);
}
int32_t GetPoisonTypedArrayStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonTypedArrayStore;
    return S_OK;
}
int32_t SetPoisonTypedArrayStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonTypedArrayStore = flag;
    return S_OK;
}
bool IsEnabledPoisonStringStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonStringStoreFlag);
}
int32_t GetPoisonStringStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonStringStore;
    return S_OK;
}
int32_t SetPoisonStringStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonStringStore = flag;
    return S_OK;
}
bool IsEnabledPoisonObjectsForStoresFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PoisonObjectsForStoresFlag);
}
int32_t GetPoisonObjectsForStoresFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PoisonObjectsForStores;
    return S_OK;
}
int32_t SetPoisonObjectsForStoresFlag(bool flag)
{
    Js::Configuration::Global.flags.PoisonObjectsForStores = flag;
    return S_OK;
}

bool IsEnabledMinInterpretCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinInterpretCountFlag);
}
int32_t GetMinInterpretCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinInterpretCount;
    return S_OK;
}
int32_t SetMinInterpretCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinInterpretCount = flag;
    return S_OK;
}
bool IsEnabledMinSimpleJitRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinSimpleJitRunCountFlag);
}
int32_t GetMinSimpleJitRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinSimpleJitRunCount;
    return S_OK;
}
int32_t SetMinSimpleJitRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinSimpleJitRunCount = flag;
    return S_OK;
}
bool IsEnabledMaxInterpretCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxInterpretCountFlag);
}
int32_t GetMaxInterpretCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxInterpretCount;
    return S_OK;
}
int32_t SetMaxInterpretCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxInterpretCount = flag;
    return S_OK;
}
bool IsEnabledMicFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MicFlag);
}
int32_t GetMicFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Mic;
    return S_OK;
}
int32_t SetMicFlag(int flag)
{
    Js::Configuration::Global.flags.Mic = flag;
    return S_OK;
}
bool IsEnabledMaxSimpleJitRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxSimpleJitRunCountFlag);
}
int32_t GetMaxSimpleJitRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxSimpleJitRunCount;
    return S_OK;
}
int32_t SetMaxSimpleJitRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxSimpleJitRunCount = flag;
    return S_OK;
}
bool IsEnabledMsjrcFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MsjrcFlag);
}
int32_t GetMsjrcFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Msjrc;
    return S_OK;
}
int32_t SetMsjrcFlag(int flag)
{
    Js::Configuration::Global.flags.Msjrc = flag;
    return S_OK;
}
bool IsEnabledMinMemOpCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinMemOpCountFlag);
}
int32_t GetMinMemOpCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinMemOpCount;
    return S_OK;
}
int32_t SetMinMemOpCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinMemOpCount = flag;
    return S_OK;
}
bool IsEnabledMmocFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MmocFlag);
}
int32_t GetMmocFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Mmoc;
    return S_OK;
}
int32_t SetMmocFlag(int flag)
{
    Js::Configuration::Global.flags.Mmoc = flag;
    return S_OK;
}

#if ENABLE_COPYONACCESS_ARRAY
bool IsEnabledMaxCopyOnAccessArrayLengthFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxCopyOnAccessArrayLengthFlag);
}
int32_t GetMaxCopyOnAccessArrayLengthFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxCopyOnAccessArrayLength;
    return S_OK;
}
int32_t SetMaxCopyOnAccessArrayLengthFlag(int flag)
{
    Js::Configuration::Global.flags.MaxCopyOnAccessArrayLength = flag;
    return S_OK;
}
bool IsEnabledMinCopyOnAccessArrayLengthFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinCopyOnAccessArrayLengthFlag);
}
int32_t GetMinCopyOnAccessArrayLengthFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinCopyOnAccessArrayLength;
    return S_OK;
}
int32_t SetMinCopyOnAccessArrayLengthFlag(int flag)
{
    Js::Configuration::Global.flags.MinCopyOnAccessArrayLength = flag;
    return S_OK;
}
bool IsEnabledCopyOnAccessArraySegmentCacheSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CopyOnAccessArraySegmentCacheSizeFlag);
}
int32_t GetCopyOnAccessArraySegmentCacheSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.CopyOnAccessArraySegmentCacheSize;
    return S_OK;
}
int32_t SetCopyOnAccessArraySegmentCacheSizeFlag(int flag)
{
    Js::Configuration::Global.flags.CopyOnAccessArraySegmentCacheSize = flag;
    return S_OK;
}
#endif

bool IsEnabledMinTemplatizedJitRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinTemplatizedJitRunCountFlag);
}
int32_t GetMinTemplatizedJitRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinTemplatizedJitRunCount;
    return S_OK;
}
int32_t SetMinTemplatizedJitRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinTemplatizedJitRunCount = flag;
    return S_OK;
}
bool IsEnabledMinAsmJsInterpreterRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinAsmJsInterpreterRunCountFlag);
}
int32_t GetMinAsmJsInterpreterRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinAsmJsInterpreterRunCount;
    return S_OK;
}
int32_t SetMinAsmJsInterpreterRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinAsmJsInterpreterRunCount = flag;
    return S_OK;
}

bool IsEnabledMinTemplatizedJitLoopRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinTemplatizedJitLoopRunCountFlag);
}
int32_t GetMinTemplatizedJitLoopRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinTemplatizedJitLoopRunCount;
    return S_OK;
}
int32_t SetMinTemplatizedJitLoopRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinTemplatizedJitLoopRunCount = flag;
    return S_OK;
}
bool IsEnabledMaxTemplatizedJitRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxTemplatizedJitRunCountFlag);
}
int32_t GetMaxTemplatizedJitRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxTemplatizedJitRunCount;
    return S_OK;
}
int32_t SetMaxTemplatizedJitRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxTemplatizedJitRunCount = flag;
    return S_OK;
}
bool IsEnabledMtjrcFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MtjrcFlag);
}
int32_t GetMtjrcFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Mtjrc;
    return S_OK;
}
int32_t SetMtjrcFlag(int flag)
{
    Js::Configuration::Global.flags.Mtjrc = flag;
    return S_OK;
}
bool IsEnabledMaxAsmJsInterpreterRunCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxAsmJsInterpreterRunCountFlag);
}
int32_t GetMaxAsmJsInterpreterRunCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxAsmJsInterpreterRunCount;
    return S_OK;
}
int32_t SetMaxAsmJsInterpreterRunCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxAsmJsInterpreterRunCount = flag;
    return S_OK;
}
bool IsEnabledMaicFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaicFlag);
}
int32_t GetMaicFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Maic;
    return S_OK;
}
int32_t SetMaicFlag(int flag)
{
    Js::Configuration::Global.flags.Maic = flag;
    return S_OK;
}

bool IsEnabledAutoProfilingInterpreter0LimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AutoProfilingInterpreter0LimitFlag);
}
int32_t GetAutoProfilingInterpreter0LimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AutoProfilingInterpreter0Limit;
    return S_OK;
}
int32_t SetAutoProfilingInterpreter0LimitFlag(int flag)
{
    Js::Configuration::Global.flags.AutoProfilingInterpreter0Limit = flag;
    return S_OK;
}
bool IsEnabledProfilingInterpreter0LimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfilingInterpreter0LimitFlag);
}
int32_t GetProfilingInterpreter0LimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ProfilingInterpreter0Limit;
    return S_OK;
}
int32_t SetProfilingInterpreter0LimitFlag(int flag)
{
    Js::Configuration::Global.flags.ProfilingInterpreter0Limit = flag;
    return S_OK;
}
bool IsEnabledAutoProfilingInterpreter1LimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AutoProfilingInterpreter1LimitFlag);
}
int32_t GetAutoProfilingInterpreter1LimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AutoProfilingInterpreter1Limit;
    return S_OK;
}
int32_t SetAutoProfilingInterpreter1LimitFlag(int flag)
{
    Js::Configuration::Global.flags.AutoProfilingInterpreter1Limit = flag;
    return S_OK;
}
bool IsEnabledSimpleJitLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SimpleJitLimitFlag);
}
int32_t GetSimpleJitLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SimpleJitLimit;
    return S_OK;
}
int32_t SetSimpleJitLimitFlag(int flag)
{
    Js::Configuration::Global.flags.SimpleJitLimit = flag;
    return S_OK;
}
bool IsEnabledProfilingInterpreter1LimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfilingInterpreter1LimitFlag);
}
int32_t GetProfilingInterpreter1LimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ProfilingInterpreter1Limit;
    return S_OK;
}
int32_t SetProfilingInterpreter1LimitFlag(int flag)
{
    Js::Configuration::Global.flags.ProfilingInterpreter1Limit = flag;
    return S_OK;
}

bool IsEnabledExecutionModeLimitsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ExecutionModeLimitsFlag);
}

int32_t GetExecutionModeLimitsFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.ExecutionModeLimits);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetExecutionModeLimitsFlag(BSTR flag)
{
    Js::Configuration::Global.flags.ExecutionModeLimits = flag;
    return S_OK;
}
bool IsEnabledEmlFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EmlFlag);
}

int32_t GetEmlFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.Eml);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetEmlFlag(BSTR flag)
{
    Js::Configuration::Global.flags.Eml = flag;
    return S_OK;
}
bool IsEnabledEnforceExecutionModeLimitsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnforceExecutionModeLimitsFlag);
}
int32_t GetEnforceExecutionModeLimitsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnforceExecutionModeLimits;
    return S_OK;
}
int32_t SetEnforceExecutionModeLimitsFlag(bool flag)
{
    Js::Configuration::Global.flags.EnforceExecutionModeLimits = flag;
    return S_OK;
}
bool IsEnabledEemlFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EemlFlag);
}
int32_t GetEemlFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Eeml;
    return S_OK;
}
int32_t SetEemlFlag(bool flag)
{
    Js::Configuration::Global.flags.Eeml = flag;
    return S_OK;
}

bool IsEnabledSimpleJitAfterFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SimpleJitAfterFlag);
}
int32_t GetSimpleJitAfterFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SimpleJitAfter;
    return S_OK;
}
int32_t SetSimpleJitAfterFlag(int flag)
{
    Js::Configuration::Global.flags.SimpleJitAfter = flag;
    return S_OK;
}
bool IsEnabledSjaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SjaFlag);
}
int32_t GetSjaFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Sja;
    return S_OK;
}
int32_t SetSjaFlag(int flag)
{
    Js::Configuration::Global.flags.Sja = flag;
    return S_OK;
}
bool IsEnabledFullJitAfterFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FullJitAfterFlag);
}
int32_t GetFullJitAfterFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.FullJitAfter;
    return S_OK;
}
int32_t SetFullJitAfterFlag(int flag)
{
    Js::Configuration::Global.flags.FullJitAfter = flag;
    return S_OK;
}
bool IsEnabledFjaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FjaFlag);
}
int32_t GetFjaFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Fja;
    return S_OK;
}
int32_t SetFjaFlag(int flag)
{
    Js::Configuration::Global.flags.Fja = flag;
    return S_OK;
}

bool IsEnabledNewSimpleJitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NewSimpleJitFlag);
}
int32_t GetNewSimpleJitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NewSimpleJit;
    return S_OK;
}
int32_t SetNewSimpleJitFlag(bool flag)
{
    Js::Configuration::Global.flags.NewSimpleJit = flag;
    return S_OK;
}

bool IsEnabledMaxLinearIntCaseCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxLinearIntCaseCountFlag);
}
int32_t GetMaxLinearIntCaseCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxLinearIntCaseCount;
    return S_OK;
}
int32_t SetMaxLinearIntCaseCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxLinearIntCaseCount = flag;
    return S_OK;
}
bool IsEnabledMaxSingleCharStrJumpTableSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxSingleCharStrJumpTableSizeFlag);
}
int32_t GetMaxSingleCharStrJumpTableSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxSingleCharStrJumpTableSize;
    return S_OK;
}
int32_t SetMaxSingleCharStrJumpTableSizeFlag(int flag)
{
    Js::Configuration::Global.flags.MaxSingleCharStrJumpTableSize = flag;
    return S_OK;
}
bool IsEnabledMaxSingleCharStrJumpTableRatioFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxSingleCharStrJumpTableRatioFlag);
}
int32_t GetMaxSingleCharStrJumpTableRatioFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxSingleCharStrJumpTableRatio;
    return S_OK;
}
int32_t SetMaxSingleCharStrJumpTableRatioFlag(int flag)
{
    Js::Configuration::Global.flags.MaxSingleCharStrJumpTableRatio = flag;
    return S_OK;
}
bool IsEnabledMinSwitchJumpTableSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinSwitchJumpTableSizeFlag);
}
int32_t GetMinSwitchJumpTableSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinSwitchJumpTableSize;
    return S_OK;
}
int32_t SetMinSwitchJumpTableSizeFlag(int flag)
{
    Js::Configuration::Global.flags.MinSwitchJumpTableSize = flag;
    return S_OK;
}
bool IsEnabledMaxLinearStringCaseCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxLinearStringCaseCountFlag);
}
int32_t GetMaxLinearStringCaseCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxLinearStringCaseCount;
    return S_OK;
}
int32_t SetMaxLinearStringCaseCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxLinearStringCaseCount = flag;
    return S_OK;
}
bool IsEnabledMinDeferredFuncTokenCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinDeferredFuncTokenCountFlag);
}
int32_t GetMinDeferredFuncTokenCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinDeferredFuncTokenCount;
    return S_OK;
}
int32_t SetMinDeferredFuncTokenCountFlag(int flag)
{
    Js::Configuration::Global.flags.MinDeferredFuncTokenCount = flag;
    return S_OK;
}
#if DBG
bool IsEnabledSkipFuncCountForBailOnNoProfileFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SkipFuncCountForBailOnNoProfileFlag);
}
int32_t GetSkipFuncCountForBailOnNoProfileFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SkipFuncCountForBailOnNoProfile;
    return S_OK;
}
int32_t SetSkipFuncCountForBailOnNoProfileFlag(int flag)
{
    Js::Configuration::Global.flags.SkipFuncCountForBailOnNoProfile = flag;
    return S_OK;
}
#endif
bool IsEnabledMaxJITFunctionBytecodeByteLengthFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxJITFunctionBytecodeByteLengthFlag);
}
int32_t GetMaxJITFunctionBytecodeByteLengthFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxJITFunctionBytecodeByteLength;
    return S_OK;
}
int32_t SetMaxJITFunctionBytecodeByteLengthFlag(int flag)
{
    Js::Configuration::Global.flags.MaxJITFunctionBytecodeByteLength = flag;
    return S_OK;
}
bool IsEnabledMaxJITFunctionBytecodeCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxJITFunctionBytecodeCountFlag);
}
int32_t GetMaxJITFunctionBytecodeCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxJITFunctionBytecodeCount;
    return S_OK;
}
int32_t SetMaxJITFunctionBytecodeCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxJITFunctionBytecodeCount = flag;
    return S_OK;
}
bool IsEnabledMaxLoopsPerFunctionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxLoopsPerFunctionFlag);
}
int32_t GetMaxLoopsPerFunctionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxLoopsPerFunction;
    return S_OK;
}
int32_t SetMaxLoopsPerFunctionFlag(int flag)
{
    Js::Configuration::Global.flags.MaxLoopsPerFunction = flag;
    return S_OK;
}
bool IsEnabledFuncObjectInlineCacheThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FuncObjectInlineCacheThresholdFlag);
}
int32_t GetFuncObjectInlineCacheThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.FuncObjectInlineCacheThreshold;
    return S_OK;
}
int32_t SetFuncObjectInlineCacheThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.FuncObjectInlineCacheThreshold = flag;
    return S_OK;
}
bool IsEnabledNoDeferParseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NoDeferParseFlag);
}
int32_t GetNoDeferParseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NoDeferParse;
    return S_OK;
}
int32_t SetNoDeferParseFlag(bool flag)
{
    Js::Configuration::Global.flags.NoDeferParse = flag;
    return S_OK;
}
bool IsEnabledNoLogoFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NoLogoFlag);
}
int32_t GetNoLogoFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NoLogo;
    return S_OK;
}
int32_t SetNoLogoFlag(bool flag)
{
    Js::Configuration::Global.flags.NoLogo = flag;
    return S_OK;
}
bool IsEnabledOOPJITMissingOptsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::OOPJITMissingOptsFlag);
}
int32_t GetOOPJITMissingOptsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.OOPJITMissingOpts;
    return S_OK;
}
int32_t SetOOPJITMissingOptsFlag(bool flag)
{
    Js::Configuration::Global.flags.OOPJITMissingOpts = flag;
    return S_OK;
}
bool IsEnabledCrashOnOOPJITFailureFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CrashOnOOPJITFailureFlag);
}
int32_t GetCrashOnOOPJITFailureFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.CrashOnOOPJITFailure;
    return S_OK;
}
int32_t SetCrashOnOOPJITFailureFlag(bool flag)
{
    Js::Configuration::Global.flags.CrashOnOOPJITFailure = flag;
    return S_OK;
}
bool IsEnabledOOPCFGRegistrationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::OOPCFGRegistrationFlag);
}
int32_t GetOOPCFGRegistrationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.OOPCFGRegistration;
    return S_OK;
}
int32_t SetOOPCFGRegistrationFlag(bool flag)
{
    Js::Configuration::Global.flags.OOPCFGRegistration = flag;
    return S_OK;
}
bool IsEnabledForceJITCFGCheckFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceJITCFGCheckFlag);
}
int32_t GetForceJITCFGCheckFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceJITCFGCheck;
    return S_OK;
}
int32_t SetForceJITCFGCheckFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceJITCFGCheck = flag;
    return S_OK;
}
bool IsEnabledUseJITTrampolineFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::UseJITTrampolineFlag);
}
int32_t GetUseJITTrampolineFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.UseJITTrampoline;
    return S_OK;
}
int32_t SetUseJITTrampolineFlag(bool flag)
{
    Js::Configuration::Global.flags.UseJITTrampoline = flag;
    return S_OK;
}
bool IsEnabledNoNativeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NoNativeFlag);
}
int32_t GetNoNativeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NoNative;
    return S_OK;
}
int32_t SetNoNativeFlag(bool flag)
{
    Js::Configuration::Global.flags.NoNative = flag;
    return S_OK;
}
bool IsEnabledNopFrequencyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NopFrequencyFlag);
}
int32_t GetNopFrequencyFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.NopFrequency;
    return S_OK;
}
int32_t SetNopFrequencyFlag(int flag)
{
    Js::Configuration::Global.flags.NopFrequency = flag;
    return S_OK;
}
bool IsEnabledNoStrictModeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NoStrictModeFlag);
}
int32_t GetNoStrictModeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NoStrictMode;
    return S_OK;
}
int32_t SetNoStrictModeFlag(bool flag)
{
    Js::Configuration::Global.flags.NoStrictMode = flag;
    return S_OK;
}
bool IsEnabledNormalizeStatsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NormalizeStatsFlag);
}
int32_t GetNormalizeStatsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.NormalizeStats;
    return S_OK;
}
int32_t SetNormalizeStatsFlag(bool flag)
{
    Js::Configuration::Global.flags.NormalizeStats = flag;
    return S_OK;
}

#ifdef ENABLE_TRACE
bool IsEnabledInMemoryTraceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InMemoryTraceFlag);
}
int32_t GetInMemoryTraceFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.InMemoryTrace;
    return S_OK;
}
int32_t SetInMemoryTraceFlag(bool flag)
{
    Js::Configuration::Global.flags.InMemoryTrace = flag;
    return S_OK;
}
bool IsEnabledInMemoryTraceBufferSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InMemoryTraceBufferSizeFlag);
}
int32_t GetInMemoryTraceBufferSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InMemoryTraceBufferSize;
    return S_OK;
}
int32_t SetInMemoryTraceBufferSizeFlag(int flag)
{
    Js::Configuration::Global.flags.InMemoryTraceBufferSize = flag;
    return S_OK;
}
#ifdef STACK_BACK_TRACE
bool IsEnabledTraceWithStackFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceWithStackFlag);
}
int32_t GetTraceWithStackFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceWithStack;
    return S_OK;
}
int32_t SetTraceWithStackFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceWithStack = flag;
    return S_OK;
}
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE
bool IsEnabledPrintRunTimeDataCollectionTraceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrintRunTimeDataCollectionTraceFlag);
}
int32_t GetPrintRunTimeDataCollectionTraceFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrintRunTimeDataCollectionTrace;
    return S_OK;
}
int32_t SetPrintRunTimeDataCollectionTraceFlag(bool flag)
{
    Js::Configuration::Global.flags.PrintRunTimeDataCollectionTrace = flag;
    return S_OK;
}
#ifdef ENABLE_PREJIT
bool IsEnabledPrejitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrejitFlag);
}
int32_t GetPrejitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Prejit;
    return S_OK;
}
int32_t SetPrejitFlag(bool flag)
{
    Js::Configuration::Global.flags.Prejit = flag;
    return S_OK;
}
#endif
bool IsEnabledPrintSrcInDumpFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrintSrcInDumpFlag);
}
int32_t GetPrintSrcInDumpFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrintSrcInDump;
    return S_OK;
}
int32_t SetPrintSrcInDumpFlag(bool flag)
{
    Js::Configuration::Global.flags.PrintSrcInDump = flag;
    return S_OK;
}
#if PROFILE_DICTIONARY
bool IsEnabledProfileDictionaryFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileDictionaryFlag);
}
int32_t GetProfileDictionaryFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileDictionary;
    return S_OK;
}
int32_t SetProfileDictionaryFlag(int flag)
{
    Js::Configuration::Global.flags.ProfileDictionary = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_EXEC
bool IsEnabledProfileThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileThresholdFlag);
}
int32_t GetProfileThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileThreshold;
    return S_OK;
}
int32_t SetProfileThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.ProfileThreshold = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_OBJECT_LITERALS
bool IsEnabledProfileObjectLiteralFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileObjectLiteralFlag);
}
int32_t GetProfileObjectLiteralFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileObjectLiteral;
    return S_OK;
}
int32_t SetProfileObjectLiteralFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileObjectLiteral = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_MEM
bool IsEnabledProfileMemoryFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileMemoryFlag);
}

int32_t GetProfileMemoryFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.ProfileMemory);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetProfileMemoryFlag(BSTR flag)
{
    Js::Configuration::Global.flags.ProfileMemory = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_STRINGS
bool IsEnabledProfileStringsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileStringsFlag);
}
int32_t GetProfileStringsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileStrings;
    return S_OK;
}
int32_t SetProfileStringsFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileStrings = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_TYPES
bool IsEnabledProfileTypesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileTypesFlag);
}
int32_t GetProfileTypesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileTypes;
    return S_OK;
}
int32_t SetProfileTypesFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileTypes = flag;
    return S_OK;
}
#endif
#ifdef PROFILE_EVALMAP
bool IsEnabledProfileEvalMapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileEvalMapFlag);
}
int32_t GetProfileEvalMapFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileEvalMap;
    return S_OK;
}
int32_t SetProfileEvalMapFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileEvalMap = flag;
    return S_OK;
}
#endif

#ifdef PROFILE_BAILOUT_RECORD_MEMORY
bool IsEnabledProfileBailOutRecordMemoryFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ProfileBailOutRecordMemoryFlag);
}
int32_t GetProfileBailOutRecordMemoryFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ProfileBailOutRecordMemory;
    return S_OK;
}
int32_t SetProfileBailOutRecordMemoryFlag(bool flag)
{
    Js::Configuration::Global.flags.ProfileBailOutRecordMemory = flag;
    return S_OK;
}
#endif

#if DBG
bool IsEnabledValidateIntRangesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ValidateIntRangesFlag);
}
int32_t GetValidateIntRangesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ValidateIntRanges;
    return S_OK;
}
int32_t SetValidateIntRangesFlag(bool flag)
{
    Js::Configuration::Global.flags.ValidateIntRanges = flag;
    return S_OK;
}
#endif
bool IsEnabledRejitMaxBailOutCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RejitMaxBailOutCountFlag);
}
int32_t GetRejitMaxBailOutCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RejitMaxBailOutCount;
    return S_OK;
}
int32_t SetRejitMaxBailOutCountFlag(int flag)
{
    Js::Configuration::Global.flags.RejitMaxBailOutCount = flag;
    return S_OK;
}
bool IsEnabledCallsToBailoutsRatioForRejitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::CallsToBailoutsRatioForRejitFlag);
}
int32_t GetCallsToBailoutsRatioForRejitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.CallsToBailoutsRatioForRejit;
    return S_OK;
}
int32_t SetCallsToBailoutsRatioForRejitFlag(int flag)
{
    Js::Configuration::Global.flags.CallsToBailoutsRatioForRejit = flag;
    return S_OK;
}
bool IsEnabledLoopIterationsToBailoutsRatioForRejitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopIterationsToBailoutsRatioForRejitFlag);
}
int32_t GetLoopIterationsToBailoutsRatioForRejitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopIterationsToBailoutsRatioForRejit;
    return S_OK;
}
int32_t SetLoopIterationsToBailoutsRatioForRejitFlag(int flag)
{
    Js::Configuration::Global.flags.LoopIterationsToBailoutsRatioForRejit = flag;
    return S_OK;
}
bool IsEnabledMinBailOutsBeforeRejitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinBailOutsBeforeRejitFlag);
}
int32_t GetMinBailOutsBeforeRejitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinBailOutsBeforeRejit;
    return S_OK;
}
int32_t SetMinBailOutsBeforeRejitFlag(int flag)
{
    Js::Configuration::Global.flags.MinBailOutsBeforeRejit = flag;
    return S_OK;
}
bool IsEnabledMinBailOutsBeforeRejitForLoopsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinBailOutsBeforeRejitForLoopsFlag);
}
int32_t GetMinBailOutsBeforeRejitForLoopsFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinBailOutsBeforeRejitForLoops;
    return S_OK;
}
int32_t SetMinBailOutsBeforeRejitForLoopsFlag(int flag)
{
    Js::Configuration::Global.flags.MinBailOutsBeforeRejitForLoops = flag;
    return S_OK;
}
bool IsEnabledLibraryStackFrameFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LibraryStackFrameFlag);
}
int32_t GetLibraryStackFrameFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.LibraryStackFrame;
    return S_OK;
}
int32_t SetLibraryStackFrameFlag(bool flag)
{
    Js::Configuration::Global.flags.LibraryStackFrame = flag;
    return S_OK;
}
bool IsEnabledLibraryStackFrameDebuggerFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LibraryStackFrameDebuggerFlag);
}
int32_t GetLibraryStackFrameDebuggerFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.LibraryStackFrameDebugger;
    return S_OK;
}
int32_t SetLibraryStackFrameDebuggerFlag(bool flag)
{
    Js::Configuration::Global.flags.LibraryStackFrameDebugger = flag;
    return S_OK;
}
#ifdef RECYCLER_STRESS
bool IsEnabledRecyclerStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerStressFlag);
}
int32_t GetRecyclerStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerStress;
    return S_OK;
}
int32_t SetRecyclerStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerBackgroundStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerBackgroundStressFlag);
}
int32_t GetRecyclerBackgroundStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerBackgroundStress;
    return S_OK;
}
int32_t SetRecyclerBackgroundStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerBackgroundStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerConcurrentStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerConcurrentStressFlag);
}
int32_t GetRecyclerConcurrentStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerConcurrentStress;
    return S_OK;
}
int32_t SetRecyclerConcurrentStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerConcurrentStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerConcurrentRepeatStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerConcurrentRepeatStressFlag);
}
int32_t GetRecyclerConcurrentRepeatStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerConcurrentRepeatStress;
    return S_OK;
}
int32_t SetRecyclerConcurrentRepeatStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerConcurrentRepeatStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerPartialStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerPartialStressFlag);
}
int32_t GetRecyclerPartialStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerPartialStress;
    return S_OK;
}
int32_t SetRecyclerPartialStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerPartialStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerTrackStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerTrackStressFlag);
}
int32_t GetRecyclerTrackStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerTrackStress;
    return S_OK;
}
int32_t SetRecyclerTrackStressFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerTrackStress = flag;
    return S_OK;
}
bool IsEnabledRecyclerInduceFalsePositivesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerInduceFalsePositivesFlag);
}
int32_t GetRecyclerInduceFalsePositivesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerInduceFalsePositives;
    return S_OK;
}
int32_t SetRecyclerInduceFalsePositivesFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerInduceFalsePositives = flag;
    return S_OK;
}
#endif // RECYCLER_STRESS
bool IsEnabledRecyclerForceMarkInteriorFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerForceMarkInteriorFlag);
}
int32_t GetRecyclerForceMarkInteriorFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerForceMarkInterior;
    return S_OK;
}
int32_t SetRecyclerForceMarkInteriorFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerForceMarkInterior = flag;
    return S_OK;
}
bool IsEnabledRecyclerPriorityBoostTimeoutFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerPriorityBoostTimeoutFlag);
}
int32_t GetRecyclerPriorityBoostTimeoutFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerPriorityBoostTimeout;
    return S_OK;
}
int32_t SetRecyclerPriorityBoostTimeoutFlag(int flag)
{
    Js::Configuration::Global.flags.RecyclerPriorityBoostTimeout = flag;
    return S_OK;
}
bool IsEnabledRecyclerThreadCollectTimeoutFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerThreadCollectTimeoutFlag);
}
int32_t GetRecyclerThreadCollectTimeoutFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerThreadCollectTimeout;
    return S_OK;
}
int32_t SetRecyclerThreadCollectTimeoutFlag(int flag)
{
    Js::Configuration::Global.flags.RecyclerThreadCollectTimeout = flag;
    return S_OK;
}
bool IsEnabledEnableConcurrentSweepAllocFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableConcurrentSweepAllocFlag);
}
int32_t GetEnableConcurrentSweepAllocFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableConcurrentSweepAlloc;
    return S_OK;
}
int32_t SetEnableConcurrentSweepAllocFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableConcurrentSweepAlloc = flag;
    return S_OK;
}
bool IsEnabledecsaFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ecsaFlag);
}
int32_t GetecsaFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ecsa;
    return S_OK;
}
int32_t SetecsaFlag(bool flag)
{
    Js::Configuration::Global.flags.ecsa = flag;
    return S_OK;
}
bool IsEnabledPageHeapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PageHeapFlag);
}
int32_t GetPageHeapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PageHeap;
    return S_OK;
}
int32_t SetPageHeapFlag(int flag)
{
    Js::Configuration::Global.flags.PageHeap = flag;
    return S_OK;
}
bool IsEnabledPageHeapAllocStackFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PageHeapAllocStackFlag);
}
int32_t GetPageHeapAllocStackFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PageHeapAllocStack;
    return S_OK;
}
int32_t SetPageHeapAllocStackFlag(bool flag)
{
    Js::Configuration::Global.flags.PageHeapAllocStack = flag;
    return S_OK;
}
bool IsEnabledPageHeapFreeStackFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PageHeapFreeStackFlag);
}
int32_t GetPageHeapFreeStackFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PageHeapFreeStack;
    return S_OK;
}
int32_t SetPageHeapFreeStackFlag(bool flag)
{
    Js::Configuration::Global.flags.PageHeapFreeStack = flag;
    return S_OK;
}
bool IsEnabledPageHeapBlockTypeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PageHeapBlockTypeFlag);
}
int32_t GetPageHeapBlockTypeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PageHeapBlockType;
    return S_OK;
}
int32_t SetPageHeapBlockTypeFlag(int flag)
{
    Js::Configuration::Global.flags.PageHeapBlockType = flag;
    return S_OK;
}
bool IsEnabledPageHeapDecommitGuardPageFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PageHeapDecommitGuardPageFlag);
}
int32_t GetPageHeapDecommitGuardPageFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PageHeapDecommitGuardPage;
    return S_OK;
}
int32_t SetPageHeapDecommitGuardPageFlag(bool flag)
{
    Js::Configuration::Global.flags.PageHeapDecommitGuardPage = flag;
    return S_OK;
}
#ifdef RECYCLER_NO_PAGE_REUSE
bool IsEnabledRecyclerNoPageReuseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerNoPageReuseFlag);
}
int32_t GetRecyclerNoPageReuseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerNoPageReuse;
    return S_OK;
}
int32_t SetRecyclerNoPageReuseFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerNoPageReuse = flag;
    return S_OK;
}
#endif
#ifdef RECYCLER_MEMORY_VERIFY
bool IsEnabledRecyclerVerifyPadSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerVerifyPadSizeFlag);
}
int32_t GetRecyclerVerifyPadSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerVerifyPadSize;
    return S_OK;
}
int32_t SetRecyclerVerifyPadSizeFlag(int flag)
{
    Js::Configuration::Global.flags.RecyclerVerifyPadSize = flag;
    return S_OK;
}
#endif
bool IsEnabledRecyclerTestFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerTestFlag);
}
int32_t GetRecyclerTestFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerTest;
    return S_OK;
}
int32_t SetRecyclerTestFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerTest = flag;
    return S_OK;
}
bool IsEnabledRecyclerProtectPagesOnRescanFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerProtectPagesOnRescanFlag);
}
int32_t GetRecyclerProtectPagesOnRescanFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerProtectPagesOnRescan;
    return S_OK;
}
int32_t SetRecyclerProtectPagesOnRescanFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerProtectPagesOnRescan = flag;
    return S_OK;
}
#ifdef RECYCLER_VERIFY_MARK
bool IsEnabledRecyclerVerifyMarkFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RecyclerVerifyMarkFlag);
}
int32_t GetRecyclerVerifyMarkFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RecyclerVerifyMark;
    return S_OK;
}
int32_t SetRecyclerVerifyMarkFlag(bool flag)
{
    Js::Configuration::Global.flags.RecyclerVerifyMark = flag;
    return S_OK;
}
#endif
bool IsEnabledLowMemoryCapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LowMemoryCapFlag);
}
int32_t GetLowMemoryCapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LowMemoryCap;
    return S_OK;
}
int32_t SetLowMemoryCapFlag(int flag)
{
    Js::Configuration::Global.flags.LowMemoryCap = flag;
    return S_OK;
}
bool IsEnabledNewPagesCapDuringBGSweepingFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NewPagesCapDuringBGSweepingFlag);
}
int32_t GetNewPagesCapDuringBGSweepingFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.NewPagesCapDuringBGSweeping;
    return S_OK;
}
int32_t SetNewPagesCapDuringBGSweepingFlag(int flag)
{
    Js::Configuration::Global.flags.NewPagesCapDuringBGSweeping = flag;
    return S_OK;
}
bool IsEnabledAllocPolicyLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::AllocPolicyLimitFlag);
}
int32_t GetAllocPolicyLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.AllocPolicyLimit;
    return S_OK;
}
int32_t SetAllocPolicyLimitFlag(int flag)
{
    Js::Configuration::Global.flags.AllocPolicyLimit = flag;
    return S_OK;
}
bool IsEnabledRuntimeDataOutputFileFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RuntimeDataOutputFileFlag);
}

int32_t GetRuntimeDataOutputFileFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.RuntimeDataOutputFile);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetRuntimeDataOutputFileFlag(BSTR flag)
{
    Js::Configuration::Global.flags.RuntimeDataOutputFile = flag;
    return S_OK;
}
bool IsEnabledSpeculationCapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SpeculationCapFlag);
}
int32_t GetSpeculationCapFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SpeculationCap;
    return S_OK;
}
int32_t SetSpeculationCapFlag(int flag)
{
    Js::Configuration::Global.flags.SpeculationCap = flag;
    return S_OK;
}
#if EXCEPTION_RECOVERY
bool IsEnabledSwallowExceptionsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SwallowExceptionsFlag);
}
int32_t GetSwallowExceptionsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.SwallowExceptions;
    return S_OK;
}
int32_t SetSwallowExceptionsFlag(bool flag)
{
    Js::Configuration::Global.flags.SwallowExceptions = flag;
    return S_OK;
}
#endif
bool IsEnabledPrintSystemExceptionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrintSystemExceptionFlag);
}
int32_t GetPrintSystemExceptionFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrintSystemException;
    return S_OK;
}
int32_t SetPrintSystemExceptionFlag(bool flag)
{
    Js::Configuration::Global.flags.PrintSystemException = flag;
    return S_OK;
}
bool IsEnabledSwitchOptHolesThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SwitchOptHolesThresholdFlag);
}
int32_t GetSwitchOptHolesThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SwitchOptHolesThreshold;
    return S_OK;
}
int32_t SetSwitchOptHolesThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.SwitchOptHolesThreshold = flag;
    return S_OK;
}
bool IsEnabledTempMinFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TempMinFlag);
}
int32_t GetTempMinFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.TempMin;
    return S_OK;
}
int32_t SetTempMinFlag(int flag)
{
    Js::Configuration::Global.flags.TempMin = flag;
    return S_OK;
}
bool IsEnabledTempMaxFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TempMaxFlag);
}
int32_t GetTempMaxFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.TempMax;
    return S_OK;
}
int32_t SetTempMaxFlag(int flag)
{
    Js::Configuration::Global.flags.TempMax = flag;
    return S_OK;
}

#if defined(_M_X64)
bool IsEnabledLoopAlignNopLimitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::LoopAlignNopLimitFlag);
}
int32_t GetLoopAlignNopLimitFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.LoopAlignNopLimit;
    return S_OK;
}
int32_t SetLoopAlignNopLimitFlag(int flag)
{
    Js::Configuration::Global.flags.LoopAlignNopLimit = flag;
    return S_OK;
}
#endif

#if DBG_DUMP || defined(RECYCLER_TRACE)
//TraceMetaDataParsing flag with optional levels:
//    Level 1 = interfaces only
//    Level 2 = interfaces and methods
//    Level 3 = interfaces, methods and parameters
//    Level 4 = interfaces and properties
//    Level 5 (default) = ALL
bool IsEnabledTraceMetaDataParsingFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceMetaDataParsingFlag);
}
int32_t GetTraceMetaDataParsingFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.TraceMetaDataParsing;
    return S_OK;
}
int32_t SetTraceMetaDataParsingFlag(int flag)
{
    Js::Configuration::Global.flags.TraceMetaDataParsing = flag;
    return S_OK;
}
bool IsEnabledTraceWin8AllocationsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceWin8AllocationsFlag);
}
int32_t GetTraceWin8AllocationsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceWin8Allocations;
    return S_OK;
}
int32_t SetTraceWin8AllocationsFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceWin8Allocations = flag;
    return S_OK;
}
bool IsEnabledTraceWin8DeallocationsImmediateFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceWin8DeallocationsImmediateFlag);
}
int32_t GetTraceWin8DeallocationsImmediateFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceWin8DeallocationsImmediate;
    return S_OK;
}
int32_t SetTraceWin8DeallocationsImmediateFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceWin8DeallocationsImmediate = flag;
    return S_OK;
}
bool IsEnabledPrintWin8StatsDetailedFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PrintWin8StatsDetailedFlag);
}
int32_t GetPrintWin8StatsDetailedFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.PrintWin8StatsDetailed;
    return S_OK;
}
int32_t SetPrintWin8StatsDetailedFlag(bool flag)
{
    Js::Configuration::Global.flags.PrintWin8StatsDetailed = flag;
    return S_OK;
}
bool IsEnabledTraceProtectPagesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceProtectPagesFlag);
}
int32_t GetTraceProtectPagesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceProtectPages;
    return S_OK;
}
int32_t SetTraceProtectPagesFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceProtectPages = flag;
    return S_OK;
}
#endif
bool IsEnabledTraceAsyncDebugCallsFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceAsyncDebugCallsFlag);
}
int32_t GetTraceAsyncDebugCallsFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceAsyncDebugCalls;
    return S_OK;
}
int32_t SetTraceAsyncDebugCallsFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceAsyncDebugCalls = flag;
    return S_OK;
}
#ifdef TRACK_DISPATCH
bool IsEnabledTrackDispatchFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TrackDispatchFlag);
}
int32_t GetTrackDispatchFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TrackDispatch;
    return S_OK;
}
int32_t SetTrackDispatchFlag(bool flag)
{
    Js::Configuration::Global.flags.TrackDispatch = flag;
    return S_OK;
}
#endif
bool IsEnabledVerboseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::VerboseFlag);
}
int32_t GetVerboseFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Verbose;
    return S_OK;
}
int32_t SetVerboseFlag(bool flag)
{
    Js::Configuration::Global.flags.Verbose = flag;
    return S_OK;
}
bool IsEnabledUseFullNameFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::UseFullNameFlag);
}
int32_t GetUseFullNameFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.UseFullName;
    return S_OK;
}
int32_t SetUseFullNameFlag(bool flag)
{
    Js::Configuration::Global.flags.UseFullName = flag;
    return S_OK;
}
bool IsEnabledUtf8Flag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::Utf8Flag);
}
int32_t GetUtf8Flag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.Utf8;
    return S_OK;
}
int32_t SetUtf8Flag(bool flag)
{
    Js::Configuration::Global.flags.Utf8 = flag;
    return S_OK;
}
bool IsEnabledVersionFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::VersionFlag);
}
int32_t GetVersionFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Version;
    return S_OK;
}
int32_t SetVersionFlag(int flag)
{
    Js::Configuration::Global.flags.Version = flag;
    return S_OK;
}
bool IsEnabledWERExceptionSupportFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WERExceptionSupportFlag);
}
int32_t GetWERExceptionSupportFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WERExceptionSupport;
    return S_OK;
}
int32_t SetWERExceptionSupportFlag(bool flag)
{
    Js::Configuration::Global.flags.WERExceptionSupport = flag;
    return S_OK;
}
bool IsEnabledExtendedErrorStackForTestHostFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ExtendedErrorStackForTestHostFlag);
}
int32_t GetExtendedErrorStackForTestHostFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ExtendedErrorStackForTestHost;
    return S_OK;
}
int32_t SetExtendedErrorStackForTestHostFlag(bool flag)
{
    Js::Configuration::Global.flags.ExtendedErrorStackForTestHost = flag;
    return S_OK;
}
bool IsEnablederrorStackTraceFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::errorStackTraceFlag);
}
int32_t GeterrorStackTraceFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.errorStackTrace;
    return S_OK;
}
int32_t SeterrorStackTraceFlag(bool flag)
{
    Js::Configuration::Global.flags.errorStackTrace = flag;
    return S_OK;
}
bool IsEnabledDoHeapEnumOnEngineShutdownFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DoHeapEnumOnEngineShutdownFlag);
}
int32_t GetDoHeapEnumOnEngineShutdownFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DoHeapEnumOnEngineShutdown;
    return S_OK;
}
int32_t SetDoHeapEnumOnEngineShutdownFlag(bool flag)
{
    Js::Configuration::Global.flags.DoHeapEnumOnEngineShutdown = flag;
    return S_OK;
}
#ifdef HEAP_ENUMERATION_VALIDATION
bool IsEnabledValidateHeapEnumFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ValidateHeapEnumFlag);
}
int32_t GetValidateHeapEnumFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ValidateHeapEnum;
    return S_OK;
}
int32_t SetValidateHeapEnumFlag(bool flag)
{
    Js::Configuration::Global.flags.ValidateHeapEnum = flag;
    return S_OK;
}
#endif

#if ENABLE_REGEX_CONFIG_OPTIONS
//
// Regex flags
//
bool IsEnabledRegexTracingFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexTracingFlag);
}
int32_t GetRegexTracingFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexTracing;
    return S_OK;
}
int32_t SetRegexTracingFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexTracing = flag;
    return S_OK;
}
bool IsEnabledRegexProfileFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexProfileFlag);
}
int32_t GetRegexProfileFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexProfile;
    return S_OK;
}
int32_t SetRegexProfileFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexProfile = flag;
    return S_OK;
}
bool IsEnabledRegexDebugFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexDebugFlag);
}
int32_t GetRegexDebugFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexDebug;
    return S_OK;
}
int32_t SetRegexDebugFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexDebug = flag;
    return S_OK;
}
bool IsEnabledRegexDebugASTFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexDebugASTFlag);
}
int32_t GetRegexDebugASTFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexDebugAST;
    return S_OK;
}
int32_t SetRegexDebugASTFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexDebugAST = flag;
    return S_OK;
}
bool IsEnabledRegexDebugAnnotatedASTFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexDebugAnnotatedASTFlag);
}
int32_t GetRegexDebugAnnotatedASTFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexDebugAnnotatedAST;
    return S_OK;
}
int32_t SetRegexDebugAnnotatedASTFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexDebugAnnotatedAST = flag;
    return S_OK;
}
bool IsEnabledRegexBytecodeDebugFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexBytecodeDebugFlag);
}
int32_t GetRegexBytecodeDebugFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexBytecodeDebug;
    return S_OK;
}
int32_t SetRegexBytecodeDebugFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexBytecodeDebug = flag;
    return S_OK;
}
bool IsEnabledRegexOptimizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::RegexOptimizeFlag);
}
int32_t GetRegexOptimizeFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.RegexOptimize;
    return S_OK;
}
int32_t SetRegexOptimizeFlag(bool flag)
{
    Js::Configuration::Global.flags.RegexOptimize = flag;
    return S_OK;
}
bool IsEnabledDynamicRegexMruListSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DynamicRegexMruListSizeFlag);
}
int32_t GetDynamicRegexMruListSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.DynamicRegexMruListSize;
    return S_OK;
}
int32_t SetDynamicRegexMruListSizeFlag(int flag)
{
    Js::Configuration::Global.flags.DynamicRegexMruListSize = flag;
    return S_OK;
}
#endif

bool IsEnabledOptimizeForManyInstancesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::OptimizeForManyInstancesFlag);
}
int32_t GetOptimizeForManyInstancesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.OptimizeForManyInstances;
    return S_OK;
}
int32_t SetOptimizeForManyInstancesFlag(bool flag)
{
    Js::Configuration::Global.flags.OptimizeForManyInstances = flag;
    return S_OK;
}
bool IsEnabledEnableArrayTypeMutationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableArrayTypeMutationFlag);
}
int32_t GetEnableArrayTypeMutationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableArrayTypeMutation;
    return S_OK;
}
int32_t SetEnableArrayTypeMutationFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableArrayTypeMutation = flag;
    return S_OK;
}
bool IsEnabledArrayMutationTestSeedFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ArrayMutationTestSeedFlag);
}
int32_t GetArrayMutationTestSeedFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ArrayMutationTestSeed;
    return S_OK;
}
int32_t SetArrayMutationTestSeedFlag(int flag)
{
    Js::Configuration::Global.flags.ArrayMutationTestSeed = flag;
    return S_OK;
}
bool IsEnabledEnableEvalMapCleanupFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableEvalMapCleanupFlag);
}
int32_t GetEnableEvalMapCleanupFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableEvalMapCleanup;
    return S_OK;
}
int32_t SetEnableEvalMapCleanupFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableEvalMapCleanup = flag;
    return S_OK;
}
#ifdef PROFILE_MEM
bool IsEnabledTraceObjectAllocationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TraceObjectAllocationFlag);
}
int32_t GetTraceObjectAllocationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TraceObjectAllocation;
    return S_OK;
}
int32_t SetTraceObjectAllocationFlag(bool flag)
{
    Js::Configuration::Global.flags.TraceObjectAllocation = flag;
    return S_OK;
}
#endif
bool IsEnabledSseFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SseFlag);
}
int32_t GetSseFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.Sse;
    return S_OK;
}
int32_t SetSseFlag(int flag)
{
    Js::Configuration::Global.flags.Sse = flag;
    return S_OK;
}
bool IsEnabledDeletedPropertyReuseThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DeletedPropertyReuseThresholdFlag);
}
int32_t GetDeletedPropertyReuseThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.DeletedPropertyReuseThreshold;
    return S_OK;
}
int32_t SetDeletedPropertyReuseThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.DeletedPropertyReuseThreshold = flag;
    return S_OK;
}
bool IsEnabledForceStringKeyedSimpleDictionaryTypeHandlerFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceStringKeyedSimpleDictionaryTypeHandlerFlag);
}
int32_t GetForceStringKeyedSimpleDictionaryTypeHandlerFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceStringKeyedSimpleDictionaryTypeHandler;
    return S_OK;
}
int32_t SetForceStringKeyedSimpleDictionaryTypeHandlerFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceStringKeyedSimpleDictionaryTypeHandler = flag;
    return S_OK;
}
bool IsEnabledBigDictionaryTypeHandlerThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BigDictionaryTypeHandlerThresholdFlag);
}
int32_t GetBigDictionaryTypeHandlerThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BigDictionaryTypeHandlerThreshold;
    return S_OK;
}
int32_t SetBigDictionaryTypeHandlerThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.BigDictionaryTypeHandlerThreshold = flag;
    return S_OK;
}
bool IsEnabledTypeSnapshotEnumerationFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::TypeSnapshotEnumerationFlag);
}
int32_t GetTypeSnapshotEnumerationFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.TypeSnapshotEnumeration;
    return S_OK;
}
int32_t SetTypeSnapshotEnumerationFlag(bool flag)
{
    Js::Configuration::Global.flags.TypeSnapshotEnumeration = flag;
    return S_OK;
}
bool IsEnabledIsolatePrototypesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::IsolatePrototypesFlag);
}
int32_t GetIsolatePrototypesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.IsolatePrototypes;
    return S_OK;
}
int32_t SetIsolatePrototypesFlag(bool flag)
{
    Js::Configuration::Global.flags.IsolatePrototypes = flag;
    return S_OK;
}
bool IsEnabledChangeTypeOnProtoFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ChangeTypeOnProtoFlag);
}
int32_t GetChangeTypeOnProtoFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ChangeTypeOnProto;
    return S_OK;
}
int32_t SetChangeTypeOnProtoFlag(bool flag)
{
    Js::Configuration::Global.flags.ChangeTypeOnProto = flag;
    return S_OK;
}
bool IsEnabledShareInlineCachesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ShareInlineCachesFlag);
}
int32_t GetShareInlineCachesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ShareInlineCaches;
    return S_OK;
}
int32_t SetShareInlineCachesFlag(bool flag)
{
    Js::Configuration::Global.flags.ShareInlineCaches = flag;
    return S_OK;
}
bool IsEnabledDisableDebugObjectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DisableDebugObjectFlag);
}
int32_t GetDisableDebugObjectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DisableDebugObject;
    return S_OK;
}
int32_t SetDisableDebugObjectFlag(bool flag)
{
    Js::Configuration::Global.flags.DisableDebugObject = flag;
    return S_OK;
}
bool IsEnabledDumpHeapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::DumpHeapFlag);
}
int32_t GetDumpHeapFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.DumpHeap;
    return S_OK;
}
int32_t SetDumpHeapFlag(bool flag)
{
    Js::Configuration::Global.flags.DumpHeap = flag;
    return S_OK;
}
bool IsEnabledautoProxyFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::autoProxyFlag);
}

int32_t GetautoProxyFlag(BSTR *flag)
{
    *flag = SysAllocString(Js::Configuration::Global.flags.autoProxy);
    return (*flag == NULL ? E_OUTOFMEMORY : S_OK);
}

int32_t SetautoProxyFlag(BSTR flag)
{
    Js::Configuration::Global.flags.autoProxy = flag;
    return S_OK;
}
bool IsEnabledPerfHintLevelFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PerfHintLevelFlag);
}
int32_t GetPerfHintLevelFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PerfHintLevel;
    return S_OK;
}
int32_t SetPerfHintLevelFlag(int flag)
{
    Js::Configuration::Global.flags.PerfHintLevel = flag;
    return S_OK;
}
#ifdef INTERNAL_MEM_PROTECT_HEAP_ALLOC
bool IsEnabledMemProtectHeapFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapFlag);
}
int32_t GetMemProtectHeapFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeap;
    return S_OK;
}
int32_t SetMemProtectHeapFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeap = flag;
    return S_OK;
}
#endif
#ifdef RECYCLER_STRESS
bool IsEnabledMemProtectHeapStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapStressFlag);
}
int32_t GetMemProtectHeapStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeapStress;
    return S_OK;
}
int32_t SetMemProtectHeapStressFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeapStress = flag;
    return S_OK;
}
bool IsEnabledMemProtectHeapBackgroundStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapBackgroundStressFlag);
}
int32_t GetMemProtectHeapBackgroundStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeapBackgroundStress;
    return S_OK;
}
int32_t SetMemProtectHeapBackgroundStressFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeapBackgroundStress = flag;
    return S_OK;
}
bool IsEnabledMemProtectHeapConcurrentStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapConcurrentStressFlag);
}
int32_t GetMemProtectHeapConcurrentStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeapConcurrentStress;
    return S_OK;
}
int32_t SetMemProtectHeapConcurrentStressFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeapConcurrentStress = flag;
    return S_OK;
}
bool IsEnabledMemProtectHeapConcurrentRepeatStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapConcurrentRepeatStressFlag);
}
int32_t GetMemProtectHeapConcurrentRepeatStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeapConcurrentRepeatStress;
    return S_OK;
}
int32_t SetMemProtectHeapConcurrentRepeatStressFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeapConcurrentRepeatStress = flag;
    return S_OK;
}
bool IsEnabledMemProtectHeapPartialStressFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MemProtectHeapPartialStressFlag);
}
int32_t GetMemProtectHeapPartialStressFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.MemProtectHeapPartialStress;
    return S_OK;
}
int32_t SetMemProtectHeapPartialStressFlag(bool flag)
{
    Js::Configuration::Global.flags.MemProtectHeapPartialStress = flag;
    return S_OK;
}
#endif
#ifdef SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
bool IsEnabledFixPropsOnPathTypesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::FixPropsOnPathTypesFlag);
}
int32_t GetFixPropsOnPathTypesFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.FixPropsOnPathTypes;
    return S_OK;
}
int32_t SetFixPropsOnPathTypesFlag(bool flag)
{
    Js::Configuration::Global.flags.FixPropsOnPathTypes = flag;
    return S_OK;
}
#endif

// recycler heuristic flags
bool IsEnabledMaxBackgroundFinishMarkCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxBackgroundFinishMarkCountFlag);
}
int32_t GetMaxBackgroundFinishMarkCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxBackgroundFinishMarkCount;
    return S_OK;
}
int32_t SetMaxBackgroundFinishMarkCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxBackgroundFinishMarkCount = flag;
    return S_OK;
}
bool IsEnabledBackgroundFinishMarkWaitTimeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::BackgroundFinishMarkWaitTimeFlag);
}
int32_t GetBackgroundFinishMarkWaitTimeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.BackgroundFinishMarkWaitTime;
    return S_OK;
}
int32_t SetBackgroundFinishMarkWaitTimeFlag(int flag)
{
    Js::Configuration::Global.flags.BackgroundFinishMarkWaitTime = flag;
    return S_OK;
}
bool IsEnabledMinBackgroundRepeatMarkRescanBytesFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MinBackgroundRepeatMarkRescanBytesFlag);
}
int32_t GetMinBackgroundRepeatMarkRescanBytesFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MinBackgroundRepeatMarkRescanBytes;
    return S_OK;
}
int32_t SetMinBackgroundRepeatMarkRescanBytesFlag(int flag)
{
    Js::Configuration::Global.flags.MinBackgroundRepeatMarkRescanBytes = flag;
    return S_OK;
}

#if defined(_M_X64)
bool IsEnabledZeroMemoryWithNonTemporalStoreFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ZeroMemoryWithNonTemporalStoreFlag);
}
int32_t GetZeroMemoryWithNonTemporalStoreFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ZeroMemoryWithNonTemporalStore;
    return S_OK;
}
int32_t SetZeroMemoryWithNonTemporalStoreFlag(bool flag)
{
    Js::Configuration::Global.flags.ZeroMemoryWithNonTemporalStore = flag;
    return S_OK;
}
#endif

// recycler memory restrict test flags
bool IsEnabledMaxMarkStackPageCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxMarkStackPageCountFlag);
}
int32_t GetMaxMarkStackPageCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxMarkStackPageCount;
    return S_OK;
}
int32_t SetMaxMarkStackPageCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxMarkStackPageCount = flag;
    return S_OK;
}
bool IsEnabledMaxTrackedObjectListCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxTrackedObjectListCountFlag);
}
int32_t GetMaxTrackedObjectListCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxTrackedObjectListCount;
    return S_OK;
}
int32_t SetMaxTrackedObjectListCountFlag(int flag)
{
    Js::Configuration::Global.flags.MaxTrackedObjectListCount = flag;
    return S_OK;
}

// make the recycler page integration path easier to hit
bool IsEnabledNumberAllocPlusSizeFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::NumberAllocPlusSizeFlag);
}
int32_t GetNumberAllocPlusSizeFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.NumberAllocPlusSize;
    return S_OK;
}
int32_t SetNumberAllocPlusSizeFlag(int flag)
{
    Js::Configuration::Global.flags.NumberAllocPlusSize = flag;
    return S_OK;
}

#if DBG
bool IsEnabledInitializeInterpreterSlotsWithInvalidStackVarFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InitializeInterpreterSlotsWithInvalidStackVarFlag);
}
int32_t GetInitializeInterpreterSlotsWithInvalidStackVarFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.InitializeInterpreterSlotsWithInvalidStackVar;
    return S_OK;
}
int32_t SetInitializeInterpreterSlotsWithInvalidStackVarFlag(bool flag)
{
    Js::Configuration::Global.flags.InitializeInterpreterSlotsWithInvalidStackVar = flag;
    return S_OK;
}
#endif

#if DBG
bool IsEnabledPRNGSeed0Flag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PRNGSeed0Flag);
}
int32_t GetPRNGSeed0Flag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PRNGSeed0;
    return S_OK;
}
int32_t SetPRNGSeed0Flag(int flag)
{
    Js::Configuration::Global.flags.PRNGSeed0 = flag;
    return S_OK;
}
bool IsEnabledPRNGSeed1Flag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::PRNGSeed1Flag);
}
int32_t GetPRNGSeed1Flag(int *flag)
{
    *flag = Js::Configuration::Global.flags.PRNGSeed1;
    return S_OK;
}
int32_t SetPRNGSeed1Flag(int flag)
{
    Js::Configuration::Global.flags.PRNGSeed1 = flag;
    return S_OK;
}
#endif

bool IsEnabledClearInlineCachesOnCollectFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ClearInlineCachesOnCollectFlag);
}
int32_t GetClearInlineCachesOnCollectFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ClearInlineCachesOnCollect;
    return S_OK;
}
int32_t SetClearInlineCachesOnCollectFlag(bool flag)
{
    Js::Configuration::Global.flags.ClearInlineCachesOnCollect = flag;
    return S_OK;
}
bool IsEnabledInlineCacheInvalidationListCompactionThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::InlineCacheInvalidationListCompactionThresholdFlag);
}
int32_t GetInlineCacheInvalidationListCompactionThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.InlineCacheInvalidationListCompactionThreshold;
    return S_OK;
}
int32_t SetInlineCacheInvalidationListCompactionThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.InlineCacheInvalidationListCompactionThreshold = flag;
    return S_OK;
}
bool IsEnabledConstructorCacheInvalidationThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ConstructorCacheInvalidationThresholdFlag);
}
int32_t GetConstructorCacheInvalidationThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.ConstructorCacheInvalidationThreshold;
    return S_OK;
}
int32_t SetConstructorCacheInvalidationThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.ConstructorCacheInvalidationThreshold = flag;
    return S_OK;
}

bool IsEnabledGCMemoryThresholdFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::GCMemoryThresholdFlag);
}
int32_t GetGCMemoryThresholdFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.GCMemoryThreshold;
    return S_OK;
}
int32_t SetGCMemoryThresholdFlag(int flag)
{
    Js::Configuration::Global.flags.GCMemoryThreshold = flag;
    return S_OK;
}

#if DBG
    bool IsEnabledSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::SimulatePolyCacheWithOneTypeForInlineCacheIndexFlag);
}
int32_t GetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.SimulatePolyCacheWithOneTypeForInlineCacheIndex;
    return S_OK;
}
int32_t SetSimulatePolyCacheWithOneTypeForInlineCacheIndexFlag(int flag)
{
    Js::Configuration::Global.flags.SimulatePolyCacheWithOneTypeForInlineCacheIndex = flag;
    return S_OK;
}
#endif

bool IsEnabledJITServerIdleTimeoutFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JITServerIdleTimeoutFlag);
}
int32_t GetJITServerIdleTimeoutFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.JITServerIdleTimeout;
    return S_OK;
}
int32_t SetJITServerIdleTimeoutFlag(int flag)
{
    Js::Configuration::Global.flags.JITServerIdleTimeout = flag;
    return S_OK;
}
bool IsEnabledJITServerMaxInactivePageAllocatorCountFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::JITServerMaxInactivePageAllocatorCountFlag);
}
int32_t GetJITServerMaxInactivePageAllocatorCountFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.JITServerMaxInactivePageAllocatorCount;
    return S_OK;
}
int32_t SetJITServerMaxInactivePageAllocatorCountFlag(int flag)
{
    Js::Configuration::Global.flags.JITServerMaxInactivePageAllocatorCount = flag;
    return S_OK;
}

bool IsEnabledStrictWriteBarrierCheckFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::StrictWriteBarrierCheckFlag);
}
int32_t GetStrictWriteBarrierCheckFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.StrictWriteBarrierCheck;
    return S_OK;
}
int32_t SetStrictWriteBarrierCheckFlag(bool flag)
{
    Js::Configuration::Global.flags.StrictWriteBarrierCheck = flag;
    return S_OK;
}
bool IsEnabledWriteBarrierTestFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::WriteBarrierTestFlag);
}
int32_t GetWriteBarrierTestFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.WriteBarrierTest;
    return S_OK;
}
int32_t SetWriteBarrierTestFlag(bool flag)
{
    Js::Configuration::Global.flags.WriteBarrierTest = flag;
    return S_OK;
}
bool IsEnabledForceSoftwareWriteBarrierFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::ForceSoftwareWriteBarrierFlag);
}
int32_t GetForceSoftwareWriteBarrierFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.ForceSoftwareWriteBarrier;
    return S_OK;
}
int32_t SetForceSoftwareWriteBarrierFlag(bool flag)
{
    Js::Configuration::Global.flags.ForceSoftwareWriteBarrier = flag;
    return S_OK;
}
bool IsEnabledVerifyBarrierBitFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::VerifyBarrierBitFlag);
}
int32_t GetVerifyBarrierBitFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.VerifyBarrierBit;
    return S_OK;
}
int32_t SetVerifyBarrierBitFlag(bool flag)
{
    Js::Configuration::Global.flags.VerifyBarrierBit = flag;
    return S_OK;
}
bool IsEnabledEnableBGFreeZeroFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::EnableBGFreeZeroFlag);
}
int32_t GetEnableBGFreeZeroFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.EnableBGFreeZero;
    return S_OK;
}
int32_t SetEnableBGFreeZeroFlag(bool flag)
{
    Js::Configuration::Global.flags.EnableBGFreeZero = flag;
    return S_OK;
}
bool IsEnabledKeepRecyclerTrackDataFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::KeepRecyclerTrackDataFlag);
}
int32_t GetKeepRecyclerTrackDataFlag(bool *flag)
{
    *flag = Js::Configuration::Global.flags.KeepRecyclerTrackData;
    return S_OK;
}
int32_t SetKeepRecyclerTrackDataFlag(bool flag)
{
    Js::Configuration::Global.flags.KeepRecyclerTrackData = flag;
    return S_OK;
}

bool IsEnabledMaxSingleAllocSizeInMBFlag()
{
    return Js::Configuration::Global.flags.IsEnabled(Js::MaxSingleAllocSizeInMBFlag);
}
int32_t GetMaxSingleAllocSizeInMBFlag(int *flag)
{
    *flag = Js::Configuration::Global.flags.MaxSingleAllocSizeInMB;
    return S_OK;
}
int32_t SetMaxSingleAllocSizeInMBFlag(int flag)
{
    Js::Configuration::Global.flags.MaxSingleAllocSizeInMB = flag;
    return S_OK;
}

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
#endif
IsEnabledAsmJsFlag, GetAsmJsFlag, SetAsmJsFlag,
IsEnabledWasmFlag, GetWasmFlag, SetWasmFlag,
IsEnabledWasmI64Flag, GetWasmI64Flag, SetWasmI64Flag,
IsEnabledWasmFastArrayFlag, GetWasmFastArrayFlag, SetWasmFastArrayFlag,
IsEnabledWasmSharedArrayVirtualBufferFlag, GetWasmSharedArrayVirtualBufferFlag, SetWasmSharedArrayVirtualBufferFlag,
IsEnabledWasmMathExFilterFlag, GetWasmMathExFilterFlag, SetWasmMathExFilterFlag,
IsEnabledWasmCheckVersionFlag, GetWasmCheckVersionFlag, SetWasmCheckVersionFlag,
IsEnabledWasmAssignModuleIDFlag, GetWasmAssignModuleIDFlag, SetWasmAssignModuleIDFlag,
IsEnabledWasmIgnoreResponseFlag, GetWasmIgnoreResponseFlag, SetWasmIgnoreResponseFlag,
IsEnabledWasmMaxTableSizeFlag, GetWasmMaxTableSizeFlag, SetWasmMaxTableSizeFlag,
IsEnabledWasmThreadsFlag, GetWasmThreadsFlag, SetWasmThreadsFlag,
IsEnabledWasmMultiValueFlag, GetWasmMultiValueFlag, SetWasmMultiValueFlag,
IsEnabledWasmSignExtendsFlag, GetWasmSignExtendsFlag, SetWasmSignExtendsFlag,
IsEnabledWasmNontrappingFlag, GetWasmNontrappingFlag, SetWasmNontrappingFlag,

// WebAssembly Experimental Features

// The default value of the experimental features will be off because the parent is off
// Turning on the parent causes the child flag to take on their default value (aka on)
// In Edge, we manually turn on the individual child flags
// Not having the DEFAULT_CONFIG_XXXX macro ensures we use CONFIG_FLAG_RELEASE instead of CONFIG_FLAG
IsEnabledWasmSimdFlag, GetWasmSimdFlag, SetWasmSimdFlag,

IsEnabledBailOnNoProfileLimitFlag, GetBailOnNoProfileLimitFlag, SetBailOnNoProfileLimitFlag,
IsEnabledBailOnNoProfileRejitLimitFlag, GetBailOnNoProfileRejitLimitFlag, SetBailOnNoProfileRejitLimitFlag,
IsEnabledBaselineModeFlag, GetBaselineModeFlag, SetBaselineModeFlag,
IsEnabledDumpOnCrashFlag, GetDumpOnCrashFlag, SetDumpOnCrashFlag,
IsEnabledFullMemoryDumpFlag, GetFullMemoryDumpFlag, SetFullMemoryDumpFlag,

IsEnabledBailOutAtEveryLineFlag, GetBailOutAtEveryLineFlag, SetBailOutAtEveryLineFlag,
IsEnabledBailOutAtEveryByteCodeFlag, GetBailOutAtEveryByteCodeFlag, SetBailOutAtEveryByteCodeFlag,
IsEnabledBailOutAtEveryImplicitCallFlag, GetBailOutAtEveryImplicitCallFlag, SetBailOutAtEveryImplicitCallFlag,

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
#if defined(CHECK_MEMORY_LEAK)
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
IsEnabledRecyclerBackgroundStressFlag, GetRecyclerBackgroundStressFlag, SetRecyclerBackgroundStressFlag,
IsEnabledRecyclerConcurrentStressFlag, GetRecyclerConcurrentStressFlag, SetRecyclerConcurrentStressFlag,
IsEnabledRecyclerConcurrentRepeatStressFlag, GetRecyclerConcurrentRepeatStressFlag, SetRecyclerConcurrentRepeatStressFlag,
IsEnabledRecyclerPartialStressFlag, GetRecyclerPartialStressFlag, SetRecyclerPartialStressFlag,
IsEnabledRecyclerTrackStressFlag, GetRecyclerTrackStressFlag, SetRecyclerTrackStressFlag,
IsEnabledRecyclerInduceFalsePositivesFlag, GetRecyclerInduceFalsePositivesFlag, SetRecyclerInduceFalsePositivesFlag,
#endif // RECYCLER_STRESS
IsEnabledRecyclerForceMarkInteriorFlag, GetRecyclerForceMarkInteriorFlag, SetRecyclerForceMarkInteriorFlag,
IsEnabledRecyclerPriorityBoostTimeoutFlag, GetRecyclerPriorityBoostTimeoutFlag, SetRecyclerPriorityBoostTimeoutFlag,
IsEnabledRecyclerThreadCollectTimeoutFlag, GetRecyclerThreadCollectTimeoutFlag, SetRecyclerThreadCollectTimeoutFlag,
IsEnabledEnableConcurrentSweepAllocFlag, GetEnableConcurrentSweepAllocFlag, SetEnableConcurrentSweepAllocFlag, IsEnabledecsaFlag, GetecsaFlag, SetecsaFlag,
IsEnabledPageHeapFlag, GetPageHeapFlag, SetPageHeapFlag,
IsEnabledPageHeapAllocStackFlag, GetPageHeapAllocStackFlag, SetPageHeapAllocStackFlag,
IsEnabledPageHeapFreeStackFlag, GetPageHeapFreeStackFlag, SetPageHeapFreeStackFlag,

IsEnabledPageHeapBlockTypeFlag, GetPageHeapBlockTypeFlag, SetPageHeapBlockTypeFlag,
IsEnabledPageHeapDecommitGuardPageFlag, GetPageHeapDecommitGuardPageFlag, SetPageHeapDecommitGuardPageFlag,
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
IsEnabledRuntimeDataOutputFileFlag, GetRuntimeDataOutputFileFlag, SetRuntimeDataOutputFileFlag,
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
IsEnabledMemProtectHeapBackgroundStressFlag, GetMemProtectHeapBackgroundStressFlag, SetMemProtectHeapBackgroundStressFlag,
IsEnabledMemProtectHeapConcurrentStressFlag, GetMemProtectHeapConcurrentStressFlag, SetMemProtectHeapConcurrentStressFlag,
IsEnabledMemProtectHeapConcurrentRepeatStressFlag, GetMemProtectHeapConcurrentRepeatStressFlag, SetMemProtectHeapConcurrentRepeatStressFlag,
IsEnabledMemProtectHeapPartialStressFlag, GetMemProtectHeapPartialStressFlag, SetMemProtectHeapPartialStressFlag,
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
    };
    return ChakraRTInterface::InitializeTestHooks(testHooks);
}

