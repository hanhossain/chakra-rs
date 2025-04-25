//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Parser.h"

#include "Runtime.h"

#include "Language/ModuleRecordBase.h"
#include "Language/SourceTextModuleRecord.h"

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

#include "ByteCode/Symbol.h"
#include "ByteCode/Scope.h"
#include "ByteCode/FuncInfo.h"
#include "ByteCode/ScopeInfo.h"
#include "ByteCode/StatementReader.h"

#include "ByteCode/ByteCodeDumper.h"
#include "ByteCode/ByteCodeWriter.h"
#include "ByteCode/AsmJsByteCodeWriter.h"
#include "ByteCode/ByteCodeGenerator.h"

#include "ByteCode/OpCodeUtilAsmJs.h"
#include "Language/AsmJsTypes.h"

#include "ByteCode/ByteCodeApi.h"
#include "ByteCode/BackendOpCodeAttr.h"
#ifdef ENABLE_WASM
#include "WasmReader.h"
#endif
#include "Exceptions/ExceptionBase.h"
#include "Exceptions/JavascriptException.h"
#include "Exceptions/OutOfMemoryException.h"
#include "Exceptions/OperationAbortedException.h"
#include "Exceptions/RejitException.h"
#include "Exceptions/ScriptAbortException.h"
#include "Exceptions/StackOverflowException.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/AsmJsParseException.h"
#include "Core/EtwTraceCore.h"
