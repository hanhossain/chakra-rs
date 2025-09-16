//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DataStructures/ContinuousPageStack.h"

#define NT_SUCCESS(Status) (((int32_t)(Status)) >= 0)
#define MAKE_HR(errnum) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_CONTROL, errnum))

// === C Runtime Header Files ===
#include <time.h>
#include <math.h>

// Exceptions
#include "Exceptions/ExceptionBase.h"
#include "Exceptions/OutOfMemoryException.h"

#include "rterror.h"

// Other Memory headers
#include "Memory/LeakReport.h"
#include "Memory/AutoPtr.h"

// Other core headers
#include "Core/FinalizableObject.h"
#include "Core/ProfileInstrument.h"
#include "Core/ProfileMemory.h"
#include "Core/StackBackTrace.h"

// Inl files
#include "Memory/Recycler.inl"
#include "Memory/MarkContext.inl"
#include "Memory/HeapBucket.inl"
#include "Memory/LargeHeapBucket.inl"
#include "Memory/HeapBlock.inl"
#include "Memory/HeapBlockMap.inl"


// Memory Protections
#define PAGE_EXECUTE_RW_TARGETS_INVALID   (PAGE_EXECUTE_READWRITE)
#define PAGE_EXECUTE_RW_TARGETS_NO_UPDATE (PAGE_EXECUTE_READWRITE)
#define PAGE_EXECUTE_RO_TARGETS_NO_UPDATE (PAGE_EXECUTE_READ)
