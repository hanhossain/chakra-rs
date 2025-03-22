//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Interface/CommonBasic.h"

// === C Runtime Header Files ===
#ifndef USING_PAL_STDLIB
#pragma warning(push)
#pragma warning(disable: 4995) /* 'function': name was marked as #pragma deprecated */
#include <stdio.h>
#pragma warning(pop)
#ifdef _WIN32
#include <intrin.h>
#endif
#endif

// === Core Header Files ===
#include "Interface/EnumHelp.h"
#include "Core/Assertions.h"
#include "Core/CriticalSection.h"
#include "Core/SysInfo.h"

#include "Core/PerfCounter.h"
#include "Core/PerfCounterSet.h"

#include "Common/MathUtil.h"
#include "Core/AllocSizeMath.h"
#include "Core/FaultInjection.h"

#include "Core/BasePtr.h"
#include "Core/AutoFile.h"
