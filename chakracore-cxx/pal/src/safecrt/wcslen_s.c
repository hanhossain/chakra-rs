//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/***
*wcslen_s.c - contains wcsnlen() routine
*

*
*Purpose:
*   wcslen returns the length of a null-terminated wide-character string,
*   not including the null char16_t itself.
*
*******************************************************************************/


#include <string.h>
#include <errno.h>
#include <limits.h>
#include "internal_securecrt.h"

#include "mbusafecrt_internal.h"

/***
*wcsnlen - return the length of a null-terminated wide-character string
*
*Purpose:
*   Finds the length in bytes of the given string, not including
*   the final null character. Only the first maxsize characters
*   are inspected: if the null character is not found, maxsize is
*   returned.
*
*Entry:
*   const char16_t * wcs - string whose length is to be computed
*   size_t maxsize
*
*Exit:
*   Length of the string "wcs", exclusive of the final null byte, or
*   maxsize if the null character is not found.
*
*Exceptions:
*
*******************************************************************************/

size_t wcsnlen(const char16_t *wcs, size_t maxsize)
{
    size_t n;

    /* Note that we do not check if s == NULL, because we do not
     * return errno_t...
     */

    for (n = 0; n < maxsize && *wcs; n++, wcs++)
        ;

    return n;
}

