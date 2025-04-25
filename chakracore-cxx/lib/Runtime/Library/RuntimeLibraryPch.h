//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Util/Pinned.h"

#ifndef IsJsDiag
#include "Parser.h"
#include "RegexCommon.h"
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

#include "Library/CustomExternalWrapperObject.h"

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

#include "Base/EtwTrace.h"

#include "Library/JavascriptNumberObject.h"
#include "Library/JavascriptStringObject.h"
#include "Library/JavascriptBooleanObject.h"
#include "Library/JavascriptBigIntObject.h"

#include "Library/ObjectPrototypeObject.h"

#include "Common/ByteSwap.h"
#include "Library/DataView.h"

#include "Library/LazyJSONString.h"
#include "Library/JSONStringBuilder.h"
#include "Library/JSONStringifier.h"
#include "Library/ProfileString.h"
#include "Library/SingleCharString.h"
#include "Library/SubString.h"
#include "Library/BufferStringBuilder.h"

#include "Library/BoundFunction.h"
#include "Library/JavascriptGeneratorFunction.h"
#include "Library/JavascriptAsyncFunction.h"
#include "Library/JavascriptAsyncGeneratorFunction.h"

#include "Library/RegexHelper.h"
#include "Library/JavascriptRegularExpression.h"
#include "Library/JavascriptRegExpConstructor.h"
#include "Library/JavascriptRegularExpressionResult.h"

#include "Library/JavascriptAsyncFromSyncIterator.h"
#include "Library/JavascriptPromise.h"
#include "Library/JavascriptSymbolObject.h"
#include "Library/CustomExternalWrapperObject.h"
#include "Library/JavascriptProxy.h"
#include "Library/JavascriptReflect.h"
#include "Library/JavascriptGenerator.h"
#include "Library/JavascriptAsyncGenerator.h"

#include "Library/SameValueComparer.h"
#include "Library/MapOrSetDataList.h"
#include "Library/JavascriptMap.h"
#include "Library/JavascriptSet.h"
#include "Library/JavascriptWeakMap.h"
#include "Library/JavascriptWeakSet.h"

#include "Types/UnscopablesWrapperObject.h"
#include "Types/PropertyIndexRanges.h"
#include "Types/DictionaryPropertyDescriptor.h"
#include "Types/DictionaryTypeHandler.h"
#include "Types/ES5ArrayTypeHandler.h"
#include "Library/ES5Array.h"

#include "Library/JavascriptArrayIndexEnumeratorBase.h"
#include "Library/JavascriptArrayIndexEnumerator.h"
#include "Library/JavascriptArrayIndexSnapshotEnumerator.h"
#include "Library/JavascriptArrayIndexStaticEnumerator.h"
#include "Library/ES5ArrayIndexEnumerator.h"
#include "Library/ES5ArrayIndexStaticEnumerator.h"
#include "Library/TypedArrayIndexEnumerator.h"
#include "Library/JavascriptStringEnumerator.h"
#include "Library/JavascriptRegExpEnumerator.h"

#include "Library/JavascriptIterator.h"
#include "Library/JavascriptArrayIterator.h"
#include "Library/JavascriptMapIterator.h"
#include "Library/JavascriptSetIterator.h"
#include "Library/JavascriptStringIterator.h"
#include "Library/JavascriptListIterator.h"

#include "Library/UriHelper.h"
#include "Library/HostObjectBase.h"

#include "Library/DateImplementation.h"
#include "Library/JavascriptDate.h"

#include "Library/ModuleRoot.h"
#include "Library/ArgumentsObject.h"
// SIMD
#include "Language/SimdOps.h"

#include "Library/WebAssemblyInstance.h"

#include "Language/JavascriptStackWalker.h"
#include "Language/CacheOperators.h"
#include "Types/TypePropertyCache.h"
// .inl files
#include "Library/JavascriptString.inl"
#include "Library/ConcatString.inl"
#include "Language/CacheOperators.inl"

#endif // !IsJsDiag

#ifdef IsJsDiag
#define JS_DIAG_INLINE inline
#else
#define JS_DIAG_INLINE
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
