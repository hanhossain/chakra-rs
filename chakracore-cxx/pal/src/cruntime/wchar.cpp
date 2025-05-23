//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    wchar.c

Abstract:

    Implementation of wide char string functions.



--*/


#include "pal/palinternal.h"
#include "pal/cruntime.h"
#include "pal/dbgmsg.h"
#include "pal/unicode_data.h"

#include "pal/thread.hpp"
#include "pal/threadsusp.hpp"


#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "runtime_proxy.h"
#include <errno.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);


/*--
Function:
  wtolower (internal)

16-bit wide character version of the ANSI tolower() function.

  --*/
static
char16_t
wtolower(char16_t c)
{
    /* Note: Surrogate pairs unicode character are not supported */

#if HAVE_TOWLOWER

    char16_t w;
    w = (char16_t) c;
    w = towlower(w);
    return (char16_t) w;

#else

    return PAL_towlower(c);

#endif

}

/*******************************************************************************
Function:
  Internal_i64tow

Parameters:
  value
    - int64_t value to be converted to a string
  string
    - out buffer to place interger string
  radix
    - numeric base to convert to
  isI64
    - TRUE if value is int64_t, FALSE if value is a long

Note:
  - only a radix of ten (and value < 0) will result in a negative
    sign in the output buffer
*******************************************************************************/
LPWSTR Internal_i64tow(int64_t value, LPWSTR string, int radix, BOOL isI64)
{
    int length = 0;
    int n;
    int r;
    UINT64 uval = value;
    LPWSTR stringPtr = string;
    int start = 0;
    int end;
    WCHAR tempCh;

    if (radix < 2 || radix > 36)
    {
        ASSERT( "Invalid radix, radix must be between 2 and 36\n" );
        SetLastError(ERROR_INVALID_PARAMETER);
        return string;
    }
    if (FALSE == isI64)
    {
        uval = (uint32_t) uval;
    }
    if (10 == radix && value < 0)
    {
        uval = value * -1;
    }
    if(0 == uval)
    {
        ++length;
        *stringPtr++ = '0';
    }
    else while (uval > 0)
    {
        ++length;
        n = uval / radix;
        r = uval - (n * radix);
        uval /= radix;
        if (r > 9)
        {
            *stringPtr++ = r + 87;
        }
        else
        {
            *stringPtr++ = r + 48;
        }
    }
    if (10 == radix && value < 0)
    {
        *stringPtr++ = '-';
        ++length;
    }
    *stringPtr = 0; /* end the string */

    /* reverse the string */
    end = length - 1;
    while (start < end)
    {
        tempCh = string[start];
        string[start] = string[end];
        string[end] = tempCh;
        ++start;
        --end;
    }

    return string;
}

/*--
Function:
  _itow

16-bit wide character version of the ANSI tolower() function.

  --*/
char16_t *
_itow(
    int value,
    char16_t *string,
    int radix)
{
    char16_t *ret;

    PERF_ENTRY(_itow);
    ENTRY("_itow (value=%d, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, FALSE);

    LOGEXIT("_itow returns char16_t* %p\n", ret);
    PERF_EXIT(_itow);

    return ret;
}

/*--
Function:
  _itow

16-bit wide character version of the ANSI ltow() function.

  --*/
char16_t *
_ltow(
    long value,
    char16_t *string,
    int radix)
{
    char16_t *ret;

    PERF_ENTRY(_ltow);
    ENTRY("_ltow (value=%d, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, FALSE);

    LOGEXIT("_ltow returns char16_t* %p\n", ret);
    PERF_EXIT(_ltow);

    return ret;
}

/*--
Function:
  _i64tow

See MSDN doc
--*/
char16_t *
 _i64tow(
    __int64 value,
    char16_t *string,
    int radix)
{
    char16_t *ret;

    PERF_ENTRY(_i64tow);
    ENTRY("_i64tow (value=%ld, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, TRUE);

    LOGEXIT("_i64tow returns char16_t* %p\n", ret);
    PERF_EXIT(_i64tow);

    return ret;
}


/*--
Function:
  _wtoi

See MSDN doc
--*/
int
_wtoi(
    const char16_t *string)
{
    int len;
    int ret;
    char *tempStr;

    PERF_ENTRY(_wtoi);
    ENTRY("_wtoi (string=%p)\n", string);

    len = WideCharToMultiByte(CP_ACP, 0, string, -1, 0, 0, 0, 0);
    if (!len)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n",
              GetLastError());
        return -1;
    }
    tempStr = (char *) PAL_malloc(len);
    if (!tempStr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return -1;
    }
    len = WideCharToMultiByte(CP_ACP, 0, string, -1, tempStr, len, 0, 0);
    if (!len)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n",
              GetLastError());
        PAL_free(tempStr);
        return -1;
    }
    ret = atoi(tempStr);

    PAL_free(tempStr);
    LOGEXIT("_wtoi returns int %d\n", ret);
    PERF_EXIT(_wtoi);
    return ret;
}


/*--
Function:
  PAL_iswspace

See MSDN doc
--*/
int
PAL_iswspace(char16_t c)
{
    int ret;

    PERF_ENTRY(iswspace);
    ENTRY("PAL_iswspace (c=%C)\n", c);

    ret = proxy_iswspace(c);

    LOGEXIT("PAL_iswspace returns int %d\n", ret);
    PERF_EXIT(iswspace);
    return ret;
}

/*++
Function:
  _wcsnicmp

Compare characters of two strings without regard to case

Return Value

The return value indicates the relationship between the substrings as follows.

Return Value

Description

< 0        string1 substring less than string2 substring
  0        string1 substring identical to string2 substring
> 0        string1 substring greater than string2 substring

Parameters

string1, string2        Null-terminated strings to compare
count                   Number of characters to compare

Remarks

The _strnicmp function lexicographically compares, at most, the first
count characters of string1 and string2. The comparison is performed
without regard to case; _strnicmp is a case-insensitive version of
strncmp. The comparison ends if a terminating null character is
reached in either string before count characters are compared. If the
strings are equal when a terminating null character is reached in
either string before count characters are compared, the shorter string
is lesser.

--*/
int
_wcsnicmp(
          const char16_t *string1,
          const char16_t *string2,
          size_t count)
{
    size_t i;
    int diff = 0;

    PERF_ENTRY(_wcsnicmp);
    ENTRY("_wcsnicmp (string1=%p (%S), string2=%p (%S), count=%lu)\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING,
         (unsigned long) count);

    for (i = 0; i < count; i++)
    {
        diff = wtolower(string1[i]) - wtolower(string2[i]);
        if (diff != 0 || 0 == string1[i] || 0 == string2[i])
        {
            break;
        }
    }
    LOGEXIT("_wcsnicmp returning int %d\n", diff);
    PERF_EXIT(_wcsnicmp);
    return diff;
}

/*++
Function:
  _wcsicmp

Compare characters of two strings without regard to case

Return Value

The return value indicates the relationship between the substrings as follows.

Return Value

Description

< 0        string1 substring less than string2 substring
  0        string1 substring identical to string2 substring
> 0        string1 substring greater than string2 substring

Parameters

string1, string2        Null-terminated strings to compare

--*/
int
_wcsicmp(
          const char16_t *string1,
          const char16_t *string2)
{
    int ret;

    PERF_ENTRY(_wcsicmp);
    ENTRY("_wcsicmp (string1=%p (%S), string2=%p (%S))\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING);

    ret = _wcsnicmp(string1, string2, 0x7fffffff);

    LOGEXIT("_wcsnicmp returns int %d\n", ret);
    PERF_EXIT(_wcsicmp);
    return ret;
}


/*++
Function:
  _wcslwr

Convert a string to lowercase.

Return Value

Returns a pointer to the converted string. Because the modification is
done in place, the pointer returned is the same as the pointer passed
as the input argument. No return value is reserved to indicate an
error.

Parameter

string   Null-terminated string to convert to lowercase

Remarks

--*/
char16_t *
_wcslwr(
        char16_t *string)
{
    int i;

    PERF_ENTRY(_wcslwr);
    ENTRY("_wcslwr (string=%p (%S))\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);

    for (i=0 ; string[i] != 0; i++)
    {
        string[i] = wtolower(string[i]);
    }

    LOGEXIT("_wcslwr returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
    PERF_EXIT(_wcslwr);
    return string;
}


/*++
Function:
  PAL_wcstol

Convert string to a long-integer value.

Return Value

wcstol returns the value represented in the string nptr, except when
the representation would cause an overflow, in which case it returns
LONG_MAX or LONG_MIN. strtol returns 0 if no conversion can be
performed. errno is set to ERANGE if overflow or underflow occurs.

Parameters

nptr    Null-terminated string to convert
endptr  Pointer to character that stops scan
base    Number base to use

Remarks

The wcstol function converts nptr to a long. It stops reading the
string nptr at the first character it cannot recognize as part of a
number. This may be the terminating null character, or it may be the
first numeric character greater than or equal to base.

Notes :
    MSDN states that only space and tab are accepted as leading whitespace, but
    tests indicate that other whitespace characters (newline, carriage return,
    etc) are also accepted. This matches the behavior on Unix systems.

    For wcstol and wcstoul, we need to check if the value to be returned
    is outside the 32 bit range. If so, the returned value needs to be set
    as appropriate, according to the MSDN pages for wcstol and wcstoul,
    and in all instances errno must be set to ERANGE (The one exception
    is converting a string representing a negative value to unsigned long).
    Note that on 64 bit Windows, long's are still 32 bit. Thus, to match
    Windows behavior, we must return long's in the 32 bit range.
--*/

/* The use of int32_t is by design, to ensure that a 32 bit value is always
returned from this function. If "long" is used instead of int32_t, then a 64 bit
value could be returned on 64 bit platforms like HP-UX, thus breaking
Windows behavior. */
int32_t
PAL_wcstol(
        const char16_t *nptr,
        char16_t **endptr,
        int base)
{
    char *s_nptr = 0;
    char *s_endptr = 0;
    long res;
    int size;
    uint32_t dwLastError = 0;

    PERF_ENTRY(wcstol);
    ENTRY("wcstol (nptr=%p (%S), endptr=%p, base=%d)\n", nptr?nptr:W16_NULLSTRING, nptr?nptr:W16_NULLSTRING,
          endptr, base);

    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, NULL, 0, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstolExit;
    }
    s_nptr = (char *)PAL_malloc(size);
    if (!s_nptr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        res = 0;
        goto PAL_wcstolExit;
    }
    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, s_nptr, size, NULL, NULL);
    if( size==0 )
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstolExit;
    }

    res = strtol(s_nptr, &s_endptr, base);

    if (res > _I32_MAX)
    {
        res = _I32_MAX;
        errno = ERANGE;
    }
    else if (res < _I32_MIN)
    {
        res = _I32_MIN;
        errno = ERANGE;
    }

    /* only ASCII characters will be accepted by strtol, and those always get
       mapped to single-byte characters, so the first rejected character will
       have the same index in the multibyte and widechar strings */
    if( endptr )
    {
        size = s_endptr - s_nptr;
        *endptr = (char16_t *)&nptr[size];
    }

PAL_wcstolExit:
    PAL_free(s_nptr);
    LOGEXIT("wcstol returning long %ld\n", res);
    PERF_EXIT(wcstol);
    /* This explicit cast to int32_t is used to silence any potential warnings
    due to implicitly casting the native long res to int32_t when returning. */
    return (int32_t)res;
}

/*++
Function:
  PAL_wcstoll

Convert string to a long-long-integer value.

Return Value

wcstoll returns the value represented in the string nptr, except when
the representation would cause an overflow, in which case it returns
LONGLONG_MAX or LONGLONG_MIN. strtoll returns 0 if no conversion can be
performed. errno is set to ERANGE if overflow or underflow occurs.

Parameters

nptr    Null-terminated string to convert
endptr  Pointer to character that stops scan
base    Number base to use

Remarks

The wcstoll function converts nptr to a long long. It stops reading the
string nptr at the first character it cannot recognize as part of a
number. This may be the terminating null character, or it may be the
first numeric character greater than or equal to base.

Notes :
    MSDN states that only space and tab are accepted as leading whitespace, but
    tests indicate that other whitespace characters (newline, carriage return,
    etc) are also accepted. This matches the behavior on Unix systems.
--*/

LONGLONG
PAL_wcstoll(
        const char16_t *nptr,
        char16_t **endptr,
        int base)
{
    char *s_nptr = 0;
    char *s_endptr = 0;
    long long res;
    int size;
    uint32_t dwLastError = 0;

    PERF_ENTRY(wcstoll);
    ENTRY("wcstoll (nptr=%p (%S), endptr=%p, base=%d)\n", nptr?nptr:W16_NULLSTRING, nptr?nptr:W16_NULLSTRING,
          endptr, base);

    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, NULL, 0, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstolExit;
    }
    s_nptr = (char *)PAL_malloc(size);
    if (!s_nptr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        res = 0;
        goto PAL_wcstolExit;
    }
    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, s_nptr, size, NULL, NULL);
    if( size==0 )
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstolExit;
    }

    res = strtoll(s_nptr, &s_endptr, base);

    /* only ASCII characters will be accepted by strtol, and those always get
       mapped to single-byte characters, so the first rejected character will
       have the same index in the multibyte and widechar strings */
    if( endptr )
    {
        size = s_endptr - s_nptr;
        *endptr = (char16_t *)&nptr[size];
    }

PAL_wcstolExit:
    PAL_free(s_nptr);
    LOGEXIT("wcstoll returning long %lld\n", res);
    PERF_EXIT(wcstoll);
    /* This explicit cast to LONGLONG is used to silence any potential warnings
    due to implicitly casting the native long res to LONGLONG when returning. */
    return (LONGLONG)res;
}

/*++
Function:
  PAL_wcstoul

Convert string to an unsigned long-integer value.

Return Value

wcstoul returns the converted value, if any, or ULONG_MAX on
overflow. It returns 0 if no conversion can be performed. errno is
set to ERANGE if overflow or underflow occurs.

Parameters

nptr    Null-terminated string to convert
endptr  Pointer to character that stops scan
base    Number base to use

Remarks

wcstoul stops reading the string nptr at the first character it cannot
recognize as part of a number. This may be the terminating null
character, or it may be the first numeric character greater than or
equal to base. The LC_NUMERIC category setting of the current locale
determines recognition of the radix character in nptr; for more
information, see setlocale. If endptr is not NULL, a pointer to the
character that stopped the scan is stored at the location pointed to
by endptr. If no conversion can be performed (no valid digits were
found or an invalid base was specified), the value of nptr is stored
at the location pointed to by endptr.

Notes :
    MSDN states that only space and tab are accepted as leading whitespace, but
    tests indicate that other whitespace characters (newline, carriage return,
    etc) are also accepted. This matches the behavior on Unix systems.

    For wcstol and wcstoul, we need to check if the value to be returned
    is outside the 32 bit range. If so, the returned value needs to be set
    as appropriate, according to the MSDN pages for wcstol and wcstoul,
    and in all instances errno must be set to ERANGE (The one exception
    is converting a string representing a negative value to unsigned long).
    Note that on 64 bit Windows, long's are still 32 bit. Thus, to match
    Windows behavior, we must return long's in the 32 bit range.
--*/

/* The use of uint32_t is by design, to ensure that a 32 bit value is always
returned from this function. If "unsigned long" is used instead of uint32_t,
then a 64 bit value could be returned on 64 bit platforms like HP-UX, thus
breaking Windows behavior .*/
uint32_t
PAL_wcstoul(
        const char16_t *nptr,
        char16_t **endptr,
        int base)
{
    char *s_nptr = 0;
    char *s_endptr = 0;
    unsigned long res;
    int size;
    uint32_t dwLastError = 0;

    PERF_ENTRY(wcstoul);
    ENTRY("wcstoul (nptr=%p (%S), endptr=%p, base=%d)\n", nptr?nptr:W16_NULLSTRING, nptr?nptr:W16_NULLSTRING,
          endptr, base);

    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, NULL, 0, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstoulExit;
    }
    s_nptr = (char *)PAL_malloc(size);
    if (!s_nptr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        res = 0;
        goto PAL_wcstoulExit;
    }
    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, s_nptr, size, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL_wcstoulExit;
    }

    res = strtoul(s_nptr, &s_endptr, base);

    if (res > _UI32_MAX)
    {
        char16_t wc = *nptr;
        while (PAL_iswspace(wc))
        {
            wc = *nptr++;
        }
        /* If the string represents a positive number that is greater than
           _UI32_MAX, set errno to ERANGE. Otherwise, don't set errno
           to match Windows behavior. */
        if (wc != '-')
        {
            res = _UI32_MAX;
            errno = ERANGE;
        }
    }

    /* only ASCII characters will be accepted by strtol, and those always get
       mapped to single-byte characters, so the first rejected character will
       have the same index in the multibyte and widechar strings */
    if( endptr )
    {
        size = s_endptr - s_nptr;
        *endptr = (char16_t *)&nptr[size];
    }

PAL_wcstoulExit:
    PAL_free(s_nptr);
    LOGEXIT("wcstoul returning unsigned long %lu\n", res);
    PERF_EXIT(wcstoul);

    /* When returning unsigned long res from this function, it will be
    implicitly cast to uint32_t. This handles situations where a string that
    represents a negative number is passed in to wcstoul. The Windows
    behavior is analogous to taking the binary equivalent of the negative
    value and treating it as a positive number. Returning a uint32_t from
    this function, as opposed to native unsigned long, allows us to match
    this behavior. The explicit case to uint32_t below is used to silence any
    potential warnings due to the implicit casting.  */
    return (uint32_t)res;
}

ULONGLONG
PAL__wcstoui64(
        const char16_t *nptr,
        char16_t **endptr,
        int base)
{
    char *s_nptr = 0;
    char *s_endptr = 0;
    unsigned long long res;
    int size;
    uint32_t dwLastError = 0;

    PERF_ENTRY(wcstoul);
    ENTRY("_wcstoui64 (nptr=%p (%S), endptr=%p, base=%d)\n", nptr?nptr:W16_NULLSTRING, nptr?nptr:W16_NULLSTRING,
          endptr, base);

    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, NULL, 0, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL__wcstoui64Exit;
    }
    s_nptr = (char *)PAL_malloc(size);
    if (!s_nptr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        res = 0;
        goto PAL__wcstoui64Exit;
    }
    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, s_nptr, size, NULL, NULL);
    if (!size)
    {
        dwLastError = GetLastError();
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", dwLastError);
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL__wcstoui64Exit;
    }

    res = strtoull(s_nptr, &s_endptr, base);

    /* only ASCII characters will be accepted by strtoull, and those always get
       mapped to single-byte characters, so the first rejected character will
       have the same index in the multibyte and widechar strings */
    if( endptr )
    {
        size = s_endptr - s_nptr;
        *endptr = (char16_t *)&nptr[size];
    }

PAL__wcstoui64Exit:
    PAL_free(s_nptr);
    LOGEXIT("_wcstoui64 returning unsigned long long %llu\n", res);
    PERF_EXIT(_wcstoui64);

    return res;
}

/*++
Function:
  PAL_towlower

See MSDN

--*/
char16_t
PAL_towlower( char16_t c )
{
    PERF_ENTRY(towlower);
    ENTRY("towlower (c=%d)\n", c);
    if(c < 128)
    {//fast path for ascii characters
        if(c >= 'A' && c <= 'Z')
        {
            c += ('a' - 'A');
            PERF_EXIT(towlower);
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }
        else
        {
            PERF_EXIT(towlower);
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }
    }
    else
    {
    #if HAVE_COREFOUNDATION
        if (!PAL_iswlower(c))
        {
            CFMutableStringRef cfString = CFStringCreateMutable(
                                                kCFAllocatorDefault, 1);
            if (cfString != NULL)
            {
                CFStringAppendCharacters(cfString, (const UniChar*)&c, 1);
                CFStringLowercase(cfString, NULL);
                c = CFStringGetCharacterAtIndex(cfString, 0);
                CFRelease(cfString);
            }
        }
        LOGEXIT("towlower returns int %d\n", c );
        PERF_EXIT(towlower);
        return c;
    #else   /* HAVE_COREFOUNDATION */
        UnicodeDataRec dataRec;

        if (!GetUnicodeData(c, &dataRec))
        {
            TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
            LOGEXIT("towlower returns int %d\n", c );
            PERF_EXIT(towlower);
            return c;
        }

        if ( (dataRec.C1_TYPE_FLAGS & C1_LOWER) || (dataRec.nOpposingCase ==  0 ))
        {
            LOGEXIT("towlower returns int %d\n", c );
            PERF_EXIT(towlower);
            return c;
        }
        else
        {
            LOGEXIT("towlower returns int %d\n", dataRec.nOpposingCase );
            PERF_EXIT(towlower);
            return dataRec.nOpposingCase;
        }
    #endif  /* HAVE_COREFOUNDATION */
    }
}


/*++
Function:
  PAL_towupper

See MSDN

--*/
char16_t
PAL_towupper( char16_t c )
{
#if HAVE_COREFOUNDATION
    PERF_ENTRY(towupper);
    ENTRY("towupper (c=%d)\n", c);
    if (!PAL_iswupper(c))
    {
        CFMutableStringRef cfString = CFStringCreateMutable(
                                            kCFAllocatorDefault, 1);
        if (cfString != NULL)
        {
            CFStringAppendCharacters(cfString, (const UniChar*)&c, 1);
            CFStringUppercase(cfString, NULL);
            c = CFStringGetCharacterAtIndex(cfString, 0);
            CFRelease(cfString);
        }
    }
    LOGEXIT("towupper returns int %d\n", c );
    PERF_EXIT(towupper);
    return c;
#else   /* HAVE_COREFOUNDATION */
    UnicodeDataRec dataRec;

    PERF_ENTRY(towupper);
    ENTRY("towupper (c=%d)\n", c);

    if (!GetUnicodeData(c, &dataRec))
    {
        TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
        LOGEXIT("towupper returns int %d\n", c );
        PERF_EXIT(towupper);
        return c;
    }

    if ( (dataRec.C1_TYPE_FLAGS & C1_UPPER) || (dataRec.nOpposingCase ==  0 ))
    {
        LOGEXIT("towupper returns int %d\n", c );
        PERF_EXIT(towupper);
        return c;
    }
    else
    {
        LOGEXIT("towupper returns int %d\n", dataRec.nOpposingCase );
        PERF_EXIT(towupper);
        return dataRec.nOpposingCase;
    }
#endif  /* HAVE_COREFOUNDATION */
}

/*++
Function:
  PAL_iswupper

See MSDN

--*/
int
PAL_iswupper( char16_t c )
{
    BOOL bRetVal = FALSE;
#if HAVE_COREFOUNDATION
    static CFCharacterSetRef sUppercaseSet;

    if (sUppercaseSet == NULL)
    {
        sUppercaseSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetUppercaseLetter);
    }
    PERF_ENTRY(iswupper);
    ENTRY( "iswupper (c=%d)\n", c );
    bRetVal = CFCharacterSetIsCharacterMember(sUppercaseSet, c);
#else   /* HAVE_COREFOUNDATION */
    UnicodeDataRec dataRec;

    PERF_ENTRY(iswupper);
    ENTRY( "iswupper (c=%d)\n", c );

    if (!GetUnicodeData(c, &dataRec))
    {
        TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
        goto exit;
    }

    if (dataRec.C1_TYPE_FLAGS & C1_UPPER)
    {
        bRetVal = TRUE;
    }
exit:
#endif  /* HAVE_COREFOUNDATION */
    LOGEXIT( "iswupper returns %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE" );
    PERF_EXIT(iswupper);
    return bRetVal;
}

/*++
Function:
  PAL_iswlower

See MSDN

--*/
int
PAL_iswlower( char16_t c )
{
    BOOL bRetVal = FALSE;
#if HAVE_COREFOUNDATION
    static CFCharacterSetRef sLowercaseSet;

    if (sLowercaseSet == NULL)
    {
        sLowercaseSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetLowercaseLetter);
    }
    PERF_ENTRY(iswlower);
    ENTRY("PAL_iswlower (c=%d)\n", c);
    bRetVal = CFCharacterSetIsCharacterMember(sLowercaseSet, c);
#else   /* HAVE_COREFOUNDATION */
    UnicodeDataRec dataRec;

    PERF_ENTRY(iswlower);
    ENTRY("PAL_iswlower (c=%d)\n", c);

    if (!GetUnicodeData(c, &dataRec))
    {
        TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
        goto exit;
    }

    if (dataRec.C1_TYPE_FLAGS & C1_LOWER)
    {
        bRetVal = TRUE;
    }
exit:
#endif  /* HAVE_COREFOUNDATION */
    LOGEXIT("PAL_iswlower returns %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE");
    PERF_EXIT(iswlower);
    return bRetVal;
}

/*++
Function:
  PAL_iswalpha

See MSDN

--*/
int
PAL_iswalpha( char16_t c )
{
    PERF_ENTRY(iswalpha);
    ENTRY( "PAL_iswalpha (c=%d)\n", c);

    if ( PAL_iswupper( c ) || PAL_iswlower( c ) )
    {
        LOGEXIT( "PAL_iswalpha returns 1.\n" );
        PERF_EXIT(iswalpha);
        return 1;
    }

    LOGEXIT( "PAL_iswalpha returns 0.\n" );
    PERF_EXIT(iswalpha);
    return 0;
}


/*++
Function:
  PAL_wcscat

See MSDN or the man page for mcscat.

--*/
char16_t *
PAL_wcscat(
        char16_t *strDestination,
        const char16_t *strSource)
{
    char16_t *ret;
    PERF_ENTRY(wcscat);
    ENTRY("wcscat (strDestination=%p (%S), strSource=%p (%S))\n",
          strDestination?strDestination:W16_NULLSTRING,
          strDestination?strDestination:W16_NULLSTRING, strSource?strSource:W16_NULLSTRING, strSource?strSource:W16_NULLSTRING);

    ret = PAL_wcsncat( strDestination, strSource, PAL_wcslen( strSource ) );

    LOGEXIT("wcscat returning char16_t %p (%S)\n", ret, ret);
    PERF_EXIT(wcscat);
    return ret;
}


/*++
Function:
  PAL_wcscpy

See MSDN or the man page for mcscpy.

--*/
char16_t *
PAL_wcscpy(
        char16_t *strDestination,
        const char16_t *strSource)
{
    char16_t *start = strDestination;

    PERF_ENTRY(wcscpy);
    ENTRY("wcscpy (strDestination=%p, strSource=%p (%S))\n",
          strDestination, strSource ? strSource:W16_NULLSTRING, strSource ? strSource:W16_NULLSTRING);

    if (strDestination == NULL)
    {
        ERROR("invalid strDestination argument\n");
        LOGEXIT("wcscpy returning char16_t NULL\n");
        PERF_EXIT(wcscpy);
        return NULL;
    }

    if (strSource == NULL)
    {
        ERROR("invalid strSource argument\n");
        LOGEXIT("wcscpy returning char16_t NULL\n");
        PERF_EXIT(wcscpy);
        return NULL;
    }

    /* copy source string to destination string */
    while(*strSource)
    {
        *strDestination++ = *strSource++;
    }

    /* add terminating null */
    *strDestination = char16_t(0);

    LOGEXIT("wcscpy returning char16_t %p (%S)\n", start, start);
    PERF_EXIT(wcscpy);
    return start;
}


/*++
Function:
  PAL_wcslen

See MSDN or the man page for wcslen.

--*/
__attribute__((no_instrument_function))
size_t
PAL_wcslen(
        const char16_t *string)
{
    size_t nChar = 0;

    // no logging here. PAL's internal output also uses this method

    if ( !string )
    {
        return 0;
    }

    while (*string++)
    {
        nChar++;
    }

    return nChar;
}

/*++
Function:
  PAL_wmemcmp

See MSDN or the man page for wmemcmp.
--*/
int
PAL_wmemcmp(
        const char16_t *string1,
        const char16_t *string2,
        size_t count)
{
    size_t i, wi = 0;
    int diff = 0;

    PERF_ENTRY(wmemcmp);
    ENTRY("wmemcmp (string1=%p (%S), string2=%p (%S) count=%lu)\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING,
          (unsigned long) count);

    if (string1 == string2) return diff;

    constexpr size_t blockSize = sizeof(size_t) / sizeof(char16_t);
    const     size_t *num1     = (const size_t*)(string1);
    const     size_t *num2     = (const size_t*)(string2);

    while( (count > blockSize * (wi + 1)) && num1[wi] == num2[wi] ) ++wi;

    for (i = blockSize * wi; i < count; ++i)
    {
        diff = string1[i] - string2[i];
        if (diff != 0)
        {
            break;
        }
    }

    LOGEXIT("wmemcmp returning int %d\n", diff);
    PERF_EXIT(wmemcmp);
    return diff;
}

/*++
Function:
  PAL_wcsncmp

See MSDN or the man page for wcsncmp.
--*/
int
PAL_wcsncmp(
        const char16_t *string1,
        const char16_t *string2,
        size_t count)
{
    size_t i;
    int diff = 0;

    PERF_ENTRY(wcsncmp);
    ENTRY("wcsncmp (string1=%p (%S), string2=%p (%S) count=%lu)\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING,
          (unsigned long) count);

    if (string1 == string2) return diff;

    for (i = 0; i < count; i++)
    {
        diff = string1[i] - string2[i];
        if (diff != 0)
        {
            break;
        }

        /* stop if we reach the end of the string */
        if(string1[i]==0)
        {
            break;
        }
    }
    LOGEXIT("wcsncmp returning int %d\n", diff);
    PERF_EXIT(wcsncmp);
    return diff;
}

/*++
Function:
  PAL_wcscmp

See MSDN or the man page for wcscmp.
--*/
int
PAL_wcscmp(
        const char16_t *string1,
        const char16_t *string2)
{
    int ret;

    PERF_ENTRY(wcscmp);
    ENTRY("wcscmp (string1=%p (%S), string2=%p (%S))\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING);

    ret = PAL_wcsncmp(string1, string2, 0x7fffffff);

    LOGEXIT("wcscmp returns int %d\n", ret);
    PERF_EXIT(wcscmp);
    return ret;
}

/*++
Function:
  PAL_wcschr

See MSDN or man page for wcschr.

--*/
char16_t _WConst_return *
PAL_wcschr(
        const char16_t * string,
        char16_t c)
{
    PERF_ENTRY(wcschr);
    ENTRY("wcschr (string=%p (%S), c=%C)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING, c);

    while (*string)
    {
        if (*string == c)
        {
            LOGEXIT("wcschr returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
            PERF_EXIT(wcschr);
            return (char16_t *) string;
        }
        string++;
    }

    // Check if the comparand was \000
    if (*string == c)
        return (char16_t *) string;

    LOGEXIT("wcschr returning char16_t NULL\n");
    PERF_EXIT(wcschr);
    return NULL;
}


/*++
Function:
  PAL_wcsrchr

See MSDN or man page for wcsrchr.

--*/
char16_t _WConst_return *
PAL_wcsrchr(
        const char16_t * string,
        char16_t c)
{
    char16_t *last = NULL;

    PERF_ENTRY(wcsrchr);
    ENTRY("wcsrchr (string=%p (%S), c=%C)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING, c);

    while (*string)
    {
        if (*string == c)
        {
            last = (char16_t *) string;
        }
        string++;
    }

    LOGEXIT("wcsrchr returning char16_t %p (%S)\n", last?last:W16_NULLSTRING, last?last:W16_NULLSTRING);
    PERF_EXIT(wcsrchr);
    return (char16_t *)last;
}


/*++
Function:
  PAL_wcsspn

See MSDN or man page for wcspbrk.
--*/
size_t
PAL_wcsspn (const char16_t *string, const char16_t *stringCharSet)
{
    ASSERT(0);
    return 0;
}


/*++
Function:
  PAL_wcspbrk

See MSDN or man page for wcspbrk.
--*/
const char16_t *
PAL_wcspbrk(
        const char16_t *string,
        const char16_t *strCharSet)
{
    PERF_ENTRY(wcspbrk);
    ENTRY("wcspbrk (string=%p (%S), strCharSet=%p (%S))\n",
          string?string:W16_NULLSTRING,
          string?string:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING);

    while (*string)
    {
        if (PAL_wcschr(strCharSet, *string) != NULL)
        {
            LOGEXIT("wcspbrk returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
            PERF_EXIT(wcspbrk);
            return (char16_t *) string;
        }

        string++;
    }

    LOGEXIT("wcspbrk returning char16_t NULL\n");
    PERF_EXIT(wcspbrk);
    return NULL;
}


/*++
Function:
  PAL_wcsstr

See MSDN or man page for wcsstr.
--*/
const char16_t *
PAL_wcsstr(
        const char16_t *string,
        const char16_t *strCharSet)
{
    char16_t *ret = NULL;
    int i;

    PERF_ENTRY(wcsstr);
    ENTRY("wcsstr (string=%p (%S), strCharSet=%p (%S))\n",
      string?string:W16_NULLSTRING,
      string?string:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING);

    if (string == NULL)
    {
        ret = NULL;
        goto leave;
    }

    if (strCharSet == NULL)
    {
        ret = NULL;
        goto leave;
    }

    if (*strCharSet == 0)
    {
        ret = (char16_t *)string;
        goto leave;
    }

    while (*string != 0)
    {
        i = 0;
        while (1)
        {
            if (*(strCharSet + i) == 0)
            {
                ret = (char16_t *) string;
                goto leave;
            }
            else if (*(string + i) == 0)
            {
                ret = NULL;
                goto leave;
            }
            if (*(string + i) != *(strCharSet + i))
            {
                break;
            }

            i++;
        }
        string++;
    }

 leave:
    LOGEXIT("wcsstr returning char16_t %p (%S)\n", ret?ret:W16_NULLSTRING, ret?ret:W16_NULLSTRING);
    PERF_EXIT(wcsstr);
    return ret;
}

/*++
Function :

    PAL_wcsncpy

see msdn doc.
--*/
char16_t *
PAL_wcsncpy( char16_t * strDest, const char16_t *strSource, size_t count )
{
    uint32_t length = sizeof( char16_t ) * count;
    PERF_ENTRY(wcsncpy);
    ENTRY("wcsncpy( strDest:%p, strSource:%p (%S), count:%lu)\n",
          strDest, strSource, strSource, (unsigned long) count);

    memset( strDest, 0, length );
    length = min( count, PAL_wcslen( strSource ) ) * sizeof( char16_t );
    memcpy( strDest, strSource, length );

    LOGEXIT("wcsncpy returning (char16_t*): %p\n", strDest);
    PERF_EXIT(wcsncpy);
    return strDest;
}

/*++
Function :

    wcsncat

see msdn doc.
--*/
char16_t *
PAL_wcsncat( char16_t * strDest, const char16_t *strSource, size_t count )
{
    char16_t *start = strDest;
    uint32_t LoopCount = 0;
    uint32_t StrSourceLength = 0;

    PERF_ENTRY(wcsncat);
    ENTRY( "wcsncat (strDestination=%p (%S), strSource=%p (%S), count=%lu )\n",
            strDest ? strDest : W16_NULLSTRING,
            strDest ? strDest : W16_NULLSTRING,
            strSource ? strSource : W16_NULLSTRING,
            strSource ? strSource : W16_NULLSTRING, (unsigned long) count);

    if ( strDest == NULL )
    {
        ERROR("invalid strDest argument\n");
        LOGEXIT("wcsncat returning char16_t NULL\n");
        PERF_EXIT(wcsncat);
        return NULL;
    }

    if ( strSource == NULL )
    {
        ERROR("invalid strSource argument\n");
        LOGEXIT("wcsncat returning char16_t NULL\n");
        PERF_EXIT(wcsncat);
        return NULL;
    }

    /* find end of source string */
    while ( *strDest )
    {
        strDest++;
    }

    StrSourceLength = PAL_wcslen( strSource );
    if ( StrSourceLength < count )
    {
        count = StrSourceLength;
    }

    /* concatenate new string */
    while( *strSource && LoopCount < count )
    {
      *strDest++ = *strSource++;
      LoopCount++;
    }

    /* add terminating null */
    *strDest = '\0';

    LOGEXIT("wcsncat returning char16_t %p (%S)\n", start, start);
    PERF_EXIT(wcsncat);
    return start;
}

static BOOL MISC_CRT_WCSTOD_IsValidCharacter( WCHAR c )
{
    if ( c == '+' || c == '-' || c == '.' || ( c >= '0' && c <= '9' ) ||
         c == 'e' || c == 'E' || c == 'd' || c == 'D' )
    {
        return TRUE;
    }
    return FALSE;
}

/*++
Function :

    wcstod

    There is a slight difference between the Windows version of wcstod
    and the BSD versio of wcstod.

    Under Windows the string "  -1b  " returns -1.000000 stop char = 'b'
    Under BSD the same string returns 0.000000 stop ' '

see msdn doc.
--*/
double
PAL_wcstod( const char16_t * nptr, char16_t **endptr )
{
    double RetVal = 0.0;
    LPSTR  lpStringRep = NULL;
    LPWSTR lpStartOfExpression = (LPWSTR)nptr;
    LPWSTR lpEndOfExpression = NULL;
    uint32_t Length = 0;

    PERF_ENTRY(wcstod);
    ENTRY( "wcstod( %p (%S), %p (%S) )\n", nptr, nptr, endptr , endptr );

    if ( !nptr )
    {
        ERROR( "nptr is invalid.\n" );
        LOGEXIT( "wcstod returning 0.0\n" );
        PERF_EXIT(wcstod);
        return 0.0;
    }

    /* Eat white space. */
    while ( PAL_iswspace( *lpStartOfExpression ) )
    {
        lpStartOfExpression++;
    }

    /* Get the end of the expression. */
    lpEndOfExpression = lpStartOfExpression;
    while ( *lpEndOfExpression )
    {
        if ( !MISC_CRT_WCSTOD_IsValidCharacter( *lpEndOfExpression ) )
        {
            break;
        }
        lpEndOfExpression++;
    }

    if ( lpEndOfExpression != lpStartOfExpression )
    {
        Length = lpEndOfExpression - lpStartOfExpression;
        lpStringRep = (LPSTR)PAL_malloc( Length + 1);

        if ( lpStringRep )
        {
            if ( WideCharToMultiByte( CP_ACP, 0, lpStartOfExpression, Length,
                                      lpStringRep, Length + 1 ,
                                      NULL, 0 ) != 0 )
            {
                LPSTR ScanStop = NULL;
                lpStringRep[Length]= 0;
                RetVal = strtod( lpStringRep, &ScanStop );

                /* See if strtod failed. */
                if ( RetVal == 0.0 && ScanStop == lpStringRep )
                {
                    ASSERT( "An error occurred in the conversion.\n" );
                    lpEndOfExpression = (LPWSTR)nptr;
                }
            }
            else
            {
                ASSERT( "Wide char to multibyte conversion failed.\n" );
                lpEndOfExpression = (LPWSTR)nptr;
            }
        }
        else
        {
            ERROR( "Not enough memory.\n" );
            lpEndOfExpression = (LPWSTR)nptr;
        }
    }
    else
    {
        ERROR( "Malformed expression.\n" );
        lpEndOfExpression = (LPWSTR)nptr;
    }

    /* Set the stop scan character. */
    if ( endptr != NULL )
    {
        *endptr = lpEndOfExpression;
    }

    PAL_free( lpStringRep );
    LOGEXIT( "wcstod returning %f.\n", RetVal );
    PERF_EXIT(wcstod);
    return RetVal;
}

/*++
Function :

    _ui64tow

See MSDN for more details.
--*/
char16_t *
_ui64tow( unsigned __int64 value , char16_t * string , int radix )
{
    uint32_t ReversedIndex = 0;
    WCHAR ReversedString[ 65 ];
    LPWSTR lpString = string;
    uint32_t Index = 0;

    PERF_ENTRY(_ui64tow);
    ENTRY( "_ui64tow( value=%I64d, string=%p (%S), radix=%d )\n",
           value, string, string, radix );

    if ( !string )
    {
        ERROR( "string has to be a valid pointer.\n" );
        LOGEXIT( "_ui64tow returning NULL.\n" );
        PERF_EXIT(_ui64tow);
        return NULL;
    }
    if ( radix < 2 || radix > 36 )
    {
        ERROR( "radix has to be between 2 and 36.\n" );
        LOGEXIT( "_ui64tow returning NULL.\n" );
        PERF_EXIT(_ui64tow);
        return NULL;
    }

    if(0 == value)
    {
        ReversedString[0] = '0';
        Index++;
    }
    else while ( value )
    {
        int temp = value % radix;
        value /= radix;

        if ( temp < 10 )
        {
            ReversedString[ Index ] = temp + '0';
            Index++;
        }
        else
        {
            ReversedString[ Index ] = temp - 10 + 'a';
            Index++;
        }
    }

    /* Reverse the string. */
    ReversedIndex = Index;
    for ( Index = 0; ReversedIndex > 0; ReversedIndex--, Index++ )
    {
        string[ Index ] = ReversedString[ ReversedIndex - 1 ];
    }

    string[ Index ] = '\0';
    LOGEXIT( "_ui64tow returning %p (%S).\n", lpString , lpString );
    PERF_EXIT(_ui64tow);
    return lpString;
}


/*++
Function:

    iswdigit

See MSDN for more details.
--*/
int
PAL_iswdigit( char16_t c )
{
    uint32_t nRetVal = 0;
#if HAVE_COREFOUNDATION
    static CFCharacterSetRef sDigitSet;

    if (sDigitSet == NULL)
    {
        sDigitSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetDecimalDigit);
    }
    PERF_ENTRY(iswdigit);
    ENTRY("PAL_iswdigit (c=%d)\n", c);
    nRetVal = CFCharacterSetIsCharacterMember(sDigitSet, c);
#else   /* HAVE_COREFOUNDATION */
    UnicodeDataRec dataRec;

    PERF_ENTRY(iswdigit);
    ENTRY("PAL_iswdigit (c=%d)\n", c);

    if (GetUnicodeData(c, &dataRec))
    {
        if (dataRec.C1_TYPE_FLAGS & C1_DIGIT)
        {
            nRetVal = 1;
        }
        else
        {
            nRetVal = 0;
        }
    }
    else
    {
        TRACE( "No corresponding unicode record for character %d.\n", c );
    }
#endif  /* HAVE_COREFOUNDATION */
    LOGEXIT("PAL_iswdigit returning %d\n", nRetVal);
    PERF_EXIT(iswdigit);
    return nRetVal;
}

/*++
Function:

    iswxdigit

See MSDN for more details.

Notes :
the information in UnicodeData doesn't help us, it doesn't have enough
granularity. Results in windows show that only ASCII and "Fullwidth" (>0xFF10)
numbers and letters are considered as "hex"; other "numbers"
(nGeneralCategory==8) aren't.
--*/
int
PAL_iswxdigit( char16_t c )
{
    uint32_t nRetVal = 0;

    PERF_ENTRY(iswxdigit);
    ENTRY("PAL_iswxdigit( c=%d )\n", c);

    /* ASCII characters */
    if((c>= 'A' && c<='F') ||        /* uppercase hex letters */
       (c>= 'a' && c<='f') ||        /* lowercase hex letters */
       (c>= '0' && c<='9'))          /* digits */
    {
        nRetVal = 1;
    }
    else
    /* "fullwidth" characters, whatever that is */
    if((c>= 0xFF10 && c<=0xFF19) ||  /* digits */
       (c>= 0xFF21 && c<=0xFF26) ||  /* uppercase hex letters */
       (c>= 0xFF41 && c<=0xFF46))    /* lowercase hex letters */
    {
        nRetVal = 1;
    }
    else
    {
        nRetVal = 0;
    }
    LOGEXIT("PAL_iswxdigit returning %d\n", nRetVal);
    PERF_EXIT(iswxdigit);
    return nRetVal;
}


/*++
Function:

    iswprint

See MSDN for more details.
--*/
int
PAL_iswprint( char16_t c )
{
    int ret;


    PERF_ENTRY(iswprint);
    ENTRY("PAL_iswprint (%#X)\n", c);

    ret = proxy_iswprint(c);

    LOGEXIT("PAL_iswprint returns %d\n", ret);
    PERF_EXIT(iswprint);
    return (ret);
}


/*++
Function:
   PAL_wcscspn

Finds the number of consecutive characters from the start of the string
that are not in the set.

Return value:

The number of characters from the start of the string that are not in
the set.

Parameters:
string          String
strCharSet      Set of delimiter characters

--*/
size_t
PAL_wcscspn(const char16_t *string, const char16_t *strCharSet)
{
    const char16_t *temp;
    size_t count = 0;

    PERF_ENTRY(wcscspn);

    while(*string != 0)
    {
        for(temp = strCharSet; *temp != 0; temp++)
        {
            if (*string == *temp)
            {
                PERF_EXIT(wcscspn);
                return count;
            }
        }
        count++;
        string++;
    }
    PERF_EXIT(wcscspn);
    return count;
}

#if HAVE_COREFOUNDATION
/*--
Function:
  PAL_iswblank

Returns TRUE if c is a Win32 "blank" character.
--*/
int
PAL_iswblank(char16_t c)
{
    int ret;
    static CFCharacterSetRef sSpaceAndNewlineSet;

    if (sSpaceAndNewlineSet == NULL)
    {
        sSpaceAndNewlineSet = CFCharacterSetGetPredefined(
                                            kCFCharacterSetWhitespaceAndNewline);
    }
    switch (c)
    {
        case 0x0085:
        case 0x1680:
        case 0x202f:
        case 0xfeff:
            // These are blank characters on Windows, but are not part
            // of the SpaceAndNewline character set in Core Foundation.
            ret = TRUE;
            break;
        case 0x2028:
        case 0x2029:
            // These are not blank characters on Windows, but are part
            // of the SpaceAndNewline character set in Core Foundation.
            ret = FALSE;
            break;
        default:
            ret = CFCharacterSetIsCharacterMember(sSpaceAndNewlineSet, c);
            break;
    }
    return ret;
}

/*--
Function:
  PAL_iswcntrl

Returns TRUE if c is a control character.
--*/
int
PAL_iswcntrl(char16_t c)
{
    int ret;
    static CFCharacterSetRef sControlSet;

    if (sControlSet == NULL)
    {
        sControlSet = CFCharacterSetGetPredefined(kCFCharacterSetControl);
    }
    ret = CFCharacterSetIsCharacterMember(sControlSet, c);
    return ret;
}

/*--
Function:
  PAL_iswpunct

Returns TRUE if c is a punctuation character.
--*/
int
PAL_iswpunct(char16_t c)
{
    int ret;
    static CFCharacterSetRef sPunctuationSet = NULL;
    static CFCharacterSetRef sSymbolSet = NULL;

    if (sPunctuationSet == NULL)
    {
        sPunctuationSet = CFCharacterSetGetPredefined(kCFCharacterSetPunctuation);
    }
    if (sSymbolSet == NULL)
    {
        sSymbolSet = CFCharacterSetGetPredefined(kCFCharacterSetSymbol);
    }
    ret = CFCharacterSetIsCharacterMember(sPunctuationSet, c) ||
          CFCharacterSetIsCharacterMember(sSymbolSet, c);
    return ret;
}
#endif  // HAVE_COREFOUNDATION
