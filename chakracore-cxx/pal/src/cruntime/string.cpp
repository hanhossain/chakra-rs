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
  _stricmp

compare two strings, ignoring case

The stricmp() function compares, with case insensitivity, the string
pointed to by s1 to the string pointed to by s2. All uppercase
characters from s1 and s2 are mapped to lowercase for the purposes of
doing the comparison.

Returns:

Value Meaning

< 0   s1 is less than s2
0     s1 is equal to s2
> 0   s1 is greater than s2

--*/
int
_stricmp(
         const char *s1,
         const char *s2)
{
    int ret;

    ENTRY("_stricmp (s1=%p (%s), s2=%p (%s))\n", s1?s1:"NULL", s1?s1:"NULL", s2?s2:"NULL", s2?s2:"NULL");

    ret = strcasecmp(s1, s2);

    LOGEXIT("_stricmp returning int %d\n", ret);
    return ret;
}


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
