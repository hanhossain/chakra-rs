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

#include "pal/thread.hpp"
#include "pal/file.hpp"
#include "shmfilelockmgr.hpp"
#include "pal/malloc.hpp"
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

CObjectType CorUnix::otFile PAL_GLOBAL (
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

CAllowedObjectTypes CorUnix::aotFile PAL_GLOBAL (otiFile);
static CSharedMemoryFileLockMgr _FileLockManager PAL_GLOBAL;
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

    IN LPSTR lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError( LPSTR lpPath, uint32_t * lpErrorCode )
{
    struct stat stat_data;
    LPSTR lpDupedPath = NULL;
    LPSTR lpLastPathSeparator = NULL;

    TRACE( "FILEGetProperNotFoundError( %s )\n", lpPath?lpPath:"(null)" );

    if ( !lpErrorCode )
    {
        ASSERT( "lpErrorCode has to be valid\n" );
        return;
    }

    if ( NULL == ( lpDupedPath = InternalStrdup( lpPath ) ) )
    {
        ERROR( "InternalStrdup() failed!\n" );
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
    
    InternalFree(lpDupedPath);
    lpDupedPath = NULL;
    TRACE( "FILEGetProperNotFoundError returning TRUE\n" );
    return;
}

/*++
Function : 
    FILEGetLastErrorFromErrnoAndFilename
    
Returns the proper error code for errno, or, if errno is ENOENT,
based on the Windows behavior for nonexistent filenames.

    IN LPSTR lpPath - The path to check.
*/
PAL_ERROR FILEGetLastErrorFromErrnoAndFilename(LPSTR lpPath)
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
CorUnix::InternalCanonicalizeRealPath(LPCSTR lpUnixPath, LPSTR lpBuffer, uint32_t cch)
{
    PAL_ERROR palError = NO_ERROR;
    LPSTR lpRealPath = NULL;

#if !REALPATH_SUPPORTS_NONEXISTENT_FILES
    LPSTR lpExistingPath = NULL;
    LPSTR pchSeparator = NULL;
    LPSTR lpFilename = NULL;
    uint32_t cchBuffer = 0;
    uint32_t cchFilename = 0;
#endif // !REALPATH_SUPPORTS_NONEXISTENT_FILES
 
    if ( (lpUnixPath == NULL) || (lpBuffer == NULL) || (cch < PATH_MAX) )
    {
        ERROR ("Invalid argument to InternalCanonicalizeRealPath\n");
        palError = ERROR_INVALID_PARAMETER;
        goto LExitDontFree;
    }

#if REALPATH_SUPPORTS_NONEXISTENT_FILES
    lpRealPath = realpath(lpUnixPath, lpBuffer);
#else   // !REALPATH_SUPPORTS_NONEXISTENT_FILES

    lpExistingPath = InternalStrdup(lpUnixPath);
    if (lpExistingPath == NULL)
    {
        ERROR ("InternalStrdup failed with error %d\n", errno);
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
        InternalFree(lpExistingPath);
    }
#endif // REALPATH_SUPPORTS_NONEXISTENT_FILES

LExitDontFree:
    if ((palError == NO_ERROR) && (lpRealPath == NULL))
    {
        // convert all these into ERROR_PATH_NOT_FOUND
        palError = ERROR_PATH_NOT_FOUND;
    }

    return palError;
}

PAL_ERROR
CorUnix::InternalCreateFile(
    CPalThread *pThread,
    LPCSTR lpFileName,
    uint32_t dwDesiredAccess,
    uint32_t dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    uint32_t dwCreationDisposition,
    uint32_t dwFlagsAndAttributes,
    HANDLE hTemplateFile,
    HANDLE *phFile
    )
{
    PAL_ERROR palError = 0;
    IPalObject *pFileObject = NULL;
    IPalObject *pRegisteredFile = NULL;
    IDataLock *pDataLock = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IFileLockController *pLockController = NULL;
    CObjectAttributes oaFile(NULL, lpSecurityAttributes);
    BOOL fFileExists = FALSE;

    BOOL inheritable = FALSE;
    LPSTR lpUnixPath = NULL;
    int   filed = -1;
    int   create_flags = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int   open_flags = 0;
    int   lock_mode = LOCK_SH;

    // track whether we've created the file with the intended name,
    // so that it can be removed on failure exit
    BOOL bFileCreated = FALSE;

    const char* szNonfilePrefix = "\\\\.\\";
    LPSTR lpFullUnixPath = NULL;
    uint32_t cchFullUnixPath = PATH_MAX+1; // InternalCanonicalizeRealPath requires this to be at least PATH_MAX

    /* for dwShareMode only three flags are accepted */
    if ( dwShareMode & ~(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE) )
    {
        ASSERT( "dwShareMode is invalid\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    if ( lpFileName == NULL )
    {
        ERROR("InternalCreateFile called with NULL filename\n");
        palError = ERROR_PATH_NOT_FOUND;
        goto done;
    }

    if ( strlen(lpFileName) >= MAX_LONGPATH )
    {
        WARN("InternalCreateFile called with a filename whose size is "
                "%d >= MAX_LONGPATH (%d)\n", strlen(lpFileName), MAX_LONGPATH);
        palError = ERROR_FILENAME_EXCED_RANGE;
        goto done;
    }

    if ( strncmp(lpFileName, szNonfilePrefix, strlen(szNonfilePrefix)) == 0 )
    {
        ERROR("InternalCreateFile does not support paths beginning with %s\n", szNonfilePrefix);
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    lpUnixPath = InternalStrdup(lpFileName);
    if ( lpUnixPath == NULL )
    {
        ERROR("InternalStrdup() failed\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }

    lpFullUnixPath =  reinterpret_cast<LPSTR>(InternalMalloc(cchFullUnixPath));
    if ( lpFullUnixPath == NULL )
    {
        ERROR("InternalMalloc() failed\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }

    // Initialize the path to zeroes...
    ZeroMemory(lpFullUnixPath, cchFullUnixPath);
    
    FILEDosToUnixPathA( lpUnixPath );

    // Compute the absolute pathname to the file.  This pathname is used
    // to determine if two file names represent the same file.
    palError = InternalCanonicalizeRealPath(lpUnixPath, lpFullUnixPath, cchFullUnixPath);
    if (palError != NO_ERROR)
    {
        goto done;
    }

    InternalFree(lpUnixPath);
    lpUnixPath = lpFullUnixPath;
    lpFullUnixPath = NULL;

    switch( dwDesiredAccess )
    {
    case 0:
        /* Device Query Access was requested. let's use open() with 
           no flags, it's basically the equivalent of O_RDONLY, since 
           O_RDONLY is defined as 0x0000 */
        break;
    case( GENERIC_READ ):
        open_flags |= O_RDONLY;
        break;
    case( GENERIC_WRITE ):
        open_flags |= O_WRONLY;
        break;
    case( GENERIC_READ | GENERIC_WRITE ):
        open_flags |= O_RDWR;
        break;
    default:
        ERROR("dwDesiredAccess value of %d is invalid\n", dwDesiredAccess);
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    TRACE("open flags are 0x%lx\n", open_flags);
    
    if ( lpSecurityAttributes )
    {
        if ( lpSecurityAttributes->nLength != sizeof( SECURITY_ATTRIBUTES ) ||
             lpSecurityAttributes->lpSecurityDescriptor != NULL ||
             !lpSecurityAttributes->bInheritHandle )
        {
            ASSERT("lpSecurityAttributes points to invalid values.\n");
            palError = ERROR_INVALID_PARAMETER;
            goto done;
        }
        inheritable = TRUE;
    }

    if ( (dwFlagsAndAttributes & PAL_LEGAL_FLAGS_ATTRIBS) !=
          dwFlagsAndAttributes)
    {
        ASSERT("Bad dwFlagsAndAttributes\n");
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    } 
    else if (dwFlagsAndAttributes & FILE_FLAG_BACKUP_SEMANTICS)
    {
        /* Override the open flags, and always open as readonly.  This
           flag is used when opening a directory, to change its
           creation/modification/access times.  On Windows, the directory
           must be open for write, but on Unix, it needs to be readonly. */
        open_flags = O_RDONLY;
    } else {
        struct stat st;

        if (stat(lpUnixPath, &st) == 0 && (st.st_mode & S_IFDIR))
        {
            /* The file exists and it is a directory.  Without
                   FILE_FLAG_BACKUP_SEMANTICS, Win32 CreateFile always fails
                   to open directories. */
                palError = ERROR_ACCESS_DENIED;
                goto done;
        }
    }

    if ( hTemplateFile )
    {
        ASSERT("hTemplateFile is not NULL, as it should be.\n");
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    //
    // The file sharing mode checks are performed by the lock manager so we need
    // to get the lock controller for this file.
    // Do this before modifying the file system since we wouldn't want to, for
    // instance, truncate a file before finding out if we have write access to it.
    // It may seem odd that in some cases we will acquire a lock on a file that
    // doesn't exist yet but the lock manager does not care -- files are
    // abstract entities represented by a name from its point of view.
    //

    palError = g_pFileLockManager->GetLockControllerForFile(
        pThread, 
        lpUnixPath,
        dwDesiredAccess,
        dwShareMode,
        &pLockController
        );

    if (NO_ERROR != palError)
    {
        goto done;
    }

    /* NB: According to MSDN docs, When CREATE_ALWAYS or OPEN_ALWAYS is
       set, CreateFile should SetLastError to ERROR_ALREADY_EXISTS,
       even though/if CreateFile will be successful.
    */
    switch( dwCreationDisposition )
    {
    case( CREATE_ALWAYS ):        
        // check whether the file exists
        if ( access( lpUnixPath, F_OK ) == 0 )
        {
            fFileExists = TRUE;
        }
        open_flags |= O_CREAT | O_TRUNC;
        break;
    case( CREATE_NEW ):
        open_flags |= O_CREAT | O_EXCL;
        break;
    case( OPEN_EXISTING ):
        /* don't need to do anything here */
        break;
    case( OPEN_ALWAYS ):
        if ( access( lpUnixPath, F_OK ) == 0 )
        {
            fFileExists = TRUE;
        }
        open_flags |= O_CREAT;
        break;
    case( TRUNCATE_EXISTING ):
        open_flags |= O_TRUNC;
        break;
    default:
        ASSERT("dwCreationDisposition value of %d is not valid\n",
              dwCreationDisposition);
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    if ( dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING )
    {
        TRACE("I/O will be unbuffered\n");
#ifdef O_DIRECT
        open_flags |= O_DIRECT;
#endif
    }
    else
    {
        TRACE("I/O will be buffered\n");
    }

    filed = InternalOpen(lpUnixPath, open_flags, create_flags);
    TRACE("Allocated file descriptor [%d]\n", filed);

    if ( filed < 0 )
    {
        TRACE("open() failed; error is %s (%d)\n", strerror(errno), errno);
        palError = FILEGetLastErrorFromErrnoAndFilename(lpUnixPath);
        goto done;
    }

    // Deduce whether we created a file in the previous operation (there's a
    // small timing window between the access() used to determine fFileExists
    // and the open() operation, but there's not much we can do about that.
    bFileCreated = (dwCreationDisposition == CREATE_ALWAYS ||
                    dwCreationDisposition == CREATE_NEW ||
                    dwCreationDisposition == OPEN_ALWAYS) &&
        !fFileExists;


    // While the lock manager is able to provide support for share modes within an instance of
    // the PAL, other PALs will ignore these locks.  In order to support a basic level of cross
    // process locking, we'll use advisory locks.  FILE_SHARE_NONE implies a exclusive lock on the
    // file and all other modes use a shared lock.  While this is not as granular as Windows,
    // you can at least implement a lock file using this.
    lock_mode = (dwShareMode == 0 /* FILE_SHARE_NONE */) ? LOCK_EX : LOCK_SH;

    if(flock(filed, lock_mode | LOCK_NB) != 0) 
    {
        TRACE("flock() failed; error is %s (%d)\n", strerror(errno), errno);
        if (errno == EWOULDBLOCK) 
        {
            palError = ERROR_SHARING_VIOLATION;
        }
        else
        {
            palError = FILEGetLastErrorFromErrno();
        }

        goto done;
    }

#ifndef O_DIRECT
    if ( dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING )
    {
#ifdef F_NOCACHE
        if (-1 == fcntl(filed, F_NOCACHE, 1))
        {
            ASSERT("Can't set F_NOCACHE; fcntl() failed. errno is %d (%s)\n",
               errno, strerror(errno));
            palError = ERROR_INTERNAL_ERROR;
            goto done;
        }
#else
#error Insufficient support for uncached I/O on this platform
#endif
    }
#endif
    
    /* make file descriptor close-on-exec; inheritable handles will get
      "uncloseonexeced" in CreateProcess if they are actually being inherited*/
    if(-1 == fcntl(filed,F_SETFD,1))
    {
        ASSERT("can't set close-on-exec flag; fcntl() failed. errno is %d "
             "(%s)\n", errno, strerror(errno));
        palError = ERROR_INTERNAL_ERROR;
        goto done;
    }

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otFile,
        &oaFile,
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

    if (strcpy_s(pLocalData->unix_filename, sizeof(pLocalData->unix_filename), lpUnixPath) != SAFECRT_SUCCESS)
    {
        palError = ERROR_INSUFFICIENT_BUFFER;
        TRACE("strcpy_s failed!\n");
        goto done;
    }

    pLocalData->inheritable = inheritable;
    pLocalData->unix_fd = filed;
    pLocalData->dwDesiredAccess = dwDesiredAccess;
    pLocalData->open_flags = open_flags;
    pLocalData->open_flags_deviceaccessonly = (dwDesiredAccess == 0);

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
        dwDesiredAccess,
        phFile,
        &pRegisteredFile
        );

    //
    // pFileObject is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line.
    //

    pFileObject = NULL;

done:

    // At this point, if we've been successful, palError will be NO_ERROR.
    // CreateFile can return ERROR_ALREADY_EXISTS in some success cases;
    // those cases are flagged by fFileExists and are handled below.
    if (NO_ERROR != palError)
    {
        if (filed >= 0)
        {
            close(filed);
        }
        if (bFileCreated)
        {
            if (-1 == unlink(lpUnixPath))
            {
                WARN("can't delete file; unlink() failed with errno %d (%s)\n",
                     errno, strerror(errno));
            }
        }
    }

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
    
    if (NULL != lpUnixPath)
    {
        InternalFree(lpUnixPath);
    }

    if (NULL != lpFullUnixPath)
    {
        InternalFree(lpFullUnixPath);
    }

    if (NO_ERROR == palError && fFileExists)
    {
        palError = ERROR_ALREADY_EXISTS;
    }

    return palError;
}

/*++
Function:
  CreateFileA

Note:
  Only bInherit flag is used from the LPSECURITY_ATTRIBUTES struct.
  Desired access is READ, WRITE or 0
  Share mode is READ, WRITE or DELETE

See MSDN doc.
--*/
HANDLE
CreateFileA(
         LPCSTR lpFileName,
         uint32_t dwDesiredAccess,
         uint32_t dwShareMode,
         LPSECURITY_ATTRIBUTES lpSecurityAttributes,
         uint32_t dwCreationDisposition,
         uint32_t dwFlagsAndAttributes,
         HANDLE hTemplateFile
        )
{
    CPalThread *pThread;
    PAL_ERROR palError = NO_ERROR;
    HANDLE  hRet = INVALID_HANDLE_VALUE;

    PERF_ENTRY(CreateFileA);
    ENTRY("CreateFileA(lpFileName=%p (%s), dwAccess=%#x, dwShareMode=%#x, "
          "lpSecurityAttr=%p, dwDisposition=%#x, dwFlags=%#x, " 
          "hTemplateFile=%p )\n",lpFileName?lpFileName:"NULL",lpFileName?lpFileName:"NULL", dwDesiredAccess, 
          dwShareMode, lpSecurityAttributes, dwCreationDisposition, 
          dwFlagsAndAttributes, hTemplateFile);

    pThread = InternalGetCurrentThread();

    palError = InternalCreateFile(
        pThread,
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile,
        &hRet
        );

    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

    pThread->SetLastError(palError);

    LOGEXIT("CreateFileA returns HANDLE %p\n", hRet);
    PERF_EXIT(CreateFileA);
    return hRet;
}


/*++
Function:
  CreateFileW

Note:
  Only bInherit flag is used from the LPSECURITY_ATTRIBUTES struct.
  Desired access is READ, WRITE or 0
  Share mode is READ, WRITE or DELETE

See MSDN doc.
--*/
HANDLE
CreateFileW(
         LPCWSTR lpFileName,
         uint32_t dwDesiredAccess,
         uint32_t dwShareMode,
         LPSECURITY_ATTRIBUTES lpSecurityAttributes,
         uint32_t dwCreationDisposition,
         uint32_t dwFlagsAndAttributes,
         HANDLE hTemplateFile)
{
    CPalThread *pThread;
    PAL_ERROR palError = NO_ERROR;
    PathCharString namePathString;
    char * name;
    int size;
    int length = 0;
    HANDLE  hRet = INVALID_HANDLE_VALUE;

    PERF_ENTRY(CreateFileW);
    ENTRY("CreateFileW(lpFileName=%p (%S), dwAccess=%#x, dwShareMode=%#x, "
          "lpSecurityAttr=%p, dwDisposition=%#x, dwFlags=%#x, hTemplateFile=%p )\n",
          lpFileName?lpFileName:W16_NULLSTRING,
          lpFileName?lpFileName:W16_NULLSTRING, dwDesiredAccess, dwShareMode,
          lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes,
          hTemplateFile);

    pThread = InternalGetCurrentThread();

    if (lpFileName != NULL)
    {
        length = (PAL_wcslen(lpFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    name = namePathString.OpenStringBuffer(length);
    if (NULL == name)
    {
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    
    size = WideCharToMultiByte( CP_ACP, 0, lpFileName, -1, name, length,
                                NULL, NULL );
    namePathString.CloseBuffer(size);    

    if( size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            palError = ERROR_FILENAME_EXCED_RANGE;
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            palError = ERROR_INTERNAL_ERROR;
        }
        goto done;
    }

    palError = InternalCreateFile(
        pThread,
        name,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile,
        &hRet
        );

    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

done:
	pThread->SetLastError(palError);
    LOGEXIT( "CreateFileW returns HANDLE %p\n", hRet );
    PERF_EXIT(CreateFileW);
    return hRet;
}


/*++
Function:
  CopyFileW

See MSDN doc.

Notes:
  There are several (most) error paths here that do not call SetLastError().
This is because we know that CreateFile, ReadFile, and WriteFile will do so,
and will have a much better idea of the specific error.
--*/
BOOL
CopyFileW(
       LPCWSTR lpExistingFileName,
       LPCWSTR lpNewFileName,
       BOOL bFailIfExists)
{
    CPalThread *pThread;
    PathCharString sourcePathString;
    PathCharString destPathString;
    char * source;
    char * dest;
    int src_size, dest_size, length = 0;
    BOOL bRet = FALSE;

    PERF_ENTRY(CopyFileW);
    ENTRY("CopyFileW(lpExistingFileName=%p (%S), lpNewFileName=%p (%S), bFailIfExists=%d)\n",
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING, bFailIfExists);

    pThread = InternalGetCurrentThread();
    if (lpExistingFileName != NULL)
    {
        length = (PAL_wcslen(lpExistingFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    source = sourcePathString.OpenStringBuffer(length);
    if (NULL == source)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }

    src_size = WideCharToMultiByte( CP_ACP, 0, lpExistingFileName, -1, source, length,
                                NULL, NULL );
    sourcePathString.CloseBuffer(src_size);
    
    if( src_size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpExistingFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        goto done;
    }

    length = 0;
    if (lpNewFileName != NULL)
    {
        length = (PAL_wcslen(lpNewFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    dest = destPathString.OpenStringBuffer(length);
    if (NULL == dest)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    dest_size = WideCharToMultiByte( CP_ACP, 0, lpNewFileName, -1, dest, length,
                                NULL, NULL );
    destPathString.CloseBuffer(dest_size);
    
    if( dest_size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpNewFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        goto done;
    }

    bRet = CopyFileA(source,dest,bFailIfExists);

done:
    LOGEXIT("CopyFileW returns BOOL %d\n", bRet);
    PERF_EXIT(CopyFileW);
    return bRet;
}


/*++
Function:
  DeleteFileA

See MSDN doc.
--*/
BOOL
DeleteFileA(
         LPCSTR lpFileName)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    int     result;
    BOOL    bRet = FALSE;
    uint32_t   dwLastError = 0;
    char * lpUnixFileName;
    int length;
    PathCharString lpUnixFileNamePS;
    LPSTR lpFullUnixFileName = NULL;
    uint32_t cchFullUnixFileName = MAX_LONGPATH+1;// InternalCanonicalizeRealPath requires this to be at least PATH_MAX

    PERF_ENTRY(DeleteFileA);
    ENTRY("DeleteFileA(lpFileName=%p (%s))\n", lpFileName?lpFileName:"NULL", lpFileName?lpFileName:"NULL");

    pThread = InternalGetCurrentThread();
    length = strlen(lpFileName);

    lpUnixFileName = lpUnixFileNamePS.OpenStringBuffer(length);
    if (NULL == lpUnixFileName)
    {
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    strcpy_s( lpUnixFileName, lpUnixFileNamePS.GetSizeOf(), lpFileName);
    lpUnixFileNamePS.CloseBuffer(length);
    
    FILEDosToUnixPathA( lpUnixFileName );

    lpFullUnixFileName =  reinterpret_cast<LPSTR>(InternalMalloc(cchFullUnixFileName));
    if ( lpFullUnixFileName == NULL )
    {
        ERROR("InternalMalloc() failed\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }

    // Initialize the path to zeroes...
    ZeroMemory(lpFullUnixFileName, cchFullUnixFileName);
    
    // Compute the absolute pathname to the file.  This pathname is used
    // to determine if two file names represent the same file.
    palError = InternalCanonicalizeRealPath(lpUnixFileName, lpFullUnixFileName, cchFullUnixFileName);
    if (palError != NO_ERROR)
    {
        InternalFree(lpFullUnixFileName);
        lpFullUnixFileName = InternalStrdup(lpUnixFileName);
        if (!lpFullUnixFileName)
        {
            palError = ERROR_NOT_ENOUGH_MEMORY;
            goto done;
        }
    }

    result = unlink( lpFullUnixFileName );

    if (result < 0)
    {
        TRACE("unlink returns %d\n", result);
        dwLastError = FILEGetLastErrorFromErrnoAndFilename(lpFullUnixFileName);
    }
    else
    {
        bRet = TRUE;
    }

done:
    if(dwLastError)
    {
        pThread->SetLastError( dwLastError );
    }
    if (NULL != lpFullUnixFileName)
    {
        InternalFree(lpFullUnixFileName);
    }
    LOGEXIT("DeleteFileA returns BOOL %d\n", bRet);
    PERF_EXIT(DeleteFileA);
    return bRet;
}


/*++
Function:
  DeleteFileW

See MSDN doc.
--*/
BOOL
DeleteFileW(
         LPCWSTR lpFileName)
{
    CPalThread *pThread;
    int  size;
    PathCharString namePS;
    char * name;
    int length = 0;
    BOOL bRet = FALSE;

    PERF_ENTRY(DeleteFileW);
    ENTRY("DeleteFileW(lpFileName=%p (%S))\n",
      lpFileName?lpFileName:W16_NULLSTRING,
      lpFileName?lpFileName:W16_NULLSTRING);

    pThread = InternalGetCurrentThread();
    
    if (lpFileName != NULL)
    {
        length = (PAL_wcslen(lpFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    name = namePS.OpenStringBuffer(length);
    if (NULL == name)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }

    size = WideCharToMultiByte( CP_ACP, 0, lpFileName, -1, name, length,
                                NULL, NULL );
    namePS.CloseBuffer(size);
    
    if( size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpFilePathName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        bRet = FALSE;
        goto done;
    }

    bRet = DeleteFileA( name );

done:
    LOGEXIT("DeleteFileW returns BOOL %d\n", bRet);
    PERF_EXIT(DeleteFileW);
    return bRet;
}


/*++
Function:
  MoveFileA

See MSDN doc.
--*/
BOOL
MoveFileA(
      LPCSTR lpExistingFileName,
      LPCSTR lpNewFileName)
{
    BOOL bRet;

    PERF_ENTRY(MoveFileA);
    ENTRY("MoveFileA(lpExistingFileName=%p (%s), lpNewFileName=%p (%s))\n",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL");

    bRet = MoveFileExA( lpExistingFileName,
            lpNewFileName,
            MOVEFILE_COPY_ALLOWED );

    LOGEXIT("MoveFileA returns BOOL %d\n", bRet);
    PERF_EXIT(MoveFileA);
    return bRet;
}


/*++
Function:
  MoveFileW

See MSDN doc.
--*/
BOOL
MoveFileW(
      LPCWSTR lpExistingFileName,
      LPCWSTR lpNewFileName)
{
    BOOL bRet;

    PERF_ENTRY(MoveFileW);
    ENTRY("MoveFileW(lpExistingFileName=%p (%S), lpNewFileName=%p (%S))\n",
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING);

    bRet = MoveFileExW( lpExistingFileName,
            lpNewFileName,
            MOVEFILE_COPY_ALLOWED );

    LOGEXIT("MoveFileW returns BOOL %d\n", bRet);
    PERF_EXIT(MoveFileW);
    return bRet;
}

/*++
Function:
  MoveFileExA

See MSDN doc.
--*/
BOOL
MoveFileExA(
         LPCSTR lpExistingFileName,
         LPCSTR lpNewFileName,
         uint32_t dwFlags)
{
    CPalThread *pThread;
    int   result;
    int length = 0;
    PathCharString sourcePS;
    PathCharString destPS;
    char * source;
    char * dest;
    BOOL  bRet = TRUE;
    uint32_t dwLastError = 0;

    PERF_ENTRY(MoveFileExA);
    ENTRY("MoveFileExA(lpExistingFileName=%p (%S), lpNewFileName=%p (%S), "
          "dwFlags=%#x)\n",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL", dwFlags);

    pThread = InternalGetCurrentThread();
    /* only two flags are accepted */
    if ( dwFlags & ~(MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING) )
    {
        ASSERT( "dwFlags is invalid\n" );
        dwLastError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    length = strlen(lpExistingFileName);
    
    source = sourcePS.OpenStringBuffer(length);
    if (NULL == source)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    strcpy_s( source, sourcePS.GetSizeOf(), lpExistingFileName);
    sourcePS.CloseBuffer(length);
    
    FILEDosToUnixPathA( source );

    length = strlen(lpNewFileName);
    
    dest = destPS.OpenStringBuffer(length);
    if (NULL == dest)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    strcpy_s( dest, destPS.GetSizeOf(), lpNewFileName);
    destPS.CloseBuffer(length);
    
    FILEDosToUnixPathA( dest );

    if ( !(dwFlags & MOVEFILE_REPLACE_EXISTING) )
    {
#if HAVE_CASE_SENSITIVE_FILESYSTEM
        if ( strcmp(source, dest) != 0 )
#else   // HAVE_CASE_SENSITIVE_FILESYSTEM
        if ( strcasecmp(source, dest) != 0 )
#endif  // HAVE_CASE_SENSITIVE_FILESYSTEM
        {
            // Let things proceed normally if source and
            // dest are the same.
            if ( access(dest, F_OK) == 0 )
            {
                dwLastError = ERROR_ALREADY_EXISTS;
                goto done;
            }
        }
    }

    result = rename( source, dest );
    if ((result < 0) && (dwFlags & MOVEFILE_REPLACE_EXISTING) &&
        ((errno == ENOTDIR) || (errno == EEXIST)))
    {
        bRet = DeleteFileA( lpNewFileName );
        
        if ( bRet ) 
        {
            result = rename( source, dest );
        }
        else
        { 
            dwLastError = GetLastError();
        }
    } 

    if ( result < 0 )
    {
        switch( errno )
        {
        case EXDEV: /* we tried to link across devices */
        
            if ( dwFlags & MOVEFILE_COPY_ALLOWED )
            {
                BOOL bFailIfExists = !(dwFlags & MOVEFILE_REPLACE_EXISTING);
            
                /* if CopyFile fails here, so should MoveFailEx */
                bRet = CopyFileA( lpExistingFileName,
                          lpNewFileName,
                          bFailIfExists );
                /* CopyFile should set the appropriate error */
                if ( !bRet ) 
                {
                    dwLastError = GetLastError();
                }
                else
                {
                    if (!DeleteFileA(lpExistingFileName))
                    {
                        ERROR("Failed to delete the source file\n");
                        dwLastError = GetLastError();
                    
                        /* Delete the destination file if we're unable to delete 
                           the source file */
                        if (!DeleteFileA(lpNewFileName))
                        {
                            ERROR("Failed to delete the destination file\n");
                        }
                    }
                }
            }
            else
            {
                dwLastError = ERROR_ACCESS_DENIED;
            }
            break;
        case EINVAL: // tried to rename "." or ".."
            dwLastError = ERROR_SHARING_VIOLATION;
            break;
        case ENOENT:
            {
                struct stat buf;
                if (lstat(source, &buf) == -1)
                {
                    FILEGetProperNotFoundError(source, &dwLastError);
                }
                else
                {
                    dwLastError = ERROR_PATH_NOT_FOUND;
                }
            }
            break;
        default:
            dwLastError = FILEGetLastErrorFromErrno();
            break;
        }
    }

done:
    if ( dwLastError )
    {
        pThread->SetLastError( dwLastError );
        bRet = FALSE;
    }

    LOGEXIT( "MoveFileExA returns BOOL %d\n", bRet );
    PERF_EXIT(MoveFileExA);
    return bRet;
}

/*++
Function:
  MoveFileExW

See MSDN doc.
--*/
BOOL
MoveFileExW(
         LPCWSTR lpExistingFileName,
         LPCWSTR lpNewFileName,
         uint32_t dwFlags)
{
    CPalThread *pThread;
    PathCharString sourcePS;
    PathCharString destPS;
    char * source;
    char * dest;
    int length = 0;
    int     src_size,dest_size;
    BOOL        bRet = FALSE;

    PERF_ENTRY(MoveFileExW);
    ENTRY("MoveFileExW(lpExistingFileName=%p (%S), lpNewFileName=%p (%S), dwFlags=%#x)\n",
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpExistingFileName?lpExistingFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING,
          lpNewFileName?lpNewFileName:W16_NULLSTRING, dwFlags);

    pThread = InternalGetCurrentThread();
    
    if (lpExistingFileName != NULL)
    {
        length = (PAL_wcslen(lpExistingFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    source = sourcePS.OpenStringBuffer(length);
    if (NULL == source)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    src_size = WideCharToMultiByte( CP_ACP, 0, lpExistingFileName, -1, source, length,
                                NULL, NULL );
    sourcePS.CloseBuffer(src_size);
    if( src_size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpExistingFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        goto done;
    }

    length = 0;
    if (lpNewFileName != NULL)
    {
        length = (PAL_wcslen(lpNewFileName)+1) * MaxWCharToAcpLengthFactor;
    }
    
    dest = destPS.OpenStringBuffer(length);
    if (NULL == dest)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    dest_size = WideCharToMultiByte( CP_ACP, 0, lpNewFileName, -1, dest, length,
                                NULL, NULL );
    destPS.CloseBuffer(dest_size);
    
    if( dest_size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpNewFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        goto done;
    }

    bRet = MoveFileExA(source,dest,dwFlags);

done:
    LOGEXIT("MoveFileExW returns BOOL %d\n", bRet);
    PERF_EXIT(MoveFileExW);
    return bRet;
}

/*++
Function:
  GetFileAttributesA

Note:
  Checking for directory and read-only file, according to Rotor spec.

Caveats:
  There are some important things to note about this implementation, which
are due to the differences between the FAT filesystem and Unix filesystems:

- fifo's, sockets, and symlinks will return -1, and GetLastError() will
  return ERROR_ACCESS_DENIED

- if a file is write-only, or has no permissions at all, it is treated
  the same as if it had mode 'rw'. This is consistent with behaviour on
  NTFS files with the same permissions.

- the following flags will never be returned:

FILE_ATTRIBUTE_SYSTEM
FILE_ATTRIBUTE_ARCHIVE
FILE_ATTRIBUTE_HIDDEN

--*/
uint32_t
GetFileAttributesA(
            LPCSTR lpFileName)
{
    CPalThread *pThread;
    struct stat stat_data;
    uint32_t dwAttr = 0;
    uint32_t dwLastError = 0;
    char * UnixFileName;
    int length = 0;
    PathCharString UnixFileNamePS;

    PERF_ENTRY(GetFileAttributesA);
    ENTRY("GetFileAttributesA(lpFileName=%p (%s))\n", lpFileName?lpFileName:"NULL", lpFileName?lpFileName:"NULL");

    pThread = InternalGetCurrentThread();
    if (lpFileName == NULL)
    {
        dwLastError = ERROR_PATH_NOT_FOUND;
        goto done;
    }

    length = strlen(lpFileName);
    
    UnixFileName = UnixFileNamePS.OpenStringBuffer(length);
    if (NULL == UnixFileName)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    strcpy_s( UnixFileName, UnixFileNamePS.GetSizeOf(), lpFileName );
    UnixFileNamePS.CloseBuffer(length);
    
    FILEDosToUnixPathA( UnixFileName );

    if ( stat(UnixFileName, &stat_data) != 0 )
    {
        dwLastError = FILEGetLastErrorFromErrnoAndFilename(UnixFileName);
        goto done;
    }

    if ( (stat_data.st_mode & S_IFMT) == S_IFDIR )
    {
        dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
    }
    else if ( (stat_data.st_mode & S_IFMT) != S_IFREG )
    {
        ERROR("Not a regular file or directory, S_IFMT is %#x\n", 
              stat_data.st_mode & S_IFMT);
        dwLastError = ERROR_ACCESS_DENIED;
        goto done;
    }

    if ( UTIL_IsReadOnlyBitsSet( &stat_data ) )
    {
        dwAttr |= FILE_ATTRIBUTE_READONLY;
    }

    /* finally, if nothing is set... */
    if ( dwAttr == 0 )
    {
        dwAttr = FILE_ATTRIBUTE_NORMAL;
    }

done:
    if (dwLastError)
    {
        pThread->SetLastError(dwLastError);
        dwAttr = INVALID_FILE_ATTRIBUTES;
    }

    LOGEXIT("GetFileAttributesA returns DWORD %#x\n", dwAttr);
    PERF_EXIT(GetFileAttributesA);
    return dwAttr;
}




/*++
Function:
  GetFileAttributesW

Note:
  Checking for directory and read-only file

See MSDN doc.
--*/
uint32_t
GetFileAttributesW(
            LPCWSTR lpFileName)
{
    CPalThread *pThread;
    int   size;
    PathCharString filenamePS;
    int length = 0;
    char * filename;
    uint32_t dwRet = (uint32_t) -1;

    PERF_ENTRY(GetFileAttributesW);
    ENTRY("GetFileAttributesW(lpFileName=%p (%S))\n",
          lpFileName?lpFileName:W16_NULLSTRING,
          lpFileName?lpFileName:W16_NULLSTRING);

    pThread = InternalGetCurrentThread();
    if (lpFileName == NULL) 
    {
        pThread->SetLastError(ERROR_PATH_NOT_FOUND);
        goto done;
    }
    
    length = (PAL_wcslen(lpFileName)+1) * MaxWCharToAcpLengthFactor;
    filename = filenamePS.OpenStringBuffer(length);
    if (NULL == filename)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    size = WideCharToMultiByte( CP_ACP, 0, lpFileName, -1, filename, length,
                                NULL, NULL );
    filenamePS.CloseBuffer(size);
    
    if( size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
    }
    else
        dwRet = GetFileAttributesA( filename );
done:
    LOGEXIT("GetFileAttributesW returns DWORD %#x\n", dwRet);
    PERF_EXIT(GetFileAttributesW);
    return dwRet;
}


/*++
Function:
  GetFileAttributesExW

See MSDN doc, and notes for GetFileAttributesW.
--*/
BOOL
GetFileAttributesExW(
              LPCWSTR lpFileName,
              GET_FILEEX_INFO_LEVELS fInfoLevelId,
              void * lpFileInformation)
{
    CPalThread *pThread;
    BOOL bRet = FALSE;
    uint32_t dwLastError = 0;
    LPWIN32_FILE_ATTRIBUTE_DATA attr_data;

    struct stat stat_data;

    char * name;
    PathCharString namePS;
    int length = 0;
    int  size;

    PERF_ENTRY(GetFileAttributesExW);
    ENTRY("GetFileAttributesExW(lpFileName=%p (%S), fInfoLevelId=%d, "
          "lpFileInformation=%p)\n", lpFileName?lpFileName:W16_NULLSTRING, lpFileName?lpFileName:W16_NULLSTRING,
          fInfoLevelId, lpFileInformation);

    pThread = InternalGetCurrentThread();
    if ( fInfoLevelId != GetFileExInfoStandard )
    {
        ASSERT("Unrecognized value for fInfoLevelId=%d\n", fInfoLevelId);
        dwLastError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    if ( !lpFileInformation )
    {
        ASSERT("lpFileInformation is NULL\n");
        dwLastError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    if (lpFileName == NULL) 
    {
        dwLastError = ERROR_PATH_NOT_FOUND;
        goto done;
    }
    
    length = (PAL_wcslen(lpFileName)+1) * MaxWCharToAcpLengthFactor;
    name = namePS.OpenStringBuffer(length);
    if (NULL == name)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    size = WideCharToMultiByte( CP_ACP, 0, lpFileName, -1, name, length,
                                NULL, NULL );
    namePS.CloseBuffer(size);
    
    if( size == 0 )
    {
        dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            dwLastError = ERROR_FILENAME_EXCED_RANGE;
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            dwLastError = ERROR_INTERNAL_ERROR;
        }
        goto done;
    }

    attr_data = (LPWIN32_FILE_ATTRIBUTE_DATA)lpFileInformation;

    attr_data->dwFileAttributes = GetFileAttributesW(lpFileName);
    /* assume that GetFileAttributes will call SetLastError appropriately */
    if ( attr_data->dwFileAttributes == (uint32_t)-1 )
    {
        goto done;
    }

    FILEDosToUnixPathA(name);
    /* do the stat */
    if ( stat(name, &stat_data) != 0 )
    {
        ERROR("stat failed on %S\n", lpFileName);
        dwLastError = FILEGetLastErrorFromErrnoAndFilename(name);
        goto done;
    }

    /* get the file times */
    attr_data->ftCreationTime =
        FILEUnixTimeToFileTime( stat_data.st_ctime,
                                ST_CTIME_NSEC(&stat_data) );
    attr_data->ftLastAccessTime =
        FILEUnixTimeToFileTime( stat_data.st_atime,
                                ST_ATIME_NSEC(&stat_data) );
    attr_data->ftLastWriteTime =
        FILEUnixTimeToFileTime( stat_data.st_mtime,
                                ST_MTIME_NSEC(&stat_data) );

    /* Get the file size. GetFileSize is not used because it gets the
       size of an already-open file */
    attr_data->nFileSizeLow = (uint32_t) stat_data.st_size;
#if SIZEOF_OFF_T > 4
    attr_data->nFileSizeHigh = (uint32_t)(stat_data.st_size >> 32);
#else
    attr_data->nFileSizeHigh = 0;
#endif

    bRet = TRUE;

done:
    if (dwLastError) pThread->SetLastError(dwLastError);

    LOGEXIT("GetFileAttributesExW returns BOOL %d\n", bRet);
    PERF_EXIT(GetFileAttributesExW);
    return bRet;
}

/*++
Function:
  SetFileAttributesW

Notes:
  Used for setting read-only attribute on file only.

--*/
BOOL
SetFileAttributesW(
            LPCWSTR lpFileName,
            uint32_t dwFileAttributes)
{
    CPalThread *pThread;
    char * name;
    PathCharString namePS;
    int length = 0;
    int  size;

    uint32_t dwLastError = 0;
    BOOL  bRet = FALSE;

    PERF_ENTRY(SetFileAttributesW);
    ENTRY("SetFileAttributesW(lpFileName=%p (%S), dwFileAttributes=%#x)\n",
        lpFileName?lpFileName:W16_NULLSTRING,
        lpFileName?lpFileName:W16_NULLSTRING, dwFileAttributes);
    
    pThread = InternalGetCurrentThread();
    if (lpFileName == NULL) 
    {
        dwLastError = ERROR_PATH_NOT_FOUND;
        goto done;
    }
    
    length = (PAL_wcslen(lpFileName)+1) * MaxWCharToAcpLengthFactor;
    name = namePS.OpenStringBuffer(length);
    if (NULL == name)
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }
    size = WideCharToMultiByte( CP_ACP, 0, lpFileName, -1, name, length,
                                NULL, NULL );
    namePS.CloseBuffer(size);
    
    if( size == 0 )
    {
        dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpFileName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            dwLastError = ERROR_FILENAME_EXCED_RANGE;
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            dwLastError = ERROR_INVALID_PARAMETER;
        }
        goto done;
    }
    bRet = SetFileAttributesA(name,dwFileAttributes);

done:
    if (dwLastError) pThread->SetLastError(dwLastError);

    LOGEXIT("SetFileAttributes returns BOOL %d\n", bRet);
    PERF_EXIT(SetFileAttributesW);
    return bRet;
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

#if WRITE_0_BYTES_HANGS_TTY
    if( nNumberOfBytesToWrite == 0 && isatty(ifd) )
    {
        res = 0;
        *lpNumberOfBytesWritten = 0;
        goto done;
    }
#endif

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
    
    PERF_ENTRY(WriteFile);
    ENTRY("WriteFile(hFile=%p, lpBuffer=%p, nToWrite=%u, lpWritten=%p, "
          "lpOverlapped=%p)\n", hFile, lpBuffer, nNumberOfBytesToWrite, 
          lpNumberOfBytesWritten, lpOverlapped);

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
    PERF_EXIT(WriteFile);
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
    
    PERF_ENTRY(ReadFile);
    ENTRY("ReadFile(hFile=%p, lpBuffer=%p, nToRead=%u, "
          "lpRead=%p, lpOverlapped=%p)\n",
          hFile, lpBuffer, nNumberOfBytesToRead, 
          lpNumberOfBytesRead, lpOverlapped);

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
    PERF_EXIT(ReadFile);
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

    PERF_ENTRY(GetStdHandle);
    ENTRY("GetStdHandle(nStdHandle=%#x)\n", nStdHandle);

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
    PERF_EXIT(GetStdHandle);
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

#if SIZEOF_OFF_T > 4
#if !HAVE_FTRUNCATE_LARGE_LENGTH_SUPPORT
    // ftruncate will return the wrong value for some large lengths.
    // We'll short-circuit the process and simply return failure for
    // the set of values that covers those cases, all of which would
    // have failed anyway on any standard-sized hard drive.
    if (curr >= 0xFFFFFFFF000ULL)
    {
        ERROR("Skipping ftruncate because the offset is too large\n");
        palError = ERROR_INVALID_PARAMETER;
        goto InternalSetEndOfFileExit;
    }
#endif  // !HAVE_FTRUNCATE_LARGE_LENGTH_SUPPORT
#endif  // SIZEOF_OFF_T

#if HAS_FTRUNCATE_LENGTH_ISSUE
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
#endif // HAS_FTRUNCATE_LENGTH_ISSUE

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
    if (palError == ERROR_DISK_FULL && (static_cast<UINT64>(curr) > 0x00000fffffff0000ULL ) )
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

    PERF_ENTRY(SetEndOfFile);
    ENTRY("SetEndOfFile(hFile=%p)\n", hFile);

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
    PERF_EXIT(SetEndOfFile);
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
    __int64 seek_offset = 0LL;
    __int64 seek_res = 0LL;
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
        seek_offset = ((__int64)*lpDistanceToMoveHigh << 32);
        
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

    seek_res = (__int64)lseek( iUnixFd,
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

/*++
Function:
  SetFilePointer

See MSDN doc.
--*/
uint32_t
SetFilePointer(
            HANDLE hFile,
            int32_t lDistanceToMove,
            int32_t * lpDistanceToMoveHigh,
            uint32_t dwMoveMethod)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    int32_t lNewFilePointerLow = 0;

    PERF_ENTRY(SetFilePointer);
    ENTRY("SetFilePointer(hFile=%p, lDistance=%d, lpDistanceHigh=%p, "
          "dwMoveMethod=%#x)\n", hFile, lDistanceToMove,
          lpDistanceToMoveHigh, dwMoveMethod);

    pThread = InternalGetCurrentThread();

    palError = InternalSetFilePointer(
        pThread,
        hFile,
        lDistanceToMove,
        lpDistanceToMoveHigh,
        dwMoveMethod,
        &lNewFilePointerLow
        );

    if (NO_ERROR != palError)
    {
        lNewFilePointerLow = INVALID_SET_FILE_POINTER;
    }

    /* This function must always call SetLastError - even if successful. 
       If we seek to a value greater than 2^32 - 1, we will effectively be
       returning a negative value from this function. Now, let's say that
       returned value is -1. Furthermore, assume that win32error has been 
       set before even entering this function. Then, when this function 
       returns to SetFilePointer in win32native.cs, it will have returned 
       -1 and win32error will have been set, which will cause an error to be
       returned. Since -1 may not be an error in this case and since we 
       can't assume that the win32error is related to SetFilePointer, 
       we need to always call SetLastError here. That way, if this function 
       succeeds, SetFilePointer in win32native won't mistakenly determine 
       that it failed. */
    pThread->SetLastError(palError);
    
    LOGEXIT("SetFilePointer returns DWORD %#x\n", lNewFilePointerLow);
    PERF_EXIT(SetFilePointer);
    return lNewFilePointerLow;
}

/*++
Function:
  SetFilePointerEx

See MSDN doc.
--*/
BOOL
SetFilePointerEx(
            HANDLE hFile,
            LARGE_INTEGER liDistanceToMove,
            PLARGE_INTEGER lpNewFilePointer,
            uint32_t dwMoveMethod)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    BOOL Ret = FALSE;

    PERF_ENTRY(SetFilePointerEx);
    ENTRY("SetFilePointerEx(hFile=%p, liDistanceToMove=0x%llx, "
           "lpNewFilePointer=%p (0x%llx), dwMoveMethod=0x%x)\n", hFile, 
           liDistanceToMove.QuadPart, lpNewFilePointer, 
           (lpNewFilePointer) ? (*lpNewFilePointer).QuadPart : 0, dwMoveMethod);

    int32_t lDistanceToMove;
    lDistanceToMove = (int32_t)liDistanceToMove.u.LowPart;
    int32_t lDistanceToMoveHigh;
    lDistanceToMoveHigh = liDistanceToMove.u.HighPart;

    int32_t lNewFilePointerLow = 0;

    pThread = InternalGetCurrentThread();

    palError = InternalSetFilePointer(
        pThread,
        hFile,
        lDistanceToMove,
        &lDistanceToMoveHigh,
        dwMoveMethod,
        &lNewFilePointerLow
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }
    else
    {
        if (lpNewFilePointer != NULL)
        {
            lpNewFilePointer->u.LowPart = (uint32_t)lNewFilePointerLow;
            lpNewFilePointer->u.HighPart = (uint32_t)lDistanceToMoveHigh;
        }
        Ret = TRUE;
    }
    
    LOGEXIT("SetFilePointerEx returns BOOL %d\n", Ret);
    PERF_EXIT(SetFilePointerEx);
    return Ret;
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
#if SIZEOF_OFF_T > 4
        *pdwFileSizeHigh = (uint32_t)(stat_data.st_size >> 32);
#else
        *pdwFileSizeHigh = 0;
#endif
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

/*++
Function:
  GetFileSize

See MSDN doc.
--*/
uint32_t
GetFileSize(
         HANDLE hFile,
         uint32_t * lpFileSizeHigh)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    uint32_t dwFileSizeLow;

    PERF_ENTRY(GetFileSize);
    ENTRY("GetFileSize(hFile=%p, lpFileSizeHigh=%p)\n", hFile, lpFileSizeHigh);

    pThread = InternalGetCurrentThread();

    palError = InternalGetFileSize(
        pThread,
        hFile, 
        &dwFileSizeLow,
        lpFileSizeHigh
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
        dwFileSizeLow = INVALID_FILE_SIZE;
    }

    LOGEXIT("GetFileSize returns DWORD %u\n", dwFileSizeLow);
    PERF_EXIT(GetFileSize);
    return dwFileSizeLow;
}

/*++
Function:
GetFileSizeEx

See MSDN doc.
--*/
BOOL
GetFileSizeEx(
   HANDLE hFile,
  PLARGE_INTEGER lpFileSize)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    uint32_t dwFileSizeHigh;
    uint32_t dwFileSizeLow;

    PERF_ENTRY(GetFileSizeEx);
    ENTRY("GetFileSizeEx(hFile=%p, lpFileSize=%p)\n", hFile, lpFileSize);

    pThread = InternalGetCurrentThread();

    if (lpFileSize != NULL)
    {
        palError = InternalGetFileSize(
            pThread,
            hFile,
            &dwFileSizeLow,
            &dwFileSizeHigh
            );

        lpFileSize->u.LowPart = dwFileSizeLow;
        lpFileSize->u.HighPart = dwFileSizeHigh;
    }
    else
    {
        palError = ERROR_INVALID_PARAMETER;
    }

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("GetFileSizeEx returns BOOL %d\n", NO_ERROR == palError);
    PERF_EXIT(GetFileSizeEx);
    return NO_ERROR == palError;
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

#if HAVE_FSYNC || defined(__APPLE__)
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
#else // HAVE_FSYNC
    /* flush all buffers out to disk - there is no way to flush
       an individual file descriptor's buffers out. */
    sync();
#endif // HAVE_FSYNC else
    

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

    PERF_ENTRY(FlushFileBuffers);
    ENTRY("FlushFileBuffers(hFile=%p)\n", hFile);

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
    PERF_EXIT(FlushFileBuffers);
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


/*++
Function:
  GetFileType

See MSDN doc.

--*/
uint32_t
GetFileType(
         HANDLE hFile)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread;
    uint32_t dwFileType;

    PERF_ENTRY(GetFileType);
    ENTRY("GetFileType(hFile=%p)\n", hFile);

    pThread = InternalGetCurrentThread();

    palError = InternalGetFileType(
        pThread,
        hFile,
        &dwFileType
        );

    if (NO_ERROR != palError)
    {
        dwFileType = FILE_TYPE_UNKNOWN;
        pThread->SetLastError(palError);
    }
    else if (FILE_TYPE_UNKNOWN == dwFileType)
    {
        pThread->SetLastError(palError);
    }


    LOGEXIT("GetFileType returns DWORD %#x\n", dwFileType);
    PERF_EXIT(GetFileType);
    return dwFileType;
}

#define ENSURE_UNIQUE_NOT_ZERO \
    if ( uUniqueSeed == 0 ) \
    {\
        uUniqueSeed++;\
    }

/*++
 Function:
   GetTempFileNameA

uUnique is always 0.
 --*/
const int MAX_PREFIX        = 3;
const int MAX_SEEDSIZE      = 8; /* length of "unique portion of 
                                   the string, plus extension(FFFF.TMP). */
static unsigned short uUniqueSeed   = 0;
static BOOL IsInitialized   = FALSE;

uint32_t
GetTempFileNameA(
                  LPCSTR lpPathName,
                  LPCSTR lpPrefixString,
                  uint32_t   uUnique,
                  LPSTR lpTempFileName)
{
    CPalThread *pThread;
    char * full_name;
    PathCharString full_namePS;
    int length;
    char * file_template;
    PathCharString file_templatePS;
    char    chLastPathNameChar;
 
    HANDLE  hTempFile;
    uint32_t    uRet = 0;
    uint32_t   dwError;
    unsigned short  uLoopCounter = 0;

    PERF_ENTRY(GetTempFileNameA);
    ENTRY("GetTempFileNameA(lpPathName=%p (%s), lpPrefixString=%p (%s), uUnique=%u, " 
          "lpTempFileName=%p)\n",  lpPathName?lpPathName:"NULL",  lpPathName?lpPathName:"NULL", 
        lpPrefixString?lpPrefixString:"NULL", 
        lpPrefixString?lpPrefixString:"NULL", uUnique, 
        lpTempFileName?lpTempFileName:"NULL");

    pThread = InternalGetCurrentThread();
    if ( !IsInitialized )
    {
        uUniqueSeed = (unsigned short)( time( NULL ) );
    
        /* On the off chance 0 is returned.
        0 being the error return code.  */
        ENSURE_UNIQUE_NOT_ZERO
        IsInitialized = TRUE;
    }

    if ( !lpPathName || *lpPathName == '\0' )
    {
       pThread->SetLastError( ERROR_DIRECTORY );
       goto done;
    }

    if ( NULL == lpTempFileName )  
    {
        ERROR( "lpTempFileName cannot be NULL\n" );
        pThread->SetLastError( ERROR_INVALID_PARAMETER );
        goto done;
    }

    if ( strlen( lpPathName ) + MAX_SEEDSIZE + MAX_PREFIX >= MAX_LONGPATH ) 
    {
        WARN( "File names larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH );
        pThread->SetLastError( ERROR_FILENAME_EXCED_RANGE );
        goto done;
    }

    length = strlen(lpPathName) + MAX_SEEDSIZE + MAX_PREFIX + 10;
    file_template = file_templatePS.OpenStringBuffer(length);
    if (NULL == file_template)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    *file_template = '\0';
    strcat_s( file_template, file_templatePS.GetSizeOf(), lpPathName );
    file_templatePS.CloseBuffer(length);

    chLastPathNameChar = file_template[strlen(file_template)-1];
    if (chLastPathNameChar != '\\' && chLastPathNameChar != '/')
    {
        strcat_s( file_template, file_templatePS.GetSizeOf(), "\\" );
    }
    
    if ( lpPrefixString )
    {
        strncat_s( file_template, file_templatePS.GetSizeOf(), lpPrefixString, MAX_PREFIX );
    }
    FILEDosToUnixPathA( file_template );
    strncat_s( file_template, file_templatePS.GetSizeOf(), "%.4x.TMP", MAX_SEEDSIZE );

    /* Create the file. */
    dwError = GetLastError();
    pThread->SetLastError( NOERROR );

    length = strlen(file_template) + MAX_SEEDSIZE + MAX_PREFIX;
    full_name = full_namePS.OpenStringBuffer(length);
    if (NULL == full_name)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }
    sprintf_s( full_name, full_namePS.GetSizeOf(), file_template, (0 == uUnique) ? uUniqueSeed : uUnique);
    full_namePS.CloseBuffer(length);
    
    hTempFile = CreateFileA( full_name, GENERIC_WRITE, 
                             FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL );
    
    if (uUnique == 0)
    {
        /* The unsigned short will overflow back to 0 if we go past
        65536 files, so break the loop after 65536 iterations.
        If the CreateFile call was not successful within that 
        number of iterations, then there are no temp file names
        left for that directory. */
        while ( ERROR_PATH_NOT_FOUND != GetLastError() && 
                INVALID_HANDLE_VALUE == hTempFile && uLoopCounter < 0xFFFF )
        {
            uUniqueSeed++;
            ENSURE_UNIQUE_NOT_ZERO;

            pThread->SetLastError( NOERROR );
            sprintf_s( full_name, full_namePS.GetSizeOf(), file_template, uUniqueSeed );
            hTempFile = CreateFileA( full_name, GENERIC_WRITE, 
                                    FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL );
            uLoopCounter++;
        
        }
    }

    /* Reset the error code.*/
    if ( NOERROR == GetLastError() )
    {
        pThread->SetLastError( dwError );
    }

    /* Windows sets ERROR_FILE_EXISTS,if there
    are no available temp files. */
    if ( INVALID_HANDLE_VALUE != hTempFile )
    {
        if (0 == uUnique)
        {
            uRet = uUniqueSeed;
            uUniqueSeed++;
            ENSURE_UNIQUE_NOT_ZERO;
        }
        else
        {
            uRet = uUnique;
        }
        
        if ( CloseHandle( hTempFile ) )
        {
            if (strcpy_s( lpTempFileName, MAX_LONGPATH, full_name ) != SAFECRT_SUCCESS)
            {
                ERROR( "strcpy_s failed!\n");
                pThread->SetLastError( ERROR_FILENAME_EXCED_RANGE );
                *lpTempFileName = '\0';
                uRet = 0;
            }
        }
        else  
        {
            ASSERT( "Unable to close the handle %p\n", hTempFile );
            pThread->SetLastError( ERROR_INTERNAL_ERROR );
            *lpTempFileName = '\0';
            uRet = 0;
        }
    }
    else if ( INVALID_HANDLE_VALUE == hTempFile && uLoopCounter < 0xFFFF )
    {
        ERROR( "Unable to create temp file. \n" );
        uRet = 0;
        
        if ( ERROR_PATH_NOT_FOUND == GetLastError() )
        {
            /* CreateFile failed because it could not 
            find the path. */
            pThread->SetLastError( ERROR_DIRECTORY );
        } /* else use the lasterror value from CreateFileA */
    }
    else
    {
        TRACE( "65535 files already exist in the directory. "
               "No temp files available for creation.\n" );
        pThread->SetLastError( ERROR_FILE_EXISTS );
    }

done:
    LOGEXIT("GetTempFileNameA returns UINT %u\n", uRet);
    PERF_EXIT(GetTempFileNameA);
    return uRet;
       
}
        
/*++
Function:
  GetTempFileNameW

uUnique is always 0.
--*/
uint32_t
GetTempFileNameW(
          LPCWSTR lpPathName,
          LPCWSTR lpPrefixString,
          uint32_t uUnique,
          LPWSTR lpTempFileName)
{
    CPalThread *pThread;
    int32_t path_size = 0;
    int32_t prefix_size = 0;
    char * full_name;
    char * prefix_string;
    char * tempfile_name;
    PathCharString full_namePS, prefix_stringPS;
    int32_t length = 0;
    uint32_t   uRet;

    PERF_ENTRY(GetTempFileNameW);
    ENTRY("GetTempFileNameW(lpPathName=%p (%S), lpPrefixString=%p (%S), uUnique=%u, "
          "lpTempFileName=%p)\n", lpPathName?lpPathName:W16_NULLSTRING, lpPathName?lpPathName:W16_NULLSTRING,
          lpPrefixString?lpPrefixString:W16_NULLSTRING,
          lpPrefixString?lpPrefixString:W16_NULLSTRING,uUnique, lpTempFileName);

    pThread = InternalGetCurrentThread();
    /* Sanity checks. */
    if ( !lpPathName || *lpPathName == '\0' )
    {
        pThread->SetLastError( ERROR_DIRECTORY );
        uRet = 0;
        goto done;
    }

    length = (PAL_wcslen(lpPathName)+1) * MaxWCharToAcpLengthFactor;
    full_name = full_namePS.OpenStringBuffer(length);
    if (NULL == full_name)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        uRet = 0;
        goto done;
    }
    path_size = WideCharToMultiByte( CP_ACP, 0, lpPathName, -1, full_name,
                                     length, NULL, NULL );
    full_namePS.CloseBuffer(path_size);
                                     
    if( path_size == 0 )
    {
        uint32_t dwLastError = GetLastError();
        if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
        {
            WARN("lpPathName is larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
            pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
        }
        else
        {
            ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
            pThread->SetLastError(ERROR_INTERNAL_ERROR);
        }
        uRet = 0;
        goto done;
    }
    
    if (lpPrefixString != NULL) 
    {
        length = (PAL_wcslen(lpPrefixString)+1) * MaxWCharToAcpLengthFactor;
        prefix_string = prefix_stringPS.OpenStringBuffer(length);
        if (NULL == prefix_string)
        {
            pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            uRet = 0;
            goto done;
        }
        prefix_size = WideCharToMultiByte( CP_ACP, 0, lpPrefixString, -1, 
                                           prefix_string,
                                           MAX_LONGPATH - path_size - MAX_SEEDSIZE, 
                                           NULL, NULL );
        prefix_stringPS.CloseBuffer(prefix_size);
        
        if( prefix_size == 0 )
        {
            uint32_t dwLastError = GetLastError();
            if( dwLastError == ERROR_INSUFFICIENT_BUFFER )
            {
                WARN("Full name would be larger than MAX_LONGPATH (%d)!\n", MAX_LONGPATH);
                pThread->SetLastError(ERROR_FILENAME_EXCED_RANGE);
            }
            else
            {
                ASSERT("WideCharToMultiByte failure! error is %d\n", dwLastError);
                pThread->SetLastError(ERROR_INTERNAL_ERROR);
            }
            uRet = 0;
            goto done;
        }
    }
    
    tempfile_name = (char*)InternalMalloc(MAX_LONGPATH);
    if (tempfile_name == NULL)
    {
        pThread->SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        uRet = 0;
        goto done;
    }
    
    uRet = GetTempFileNameA(full_name, 
                            (lpPrefixString == NULL) ? NULL : prefix_string,
                            0, tempfile_name);
    if (uRet)
    {                    
        path_size = MultiByteToWideChar( CP_ACP, 0, tempfile_name, -1, 
                                           lpTempFileName, MAX_LONGPATH );

        InternalFree(tempfile_name);
        tempfile_name = NULL;
        if (!path_size)
        {
            uint32_t dwLastError = GetLastError();
            if (dwLastError == ERROR_INSUFFICIENT_BUFFER)
            {
                WARN("File names larger than MAX_PATH_FNAME (%d)! \n", MAX_LONGPATH);
                dwLastError = ERROR_FILENAME_EXCED_RANGE;
            }
            else
            {
                ASSERT("MultiByteToWideChar failure! error is %d", dwLastError);     
                dwLastError = ERROR_INTERNAL_ERROR;
            }
            pThread->SetLastError(dwLastError);
            uRet = 0;
        }
    }

done:
    LOGEXIT("GetTempFileNameW returns UINT %u\n", uRet);
    PERF_EXIT(GetTempFileNameW);
    return uRet;
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
Function:
  CopyFileA

See MSDN doc.

Notes:
  There are several (most) error paths here that do not call SetLastError().
This is because we know that CreateFile, ReadFile, and WriteFile will do so,
and will have a much better idea of the specific error.
--*/
BOOL
CopyFileA(
       LPCSTR lpExistingFileName,
       LPCSTR lpNewFileName,
       BOOL bFailIfExists)
{
    CPalThread *pThread;
    HANDLE       hSource = INVALID_HANDLE_VALUE;
    HANDLE       hDest = INVALID_HANDLE_VALUE;
    uint32_t        dwDestCreationMode;
    BOOL         bGood = FALSE;
    uint32_t        dwSrcFileAttributes;
    struct stat  SrcFileStats;
    
    LPSTR lpUnixPath = NULL;
    const int    buffer_size = 16*1024;
    char        *buffer = (char*)alloca(buffer_size);
    uint32_t        bytes_read;
    uint32_t        bytes_written;
    int          permissions;


    PERF_ENTRY(CopyFileA);
    ENTRY("CopyFileA(lpExistingFileName=%p (%s), lpNewFileName=%p (%s), bFailIfExists=%d)\n",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpExistingFileName?lpExistingFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL",
          lpNewFileName?lpNewFileName:"NULL", bFailIfExists);

    pThread = InternalGetCurrentThread();
    if ( bFailIfExists )
    {
        dwDestCreationMode = CREATE_NEW;
    }
    else
    {
        dwDestCreationMode = CREATE_ALWAYS;
    }
    
    hSource = CreateFileA( lpExistingFileName,
               GENERIC_READ,
               FILE_SHARE_READ,
               NULL,
               OPEN_EXISTING,
               0,
               NULL );

    if ( hSource == INVALID_HANDLE_VALUE )
    {
        ERROR("CreateFileA failed for %s\n", lpExistingFileName);
        goto done;
    }

    /* Need to preserve the file attributes */
    dwSrcFileAttributes = GetFileAttributesA(lpExistingFileName);
    if (dwSrcFileAttributes == 0xffffffff)
    {
        ERROR("GetFileAttributes failed for %s\n", lpExistingFileName);
        goto done;
    }

    /* Need to preserve the owner/group and chmod() flags */
    lpUnixPath = InternalStrdup(lpExistingFileName);
    if ( lpUnixPath == NULL )
    {
        ERROR("InternalStrdup() failed\n");
        pThread->SetLastError(FILEGetLastErrorFromErrno());
        goto done;
    }
    FILEDosToUnixPathA(lpUnixPath);
    if (stat (lpUnixPath, &SrcFileStats) == -1)
    {
        ERROR("stat() failed for %s\n", lpExistingFileName);
        pThread->SetLastError(FILEGetLastErrorFromErrnoAndFilename(lpUnixPath));
        goto done;
    }

    hDest = CreateFileA( lpNewFileName,
             GENERIC_WRITE,
             FILE_SHARE_READ,
             NULL,
             dwDestCreationMode,
             0,
             NULL );

    if ( hDest == INVALID_HANDLE_VALUE )
    {
        ERROR("CreateFileA failed for %s\n", lpNewFileName);    
        goto done;
    }

    InternalFree(lpUnixPath);
    lpUnixPath = InternalStrdup(lpNewFileName);
    if ( lpUnixPath == NULL )
    {
        ERROR("InternalStrdup() failed\n");
        pThread->SetLastError(FILEGetLastErrorFromErrno());
        goto done;
    }
    FILEDosToUnixPathA( lpUnixPath );
    
 
    // We don't set file attributes in CreateFile. The only attribute
    // that is reflected on disk in Unix is read-only, and we set that
    // here.
    permissions = (S_IRWXU | S_IRWXG | S_IRWXO);
    if ((dwSrcFileAttributes & FILE_ATTRIBUTE_READONLY) != 0)
    {
        permissions &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    }

    /* Make sure the new file has the same chmod() flags. */
    if (chmod(lpUnixPath, SrcFileStats.st_mode & permissions) == -1)
    {
        WARN ("chmod() failed to set mode 0x%x on new file\n",
              SrcFileStats.st_mode & permissions);
        pThread->SetLastError(FILEGetLastErrorFromErrnoAndFilename(lpUnixPath));
        goto done;
    }

    while( (bGood = ReadFile( hSource, buffer, buffer_size, &bytes_read, NULL ))
           && bytes_read > 0 )
    {
        bGood = ( WriteFile( hDest, buffer, bytes_read, &bytes_written, NULL )
          && bytes_written == bytes_read);
        if (!bGood) break;
    }

    if (!bGood)
    {
        ERROR("Copy failed\n");

        if ( !CloseHandle(hDest) ||
             !DeleteFileA(lpNewFileName) )
        {
            ERROR("Unable to clean up partial copy\n");
        }
        hDest = INVALID_HANDLE_VALUE;

        goto done;
    }
    
done:

    if ( hSource != INVALID_HANDLE_VALUE ) 
    {
        CloseHandle( hSource );
    }
    if ( hDest != INVALID_HANDLE_VALUE )
    {
        CloseHandle( hDest );
    }
    if (lpUnixPath) 
    {
        InternalFree(lpUnixPath);
    }

    LOGEXIT("CopyFileA returns BOOL %d\n", bGood);
    PERF_EXIT(CopyFileA);
    return bGood;
}


/*++
Function:
  SetFileAttributesA

Notes:
  Used for setting read-only attribute on file only.

--*/
BOOL
SetFileAttributesA(
            LPCSTR lpFileName,
            uint32_t dwFileAttributes)
{
    CPalThread *pThread;
    struct stat stat_data;
    mode_t new_mode;

    uint32_t dwLastError = 0;
    BOOL  bRet = FALSE;
    LPSTR UnixFileName = NULL;

    PERF_ENTRY(SetFileAttributesA);
    ENTRY("SetFileAttributesA(lpFileName=%p (%s), dwFileAttributes=%#x)\n",
        lpFileName?lpFileName:"NULL",
        lpFileName?lpFileName:"NULL", dwFileAttributes);

    pThread = InternalGetCurrentThread();

    /* Windows behavior for SetFileAttributes is that any valid attributes
    are set on a file and any invalid attributes are ignored. SetFileAttributes 
    returns success and does not set an error even if some or all of the 
    attributes are invalid. If all the attributes are invalid, SetFileAttributes
    sets a file's attribute to NORMAL. */

    /* If dwFileAttributes does not contain READONLY or NORMAL, set it to NORMAL
    and print a warning message. */
    if ( !(dwFileAttributes & (FILE_ATTRIBUTE_READONLY |FILE_ATTRIBUTE_NORMAL)) )
    {
        dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
        WARN("dwFileAttributes(%#x) contains attributes that are either not supported "
            "or cannot be set via SetFileAttributes.\n");
    }
     
    if ( (dwFileAttributes & FILE_ATTRIBUTE_NORMAL) &&
         (dwFileAttributes != FILE_ATTRIBUTE_NORMAL) )
    {
        WARN("Ignoring FILE_ATTRIBUTE_NORMAL -- it must be used alone\n");
    }

    if (lpFileName == NULL)
    {
        dwLastError = ERROR_FILE_NOT_FOUND;
        goto done;
    }

    if ((UnixFileName = InternalStrdup(lpFileName)) == NULL)
    {
        ERROR("InternalStrdup() failed\n");
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
        goto done;
    }

    FILEDosToUnixPathA( UnixFileName );
    if ( stat(UnixFileName, &stat_data) != 0 )
    {
        TRACE("stat failed on %s; errno is %d (%s)\n", 
             UnixFileName, errno, strerror(errno));
        dwLastError = FILEGetLastErrorFromErrnoAndFilename(UnixFileName);
        goto done;
    }

    new_mode = stat_data.st_mode;
    TRACE("st_mode is %#x\n", new_mode);

    /* if we can't do GetFileAttributes on it, don't do SetFileAttributes */
    if ( !(new_mode & S_IFREG) && !(new_mode & S_IFDIR) )
    {
        ERROR("Not a regular file or directory, S_IFMT is %#x\n",
              new_mode & S_IFMT);
        dwLastError = ERROR_ACCESS_DENIED;
        goto done;    
    }

    /* set or unset the "read-only" attribute */
    if (dwFileAttributes & FILE_ATTRIBUTE_READONLY)
    {
        /* remove the write bit from everybody */
        new_mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    }
    else
    {
        /* give write permission to the owner if the owner
         * already has read permission */
        if ( new_mode & S_IRUSR )
        {
            new_mode |= S_IWUSR;
        }
    }
    TRACE("new mode is %#x\n", new_mode);

    bRet = TRUE;
    if ( new_mode != stat_data.st_mode )
    {
        if ( chmod(UnixFileName, new_mode) != 0 )
        {
            ERROR("chmod(%s, %#x) failed\n", UnixFileName, new_mode);
            dwLastError = FILEGetLastErrorFromErrnoAndFilename(UnixFileName);
            bRet = FALSE;
        }
    }

done:
    if (dwLastError)
    {
        pThread->SetLastError(dwLastError);
    }
    
    InternalFree(UnixFileName);

    LOGEXIT("SetFileAttributesA returns BOOL %d\n", bRet);
    PERF_EXIT(SetFileAttributesA);
    return bRet;
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

/*++
Function:
  CreatePipe

See MSDN doc.
--*/
BOOL
CreatePipe(
         PHANDLE hReadPipe,
         PHANDLE hWritePipe,
         LPSECURITY_ATTRIBUTES lpPipeAttributes,
         uint32_t nSize)
{
    PAL_ERROR palError;
    CPalThread *pThread;

    PERF_ENTRY(CreatePipe);
    ENTRY("CreatePipe(hReadPipe:%p, hWritePipe:%p, lpPipeAttributes:%p, nSize:%d\n",
          hReadPipe, hWritePipe, lpPipeAttributes, nSize);

    pThread = InternalGetCurrentThread();

    palError = InternalCreatePipe(
        pThread,
        hReadPipe,
        hWritePipe,
        lpPipeAttributes,
        nSize
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("CreatePipe return %s\n", NO_ERROR == palError ? "TRUE":"FALSE");
    PERF_EXIT(CreatePipe);
    return NO_ERROR == palError;
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


/*++
Function:
  LockFile

See MSDN doc.
--*/
BOOL
LockFile(HANDLE hFile,
         uint32_t dwFileOffsetLow,
         uint32_t dwFileOffsetHigh,
         uint32_t nNumberOfBytesToLockLow,
         uint32_t nNumberOfBytesToLockHigh)
{
    CPalThread *pThread;
    PAL_ERROR palError = NO_ERROR;

    PERF_ENTRY(LockFile);
    ENTRY("LockFile(hFile:%p, offsetLow:%u, offsetHigh:%u, nbBytesLow:%u,"
           " nbBytesHigh:%u\n", hFile, dwFileOffsetLow, dwFileOffsetHigh, 
          nNumberOfBytesToLockLow, nNumberOfBytesToLockHigh);

    pThread = InternalGetCurrentThread();

    palError = InternalLockFile(
        pThread,
        hFile,
        dwFileOffsetLow,
        dwFileOffsetHigh,
        nNumberOfBytesToLockLow,
        nNumberOfBytesToLockHigh
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("LockFile returns %s\n", NO_ERROR == palError ? "TRUE":"FALSE");
    PERF_EXIT(LockFile);
    return NO_ERROR == palError;
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
Function:
  UnlockFile

See MSDN doc.
--*/
BOOL
UnlockFile(HANDLE hFile,
           uint32_t dwFileOffsetLow,
           uint32_t dwFileOffsetHigh,
           uint32_t nNumberOfBytesToUnlockLow,
           uint32_t nNumberOfBytesToUnlockHigh)
{
    CPalThread *pThread;
    PAL_ERROR palError = NO_ERROR;

    PERF_ENTRY(UnlockFile);
    ENTRY("UnlockFile(hFile:%p, offsetLow:%u, offsetHigh:%u, nbBytesLow:%u,"
          "nbBytesHigh:%u\n", hFile, dwFileOffsetLow, dwFileOffsetHigh, 
          nNumberOfBytesToUnlockLow, nNumberOfBytesToUnlockHigh);

    pThread = InternalGetCurrentThread();

    palError = InternalUnlockFile(
        pThread,
        hFile,
        dwFileOffsetLow,
        dwFileOffsetHigh,
        nNumberOfBytesToUnlockLow,
        nNumberOfBytesToUnlockHigh
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT("UnlockFile returns %s\n", NO_ERROR == palError ? "TRUE" : "FALSE");
    PERF_EXIT(UnlockFile);
    return NO_ERROR == palError;
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


/*++
Function:
  GetFileInformationByHandle

See MSDN doc.
--*/
BOOL
GetFileInformationByHandle(
              HANDLE hFile,
              LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
{
    CPalThread *pThread;
    BOOL bRet = FALSE;
    uint32_t dwLastError = 0;

    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    uint32_t dwAttr = 0;
    struct stat stat_data;

    PERF_ENTRY(GetFileInformationByHandle);
    ENTRY("GetFileInformationByHandle(hFile=%p, lpFileInformation=%p)\n",
          hFile, lpFileInformation);

    pThread = InternalGetCurrentThread();

    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR( "Invalid file handle\n" );
        dwLastError = ERROR_INVALID_HANDLE;
        goto done;
    }

    dwLastError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        GENERIC_READ,
        &pFileObject
        );

    if (NO_ERROR != dwLastError)
    {
        goto done;
    }
    
    dwLastError = pFileObject->GetProcessLocalData(
        pThread,
        ReadLock, 
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != dwLastError)
    {
        goto done;
    }

    if ( fstat(pLocalData->unix_fd, &stat_data) != 0 )
    {
        if ((dwLastError = FILEGetLastErrorFromErrno()) == ERROR_INTERNAL_ERROR)
        {
            ASSERT("fstat() not expected to fail with errno:%d (%s)\n",
                   errno, strerror(errno));
        }
        goto done;
    }

    if ( (stat_data.st_mode & S_IFMT) == S_IFDIR )
    {
        dwAttr |= FILE_ATTRIBUTE_DIRECTORY;
    }
    else if ( (stat_data.st_mode & S_IFMT) != S_IFREG )
    {
        ERROR("Not a regular file or directory, S_IFMT is %#x\n", 
              stat_data.st_mode & S_IFMT);
        dwLastError = ERROR_ACCESS_DENIED;
        goto done;
    }

    if ( UTIL_IsReadOnlyBitsSet( &stat_data ) )
    {
        dwAttr |= FILE_ATTRIBUTE_READONLY;
    }

    /* finally, if nothing is set... */
    if ( dwAttr == 0 )
    {
        dwAttr = FILE_ATTRIBUTE_NORMAL;
    }

    lpFileInformation->dwFileAttributes = dwAttr;

    /* get the file times */
    lpFileInformation->ftCreationTime =
        FILEUnixTimeToFileTime( stat_data.st_ctime,
                                ST_CTIME_NSEC(&stat_data) );
    lpFileInformation->ftLastAccessTime =
        FILEUnixTimeToFileTime( stat_data.st_atime,
                                ST_ATIME_NSEC(&stat_data) );
    lpFileInformation->ftLastWriteTime =
        FILEUnixTimeToFileTime( stat_data.st_mtime,
                                ST_MTIME_NSEC(&stat_data) );

    lpFileInformation->dwVolumeSerialNumber = stat_data.st_dev;

    /* Get the file size. GetFileSize is not used because it gets the
       size of an already-open file */
    lpFileInformation->nFileSizeLow = (uint32_t) stat_data.st_size;
#if SIZEOF_OFF_T > 4
    lpFileInformation->nFileSizeHigh = (uint32_t)(stat_data.st_size >> 32);
#else
    lpFileInformation->nFileSizeHigh = 0;
#endif

    lpFileInformation->nNumberOfLinks = stat_data.st_nlink;
    lpFileInformation->nFileIndexHigh = 0;
    lpFileInformation->nFileIndexLow = stat_data.st_ino;

    bRet = TRUE;

done:
    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    if (dwLastError) pThread->SetLastError(dwLastError);

    LOGEXIT("GetFileInformationByHandle returns BOOL %d\n", bRet);
    PERF_EXIT(GetFileInformationByHandle);
    return bRet;
}
