//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Runtime.h"
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

#include "Language/CacheOperators.h"
#include "Language/InlineCachePointerArray.h"
#include "Types/UnscopablesWrapperObject.h"
#include "Types/TypePropertyCache.h"
#include "Types/MissingPropertyTypeHandler.h"
#include "Types/PathTypeHandler.h"
#include "Types/PropertyIndexRanges.h"
#include "Types/SimpleDictionaryPropertyDescriptor.h"
#include "Types/SimpleDictionaryTypeHandler.h"
#include "Types/SimpleDictionaryUnorderedTypeHandler.h"
#include "Types/DictionaryPropertyDescriptor.h"
#include "Types/DictionaryTypeHandler.h"
#include "Types/ES5ArrayTypeHandler.h"
#include "Util/Pinned.h"

// .inl files
#include "Language/CacheOperators.inl"

