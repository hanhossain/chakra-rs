//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCommonPch.h"
#include "Common/Int64Math.h"

#if defined(_M_X64)
#if !(__has_builtin(__builtin_mul_overflow) && !(defined(_ARM_) && defined(__clang__))) // _ARM_ && __clang__ linker fails
    static long _mul128(const long left, const long right, long *high) noexcept
    {
        long low;
        __asm__
        (
            "imulq %[right]\n"
            // (I)MUL (Q/64) R[D/A]X <- RAX * r/m64
            : "=d"(*high), "=a"(low)
            : [right]"rm"(right), "1"(left)
        );

        return low;
    }
#endif
#endif

// Returns true if we overflowed, false if we didn't
bool
Int64Math::Mul(long left, long right, long *pResult)
{
#if __has_builtin(__builtin_mul_overflow) && !(defined(_ARM_) && defined(__clang__))
    return IntMathCommon<long>::Mul(left, right, pResult);
#else

#if defined(_M_X64)
    long high;
    *pResult = _mul128(left, right, &high);
    return ((*pResult > 0) && high != 0) || ((*pResult < 0) && (high != -1));
#else
    *pResult = left * right;
    return (left != 0 && right != 0 && (*pResult / left) != right);
#endif

#endif  // !__has_builtin(__builtin_mul_overflow)
}

bool
Int64Math::Shl(long left, long right, long *pResult)
{
    *pResult = left << (right & 63);
    return (left != (long)((unsigned long)*pResult >> right));
}

bool
Int64Math::Shr(long left, long right, long *pResult)
{
    *pResult = left >> (right & 63);
    return false;
}

bool
Int64Math::ShrU(long left, long right, long *pResult)
{
    unsigned long uResult = ((unsigned long)left) >> (right & 63);
    *pResult = uResult;
    return false;
}
