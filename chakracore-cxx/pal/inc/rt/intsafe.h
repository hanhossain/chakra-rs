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


#if defined(_AMD64_)
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif // _AMD64_

#define INTSAFE_E_ARITHMETIC_OVERFLOW       ((HRESULT)0x80070216L)  // 0x216 = 534 = ERROR_ARITHMETIC_OVERFLOW

#ifndef LOWORD
#define LOWORD(l)       ((uint16_t)(((DWORD_PTR)(l)) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)       ((uint16_t)(((DWORD_PTR)(l)) >> 16))
#endif

#define HIDWORD(_qw)    ((uint32_t)((_qw) >> 32))
#define LODWORD(_qw)    ((uint32_t)(_qw))

#if defined(MIDL_PASS) || defined(RC_INVOKED) || defined(_M_CEE_PURE) \
    || defined(_68K_) || defined(_MPPC_) || defined(_PPC_)            \
    || defined(_M_IA64) || defined(_M_AMD64) || defined(__ARM_ARCH)

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) ((unsigned __int64)((uint32_t)(a)) * (unsigned __int64)((uint32_t)(b)))
#endif

#elif defined(_M_IX86)

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) (unsigned __int64)((unsigned __int64)(uint32_t)(a) * (uint32_t)(b))
#endif

#else

#error Must define a target architecture.

#endif

#define INT_MAX         2147483647
#define LONG_MAX        2147483647L
#define USHRT_MAX       0xffff
#define UINT_MAX        0xffffffff
#define ULONG_MAX       0xffffffffUL
#define DWORD_MAX       0xffffffffUL

//
// It is common for -1 to be used as an error value for various types
//
#define USHORT_ERROR    (0xffff)
#define INT_ERROR       (-1)
#define LONG_ERROR      (-1L)
#define UINT_ERROR      (0xffffffff)
#define ULONG_ERROR     (0xffffffffUL)
#define ULONGLONG_ERROR (0xffffffffffffffffULL)
#define HIDWORD_MASK (0xffffffff00000000ULL)
#define SIZET_ERROR     ULONGLONG_ERROR

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
HRESULT
IntToSignedChar(
     int32_t iOperand,
     signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
HRESULT
IntToUChar(
     int32_t iOperand,
     unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
HRESULT
LongToUChar(
     int32_t lOperand,
     unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
__forceinline
HRESULT
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
HRESULT
IntToUShort(
     int32_t iOperand,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

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
HRESULT
IntToUInt(
     int32_t iOperand,
     uint32_t* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

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
HRESULT
IntToULong(
     int32_t iOperand,
     uint32_t* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (iOperand >= 0)
    {
        *pulResult = (uint32_t)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> ULONGLONG conversion
//
__inline
HRESULT
IntToULongLong(
     int32_t iOperand,
     ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONG_ERROR;

    if (iOperand >= 0)
    {
        *pullResult = (ULONGLONG)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> signed char conversion
//
__inline
HRESULT
UIntToSignedChar(
     uint32_t uOperand,
     signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
HRESULT
UIntToUChar(
     uint32_t uOperand,
     unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (uOperand <= 255)
    {
        *pch = (unsigned char)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// uint32_t -> uint8_t conversion
//
#define UIntToByte   UIntToUChar

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// uint32_t -> char conversion
//
__forceinline
HRESULT
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
HRESULT
UIntToInt(
     uint32_t uOperand,
     int32_t* piResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *piResult = INT_ERROR;

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
HRESULT
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
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// uint32_t -> uint32_t conversion
//
__inline
HRESULT
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
HRESULT
ULongToSignedChar(
     uint32_t ulOperand,
     signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
HRESULT
ULongToUChar(
     uint32_t ulOperand,
     unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
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
__forceinline
HRESULT
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
HRESULT
ULongToUShort(
     uint32_t ulOperand,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

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
HRESULT
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
        *piResult = INT_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// uint32_t -> uint32_t conversion
//
__inline
HRESULT
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
HRESULT
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
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// ULONGLONG -> INT conversion
//
__inline
HRESULT
ULongLongToInt(
     ULONGLONG ullOperand,
     int32_t* piResult)
{
    if (ullOperand <= INT_MAX)
    {
        *piResult = (int32_t)ullOperand;
        return S_OK;
    }
    else
    {
        *piResult = INT_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// ULONGLONG -> int32_t conversion
//
__inline
HRESULT
ULongLongToLong(
     ULONGLONG Operand,
     int32_t* Result)
{
    if (Operand <= LONG_MAX)
    {
        *Result = (int32_t)Operand;
        return S_OK;
    }
    else
    {
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// uint32_t -> unsigned short conversion
//
__inline
HRESULT
UIntToUShort(
     uint32_t uOperand,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (uOperand <= USHRT_MAX)
    {
        *pusResult = (unsigned short)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// ULONGLONG -> unsigned short conversion
//
__inline
HRESULT
ULongLongToUShort(
     ULONGLONG ullOperand,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    unsigned short usResult = USHORT_ERROR;

    if (ullOperand <= USHRT_MAX)
    {
        usResult = (unsigned short)ullOperand;
        hr = S_OK;
    }
    *pusResult = usResult;

    return hr;
}

//
// ULONGLONG -> uint32_t conversion
//
__inline
HRESULT
ULongLongToULong(
     ULONGLONG ullOperand,
     uint32_t* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (ullOperand <= ULONG_MAX)
    {
        *pulResult = (uint32_t)ullOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT_PTR -> uint32_t conversion
// size_t -> uint32_t conversion
//
#define UIntPtrToULong  ULongLongToULong
#define ULongPtrToULong ULongLongToULong

//
// ULONGLONG -> uint32_t conversion
//
__inline
HRESULT
ULongLongToUInt(
     ULONGLONG ullOperand,
     uint32_t* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (ullOperand <= UINT_MAX)
    {
        *puResult = (uint32_t)ullOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT_PTR -> uint32_t conversion
// size_t -> uint32_t conversion
//
#define UIntPtrToUInt  ULongLongToUInt
#define ULongPtrToUInt ULongLongToUInt

//
// * -> uint8_t conversion (uint8_t is always unsigned char)
//
#define  IntToByte   IntToUChar
#define UIntToByte  UIntToUChar
#define  LongToByte  LongToUChar
#define ULongToByte ULongToUChar

//
// * -> uint16_t conversion (uint16_t is always unsigned short)
//
#define IntToWord               IntToUShort
#define LongToWord              LongToUShort
#define LongLongToWord          LongLongToUShort
#define UIntToWord              UIntToUShort
#define ULongToWord             ULongToUShort
#define ULongLongToWord         ULongLongToUShort
#define UIntPtrToWord           UIntPtrToUShort
#define ULongPtrToWord          ULongPtrToUShort
#define SizeTToWord             SizeTToUShort
#define SIZETToWord             SIZETToUShort

//
// uint16_t -> * conversion (uint16_t is always unsigned short)
//
#define WordToUChar             UShortToUChar
#define WordToByte              UShortToByte
#define WordToChar              UShortToChar
#define WordToSignedChar        UShortToSignedChar
#define WordToInt               UShortToInt
#define WordToLong              UShortToLong
#define WordToLongLong          UShortToLongLong
#define WordToIntPtr            UShortToIntPtr
#define WordToLongPtr           UShortToLongPtr

//
// * -> uint32_t conversion (uint32_t is always uint32_t)
//
#define CharToDWord             CharToULong
#define SignedCharToDWord       SignedCharToULong
#define ShortToDWord            ShortToULong
#define IntToDWord              IntToULong
#define LongToDWord             LongToULong
#define LongLongToDWord         LongLongToULong
#define UIntToDWord             UIntToULong
#define ULongLongToDWord        ULongLongToULong
#define IntPtrToDWord           IntPtrToULong
#define LongPtrToDWord          LongPtrToULong
#define UIntPtrToDWord          UIntPtrToULong
#define ULongPtrToDWord         ULongPtrToULong
#define SizeTToDWord            SizeTToULong
#define SIZETToDWord            SIZETToULong

//
// uint32_t -> * conversion (uint32_t is always uint32_t)
//
#define DWordToChar             ULongToChar
#define DWordToUChar            ULongToUChar
#define DWordToByte             ULongToByte
#define DWordToSignedChar       ULongToSignedChar
#define DWordToUShort           ULongToUShort
#define DWordToUInt             ULongToUInt
#define DWordToInt              ULongToInt
#define DWordToLong             ULongToLong
#define DWordToLongLong         ULongToLongLong
#define DWordToIntPtr           ULongToIntPtr
#define DWordToLongPtr          ULongToLongPtr


//
// * -> UINT_PTR conversion (UINT_PTR is uint32_t on Win32, ULONGLONG on Win64)
//
#define CharToUIntPtr           CharToULongLong
#define SignedCharToUIntPtr     SignedCharToULongLong
#define ShortToUIntPtr          ShortToULongLong
#define IntToUIntPtr            IntToULongLong
#define LongToUIntPtr           LongToULongLong
#define LongLongToUIntPtr       LongLongToULongLong
#define IntPtrToUIntPtr         IntPtrToULongLong
#define LongPtrToUIntPtr        LongPtrToULongLong

__inline
HRESULT
ULongLongToUIntPtr(
     ULONGLONG ullOperand,
     UINT_PTR* puResult)
{
	*puResult = ullOperand;
	return S_OK;
}


//
// UINT_PTR -> * conversion (UINT_PTR is uint32_t on Win32, ULONGLONG on Win64)
//
#define UIntPtrToUShort         ULongLongToUShort
#define UIntPtrToInt            ULongLongToInt
#define UIntPtrToLong           ULongLongToLong
#define UIntPtrToLongLong       ULongLongToLongLong
#define UIntPtrToIntPtr         ULongLongToIntPtr
#define UIntPtrToLongPtr        ULongLongToLongPtr


//
// * -> size_t conversion (size_t is uint32_t on Win32, ULONGLONG on Win64)
//
#define CharToULongPtr          CharToULongLong
#define SignedCharToULongPtr    SignedCharToULongLong
#define ShortToULongPtr         ShortToULongLong
#define IntToULongPtr           IntToULongLong
#define LongToULongPtr          LongToULongLong
#define LongLongToULongPtr      LongLongToULongLong
#define IntPtrToULongPtr        IntPtrToULongLong
#define LongPtrToULongPtr       LongPtrToULongLong

__inline
HRESULT
ULongLongToULongPtr(
     ULONGLONG ullOperand,
     size_t* pulResult)
{
	*pulResult = ullOperand;
	return S_OK;
}


//
// size_t -> * conversion (size_t is uint32_t on Win32, ULONGLONG on Win64)
//
#define ULongPtrToUShort        ULongLongToUShort
#define ULongPtrToInt           ULongLongToInt
#define ULongPtrToLong          ULongLongToLong
#define ULongPtrToLongLong      ULongLongToLongLong
#define ULongPtrToIntPtr        ULongLongToIntPtr
#define ULongPtrToLongPtr       ULongLongToLongPtr

//
// * -> size_t conversion (size_t is always UINT_PTR)
//
#define CharToSizeT             CharToUIntPtr
#define SignedCharToSizeT       SignedCharToUIntPtr
#define ShortToSizeT            ShortToUIntPtr
#define IntToSizeT              IntToUIntPtr
#define LongToSizeT             LongToUIntPtr
#define LongLongToSizeT         LongLongToUIntPtr
#define ULongLongToSizeT        ULongLongToUIntPtr
#define IntPtrToSizeT           IntPtrToUIntPtr
#define LongPtrToSizeT          LongPtrToUIntPtr

//
// size_t -> * conversion (size_t is always UINT_PTR)
//
#define SizeTToUShort           UIntPtrToUShort
#define SizeTToUInt             UIntPtrToUInt
#define SizeTToULong            UIntPtrToULong
#define SizeTToInt              UIntPtrToInt
#define SizeTToLong             UIntPtrToLong
#define SizeTToLongLong         UIntPtrToLongLong
#define SizeTToIntPtr           UIntPtrToIntPtr
#define SizeTToLongPtr          UIntPtrToLongPtr

//
// * -> SIZE_T conversion (SIZE_T is always size_t)
//
#define CharToSIZET             CharToULongPtr
#define SignedCharToSIZET       SignedCharToULongPtr
#define ShortToSIZET            ShortToULongPtr
#define IntToSIZET              IntToULongPtr
#define LongToSIZET             LongToULongPtr
#define LongLongToSIZET         LongLongToULongPtr
#define IntPtrToSIZET           IntPtrToULongPtr
#define LongPtrToSIZET          LongPtrToULongPtr
#define ULongLongToSIZET        ULongLongToULongPtr

//
// SIZE_T -> * conversion (SIZE_T is always size_t)
//
#define SIZETToUShort           ULongPtrToUShort
#define SIZETToUInt             ULongPtrToUInt
#define SIZETToULong            ULongPtrToULong
#define SIZETToUIntPtr          ULongPtrToUIntPtr
#define SIZETToULongPtr         ULongPtrToULongPtr
#define SIZETToInt              ULongPtrToInt
#define SIZETToLong             ULongPtrToLong
#define SIZETToLongLong         ULongPtrToLongLong
#define SIZETToIntPtr           ULongPtrToIntPtr
#define SIZETToLongPtr          ULongPtrToLongPtr

//
// * -> DWORD_PTR conversion (DWORD_PTR is always size_t)
//
#define CharToDWordPtr             CharToULongPtr
#define SignedCharToDWordPtr       SignedCharToULongPtr
#define ShortToDWordPtr            ShortToULongPtr
#define IntToDWordPtr              IntToULongPtr
#define LongToDWordPtr             LongToULongPtr
#define LongLongToDWordPtr         LongLongToULongPtr
#define ULongLongToDWordPtr        ULongLongToULongPtr
#define IntPtrToDWordPtr           IntPtrToULongPtr
#define LongPtrToDWordPtr          LongPtrToULongPtr

//
// DWORD_PTR -> * conversion (DWORD_PTR is always size_t)
//
#define DWordPtrToUShort           ULongPtrToUShort
#define DWordPtrToUInt             ULongPtrToUInt
#define DWordPtrToULong            ULongPtrToULong
#define DWordPtrToDWord            ULongPtrToDWord
#define DWordPtrToInt              ULongPtrToInt
#define DWordPtrToLong             ULongPtrToLong
#define DWordPtrToLongLong         ULongPtrToLongLong
#define DWordPtrToIntPtr           ULongPtrToIntPtr
#define DWordPtrToLongPtr          ULongPtrToLongPtr

//
// unsigned short addition
//
__inline
HRESULT
UShortAdd(
     unsigned short usAugend,
     unsigned short usAddend,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (((unsigned short)(usAugend + usAddend)) >= usAugend)
    {
        *pusResult = (usAugend + usAddend);
        hr = S_OK;
    }

    return hr;
}

//
// uint16_t addtition
//
#define WordAdd     UShortAdd

//
// uint32_t addition
//
__inline
HRESULT
UIntAdd(
     uint32_t uAugend,
     uint32_t uAddend,
     uint32_t* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if ((uAugend + uAddend) >= uAugend)
    {
        *puResult = (uAugend + uAddend);
        hr = S_OK;
    }

    return hr;
}

//
// UINT_PTR addition
//
#define UIntPtrAdd		SizeTAdd

//
// uint32_t addition
//
__inline
HRESULT
ULongAdd(
     uint32_t ulAugend,
     uint32_t ulAddend,
     uint32_t* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

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
#define ULongPtrAdd     ULongLongAdd

//
// uint32_t addition
//
#define DWordAdd        ULongAdd

//
// DWORD_PTR addition
//
#define DWordPtrAdd		ULongPtrAdd

//
// size_t addition
//
__inline
HRESULT
SizeTAdd(
     size_t Augend,
     size_t Addend,
     size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if ((Augend + Addend) >= Augend)
    {
        *pResult = (Augend + Addend);
        hr = S_OK;
    }

    return hr;
}

//
// SIZE_T addition
//
#define SIZETAdd	ULongPtrAdd

//
// ULONGLONG addition
//
__inline
HRESULT
ULongLongAdd(
     ULONGLONG ullAugend,
     ULONGLONG ullAddend,
     ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

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
HRESULT
UShortSub(
     unsigned short usMinuend,
     unsigned short usSubtrahend,
     unsigned short* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (usMinuend >= usSubtrahend)
    {
        *pusResult = (usMinuend - usSubtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// uint16_t subtraction
//
#define WordSub     UShortSub


//
// uint32_t subtraction
//
__inline
HRESULT
UIntSub(
     uint32_t uMinuend,
     uint32_t uSubtrahend,
     uint32_t* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (uMinuend >= uSubtrahend)
    {
        *puResult = (uMinuend - uSubtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// UINT_PTR subtraction
//
#define UIntPtrSub	SizeTSub

//
// uint32_t subtraction
//
__inline
HRESULT
ULongSub(
     uint32_t ulMinuend,
     uint32_t ulSubtrahend,
     uint32_t* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

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
#define ULongPtrSub ULongLongSub


//
// uint32_t subtraction
//
#define DWordSub        ULongSub

//
// DWORD_PTR subtraction
//
#define DWordPtrSub		ULongPtrSub

//
// size_t subtraction
//
__inline
HRESULT
SizeTSub(
     size_t Minuend,
     size_t Subtrahend,
     size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if (Minuend >= Subtrahend)
    {
        *pResult = (Minuend - Subtrahend);
        hr = S_OK;
    }

    return hr;
}

//
// SIZE_T subtraction
//
#define SIZETSub	ULongPtrSub

//
// ULONGLONG subtraction
//
__inline
HRESULT
ULongLongSub(
     ULONGLONG ullMinuend,
     ULONGLONG ullSubtrahend,
     ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

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
HRESULT
UShortMult(
     unsigned short usMultiplicand,
     unsigned short usMultiplier,
     unsigned short* pusResult)
{
    uint32_t ulResult = ((uint32_t)usMultiplicand) * (uint32_t)usMultiplier;

    return ULongToUShort(ulResult, pusResult);
}

//
// uint16_t multiplication
//
#define WordMult      UShortMult

//
// uint32_t multiplication
//
__inline
HRESULT
UIntMult(
     uint32_t uMultiplicand,
     uint32_t uMultiplier,
     uint32_t* puResult)
{
    ULONGLONG ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

    return ULongLongToUInt(ull64Result, puResult);
}

//
// uint32_t multiplication
//
__inline
HRESULT
ULongMult(
     uint32_t ulMultiplicand,
     uint32_t ulMultiplier,
     uint32_t* pulResult)
{
    ULONGLONG ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);

    return ULongLongToULong(ull64Result, pulResult);
}

//
// uint32_t multiplication
//
#define DWordMult       ULongMult

//
// DWORD_PTR multiplication
//
#define DWordPtrMult	ULongPtrMult

//
// size_t multiplication
//
#define SizeTMult		UIntPtrMult

//
// SIZE_T multiplication
//
#define SIZETMult		ULongPtrMult

#endif // _INTSAFE_H_INCLUDED_
