//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

bool
Int32Math::Add(int32_t left, int32_t right, int32_t *pResult)
{
#if __has_builtin(__builtin_add_overflow)
    return IntMathCommon<int32_t>::Add(left, right, pResult);
#else

    Assert(sizeof(void *) == 8);
    long result64 = (long)left + (long)right;
    *pResult = (int32_t)result64;
    return result64 != (long)(*pResult);

#endif
}

bool
Int32Math::Mul(int32_t left, int32_t right, int32_t *pResult)
{
#if __has_builtin(__builtin_mul_overflow) && !(defined(_ARM_) && defined(__clang__))
    return IntMathCommon<int32_t>::Mul(left, right, pResult);
#else

    bool fOverflow;
    long result64 = (long)left * (long)right;

    *pResult = (int32_t)result64;

    fOverflow = (result64 != (long)(*pResult));

    return fOverflow;

#endif  // !__has_builtin(__builtin_mul_overflow)
}

bool
Int32Math::Mul(int32_t left, int32_t right, int32_t *pResult, int32_t* pOverflowValue)
{
    bool fOverflow;
    long result64 = static_cast<long>(left) * static_cast<long>(right);

    *pResult = static_cast<int32_t>(result64);
    *pOverflowValue = static_cast<int32_t>(result64 >> 32);

    fOverflow = (result64 != static_cast<long>(*pResult));

    return fOverflow;
}

bool
Int32Math::Shl(int32_t left, int32_t right, int32_t *pResult)
{
    *pResult = left << (right & 0x1F);
    return (left != static_cast<int32_t>(static_cast<uint32_t>(*pResult) >> right));
}

bool
Int32Math::Shr(int32_t left, int32_t right, int32_t *pResult)
{
    *pResult = left >> (right & 0x1F);
    return false;
}

bool
Int32Math::ShrU(int32_t left, int32_t right, int32_t *pResult)
{
    uint32_t uResult = static_cast<uint32_t>(left) >> (right & 0x1F);
    *pResult = uResult;
    return false;
}
