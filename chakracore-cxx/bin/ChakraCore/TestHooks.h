//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

#include <oaidl.h>
interface ICustomConfigFlags;

struct TestHooks
{
    using SetConfigFlagsPtr = int32_t(*)(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags);
    typedef int32_t(*PrintConfigFlagsUsageStringPtr)(void);
    typedef int32_t(*SetAssertToConsoleFlagPtr)(bool flag);
    typedef int(*LogicalStringCompareImpl)(const char16_t* p1, int p1size, const char16_t* p2, int p2size);

    SetConfigFlagsPtr pfSetConfigFlags;
    PrintConfigFlagsUsageStringPtr pfPrintConfigFlagsUsageString;
    SetAssertToConsoleFlagPtr pfSetAssertToConsoleFlag;
    LogicalStringCompareImpl pfLogicalCompareStringImpl;

    // Javasscript Bigint hooks
    typedef digit_t(*AddDigit)(digit_t a, digit_t b, digit_t* carry);
    typedef digit_t(*SubDigit)(digit_t a, digit_t b, digit_t* borrow);
    typedef digit_t(*MulDigit)(digit_t a, digit_t b, digit_t* high);
    AddDigit pfAddDigit;
    SubDigit pfSubDigit;
    MulDigit pfMulDigit;
};

int32_t OnChakraCoreLoaded();

