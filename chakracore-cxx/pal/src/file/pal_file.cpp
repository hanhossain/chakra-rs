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
#include "chakra/Logger.h"

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(FILE);

int MaxWCharToAcpLengthFactor = 3;

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool);

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

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool)
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
        chakra::Logger::error("Unable to obtain data to cleanup file object");
        return;
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

    //
    // We've finished initializing our local data, so release that lock
    //

    pDataLock->ReleaseLock(pThread);
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

    return TRUE;

fail:
    pStdIn = INVALID_HANDLE_VALUE;
    pStdOut = INVALID_HANDLE_VALUE;
    return FALSE;
}
