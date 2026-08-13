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
    static int32_t SetConfigFlags(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags);
    static int32_t SetAssertToConsoleFlag(bool flag);
    static int32_t PrintConfigFlagsUsageString();
};
