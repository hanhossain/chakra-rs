//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    path.c

Abstract:

    Implementation of all functions related to path support

Revision History:



--*/

#include <string>
#include "pal/thread.hpp"
#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include <new>
#include "pal/stackstring.hpp"

#include <errno.h>

#include <unistd.h>
#include <alloca.h>

SET_DEFAULT_DEBUG_CHANNEL(FILE);


// In safemath.h, Template SafeInt uses macro _ASSERTE, which need to use variable
// defdbgchan defined by SET_DEFAULT_DEBUG_CHANNEL. Therefore, the include statement
// should be placed after the SET_DEFAULT_DEBUG_CHANNEL(FILE)
#include <safemath.h>

int MaxWCharToAcpLengthRatio = 3;

/*++
Function:
  GetTempPathA

See MSDN.

Notes:
    On Windows, the temp path is determined by the following steps:
    1. The value of the "TMP" environment variable, or if it doesn't exist,
    2. The value of the "TEMP" environment variable, or if it doesn't exist,
    3. The Windows directory.

    On Unix, we follow in spirit:
    1. The value of the "TMPDIR" environment variable, or if it doesn't exist,
    2. The /tmp directory.
    This is the same approach employed by mktemp.

--*/
uint32_t
GetTempPathA(
	      uint32_t nBufferLength,
	      char* lpBuffer)
{
    uint32_t dwPathLen = 0;

    if ( !lpBuffer )
    {
        ERROR( "lpBuffer was not a valid pointer.\n" );
        SetLastError( ERROR_INVALID_PARAMETER );
        LOGEXIT("GetTempPathA returns DWORD %u\n", dwPathLen);
        return 0;
    }

    /* Try the TMPDIR environment variable. This is the same env var checked by mktemp. */
    dwPathLen = GetEnvironmentVariableA("TMPDIR", lpBuffer, nBufferLength);
    if (dwPathLen > 0)
    {
        /* The env var existed. dwPathLen will be the length without null termination
         * if the entire value was successfully retrieved, or it'll be the length
         * required to store the value with null termination.
         */
        if (dwPathLen < nBufferLength)
        {
            /* The environment variable fit in the buffer. Make sure it ends with '/'. */
            if (lpBuffer[dwPathLen - 1] != '/')
            {
                /* If adding the slash would still fit in our provided buffer, do it.  Otherwise,
                 * let the caller know how much space would be needed.
                 */
                if (dwPathLen + 2 <= nBufferLength)
                {
                    lpBuffer[dwPathLen++] = '/';
                    lpBuffer[dwPathLen] = '\0';
                }
                else
                {
                    dwPathLen += 2;
                }
            }
        }
        else /* dwPathLen >= nBufferLength */
        {
            /* The value is too long for the supplied buffer.  dwPathLen will now be the
             * length required to hold the value, but we don't know whether that value
             * is going to be '/' terminated.  Since we'll need enough space for the '/', and since
             * a caller would assume that the dwPathLen we return will be sufficient,
             * we make sure to account for it in dwPathLen even if that means we end up saying
             * one more byte of space is needed than actually is.
             */
            dwPathLen++;
        }
    }
    else /* env var not found or was empty */
    {
        /* no luck, use /tmp/ */
        const char *defaultDir = "/tmp/";
        int defaultDirLen = strlen(defaultDir);
        if (defaultDirLen < nBufferLength)
        {
            dwPathLen = defaultDirLen;
            strcpy_s(lpBuffer, nBufferLength, defaultDir);
        }
        else
        {
            /* get the required length */
            dwPathLen = defaultDirLen + 1;
        }
    }

    if ( dwPathLen >= nBufferLength )
    {
        ERROR("Buffer is too small, need space for %d characters including null termination\n", dwPathLen);
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
    }

    LOGEXIT("GetTempPathA returns DWORD %u\n", dwPathLen);
    return dwPathLen;
}

/*++
Function:
  GetTempPathW

See MSDN.
See also the comment for GetTempPathA.
--*/
uint32_t
GetTempPathW(
	      uint32_t nBufferLength,
	      char16_t* lpBuffer)
{
    if (!lpBuffer)
    {
        ERROR("lpBuffer was not a valid pointer.\n");
        SetLastError(ERROR_INVALID_PARAMETER);
        LOGEXIT("GetTempPathW returns DWORD 0\n");
        return 0;
    }

    char TempBuffer[nBufferLength > 0 ? nBufferLength : 1];
    uint32_t dwRetVal = GetTempPathA( nBufferLength, TempBuffer );

    if ( dwRetVal >= nBufferLength )
    {
        ERROR( "lpBuffer was not large enough.\n" );
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        *lpBuffer = '\0';
    }
    else if ( dwRetVal != 0 )
    {
        /* Convert to wide. */
        if ( 0 == MultiByteToWideChar( CP_ACP, 0, TempBuffer, -1,
                                       lpBuffer, dwRetVal + 1 ) )
        {
            ASSERT( "An error occurred while converting the string to wide.\n" );
            SetLastError( ERROR_INTERNAL_ERROR );
            dwRetVal = 0;
        }
    }
    else
    {
        ERROR( "The function failed.\n" );
        *lpBuffer = '\0';
    }

    LOGEXIT("GetTempPathW returns DWORD %u\n", dwRetVal );
    return dwRetVal;
}



/*++
Function:
  FileDosToUnixPathA

Abstract:
  Change a DOS path to a Unix path.

  Replaces '\' by '/', removes any trailing dots on directory/filenames,
  and changes '*.*' to be equal to '*'

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEDosToUnixPathA(
       char* lpPath)
{
    char* p;
    char* pPointAtDot=NULL;
    char charBeforeFirstDot='\0';

    TRACE("Original DOS path = [%s]\n", lpPath);

    if (!lpPath)
    {
        return;
    }

    for (p = lpPath; *p; p++)
    {
        /* Make the \\ to / switch first */
        if (*p == '\\')
        {
            /* Replace \ with / */
            *p = '/';
        }

        if (pPointAtDot)
        {
            /* If pPointAtDot is not NULL, it is pointing at the first encountered
               dot.  If we encountered a \, that means it could be a trailing dot */
            if (*p == '/')
            {
                /* If char before the first dot is a '\' or '.' (special case if the
                   dot is the first char in the path) , then we leave it alone,
                   because it is either . or .., otherwise it is a trailing dot
                   pattern and will be truncated */
                if (charBeforeFirstDot != '.' && charBeforeFirstDot != '/')
                {
                    memmove(pPointAtDot,p,(strlen(p)*sizeof(char))+1);
                    p = pPointAtDot;
                }
                pPointAtDot = NULL; /* Need to reset this */
            }
            else if (*p == '*')
            {
                /* Check our size before doing anything with our pointers */
                if ((p - lpPath) >= 3)
                {
                    /* At this point, we know that there is 1 or more dots and
                       then a star.  AND we know the size of our string at this
                       point is at least 3 (so we can go backwards from our pointer
                       safely AND there could possilby be two characters back)
                       So lets check if there is a '*' and a '.' before, if there
                       is, replace just a '*'.  Otherwise, reset pPointAtDot to NULL
                       and do nothing */
                    if (p[-2] == '*' &&
                        p[-1] == '.' &&
                        p[0] == '*')
                    {
                        memmove(&(p[-2]),p,(strlen(p)*sizeof(char))+1);
                    }

                    pPointAtDot  = NULL;
                }
            }
            else if (*p != '.')
            {
                /* If we are here, that means that this is NOT a trailing dot,
                   some other character is here, so forget our pointer */
                pPointAtDot = NULL;
            }
        }
        else
        {
            if (*p == '.')
            {
                /* If pPointAtDot is NULL, and we encounter a dot, save the pointer */
                pPointAtDot = p;
                if (pPointAtDot != lpPath)
                {
                    charBeforeFirstDot = p[-1];
                }
                else
                {
                    charBeforeFirstDot = lpPath[0];
                }
            }
        }
    }

    /* If pPointAtDot still points at anything, then we still have trailing dots.
       Truncate at pPointAtDot, unless the dots are path specifiers (. or ..) */
    if (pPointAtDot)
    {
        /* make sure the trailing dots don't follow a '/', and that they aren't
           the only thing in the name */
        if(pPointAtDot != lpPath && *(pPointAtDot-1) != '/')
        {
            *pPointAtDot = '\0';
        }
    }

    TRACE("Resulting Unix path = [%s]\n", lpPath);
}


/*++
Function:
  FileDosToUnixPathW

Abstract:
  Change a DOS path to a Unix path.

  Replaces '\' by '/', removes any trailing dots on directory/filenames,
  and changes '*.*' to be equal to '*'

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEDosToUnixPathW(
       char16_t* lpPath)
{
    char16_t* p;
    char16_t* pPointAtDot=NULL;
    char16_t charBeforeFirstDot='\0';

    TRACE("Original DOS path = [%S]\n", lpPath);

    if (!lpPath)
    {
        return;
    }

    for (p = lpPath; *p; p++)
    {
        /* Make the \\ to / switch first */
        if (*p == '\\')
        {
            /* Replace \ with / */
            *p = '/';
        }

        if (pPointAtDot)
        {
            /* If pPointAtDot is not NULL, it is pointing at the first encountered
               dot.  If we encountered a \, that means it could be a trailing dot */
            if (*p == '/')
            {
                /* If char before the first dot is a '\' or '.' (special case if the
                   dot is the first char in the path) , then we leave it alone,
                   because it is either . or .., otherwise it is a trailing dot
                   pattern and will be truncated */
                if (charBeforeFirstDot != '.' && charBeforeFirstDot != '/')
                {
                    memmove(pPointAtDot,p,((std::u16string(p).length()+1)*sizeof(char16_t)));
                    p = pPointAtDot;
                }
                pPointAtDot = NULL; /* Need to reset this */
            }
            else if (*p == '*')
            {
                /* Check our size before doing anything with our pointers */
                if ((p - lpPath) >= 3)
                {
                    /* At this point, we know that there is 1 or more dots and
                       then a star.  AND we know the size of our string at this
                       point is at least 3 (so we can go backwards from our pointer
                       safely AND there could possilby be two characters back)
                       So lets check if there is a '*' and a '.' before, if there
                       is, replace just a '*'.  Otherwise, reset pPointAtDot to NULL
                       and do nothing */
                    if (p[-2] == '*' &&
                        p[-1] == '.' &&
                        p[0] == '*')
                    {
                        memmove(&(p[-2]),p,(std::u16string(p).length()*sizeof(char16_t)));
                    }

                    pPointAtDot  = NULL;
                }
            }
            else if (*p != '.')
            {
                /* If we are here, that means that this is NOT a trailing dot,
                   some other character is here, so forget our pointer */
                pPointAtDot = NULL;
            }
        }
        else
        {
            if (*p == '.')
            {
                /* If pPointAtDot is NULL, and we encounter a dot, save the pointer */
                pPointAtDot = p;
                if (pPointAtDot != lpPath)
                {
                    charBeforeFirstDot = p[-1];
                }
                else
                {
                    charBeforeFirstDot = lpPath[0];
                }
            }
        }
    }

    /* If pPointAtDot still points at anything, then we still have trailing dots.
       Truncate at pPointAtDot, unless the dots are path specifiers (. or ..) */
    if (pPointAtDot)
    {
        /* make sure the trailing dots don't follow a '/', and that they aren't
           the only thing in the name */
        if(pPointAtDot != lpPath && *(pPointAtDot-1) != '/')
        {
            *pPointAtDot = '\0';
        }
    }

    TRACE("Resulting Unix path = [%S]\n", lpPath);
}


/*++
Function:
  FileUnixToDosPathA

Abstract:
  Change a Unix path to a DOS path. Replace '/' by '\'.

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEUnixToDosPathA(
       char* lpPath)
{
    char* p;

    TRACE("Original Unix path = [%s]\n", lpPath);

    if (!lpPath)
        return;

    for (p = lpPath; *p; p++)
    {
        if (*p == '/')
            *p = '\\';
    }

    TRACE("Resulting DOS path = [%s]\n", lpPath);
}


/*++
Function:
  FILEGetDirectoryFromFullPathA

Parse the given path. If it contains a directory part and a file part,
put the directory part into the supplied buffer, and return the number of
characters written to the buffer. If the buffer is not large enough,
return the required size of the buffer including the NULL character. If
there is no directory part in the path, return 0.
--*/
uint32_t FILEGetDirectoryFromFullPathA( const char * lpFullPath,
                     uint32_t  nBufferLength,
                     char*  lpBuffer )
{
    int    full_len, dir_len, i;
    const char * lpDirEnd;
    uint32_t  dwRetLength;

    full_len = lstrlenA( lpFullPath );

    /* look for the first path separator backwards */
    lpDirEnd = lpFullPath + full_len - 1;
    while( lpDirEnd >= lpFullPath && *lpDirEnd != '/' && *lpDirEnd != '\\')
    --lpDirEnd;

    dir_len = lpDirEnd - lpFullPath + 1; /* +1 for fencepost */

    if ( dir_len <= 0 )
    {
        dwRetLength = 0;
    }
    else if (static_cast<uint32_t>(dir_len) >= nBufferLength)
    {
        dwRetLength = dir_len + 1; /* +1 for NULL char */
    }
    else
    {
    /* put the directory into the buffer, including 1 or more
       trailing path separators */
    for( i = 0; i < dir_len; ++i )
        *(lpBuffer + i) = *(lpFullPath + i);

    *(lpBuffer + i) = '\0';

    dwRetLength = dir_len;
    }

    return( dwRetLength );
}

/*++
Function:
  FILEGetFileNameFromFullPath

Given a full path, return a pointer to the first char of the filename part.
--*/
const char * FILEGetFileNameFromFullPathA( const char * lpFullPath )
{
    int DirLen = FILEGetDirectoryFromFullPathA( lpFullPath, 0, NULL );

    if ( DirLen > 0 )
    {
        return lpFullPath + DirLen - 1;
    }
    else
    {
        return lpFullPath;
    }
}

/*++
FILECanonicalizePath
    Removes all instances of '/./', '/../' and '//' from an absolute path.

Parameters:
    char* lpUnixPath : absolute path to modify, in Unix format

(no return value)

Notes :
-behavior is undefined if path is not absolute
-the order of steps *is* important: /one/./../two would give /one/two
 instead of /two if step 3 was done before step 2
-reason for this function is that GetFullPathName can't use realpath(), since
 realpath() requires the given path to be valid and GetFullPathName does not.
--*/
void FILECanonicalizePath(char* lpUnixPath)
{
    char* slashslashptr;
    char* dotdotptr;
    char* slashdotptr;
    char* slashptr;

    /* step 1 : replace '//' sequences by a single '/' */

    slashslashptr = lpUnixPath;
    while(1)
    {
        slashslashptr = strstr(slashslashptr,"//");
        if(NULL == slashslashptr)
        {
            break;
        }
        /* remove extra '/' */
        TRACE("stripping '//' from %s\n", lpUnixPath);
        memmove(slashslashptr,slashslashptr+1,strlen(slashslashptr+1)+1);
    }

    /* step 2 : replace '/./' sequences by a single '/' */

    slashdotptr = lpUnixPath;
    while(1)
    {
        slashdotptr = strstr(slashdotptr,"/./");
        if(NULL == slashdotptr)
        {
            break;
        }
        /* strip the extra '/.' */
        TRACE("removing '/./' sequence from %s\n", lpUnixPath);
        memmove(slashdotptr,slashdotptr+2,strlen(slashdotptr+2)+1);
    }

    /* step 3 : replace '/<name>/../' sequences by a single '/' */

    while(1)
    {
        dotdotptr = strstr(lpUnixPath,"/../");
        if(NULL == dotdotptr)
        {
            break;
        }
        if(dotdotptr == lpUnixPath)
        {
            /* special case : '/../' at the beginning of the path are replaced
               by a single '/' */
            TRACE("stripping leading '/../' from %s\n", lpUnixPath);
            memmove(lpUnixPath, lpUnixPath+3,strlen(lpUnixPath+3)+1);
            continue;
        }

        /* null-terminate the string before the '/../', so that strrchr will
           start looking right before it */
        *dotdotptr = '\0';
        slashptr = strrchr(lpUnixPath,'/');
        if(NULL == slashptr)
        {
            /* this happens if this function was called with a relative path.
               don't do that.  */
            ASSERT("can't find leading '/' before '/../ sequence\n");
            break;
        }
        TRACE("removing '/<dir>/../' sequence from %s\n", lpUnixPath);
        memmove(slashptr,dotdotptr+3,strlen(dotdotptr+3)+1);
    }

    /* step 4 : remove a trailing '/..' */

    dotdotptr = strstr(lpUnixPath,"/..");
    if(dotdotptr == lpUnixPath)
    {
        /* if the full path is simply '/..', replace it by '/' */
        lpUnixPath[1] = '\0';
    }
    else if(NULL != dotdotptr && '\0' == dotdotptr[3])
    {
        *dotdotptr = '\0';
        slashptr = strrchr(lpUnixPath,'/');
        if(NULL != slashptr)
        {
            /* make sure the last slash isn't the root */
            if (slashptr == lpUnixPath)
            {
                lpUnixPath[1] = '\0';
            }
            else
            {
                *slashptr = '\0';
            }
        }
    }

    /* step 5 : remove a traling '/.' */

    slashdotptr = strstr(lpUnixPath,"/.");
    if (slashdotptr != NULL && slashdotptr[2] == '\0')
    {
        if(slashdotptr == lpUnixPath)
        {
            // if the full path is simply '/.', replace it by '/' */
            lpUnixPath[1] = '\0';
        }
        else
        {
            *slashdotptr = '\0';
        }
    }
}


/*++
Function:
  PathFindFileNameW

See MSDN doc.
--*/
char16_t*
PathFindFileNameW(
     const char16_t* pPath
    )
{
    char16_t* ret = (char16_t*)pPath;
    if (ret != NULL && *ret != W('\0'))
    {
        ret = PAL_wcschr(ret, W('\0')) - 1;
        if (ret > pPath && *ret == W('/'))
        {
            ret--;
        }
        while (ret > pPath && *ret != W('/'))
        {
            ret--;
        }
        if (*ret == W('/') && *(ret + 1) != W('\0'))
        {
            ret++;
        }
    }

    LOGEXIT("PathFindFileNameW returns %S\n", ret);
    return ret;
}
