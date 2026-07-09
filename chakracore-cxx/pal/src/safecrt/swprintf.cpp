//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/***
*swprintf.c - print formatted to string
*
*Purpose:
*       defines _swprintf(), _swprintf_c - print formatted data
*       to string
*
*******************************************************************************/

#include <string.h>
#include <errno.h>
#include <limits.h>
#include "internal_securecrt.h"

#include "mbusafecrt_internal.h"

/***
*ifndef _COUNT_
*int _swprintf(string, format, ...) - print formatted data to string
*else
*int _swprintf_c(string, cnt, format, ...) - print formatted data to string
*endif
*
*Purpose:
*       Prints formatted data to the using the format string to
*       format data and getting as many arguments as called for
*       Sets up a FILE so file i/o operations can be used, make
*       string look like a huge buffer to it, but _flsbuf will
*       refuse to flush it if it fills up.  Appends '\0' to make
*       it a true string. _output does the real work here
*
*       Allocate the 'fake' _iob[] entry statically instead of on
*       the stack so that other routines can assume that _iob[]
*       entries are in are in DGROUP and, thus, are near.
*
*       We alias swprintf to _swprintf
*
*ifdef _COUNT_
*       Multi-thread: (1) Since there is no stream, this routine must
*       never try to get the stream lock (i.e., there is no stream
*       lock either). (2) Also, since there is only one statically
*       allocated 'fake' iob, we must lock/unlock to prevent collisions.
*
*Entry:
*       char16_t *string - pointer to place to put output
*ifdef _COUNT_
*       size_t count - max number of wide characters to put in buffer
*endif
*       char16_t *format - format string to control data format/number
*       of arguments followed by list of arguments, number and type
*       controlled by format string
*
*Exit:
*       returns number of wide characters printed
*
*Exceptions:
*
*******************************************************************************/

int swprintf_s (
        char16_t *string,
        size_t sizeInWords,
        const char16_t *format,
        ...
        )
{
    int ret;
    va_list arglist;

    va_start(arglist, format);

    ret = _vswprintf_s(string, sizeInWords, format, arglist);

    va_end(arglist);

    return ret;
}

int _snwprintf_s (
        char16_t *string,
        size_t sizeInWords,
        size_t count,
        const char16_t *format,
        ...
        )
{
    int ret;
    va_list arglist;

    va_start(arglist, format);

    ret = _vsnwprintf_s(string, sizeInWords, count, format, arglist);

    va_end(arglist);

    return ret;
}
