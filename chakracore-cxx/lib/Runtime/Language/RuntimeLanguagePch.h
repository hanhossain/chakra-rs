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

#include "WasmReader.h"

#include "Language/AsmJsUtils.h"
#include "Language/AsmJsLink.h"
#ifdef ASMJS_PLAT
#include "Language/AsmJsJitTemplate.h"
#include "Language/AsmJsEncoder.h"
#include "Language/AsmJsCodeGenerator.h"
#include "Language/AsmJs.h"
#include "Language/AsmJsModule.h"
#include "Language/AsmJsTypes.h"
#endif

#include "Language/ProfilingHelpers.h"
#include "Language/CacheOperators.h"

#include "Language/JavascriptMathOperators.h"
#include "Language/JavascriptStackWalker.h"
#ifdef DYNAMIC_PROFILE_STORAGE
#include "Language/DynamicProfileStorage.h"
#endif
#include "Language/SourceDynamicProfileManager.h"
#include "Language/SimpleDataCacheWrapper.h"

#include "Base/EtwTrace.h"

#include "Library/ArgumentsObject.h"

#include "Types/TypePropertyCache.h"
#include "Library/JavascriptAsyncFromSyncIterator.h"
#include "Library/CustomExternalWrapperObject.h"
#include "Library/JavascriptProxy.h"
#include "Library/JavascriptSymbolObject.h"
#include "Library/JavascriptGenerator.h"
#include "Library/ScriptFunction.h"
#include "Library/StackScriptFunction.h"
#include "Library/HostObjectBase.h"

#ifdef ENABLE_MUTATION_BREAKPOINT
// REVIEW: ChakraCore Dependency
#include "activdbg_private.h"
#include "Debug/MutationBreakpoint.h"
#endif

// SIMD
#include "Language/SimdOps.h"
#include "Language/SimdUtils.h"

#ifdef ENABLE_SCRIPT_DEBUGGING
#include "Debug/DebuggingFlags.h"
#include "Debug/DiagProbe.h"
#include "Debug/DebugManager.h"
#include "Debug/ProbeContainer.h"
#include "Debug/DebugContext.h"
#endif

// .inl files
#include "Language/CacheOperators.inl"
#include "Language/JavascriptMathOperators.inl"
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
