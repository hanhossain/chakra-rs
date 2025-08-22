//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Common/IntMathCommon.h"

class Int64Math: public IntMathCommon<long>
{
public:
    static bool Mul(long left, long right, long *pResult);
    static bool Shl(long left, long right, long *pResult);
    static bool Shr(long left, long right, long *pResult);
    static bool ShrU(long left, long right, long *pResult);
};
