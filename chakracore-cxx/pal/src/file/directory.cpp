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
  GetCurrentDirectoryA

See MSDN doc.
--*/
uint32_t
GetCurrentDirectoryA(
              uint32_t nBufferLength,
              char* lpBuffer)
{
    uint32_t dwDirLen = 0;
    uint32_t dwLastError = 0;

    char  *current_dir;

    /* NULL first arg means getcwd will allocate the string */
    current_dir = getcwd( NULL, MAX_LONGPATH + 1 );

    if ( !current_dir )
    {
        WARN( "PAL__getcwd returned NULL\n" );
        dwLastError = DIRGetLastErrorFromErrno();
        goto done;
    }

    dwDirLen = strlen( current_dir );

    /* if the supplied buffer isn't long enough, return the required
       length, including room for the NULL terminator */
    if ( nBufferLength <= dwDirLen )
    {
        ++dwDirLen; /* include space for the NULL */
        goto done;
    }
    else
    {
        strcpy_s( lpBuffer, nBufferLength, current_dir );
    }

done:
    free( current_dir );

    if ( dwLastError )
    {
        SetLastError(dwLastError);
    }

    LOGEXIT("GetCurrentDirectoryA returns DWORD %u\n", dwDirLen);
    return dwDirLen;
}


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


/*++
Function:
  SetCurrentDirectoryW

See MSDN doc.
--*/
BOOL
SetCurrentDirectoryW(
             const char16_t* lpPathName)
{
    BOOL bRet;
    uint32_t dwLastError = 0;
    PathCharString dirPathString;
    int  size;
    size_t length;
    char * dir;
    
   /*check if the given path is null. If so
     return FALSE*/
    if (lpPathName == NULL )
    {
        ERROR("Invalid path/directory name\n");
        dwLastError = ERROR_INVALID_NAME;
        bRet = FALSE;
        goto done;
    }

    length = (std::u16string(lpPathName).length()+1) * 3;
    dir = dirPathString.OpenStringBuffer(length);
    if (NULL == dir)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        bRet = FALSE;
        goto done;
    }
    
    size = WideCharToMultiByte( CP_ACP, 0, lpPathName, -1, dir, length,
                                NULL, NULL );
    dirPathString.CloseBuffer(size);
    
    if( size == 0 )
    {
        dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpPathName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            dwLastError = ERROR_FILENAME_EXCED_RANGE;
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            dwLastError = ERROR_INTERNAL_ERROR;
        }
        bRet = FALSE;
        goto done;
    }

    bRet = SetCurrentDirectoryA(dir);
done:
    if( dwLastError )
    {
        SetLastError(dwLastError);
    }

    LOGEXIT("SetCurrentDirectoryW returns BOOL %d\n", bRet);
    return bRet;
}

/*++
Function:
  SetCurrentDirectoryA

See MSDN doc.
--*/
BOOL
SetCurrentDirectoryA(
             const char * lpPathName)
{
    BOOL bRet = FALSE;
    uint32_t dwLastError = 0;
    int result;
    char* UnixPathName = NULL;

   /*check if the given path is null. If so
     return FALSE*/
    if (lpPathName == NULL )
    {
        ERROR("Invalid path/directory name\n");
        dwLastError = ERROR_INVALID_NAME;
        goto done;
    }
    if (strlen(lpPathName) >= MAX_LONGPATH)
    {
        WARN("Path/directory name longer than MAX_LONGPATH characters\n");
        dwLastError = ERROR_FILENAME_EXCED_RANGE;
        goto done;
    }

    UnixPathName = strdup(lpPathName);
    if (UnixPathName == NULL )
    {
        ERROR("strdup() failed\n");
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    FILEDosToUnixPathA( UnixPathName );

    TRACE("Attempting to open Unix dir [%s]\n", UnixPathName);
    result = chdir(UnixPathName);

    if ( result == 0 )
    {
        bRet = TRUE;
    }
    else
    {
        if ( errno == ENOTDIR || errno == ENOENT )
        {
            struct stat stat_data;

            if ( stat( UnixPathName, &stat_data) == 0 &&
                 (stat_data.st_mode & S_IFMT) == S_IFREG )
            {
                /* Not a directory, it is a file. */
                dwLastError = ERROR_DIRECTORY;
            }
            else
            {
                FILEGetProperNotFoundError( UnixPathName, &dwLastError );
            }
            TRACE("chdir() failed, path was invalid.\n");
        }
        else
        {
            dwLastError = ERROR_ACCESS_DENIED;
            ERROR("chdir() failed; errno is %d (%s)\n", errno, strerror(errno));
        }
    }


done:
    if( dwLastError )
    {
        SetLastError(dwLastError);
    }

    if(UnixPathName != NULL)
    {
        free( UnixPathName );
    }

    LOGEXIT("SetCurrentDirectoryA returns BOOL %d\n", bRet);
    return bRet;
}
