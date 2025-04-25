//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// Parser Includes
#include "Common/Common.h"
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

#if ENABLE_TTD
#include "screrror.h"

#include "Debug/TTRuntimeInfoTracker.h"
#include "Debug/TTExecutionInfo.h"
#include "Debug/TTInflateMap.h"
#include "Debug/TTSnapTypes.h"
#include "Debug/TTSnapValues.h"
#include "Debug/TTSnapObjects.h"
#include "Debug/TTSnapshot.h"
#include "Debug/TTSnapshotExtractor.h"
#include "Debug/TTEvents.h"
#include "Debug/TTActionEvents.h"
#include "Debug/TTEventLog.h"
#endif

#include "Language/AsmJsModule.h"
#include "WasmReader.h"

#ifdef ENABLE_WASM
#if ENABLE_DEBUG_CONFIG_OPTIONS
#define TRACE_WASM(condition, ...) \
    if (condition)\
    {\
        Output::Print(__VA_ARGS__); \
        Output::Print(_u("\n")); \
        Output::Flush(); \
    }

// Level of tracing
#define WASM_TRACE_BODY_CHECK(phase) (GetFunctionBody() ? PHASE_TRACE(phase, GetFunctionBody()) : PHASE_TRACE1(phase))
#define DO_WASM_TRACE_BYTECODE WASM_TRACE_BODY_CHECK(Js::WasmBytecodePhase)
#define DO_WASM_TRACE_DECODER  WASM_TRACE_BODY_CHECK(Js::WasmReaderPhase)
#define DO_WASM_TRACE_SECTION  WASM_TRACE_BODY_CHECK(Js::WasmSectionPhase)
#else
#define TRACE_WASM(...)
#define DO_WASM_TRACE_BYTECODE (false)
#define DO_WASM_TRACE_DECODER (false)
#define DO_WASM_TRACE_SECTION (false)
#endif
#define TRACE_WASM_BYTECODE(...) TRACE_WASM(DO_WASM_TRACE_BYTECODE, __VA_ARGS__)
#define TRACE_WASM_DECODER(...) TRACE_WASM(DO_WASM_TRACE_DECODER, __VA_ARGS__)
#define TRACE_WASM_SECTION(...) TRACE_WASM(DO_WASM_TRACE_SECTION, __VA_ARGS__)

#include "WasmSection.h"
#include "WasmReaderBase.h"
#include "WasmBinaryReader.h"
#include "WasmCustomReader.h"
#include "WasmGlobal.h"
#include "WasmDataSegment.h"
#include "WasmElementSegment.h"
#include "WasmByteCodeGenerator.h"
#include "WasmSectionLimits.h"

#endif
