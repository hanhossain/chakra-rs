//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    string.cpp

Abstract:

    Implementation of the string functions in the C runtime library that are Windows specific.



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/cruntime.h"

#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>


SET_DEFAULT_DEBUG_CHANNEL(CRT);


/*++
Function:
  _strlwr

Convert a string to lowercase.


This function returns a pointer to the converted string. Because the
modification is done in place, the pointer returned is the same as the
pointer passed as the input argument. No return value is reserved to
indicate an error.

Parameter

string  Null-terminated string to convert to lowercase

Remarks

The _strlwr function converts any uppercase letters in string to
lowercase as determined by the LC_CTYPE category setting of the
current locale. Other characters are not affected. For more
information on LC_CTYPE, see setlocale.

--*/
char *
_strlwr(
        char *str)
{
    char *orig = str;

    ENTRY("_strlwr (str=%p (%s))\n", str?str:"NULL", str?str:"NULL");

    while (*str)
    {
        *str = tolower(*str);
        str++;
    }

    LOGEXIT("_strlwr returning char* %p (%s)\n", orig?orig:"NULL", orig?orig:"NULL");
    return orig;
}
