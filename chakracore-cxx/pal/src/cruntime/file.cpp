//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    file.c

Abstract:

    Implementation of the file functions in the C runtime library that
    are Windows specific.



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/cruntime.h"

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <limits.h>

    #define CLEARERR(f)

SET_DEFAULT_DEBUG_CHANNEL(CRT);

/*++

Function:

    CRTInitStdStreams.

    Initilizes the standard streams.
    Returns TRUE on success, FALSE otherwise.
--*/
BOOL CRTInitStdStreams()
{
    return TRUE;
}

/*++
Function :

    MapFileOpenModes

    Maps Windows file open modes to Unix fopen modes and validates.

--*/
static char* MapFileOpenModes(char* str)
{
    char* retval = NULL;
    char* temp = NULL;

    if (NULL == str)
    {
        ASSERT("MapFileOpenModes called with a NULL parameter for str.\n");
        return NULL;
    }

    /* The PAL behaves differently for some Windows file open modes:

    c, n, S, R, and T: these are all hints to the system that aren't supported
    by the PAL. Since the user cannot depend on this behavior, it's safe to
    simply ignore these modes.

    D: specifies a file as temporary. This file is expected to be deleted when
    the last file descriptor is closed. The PAL does not support this behavior
    and asserts when this mode is used.

    t: represents opening in text mode. Calls to fdopen on Unix don't accept
    't' so it is silently stripped out. However, the PAL supports the mode by
    having the PAL wrappers do the translation of CR-LF to LF and vice versa.

    t vs. b: To get binary mode, you must explicitly use 'b'. If neither mode
    is specified on Windows, the default mode is defined by the global
    variable _fmode. The PAL simply defaults to text mode. After examining
    CLR usage patterns, the PAL behavior seems acceptable. */

    /* Check if the mode specifies deleting the temporary file
    automatically when the last file descriptor is closed.
    The PAL does not support this behavior. */
    if (NULL != strchr(str,'D'))
    {
        ASSERT("The PAL doesn't support the 'D' flag for _fdopen and fopen.\n");
        return NULL;
    }

    retval = (char*)malloc( ( strlen( str ) + 1 ) * sizeof( char ) );
    if (NULL == retval)
    {
        ERROR("Unable to allocate memory.\n");
        return NULL;
    }

    temp = retval;
    while ( *str )
    {
        if ( *str == 'r' || *str == 'w' || *str == 'a' )
        {
            *temp = *str;
            temp++;
            if ( ( ++str != NULL ) && *str == '+' )
            {
                *temp = *str;
                temp++;
                str++;
            }
        }
        else
        {
            str++;
        }
    }
    *temp = '\0';
    return retval;
}

/*++

Function :
    fopen

see MSDN doc.

--*/
FILE *
PAL_fopen(const char * fileName, const char * mode)
{
    FILE *f = nullptr;
    char* supported = NULL;
    char* UnixFileName = NULL;
    struct stat stat_data;

    ENTRY("fopen ( fileName=%p (%s) mode=%p (%s))\n", fileName, fileName, mode , mode );

    _ASSERTE(fileName != NULL);
    _ASSERTE(mode != NULL);

    if ( *mode == 'r' || *mode == 'w' || *mode == 'a' )
    {
        supported = MapFileOpenModes( (char*)mode);

        if ( !supported )
        {
            goto done;
        }

        UnixFileName = strdup(fileName);
        if (UnixFileName == NULL )
        {
            ERROR("strdup() failed\n");
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            goto done;
        }

        FILEDosToUnixPathA( UnixFileName );

        /*I am not checking for the case where stat fails
         *as fopen will handle the error more gracefully in case
         *UnixFileName is invalid*/
        if ((stat(UnixFileName, &stat_data) == 0 ) &&
            ((stat_data.st_mode & S_IFMT) == S_IFDIR))
        {
            goto done;
        }

        f = (FILE*)fopen( UnixFileName, supported );
    }
    else
    {
        ERROR( "The mode flags must start with either an a, w, or r.\n" );
    }

done:
    free( supported );
    supported = NULL;
    free( UnixFileName );

    LOGEXIT( "fopen returns FILE* %p\n", f );
    return f;
}

/*++
Function:
  _wfopen

see MSDN doc.

--*/
FILE *
_wfopen(
    const char16_t *fileName,
    const char16_t *mode)
{
    char mbFileName[ _MAX_PATH ];
    char mbMode[ 10 ];
    FILE * filePtr = NULL;

    ENTRY("_wfopen(fileName:%p (%S), mode:%p (%S))\n", fileName, fileName, mode, mode);

    _ASSERTE(fileName != NULL);
    _ASSERTE(mode != NULL);

    /* Convert the parameters to ASCII and defer to PAL_fopen */
    if ( WideCharToMultiByte( CP_ACP, 0, fileName, -1, mbFileName,
                              sizeof mbFileName, NULL, NULL ) != 0 )
    {
        if ( WideCharToMultiByte( CP_ACP, 0, mode, -1, mbMode,
                                  sizeof mbMode, NULL, NULL ) != 0 )
        {
            filePtr = PAL_fopen(mbFileName, mbMode);
        }
        else
        {
            ERROR( "An error occurred while converting mode to ANSI.\n" );
        }
    }
    else
    {
        ERROR( "An error occurred while converting"
               " fileName to ANSI string.\n" );
    }
    LOGEXIT("_wfopen returning FILE* %p\n", filePtr);
    return filePtr;
}
