//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    silent_printf.c

Abstract:

    Implementation of a silent version of PAL_vsprintf and PAL_vfprintf function.
    (without any reference to TRACE/ERROR/... macros, needed by the tracing macros) 

Revision History:



--*/


#include "pal/palinternal.h"
#include "pal/cruntime.h"
#include "pal/locale.h"
#include "pal/printfcpp.hpp"

/* clip strings (%s, %S) at this number of characters */
#define MAX_STR_LEN 300

static int Silent_WideCharToMultiByte(LPCWSTR lpWideCharStr, int cchWideChar, 
                                      LPSTR lpMultiByteStr, int cbMultiByte);
static BOOL Silent_ExtractFormatA(LPCSTR *Fmt, LPSTR Out, int32_t * Flags, int32_t * Width,
                                  int32_t * Precision, int32_t * Prefix, int32_t * Type);
static int32_t Silent_AddPaddingVfprintf(PAL_FILE *stream, LPSTR In, int32_t Padding,
                                     int32_t Flags);

static size_t Silent_PAL_wcslen(const char16_t *string);

/*******************************************************************************
Function:
  PAL_vsnprintf (silent version)
  for more details, see PAL_vsnprintf in printf.c
*******************************************************************************/
int32_t Silent_PAL_vsnprintf(LPSTR Buffer, int32_t Count, LPCSTR Format, va_list aparg)
{
    BOOL BufferRanOut = FALSE;
    char TempBuff[1024]; /* used to hold a single %<foo> format string */
    LPSTR BufferPtr = Buffer;
    LPCSTR Fmt = Format;
    LPWSTR TempWStr;
    char TempStr[MAX_STR_LEN+1];
    WCHAR TempWChar;
    int32_t Flags;
    int32_t Width;
    int32_t Precision;
    int32_t Prefix;
    int32_t Type;
    int32_t Length;
    int32_t TempInt;
    int wctombResult;
    va_list ap;
    
    va_copy(ap, aparg);

    while (*Fmt)
    {
        if ((BufferPtr - Buffer) >= Count)
        {
            BufferRanOut = TRUE;
            break;
        }
        else if(*Fmt == '%' &&
                TRUE == Silent_ExtractFormatA(&Fmt, TempBuff, &Flags,
                                                &Width, &Precision,
                                                &Prefix, &Type))
        {
            if (Prefix == PFF_PREFIX_LONG && Type == PFF_TYPE_STRING)
            {
                if (WIDTH_STAR == Width)
                {
                    Width = va_arg(ap, int32_t);
                }
                else if (WIDTH_INVALID == Width)
                {
                    /* both a '*' and a number, ignore, but remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                if (PRECISION_STAR == Precision)
                {
                    Precision = va_arg(ap, int32_t);
                }
                else if (PRECISION_INVALID == Precision)
                {
                    /* both a '*' and a number, ignore, but remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                TempWStr = va_arg(ap, LPWSTR);
                Length = Silent_WideCharToMultiByte(TempWStr, -1, 0, 0);
                if (!Length)
                {
                    va_end(ap);
                    return -1;
                }

                /* clip string output to MAX_STR_LEN characters */
                if (PRECISION_DOT == Precision)
                {
                    Precision = MAX_STR_LEN;
                }

                if (PRECISION_DOT == Precision)
                {
                    /* copy nothing */
                    *TempStr = 0;
                    Length = 0;
                }
                else if (Precision > 0 && Precision < Length - 1)
                {
                    Length = Silent_WideCharToMultiByte(TempWStr, Precision, 
                                                        TempStr, Length);
                    if (!Length)
                    {
                        va_end(ap);
                        return -1;
                    }
                    TempStr[Length] = 0;
                    Length = Precision;
                }
                /* copy everything */
                else
                {
                    wctombResult = Silent_WideCharToMultiByte(TempWStr, -1,
                                                              TempStr, Length);
                    if (!wctombResult)
                    {
                        PAL_free(TempStr);
                        va_end(ap);
                        return -1;
                    }
                    --Length; /* exclude null char */
                }

                /* do the padding (if needed)*/
                BufferRanOut = !Internal_AddPaddingA(&BufferPtr,
                                                   Count - (BufferPtr - Buffer),
                                                   TempStr,
                                                   Width - Length,
                                                   Flags);
            }
            else if (Prefix == PFF_PREFIX_LONG && Type == PFF_TYPE_CHAR)
            {
                char TempBuffer[4];

                if (WIDTH_STAR == Width ||
                    WIDTH_INVALID == Width)
                {
                    /* ignore (because it's a char), and remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }
                if (PRECISION_STAR == Precision ||
                    PRECISION_INVALID == Precision)
                {
                    /* ignore (because it's a char), and remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                TempWChar = va_arg(ap, int);
                Length = Silent_WideCharToMultiByte(&TempWChar, 1, TempBuffer, 4);
                if (!Length)
                {
                    va_end(ap);
                    return -1;
                }
                TempBuffer[Length] = 0;

                /* do the padding (if needed)*/
                BufferRanOut = !Internal_AddPaddingA(&BufferPtr,
                                                   Count - (BufferPtr - Buffer),
                                                   TempBuffer,
                                                   Width - Length,
                                                   Flags);

            }
            /* this places the number of bytes written to the buffer in the
               next arg */
            else if (Type == PFF_TYPE_N)
            {
                if (WIDTH_STAR == Width)
                {
                    Width = va_arg(ap, int32_t);
                }
                if (PRECISION_STAR == Precision)
                {
                    Precision = va_arg(ap, int32_t);
                }
                if (Prefix == PFF_PREFIX_SHORT)
                {
                    *(va_arg(ap, short *)) = BufferPtr - Buffer;
                }
                else
                {
                    *(va_arg(ap, int32_t *)) = BufferPtr - Buffer;
                }
            }
            else if (Type == PFF_TYPE_CHAR && (Flags & PFF_ZERO) != 0)
            {
                // Some versions of sprintf don't support 0-padded chars,
                // so we handle them here.
                char ch[2];

                ch[0] = (char) va_arg(ap, int);
                ch[1] = '\0';
                Length = 1;
                BufferRanOut = !Internal_AddPaddingA(&BufferPtr,
                                           Count - (BufferPtr - Buffer),
                                           ch,
                                           Width - Length,
                                           Flags);
            }
            else if (Type == PFF_TYPE_STRING && (Flags & PFF_ZERO) != 0)
            {
                // Some versions of sprintf don't support 0-padded strings,
                // so we handle them here.
                char *tempStr;

                tempStr = va_arg(ap, char *);
                Length = strlen(tempStr);
                BufferRanOut = !Internal_AddPaddingA(&BufferPtr,
                                           Count - (BufferPtr - Buffer),
                                           tempStr,
                                           Width - Length,
                                           Flags);
            }
            /* types that sprintf can handle */
            else
            {
                size_t TempCount = Count - (BufferPtr - Buffer);

                TempInt = 0;
                /* %h (short) doesn't seem to be handled properly by local sprintf,
                   so lets do the truncation ourselves.  (ptr -> int -> short to avoid
                   warnings */
                if (Type == PFF_TYPE_P && Prefix == PFF_PREFIX_SHORT)
                {
                    long trunc1;
                    short trunc2;

                    trunc1 = va_arg(ap, int32_t);
                    trunc2 = (short)trunc1;

                    TempInt = snprintf(BufferPtr, TempCount, TempBuff, trunc2);
                }
                else if (Type == PFF_TYPE_INT && Prefix == PFF_PREFIX_SHORT)
                {
                    // Convert explicitly from int to short to get
                    // correct sign extension for shorts on all systems.
                    int n;
                    short s;

                    n = va_arg(ap, int);
                    s = (short) n;

                    TempInt = snprintf(BufferPtr, TempCount, TempBuff, s);
                }
                else
                {
                    /* limit string output (%s) to 300 characters */
                    if(TempBuff[0] == '%' && TempBuff[1] == 's')
                    {
                        if (strcpy_s(TempBuff, sizeof(TempBuff), "%.300s") != SAFECRT_SUCCESS)
                        {
                            va_end(ap);
                            return -1;
                        }
                    }                                     
                    va_list apcopy;
                    va_copy(apcopy, ap);
                    TempInt = PAL__vsnprintf(BufferPtr, TempCount, TempBuff, apcopy);
                    va_end(apcopy);
                    PAL_printf_arg_remover(&ap, Width, Precision, Type, Prefix);
                }

                if (TempInt < 0 || static_cast<size_t>(TempInt) >= TempCount) /* buffer not long enough */
                {
                    BufferPtr += TempCount;
                    BufferRanOut = TRUE;
                }
                else
                {
                    BufferPtr += TempInt;
                }
            }
        }
        else
        {
            *BufferPtr++ = *Fmt++; /* copy regular chars into buffer */
        }
    }

    if (Count > (BufferPtr - Buffer))
    {
        *BufferPtr = 0; /* end the string */
    }

    va_end(ap);

    if (BufferRanOut)
    {
        return -1;
    }
    else
    {
        return BufferPtr - Buffer;
    }
}

/*++
Function:
  PAL_vfprintf (silent version)

  for more details, see PAL_vfprintf in printf.c
--*/
int Silent_PAL_vfprintf(PAL_FILE *stream, const char *format, va_list aparg)
{
    char TempBuff[1024]; /* used to hold a single %<foo> format string */
    LPCSTR Fmt = format;
    LPWSTR TempWStr;
    LPSTR TempStr;
    WCHAR TempWChar;
    int32_t Flags;
    int32_t Width;
    int32_t Precision;
    int32_t Prefix;
    int32_t Type;
    int32_t Length;
    int32_t TempInt;
    int wctombResult;
    int written = 0;
    int paddingReturnValue;
    va_list ap;
    
    va_copy(ap, aparg);

    while (*Fmt)
    {
        if (*Fmt == '%' &&
            TRUE == Silent_ExtractFormatA(&Fmt, TempBuff, &Flags, &Width, 
                                          &Precision, &Prefix, &Type))
        {
            if (Prefix == PFF_PREFIX_LONG && Type == PFF_TYPE_STRING)
            {
                if (WIDTH_STAR == Width)
                {
                    Width = va_arg(ap, int32_t);
                }
                else if (WIDTH_INVALID == Width)
                {
                    /* both a '*' and a number, ignore, but remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                if (PRECISION_STAR == Precision)
                {
                    Precision = va_arg(ap, int32_t);
                }
                else if (PRECISION_INVALID == Precision)
                {
                    /* both a '*' and a number, ignore, but remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                TempWStr = va_arg(ap, LPWSTR);
                Length = Silent_WideCharToMultiByte(TempWStr, -1, 0, 0);
                if (!Length)
                {
                    va_end(ap);
                    return -1;
                }
                TempStr = (LPSTR) PAL_malloc(Length);
                if (!TempStr)
                {
                    va_end(ap);
                    return -1;
                }
                if (PRECISION_DOT == Precision)
                {
                    /* copy nothing */
                    *TempStr = 0;
                    Length = 0;
                }
                else if (Precision > 0 && Precision < Length - 1)
                {
                    Length = Silent_WideCharToMultiByte(TempWStr, Precision, 
                                                        TempStr, Length);
                    if (!Length)
                    {
                        PAL_free(TempStr);
                        va_end(ap);
                        return -1;
                    }
                    TempStr[Length] = 0;
                    Length = Precision;
                }
                /* copy everything */
                else
                {
                    wctombResult = Silent_WideCharToMultiByte(TempWStr, -1, 
                                                              TempStr, Length);
                    if (!wctombResult)
                    {
                        PAL_free(TempStr);
                        va_end(ap);
                        return -1;
                    }
                    --Length; /* exclude null char */
                }

                /* do the padding (if needed)*/
                paddingReturnValue =
                  Silent_AddPaddingVfprintf(stream, TempStr, Width - Length, Flags);
                if (-1 == paddingReturnValue)
                {
                    PAL_free(TempStr);
                    va_end(ap);
                    return -1;
                }
                written += paddingReturnValue;

                PAL_free(TempStr);
            }
            else if (Prefix == PFF_PREFIX_LONG && Type == PFF_TYPE_CHAR)
            {
                char TempBuffer[4];
                if (WIDTH_STAR == Width ||
                    WIDTH_INVALID == Width)
                {
                    /* ignore (because it's a char), and remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }
                if (PRECISION_STAR == Precision ||
                    PRECISION_INVALID == Precision)
                {
                    /* ignore (because it's a char), and remove arg */
                    TempInt = va_arg(ap, int32_t); /* value not used */
                }

                TempWChar = va_arg(ap, int);
                Length = Silent_WideCharToMultiByte(&TempWChar, 1, TempBuffer, 4);
                if (!Length)
                {
                    va_end(ap);
                    return -1;
                }
                TempBuffer[Length] = 0;

                /* do the padding (if needed)*/
                paddingReturnValue =
                  Silent_AddPaddingVfprintf(stream, TempBuffer,
                                              Width - Length, Flags);
                if (-1 == paddingReturnValue)
                {
                    va_end(ap);
                    return -1;
                }
                written += paddingReturnValue;

            }
            /* this places the number of bytes written to the buffer in the
               next arg */
            else if (Type == PFF_TYPE_N)
            {
                if (WIDTH_STAR == Width)
                {
                    Width = va_arg(ap, int32_t);
                }
                if (PRECISION_STAR == Precision)
                {
                    Precision = va_arg(ap, int32_t);
                }

                if (Prefix == PFF_PREFIX_SHORT)
                {
                    *(va_arg(ap, short *)) = written;
                }
                else
                {
                    *(va_arg(ap, int32_t *)) = written;
                }
            }
            /* types that sprintf can handle */
            else
            {
                TempInt = 0;

                /* %h (short) doesn't seem to be handled properly by local sprintf,
                   so lets do the truncation ourselves.  (ptr -> int -> short to avoid
                   warnings */
                if (Type == PFF_TYPE_P && Prefix == PFF_PREFIX_SHORT)
                {
                    long trunc1;
                    short trunc2;

                    trunc1 = va_arg(ap, int32_t);
                    trunc2 = (short)trunc1;

                    TempInt = fprintf((FILE*)stream, TempBuff, trunc2);
                }
                else if (Type == PFF_TYPE_INT && Prefix == PFF_PREFIX_SHORT)
                {
                    // Convert explicitly from int to short to get
                    // correct sign extension for shorts on all systems.
                    int n;
                    short s;

                    n = va_arg(ap, int);
                    s = (short)n;

                    TempInt = fprintf((FILE*)stream, TempBuff, s);
                }
                else
                {
                    va_list apcopy;
                    va_copy(apcopy, ap);
                    TempInt = PAL_vfprintf(stream, TempBuff, apcopy);
                    va_end(apcopy);
                    PAL_printf_arg_remover(&ap, Width, Precision, Type, Prefix);
                }

                if (-1 != TempInt)
                {
                    written += TempInt;
                }
            }
        }
        else
        {
#if FILE_OPS_CHECK_FERROR_OF_PREVIOUS_CALL
            clearerr((FILE*)stream);
#endif

            PAL_fwrite(Fmt++, 1, 1, stream); /* copy regular chars into buffer */
            if (stream->PALferrorCode == PAL_FILE_ERROR)
            {
                va_end(ap);
                return -1;
            }
            ++written;
        }
    }

    va_end(ap);
    return written;
}

/*++
Function:
  WideCharToMultiByte (reduced and silent version)

See MSDN doc.
--*/
int Silent_WideCharToMultiByte(LPCWSTR lpWideCharStr, int cchWideChar,
                               LPSTR lpMultiByteStr, int cbMultiByte)
{
    int32_t retval =0;

    if ((lpWideCharStr == NULL)||
        (lpWideCharStr == (LPCWSTR) lpMultiByteStr))
    {
        goto EXIT;
    }

    if (cchWideChar == -1)
    {
        cchWideChar = Silent_PAL_wcslen(lpWideCharStr) + 1;
    }

    if (cbMultiByte == 0)
    {
        retval = cchWideChar;
        goto EXIT;
    }
    else if(cbMultiByte < cchWideChar)
    {
        retval = 0;
        goto EXIT;
    }
    
    retval = cchWideChar;
    while(cchWideChar > 0)
    {
        if(*lpWideCharStr > 255)
        {
            *lpMultiByteStr = '?';
        }
        else
        {
            *lpMultiByteStr = (unsigned char)*lpWideCharStr;
        }
        lpMultiByteStr++;
        lpWideCharStr++;
        cchWideChar--;
    }

EXIT:
    return retval;
}

/*******************************************************************************
Function:
  Internal_ExtractFormatA (silent version)
  
  see Internal_ExtractFormatA function in printf.c
*******************************************************************************/
BOOL Silent_ExtractFormatA(LPCSTR *Fmt, LPSTR Out, int32_t * Flags, int32_t * Width, int32_t * Precision, int32_t * Prefix, int32_t * Type)
{
    BOOL Result = FALSE;
    LPSTR TempStr;
    LPSTR TempStrPtr;

    *Width = WIDTH_DEFAULT;
    *Precision = PRECISION_DEFAULT;
    *Flags = PFF_NONE;
    *Prefix = PFF_PREFIX_DEFAULT;
    *Type = PFF_TYPE_DEFAULT;

    if (*Fmt && **Fmt == '%')
    {
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
        return Result;
    }

    /* parse flags */
    while (**Fmt && (**Fmt == '-' || **Fmt == '+' ||
           **Fmt == '0' || **Fmt == ' ' || **Fmt == '#'))
    {
        switch (**Fmt)
        {
        case '-':
            *Flags |= PFF_MINUS; break;
        case '+':
            *Flags |= PFF_PLUS; break;
        case '0':
            *Flags |= PFF_ZERO; break;
        case ' ':
            *Flags |= PFF_SPACE; break;
        case '#':
            *Flags |= PFF_POUND; break;
        }
            *Out++ = *(*Fmt)++;
    }
    /* '-' flag negates '0' flag */
    if ((*Flags & PFF_MINUS) && (*Flags & PFF_ZERO))
    {
        *Flags -= PFF_ZERO;
    }

    /* grab width specifier */
    if (isdigit((unsigned char) **Fmt))
    {
        TempStrPtr = TempStr;
        while (isdigit((unsigned char) **Fmt))
        {
            *TempStrPtr++ = **Fmt;
            *Out++ = *(*Fmt)++;
        }
        *TempStrPtr = 0; /* end string */
        *Width = atoi(TempStr);
        if (*Width < 0)
        {
            SetLastError(ERROR_INTERNAL_ERROR);
            return Result;
        }
    }
    else if (**Fmt == '*')
    {
        *Width = WIDTH_STAR;
        *Out++ = *(*Fmt)++;
        if (isdigit((unsigned char) **Fmt))
        {
            /* this is an invalid width because we have a * then a number */
            /* printf handles this by just printing the whole string */
            *Width = WIDTH_INVALID;
            while (isdigit((unsigned char) **Fmt))
            {
               *Out++ = *(*Fmt)++;
            }
        }
    }


    /* grab precision specifier */
    if (**Fmt == '.')
    {
        *Out++ = *(*Fmt)++;
        if (isdigit((unsigned char) **Fmt))
        {
            TempStrPtr = TempStr;
            while (isdigit((unsigned char) **Fmt))
            {
                *TempStrPtr++ = **Fmt;
                *Out++ = *(*Fmt)++;
            }
            *TempStrPtr = 0; /* end string */
            *Precision = atoi(TempStr);
            if (*Precision < 0)
            {
                SetLastError(ERROR_INTERNAL_ERROR);
                return Result;
            }
        }
        else if (**Fmt == '*')
        {
            *Precision = PRECISION_STAR;
            *Out++ = *(*Fmt)++;
            if (isdigit((unsigned char) **Fmt))
            {
                /* this is an invalid precision because we have a .* then a
                   number */
                /* printf handles this by just printing the whole string */
                *Precision = PRECISION_INVALID;
                while (isdigit((unsigned char) **Fmt))
                {
                    *Out++ = *(*Fmt)++;
                }
            }
        }
        else
        {
            *Precision = PRECISION_DOT;
        }
    }

    if (**Fmt == 'p')
    {
        *Prefix = PFF_PREFIX_LONGLONG;
    }
    /* grab prefix of 'I64' for __int64 */
    if ((*Fmt)[0] == 'I' && (*Fmt)[1] == '6' && (*Fmt)[2] == '4')
    {
        /* convert to 'll' so BSD's snprintf can handle it */
        *Fmt += 3;
        *Prefix = PFF_PREFIX_LONGLONG;
    }
    /* grab a prefix of 'h' */
    else if (**Fmt == 'h')
    {
        *Prefix = PFF_PREFIX_SHORT;
        ++(*Fmt);
    }
    /* grab prefix of 'l' or the undocumented 'w' (at least in MSDN) */
    else if (**Fmt == 'l' || **Fmt == 'w')
    {
        ++(*Fmt);
        // Only want to change the prefix on 64 bit when printing characters.
        if (**Fmt == 'c' || **Fmt == 's')
        {
            *Prefix = PFF_PREFIX_LONG;
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
        *Type = PFF_TYPE_CHAR;
        if (*Prefix != PFF_PREFIX_SHORT && **Fmt == 'C')
        {
            *Prefix = PFF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == PFF_PREFIX_LONG)
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
        *Type = PFF_TYPE_STRING;
        if (*Prefix != PFF_PREFIX_SHORT && **Fmt == 'S')
        {
            *Prefix = PFF_PREFIX_LONG; /* give it a wide prefix */
        }
        if (*Prefix == PFF_PREFIX_LONG)
        {
            *Out++ = 'l';
        }
        *Out++ = 's';
        ++(*Fmt);
        Result = TRUE;
    }
    /* grab int types types */
    else if (**Fmt == 'd' || **Fmt == 'i' || **Fmt == 'o' ||
             **Fmt == 'u' || **Fmt == 'x' || **Fmt == 'X')
    {
        *Type = PFF_TYPE_INT;
        if (*Prefix == PFF_PREFIX_SHORT)
        {
            *Out++ = 'h';
        }
        else if (*Prefix == PFF_PREFIX_LONG)
        {
            *Out++ = 'l';
        }
        else if (*Prefix == PFF_PREFIX_LONGLONG)
        {
            *Out++ = 'l';
            *Out++ = 'l';
        }
        *Out++ = *(*Fmt)++;
        Result = TRUE;
    }
    else if (**Fmt == 'e' || **Fmt == 'E' || **Fmt == 'f' ||
             **Fmt == 'g' || **Fmt == 'G')
    {
        /* we can safely ignore the prefixes and only add the type*/
        *Type = PFF_TYPE_FLOAT;
        *Out++ = *(*Fmt)++;
        Result = TRUE;
    }
    else if (**Fmt == 'n')
    {
        if (*Prefix == PFF_PREFIX_SHORT)
        {
            *Out++ = 'h';
        }
        *Out++ = *(*Fmt)++;
        *Type = PFF_TYPE_N;
        Result = TRUE;
    }
    else if (**Fmt == 'p')
    {
        *Type = PFF_TYPE_P;
        *Out++ = *(*Fmt)++;

        if (*Prefix == PFF_PREFIX_LONGLONG)
        {
            if (*Precision == PRECISION_DEFAULT)
            {
                *Precision = 16;
            }
        }
        else
        {
            if (*Precision == PRECISION_DEFAULT)
            {
                *Precision = 8;
            }
        }
        Result = TRUE;
    }

    *Out = 0;  /* end the string */
    PAL_free(TempStr);
    return Result;
}

/*******************************************************************************
Function:
  AddPaddingVfprintf (silent version)
    see Internal_AddPaddingVfprintf in printf.c
*******************************************************************************/
int32_t Silent_AddPaddingVfprintf(PAL_FILE *stream, LPSTR In, int32_t Padding, int32_t Flags)
{
    LPSTR Out;
    int32_t LengthInStr;
    int32_t Length;
    LPSTR OutOriginal;
    int32_t Written;

    LengthInStr = strlen(In);
    Length = LengthInStr;


    if (Padding > 0)
    {
        Length += Padding;
    }
    Out = (LPSTR) PAL_malloc(Length+1);
    int iLen = Length+1;
    if (!Out)
    {
        return -1;
    }
    OutOriginal = Out;

    if (Flags & PFF_MINUS) /* pad on right */
    {
        if (strcpy_s(Out, iLen, In) != SAFECRT_SUCCESS)
        {
            Written = -1;
            goto Done;
        }

        Out += LengthInStr;
        iLen -= LengthInStr;
    }
    if (Padding > 0)
    {
        iLen -= Padding;
        if (Flags & PFF_ZERO) /* '0', pad with zeros */
        {
            while (Padding--)
            {
                *Out++ = '0';
            }
        }
        else /* pad with spaces */
        {
            while (Padding--)
            {
                *Out++ = ' ';
            }
        }
    }
    if (!(Flags & PFF_MINUS)) /* put 'In' after padding */
    {
        if (strcpy_s(Out, Length+1, In) != SAFECRT_SUCCESS)
        {
            Written = -1;
            goto Done;
        }

        Out += LengthInStr;
        iLen -= LengthInStr;
    }

#if FILE_OPS_CHECK_FERROR_OF_PREVIOUS_CALL
    clearerr((FILE*)stream);
#endif

    Written = PAL_fwrite(OutOriginal, 1, Length, stream);
    if (stream->PALferrorCode == PAL_FILE_ERROR)
    {
        Written = -1;
    }

Done:
    PAL_free(OutOriginal);
    return Written;
}

/*++
Function:
  PAL_wcslen (silent version)

See MSDN or the man page for wcslen.

--*/
size_t Silent_PAL_wcslen(const char16_t *string)
{
    size_t nChar = 0;

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

