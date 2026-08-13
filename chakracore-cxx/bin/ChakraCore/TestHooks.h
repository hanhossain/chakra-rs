//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

interface ICustomConfigFlags;

struct TestHooks
{
    typedef int(*LogicalStringCompareImpl)(const char16_t* p1, int p1size, const char16_t* p2, int p2size);

    LogicalStringCompareImpl pfLogicalCompareStringImpl;

    // Javasscript Bigint hooks
    typedef digit_t(*AddDigit)(digit_t a, digit_t b, digit_t* carry);
    typedef digit_t(*SubDigit)(digit_t a, digit_t b, digit_t* borrow);
    typedef digit_t(*MulDigit)(digit_t a, digit_t b, digit_t* high);
    AddDigit pfAddDigit;
    SubDigit pfSubDigit;
    MulDigit pfMulDigit;

    static int32_t SetConfigFlags(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags);
    static int32_t SetAssertToConsoleFlag(bool flag);
    static int32_t PrintConfigFlagsUsageString();
};

int32_t OnChakraCoreLoaded(const std::vector<std::u16string> &vargs);

