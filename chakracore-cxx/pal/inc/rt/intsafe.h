// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/******************************************************************
*                                                                 *
*  intsafe.h -- This module defines helper functions to prevent   *
*               integer overflow issues.                          *
*                                                                 *
*                                                                 *
******************************************************************/
#ifndef _INTSAFE_H_INCLUDED_
#define _INTSAFE_H_INCLUDED_

#include "specstrings.h"    // for IN, etc.

#include <limits.h>

#define INTSAFE_E_ARITHMETIC_OVERFLOW       ((int32_t)0x80070216L)  // 0x216 = 534 = ERROR_ARITHMETIC_OVERFLOW

//
// We make some assumptions about the sizes of various types. Let's be
// explicit about those assumptions and check them.
//
C_ASSERT(sizeof(unsigned short) == 2);
C_ASSERT(sizeof(unsigned int) == 4);
C_ASSERT(sizeof(uint32_t) == 4);

//
// uint32_t -> INT conversion
//
__inline
int32_t
UIntToInt(
     uint32_t uOperand,
     int32_t* piResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *piResult = -1;

    if (uOperand <= INT_MAX)
    {
        *piResult = (int32_t)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> int32_t conversion
//
__inline
int32_t
ULongToLong(
     uint32_t Operand,
     int32_t* Result)
{
    if (Operand <= INT_MAX)
    {
        *Result = (int32_t)Operand;
        return S_OK;
    }
    else
    {
        *Result = -1L;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// unsigned long -> INT conversion
//
__inline
int32_t
ULongLongToInt(
     unsigned long ullOperand,
     int32_t* piResult)
{
    if (ullOperand <= INT_MAX)
    {
        *piResult = (int32_t)ullOperand;
        return S_OK;
    }
    else
    {
        *piResult = -1;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// unsigned long -> uint32_t conversion
//
__inline
int32_t
ULongLongToULong(
     unsigned long ullOperand,
     uint32_t* pulResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = 0xffffffffUL;

    if (ullOperand <= UINT_MAX)
    {
        *pulResult = (uint32_t)ullOperand;
        hr = S_OK;
    }

    return hr;
}

//
// unsigned long -> uint32_t conversion
//
__inline
int32_t
ULongLongToUInt(
     unsigned long ullOperand,
     uint32_t* puResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = 0xffffffff;

    if (ullOperand <= 0xffffffff)
    {
        *puResult = (uint32_t)ullOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t addition
//
__inline
int32_t
UIntAdd(
     uint32_t uAugend,
     uint32_t uAddend,
     uint32_t* puResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = 0xffffffff;

    if ((uAugend + uAddend) >= uAugend)
    {
        *puResult = (uAugend + uAddend);
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t addition
//
__inline
int32_t
ULongAdd(
     uint32_t ulAugend,
     uint32_t ulAddend,
     uint32_t* pulResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = 0xffffffffUL;

    if ((ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = (ulAugend + ulAddend);
        hr = S_OK;
    }

    return hr;
}

//
// size_t addition
//
__inline
int32_t
SizeTAdd(
     size_t Augend,
     size_t Addend,
     size_t* pResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = 0xffffffffffffffffULL;

    if ((Augend + Addend) >= Augend)
    {
        *pResult = (Augend + Addend);
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t multiplication
//
__inline
int32_t
UIntMult(
     uint32_t uMultiplicand,
     uint32_t uMultiplier,
     uint32_t* puResult)
{
    unsigned long ull64Result = ((unsigned long)uMultiplicand * (unsigned long)uMultiplier);
    return ULongLongToUInt(ull64Result, puResult);
}

//
// uint32_t multiplication
//
__inline
int32_t
ULongMult(
     uint32_t ulMultiplicand,
     uint32_t ulMultiplier,
     uint32_t* pulResult)
{
    unsigned long ull64Result = ((unsigned long)ulMultiplicand * (unsigned long)ulMultiplier);
    return ULongLongToULong(ull64Result, pulResult);
}

#endif // _INTSAFE_H_INCLUDED_
