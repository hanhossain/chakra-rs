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

#define INTSAFE_E_ARITHMETIC_OVERFLOW       ((int32_t)0x80070216L)  // 0x216 = 534 = ERROR_ARITHMETIC_OVERFLOW

#define INT_MAX         2147483647
#define LONG_MAX        2147483647L
#define USHRT_MAX       0xffff
#define UINT_MAX        0xffffffff
#define ULONG_MAX       0xffffffffUL
#define DWORD_MAX       0xffffffffUL

//
// We make some assumptions about the sizes of various types. Let's be
// explicit about those assumptions and check them.
//
C_ASSERT(sizeof(unsigned short) == 2);
C_ASSERT(sizeof(unsigned int) == 4);
C_ASSERT(sizeof(uint32_t) == 4);

//
// INT -> signed char conversion
//
__inline
int32_t
IntToSignedChar(
     int32_t iOperand,
     signed char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((iOperand >= -128) && (iOperand <= 127))
    {
        *pch = (signed char)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> unsigned char conversion
//
__inline
int32_t
IntToUChar(
     int32_t iOperand,
     unsigned char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((iOperand >= 0) && (iOperand <= 255))
    {
        *pch = (unsigned char)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// int32_t -> unsigned char conversion
//
__inline
int32_t
LongToUChar(
     int32_t lOperand,
     unsigned char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((lOperand >= 0) && (lOperand <= 255))
    {
        *pch = (unsigned char)lOperand;
        hr = S_OK;
    }

    return hr;
}

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// INT -> char conversion
//
inline
int32_t
IntToChar(
     int32_t iOperand,
     char* pch)
{
#ifdef _CHAR_UNSIGNED
    return IntToUChar(iOperand, (unsigned char*)pch);
#else
    return IntToSignedChar(iOperand, (signed char*)pch);
#endif
}

//
// INT -> unsigned short conversion
//
__inline
int32_t
IntToUShort(
     int32_t iOperand,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = 0xffff;

    if ((iOperand >= 0) && (iOperand <= USHRT_MAX))
    {
        *pusResult = (unsigned short)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> uint32_t conversion
//
__inline
int32_t
IntToUInt(
     int32_t iOperand,
     uint32_t* puResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = 0xffffffff;

    if (iOperand >= 0)
    {
        *puResult = (uint32_t)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> uint32_t conversion
//
__inline
int32_t
IntToULong(
     int32_t iOperand,
     uint32_t* pulResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = 0xffffffffUL;

    if (iOperand >= 0)
    {
        *pulResult = (uint32_t)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> unsigned long conversion
//
__inline
int32_t
IntToULongLong(
     int32_t iOperand,
     unsigned long* pullResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = 0xffffffffUL;

    if (iOperand >= 0)
    {
        *pullResult = (unsigned long)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> signed char conversion
//
__inline
int32_t
UIntToSignedChar(
     uint32_t uOperand,
     signed char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (uOperand <= 127)
    {
        *pch = (signed char)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> unsigned char conversion
//
__inline
int32_t
UIntToUChar(
     uint32_t uOperand,
     unsigned char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (uOperand <= 255)
    {
        *pch = (unsigned char)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// uint32_t -> char conversion
//
inline
int32_t
UIntToChar(
     uint32_t uOperand,
     char* pch)
{
#ifdef _CHAR_UNSIGNED
    return UIntToUChar(uOperand, (unsigned char*)pch);
#else
    return UIntToSignedChar(uOperand, (signed char*)pch);
#endif // _CHAR_UNSIGNED
}

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
UIntToLong(
     uint32_t Operand,
     int32_t* Result)
{
    if (Operand <= LONG_MAX)
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
// uint32_t -> uint32_t conversion
//
__inline
int32_t
UIntToULong(
     uint32_t uOperand,
     uint32_t* pulResult)
{
    *pulResult = (uint32_t)uOperand;

    return S_OK;
}

//
// uint32_t -> unsigned char conversion
//
__inline
int32_t
ULongToSignedChar(
     uint32_t ulOperand,
     signed char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (ulOperand <= 127)
    {
        *pch = (signed char)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> unsigned char conversion
//
__inline
int32_t
ULongToUChar(
     uint32_t ulOperand,
     unsigned char* pch)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (ulOperand <= 255)
    {
        *pch = (unsigned char)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// uint32_t -> char conversion
//
inline
int32_t
ULongToChar(
     uint32_t ulOperand,
     char* pch)
{
#ifdef _CHAR_UNSIGNED
    return ULongToUChar(ulOperand, (unsigned char*)pch);
#else
    return ULongToSignedChar(ulOperand, (signed char*)pch);
#endif // _CHAR_UNSIGNED
}

//
// uint32_t -> unsigned short conversion
//
__inline
int32_t
ULongToUShort(
     uint32_t ulOperand,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = 0xffff;

    if (ulOperand <= USHRT_MAX)
    {
        *pusResult = (unsigned short)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> INT conversion
//
__inline
int32_t
ULongToInt(
     uint32_t ulOperand,
     int32_t* piResult)
{
    if (ulOperand <= INT_MAX)
    {
        *piResult = (int32_t)ulOperand;
        return S_OK;
    }
    else
    {
        *piResult = -1;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// uint32_t -> uint32_t conversion
//
__inline
int32_t
ULongToUInt(
     uint32_t ulOperand,
     uint32_t* puResult)
{
    *puResult = (uint32_t)ulOperand;

    return S_OK;
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
    if (Operand <= LONG_MAX)
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
// unsigned long -> int32_t conversion
//
__inline
int32_t
ULongLongToLong(
     unsigned long Operand,
     int32_t* Result)
{
    if (Operand <= LONG_MAX)
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
// uint32_t -> unsigned short conversion
//
__inline
int32_t
UIntToUShort(
     uint32_t uOperand,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = 0xffff;

    if (uOperand <= USHRT_MAX)
    {
        *pusResult = (unsigned short)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// unsigned long -> unsigned short conversion
//
__inline
int32_t
ULongLongToUShort(
     unsigned long ullOperand,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    unsigned short usResult = 0xffff;

    if (ullOperand <= USHRT_MAX)
    {
        usResult = (unsigned short)ullOperand;
        hr = S_OK;
    }
    *pusResult = usResult;

    return hr;
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

    if (ullOperand <= ULONG_MAX)
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

    if (ullOperand <= UINT_MAX)
    {
        *puResult = (uint32_t)ullOperand;
        hr = S_OK;
    }

    return hr;
}

__inline
int32_t
ULongLongToUIntPtr(
     unsigned long ullOperand,
     UINT_PTR* puResult)
{
	*puResult = ullOperand;
	return S_OK;
}

__inline
int32_t
ULongLongToULongPtr(
     unsigned long ullOperand,
     size_t* pulResult)
{
	*pulResult = ullOperand;
	return S_OK;
}

//
// unsigned short addition
//
__inline
int32_t
UShortAdd(
     unsigned short usAugend,
     unsigned short usAddend,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = 0xffff;

    if (((unsigned short)(usAugend + usAddend)) >= usAugend)
    {
        *pusResult = (usAugend + usAddend);
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
// unsigned long addition
//
__inline
int32_t
ULongLongAdd(
     unsigned long ullAugend,
     unsigned long ullAddend,
     unsigned long* pullResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = 0xffffffffffffffffULL;

    if ((ullAugend + ullAddend) >= ullAugend)
    {
        *pullResult = (ullAugend + ullAddend);
        hr = S_OK;
    }

    return hr;
}

//
// unsigned short subtraction
//
__inline
int32_t
UShortSub(
     unsigned short usMinuend,
     unsigned short usSubtrahend,
     unsigned short* pusResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = 0xffff;

    if (usMinuend >= usSubtrahend)
    {
        *pusResult = (usMinuend - usSubtrahend);
        hr = S_OK;
    }

    return hr;
}


//
// uint32_t subtraction
//
__inline
int32_t
UIntSub(
     uint32_t uMinuend,
     uint32_t uSubtrahend,
     uint32_t* puResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = 0xffffffff;

    if (uMinuend >= uSubtrahend)
    {
        *puResult = (uMinuend - uSubtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t subtraction
//
__inline
int32_t
ULongSub(
     uint32_t ulMinuend,
     uint32_t ulSubtrahend,
     uint32_t* pulResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = 0xffffffffUL;

    if (ulMinuend >= ulSubtrahend)
    {
        *pulResult = (ulMinuend - ulSubtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// size_t subtraction
//
__inline
int32_t
SizeTSub(
     size_t Minuend,
     size_t Subtrahend,
     size_t* pResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = 0xffffffffffffffffULL;

    if (Minuend >= Subtrahend)
    {
        *pResult = (Minuend - Subtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// unsigned long subtraction
//
__inline
int32_t
ULongLongSub(
     unsigned long ullMinuend,
     unsigned long ullSubtrahend,
     unsigned long* pullResult)
{
    int32_t hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = 0xffffffffffffffffULL;

    if (ullMinuend >= ullSubtrahend)
    {
        *pullResult = (ullMinuend - ullSubtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// unsigned short multiplication
//
__inline
int32_t
UShortMult(
     unsigned short usMultiplicand,
     unsigned short usMultiplier,
     unsigned short* pusResult)
{
    uint32_t ulResult = ((uint32_t)usMultiplicand) * (uint32_t)usMultiplier;

    return ULongToUShort(ulResult, pusResult);
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
