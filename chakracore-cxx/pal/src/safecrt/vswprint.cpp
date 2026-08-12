//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/***
*vswprint.c - print formatted data into a string from var arg list
*
*Purpose:
*       defines vswprintf(), _vswprintf_c and _vsnwprintf() - print formatted output to
*       a string, get the data from an argument ptr instead of explicit
*       arguments.
*
*******************************************************************************/


#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "internal_securecrt.h"

#include "mbusafecrt_internal.h"

typedef int (*WOUTPUTFN)(miniFILE *, const char16_t *, va_list);

static int _vswprintf_helper( WOUTPUTFN outfn, char16_t *string, size_t count, const char16_t *format, va_list ap );

int _vswprintf_helper (
        WOUTPUTFN woutfn,
        char16_t *string,
        size_t count,
        const char16_t *format,
        va_list ap
        )
{
        miniFILE str;
        miniFILE *outfile = &str;
        int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        _VALIDATE_RETURN( (count == 0) || (string != NULL), EINVAL, -1 );

        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = reinterpret_cast<char*>(string);

        if(count>(INT_MAX/sizeof(char16_t)))
        {
           /* old-style functions allow any large value to mean unbounded */
           outfile->_cnt = INT_MAX;
        }
        else
        {
            outfile->_cnt = static_cast<int>(count * sizeof(char16_t));
        }

        retval = woutfn(outfile, format, ap );

        if(string==NULL)
        {
            return retval;
        }

        if((retval >= 0) && (_putc_nolock('\0',outfile) != EOF) && (_putc_nolock('\0',outfile) != EOF))
            return(retval);

        string[count - 1] = 0;
        if (outfile->_cnt < 0)
        {
            /* the buffer was too small; we return -2 to indicate truncation */
            return -2;
        }
        return -1;
}

int _vswprintf_s (
        char16_t *string,
        size_t sizeInWords,
        const char16_t *format,
        va_list ap
        )
{
    int retvalue = -1;

    /* validation section */
    _VALIDATE_RETURN(format != NULL, EINVAL, -1);
    _VALIDATE_RETURN(string != NULL && sizeInWords > 0, EINVAL, -1);

    retvalue = _vswprintf_helper(_woutput_s, string, sizeInWords, format, ap);
    if (retvalue < 0)
    {
        string[0] = 0;
        _SECURECRT__FILL_STRING(string, sizeInWords, 1);
    }
    if (retvalue == -2)
    {
        _VALIDATE_RETURN(("Buffer too small" && 0), ERANGE, -1);
    }
    if (retvalue >= 0)
    {
        _SECURECRT__FILL_STRING(string, sizeInWords, retvalue + 1);
    }

    return retvalue;
}

extern "C" int vswprintf_s (
        char16_t *string,
        size_t sizeInWords,
        const char16_t *format,
        va_list ap
        )
{
    return _vswprintf_s(string, sizeInWords, format, ap);
}
