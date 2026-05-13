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

#include <string>
#include "pal/thread.hpp"
#include "pal/file.hpp"
#include "shmfilelockmgr.hpp"
#include <new>
#include "pal/stackstring.hpp"

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/filetime.h"
#include "pal/utils.h"

#include <time.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <errno.h>
#include <limits.h>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(FILE);

int MaxWCharToAcpLengthFactor = 3;

PAL_ERROR
InternalSetFilePointerForUnixFd(
    int iUnixFd,
    int32_t lDistanceToMove,
    int32_t * lpDistanceToMoveHigh,
    uint32_t dwMoveMethod,
    int32_t * lpNewFilePointerLow
    );

void
FileCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    );

CObjectType CorUnix::otFile __attribute__((init_priority(200))) (
                otiFile,
                FileCleanupRoutine,
                NULL,   // No initialization routine
                0,      // No immutable data
                sizeof(CFileProcessLocalData),
                0,      // No shared data
                GENERIC_READ|GENERIC_WRITE,  // Ignored -- no Win32 object security support
                CObjectType::SecuritySupported,
                CObjectType::OSPersistedSecurityInfo,
                CObjectType::UnnamedObject,
                CObjectType::LocalDuplicationOnly,
                CObjectType::UnwaitableObject,
                CObjectType::SignalingNotApplicable,
                CObjectType::ThreadReleaseNotApplicable,
                CObjectType::OwnershipNotApplicable
                );

CAllowedObjectTypes CorUnix::aotFile __attribute__((init_priority(200))) (otiFile);
static CSharedMemoryFileLockMgr _FileLockManager __attribute__((init_priority(200)));
IFileLockManager *CorUnix::g_pFileLockManager = &_FileLockManager;

void
FileCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    )
{
    PAL_ERROR palError;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    palError = pObjectToCleanup->GetProcessLocalData(
        pThread, 
        ReadLock,
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

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

    pLocalDataLock->ReleaseLock(pThread, FALSE);
}

typedef enum
{
  PIID_STDIN_HANDLE, 
  PIID_STDOUT_HANDLE,
  PIID_STDERR_HANDLE
} PROCINFO_ID; 

#define PAL_LEGAL_FLAGS_ATTRIBS (FILE_ATTRIBUTE_NORMAL| \
                                 FILE_FLAG_SEQUENTIAL_SCAN| \
                                 FILE_FLAG_WRITE_THROUGH| \
                                 FILE_FLAG_NO_BUFFERING| \
                                 FILE_FLAG_RANDOM_ACCESS| \
                                 FILE_FLAG_BACKUP_SEMANTICS)

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
void FILEGetProperNotFoundError( char* lpPath, uint32_t * lpErrorCode )
{
    struct stat stat_data;
    char* lpDupedPath = NULL;
    char* lpLastPathSeparator = NULL;

    TRACE( "FILEGetProperNotFoundError( %s )\n", lpPath?lpPath:"(null)" );

    if ( !lpErrorCode )
    {
        ASSERT( "lpErrorCode has to be valid\n" );
        return;
    }

    if ( NULL == ( lpDupedPath = strdup( lpPath ) ) )
    {
        ERROR( "strdup() failed!\n" );
        *lpErrorCode = ERROR_NOT_ENOUGH_MEMORY;
        return;
    }

    /* Determine whether it's a file not found or path not found. */
    lpLastPathSeparator = strrchr( lpDupedPath, '/');
    if ( lpLastPathSeparator != NULL )
    {
        *lpLastPathSeparator = '\0';
        
        /* If the last path component is a directory,
           we return file not found. If it's a file or
           doesn't exist, we return path not found. */
        if ( '\0' == *lpDupedPath || 
             ( stat( lpDupedPath, &stat_data ) == 0 && 
             ( stat_data.st_mode & S_IFMT ) == S_IFDIR ) )
        {
            TRACE( "ERROR_FILE_NOT_FOUND\n" );
            *lpErrorCode = ERROR_FILE_NOT_FOUND;
        }
        else
        {
            TRACE( "ERROR_PATH_NOT_FOUND\n" );
            *lpErrorCode = ERROR_PATH_NOT_FOUND;
        }
    }
    else
    {
        TRACE( "ERROR_FILE_NOT_FOUND\n" );
        *lpErrorCode = ERROR_FILE_NOT_FOUND;
    }
    
    free(lpDupedPath);
    lpDupedPath = NULL;
    TRACE( "FILEGetProperNotFoundError returning TRUE\n" );
    return;
}

/*++
Function : 
    FILEGetLastErrorFromErrnoAndFilename
    
Returns the proper error code for errno, or, if errno is ENOENT,
based on the Windows behavior for nonexistent filenames.

    IN char* lpPath - The path to check.
*/
PAL_ERROR FILEGetLastErrorFromErrnoAndFilename(char* lpPath)
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

/*++
InternalCanonicalizeRealPath
    Wraps realpath() to hide platform differences. See the man page for
    realpath(3) for details of how realpath() works.
    
    On systems on which realpath() allows the last path component to not
    exist, this is a straight thunk through to realpath(). On other
    systems, we remove the last path component, then call realpath().

    cch is the size of lpBuffer and has to be at least PATH_MAX (since 
    realpath() requires the buffer to be at least PATH_MAX).
--*/
PAL_ERROR
CorUnix::InternalCanonicalizeRealPath(const char * lpUnixPath, char* lpBuffer, uint32_t cch)
{
    PAL_ERROR palError = NO_ERROR;
    char* lpRealPath = NULL;

    char* lpExistingPath = NULL;
    char* pchSeparator = NULL;
    char* lpFilename = NULL;
    uint32_t cchBuffer = 0;
    uint32_t cchFilename = 0;

    if ( (lpUnixPath == NULL) || (lpBuffer == NULL) || (cch < PATH_MAX) )
    {
        ERROR ("Invalid argument to InternalCanonicalizeRealPath\n");
        palError = ERROR_INVALID_PARAMETER;
        goto LExitDontFree;
    }

    lpExistingPath = strdup(lpUnixPath);
    if (lpExistingPath == NULL)
    {
        ERROR ("strdup failed with error %d\n", errno);
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto LExit;
    }

    pchSeparator = strrchr(lpExistingPath, '/');
    if (pchSeparator == NULL)
    {
        char pszCwdBuffer[MAXPATHLEN+1]; // MAXPATHLEN is for getcwd()
        uint32_t cchCwdBuffer = sizeof(pszCwdBuffer)/sizeof(pszCwdBuffer[0]);

        if (getcwd(pszCwdBuffer, cchCwdBuffer) == NULL)
        {
            WARN("getcwd(NULL) failed with error %d\n", errno);
            palError = DIRGetLastErrorFromErrno();
            goto LExit;
        }

        lpRealPath = realpath(pszCwdBuffer, lpBuffer);
        if (lpRealPath == NULL)
        {
            WARN("realpath() failed with error %d\n", errno);
            palError = FILEGetLastErrorFromErrno();
#if defined(_AMD64_)
            // If we are here, then we tried to invoke realpath
            // against a directory.
            //
            // On Mac64, realpath implementation differs from Mac32
            // by *not* supporting invalid filenames in the path (while
            // Mac32 implementation does).
            //
            // Thus, if we are here, and the error code we see is
            // ERROR_FILE_NOT_FOUND, then we should map it to
            // ERROR_PATH_NOT_FOUND since it was a directory that
            // was not found (and not a file).
            if (palError == ERROR_FILE_NOT_FOUND)
            {
                // Since lpBuffer can be modified by the realpath call,
                // and can result in a truncated subset of the original buffer,
                // we use strstr as a level of safety.
                 if (strstr(pszCwdBuffer, lpBuffer) != 0)
                 {
                     palError = ERROR_PATH_NOT_FOUND;
                 }
            }
#endif // defined(_AMD64_)
            
            goto LExit;
        }
        lpFilename = lpExistingPath;
    }
    else
    {
#if defined(_AMD64_)
        bool fSetFilename = true;
        // Since realpath implementation cannot handle inexistent filenames,
        // check if we are going to truncate the "/" corresponding to the
        // root folder (e.g. case of "/Volumes"). If so:
        //
        // 1) Set the seperator to point to the NULL terminator of the specified
        //    file/folder name.
        //
        // 2) Null terminate lpBuffer
        //
        // 3) Since there is no explicit filename component in lpExistingPath (as
        //    we only have "/" corresponding to the root), set lpFilename to NULL,
        //    alongwith a flag indicating that it has already been set.
        if (pchSeparator == lpExistingPath)
        {
            pchSeparator = lpExistingPath+strlen(lpExistingPath);

            // Set the lpBuffer to NULL
            lpBuffer[0] = '\0';
            lpFilename = NULL;
            fSetFilename = false;
        }
        else
#endif // defined(_AMD64_)
            *pchSeparator = '\0';
            
        lpRealPath = realpath(lpExistingPath, lpBuffer);
        if (lpRealPath == NULL)
        {
            WARN("realpath() failed with error %d\n", errno);
            palError = FILEGetLastErrorFromErrno();

#if defined(_AMD64_)
            // If we are here, then we tried to invoke realpath
            // against a directory after stripping out the filename
            // from the original path.
            //
            // On Mac64, realpath implementation differs from Mac32
            // by *not* supporting invalid filenames in the path (while
            // Mac32 implementation does).
            //
            // Thus, if we are here, and the error code we see is
            // ERROR_FILE_NOT_FOUND, then we should map it to
            // ERROR_PATH_NOT_FOUND since it was a directory that
            // was not found (and not a file).
            if (palError == ERROR_FILE_NOT_FOUND)
            {
                // Since lpBuffer can be modified by the realpath call,
                // and can result in a truncated subset of the original buffer,
                // we use strstr as a level of safety.
                if (strstr(lpExistingPath, lpBuffer) != 0)
                 {
                     palError = ERROR_PATH_NOT_FOUND;
                 }
            }
#endif // defined(_AMD64_)

            goto LExit;
        }

#if defined(_AMD64_)
        if (fSetFilename == true)
#endif // defined(_AMD64_)
            lpFilename = pchSeparator + 1;
    }

    cchBuffer = strlen(lpBuffer);

#if defined(_AMD64_)
    if (lpFilename == NULL)
        cchFilename = 0;
    else
#endif // _AMD64_
        cchFilename = strlen(lpFilename);

    // Append the file name.
    // 2 is for "/" and NULL
    if (cchBuffer + cchFilename + 2 > cch)
    {
        ERROR ("Provided buffer size (%d) is smaller than the required (%d)\n", 
                cch, cchBuffer + cchFilename + 2);
        palError = ERROR_INSUFFICIENT_BUFFER;
        lpRealPath = NULL;
        goto LExit;
    }

    lpBuffer[cchBuffer] = '/';
    lpBuffer[cchBuffer+1] = '\0';

#if defined(_AMD64_)
    if (lpFilename == NULL)
        goto LExit;
#endif // _AMD64_

    if (strcat_s(lpBuffer, cch, lpFilename) != SAFECRT_SUCCESS)
    {
        ERROR ("strcat_s failed!\n");
        palError = ERROR_INSUFFICIENT_BUFFER;
        lpRealPath = NULL;

        // Doing a goto here since we want to exit now. This will work
        // incase someone else adds another if clause below us.
        goto LExit;
    }

LExit:
    if (lpExistingPath != NULL)
    {
        free(lpExistingPath);
    }

LExitDontFree:
    if ((palError == NO_ERROR) && (lpRealPath == NULL))
    {
        // convert all these into ERROR_PATH_NOT_FOUND
        palError = ERROR_PATH_NOT_FOUND;
    }

    return palError;
}


PAL_ERROR
CorUnix::InternalWriteFile(
    CPalThread *pThread,
    HANDLE hFile,
    const void * lpBuffer,
    uint32_t nNumberOfBytesToWrite,
    uint32_t * lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    )
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
        ASSERT( "lpNumberOfBytesWritten is NULL\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    // Win32 WriteFile disallows writing to STD_INPUT_HANDLE
    if (hFile == INVALID_HANDLE_VALUE || hFile == pStdIn)
    {
        palError = ERROR_INVALID_HANDLE;
        goto done;
    }
    else if ( lpOverlapped )
    {
        ASSERT( "lpOverlapped is not NULL, as it should be.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_WRITE,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto done;
    }
    
    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

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
        palError = InternalSetFilePointerForUnixFd(
            ifd,
            0,
            &writeOffsetStartHigh,
            FILE_CURRENT,
            &writeOffsetStartLow
            );

        if (NO_ERROR != palError)
        {
            ASSERT("Failed to get the current file position\n");
            palError = ERROR_INTERNAL_ERROR;
            goto done;
        }

        palError = pLocalData->pLockController->GetTransactionLock(
            pThread,
            IFileLockController::WriteLock,
            writeOffsetStartLow,
            writeOffsetStartHigh,
            nNumberOfBytesToWrite,
            0,
            &pTransactionLock
            );

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

    pLocalDataLock->ReleaseLock(pThread, FALSE);
    pLocalDataLock = NULL;
    pLocalData = NULL;

    res = write( ifd, lpBuffer, nNumberOfBytesToWrite );
    TRACE("write() returns %d\n", res);

    if ( res >= 0 )
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
        pLocalDataLock->ReleaseLock(pThread, FALSE);
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
BOOL
WriteFile(
       HANDLE hFile,
       const void * lpBuffer,
       uint32_t nNumberOfBytesToWrite,
       uint32_t * lpNumberOfBytesWritten,
       LPOVERLAPPED lpOverlapped)
{
    PAL_ERROR palError;
    CPalThread *pThread;
    
    pThread = InternalGetCurrentThread();

    palError = InternalWriteFile(
        pThread,
        hFile,
        lpBuffer,
        nNumberOfBytesToWrite,
        lpNumberOfBytesWritten,
        lpOverlapped
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("WriteFile returns BOOL %d\n", NO_ERROR == palError);
    return NO_ERROR == palError;
}

PAL_ERROR
CorUnix::InternalReadFile(
    CPalThread *pThread,
    HANDLE hFile,
    void * lpBuffer,
    uint32_t nNumberOfBytesToRead,
    uint32_t * lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
    )
{
    PAL_ERROR palError = 0;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;
    IFileTransactionLock *pTransactionLock = NULL;
    int ifd;
    
    int32_t readOffsetStartLow = 0, readOffsetStartHigh = 0;
    int res;

    if (NULL != lpNumberOfBytesRead)
    {
        //
        // This must be set to 0 before any other error checking takes
        // place, per MSDN
        //

        *lpNumberOfBytesRead = 0;        
    }
    else
    {
        ERROR( "lpNumberOfBytesRead is NULL\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto done;
    }
    else if (NULL != lpOverlapped)
    {
        ASSERT( "lpOverlapped is not NULL, as it should be.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }
    else if (NULL == lpBuffer)
    {
        ERROR( "Invalid parameter. (lpBuffer:%p)\n", lpBuffer);
        palError = ERROR_NOACCESS;
        goto done;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto done;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

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
    // to perform a read. (Note that pipes don't have lock controllers.)
    //
    
    if (NULL != pLocalData->pLockController)
    {
        /* Get the current file position to calculate the region to lock */
        palError = InternalSetFilePointerForUnixFd(
            ifd,
            0,
            &readOffsetStartHigh,
            FILE_CURRENT,
            &readOffsetStartLow
            );

        if (NO_ERROR != palError)
        {
            ASSERT("Failed to get the current file position\n");
            palError = ERROR_INTERNAL_ERROR;
            goto done;
        }

        palError = pLocalData->pLockController->GetTransactionLock(
            pThread,
            IFileLockController::ReadLock,
            readOffsetStartLow,
            readOffsetStartHigh,
            nNumberOfBytesToRead,
            0,
            &pTransactionLock
            );

        if (NO_ERROR != palError)
        {
            ERROR("Unable to obtain read transaction lock");
            goto done;
        }
    }

    //
    // Release the data lock before performing the (possibly blocking)
    // read call
    //

    pLocalDataLock->ReleaseLock(pThread, FALSE);
    pLocalDataLock = NULL;
    pLocalData = NULL;

Read:
    TRACE("Reading from file descriptor %d\n", ifd);
    res = read(ifd, lpBuffer, nNumberOfBytesToRead);
    TRACE("read() returns %d\n", res);

    if (res >= 0)
    {
        *lpNumberOfBytesRead = res;
    }
    else if (errno == EINTR)
    {
        // Try to read again.
        goto Read;
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
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}

/*++
Function:
  ReadFile

Note:
  lpOverlapped always NULL.

See MSDN doc.
--*/
BOOL
ReadFile(
      HANDLE hFile,
      void * lpBuffer,
      uint32_t nNumberOfBytesToRead,
      uint32_t * lpNumberOfBytesRead,
      LPOVERLAPPED lpOverlapped)
{
    PAL_ERROR palError;
    CPalThread *pThread;
    
    pThread = InternalGetCurrentThread();

    palError = InternalReadFile(
        pThread,
        hFile,
        lpBuffer,
        nNumberOfBytesToRead,
        lpNumberOfBytesRead,
        lpOverlapped
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("ReadFile returns BOOL %d\n", NO_ERROR == palError);
    return NO_ERROR == palError;
}


/*++
Function:
  GetStdHandle

See MSDN doc.
--*/
HANDLE
GetStdHandle(
          uint32_t nStdHandle)
{
    CPalThread *pThread;
    HANDLE hRet = INVALID_HANDLE_VALUE;

    pThread = InternalGetCurrentThread();
    switch( nStdHandle )
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

PAL_ERROR
CorUnix::InternalSetEndOfFile(
    CPalThread *pThread,
    HANDLE hFile
    )
{
    PAL_ERROR palError = 0;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    off_t curr = 0;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalSetEndOfFileExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_WRITE,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalSetEndOfFileExit;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalSetEndOfFileExit;
    }

    if (pLocalData->open_flags_deviceaccessonly == TRUE)
    {
        ERROR("File open for device access only\n");
        palError = ERROR_ACCESS_DENIED;
        goto InternalSetEndOfFileExit;
    }
    
    curr = lseek(pLocalData->unix_fd, 0, SEEK_CUR);

    TRACE("current file pointer offset is %u\n", curr);
    if ( curr < 0 )
    {
        ERROR("lseek returned %ld\n", curr);
        palError = FILEGetLastErrorFromErrno();
        goto InternalSetEndOfFileExit;
    }

#if defined(__APPLE__)
    // Perform an additional check to make sure that there's likely to be enough free space to satisfy the
    // request. Do this because it's been observed on Mac OSX that ftruncate can return failure but still
    // extend the file to consume the remainder of free space.
    // 
    struct statfs sFileSystemStats;
    off_t cbFreeSpace;
    if (fstatfs(pLocalData->unix_fd, &sFileSystemStats) != 0)
    {
        ERROR("fstatfs failed\n");
        palError = FILEGetLastErrorFromErrno();
        goto InternalSetEndOfFileExit;
    } 

    // Free space is free blocks times the size of each block in bytes.
    cbFreeSpace = (off_t)sFileSystemStats.f_bavail * (off_t)sFileSystemStats.f_bsize;

    if (curr > cbFreeSpace)
    {
        ERROR("Not enough disk space for ftruncate\n");
        palError = ERROR_DISK_FULL;
        goto InternalSetEndOfFileExit;
    }
#endif // defined(__APPLE__)

    if ( ftruncate(pLocalData->unix_fd, curr) != 0 )
    {
        ERROR("ftruncate failed\n");
        if ( errno == EACCES )
        {
            ERROR("file may not be writable\n");
        }
        palError = FILEGetLastErrorFromErrno();
        goto InternalSetEndOfFileExit;
    }


InternalSetEndOfFileExit:

    // Windows starts returning ERROR_INVALID_PARAMETER at an arbitrary file size (~16TB). The file system
    // underneath us may be able to support larger and it would be a shame to prevent that. As a compromise,
    // if the operation fails and the file size was above the Windows limit map ERROR_DISK_FULL to
    // ERROR_INVALID_PARAMETER.
    // curr has been checked to be positive after getting the value from lseek. The following cast is put to
    // suppress the compilation warning.
    if (palError == ERROR_DISK_FULL && (static_cast<unsigned long>(curr) > 0x00000fffffff0000ULL ) )
        palError = ERROR_INVALID_PARAMETER;

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}



/*++
Function:
  SetEndOfFile

See MSDN doc.
--*/
BOOL
SetEndOfFile(
          HANDLE hFile)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;;

    pThread = InternalGetCurrentThread();

    palError = InternalSetEndOfFile(
        pThread,
        hFile
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("SetEndOfFile returns BOOL %d\n", NO_ERROR == palError);
    return NO_ERROR == palError;
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
InternalSetFilePointerForUnixFd(
    int iUnixFd,
    int32_t lDistanceToMove,
    int32_t * lpDistanceToMoveHigh,
    uint32_t dwMoveMethod,
    int32_t * lpNewFilePointerLow
    )
{
    PAL_ERROR palError = NO_ERROR;
    int     seek_whence = 0;
    long seek_offset = 0LL;
    long seek_res = 0LL;
    off_t old_offset;

    switch( dwMoveMethod )
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
    
    if ( lpDistanceToMoveHigh )
    {
        /* set the high 32 bits of the offset */
        seek_offset = ((long)*lpDistanceToMoveHigh << 32);
        
        /* set the low 32 bits */
        /* cast to unsigned long to avoid sign extension */
        seek_offset |= (uint32_t) lDistanceToMove;
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
        ERROR("lseek(fd,0,SEEK_CUR) failed errno:%d (%s)\n", 
              errno, strerror(errno));
        palError = ERROR_ACCESS_DENIED;
        goto done;
    }
    
    // Check to see if we're going to seek to a negative offset.
    // If we're seeking from the beginning or the current mark,
    // this is simple.
    if ((seek_whence == SEEK_SET && seek_offset < 0) ||
        (seek_whence == SEEK_CUR && seek_offset + old_offset < 0))
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

    seek_res = (long)lseek( iUnixFd,
                               seek_offset,
                               seek_whence );
    if ( seek_res < 0 )
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
        if ( lpDistanceToMoveHigh )
            *lpDistanceToMoveHigh = (uint32_t)(seek_res >> 32);
    
        /* return low-order uint32_t of seek result */
        *lpNewFilePointerLow = (uint32_t)seek_res;
    }

done:

    return palError;
}

PAL_ERROR
CorUnix::InternalSetFilePointer(
    CPalThread *pThread,
    HANDLE hFile,
    int32_t lDistanceToMove,
    int32_t * lpDistanceToMoveHigh,
    uint32_t dwMoveMethod,
    int32_t * lpNewFilePointerLow
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalSetFilePointerExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalSetFilePointerExit;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalSetFilePointerExit;
    }

    palError = InternalSetFilePointerForUnixFd(
        pLocalData->unix_fd,
        lDistanceToMove,
        lpDistanceToMoveHigh,
        dwMoveMethod,
        lpNewFilePointerLow
        );
    
InternalSetFilePointerExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}

PAL_ERROR
CorUnix::InternalGetFileSize(
    CPalThread *pThread,
    HANDLE hFile,
    uint32_t *pdwFileSizeLow,
    uint32_t *pdwFileSizeHigh
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    struct stat stat_data;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalGetFileSizeExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalGetFileSizeExit;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalGetFileSizeExit;
    }

    if (fstat(pLocalData->unix_fd, &stat_data) != 0)
    {
        ERROR("fstat failed of file descriptor %d\n", pLocalData->unix_fd);
        palError = FILEGetLastErrorFromErrno();
        goto InternalGetFileSizeExit;
    }

    *pdwFileSizeLow = (uint32_t)stat_data.st_size;
    
    if (NULL != pdwFileSizeHigh)
    {
        *pdwFileSizeHigh = (uint32_t)(stat_data.st_size >> 32);
    }

InternalGetFileSizeExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}

PAL_ERROR
CorUnix::InternalFlushFileBuffers(
    CPalThread *pThread,
    HANDLE hFile
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalFlushFileBuffersExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_WRITE,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalFlushFileBuffersExit;
    }
    
    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalFlushFileBuffersExit;
    }

    if (pLocalData->open_flags_deviceaccessonly == TRUE)
    {
        ERROR("File open for device access only\n");
        palError = ERROR_ACCESS_DENIED;
        goto InternalFlushFileBuffersExit;
    }

    do
    {

#if defined(__APPLE__)
        if (fcntl(pLocalData->unix_fd, F_FULLFSYNC) != -1)
            break;
#else // __APPLE__
        if (fsync(pLocalData->unix_fd) == 0)
            break;
#endif // __APPLE__
            
        switch (errno)
        {
        case EINTR:
            // Execution was interrupted by a signal, so restart.
            TRACE("fsync(%d) was interrupted. Restarting\n", pLocalData->unix_fd);
            break;
            
        default:
            palError = FILEGetLastErrorFromErrno();
            WARN("fsync(%d) failed with error %d\n", pLocalData->unix_fd, errno);
            break;
        }
    } while (NO_ERROR == palError);

InternalFlushFileBuffersExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}


/*++
Function:
  FlushFileBuffers

See MSDN doc.
--*/
BOOL
FlushFileBuffers(
          HANDLE hFile)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;

    pThread = InternalGetCurrentThread();

    palError = InternalFlushFileBuffers(
        pThread,
        hFile
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("FlushFileBuffers returns BOOL %d\n", NO_ERROR == palError);
    return NO_ERROR == palError;
}

PAL_ERROR
CorUnix::InternalGetFileType(
    CPalThread *pThread,
    HANDLE hFile,
    uint32_t *pdwFileType
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    struct stat stat_data;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalGetFileTypeExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalGetFileTypeExit;
    }
    
    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalGetFileTypeExit;
    }

    if (pLocalData->open_flags_deviceaccessonly == TRUE)
    {
        ERROR("File open for device access only\n");
        palError = ERROR_ACCESS_DENIED;
        goto InternalGetFileTypeExit;
    }

    if (fstat(pLocalData->unix_fd, &stat_data) != 0)
    {
        ERROR("fstat failed of file descriptor %d\n", pLocalData->unix_fd);
        palError = FILEGetLastErrorFromErrno();
        goto InternalGetFileTypeExit;
    }

    TRACE("st_mode & S_IFMT = %#x\n", stat_data.st_mode & S_IFMT);
    if (S_ISREG(stat_data.st_mode) || S_ISDIR(stat_data.st_mode))
    {
        *pdwFileType = FILE_TYPE_DISK;
    }
    else if (S_ISCHR(stat_data.st_mode))
    {
        *pdwFileType = FILE_TYPE_CHAR;
    }
    else if (S_ISFIFO(stat_data.st_mode))
    {
        *pdwFileType = FILE_TYPE_PIPE;
    }
    else
    {
        *pdwFileType = FILE_TYPE_UNKNOWN;
    }
    

InternalGetFileTypeExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;

}

#define ENSURE_UNIQUE_NOT_ZERO \
    if ( uUniqueSeed == 0 ) \
    {\
        uUniqueSeed++;\
    }

/*++
Function:
  FILEGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t FILEGetLastErrorFromErrno( void )
{
    uint32_t dwRet;

    switch(errno)
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
        ERROR("unexpected errno %d (%s); returning ERROR_GEN_FAILURE\n",
              errno, strerror(errno));
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
uint32_t DIRGetLastErrorFromErrno( void )
{
    if (errno == ENOENT)
        return ERROR_PATH_NOT_FOUND;
    else
        return FILEGetLastErrorFromErrno();
}


PAL_ERROR
CorUnix::InternalCreatePipe(
    CPalThread *pThread,
    HANDLE *phReadPipe,
    HANDLE *phWritePipe,
    LPSECURITY_ATTRIBUTES lpPipeAttributes,
    uint32_t nSize
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pReadFileObject = NULL;
    IPalObject *pReadRegisteredFile = NULL;
    IPalObject *pWriteFileObject = NULL;
    IPalObject *pWriteRegisteredFile = NULL;
    IDataLock *pDataLock = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    CObjectAttributes oaFile(NULL, lpPipeAttributes);
        
    int readWritePipeDes[2] = {-1, -1};

    if ((phReadPipe == NULL) || (phWritePipe == NULL))
    {
        ERROR("One of the two parameters hReadPipe(%p) and hWritePipe(%p) is Null\n",phReadPipe,phWritePipe);
        palError = ERROR_INVALID_PARAMETER;
        goto InternalCreatePipeExit;
    }

    if ((lpPipeAttributes == NULL) || 
        (lpPipeAttributes->bInheritHandle == FALSE) ||
        (lpPipeAttributes->lpSecurityDescriptor != NULL))
    {
        ASSERT("invalid security attributes!\n");
        palError = ERROR_INVALID_PARAMETER;
        goto InternalCreatePipeExit;
    }

    if (pipe(readWritePipeDes) == -1)
    {
        ERROR("pipe() call failed errno:%d (%s) \n", errno, strerror(errno));
        palError = ERROR_INTERNAL_ERROR;
        goto InternalCreatePipeExit;
    }

    /* enable close-on-exec for both pipes; if one gets passed to CreateProcess
       it will be "uncloseonexeced" in order to be inherited */
    if(-1 == fcntl(readWritePipeDes[0],F_SETFD,1))
    {
        ASSERT("can't set close-on-exec flag; fcntl() failed. errno is %d "
             "(%s)\n", errno, strerror(errno));
        palError = ERROR_INTERNAL_ERROR;
        goto InternalCreatePipeExit;
    }
    if(-1 == fcntl(readWritePipeDes[1],F_SETFD,1))
    {
        ASSERT("can't set close-on-exec flag; fcntl() failed. errno is %d "
             "(%s)\n", errno, strerror(errno));
        palError = ERROR_INTERNAL_ERROR;
        goto InternalCreatePipeExit;
    }

    //
    // Setup the object for the read end of the pipe
    //

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otFile,
        &oaFile,
        &pReadFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalCreatePipeExit;
    }

    palError = pReadFileObject->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalCreatePipeExit;
    }

    pLocalData->inheritable = TRUE;
    pLocalData->open_flags = O_RDONLY;

    //
    // After storing the file descriptor in the object's local data
    // we want to clear it from the array to prevent a possible double
    // close if an error occurs.
    //

    pLocalData->unix_fd = readWritePipeDes[0];
    readWritePipeDes[0] = -1;

    pDataLock->ReleaseLock(pThread, TRUE);
    pDataLock = NULL;

    //
    // Setup the object for the write end of the pipe
    //

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otFile,
        &oaFile,
        &pWriteFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalCreatePipeExit;
    }

    palError = pWriteFileObject->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalCreatePipeExit;
    }

    pLocalData->inheritable = TRUE;
    pLocalData->open_flags = O_WRONLY;

    //
    // After storing the file descriptor in the object's local data
    // we want to clear it from the array to prevent a possible double
    // close if an error occurs.
    //

    pLocalData->unix_fd = readWritePipeDes[1];
    readWritePipeDes[1] = -1;

    pDataLock->ReleaseLock(pThread, TRUE);
    pDataLock = NULL;

    //
    // Register the pipe objects
    //

    palError = g_pObjectManager->RegisterObject(
        pThread,
        pReadFileObject,
        &aotFile,
        GENERIC_READ,
        phReadPipe,
        &pReadRegisteredFile
        );

    //
    // pReadFileObject is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line.
    //

    pReadFileObject = NULL;

    if (NO_ERROR != palError)
    {
        goto InternalCreatePipeExit;
    }

    palError = g_pObjectManager->RegisterObject(
        pThread,
        pWriteFileObject,
        &aotFile,
        GENERIC_WRITE,
        phWritePipe,
        &pWriteRegisteredFile
        );

    //
    // pWriteFileObject is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line.
    //

    pWriteFileObject = NULL;
    
InternalCreatePipeExit:

    if (NO_ERROR != palError)
    {
        if (-1 != readWritePipeDes[0])
        {
            close(readWritePipeDes[0]);
        }

        if (-1 != readWritePipeDes[1])
        {
            close(readWritePipeDes[1]);
        }
    }

    if (NULL != pReadFileObject)
    {
        pReadFileObject->ReleaseReference(pThread);
    }

    if (NULL != pReadRegisteredFile)
    {
        pReadRegisteredFile->ReleaseReference(pThread);
    }

    if (NULL != pWriteFileObject)
    {
        pWriteFileObject->ReleaseReference(pThread);
    }

    if (NULL != pWriteRegisteredFile)
    {
        pWriteRegisteredFile->ReleaseReference(pThread);
    }

    return palError;
}

        PAL_ERROR
CorUnix::InternalLockFile(
    CPalThread *pThread,
    HANDLE hFile,
    uint32_t dwFileOffsetLow,
    uint32_t dwFileOffsetHigh,
    uint32_t nNumberOfBytesToLockLow,
    uint32_t nNumberOfBytesToLockHigh
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalLockFileExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalLockFileExit;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalLockFileExit;
    }
    
    if (NULL != pLocalData->pLockController)
    {
        palError = pLocalData->pLockController->CreateFileLock(
            pThread,
            dwFileOffsetLow,
            dwFileOffsetHigh,
            nNumberOfBytesToLockLow,
            nNumberOfBytesToLockHigh,
            IFileLockController::ExclusiveFileLock,
            IFileLockController::FailImmediately
            );
    }
    else
    {
        //
        // This isn't a lockable file (e.g., it may be a pipe)
        //
        
        palError = ERROR_ACCESS_DENIED;
        goto InternalLockFileExit;
    }

InternalLockFileExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}

PAL_ERROR
CorUnix::InternalUnlockFile(
    CPalThread *pThread,
    HANDLE hFile,
    uint32_t dwFileOffsetLow,
    uint32_t dwFileOffsetHigh,
    uint32_t nNumberOfBytesToUnlockLow,
    uint32_t nNumberOfBytesToUnlockHigh
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
        goto InternalUnlockFileExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto InternalUnlockFileExit;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto InternalUnlockFileExit;
    }
    
    if (NULL != pLocalData->pLockController)
    {
        palError = pLocalData->pLockController->ReleaseFileLock(
            pThread,
            dwFileOffsetLow,
            dwFileOffsetHigh,
            nNumberOfBytesToUnlockLow,
            nNumberOfBytesToUnlockHigh
            );
    }
    else
    {
        //
        // This isn't a lockable file (e.g., it may be a pipe)
        //
        
        palError = ERROR_ACCESS_DENIED;
        goto InternalUnlockFileExit;
    }

InternalUnlockFileExit:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
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
static HANDLE init_std_handle(HANDLE * pStd, FILE *stream)
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
    if(-1 == new_fd)
    {
        ERROR("dup() failed; errno is %d (%s)\n", errno, strerror(errno));
        goto done;
    }

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otFile,
        &oa,
        &pFileObject
        );

    if (NO_ERROR != palError)
    {
        goto done;
    }

    palError = pFileObject->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

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

    pDataLock->ReleaseLock(pThread, TRUE);
    pDataLock = NULL;

    palError = g_pObjectManager->RegisterObject(
        pThread,
        pFileObject,
        &aotFile, 
        0,
        &hFile,
        &pRegisteredFile
        );

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
        pDataLock->ReleaseLock(pThread, TRUE);
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
    if(INVALID_HANDLE_VALUE == stdin_handle)
    {
        ERROR("failed to create stdin handle\n");
        goto fail;
    }

    stdout_handle = init_std_handle(&pStdOut, stdout);
    if(INVALID_HANDLE_VALUE == stdout_handle)
    {
        ERROR("failed to create stdout handle\n");
        CloseHandle(stdin_handle);
        goto fail;
    }

    stderr_handle = init_std_handle(&pStdErr, stderr);
    if(INVALID_HANDLE_VALUE == stderr_handle)
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
