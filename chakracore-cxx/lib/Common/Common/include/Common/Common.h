//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Interface/CommonBasic.h"

#define MAKE_HR(errnum) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_CONTROL, errnum))

#include "Common/GetCurrentFrameId.h"
namespace Js
{
    typedef int32 PropertyId;
    typedef uint32 ModuleID;
}
