//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    directory.c

Abstract:

    Implementation of the file WIN API for the PAL

Revision History:



--*/

#include <string>
#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/stackstring.hpp"

#include <alloca.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

SET_DEFAULT_DEBUG_CHANNEL(FILE);


/*++
Function:
  GetCurrentDirectoryW

See MSDN doc.
--*/
uint32_t
GetCurrentDirectoryW(
              uint32_t nBufferLength,
              char16_t* lpBuffer)
{
    uint32_t dwWideLen = 0;
    uint32_t dwLastError = 0;

    char  *current_dir;
    int   dir_len;

    current_dir = getcwd( NULL, MAX_LONGPATH + 1 );

    if ( !current_dir )
    {
        WARN( "PAL__getcwd returned NULL\n" );
        dwLastError = DIRGetLastErrorFromErrno();
        goto done;
    }

    dir_len = strlen( current_dir );
    dwWideLen = MultiByteToWideChar( CP_ACP, 0,
                 current_dir, dir_len,
                 NULL, 0 );

    /* if the supplied buffer isn't long enough, return the required
       length, including room for the NULL terminator */
    if ( nBufferLength > dwWideLen )
    {
        if(!MultiByteToWideChar( CP_ACP, 0, current_dir, dir_len + 1,
                     lpBuffer, nBufferLength ))
        {
            ASSERT("MultiByteToWideChar failure!\n");
            dwWideLen = 0;
            dwLastError = ERROR_INTERNAL_ERROR;
        }
    }
    else
    {
        ++dwWideLen; /* include space for the NULL */
    }

done:
    free( current_dir );

    if ( dwLastError )
    {
        SetLastError(dwLastError);
    }

    LOGEXIT("GetCurrentDirectoryW returns DWORD %u\n", dwWideLen);
    return dwWideLen;
}