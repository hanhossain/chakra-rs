//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

unicode.cpp

Abstract:

Implementation of all functions related to Unicode support

Revision History:



--*/

#include <string>
#include "pal/thread.hpp"

#include "pal/palinternal.h"
#include "pal/unicode_data.h"
#include "pal/dbgmsg.h"
#include "pal/utf8.h"
#include "pal/locale.h"
#include "pal/cruntime.h"
#include "pal/stackstring.hpp"
#include "chakra/Logger.h"
#include <format>

#include <pthread.h>
#include <locale.h>
#if !defined(__APPLE__)
#include <libintl.h>
#endif // __APPLE__
#include <errno.h>
#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif // defined(__APPLE__)

#include <debugmacrosext.h>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(UNICODE);

#if !defined(__APPLE__)
/*++
Function:
UnicodeDataComp
This is the comparison function used by the bsearch function to search
for unicode characters in the UnicodeData array.

Parameter:
pnKey
The unicode character value to search for.
elem
A pointer to a UnicodeDataRec.

Return value:
<0 if pnKey < elem->nUnicodeValue
0 if pnKey == elem->nUnicodeValue
>0 if pnKey > elem->nUnicodeValue
--*/
static int UnicodeDataComp(const void *pnKey, const void *elem)
{
    char16_t uValue = static_cast<const UnicodeDataRec*>(elem)->nUnicodeValue;
    uint16_t  rangeValue = static_cast<const UnicodeDataRec*>(elem)->rangeValue;

    if (*static_cast<const int32_t*>(pnKey) < uValue)
    {
        return -1;
    }
    else
    {
        if (*static_cast<const int32_t*>(pnKey) > (uValue + rangeValue))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/*++
Function:
GetUnicodeData
This function is used to get information about a Unicode character.

Parameters:
nUnicodeValue
The numeric value of the Unicode character to get information about.
pDataRec
The UnicodeDataRec to fill in with the data for the Unicode character.

Return value:
TRUE if the Unicode character was found.

--*/
BOOL GetUnicodeData(int32_t nUnicodeValue, UnicodeDataRec *pDataRec)
{
    BOOL bRet;
    if (nUnicodeValue <= UNICODE_DATA_DIRECT_ACCESS)
    {
        *pDataRec = UnicodeData[nUnicodeValue];
        bRet = TRUE;
    }
    else
    {
        UnicodeDataRec *dataRec;
        int32_t nNumOfChars = UNICODE_DATA_SIZE;
        dataRec = static_cast<UnicodeDataRec*>(bsearch(&nUnicodeValue, UnicodeData, nNumOfChars,
                                                       sizeof(UnicodeDataRec), UnicodeDataComp));
        if (dataRec == NULL)
        {
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
            *pDataRec = *dataRec;
        }
    }
    return bRet;
}
#endif /* !defined(__APPLE__) */

/*++
Function:
MultiByteToWideChar

See MSDN doc.

--*/
int
MultiByteToWideChar(const char *lpMultiByteStr, int cbMultiByte, char16_t *lpWideCharStr, int cchWideChar)
{
    int32_t retval =0;

    if ( (cbMultiByte == 0) || (cchWideChar < 0) ||
        (lpMultiByteStr == NULL) ||
        ((cchWideChar != 0) &&
        ((lpWideCharStr == NULL) ||
        (lpMultiByteStr == reinterpret_cast<char*>(lpWideCharStr)))) )
    {
        ERROR("Error lpMultiByteStr parameters are invalid\n");
        SetLastError(ERROR_INVALID_PARAMETER);
        goto EXIT;
    }

    // Use UTF8ToUnicode on all systems, since it replaces
    // invalid characters and Core Foundation doesn't do that.
    if (cbMultiByte <= -1)
    {
        cbMultiByte = strlen(lpMultiByteStr) + 1;
    }

    retval = UTF8ToUnicode(lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);

EXIT:

    LOGEXIT("MultiByteToWideChar returns %d.\n",retval);
    return retval;
}
