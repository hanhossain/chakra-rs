//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    file.cpp

Abstract:

    Implementation of the file WIN API for the PAL



--*/

#include <new>
#include <string>
#include "pal/file.hpp"
#include "pal/stackstring.hpp"
#include "pal/thread.hpp"
#include "shmfilelockmgr.hpp"

#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/filetime.h"
#include "pal/palinternal.h"
#include "pal/utils.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(FILE);

int MaxWCharToAcpLengthFactor = 3;

PAL_ERROR
InternalSetFilePointerForUnixFd(int iUnixFd, int32_t lDistanceToMove, int32_t *lpDistanceToMoveHigh,
                                uint32_t dwMoveMethod, int32_t *lpNewFilePointerLow);

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool);

CObjectType CorUnix::otFile
    __attribute__((init_priority(200))) (otiFile, FileCleanupRoutine,
                                         NULL, // No initialization routine
                                         0, // No immutable data
                                         sizeof(CFileProcessLocalData),
                                         0, // No shared data
                                         GENERIC_READ | GENERIC_WRITE, // Ignored -- no Win32 object security support
                                         CObjectType::SecuritySupported, CObjectType::OSPersistedSecurityInfo,
                                         CObjectType::UnnamedObject, CObjectType::LocalDuplicationOnly,
                                         CObjectType::UnwaitableObject, CObjectType::SignalingNotApplicable,
                                         CObjectType::ThreadReleaseNotApplicable, CObjectType::OwnershipNotApplicable);

CAllowedObjectTypes CorUnix::aotFile __attribute__((init_priority(200))) (otiFile);

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool)
{
    PAL_ERROR palError;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    palError = pObjectToCleanup->GetProcessLocalData(pThread, ReadLock, &pLocalDataLock,
                                                     reinterpret_cast<void **>(&pLocalData));

    if (NO_ERROR != palError)
    {
        ASSERT("Unable to obtain data to cleanup file object");
        return;
    }

    if (pLocalData->pLockController != NULL)
    {
        pLocalData->pLockController->ReleaseController();
    }

    if (!fShutdown && -1 != pLocalData->unix_fd)
    {
        close(pLocalData->unix_fd);
    }

    pLocalDataLock->ReleaseLock(pThread);
}

typedef enum
{
    PIID_STDIN_HANDLE,
    PIID_STDOUT_HANDLE,
    PIID_STDERR_HANDLE
} PROCINFO_ID;

#define PAL_LEGAL_FLAGS_ATTRIBS                                                                                        \
    (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING |            \
     FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_BACKUP_SEMANTICS)

/* Static global. The init function must be called
before any other functions and if it is not successful,
no other functions should be done. */
static HANDLE pStdIn;
static HANDLE pStdOut;
static HANDLE pStdErr;

/*++
Function :
    FILEGetProperNotFoundError

Returns the proper error code, based on the
Windows behavior.

    IN char* lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError(char *lpPath, uint32_t *lpErrorCode)
{
    struct stat stat_data;
    char *lpDupedPath = NULL;
    char *lpLastPathSeparator = NULL;

    TRACE("FILEGetProperNotFoundError( %s )\n", lpPath ? lpPath : "(null)");

    if (!lpErrorCode)
    {
        ASSERT("lpErrorCode has to be valid\n");
        return;
    }

    if (NULL == (lpDupedPath = strdup(lpPath)))
    {
        ERROR("strdup() failed!\n");
        *lpErrorCode = ERROR_NOT_ENOUGH_MEMORY;
        return;
    }

    /* Determine whether it's a file not found or path not found. */
    lpLastPathSeparator = strrchr(lpDupedPath, '/');
    if (lpLastPathSeparator != NULL)
    {
        *lpLastPathSeparator = '\0';

        /* If the last path component is a directory,
           we return file not found. If it's a file or
           doesn't exist, we return path not found. */
        if ('\0' == *lpDupedPath || (stat(lpDupedPath, &stat_data) == 0 && (stat_data.st_mode & S_IFMT) == S_IFDIR))
        {
            TRACE("ERROR_FILE_NOT_FOUND\n");
            *lpErrorCode = ERROR_FILE_NOT_FOUND;
        }
        else
        {
            TRACE("ERROR_PATH_NOT_FOUND\n");
            *lpErrorCode = ERROR_PATH_NOT_FOUND;
        }
    }
    else
    {
        TRACE("ERROR_FILE_NOT_FOUND\n");
        *lpErrorCode = ERROR_FILE_NOT_FOUND;
    }

    free(lpDupedPath);
    lpDupedPath = NULL;
    TRACE("FILEGetProperNotFoundError returning TRUE\n");
    return;
}

/*++
Function :
    FILEGetLastErrorFromErrnoAndFilename

Returns the proper error code for errno, or, if errno is ENOENT,
based on the Windows behavior for nonexistent filenames.

    IN char* lpPath - The path to check.
*/
PAL_ERROR FILEGetLastErrorFromErrnoAndFilename(char *lpPath)
{
    PAL_ERROR palError;
    if (ENOENT == errno)
    {
        FILEGetProperNotFoundError(lpPath, &palError);
    }
    else
    {
        palError = FILEGetLastErrorFromErrno();
    }
    return palError;
}


PAL_ERROR
CorUnix::InternalWriteFile(CPalThread *pThread, HANDLE hFile, const void *lpBuffer, uint32_t nNumberOfBytesToWrite,
                           uint32_t *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    PAL_ERROR palError = 0;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;
    IFileTransactionLock *pTransactionLock = NULL;
    int ifd;

    int32_t writeOffsetStartLow = 0, writeOffsetStartHigh = 0;
    int res;

    if (NULL != lpNumberOfBytesWritten)
    {
        //
        // This must be set to 0 before any other error checking takes
        // place, per MSDN
        //

        *lpNumberOfBytesWritten = 0;
    }
    else
    {
        ASSERT("lpNumberOfBytesWritten is NULL\n");
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    // Win32 WriteFile disallows writing to STD_INPUT_HANDLE
    if (hFile == INVALID_HANDLE_VALUE || hFile == pStdIn)
    {
        palError = ERROR_INVALID_HANDLE;
        goto done;
    }
    else if (lpOverlapped)
    {
        ASSERT("lpOverlapped is not NULL, as it should be.\n");
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(pThread, hFile, &aotFile, &pFileObject);

    if (NO_ERROR != palError)
    {
        goto done;
    }

    palError =
        pFileObject->GetProcessLocalData(pThread, ReadLock, &pLocalDataLock, reinterpret_cast<void **>(&pLocalData));

    if (NO_ERROR != palError)
    {
        goto done;
    }

    if (pLocalData->open_flags_deviceaccessonly == TRUE)
    {
        ERROR("File open for device access only\n");
        palError = ERROR_ACCESS_DENIED;
        goto done;
    }

    ifd = pLocalData->unix_fd;

    //
    // Inform the lock controller for this file (if any) of our intention
    // to perform a write. (Note that pipes don't have lock controllers.)
    //

    if (NULL != pLocalData->pLockController)
    {
        /* Get the current file position to calculate the region to lock */
        palError = InternalSetFilePointerForUnixFd(ifd, 0, &writeOffsetStartHigh, FILE_CURRENT, &writeOffsetStartLow);

        if (NO_ERROR != palError)
        {
            ASSERT("Failed to get the current file position\n");
            palError = ERROR_INTERNAL_ERROR;
            goto done;
        }

        palError = pLocalData->pLockController->GetTransactionLock(writeOffsetStartLow, writeOffsetStartHigh,
                                                                   nNumberOfBytesToWrite, 0, &pTransactionLock);

        if (NO_ERROR != palError)
        {
            ERROR("Unable to obtain write transaction lock");
            goto done;
        }
    }

    //
    // Release the data lock before performing the (possibly blocking)
    // write call
    //

    pLocalDataLock->ReleaseLock(pThread);
    pLocalDataLock = NULL;
    pLocalData = NULL;

    res = write(ifd, lpBuffer, nNumberOfBytesToWrite);
    TRACE("write() returns %d\n", res);

    if (res >= 0)
    {
        *lpNumberOfBytesWritten = res;
    }
    else
    {
        palError = FILEGetLastErrorFromErrno();
    }

done:

    if (NULL != pTransactionLock)
    {
        pTransactionLock->ReleaseLock();
    }

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}


/*++
Function:
  WriteFileW

Note:
  lpOverlapped always NULL.

See MSDN doc.
--*/
BOOL WriteFile(HANDLE hFile, const void *lpBuffer, uint32_t nNumberOfBytesToWrite, uint32_t *lpNumberOfBytesWritten,
               LPOVERLAPPED lpOverlapped)
{
    PAL_ERROR palError;
    CPalThread *pThread;

    pThread = InternalGetCurrentThread();

    palError = InternalWriteFile(pThread, hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("WriteFile returns BOOL %d\n", NO_ERROR == palError);
    return NO_ERROR == palError;
}

/*++
Function:
  GetStdHandle

See MSDN doc.
--*/
HANDLE
GetStdHandle(uint32_t nStdHandle)
{
    CPalThread *pThread;
    HANDLE hRet = INVALID_HANDLE_VALUE;

    pThread = InternalGetCurrentThread();
    switch (nStdHandle)
    {
    case STD_INPUT_HANDLE:
        hRet = pStdIn;
        break;
    case STD_OUTPUT_HANDLE:
        hRet = pStdOut;
        break;
    case STD_ERROR_HANDLE:
        hRet = pStdErr;
        break;
    default:
        ERROR("nStdHandle is invalid\n");
        pThread->SetLastError(ERROR_INVALID_PARAMETER);
        break;
    }

    LOGEXIT("GetStdHandle returns HANDLE %p\n", hRet);
    return hRet;
}


//
// We need to break out the actual mechanics of setting the file pointer
// on the unix FD for InternalReadFile and InternalWriteFile, as they
// need to call this routine in order to determine the value of the
// current file pointer when computing the scope of their transaction
// lock. If we didn't break out this logic we'd end up referencing the file
// handle multiple times, and, in the process, would attempt to recursively
// obtain the local process data lock for the underlying file object.
//

PAL_ERROR
InternalSetFilePointerForUnixFd(int iUnixFd, int32_t lDistanceToMove, int32_t *lpDistanceToMoveHigh,
                                uint32_t dwMoveMethod, int32_t *lpNewFilePointerLow)
{
    PAL_ERROR palError = NO_ERROR;
    int seek_whence = 0;
    long seek_offset = 0LL;
    long seek_res = 0LL;
    off_t old_offset;

    switch (dwMoveMethod)
    {
    case FILE_BEGIN:
        seek_whence = SEEK_SET;
        break;
    case FILE_CURRENT:
        seek_whence = SEEK_CUR;
        break;
    case FILE_END:
        seek_whence = SEEK_END;
        break;
    default:
        ERROR("dwMoveMethod = %d is invalid\n", dwMoveMethod);
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    //
    // According to MSDN, if lpDistanceToMoveHigh is not null,
    // lDistanceToMove is treated as unsigned;
    // it is treated as signed otherwise
    //

    if (lpDistanceToMoveHigh)
    {
        /* set the high 32 bits of the offset */
        seek_offset = (static_cast<long>(*lpDistanceToMoveHigh) << 32);

        /* set the low 32 bits */
        /* cast to unsigned long to avoid sign extension */
        seek_offset |= static_cast<uint32_t>(lDistanceToMove);
    }
    else
    {
        seek_offset |= lDistanceToMove;
    }

    /* store the current position, in case the lseek moves the pointer
       before the beginning of the file */
    old_offset = lseek(iUnixFd, 0, SEEK_CUR);
    if (old_offset == -1)
    {
        ERROR("lseek(fd,0,SEEK_CUR) failed errno:%d (%s)\n", errno, strerror(errno));
        palError = ERROR_ACCESS_DENIED;
        goto done;
    }

    // Check to see if we're going to seek to a negative offset.
    // If we're seeking from the beginning or the current mark,
    // this is simple.
    if ((seek_whence == SEEK_SET && seek_offset < 0) || (seek_whence == SEEK_CUR && seek_offset + old_offset < 0))
    {
        palError = ERROR_NEGATIVE_SEEK;
        goto done;
    }
    else if (seek_whence == SEEK_END && seek_offset < 0)
    {
        // We need to determine if we're seeking past the
        // beginning of the file, but we don't want to adjust
        // the mark in the process. stat is the only way to
        // do that.
        struct stat fileData;
        int result;

        result = fstat(iUnixFd, &fileData);
        if (result == -1)
        {
            // It's a bad fd. This shouldn't happen because
            // we've already called lseek on it, but you
            // never know. This is the best we can do.
            palError = ERROR_ACCESS_DENIED;
            goto done;
        }
        if (fileData.st_size < -seek_offset)
        {
            // Seeking past the beginning.
            palError = ERROR_NEGATIVE_SEEK;
            goto done;
        }
    }

    seek_res = static_cast<long>(lseek(iUnixFd, seek_offset, seek_whence));
    if (seek_res < 0)
    {
        /* lseek() returns -1 on error, but also can seek to negative
           file offsets, so -1 can also indicate a successful seek to offset
           -1.  Win32 doesn't allow negative file offsets, so either case
           is an error. */
        ERROR("lseek failed errno:%d (%s)\n", errno, strerror(errno));
        lseek(iUnixFd, old_offset, SEEK_SET);
        palError = ERROR_ACCESS_DENIED;
    }
    else
    {
        /* store high-order uint32_t */
        if (lpDistanceToMoveHigh)
            *lpDistanceToMoveHigh = static_cast<uint32_t>(seek_res >> 32);

        /* return low-order uint32_t of seek result */
        *lpNewFilePointerLow = static_cast<uint32_t>(seek_res);
    }

done:

    return palError;
}


#define ENSURE_UNIQUE_NOT_ZERO                                                                                         \
    if (uUniqueSeed == 0)                                                                                              \
    {                                                                                                                  \
        uUniqueSeed++;                                                                                                 \
    }

/*++
Function:
  FILEGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t FILEGetLastErrorFromErrno(void)
{
    uint32_t dwRet;

    switch (errno)
    {
    case 0:
        dwRet = ERROR_SUCCESS;
        break;
    case ENAMETOOLONG:
        dwRet = ERROR_FILENAME_EXCED_RANGE;
        break;
    case ENOTDIR:
        dwRet = ERROR_PATH_NOT_FOUND;
        break;
    case ENOENT:
        dwRet = ERROR_FILE_NOT_FOUND;
        break;
    case EACCES:
    case EPERM:
    case EROFS:
    case EISDIR:
        dwRet = ERROR_ACCESS_DENIED;
        break;
    case EEXIST:
        dwRet = ERROR_ALREADY_EXISTS;
        break;
#if !defined(_AIX)
    // ENOTEMPTY is the same as EEXIST on AIX. Meaningful when involving directory operations
    case ENOTEMPTY:
        dwRet = ERROR_DIR_NOT_EMPTY;
        break;
#endif
    case EBADF:
        dwRet = ERROR_INVALID_HANDLE;
        break;
    case ENOMEM:
        dwRet = ERROR_NOT_ENOUGH_MEMORY;
        break;
    case EBUSY:
        dwRet = ERROR_BUSY;
        break;
    case ENOSPC:
    case EDQUOT:
        dwRet = ERROR_DISK_FULL;
        break;
    case ELOOP:
        dwRet = ERROR_BAD_PATHNAME;
        break;
    case EIO:
        dwRet = ERROR_WRITE_FAULT;
        break;
    case ERANGE:
        dwRet = ERROR_BAD_PATHNAME;
        break;
    default:
        ERROR("unexpected errno %d (%s); returning ERROR_GEN_FAILURE\n", errno, strerror(errno));
        dwRet = ERROR_GEN_FAILURE;
    }

    TRACE("errno = %d (%s), LastError = %d\n", errno, strerror(errno), dwRet);

    return dwRet;
}

/*++
Function:
  DIRGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t DIRGetLastErrorFromErrno(void)
{
    if (errno == ENOENT)
        return ERROR_PATH_NOT_FOUND;
    else
        return FILEGetLastErrorFromErrno();
}


/*++
init_std_handle [static]

utility function for FILEInitStdHandles. do the work that is common to all
three standard handles

Parameters:
    HANDLE pStd : Defines which standard handle to assign
    FILE *stream        : file stream to associate to handle

Return value:
    handle for specified stream, or INVALID_HANDLE_VALUE on failure
--*/
static HANDLE init_std_handle(HANDLE *pStd, FILE *stream)
{
    CPalThread *pThread = InternalGetCurrentThread();
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    IPalObject *pRegisteredFile = NULL;
    IDataLock *pDataLock = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IFileLockController *pLockController = NULL;
    CObjectAttributes oa;

    HANDLE hFile = INVALID_HANDLE_VALUE;
    int new_fd = -1;

    /* duplicate the FILE *, so that we can fclose() in FILECloseHandle without
       closing the original */
    new_fd = dup(fileno(stream));
    if (-1 == new_fd)
    {
        ERROR("dup() failed; errno is %d (%s)\n", errno, strerror(errno));
        goto done;
    }

    palError = g_pObjectManager->AllocateObject(pThread, &otFile, &oa, &pFileObject);

    if (NO_ERROR != palError)
    {
        goto done;
    }

    palError = pFileObject->GetProcessLocalData(pThread, WriteLock, &pDataLock, reinterpret_cast<void **>(&pLocalData));

    if (NO_ERROR != palError)
    {
        goto done;
    }

    pLocalData->inheritable = TRUE;
    pLocalData->unix_fd = new_fd;
    pLocalData->dwDesiredAccess = 0;
    pLocalData->open_flags = 0;
    pLocalData->open_flags_deviceaccessonly = FALSE;

    //
    // Transfer the lock controller reference from our local variable
    // to the local file data
    //

    pLocalData->pLockController = pLockController;
    pLockController = NULL;

    //
    // We've finished initializing our local data, so release that lock
    //

    pDataLock->ReleaseLock(pThread);
    pDataLock = NULL;

    palError = g_pObjectManager->RegisterObject(pThread, pFileObject, &aotFile, 0, &hFile, &pRegisteredFile);

    //
    // pFileObject is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line.
    //

    pFileObject = NULL;

done:

    if (NULL != pLockController)
    {
        pLockController->ReleaseController();
    }

    if (NULL != pDataLock)
    {
        pDataLock->ReleaseLock(pThread);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    if (NULL != pRegisteredFile)
    {
        pRegisteredFile->ReleaseReference(pThread);
    }

    if (NO_ERROR == palError)
    {
        *pStd = hFile;
    }
    else if (-1 != new_fd)
    {
        close(new_fd);
    }

    return hFile;
}


/*++
FILEInitStdHandles

Create handle objects for stdin, stdout and stderr

(no parameters)

Return value:
    TRUE on success, FALSE on failure
--*/
BOOL FILEInitStdHandles(void)
{
    HANDLE stdin_handle;
    HANDLE stdout_handle;
    HANDLE stderr_handle;

    TRACE("creating handle objects for stdin, stdout, stderr\n");

    stdin_handle = init_std_handle(&pStdIn, stdin);
    if (INVALID_HANDLE_VALUE == stdin_handle)
    {
        ERROR("failed to create stdin handle\n");
        goto fail;
    }

    stdout_handle = init_std_handle(&pStdOut, stdout);
    if (INVALID_HANDLE_VALUE == stdout_handle)
    {
        ERROR("failed to create stdout handle\n");
        CloseHandle(stdin_handle);
        goto fail;
    }

    stderr_handle = init_std_handle(&pStdErr, stderr);
    if (INVALID_HANDLE_VALUE == stderr_handle)
    {
        ERROR("failed to create stderr handle\n");
        CloseHandle(stdin_handle);
        CloseHandle(stdout_handle);
        goto fail;
    }
    return TRUE;

fail:
    pStdIn = INVALID_HANDLE_VALUE;
    pStdOut = INVALID_HANDLE_VALUE;
    pStdErr = INVALID_HANDLE_VALUE;
    return FALSE;
}


/*++
FILECleanupStdHandles

Remove all regions, locked by a file pointer, from shared memory

(no parameters)

--*/
void FILECleanupStdHandles(void)
{
    HANDLE stdin_handle;
    HANDLE stdout_handle;
    HANDLE stderr_handle;

    TRACE("closing standard handles\n");
    stdin_handle = pStdIn;
    stdout_handle = pStdOut;
    stderr_handle = pStdErr;
    pStdIn = INVALID_HANDLE_VALUE;
    pStdOut = INVALID_HANDLE_VALUE;
    pStdErr = INVALID_HANDLE_VALUE;
    CloseHandle(stdin_handle);
    CloseHandle(stdout_handle);
    CloseHandle(stderr_handle);
}
