//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCommonPch.h"
#include "MathUtil.h"

uint32
Math::NextPowerOf2(uint32 n)
{
    n = n - 1;
    n = n | (n >> 1);
    n = n | (n >> 2);
    n = n | (n >> 4);
    n = n | (n >> 8);
    n = n | (n >> 16);
    n++;
    return n;
}

unsigned long
Math::Rand()
{
    unsigned int rand;
    rand_s(&rand);
    unsigned long newRand = static_cast<unsigned long>(rand);

#if TARGET_64
    rand_s(&rand);
    newRand |= static_cast<unsigned long>(rand) << 32;
#endif

    return newRand;
}

__declspec(noreturn) void Math::DefaultOverflowPolicy()
{
    Js::Throw::OutOfMemory();
}
