//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <limits.h>

#define _u(s) u##s

typedef char16_t wchar;
typedef unsigned int uint;
typedef unsigned short ushort;

typedef signed char sbyte;
// TODO (hanhossain): remove (u)int typedefs
typedef int16_t int16;
typedef int32_t int32;

typedef unsigned char byte;

typedef uint16_t uint16;
typedef uint32_t uint32;

// charcount_t represents a count of characters in a JavascriptString
// It is unsigned and the maximum value is (INT_MAX-1)
typedef uint32_t charcount_t;

//A Unicode code point
typedef uint32_t codepoint_t;
const codepoint_t INVALID_CODEPOINT = (codepoint_t)-1;

// Synonym for above, 2^31-1 is used as the limit to protect against addition overflow
typedef uint32_t CharCount;
const CharCount MaxCharCount = INT_MAX-1;
// As above, but 2^32-1 is used to signal a 'flag' condition (e.g. undefined)
typedef uint32_t CharCountOrFlag;
const CharCountOrFlag CharCountFlag = (CharCountOrFlag)-1;

#define QUOTE(s) #s
#define STRINGIZE(s) QUOTE(s)
#define STRINGIZEW(s) _u(#s)

namespace Js
{
    typedef uint32_t LocalFunctionId;
};

// digit_t represents a digit in bigint underline
typedef uintptr_t digit_t;
