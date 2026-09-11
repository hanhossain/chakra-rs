//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <rust/cxx.h>

struct TestHooks
{
    static int32_t SetConfigFlags(const rust::Vec<rust::String> &vargs);
    static int32_t SetAssertToConsoleFlag(bool flag);
};
