//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// Parser Includes
#include "Common/Common.h"
#include "Runtime.h"

#include "Base/ThreadContext.h"
#include "Library/DelayFreeArrayBufferHelper.h"

#include "Base/ScriptContextProfiler.h"
#include "Base/StackProber.h"

#include "Language/JavascriptConversion.h"

#include "Base/LeaveScriptObject.h"
#include "Base/PropertyRecord.h"
#include "Base/ScriptContext.h"
#include "Base/ScriptContextBase.h"
#include "Base/ScriptContextInfo.h"
#include "Base/ScriptContextOptimizationOverrideInfo.h"

#include "ByteCode/ByteCodeReader.h"
#include "Language/TaggedInt.h"

#include "Library/GlobalObject.h"
#include "Library/RootObjectBase.h"

#include "Library/CompoundString.h"
#include "Library/ConcatString.h"
#include "Library/LiteralString.h"
#include "Library/PropertyRecordUsageCache.h"
#include "Library/PropertyString.h"
#include "Library/SingleCharString.h"

#include "Library/JavascriptArray.h"
#include "Library/JavascriptError.h"
#include "Library/JavascriptSymbol.h"
#include "Library/JavascriptTypedNumber.h"
#include "Library/SparseArraySegment.h"

#include "Library/ArrayBuffer.h"
#include "Library/AtomicsObject.h"
#include "Library/DetachedStateBase.h"
#include "Library/JavascriptBigInt.h"
#include "Library/JavascriptBoolean.h"
#include "Library/SharedArrayBuffer.h"
#include "Library/TypedArray.h"
#include "Library/WebAssembly.h"
#include "Library/WebAssemblyEnvironment.h"
#include "Library/WebAssemblyMemory.h"
#include "Library/WebAssemblyModule.h"
#include "Library/WebAssemblyTable.h"

#include "Language/AsmJsModule.h"
#include "WasmReader.h"

#ifdef ENABLE_WASM
#define TRACE_WASM(condition, ...)                                                                                     \
    if (condition)                                                                                                     \
    {                                                                                                                  \
        Output::Print(__VA_ARGS__);                                                                                    \
        Output::Print(u"\n");                                                                                          \
        Output::Flush();                                                                                               \
    }

// Level of tracing
#define WASM_TRACE_BODY_CHECK(phase) (GetFunctionBody() ? PHASE_TRACE(phase, GetFunctionBody()) : PHASE_TRACE1(phase))
#define DO_WASM_TRACE_BYTECODE WASM_TRACE_BODY_CHECK(Js::WasmBytecodePhase)
#define DO_WASM_TRACE_DECODER WASM_TRACE_BODY_CHECK(Js::WasmReaderPhase)
#define DO_WASM_TRACE_SECTION WASM_TRACE_BODY_CHECK(Js::WasmSectionPhase)
#define TRACE_WASM_BYTECODE(...) TRACE_WASM(DO_WASM_TRACE_BYTECODE, __VA_ARGS__)
#define TRACE_WASM_DECODER(...) TRACE_WASM(DO_WASM_TRACE_DECODER, __VA_ARGS__)
#define TRACE_WASM_SECTION(...) TRACE_WASM(DO_WASM_TRACE_SECTION, __VA_ARGS__)

#include "WasmBinaryReader.h"
#include "WasmByteCodeGenerator.h"
#include "WasmCustomReader.h"
#include "WasmDataSegment.h"
#include "WasmElementSegment.h"
#include "WasmGlobal.h"
#include "WasmReaderBase.h"
#include "WasmSection.h"
#include "WasmSectionLimits.h"

#endif
