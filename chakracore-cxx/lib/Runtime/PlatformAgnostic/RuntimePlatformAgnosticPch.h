//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "pal.h"

// Minimal definitions to use AssertMsg in the PAL
#define DbgRaiseAssertionFailure() __builtin_trap()
#define __analysis_assume(x)
#define __inout

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

namespace Js
{
class Throw
{
public:
    static bool ReportAssert(const char* fileName, unsigned int lineNumber, const char* error, const char* message);
    static void LogAssert();
    static void __declspec(noreturn) FatalInternalError();
};
}

#include <Core/Assertions.h>
