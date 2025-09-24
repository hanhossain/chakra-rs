//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    path.c

Abstract:

    Implementation of path functions part of Windows runtime library.

Revision History:



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/printfcpp.hpp"

#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <errno.h>
#include <limits.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);


/* ON_ERROR. A Helper macro for _?splitpath functions. */
#define ON_ERROR if ( drive ) \
        {\
            drive[0] = 0;\
        }\
        if(dir)\
        {\
            dir[0] = 0;\
        }\
        if(fname)\
        {\
            fname[0] = 0;\
        }\
        if(ext)\
        {\
            ext[0] = 0;\
        }\
        goto done;\



/*++
Function:
  _fullpath

See MSDN doc.

--*/
char *
_fullpath(
          char *absPath,
          const char *relPath,
          size_t maxLength)
{
    char realpath_buf[PATH_MAX+1];
    char path_copy[PATH_MAX+1];
    char *retval = NULL;
    uint32_t cPathCopy = sizeof(path_copy)/sizeof(path_copy[0]);
    size_t min_length;
    BOOL fBufAllocated = FALSE;

    ENTRY("_fullpath (absPath=%p, relPath=%p (%s), maxLength = %lu)\n",
          absPath, relPath ? relPath:"NULL", relPath ? relPath:"NULL", maxLength);

    if (strncpy_s(path_copy, sizeof(path_copy), relPath ? relPath : ".", cPathCopy) != SAFECRT_SUCCESS)
    {
        TRACE("_fullpath: strncpy_s failed!\n");
        goto fullpathExit;
    }

    FILEDosToUnixPathA(path_copy);

    if(NULL == realpath(path_copy, realpath_buf))
    {
        // do nothing. filename may not be there yet..
    }

    TRACE("real path is %s\n", realpath_buf);
    min_length = strlen(realpath_buf)+1; // +1 for the NULL terminator

    if(NULL == absPath)
    {
        absPath = static_cast<char *>(
            malloc(_MAX_PATH * sizeof(char)));
        if (!absPath)
        {
            ERROR("malloc failed with error %d\n", errno);
            goto fullpathExit;
        }
        maxLength = _MAX_PATH;
        fBufAllocated = TRUE;
    }

    if(min_length > maxLength)
    {
        ERROR("maxLength is %lu, we need at least %lu\n",
                maxLength, min_length);
        if (fBufAllocated)
        {
            free(absPath);
            fBufAllocated = FALSE;
        }
        goto fullpathExit;
    }

    strcpy_s(absPath, maxLength, realpath_buf);
    retval = absPath;

fullpathExit:
    LOGEXIT("_fullpath returns char * %p\n", retval);
    return retval;
}
