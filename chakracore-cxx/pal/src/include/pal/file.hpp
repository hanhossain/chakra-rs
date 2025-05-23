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

#include <sys/types.h>
#include <sys/param.h>
#include <dirent.h>


namespace CorUnix
{
    extern CObjectType otFile;
    extern CAllowedObjectTypes aotFile;

    class CFileProcessLocalData
    {
    public:
        IFileLockController *pLockController;

        int  unix_fd;
        uint32_t dwDesiredAccess; /* Unix assumes files are always opened for reading.
                                  In Windows we can open a file for writing only */
        int  open_flags;       /* stores Unix file creation flags */
        BOOL open_flags_deviceaccessonly;
        char unix_filename[MAXPATHLEN];
        BOOL inheritable;
    };

    PAL_ERROR
    InternalCreateFile(
        CPalThread *pThread,
        LPCSTR lpFileName,
        uint32_t dwDesiredAccess,
        uint32_t dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        uint32_t dwCreationDisposition,
        uint32_t dwFlagsAndAttributes,
        HANDLE hTemplateFile,
        HANDLE *pFileHandle
        );

    PAL_ERROR
    InternalWriteFile(
        CPalThread *pThread,
        HANDLE hFile,
        const void * lpBuffer,
        uint32_t nNumberOfBytesToWrite,
        uint32_t * lpNumberOfBytesWritten,
        LPOVERLAPPED lpOverlapped
        );

    PAL_ERROR
    InternalReadFile(
        CPalThread *pThread,
        HANDLE hFile,
        void * lpBuffer,
        uint32_t nNumberOfBytesToRead,
        uint32_t * lpNumberOfBytesRead,
        LPOVERLAPPED lpOverlapped
        );

    PAL_ERROR
    InternalSetEndOfFile(
        CPalThread *pThread,
        HANDLE hFile
        );

    PAL_ERROR
    InternalGetFileSize(
        CPalThread *pThread,
        HANDLE hFile,
        uint32_t *pdwFileSizeLow,
        uint32_t *pdwFileSizeHigh
        );

    PAL_ERROR
    InternalFlushFileBuffers(
        CPalThread *pThread,
        HANDLE hFile
        );

    PAL_ERROR
    InternalGetFileType(
        CPalThread *pThread,
        HANDLE hFile,
        uint32_t *pdwFileType
        );

    PAL_ERROR
    InternalCreatePipe(
        CPalThread *pThread,
        HANDLE *phReadPipe,
        HANDLE *phWritePipe,
        LPSECURITY_ATTRIBUTES lpPipeAttributes,
        uint32_t nSize
        );

    PAL_ERROR
    InternalLockFile(
        CPalThread *pThread,
        HANDLE hFile,
        uint32_t dwFileOffsetLow,
        uint32_t dwFileOffsetHigh,
        uint32_t nNumberOfBytesToLockLow,
        uint32_t nNumberOfBytesToLockHigh
        );

    PAL_ERROR
    InternalUnlockFile(
        CPalThread *pThread,
        HANDLE hFile,
        uint32_t dwFileOffsetLow,
        uint32_t dwFileOffsetHigh,
        uint32_t nNumberOfBytesToUnlockLow,
        uint32_t nNumberOfBytesToUnlockHigh
        );

    PAL_ERROR
    InternalSetFilePointer(
        CPalThread *pThread,
        HANDLE hFile,
        int32_t lDistanceToMove,
        int32_t * lpDistanceToMoveHigh,
        uint32_t dwMoveMethod,
        int32_t * lpNewFilePointerLow
        );

    PAL_ERROR
    InternalSetFileTime(
        CPalThread *pThread,
         HANDLE hFile,
         const FILETIME *lpCreationTime,
         const FILETIME *lpLastAccessTime,
         const FILETIME *lpLastWriteTime
        );

    PAL_ERROR
    InternalGetFileTime(
        CPalThread *pThread,
         HANDLE hFile,
         LPFILETIME lpCreationTime,
         LPFILETIME lpLastAccessTime,
         LPFILETIME lpLastWriteTime
        );

    /*++
      InternalCanonicalizeRealPath
      Wraps realpath() to hide platform differences. See the man page for
      realpath(3) for details of how realpath() works.

      On systems on which realpath() allows the last path component to not
      exist, this is a straight thunk through to realpath(). On other
      systems, we remove the last path component, then call realpath().

      cch is the size of lpBuffer and has to be atleast PATH_MAX (since 
      realpath() requires the buffer to be atleast PATH_MAX).
      --*/
    PAL_ERROR
    InternalCanonicalizeRealPath(
        LPCSTR lpUnixPath,
        LPSTR lpBuffer,
        uint32_t cch
        );

    /*++
    InternalMkstemp
    Wraps mkstemp
    --*/
    int 
    InternalMkstemp(
        char *szNameTemplate
        );

    /*++
    InternalFgets
    Wraps fgets
    --*/
    char *
    InternalFgets(
        char *sz,
        int nSize,
        FILE *f,
        bool fTextMode
        );

    /*++
    InternalFwrite
    Wraps fwrite
    --*/
    size_t
    InternalFwrite(
        const void *pvBuffer,
        size_t nSize,
        size_t nCount,
        FILE *f,
        int32_t *pnErrorCode
        );

    /*++
    InternalOpen
    Wraps open
    --*/
    int
    InternalOpen(
        const char *szFilename,
        int nFlags,
        ...
        );
}

extern "C"
{

//
// These routines should all be separated out into something along the lines
// of fileutils.* (instead of being commingled with the core file object
// code).
//

/*++
FILECanonicalizePath
    Removes all instances of '/./', '/../' and '//' from an absolute path. 
    
Parameters:
    LPSTR lpUnixPath : absolute path to modify, in Unix format

(no return value)                                             
 
Notes :
-behavior is undefined if path is not absolute
-the order of steps *is* important: /one/./../two would give /one/two 
 instead of /two if step 3 was done before step 2
-reason for this function is that GetFullPathName can't use realpath(), since 
 realpath() requires the given path to be valid and GetFullPathName does not.
--*/
void FILECanonicalizePath(LPSTR lpUnixPath);

/*++
Function:
  FileDosToUnixPathA

Abstract:
  Change a DOS path to a Unix path. Replace '\' by '/'.

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void 
FILEDosToUnixPathA(LPSTR lpPath);

/*++
Function:
  FileDosToUnixPathW

Abstract:
  Change a DOS path to a Unix path. Replace '\' by '/'.

Parameter:
  IN/OUT lpPath: path to be modified
  --*/
void
FILEDosToUnixPathW(LPWSTR lpPath);

/*++
Function:
  FileUnixToDosPathA

Abstract:
  Change a Unix path to a DOS path. Replace '/' by '\'.

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void 
FILEUnixToDosPathA(LPSTR lpPath);


/*++
Function:
  FILEGetDirectoryFromFullPathA

Parse the given path. If it contains a directory part and a file part,
put the directory part into the supplied buffer, and return the number of
characters written to the buffer. If the buffer is not large enough,
return the required size of the buffer including the NULL character. If
there is no directory part in the path, return 0.
--*/
uint32_t FILEGetDirectoryFromFullPathA( LPCSTR lpFullPath,
                     uint32_t  nBufferLength,
                     LPSTR  lpBuffer );

/*++
Function:
  FILEGetFileNameFromFullPath

Given a full path, return a pointer to the first char of the filename part.
--*/
LPCSTR FILEGetFileNameFromFullPathA( LPCSTR lpFullPath );

/*++
Function:
  FILEGetLastErrorFromErrno
  
Convert errno into the appropriate win32 error and return it.
--*/
uint32_t FILEGetLastErrorFromErrno( void );

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

    IN LPSTR lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError( LPSTR lpPath, uint32_t * lpErrorCode );

}

#endif /* _PAL_FILE_HPP_ */

