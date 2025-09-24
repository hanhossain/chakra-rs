//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    environ.c

Abstract:

    Implementation of functions manipulating environment variables.

Revision History:



--*/

#include "pal/palinternal.h"
#include "pal/critsect.h"
#include "pal/dbgmsg.h"
#include "pal/misc.h"

#include <stdlib.h>

SET_DEFAULT_DEBUG_CHANNEL(MISC);

/*++
Function:
  GetEnvironmentVariableA

The GetEnvironmentVariable function retrieves the value of the
specified variable from the environment block of the calling
process. The value is in the form of a null-terminated string of
characters.

Parameters

lpName 
       [in] Pointer to a null-terminated string that specifies the environment variable. 
lpBuffer 
       [out] Pointer to a buffer to receive the value of the specified environment variable. 
nSize 
       [in] Specifies the size, in TCHARs, of the buffer pointed to by the lpBuffer parameter. 

Return Values

If the function succeeds, the return value is the number of TCHARs
stored into the buffer pointed to by lpBuffer, not including the
terminating null character.

If the specified environment variable name was not found in the
environment block for the current process, the return value is zero.

If the buffer pointed to by lpBuffer is not large enough, the return
value is the buffer size, in TCHARs, required to hold the value string
and its terminating null character.

--*/
uint32_t
GetEnvironmentVariableA(
             const char * lpName,
             char* lpBuffer,
             uint32_t nSize)
{
    char  *value;
    uint32_t dwRet = 0;

    ENTRY("GetEnvironmentVariableA(lpName=%p (%s), lpBuffer=%p, nSize=%u)\n",
        lpName?lpName:"NULL",
        lpName?lpName:"NULL", lpBuffer, nSize);
    
    if (lpName == NULL)
    {
        ERROR("lpName is NULL\n");
        SetLastError(ERROR_INVALID_PARAMETER);
        goto done;
    }

    if (lpName[0] == 0)
    {
        TRACE("lpName is empty string\n", lpName);
        SetLastError(ERROR_ENVVAR_NOT_FOUND);
        goto done;
    }
    
    if (strchr(lpName, '=') != NULL)
    {
        // GetEnvironmentVariable doesn't permit '=' in variable names.
        value = NULL;
    }
    else
    {
        value = MiscGetenv(lpName);
    }
    
    if (value == NULL)
    {
        TRACE("%s is not found\n", lpName);
        SetLastError(ERROR_ENVVAR_NOT_FOUND);
        goto done;
    }

    if (strlen(value) < nSize)
    {
        strcpy_s(lpBuffer, nSize, value);
        dwRet = strlen(value);
    } 
    else 
    {
        dwRet = strlen(value)+1;
    }
    SetLastError(ERROR_SUCCESS);

done:
    LOGEXIT("GetEnvironmentVariableA returns DWORD 0x%x\n", dwRet);
    return dwRet;
}


/*++
Function:
  GetEnvironmentVariableW

See MSDN doc.
--*/
uint32_t
GetEnvironmentVariableW(
             const char16_t* lpName,
             char16_t* lpBuffer,
             uint32_t nSize)
{
    char *inBuff = NULL;
    char *outBuff = NULL;
    int32_t inBuffSize;
    uint32_t size = 0;

    ENTRY("GetEnvironmentVariableW(lpName=%p (%S), lpBuffer=%p, nSize=%u)\n",
          lpName?lpName:W16_NULLSTRING,
          lpName?lpName:W16_NULLSTRING, lpBuffer, nSize);

    inBuffSize = WideCharToMultiByte( CP_ACP, 0, lpName, -1, 
                                      inBuff, 0, NULL, NULL);
    if ( 0 == inBuffSize )
    {
        ERROR( "lpName has to be a valid parameter\n" );
        SetLastError( ERROR_INVALID_PARAMETER );
        goto done;
    }

    inBuff = (char *)malloc(inBuffSize);
    if (inBuff == NULL)
    {
        ERROR("malloc failed\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    
    if (nSize) {
        outBuff = (char *)malloc(nSize*2);
        if (outBuff == NULL)
        {
            ERROR("malloc failed\n");
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            goto done;
        }
    }

    if ( 0 == WideCharToMultiByte( CP_ACP, 0, lpName, -1, inBuff, 
                                   inBuffSize, NULL, NULL ) )
    {
        ASSERT( "WideCharToMultiByte failed!\n" );
        SetLastError( ERROR_INTERNAL_ERROR );
        goto done;
    }
    size = GetEnvironmentVariableA(inBuff, outBuff, nSize);
    if (size > nSize)
    {
        TRACE("Insufficient buffer\n");
    }
    else if ( size == 0 )
    {
        /* error handle in GetEnvironmentVariableA */
    }
    else
    {
        size = MultiByteToWideChar(CP_ACP, 0, outBuff, -1, lpBuffer, nSize);
        if ( 0 != size )
        {
            /* Not including the NULL. */
            size--;
        }
        else
        {
            ASSERT( "MultiByteToWideChar failed!\n" );
            SetLastError( ERROR_INTERNAL_ERROR );
            size = 0;
            *lpBuffer = '\0';
        }
    }

done:
    free(outBuff);
    free(inBuff);

    LOGEXIT("GetEnvironmentVariableW returns DWORD 0x%x\n", size);
    return size;
}


