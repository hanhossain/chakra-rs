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
#include "JsrtContext.h"
#include "TestHooks.h"

#ifdef __APPLE__
// dummy usage of some Jsrt to force export Jsrt on dylib
#include "ChakraCore.h"
void DummyJSRTCall()
{
    JsRuntimeHandle *runtime;
    JsRuntimeAttributes attr;
    JsCreateRuntime(attr, nullptr, runtime);
    JsDiagStartDebugging(runtime, nullptr, nullptr);
    JsInitializeModuleRecord(nullptr, nullptr, nullptr);
}
#endif

__attribute__ ((visibility ("default")))
EXTERN_C BOOL WINAPI DllMain(HINSTANCE hmod, uint32_t dwReason, void * pvReserved)
{
    // xplat doesn't need DllMain is being called.
    // Instead, we handle attach / detach stuff internally. (see lib/Jsrt/JsrtHelper.cpp)
    // However, PAL/ch will end-up calling here in order to enable TestFlags tooling
#ifdef ENABLE_TEST_HOOKS
    if (dwReason == DLL_PROCESS_ATTACH) OnChakraCoreLoaded();
#endif
    return TRUE;
}

static_assert(__LINE__ == 35, "You shouldn't add anything to this file or ChakraCoreDllFunc.cpp. Please consider again!");
