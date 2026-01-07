//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeMathPch.h"

namespace Js
{
    // These implementations need to be compiled with /arch:sse2
#pragma prefast(push)
#pragma prefast(disable:26450) // PREfast takes issue with how INT_MIN is defined

    // Double
    template<> double AsmJsMath::DivChecked<double>(double aLeft, double aRight) { return aLeft / aRight; }
    template<> double AsmJsMath::DivUnsafe<double>(double aLeft, double aRight) { return aLeft / aRight; }
    template<> double AsmJsMath::Mul<double>(double aLeft, double aRight) { return aLeft * aRight; }
    template<> double AsmJsMath::RemChecked<double>(double aLeft, double aRight) { return NumberUtilities::Modulus(aLeft, aRight); }
    template<> double AsmJsMath::RemUnsafe<double>(double aLeft, double aRight) { return NumberUtilities::Modulus(aLeft, aRight); }

    // Float
    template<> float AsmJsMath::DivChecked<float>(float aLeft, float aRight) { return aLeft / aRight; }
    template<> float AsmJsMath::DivUnsafe<float>(float aLeft, float aRight) { return aLeft / aRight; }
    template<> float AsmJsMath::Mul<float>(float aLeft, float aRight) { return aLeft * aRight; }

    // Int32
    template<> int32_t AsmJsMath::Mul<int32_t>(int32_t aLeft, int32_t aRight) { return aLeft * aRight; }
    template<> int32_t AsmJsMath::DivUnsafe<int32_t>(int32_t aLeft, int32_t aRight) { return aLeft / aRight; }
    template<>
    int32_t AsmJsMath::DivChecked<int32_t>(int32_t aLeft, int32_t aRight)
    {
        return aRight == 0 ? 0 : (aLeft == INT_MIN && aRight == -1) ? INT_MIN : aLeft / aRight;
    }
    template<> bool AsmJsMath::DivWouldTrap(int32_t aLeft, int32_t aRight) { return aRight == 0 || (aLeft == INT_MIN && aRight == -1); }
    template<> int32_t AsmJsMath::RemUnsafe<int32_t>(int32_t aLeft, int32_t aRight) { return aLeft % aRight; }
    template<>
    int32_t AsmJsMath::RemChecked<int32_t>(int32_t aLeft, int32_t aRight)
    {
        return ((aRight == 0) || (aLeft == INT_MIN && aRight == -1)) ? 0 : aLeft % aRight;
    }
    template<> bool AsmJsMath::RemWouldTrap(int32_t aLeft, int32_t aRight) { return aRight == 0 || (aLeft == INT_MIN && aRight == -1); }

    // Unsigned Int32
    template<> uint32_t AsmJsMath::Mul<uint32_t>(uint32_t aLeft, uint32_t aRight) { return aLeft * aRight; }
    template<> uint32_t AsmJsMath::DivUnsafe<uint32_t>(uint32_t aLeft, uint32_t aRight) { return aLeft / aRight; }
    template<>
    uint32_t AsmJsMath::DivChecked<uint32_t>(uint32_t aLeft, uint32_t aRight)
    {
        return aRight == 0 ? 0 : aLeft / aRight;
    }
    template<> bool AsmJsMath::DivWouldTrap(uint32_t aLeft, uint32_t aRight) { return aRight == 0; }
    template<> uint32_t AsmJsMath::RemUnsafe<uint32_t>(uint32_t aLeft, uint32_t aRight) { return aLeft % aRight; }
    template<>
    uint32_t AsmJsMath::RemChecked<uint32_t>(uint32_t aLeft, uint32_t aRight)
    {
        return aRight == 0 ? 0 : (aLeft == INT_MIN && aRight == -1) ? INT_MIN : aLeft % aRight;
    }
    template<> bool AsmJsMath::RemWouldTrap(uint32_t aLeft, uint32_t aRight) { return aRight == 0 || (aLeft == INT_MIN && aRight == -1); }

    // Int64
    template<> long AsmJsMath::Mul<long>(long aLeft, long aRight) { return aLeft * aRight; }
    template<> long AsmJsMath::DivUnsafe<long>(long aLeft, long aRight) { return aLeft / aRight; }
    template<>
    long AsmJsMath::DivChecked<long>(long aLeft, long aRight)
    {
        return aRight == 0 ? 0 : (aLeft == INT64_MIN && aRight == -1) ? INT64_MIN : aLeft / aRight;
    }
    template<> bool AsmJsMath::DivWouldTrap(long aLeft, long aRight) { return aRight == 0 || (aLeft == INT64_MIN && aRight == -1); }
    template<> long AsmJsMath::RemUnsafe<long>(long aLeft, long aRight) { return aLeft % aRight; }
    template<>
    long AsmJsMath::RemChecked<long>(long aLeft, long aRight)
    {
        return ((aRight == 0) || (aLeft == INT64_MIN && aRight == -1)) ? 0 : aLeft % aRight;
    }
    template<> bool AsmJsMath::RemWouldTrap(long aLeft, long aRight) { return aRight == 0 || (aLeft == INT64_MIN && aRight == -1); }

    // Unsigned Int64
    template<> unsigned long AsmJsMath::Mul<unsigned long>(unsigned long aLeft, unsigned long aRight) { return aLeft * aRight; }
    template<> unsigned long AsmJsMath::DivUnsafe<unsigned long>(unsigned long aLeft, unsigned long aRight) { return aLeft / aRight; }
    template<>
    unsigned long AsmJsMath::DivChecked<unsigned long>(unsigned long aLeft, unsigned long aRight)
    {
        return aRight == 0 ? 0 : aLeft / aRight;
    }
    template<> bool AsmJsMath::DivWouldTrap(unsigned long aLeft, unsigned long aRight) { return aRight == 0; }
    template<> unsigned long AsmJsMath::RemUnsafe<unsigned long>(unsigned long aLeft, unsigned long aRight) { return aLeft % aRight; }
    template<>
    unsigned long AsmJsMath::RemChecked<unsigned long>(unsigned long aLeft, unsigned long aRight)
    {
        return aRight == 0 ? 0 : (aLeft == INT64_MIN && aRight == -1) ? INT64_MIN : aLeft % aRight;
    }
    template<> bool AsmJsMath::RemWouldTrap(unsigned long aLeft, unsigned long aRight) { return aRight == 0 || (aLeft == INT64_MIN && aRight == -1); }
}
#pragma prefast(pop
