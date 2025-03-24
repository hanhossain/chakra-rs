//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DataStructures/ContinuousPageStack.h"

#ifdef _WIN32
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#define MAKE_HR(errnum) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_CONTROL, errnum))

// If we're using the PAL for C++ standard library compat,
// we don't need to include wchar for string handling
#ifndef USING_PAL_STDLIB
// === C Runtime Header Files ===
#include <wchar.h>
#include <stdarg.h>
#include <float.h>
#include <limits.h>
#if defined(_UCRT)
#include <cmath>
#else
#include <math.h>
#endif
#include <time.h>
#include <io.h>
#include <malloc.h>
#endif

#include "Common/GetCurrentFrameId.h"
namespace Js
{
    typedef int32 PropertyId;
    typedef uint32 ModuleID;
}

#define IsTrueOrFalse(value)     ((value) ? _u("True") : _u("False"))

// Header files
#include "Core/BinaryFeatureControl.h"
#include "Interface/TemplateParameter.h"

#include "Common/vtinfo.h"
#include "Interface/EnumClassHelp.h"
#include "Common/Tick.h"

#include "Common/IntMathCommon.h"
#include "Common/Int16Math.h"
#include "Common/Int32Math.h"
#include "Common/UInt16Math.h"
#include "Common/UInt32Math.h"
#include "Common/Int64Math.h"
