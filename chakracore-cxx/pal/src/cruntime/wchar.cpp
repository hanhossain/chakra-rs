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
#include <limits.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);

char16_t *PAL_wcsncat( char16_t * strDest, const char16_t *strSource, size_t count );


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
char16_t* Internal_i64tow(int64_t value, char16_t* string, int radix, BOOL isI64)
{
    int length = 0;
    int n;
    int r;
    unsigned long uval = value;
    char16_t* stringPtr = string;
    int start = 0;
    int end;
    char16_t tempCh;

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

    ENTRY("_itow (value=%d, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, FALSE);

    LOGEXIT("_itow returns char16_t* %p\n", ret);

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

    ENTRY("_ltow (value=%d, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, FALSE);

    LOGEXIT("_ltow returns char16_t* %p\n", ret);

    return ret;
}

/*--
Function:
  _i64tow

See MSDN doc
--*/
char16_t *
 _i64tow(
    long value,
    char16_t *string,
    int radix)
{
    char16_t *ret;

    ENTRY("_i64tow (value=%ld, string=%p, radix=%d)\n",
          value, string, radix);

    ret = Internal_i64tow(value, string, radix, TRUE);

    LOGEXIT("_i64tow returns char16_t* %p\n", ret);

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

    ENTRY("_wtoi (string=%p)\n", string);

    len = WideCharToMultiByte(CP_ACP, 0, string, -1, 0, 0, 0, 0);
    if (!len)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n",
              GetLastError());
        return -1;
    }
    tempStr = (char *) malloc(len);
    if (!tempStr)
    {
        ERROR("malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return -1;
    }
    len = WideCharToMultiByte(CP_ACP, 0, string, -1, tempStr, len, 0, 0);
    if (!len)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n",
              GetLastError());
        free(tempStr);
        return -1;
    }
    ret = atoi(tempStr);

    free(tempStr);
    LOGEXIT("_wtoi returns int %d\n", ret);
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

    ENTRY("PAL_iswspace (c=%C)\n", c);

    ret = proxy_iswspace(c);

    LOGEXIT("PAL_iswspace returns int %d\n", ret);
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

    ENTRY("_wcsicmp (string1=%p (%S), string2=%p (%S))\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING);

    ret = _wcsnicmp(string1, string2, 0x7fffffff);

    LOGEXIT("_wcsnicmp returns int %d\n", ret);
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

    ENTRY("_wcslwr (string=%p (%S))\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);

    for (i=0 ; string[i] != 0; i++)
    {
        string[i] = wtolower(string[i]);
    }

    LOGEXIT("_wcslwr returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
    return string;
}

unsigned long
PAL__wcstoui64(
        const char16_t *nptr,
        char16_t **endptr,
        int base)
{
    char *s_nptr = 0;
    char *s_endptr = 0;
    unsigned long long res;
    int size;

    ENTRY("_wcstoui64 (nptr=%p (%S), endptr=%p, base=%d)\n", nptr?nptr:W16_NULLSTRING, nptr?nptr:W16_NULLSTRING,
          endptr, base);

    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, NULL, 0, NULL, NULL);
    if (!size)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", GetLastError());
        SetLastError(ERROR_INVALID_PARAMETER);
        res = 0;
        goto PAL__wcstoui64Exit;
    }
    s_nptr = (char *)malloc(size);
    if (!s_nptr)
    {
        ERROR("malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        res = 0;
        goto PAL__wcstoui64Exit;
    }
    size = WideCharToMultiByte(CP_ACP, 0, nptr, -1, s_nptr, size, NULL, NULL);
    if (!size)
    {
        ASSERT("WideCharToMultiByte failed.  Error is %d\n", GetLastError());
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
    free(s_nptr);
    LOGEXIT("_wcstoui64 returning unsigned long long %llu\n", res);

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
    ENTRY("towlower (c=%d)\n", c);
    if(c < 128)
    {//fast path for ascii characters
        if(c >= 'A' && c <= 'Z')
        {
            c += ('a' - 'A');
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }
        else
        {
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }
    }
    else
    {
    #if defined(__APPLE__)
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
        return c;
    #else   /* defined(__APPLE__) */
        UnicodeDataRec dataRec;

        if (!GetUnicodeData(c, &dataRec))
        {
            TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }

        if ( (dataRec.C1_TYPE_FLAGS & C1_LOWER) || (dataRec.nOpposingCase ==  0 ))
        {
            LOGEXIT("towlower returns int %d\n", c );
            return c;
        }
        else
        {
            LOGEXIT("towlower returns int %d\n", dataRec.nOpposingCase );
            return dataRec.nOpposingCase;
        }
    #endif  /* defined(__APPLE__) */
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
#if defined(__APPLE__)
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
    return c;
#else   /* defined(__APPLE__) */
    UnicodeDataRec dataRec;

    ENTRY("towupper (c=%d)\n", c);

    if (!GetUnicodeData(c, &dataRec))
    {
        TRACE( "Unable to retrieve unicode data for the character %c.\n", c );
        LOGEXIT("towupper returns int %d\n", c );
        return c;
    }

    if ( (dataRec.C1_TYPE_FLAGS & C1_UPPER) || (dataRec.nOpposingCase ==  0 ))
    {
        LOGEXIT("towupper returns int %d\n", c );
        return c;
    }
    else
    {
        LOGEXIT("towupper returns int %d\n", dataRec.nOpposingCase );
        return dataRec.nOpposingCase;
    }
#endif  /* defined(__APPLE__) */
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
#if defined(__APPLE__)
    static CFCharacterSetRef sUppercaseSet;

    if (sUppercaseSet == NULL)
    {
        sUppercaseSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetUppercaseLetter);
    }
    ENTRY( "iswupper (c=%d)\n", c );
    bRetVal = CFCharacterSetIsCharacterMember(sUppercaseSet, c);
#else   /* defined(__APPLE__) */
    UnicodeDataRec dataRec;

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
#endif  /* defined(__APPLE__) */
    LOGEXIT( "iswupper returns %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE" );
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
#if defined(__APPLE__)
    static CFCharacterSetRef sLowercaseSet;

    if (sLowercaseSet == NULL)
    {
        sLowercaseSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetLowercaseLetter);
    }
    ENTRY("PAL_iswlower (c=%d)\n", c);
    bRetVal = CFCharacterSetIsCharacterMember(sLowercaseSet, c);
#else   /* defined(__APPLE__) */
    UnicodeDataRec dataRec;

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
#endif  /* defined(__APPLE__) */
    LOGEXIT("PAL_iswlower returns %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE");
    return bRetVal;
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

    ENTRY("wcscpy (strDestination=%p, strSource=%p (%S))\n",
          strDestination, strSource ? strSource:W16_NULLSTRING, strSource ? strSource:W16_NULLSTRING);

    if (strDestination == NULL)
    {
        ERROR("invalid strDestination argument\n");
        LOGEXIT("wcscpy returning char16_t NULL\n");
        return NULL;
    }

    if (strSource == NULL)
    {
        ERROR("invalid strSource argument\n");
        LOGEXIT("wcscpy returning char16_t NULL\n");
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

    ENTRY("wcscmp (string1=%p (%S), string2=%p (%S))\n",
          string1?string1:W16_NULLSTRING,
          string1?string1:W16_NULLSTRING, string2?string2:W16_NULLSTRING, string2?string2:W16_NULLSTRING);

    ret = PAL_wcsncmp(string1, string2, 0x7fffffff);

    LOGEXIT("wcscmp returns int %d\n", ret);
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
    ENTRY("wcschr (string=%p (%S), c=%C)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING, c);

    while (*string)
    {
        if (*string == c)
        {
            LOGEXIT("wcschr returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
            return (char16_t *) string;
        }
        string++;
    }

    // Check if the comparand was \000
    if (*string == c)
        return (char16_t *) string;

    LOGEXIT("wcschr returning char16_t NULL\n");
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
    return (char16_t *)last;
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
    ENTRY("wcspbrk (string=%p (%S), strCharSet=%p (%S))\n",
          string?string:W16_NULLSTRING,
          string?string:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING, strCharSet?strCharSet:W16_NULLSTRING);

    while (*string)
    {
        if (PAL_wcschr(strCharSet, *string) != NULL)
        {
            LOGEXIT("wcspbrk returning char16_t %p (%S)\n", string?string:W16_NULLSTRING, string?string:W16_NULLSTRING);
            return (char16_t *) string;
        }

        string++;
    }

    LOGEXIT("wcspbrk returning char16_t NULL\n");
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
    return ret;
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

    ENTRY( "wcsncat (strDestination=%p (%S), strSource=%p (%S), count=%lu )\n",
            strDest ? strDest : W16_NULLSTRING,
            strDest ? strDest : W16_NULLSTRING,
            strSource ? strSource : W16_NULLSTRING,
            strSource ? strSource : W16_NULLSTRING, (unsigned long) count);

    if ( strDest == NULL )
    {
        ERROR("invalid strDest argument\n");
        LOGEXIT("wcsncat returning char16_t NULL\n");
        return NULL;
    }

    if ( strSource == NULL )
    {
        ERROR("invalid strSource argument\n");
        LOGEXIT("wcsncat returning char16_t NULL\n");
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
    return start;
}

/*++
Function :

    _ui64tow

See MSDN for more details.
--*/
char16_t *
_ui64tow( unsigned long value , char16_t * string , int radix )
{
    uint32_t ReversedIndex = 0;
    char16_t ReversedString[ 65 ];
    char16_t* lpString = string;
    uint32_t Index = 0;

    ENTRY( "_ui64tow( value=%I64d, string=%p (%S), radix=%d )\n",
           value, string, string, radix );

    if ( !string )
    {
        ERROR( "string has to be a valid pointer.\n" );
        LOGEXIT( "_ui64tow returning NULL.\n" );
        return NULL;
    }
    if ( radix < 2 || radix > 36 )
    {
        ERROR( "radix has to be between 2 and 36.\n" );
        LOGEXIT( "_ui64tow returning NULL.\n" );
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
#if defined(__APPLE__)
    static CFCharacterSetRef sDigitSet;

    if (sDigitSet == NULL)
    {
        sDigitSet = CFCharacterSetGetPredefined(
                                        kCFCharacterSetDecimalDigit);
    }
    ENTRY("PAL_iswdigit (c=%d)\n", c);
    nRetVal = CFCharacterSetIsCharacterMember(sDigitSet, c);
#else   /* defined(__APPLE__) */
    UnicodeDataRec dataRec;

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
#endif  /* defined(__APPLE__) */
    LOGEXIT("PAL_iswdigit returning %d\n", nRetVal);
    return nRetVal;
}

#if defined(__APPLE__)
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
#endif  // defined(__APPLE__)
