//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DelayLoadLibrary.h"

class GlobalSecurityPolicy : private DelayLoadLibrary
{
public:
    static bool IsCFGEnabled() { return false; }
};
