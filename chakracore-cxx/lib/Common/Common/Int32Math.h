//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "IntMathCommon.h"

class Int32Math: public IntMathCommon<int32_t>
{
public:
    static bool Add(int32_t left, int32_t right, int32_t *pResult);
    static bool Mul(int32_t left, int32_t right, int32_t *pResult);
    static bool Mul(int32_t left, int32_t right, int32_t *pResult, int32_t* pOverflowValue);

    static bool Shl(int32_t left, int32_t right, int32_t *pResult);
    static bool Shr(int32_t left, int32_t right, int32_t *pResult);
    static bool ShrU(int32_t left, int32_t right, int32_t *pResult);
};

template <>
inline bool Math::IncImpl<int32_t>(int32_t val, int32_t *pResult)
{
    return Int32Math::Inc(val, pResult);
}

template <>
inline bool Math::AddImpl<int32_t>(int32_t left, int32_t right, int32_t *pResult)
{
    return Int32Math::Add(left, right, pResult);
}
