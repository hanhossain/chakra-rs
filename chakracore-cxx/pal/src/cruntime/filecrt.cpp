//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    filecrt.cpp

Abstract:

    Implementation of the file functions in the C runtime library that
    are Windows specific.



--*/

#include "pal/thread.hpp"
#include "pal/file.hpp"

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/cruntime.h"

#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef __APPLE__
#include <sys/syscall.h>
#endif // __APPLE__

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(CRT);


/*++
InternalOpen

Wrapper for open.

Input parameters:

szPath = pointer to a pathname of a file to be opened
nFlags = arguments that control how the file should be accessed
mode = file permission settings that are used only when a file is created

Return value:
    File descriptor on success, -1 on failure
--*/
int
CorUnix::InternalOpen(
    const char *szPath,
    int nFlags,
    ...
    )
{
    int nRet = -1;
    int mode = 0;
    va_list ap;

    // If nFlags does not contain O_CREAT, the mode parameter will be ignored.
    if (nFlags & O_CREAT)
    {
        va_start(ap, nFlags);
        mode = va_arg(ap, int);
        va_end(ap);
    }

        nRet = open(szPath, nFlags, mode);
    return nRet;
}