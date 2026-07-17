//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "pal.h"

// Minimal definitions to use AssertMsg in the PAL
#define DbgRaiseAssertionFailure() __builtin_trap()

#ifndef USE_ICU
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
#else
#define U_USING_ICU_NAMESPACE 0
#include <unicode/umachine.h>
#endif

#include <Core/Assertions.h>
