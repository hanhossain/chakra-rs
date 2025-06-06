//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    printf.c

Abstract:

    Implementation of the printf family functions.

Revision History:



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/cruntime.h"
#include "pal/thread.hpp"
#include "pal/threadsusp.hpp"
#include "pal/printfcpp.hpp"

/* <stdarg.h> needs to be included after "palinternal.h" to avoid name
   collision for va_start and va_end */
#include <stdarg.h>
#include <errno.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);

#if SSCANF_SUPPORT_ll
const static char *scanf_longlongfmt = "ll";
#else
const static char *scanf_longlongfmt = "q";
#endif

#if SSCANF_CANNOT_HANDLE_MISSING_EXPONENT
static int SscanfFloatCheckExponent(LPCSTR buff, LPCSTR floatFmt,
                                      void * voidPtr, int * pn);
#endif // SSCANF_CANNOT_HANDLE_MISSING_EXPONENT

/*******************************************************************************
Function:
  Internal_AddPaddingA

Parameters:
  Out
    - buffer to place padding and given string (In)
  Count
    - maximum chars to be copied so as not to overrun given buffer
  In
    - string to place into (Out) accompanied with padding
  Padding
    - number of padding chars to add
  Flags
    - padding style flags (PRINTF_FORMAT_FLAGS)
*******************************************************************************/
__attribute__((no_instrument_function))
BOOL Internal_AddPaddingA(LPSTR *Out, int32_t Count, LPSTR In,
                                 int32_t Padding, int32_t Flags)
{
    LPSTR OutOriginal = *Out;
    int32_t PaddingOriginal = Padding;
    int32_t LengthInStr;
    LengthInStr = strlen(In);


    if (Padding < 0)
    {
        /* this is used at the bottom to determine if the buffer ran out */
        PaddingOriginal = 0;
    }
    if (Flags & PFF_MINUS) /* pad on right */
    {
        if (strncpy_s(*Out, Count, In, min(LengthInStr + 1, Count)) != SAFECRT_SUCCESS)
        {
            return FALSE;
        }

        *Out += min(LengthInStr, Count);
    }
    if (Padding > 0)
    {
        if (Flags & PFF_ZERO) /* '0', pad with zeros */
        {
            while (Padding-- && Count > *Out - OutOriginal)
            {
                *(*Out)++ = '0';
            }
        }
        else /* pad left with spaces */
        {
            while (Padding-- && Count > *Out - OutOriginal)
            {
                *(*Out)++ = ' ';
            }
        }
    }
    if (!(Flags & PFF_MINUS)) /* put 'In' after padding */
    {
        if (strncpy_s(*Out, Count, In,
                min(LengthInStr + 1, Count - (*Out - OutOriginal))) != SAFECRT_SUCCESS)
        {
            return FALSE;
        }

        *Out += min(LengthInStr, Count - (*Out - OutOriginal));
    }

    if (LengthInStr + PaddingOriginal > Count)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*******************************************************************************
Function:
  PAL_printf_arg_remover

Parameters:
  ap
    - pointer to the va_list from which to remove arguments
  Width
    - the width of the current format operation
  Precision
    - the precision of the current format option
  Type
    - the type of the argument for the current format option
  Prefix
    - the prefix for the current format option
*******************************************************************************/
__attribute__((no_instrument_function))
void PAL_printf_arg_remover(va_list *ap, int32_t Width, int32_t Precision, int32_t Type, int32_t Prefix)
{
    /* remove arg and precision if needed */
    if (PRECISION_STAR == Precision ||
        PRECISION_INVALID == Precision)
    {
        (void)va_arg(*ap, int);
    }
    if (WIDTH_STAR == Width ||
        WIDTH_INVALID == Width)
    {
        (void)va_arg(*ap, int);
    }
    if (Type == PFF_TYPE_FLOAT)
    {
        (void)va_arg(*ap, double);
    }
    else if (Type == PFF_TYPE_INT && Prefix == PFF_PREFIX_LONGLONG)
    {
        (void)va_arg(*ap, int64_t);
    }
    else if (Type == PFF_TYPE_INT || Type == PFF_TYPE_CHAR)
    {
        (void)va_arg(*ap, int);
    }
    else
    {
        (void)va_arg(*ap, void *);
    }
}

/*++
Function:
  PAL_fprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_fprintf(PAL_FILE *stream,const char *format,...)
{
    int32_t Length = 0;
    va_list ap;

    PERF_ENTRY(fprintf);
    ENTRY("PAL_fprintf(stream=%p,format=%p (%s))\n",stream, format, format);

    va_start(ap, format);
    Length = PAL_vfprintf( stream, format, ap);
    va_end(ap);

    LOGEXIT("PAL_fprintf returns int %d\n", Length);
    PERF_EXIT(fprintf);
    return Length;
}

/*++
Function:
  PAL_wprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_wprintf(
      const char16_t *format,
      ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(wprintf);
    ENTRY("PAL_wprintf (format=%p (%S))\n", format, format);

    va_start(ap, format);
    Length = PAL_vfwprintf( PAL_get_stdout(PAL_get_caller), format, ap);
    va_end(ap);

    LOGEXIT("PAL_wprintf returns int %d\n", Length);
    PERF_EXIT(wprintf);
    return Length;
}

/*++
Function:
  wsprintfA

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
wsprintfA(
       LPSTR buffer,
       LPCSTR format,
      ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(wsprintfA);
    ENTRY("wsprintfA (buffer=%p, format=%p (%s))\n", buffer, format, format);

    va_start(ap, format);
    Length = PAL__vsnprintf(buffer, 1024, format, ap);
    va_end(ap);

    LOGEXIT("wsprintfA returns int %d\n", Length);
    PERF_EXIT(wsprintfA);
    return Length;
}

/*++
Function:
  wsprintfW

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
wsprintfW(
       LPWSTR buffer,
       LPCWSTR format,
      ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(wsprintfW);
    ENTRY("wsprintfW (buffer=%p, format=%p (%S))\n", buffer, format, format);

    va_start(ap, format);
    Length = PAL__wvsnprintf(buffer, 1024, format, ap);
    va_end(ap);

    LOGEXIT("wsprintfW returns int %d\n", Length);
    PERF_EXIT(wsprintfW);
    return Length;
}


/*++
Function:
  _snprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
_snprintf(
     char *buffer,
     size_t count,
     const char *format,
     ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(_snprintf);
    ENTRY("_snprintf (buffer=%p, count=%lu, format=%p (%s))\n",
          buffer, (unsigned long) count, format, format);

    va_start(ap, format);
    Length = PAL__vsnprintf(buffer, count, format, ap);
    va_end(ap);

    LOGEXIT("_snprintf returns int %d\n", Length);
    PERF_EXIT(_snprintf);
    return Length;
}


/*++
Function:
  _snwprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
_snwprintf(
     char16_t *buffer,
     size_t count,
     const char16_t *format,
     ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(_snwprintf);
    ENTRY("_snwprintf (buffer=%p, count=%lu, format=%p (%S))\n",
          buffer, (unsigned long) count, format, format);

    va_start(ap, format);
    Length = PAL__wvsnprintf(buffer, count, format, ap);
    va_end(ap);

    LOGEXIT("_snwprintf returns int %d\n", Length);
    PERF_EXIT(_snwprintf);
    return Length;
}

/*++
Function:
  fwprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_fwprintf(
     PAL_FILE *stream,
     const char16_t *format,
     ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(fwprintf);
    ENTRY("PAL_fwprintf (stream=%p, format=%p (%S))\n", stream, format, format);

    va_start(ap, format);
    Length = PAL_vfwprintf( stream, format, ap);
    va_end(ap);

    LOGEXIT("PAL_fwprintf returns int %d\n", Length);
    PERF_EXIT(fwprintf);
    return Length;
}

/*******************************************************************************
Function:
  Internal_ScanfExtractFormatA

Parameters:
  Fmt
    - format string to parse
    - first character must be a '%'
    - parameter gets updated to point to the character after
      the %<foo> format string
  Out
    - buffer will contain the %<foo> format string
  Store
    - boolean value representing whether to store the type to be parsed
    - '*' flag
  Width
    - will contain the width specified by the format string
    - -1 if none given
  Prefix
    - an enumeration of the type prefix
  Type
    - an enumeration of the value type to be parsed

Notes:
  - I'm also handling the undocumented %ws, %wc, %w...
*******************************************************************************/

#define CHECK_OUT_IN_ITS_RANGE(Out,BeginOut,EndOut)                                             \
    if ((Out)<(BeginOut) || (Out)>=(EndOut))                                                    \
    {                                                                                           \
        SetLastError(ERROR_INSUFFICIENT_BUFFER);                                                \
        ERROR("Pointer Out wanted to access 0x%p. However the range of buffer is [0x%p,0x%p).",\
                 (Out), (BeginOut), (EndOut));                                                  \
        return false;                                                                           \
    }

__attribute__((no_instrument_function))
static BOOL Internal_ScanfExtractFormatA(LPCSTR *Fmt, LPSTR Out, int iOutSize, LPBOOL Store,
                                         int32_t * Width, int32_t * Prefix, int32_t * Type)
{
    BOOL Result = FALSE;
    LPSTR TempStr;
    LPSTR TempStrPtr;
    LPSTR BaseOut = Out;
    LPSTR EndOut = Out + iOutSize;

    *Width = -1;
    *Store = TRUE;
    *Prefix = -1;
    *Type = -1;

    if (*Fmt && **Fmt == '%')
    {
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = *(*Fmt)++;
    }
    else
    {
        return Result;
    }

    /* we'll never need a temp string longer than the original */
    TempStrPtr = TempStr = (LPSTR) PAL_malloc(strlen(*Fmt)+1);
    if (!TempStr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return Result;
    }

    /* parse '*' flag which means don't store */
    if (**Fmt == '*')
    {
        *Store = FALSE;
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = *(*Fmt)++;
    }

    /* grab width specifier */
    if (isdigit((unsigned char) **Fmt))
    {
        TempStrPtr = TempStr;
        while (isdigit((unsigned char) **Fmt))
        {
            *TempStrPtr++ = **Fmt;
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = *(*Fmt)++;
        }
        *TempStrPtr = 0; /* end string */
        *Width = atoi(TempStr);
        if (*Width < 0)
        {
            ERROR("atoi returned a negative value indicative of an overflow.\n");
            SetLastError(ERROR_INTERNAL_ERROR);
            return Result;
        }
    }

    if (**Fmt == 'p')
    {
        *Prefix = SCANF_PREFIX_LONGLONG;
    }
    /* grab prefix of 'I64' for __int64 */
    if ((*Fmt)[0] == 'I' && (*Fmt)[1] == '6' && (*Fmt)[2] == '4')
    {
        /* convert to 'q'/'ll' so Unix sscanf can handle it */
        *Fmt += 3;
        *Prefix = SCANF_PREFIX_LONGLONG;
    }
    /* grab a prefix of 'h' */
    else if (**Fmt == 'h')
    {
        *Prefix = SCANF_PREFIX_SHORT;
        ++(*Fmt);
    }
    /* grab prefix of 'l' or the undocumented 'w' (at least in MSDN) */
    else if (**Fmt == 'l' || **Fmt == 'w')
    {
        ++(*Fmt);
        // Only want to change the prefix on 64 bit when inputing characters.
        if (**Fmt == 'c' || **Fmt == 's')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (**Fmt == 'l')
        {
            *Prefix = SCANF_PREFIX_LONGLONG;
            ++(*Fmt);
        }
    }
    else if (**Fmt == 'L')
    {
        /* a prefix of 'L' seems to be ignored */
        ++(*Fmt);
    }

    /* grab type 'c' */
    if (**Fmt == 'c' || **Fmt == 'C')
    {
        *Type = SCANF_TYPE_CHAR;
        if (*Prefix != SCANF_PREFIX_SHORT && **Fmt == 'C')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == SCANF_PREFIX_LONG)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = 'l';
        }
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = 'c';
        ++(*Fmt);
        Result = TRUE;
    }
    /* grab type 's' */
    else if (**Fmt == 's' || **Fmt == 'S')
    {
        *Type = SCANF_TYPE_STRING;
        if (*Prefix != SCANF_PREFIX_SHORT && **Fmt == 'S')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == SCANF_PREFIX_LONG)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = 'l';
        }
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = 's';
        ++(*Fmt);
        Result = TRUE;
    }
    /* grab int types */
    else if (**Fmt == 'd' || **Fmt == 'i' || **Fmt == 'o' ||
             **Fmt == 'u' || **Fmt == 'x' || **Fmt == 'X' ||
             **Fmt == 'p')
    {
        *Type = SCANF_TYPE_INT;
        if (*Prefix == SCANF_PREFIX_SHORT)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = 'h';
        }
        else if (*Prefix == SCANF_PREFIX_LONG)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = 'l';
        }
        else if (*Prefix == SCANF_PREFIX_LONGLONG)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)

            if (strcpy_s(Out, iOutSize-(Out-BaseOut), scanf_longlongfmt) != SAFECRT_SUCCESS)
            {
                ERROR("strcpy_s failed\n");
                SetLastError(ERROR_INSUFFICIENT_BUFFER);
                return FALSE;
            }

            Out += strlen(scanf_longlongfmt);
        }
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = *(*Fmt)++;
        Result = TRUE;
    }
    else if (**Fmt == 'e' || **Fmt == 'E' || **Fmt == 'f' ||
             **Fmt == 'g' || **Fmt == 'G')
    {
        /* we can safely ignore the prefixes and only add the type*/
        *Type = SCANF_TYPE_FLOAT;
        /* this gets rid of %E/%G since they're they're the
           same when scanning */
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = tolower( *(*Fmt)++ );
        Result = TRUE;
    }
    else if (**Fmt == 'n')
    {
        if (*Prefix == SCANF_PREFIX_SHORT)
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = 'h';
        }
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = *(*Fmt)++;
        *Type = SCANF_TYPE_N;
        Result = TRUE;
    }
    else if (**Fmt == '[')
    {
        /* There is a small compatibility problem in the handling of the []
           option in FreeBSD vs. Windows.  In Windows, you can have [z-a]
           as well as [a-z].  In FreeBSD, [z-a] fails.  So, we need to
           reverse the instances of z-a to a-z (and [m-e] to [e-m], etc). */

        /* step 1 : copy the leading [ */
        CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
        *Out++ = '[';
        (*Fmt)++;

        /* step 2 : copy a leading ^, if present */
        if( '^' == **Fmt )
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = '^';
            (*Fmt)++;
        }

        /* step 3 : copy a leading ], if present; a ] immediately after the
           leading [ (or [^) does *not* end the sequence, it is part of the
           characters to match */
        if( ']' == **Fmt )
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = ']';
            (*Fmt)++;
        }

        /* step 4 : if the next character is already a '-', it's not part of an
           interval specifier, so just copy it */
        if('-' == **Fmt )
        {
            CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
            *Out++ = '-';
            (*Fmt)++;
        }

        /* ok then, process the rest of it */
        while( '\0' != **Fmt )
        {
            if(']' == **Fmt)
            {
                /* ']' marks end of the format specifier; we're done */
                CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
                *Out++ = ']';
                (*Fmt)++;
                break;
            }
            if('-' == **Fmt)
            {
                if( ']' == (*Fmt)[1] )
                {
                    /* got a '-', next character is the terminating ']';
                       copy '-' literally */
                    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
                    *Out++ = '-';
                    (*Fmt)++;
                }
                else
                {
                    /* got a '-' indicating an interval specifier */
                    unsigned char prev, next;

                    /* get the interval boundaries */
                    prev = (*Fmt)[-1];
                    next = (*Fmt)[1];

                    /* if boundaries were inverted, replace the already-copied
                       low boundary by the 'real' low boundary */
                    if( prev > next )
                    {
                        CHECK_OUT_IN_ITS_RANGE(Out-1,BaseOut,EndOut)
                        Out[-1] = next;

                        /* ...and save the 'real' upper boundary, which will be
                           copied to 'Out' below */
                        next = prev;
                    }

                    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
                    *Out++ = '-';
                    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
                    *Out++ = next;

                    /* skip over the '-' and the next character, which we
                       already copied */
                    (*Fmt)+=2;
                }
            }
            else
            {
                /* plain character; just copy it */
                CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
                *Out++ = **Fmt;
                (*Fmt)++;
            }
        }

        *Type = SCANF_TYPE_BRACKETS;
        Result = TRUE;
    }
    else if (**Fmt == ' ')
    {
        *Type = SCANF_TYPE_SPACE;
    }

    /* add %n so we know how far to increment the pointer */
    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
    *Out++ = '%';
    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
    *Out++ = 'n';

    CHECK_OUT_IN_ITS_RANGE(Out,BaseOut,EndOut)
    *Out = 0;  /* end the string */
    PAL_free(TempStr);
    return Result;
}

/*******************************************************************************
Function:
  Internal_ScanfExtractFormatW

  -- see Internal_ScanfExtractFormatA above
*******************************************************************************/
__attribute__((no_instrument_function))
static BOOL Internal_ScanfExtractFormatW(LPCWSTR *Fmt, LPSTR Out, int iOutSize, LPBOOL Store,
                                         int32_t * Width, int32_t * Prefix, int32_t * Type)
{
    BOOL Result = FALSE;
    LPSTR TempStr;
    LPSTR TempStrPtr;

    *Width = -1;
    *Store = TRUE;
    *Prefix = -1;
    *Type = -1;

    if (*Fmt && **Fmt == '%')
    {
        *Out++ = *(*Fmt)++;
    }
    else
    {
        return Result;
    }

    /* we'll never need a temp string longer than the original */
    TempStrPtr = TempStr = (LPSTR) PAL_malloc(PAL_wcslen(*Fmt)+1);
    if (!TempStr)
    {
        ERROR("PAL_malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return Result;
    }

    /* parse '*' flag which means don't store */
    if (**Fmt == '*')
    {
        *Store = FALSE;
        *Out++ = *(*Fmt)++;
    }

    /* grab width specifier */
    if (isdigit(**Fmt))
    {
        TempStrPtr = TempStr;
        while (isdigit(**Fmt))
        {
            *TempStrPtr++ = **Fmt;
            *Out++ = *(*Fmt)++;
        }
        *TempStrPtr = 0; /* end string */
        *Width = atoi(TempStr);
        if (*Width < 0)
        {
            ERROR("atoi returned a negative value indicative of an overflow.\n");
            SetLastError(ERROR_INTERNAL_ERROR);
            return Result;
        }
    }

    if (**Fmt == 'p')
    {
        *Prefix = SCANF_PREFIX_LONGLONG;
    }
    /* grab prefix of 'I64' for __int64 */
    if ((*Fmt)[0] == 'I' && (*Fmt)[1] == '6' && (*Fmt)[2] == '4')
    {
        /* convert to 'q'/'ll' so that Unix sscanf can handle it */
        *Fmt += 3;
        *Prefix = SCANF_PREFIX_LONGLONG;
    }
    /* grab a prefix of 'h' */
    else if (**Fmt == 'h')
    {
        *Prefix = SCANF_PREFIX_SHORT;
        ++(*Fmt);
    }
    /* grab prefix of 'l' or the undocumented 'w' (at least in MSDN) */
    else if (**Fmt == 'l' || **Fmt == 'w')
    {
        ++(*Fmt);
        // Only want to change the prefix on 64 bit when inputing characters.
        if (**Fmt == 'C' || **Fmt == 'S')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (**Fmt == 'l')
        {
            *Prefix = SCANF_PREFIX_LONGLONG;
            ++(*Fmt);
        }
    }
    else if (**Fmt == 'L')
    {
        /* a prefix of 'L' seems to be ignored */
        ++(*Fmt);
    }

    /* grab type 'c' */
    if (**Fmt == 'c' || **Fmt == 'C')
    {
        *Type = SCANF_TYPE_CHAR;
        if (*Prefix != SCANF_PREFIX_SHORT && **Fmt == 'c')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == SCANF_PREFIX_LONG)
        {
            *Out++ = 'l';
        }
        *Out++ = 'c';
        ++(*Fmt);
        Result = TRUE;
    }
    /* grab type 's' */
    else if (**Fmt == 's' || **Fmt == 'S')
    {
        *Type = SCANF_TYPE_STRING;
        if (*Prefix != SCANF_PREFIX_SHORT && **Fmt == 's')
        {
            *Prefix = SCANF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == SCANF_PREFIX_LONG)
        {
            *Out++ = 'l';
        }
        *Out++ = 's';
        ++(*Fmt);
        Result = TRUE;
    }
    /* grab int types */
    else if (**Fmt == 'd' || **Fmt == 'i' || **Fmt == 'o' ||
             **Fmt == 'u' || **Fmt == 'x' || **Fmt == 'X' ||
             **Fmt == 'p')
    {
        *Type = SCANF_TYPE_INT;
        if (*Prefix == SCANF_PREFIX_SHORT)
        {
            *Out++ = 'h';
        }
        else if (*Prefix == SCANF_PREFIX_LONG)
        {
            *Out++ = 'l';
        }
        else if (*Prefix == SCANF_PREFIX_LONGLONG)
        {
            if (strcpy_s(Out, iOutSize, scanf_longlongfmt) != SAFECRT_SUCCESS)
            {
                ERROR("strcpy_s failed\n");
                SetLastError(ERROR_INSUFFICIENT_BUFFER);
                return FALSE;
            }

            Out += strlen(scanf_longlongfmt);
        }
        *Out++ = *(*Fmt)++;
        Result = TRUE;
    }
    else if (**Fmt == 'e' || **Fmt == 'E' || **Fmt == 'f' ||
             **Fmt == 'g' || **Fmt == 'G')
    {
        /* we can safely ignore the prefixes and only add the type*/
        *Type = SCANF_TYPE_FLOAT;
        /* this gets rid of %E/%G since they're they're the
           same when scanning */
        *Out++ = tolower( *(*Fmt)++ );
        Result = TRUE;
    }
    else if (**Fmt == 'n')
    {
        if (*Prefix == SCANF_PREFIX_SHORT)
        {
            *Out++ = 'h';
        }
        *Out++ = *(*Fmt)++;
        *Type = SCANF_TYPE_N;
        Result = TRUE;
    }
    else if (**Fmt == '[')
    {
        /* There is a small compatibility problem in the handling of the []
           option in FreeBSD vs. Windows.  In Windows, you can have [z-a]
           as well as [a-z].  In FreeBSD, [z-a] fails.  So, we need to
           reverse the instances of z-a to a-z (and [m-e] to [e-m], etc). */

        /* step 1 : copy the leading [ */
        *Out++ = '[';
        (*Fmt)++;

        /* step 2 : copy a leading ^, if present */
        if( '^' == **Fmt )
        {
            *Out++ = '^';
            (*Fmt)++;
        }

        /* step 3 : copy a leading ], if present; a ] immediately after the
           leading [ (or [^) does *not* end the sequence, it is part of the
           characters to match */
        if( ']' == **Fmt )
        {
            *Out++ = ']';
            (*Fmt)++;
        }

        /* step 4 : if the next character is already a '-', it's not part of an
           interval specifier, so just copy it */
        if('-' == **Fmt )
        {
            *Out++ = '-';
            (*Fmt)++;
        }

        /* ok then, process the rest of it */
        while( '\0' != **Fmt )
        {
            if(']' == **Fmt)
            {
                /* ']' marks end of the format specifier; we're done */
                *Out++ = ']';
                (*Fmt)++;
                break;
            }
            if('-' == **Fmt)
            {
                if( ']' == (*Fmt)[1] )
                {
                    /* got a '-', next character is the terminating ']';
                       copy '-' literally */
                    *Out++ = '-';
                    (*Fmt)++;
                }
                else
                {
                    /* got a '-' indicating an interval specifier */
                    unsigned char prev, next;

                    /* get the interval boundaries */
                    prev = (*Fmt)[-1];
                    next = (*Fmt)[1];

                    /* if boundaries were inverted, replace the already-copied
                       low boundary by the 'real' low boundary */
                    if( prev > next )
                    {
                        Out[-1] = next;

                        /* ...and save the 'real' upper boundary, which will be
                           copied to 'Out' below */
                        next = prev;
                    }

                    *Out++ = '-';
                    *Out++ = next;

                    /* skip over the '-' and the next character, which we
                       already copied */
                    (*Fmt)+=2;
                }
            }
            else
            {
                /* plain character; just copy it */
                *Out++ = **Fmt;
                (*Fmt)++;
            }
        }

        *Type = SCANF_TYPE_BRACKETS;
        Result = TRUE;
    }
    else if (**Fmt == ' ')
    {
        *Type = SCANF_TYPE_SPACE;
    }

    /* add %n so we know how far to increment the pointer */
    *Out++ = '%';
    *Out++ = 'n';

    *Out = 0;  /* end the string */
    PAL_free(TempStr);
    return Result;
}

/*******************************************************************************
Function:
  PAL_vsscanf

Parameters:
  Buffer
    - buffer to parse values from
  Format
    - format string
  ap
    - stdarg parameter list
*******************************************************************************/
__attribute__((no_instrument_function))
int PAL_vsscanf(LPCSTR Buffer, LPCSTR Format, va_list ap)
{
    int32_t Length = 0;
    LPCSTR Buff = Buffer;
    LPCSTR Fmt = Format;
    char TempBuff[1024]; /* used to hold a single %<foo> format string */
    BOOL Store;
    int32_t Width;
    int32_t Prefix;
    int32_t Type = -1;

    while (*Fmt)
    {
        if (!*Buff && Length == 0)
        {
            Length = EOF;
            break;
        }
        /* remove any number of blanks */
        else if (isspace((unsigned char) *Fmt))
        {
            while (isspace((unsigned char) *Buff))
            {
                ++Buff;
            }
            ++Fmt;
        }
        else if (*Fmt == '%' &&
                 Internal_ScanfExtractFormatA(&Fmt, TempBuff, sizeof(TempBuff), &Store,
                                              &Width, &Prefix, &Type))
        {
            if (Prefix == SCANF_PREFIX_LONG &&
                (Type == SCANF_TYPE_STRING || Type == SCANF_TYPE_CHAR))
            {
                int len = 0;
                int res;
                WCHAR *charPtr = 0;

                /* a single character */
                if (Type == SCANF_TYPE_CHAR && Width == -1)
                {
                    len = Width = 1;
                }

                /* calculate length of string to copy */
                while (Buff[len] && !isspace((unsigned char) Buff[len]))
                {
                    if (Width != -1 && len >= Width)
                    {
                        break;
                    }
                    ++len;
                }

                if (Store)
                {
                    charPtr = va_arg(ap, WCHAR *);

                    res = MultiByteToWideChar(CP_ACP, 0, Buff, len,
                                            charPtr, len);
                    if (!res)
                    {
                        ASSERT("MultiByteToWideChar failed.  Error is %d\n",
                            GetLastError());
                        return -1;
                    }
                    if (Type == SCANF_TYPE_STRING)
                    {
                        /* end string */
                        charPtr[res] = 0;
                    }
                    ++Length;
                }
                Buff += len;
            }
            /* this places the number of bytes stored into the next arg */
            else if (Type == SCANF_TYPE_N)
            {
                if (Prefix == SCANF_PREFIX_SHORT)
                {
                    *(va_arg(ap, short *)) = Buff - Buffer;
                }
                else
                {
                    *(va_arg(ap, int32_t *)) = Buff - Buffer;
                }
            }
            /* types that sscanf can handle */
            else
            {
                int ret;
                int n;
                void * voidPtr = NULL;

                if (Store)
                {
                    // sscanf_s requires that if we are trying to read "%s" or "%c" or "%[", then
                    // the size of the buffer must follow the buffer we are trying to read into.
                    voidPtr = va_arg(ap, void *);
                    unsigned typeLen = 0;
                    if ((Type == SCANF_TYPE_STRING) || (Type == SCANF_TYPE_BRACKETS))
                    {
                        // Since this is not a Safe CRT API we don't really know the size of the destination
                        // buffer provided by the caller. So we have to assume that the caller has allocated
                        // enough space to hold either the width specified in the format or the entire input
                        // string plus '\0'.
                        typeLen = ((Width > 0) ? Width : strlen(Buffer)) + 1;
                    }
                    else if (Type == SCANF_TYPE_CHAR)
                    {
                        // Check whether the format string contains number of characters
                        // that should be read from the input string.
                        // Note: '\0' does not get appended in the "%c" case.
                        typeLen = (Width > 0) ? Width : 1;
                    }

                    if (typeLen > 0)
                    {
                        ret = sscanf_s(Buff, TempBuff, voidPtr, typeLen, &n);
                    }
                    else
                    {
                        ret = sscanf_s(Buff, TempBuff, voidPtr, &n);
                    }
                }
                else
                {
                    ret = sscanf_s(Buff, TempBuff, &n);
                }

#if SSCANF_CANNOT_HANDLE_MISSING_EXPONENT
                if ((ret == 0) && (Type == SCANF_TYPE_FLOAT))
                {
                    ret = SscanfFloatCheckExponent(Buff, TempBuff, voidPtr, &n);
                }
#endif // SSCANF_CANNOT_HANDLE_MISSING_EXPONENT

                if (ret > 0)
                {
                    Length += ret;
                }
                else
                {
                    /* no match, break scan */
                    break;
                }
                Buff += n;
            }
        }
        else
        {
            /* grab, but not store */
            if (*Fmt == *Buff && Type != SCANF_TYPE_SPACE)
            {
                ++Fmt;
                ++Buff;
            }
            /* doesn't match, break scan */
            else
            {
                break;
            }
        }
    }

    return Length;
}

/*******************************************************************************
Function:
  PAL_wvsscanf

  -- see PAL_vsscanf above
*******************************************************************************/
__attribute__((no_instrument_function))
int PAL_wvsscanf(LPCWSTR Buffer, LPCWSTR Format, va_list ap)
{
    int32_t Length = 0;
    LPCWSTR Buff = Buffer;
    LPCWSTR Fmt = Format;
    char TempBuff[1024]; /* used to hold a single %<foo> format string */
    BOOL Store;
    int32_t Width;
    int32_t Prefix;
    int32_t Type = -1;

    while (*Fmt)
    {
        if (!*Buff && Length == 0)
        {
            Length = EOF;
            break;
        }
        /* remove any number of blanks */
        else if (isspace(*Fmt))
        {
            while (isspace(*Buff))
            {
                ++Buff;
            }
            ++Fmt;
        }
        else if (*Fmt == '%' &&
                 Internal_ScanfExtractFormatW(&Fmt, TempBuff, sizeof(TempBuff), &Store,
                                              &Width, &Prefix, &Type))
        {
            if (Prefix == SCANF_PREFIX_LONG &&
                (Type == SCANF_TYPE_STRING || Type == SCANF_TYPE_CHAR))
            {
                int len = 0;
                WCHAR *charPtr = 0;

                /* a single character */
                if (Type == SCANF_TYPE_CHAR && Width == -1)
                {
                    len = Width = 1;
                }

                /* calculate length of string to copy */
                while (Buff[len] && !isspace(Buff[len]))
                {
                    if (Width != -1 && len >= Width)
                    {
                        break;
                    }
                    ++len;
                }

                if (Store)
                {
                    int i;
                    charPtr = va_arg(ap, WCHAR *);

                    for (i = 0; i < len; i++)
                    {
                        charPtr[i] = Buff[i];
                    }
                    if (Type == SCANF_TYPE_STRING)
                    {
                        /* end string */
                        charPtr[len] = 0;
                    }
                    ++Length;
                }
                Buff += len;
            }
            /* this places the number of bytes stored into the next arg */
            else if (Type == SCANF_TYPE_N)
            {
                if (Prefix == SCANF_PREFIX_SHORT)
                {
                    *(va_arg(ap, short *)) = Buff - Buffer;
                }
                else
                {
                    *(va_arg(ap, int32_t *)) = Buff - Buffer;
                }
            }
            /* types that sscanf can handle */
            else
            {
                int ret;
                int n;
                int size;
                LPSTR newBuff = 0;
                void * voidPtr = NULL;

                size = WideCharToMultiByte(CP_ACP, 0, Buff, -1, 0, 0, 0, 0);
                if (!size)
                {
                    ASSERT("WideCharToMultiByte failed.  Error is %d\n",
                        GetLastError());
                    return -1;
                }
                newBuff = (LPSTR) PAL_malloc(size);
                if (!newBuff)
                {
                    ERROR("PAL_malloc failed\n");
                    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                    return -1;
                }
                size = WideCharToMultiByte(CP_ACP, 0, Buff, size,
                                           newBuff, size, 0, 0);
                if (!size)
                {
                    ASSERT("WideCharToMultiByte failed.  Error is %d\n",
                        GetLastError());
                    PAL_free(newBuff);
                    return -1;
                }

                if (Store)
                {
                    if (Type == SCANF_TYPE_BRACKETS)
                    {
                        WCHAR *strPtr;
                        int i;

                        /* add a '*' to %[] --> %*[]  */
                        i = strlen(TempBuff) + 1;
                        while (i)
                        {
                            /* shift everything right one */
                            TempBuff[i] = TempBuff[i - 1];
                            --i;
                        }
                        TempBuff[0] = '%';
                        TempBuff[1] = '*';

                        /* %n doesn't count as a conversion. Since we're
                           suppressing conversion of the %[], sscanf will
                           always return 0, so we can't use the return value
                           to determine success. Set n to 0 before the call; if
                           it's still 0 afterwards, we know the call failed */
                        n = 0;
                        sscanf_s(newBuff, TempBuff, &n);
                        if(0 == n)
                        {
                            /* sscanf failed, nothing matched. set ret to 0,
                               so we know we have to break */
                            ret = 0;
                        }
                        else
                        {
                            strPtr = va_arg(ap, WCHAR *);
                            for (i = 0; i < n; i++)
                            {
                                strPtr[i] = Buff[i];
                            }
                            strPtr[n] = 0; /* end string */
                            ret = 1;
                        }
                    }
                    else
                    {
                        voidPtr = va_arg(ap, void *);
                        // sscanf_s requires that if we are trying to read "%s" or "%c", then
                        // the size of the buffer must follow the buffer we are trying to read into.
                        unsigned typeLen = 0;
                        if (Type == SCANF_TYPE_STRING)
                        {
                            // We don't really know the size of the destination buffer provided by the
                            // caller. So we have to assume that the caller has allocated enough space
                            // to hold either the width specified in the format or the entire input
                            // string plus '\0'.
                            typeLen = ((Width > 0) ? Width : PAL_wcslen(Buffer)) + 1;
                        }
                        else if (Type == SCANF_TYPE_CHAR)
                        {
                            // Check whether the format string contains number of characters
                            // that should be read from the input string.
                            // Note: '\0' does not get appended in the "%c" case.
                            typeLen = (Width > 0) ? Width : 1;
                        }

                        if (typeLen > 0)
                        {
                            ret = sscanf_s(newBuff, TempBuff, voidPtr, typeLen, &n);
                        }
                        else
                            ret = sscanf_s(newBuff, TempBuff, voidPtr, &n);
                    }
                }
                else
                {
                    ret = sscanf_s(newBuff, TempBuff, &n);
                }

#if SSCANF_CANNOT_HANDLE_MISSING_EXPONENT
                if ((ret == 0) && (Type == SCANF_TYPE_FLOAT))
                {
                    ret = SscanfFloatCheckExponent(newBuff, TempBuff, voidPtr, &n);
                }
#endif // SSCANF_CANNOT_HANDLE_MISSING_EXPONENT

                PAL_free(newBuff);
                if (ret > 0)
                {
                    Length += ret;
                }
                else
                {
                    /* no match; break scan */
                    break;
                }
                Buff += n;
            }
       }
        else
        {
            /* grab, but not store */
            if (*Fmt == *Buff && Type != SCANF_TYPE_SPACE)
            {
                ++Fmt;
                ++Buff;
            }
            /* doesn't match, break scan */
            else
            {
                break;
            }
        }
    }

    return Length;
}

/*++
Function:
  PAL_sscanf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_sscanf(
           const char *buffer,
           const char *format,
           ...)
{
    int Length;
    va_list ap;

    PERF_ENTRY(sscanf);
    ENTRY("PAL_sscanf (buffer=%p (%s), format=%p (%s))\n", buffer, buffer, format, format);

    va_start(ap, format);
    Length = PAL_vsscanf(buffer, format, ap);
    va_end(ap);

    LOGEXIT("PAL_sscanf returns int %d\n", Length);
    PERF_EXIT(sscanf);
    return Length;
}

/*++
Function:
  PAL_swprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_swprintf(
          char16_t *buffer,
          const char16_t *format,
          ...)
{
    int32_t Length;
    va_list ap;

    PERF_ENTRY(swprintf);
    ENTRY("PAL_swprintf (buffer=%p, format=%p (%S))\n", buffer, format, format);

    va_start(ap, format);
    Length = PAL__wvsnprintf(buffer, 0x7fffffff, format, ap);
    va_end(ap);

    LOGEXIT("PAL_swprintf returns int %d\n", Length);
    PERF_EXIT(swprintf);
    return Length;
}

/*++
Function:
  PAL_swscanf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_swscanf(
          const char16_t *buffer,
          const char16_t *format,
          ...)
{
    int Length;
    va_list ap;

    PERF_ENTRY(swscanf);
    ENTRY("PAL_swscanf (buffer=%p (%S), format=%p (%S))\n", buffer, buffer, format, format);

    va_start(ap, format);
    Length = PAL_wvsscanf(buffer, format, ap);
    va_end(ap);

    LOGEXIT("PAL_swscanf returns int %d\n", Length);
    PERF_EXIT(swscanf);
    return Length;
}


/*++
Function:
  PAL_vsprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_vsprintf(char *buffer,
         const char *format,
         va_list argptr)
{
    int32_t Length;

    PERF_ENTRY(vsprintf);
    ENTRY("PAL_vsprintf (buffer=%p, format=%p (%s), argptr=%p)\n",
          buffer, format, format, argptr);

    Length = PAL__vsnprintf(buffer, 0x7fffffff, format, argptr);

    LOGEXIT("PAL_vsprintf returns int %d\n", Length);
    PERF_EXIT(vsprintf);

    return Length;
}


/*++
Function:
  _vsnprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
_vsnprintf(char *buffer,
           size_t count,
           const char *format,
           va_list argptr)
{
    int32_t Length;

    PERF_ENTRY(_vsnprintf);
    ENTRY("_vsnprintf (buffer=%p, count=%d, format=%p (%s), argptr=%p)\n",
          buffer, count, format, format, argptr);

    Length = PAL__vsnprintf(buffer, count, format, argptr);

    LOGEXIT("_vsnprintf returns int %d\n", Length);
    PERF_EXIT(_vsnprintf);

    return Length;
}



/*++
Function:
  PAL_vswprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_vswprintf(char16_t *buffer,
              const char16_t *format,
              va_list argptr)
{
    int32_t Length;

    PERF_ENTRY(vswprintf);
    ENTRY("PAL_vswprintf (buffer=%p, format=%p (%S), argptr=%p)\n",
          buffer, format, format, argptr);

    Length = PAL__wvsnprintf(buffer, 0x7fffffff, format, argptr);

    LOGEXIT("PAL_vswprintf returns int %d\n", Length);
    PERF_EXIT(vswprintf);

    return Length;
}


/*++
Function:
  _vsnwprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
_vsnwprintf(char16_t *buffer,
            size_t count,
            const char16_t *format,
            va_list argptr)
{
    int32_t Length;

    PERF_ENTRY(_vsnwprintf);
    ENTRY("_vsnwprintf (buffer=%p, count=%lu, format=%p (%S), argptr=%p)\n",
          buffer, (unsigned long) count, format, format, argptr);

    Length = PAL__wvsnprintf(buffer, count, format, argptr);

    LOGEXIT("_vsnwprintf returns int %d\n", Length);
    PERF_EXIT(_vsnwprintf);

    return Length;
}

#if SSCANF_CANNOT_HANDLE_MISSING_EXPONENT
/*++
Function:
  SscanfFloatCheckExponent

  Parameters:
  buff:     pointer to the buffer to be parsed; the target float must be at
            the beginning of the buffer, except for any number of leading
            spaces
  floatFmt: must be "%e%n" (or "%f%n" or "%g%n")
  voidptr:  optional pointer to output variable (which should be a float)
  pn:       pointer to an int to receive the number of bytes parsed.

  Notes:
  On some platforms (specifically AIX) sscanf fails to parse a float from
  a string such as 12.34e (while it succeeds for e.g. 12.34a). Sscanf
  initially interprets the 'e' as the keyword for the beginning of a
  10-exponent of a floating point in scientific notation (as in 12.34e5),
  but then it fails to parse the actual exponent. At this point sscanf should
  be able to fall back on the narrower pattern, and parse the floating point
  in common decimal notation (i.e. 12.34). However AIX's sscanf fails to do
  so and it does not parse any number.
  This function checks the given string for a such case and removes
  the 'e' before parsing the float.

--*/

__attribute__((no_instrument_function))
static int SscanfFloatCheckExponent(LPCSTR buff, LPCSTR floatFmt,
                                      void * voidPtr, int * pn)
{
    int ret = 0;
    int digits = 0;
    int points = 0;
    LPCSTR pos = buff;

    /* skip initial spaces */
    while (*pos && isspace(*pos))
        pos++;

    /* go to the end of a float, if there is one */
    while (*pos)
    {
        if (isdigit(*pos))
            digits++;
        else if (*pos == '.')
        {
            if (++points > 1)
                break;
        }
        else
            break;

        pos++;
    }

    /* check if it is something like 12.34e and the trailing 'e' is not
       the suffix of a valid exponent of 10, such as 12.34e+5 */
    if ( digits > 0 && *pos && tolower(*pos) == 'e' &&
         !( *(pos+1) &&
            ( isdigit(*(pos+1)) ||
              ( (*(pos+1) == '+' || *(pos+1) == '-') && isdigit(*(pos+2)) )
                )
             )
        )
    {
        char * pLocBuf = (char *)PAL_malloc((pos-buff+1)*sizeof(char));
        if (pLocBuf)
        {
            memcpy(pLocBuf, buff, (pos-buff)*sizeof(char));
            pLocBuf[pos-buff] = 0;
            if (voidPtr)
                ret = sscanf_s(pLocBuf, floatFmt, voidPtr, pn);
            else
                ret = sscanf_s(pLocBuf, floatFmt, pn);
            PAL_free (pLocBuf);
        }
    }
    return ret;
}
#endif // SSCANF_CANNOT_HANDLE_MISSING_EXPONENT
