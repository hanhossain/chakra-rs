//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

// Parser Includes
#include "Parser.h"
#include "keywords.h"
#include "globals.h"

#include "RegexCommon.h"
#include "DebugWriter.h"
#include "RegexStats.h"
#include "StandardChars.h"
#include "OctoquadIdentifier.h"
#include "RegexCompileTime.h"
#include "RegexParser.h"
#include "RegexPattern.h"

// Runtime includes
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

#include "ByteCode/Symbol.h"
#include "ByteCode/Scope.h"
#include "ByteCode/FuncInfo.h"
#include "ByteCode/ScopeInfo.h"

#include "Library/JavascriptFunction.h"
#include "Language/JavascriptStackWalker.h"
#include "Exceptions/AsmJsParseException.h"
#include "Core/EtwTraceCore.h"
