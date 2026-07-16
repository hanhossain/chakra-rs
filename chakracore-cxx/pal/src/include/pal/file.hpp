//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    include/pal/file.hpp

Abstract:
    Header file for file utility functions.

Revision History:



--*/

#ifndef _PAL_FILE_HPP_
#define _PAL_FILE_HPP_

#include "corunix.hpp"

#include <dirent.h>
#include <sys/param.h>
#include <sys/types.h>


namespace CorUnix
{
    extern CObjectType otFile;
    extern CAllowedObjectTypes aotFile;

    class CFileProcessLocalData
    {
    public:
        IFileLockController *pLockController;

        int unix_fd;
        uint32_t dwDesiredAccess; /* Unix assumes files are always opened for reading.
                                  In Windows we can open a file for writing only */
        int open_flags; /* stores Unix file creation flags */
        BOOL open_flags_deviceaccessonly;
        char unix_filename[MAXPATHLEN];
        BOOL inheritable;
    };

    PAL_ERROR
    InternalWriteFile(CPalThread *pThread, HANDLE hFile, const void *lpBuffer, uint32_t nNumberOfBytesToWrite,
                      uint32_t *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

    /*++
    InternalOpen
    Wraps open
    --*/
    int InternalOpen(const char *szFilename, int nFlags, ...);
} // namespace CorUnix

extern "C" {

//
// These routines should all be separated out into something along the lines
// of fileutils.* (instead of being commingled with the core file object
// code).
//


/*++
Function:
  FILEGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t FILEGetLastErrorFromErrno(void);

/*++
FILEInitStdHandles

Create handle objects for stdin, stdout and stderr

(no parameters)

Return value:
    TRUE on success, FALSE on failure
--*/
BOOL FILEInitStdHandles(void);

/*++
FILECleanupStdHandles

Close primary handles for stdin, stdout and stderr

(no parameters, no return value)
--*/
void FILECleanupStdHandles(void);

/*++

Function :
    FILEGetProperNotFoundError

Returns the proper error code, based on the
Windows behavoir.

    IN char* lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError(char *lpPath, uint32_t *lpErrorCode);
}

#endif /* _PAL_FILE_HPP_ */
