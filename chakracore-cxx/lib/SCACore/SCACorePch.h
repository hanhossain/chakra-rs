//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if defined(__ANDROID__)
#include <pthread.h>
#endif

#include "Runtime.h"

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
#include "Library/JavascriptRegularExpression.h"
#include "Library/JavascriptProxy.h"
#include "Library/SameValueComparer.h"
#include "Library/MapOrSetDataList.h"
#include "Library/JavascriptMap.h"
#include "Library/JavascriptSet.h"
#include "Library/JavascriptWeakMap.h"
#include "Library/JavascriptWeakSet.h"
// =================

#include "SCATypes.h"
#include "SCAEngine.h"
#include "SCAPropBag.h"
#include "StreamHelper.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include "SCADeserialization.h"
#include "SCASerialization.h"
#include "SCACore.h"
#include "Exceptions/ExceptionBase.h"
#include "Exceptions/JavascriptException.h"
#include "Exceptions/OutOfMemoryException.h"
#include "Exceptions/OperationAbortedException.h"
#include "Exceptions/RejitException.h"
#include "Exceptions/ScriptAbortException.h"
#include "Exceptions/StackOverflowException.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/AsmJsParseException.h"
