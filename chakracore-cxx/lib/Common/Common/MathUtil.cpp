//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCommonPch.h"
#include <random>
#include "MathUtil.h"

uint32_t
Math::NextPowerOf2(uint32_t n)
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
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long> dist;

    return dist(gen);
}

__declspec(noreturn) void Math::DefaultOverflowPolicy()
{
    Js::Throw::OutOfMemory();
}
