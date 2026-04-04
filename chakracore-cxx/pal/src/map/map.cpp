//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    map.cpp

Abstract:

    Implementation of file mapping API.



--*/


#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/init.h"
#include "pal/critsect.h"
#include "pal/virtual.h"
#include "common.h"
#include "pal/map.hpp"
#include "pal/thread.hpp"
#include "pal/file.hpp"
#include <new>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(VIRTUAL);

//
// The mapping critical section guards access to the list
// of currently mapped views. If a thread needs to access
// both this critical section and the data for an object
// it must acquire the object data first. That is, a thread
// cannot acquire any other locks after taking hold of
// this critical section.
//

CRITICAL_SECTION mapping_critsec __attribute__((init_priority(200)));
LIST_ENTRY MappedViewList __attribute__((init_priority(200)));

static PAL_ERROR MAPGrowLocalFile(int32_t, uint32_t);
static PMAPPED_VIEW_LIST MAPGetViewForAddress( const void * );
static PAL_ERROR MAPDesiredAccessAllowed( uint32_t, uint32_t, uint32_t );

static int32_t MAPProtectionToFileOpenFlags( uint32_t );
static BOOL MAPIsRequestPermissible( uint32_t, CFileProcessLocalData * );
static BOOL MAPContainsInvalidFlags( uint32_t );
static uint32_t MAPConvertProtectToAccess( uint32_t );
static int32_t MAPFileMapToMmapFlags( uint32_t );

#if !defined(__linux__)
/* We need MAP_ANON. However on some platforms like HP-UX, it is defined as MAP_ANONYMOUS */
#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#define MAP_ANON MAP_ANONYMOUS
#endif
#endif

void
FileMappingCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    );

PAL_ERROR
FileMappingInitializationRoutine(
    CPalThread *pThread,
    CObjectType *pObjectType,
    void *pImmutableData,
    void *pSharedData,
    void *pProcessLocalData
    );

CObjectType CorUnix::otFileMapping __attribute__((init_priority(200)))(
                otiFileMapping,
                FileMappingCleanupRoutine,
                FileMappingInitializationRoutine,
                sizeof(CFileMappingImmutableData),
                sizeof(CFileMappingProcessLocalData),
                0,
                PAGE_READWRITE | PAGE_READONLY | PAGE_WRITECOPY,
                CObjectType::SecuritySupported,
                CObjectType::SecurityInfoNotPersisted,
                CObjectType::ObjectCanHaveName,
                CObjectType::LocalDuplicationOnly,
                CObjectType::UnwaitableObject,
                CObjectType::SignalingNotApplicable,
                CObjectType::ThreadReleaseNotApplicable,
                CObjectType::OwnershipNotApplicable
                );

CAllowedObjectTypes aotFileMapping __attribute__((init_priority(200))) (otiFileMapping);

void
FileMappingCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    )
{
    PAL_ERROR palError = NO_ERROR;
    CFileMappingImmutableData *pImmutableData = NULL;
    CFileMappingProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;
    bool fDataChanged = FALSE;

    if (TRUE == fCleanupSharedState)
    {
        //
        // If we created a temporary file to back this mapping we need
        // to unlink it now
        //
        
        palError = pObjectToCleanup->GetImmutableData(
            reinterpret_cast<void**>(&pImmutableData)
            );

        if (NO_ERROR != palError)
        {
            ASSERT("Unable to obtain immutable data for object to be reclaimed");
            return;
        }

        if (pImmutableData->bPALCreatedTempFile)
        {
            unlink(pImmutableData->szFileName);
        }
    }

    if (FALSE == fShutdown)
    {
        //
        // We only need to close the object's descriptor if we're not
        // shutting down
        //
        
        palError = pObjectToCleanup->GetProcessLocalData(
            pThread,
            WriteLock,
            &pLocalDataLock,
            reinterpret_cast<void**>(&pLocalData)
            );

        if (NO_ERROR != palError)
        {
            ASSERT("Unable to obtain process local data for object to be reclaimed");
            return;
        }

        if (-1 != pLocalData->UnixFd)
        {
            close(pLocalData->UnixFd);
            pLocalData->UnixFd = -1;
            fDataChanged = TRUE;
        }

        pLocalDataLock->ReleaseLock(pThread, fDataChanged);
    }

    //
    // Why don't we need to deal with any views that may have been created
    // from this mapping? If the process is shutting down then there's nothing
    // that we need to take care of, as the OS will remove the underlying
    // mappings when the process goes away. If we're not shutting down then
    // there's no way for a view to exist against this mapping, since each
    // view holds a reference against the mapping object.
    //
}

PAL_ERROR
FileMappingInitializationRoutine(
    CPalThread *pThread,
    CObjectType *pObjectType,
    void *pvImmutableData,
    void *pvSharedData,
    void *pvProcessLocalData
    )
{
    PAL_ERROR palError = NO_ERROR;
    
    CFileMappingImmutableData *pImmutableData =
        reinterpret_cast<CFileMappingImmutableData *>(pvImmutableData);
    CFileMappingProcessLocalData *pProcessLocalData =
        reinterpret_cast<CFileMappingProcessLocalData *>(pvProcessLocalData);

    pProcessLocalData->UnixFd = InternalOpen(
        pImmutableData->szFileName,
        MAPProtectionToFileOpenFlags(pImmutableData->flProtect)
        );

    if (-1 == pProcessLocalData->UnixFd)
    {
        palError = ERROR_INTERNAL_ERROR;
        goto ExitFileMappingInitializationRoutine;
    }

ExitFileMappingInitializationRoutine:

    return palError;
}

/*++
Function:
  CreateFileMappingW

Note:
  File mapping are used to do inter-process communication.

See MSDN doc.
--*/
HANDLE
CreateFileMappingW(
                HANDLE hFile,
                LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
                uint32_t flProtect,
                uint32_t dwMaximumSizeHigh,
                uint32_t dwMaximumSizeLow,
                const char16_t* lpName)
{
    HANDLE hFileMapping = NULL;
    CPalThread *pThread = NULL;
    PAL_ERROR palError = NO_ERROR;
    
    ENTRY("CreateFileMappingW(hFile=%p, lpAttributes=%p, flProtect=%#x, "
          "dwMaxSizeH=%u, dwMaxSizeL=%u, lpName=%p (%S))\n",
          hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, 
          dwMaximumSizeLow, lpName?lpName:W16_NULLSTRING, lpName?lpName:W16_NULLSTRING);

    pThread = InternalGetCurrentThread();

    palError = InternalCreateFileMapping(
        pThread,
        hFile,
        lpFileMappingAttributes,
        flProtect,
        dwMaximumSizeHigh,
        dwMaximumSizeLow,
        lpName,
        &hFileMapping
        );

    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

    pThread->SetLastError(palError);

    LOGEXIT( "CreateFileMappingW returning %p .\n", hFileMapping );
    return hFileMapping;
}

PAL_ERROR
CorUnix::InternalCreateFileMapping(
    CPalThread *pThread,
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    uint32_t flProtect,
    uint32_t dwMaximumSizeHigh,
    uint32_t dwMaximumSizeLow,
    const char16_t* lpName,
    HANDLE *phMapping
    )
{
    CObjectAttributes objectAttributes(lpName, lpFileMappingAttributes);
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pMapping = NULL;
    IPalObject *pRegisteredMapping = NULL;
    CFileMappingProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;
    CFileMappingImmutableData *pImmutableData = NULL;
    IPalObject *pFileObject = NULL;
    CFileProcessLocalData *pFileLocalData = NULL;
    IDataLock *pFileLocalDataLock = NULL;
    
    struct stat UnixFileInformation;
    int32_t UnixFd = -1;
    BOOL bPALCreatedTempFile = FALSE;
    uint32_t nFileSize = 0;

    //
    // Validate parameters
    //

    if (lpName != nullptr)
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
        goto ExitInternalCreateFileMapping;
    }

    if (0 != dwMaximumSizeHigh)
    {
        ASSERT("dwMaximumSizeHigh is always 0.\n");
        palError = ERROR_INVALID_PARAMETER;
        goto ExitInternalCreateFileMapping;
    }
    
    if (PAGE_READWRITE != flProtect
        && PAGE_READONLY != flProtect
        && PAGE_WRITECOPY != flProtect)
    {
        ASSERT( "invalid flProtect %#x, acceptable values are PAGE_READONLY "
                "(%#x), PAGE_READWRITE (%#x) and PAGE_WRITECOPY (%#x).\n", 
                flProtect, PAGE_READONLY, PAGE_READWRITE, PAGE_WRITECOPY );
        palError = ERROR_INVALID_PARAMETER;
        goto ExitInternalCreateFileMapping;
    }

    if (hFile == INVALID_HANDLE_VALUE && 0 == dwMaximumSizeLow)
    {
        ERROR( "If hFile is INVALID_HANDLE_VALUE, then you must specify a size.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto ExitInternalCreateFileMapping;
    }

    if (hFile != INVALID_HANDLE_VALUE && NULL != lpName)
    {
        ASSERT( "If hFile is not -1, then lpName must be NULL.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto ExitInternalCreateFileMapping;
    }

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otFileMapping,
        &objectAttributes,
        &pMapping
        );

    if (NO_ERROR != palError)
    {
        goto ExitInternalCreateFileMapping;
    }

    palError = pMapping->GetImmutableData(reinterpret_cast<void**>(&pImmutableData));
    if (NO_ERROR != palError)
    {
        goto ExitInternalCreateFileMapping;
    }

    if (hFile == INVALID_HANDLE_VALUE)
    {
        //
        // Note: this path is what prevents us supporting the
        // duplication of file mapping objects across processes, since
        // there is no backing file that the other process can open. We can
        // avoid this restriction by always using a temp backing file for
        // anonymous mappings.
        //
        
        /* Anonymous mapped files. */
        if (strcpy_s(pImmutableData->szFileName, sizeof(pImmutableData->szFileName), "/dev/zero") != SAFECRT_SUCCESS)
        {
            ERROR( "strcpy_s failed!\n" );
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }

#if defined(__linux__)

        UnixFd = InternalOpen(pImmutableData->szFileName, O_RDWR);
        if ( -1 == UnixFd )
        {
            ERROR( "Unable to open the file.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }

#else //!defined(__linux__)

        UnixFd = -1;  /* will pass MAP_ANON to mmap() instead */

#endif //!defined(__linux__)

    }
    else
    {
        if ( hFile != INVALID_HANDLE_VALUE )
        {
            palError = g_pObjectManager->ReferenceObjectByHandle(
                pThread,
                hFile,
                &aotFile,
                GENERIC_READ,
                &pFileObject
                );

            if (NO_ERROR != palError)
            {
                ERROR("Unable to obtain file data.\n");
                palError = ERROR_INVALID_PARAMETER;
                goto ExitInternalCreateFileMapping;
            }

            palError = pFileObject->GetProcessLocalData(
                pThread,
                ReadLock, 
                &pFileLocalDataLock,
                reinterpret_cast<void**>(&pFileLocalData)
                );

            if (NO_ERROR != palError)
            {
                goto ExitInternalCreateFileMapping;
            }
        
            /* We need to check to ensure flProtect jives with 
               the permission on the file handle */
            if (!MAPIsRequestPermissible(flProtect, pFileLocalData))
            {
                ERROR("File handle does not have the correct "
                      "permissions to create mapping\n" );
                palError = ERROR_ACCESS_DENIED;
                if (NULL != pFileLocalDataLock)
                {
                    pFileLocalDataLock->ReleaseLock(pThread, FALSE);
                }
                goto ExitInternalCreateFileMapping;
            }

            //
            // TODO: technically, the file mapping object should hold
            // a reference to the passed in file object. This implementation
            // only keeps the underlying native file structure (i.e., what
            // the duplicated descriptors point to) open. There may be a risk
            // here pertaining to the file lock information that the PAL must
            // maintain (e.g,. if the passed in handle is closed immediately
            // after the file mapping is opened then the lock information will
            // be released, since we're not doing anything to keep it alive
            // here).
            //
            // Having a direct reference to the underlying file object adds
            // some complication, especially in cross-process cases. We may
            // want to consider adding a reference to the PAL's file lock
            // information, though...
            //
            
            UnixFd = dup(pFileLocalData->unix_fd);
            if (-1 == UnixFd)
            {
                ERROR( "Unable to duplicate the Unix file descriptor!\n" );
                palError = ERROR_INTERNAL_ERROR;
                if (NULL != pFileLocalDataLock)
                {
                    pFileLocalDataLock->ReleaseLock(pThread, FALSE);
                }
                goto ExitInternalCreateFileMapping;
            }
  
            if (strcpy_s(pImmutableData->szFileName, sizeof(pImmutableData->szFileName), pFileLocalData->unix_filename) != SAFECRT_SUCCESS)
            {
                ERROR( "strcpy_s failed!\n" );
                palError = ERROR_INTERNAL_ERROR;
                if (NULL != pFileLocalDataLock)
                {
                    pFileLocalDataLock->ReleaseLock(pThread, FALSE);
                }
                goto ExitInternalCreateFileMapping;
            }

            if (NULL != pFileLocalDataLock)
            {
                pFileLocalDataLock->ReleaseLock(pThread, FALSE);
            }
        } 
        else 
        {
            ASSERT("should not get here\n");
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }
    
        if (-1 == fstat(UnixFd, &UnixFileInformation))
        {
            ASSERT("fstat() failed for this reason %s.\n", strerror(errno));
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }

        if ( 0 == UnixFileInformation.st_size && 
             0 == dwMaximumSizeHigh && 0 == dwMaximumSizeLow )
        {
            ERROR( "The file cannot be a zero length file.\n" );
            palError = ERROR_FILE_INVALID;
            goto ExitInternalCreateFileMapping;
        }

        if ( INVALID_HANDLE_VALUE != hFile && 
             dwMaximumSizeLow > (uint32_t) UnixFileInformation.st_size &&
             ( PAGE_READONLY == flProtect || PAGE_WRITECOPY == flProtect ) )
        {
            /* In this situation, Windows returns an error, because the
               permissions requested do not allow growing the file */
            ERROR( "The file cannot be grown do to the map's permissions.\n" );
            palError = ERROR_NOT_ENOUGH_MEMORY;
            goto ExitInternalCreateFileMapping;
        }
      
        if ( (uint32_t) UnixFileInformation.st_size < dwMaximumSizeLow )
        {
            TRACE( "Growing the size of file on disk to match requested size.\n" );

            /* Need to grow the file on disk to match size. */
            palError = MAPGrowLocalFile(UnixFd, dwMaximumSizeLow);
            if (NO_ERROR != palError)
            {
                ERROR( "Unable to grow the file on disk.\n" );
                goto ExitInternalCreateFileMapping;
            }
        }
    }

    nFileSize = ( 0 == dwMaximumSizeLow && 0 == dwMaximumSizeHigh ) ? 
        UnixFileInformation.st_size : dwMaximumSizeLow;

    pImmutableData->MaxSize = nFileSize;
    pImmutableData->flProtect = flProtect;
    pImmutableData->bPALCreatedTempFile = bPALCreatedTempFile;
    pImmutableData->dwDesiredAccessWhenOpened = MAPConvertProtectToAccess(flProtect);
    

    //
    // The local data isn't grabbed / modified until here so that we don't
    // need to worry ourselves with locking issues with the passed in
    // file handle -- all operations concerning the file handle are completed
    // before we deal with the lock for the new object.
    //

    palError = pMapping->GetProcessLocalData(
        pThread,
        WriteLock,
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto ExitInternalCreateFileMapping;
    }

    pLocalData->UnixFd = UnixFd;

    pLocalDataLock->ReleaseLock(pThread, TRUE);
    pLocalDataLock = NULL;

    palError = g_pObjectManager->RegisterObject(
        pThread,
        pMapping,
        &aotFileMapping, 
        flProtect,          // TODO: is flProtect really an access right?
        phMapping,
        &pRegisteredMapping
        );

    //
    // pMapping is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line. This also ensures that we won't attempt to release
    // any data associated with the mapping object here, as if any cleanup is
    // necessary due to a failure in RegisterObject (which includes another
    // object by the same name already existing) the cleanup will take place
    // when that routine releases the reference to pMapping.
    //
    
    pMapping = NULL;

ExitInternalCreateFileMapping:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(
            pThread,
            TRUE
            );
    }

    if (NULL != pMapping)
    {
        pMapping->ReleaseReference(pThread);

        if (bPALCreatedTempFile)
        {
            unlink(pImmutableData->szFileName);
        }

        if (-1 != UnixFd)
        {
            close(UnixFd);
        }
    }

    if (NULL != pRegisteredMapping)
    {
        pRegisteredMapping->ReleaseReference(pThread);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    return palError;
}


/*++
Function:
  OpenFileMappingW

See MSDN doc.
--*/
HANDLE
OpenFileMappingW(
          uint32_t dwDesiredAccess,
          BOOL bInheritHandle,
          const char16_t* lpName)
{
    HANDLE hFileMapping = NULL;
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    
    ENTRY("OpenFileMappingW(dwDesiredAccess=%#x, bInheritHandle=%d, lpName=%p (%S)\n",
          dwDesiredAccess, bInheritHandle, lpName?lpName:W16_NULLSTRING, lpName?lpName:W16_NULLSTRING);

    pThread = InternalGetCurrentThread();

    /* validate parameters */
    if (lpName == nullptr)
    {
        ERROR("name is NULL\n");
        palError = ERROR_INVALID_PARAMETER;
    }
    else
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
    }

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }
    LOGEXIT("OpenFileMappingW returning %p.\n", hFileMapping);
    return hFileMapping;
}

PAL_ERROR
CorUnix::InternalOpenFileMapping(
    CPalThread *pThread,
    uint32_t dwDesiredAccess,
    BOOL bInheritHandle,
    const char16_t* lpName,
    HANDLE *phMapping
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pFileMapping = NULL;
    CPalString sObjectName(lpName);

    if ( MAPContainsInvalidFlags( dwDesiredAccess ) ) 
    {
        ASSERT( "dwDesiredAccess can be one or more of FILE_MAP_READ, " 
               "FILE_MAP_WRITE, FILE_MAP_COPY or FILE_MAP_ALL_ACCESS.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto ExitInternalOpenFileMapping;
    }

    palError = g_pObjectManager->LocateObject(
        pThread,
        &sObjectName,
        &aotFileMapping, 
        &pFileMapping
        );

    if (NO_ERROR != palError)
    {
        goto ExitInternalOpenFileMapping;
    }

    palError = g_pObjectManager->ObtainHandleForObject(
        pThread,
        pFileMapping,
        dwDesiredAccess,
        bInheritHandle,
        NULL,
        phMapping
        );

    if (NO_ERROR != palError)
    {
        goto ExitInternalOpenFileMapping;
    }

ExitInternalOpenFileMapping:

    if (NULL != pFileMapping)
    {
        pFileMapping->ReleaseReference(pThread);
    }
    
    return palError;
}

/*++
Function:
  MapViewOfFile

  Limitations: 1) Currently file mappings are supported only at file
                  offset 0.
               2) Some platforms (specifically HP-UX) do not support
                  multiple simultaneous shared mapping of the same file
                  region in the same process. On these platforms, in case
                  we are asked for a new view completely contained in an
                  existing one, we return an address within the existing
                  mapping. In case the new requested view is overlapping
                  with the existing one, but not contained in it, the
                  mapping is impossible, and MapViewOfFile will fail.
                  Since currently the mappings are supported only at file
                  offset 0, MapViewOfFile will succeed if the new view
                  is equal or smaller of the existing one, and the address
                  returned will be the same address of the existing 
                  mapping.
                  Since the underlying mapping is always the same, all 
                  the shared views of the same file region will share the
                  same protection, i.e. they will have the largest 
                  protection requested. If any mapping asked for a
                  read-write access, all the read-only mappings of the 
                  same region will silently get a read-write access to 
                  it.

See MSDN doc.
--*/
void *
MapViewOfFile(
           HANDLE hFileMappingObject,
           uint32_t dwDesiredAccess,
           uint32_t dwFileOffsetHigh,
           uint32_t dwFileOffsetLow,
           size_t dwNumberOfBytesToMap)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    void * pvMappedBaseAddress = NULL;

    ENTRY("MapViewOfFile(hFileMapping=%p, dwDesiredAccess=%u, "
          "dwFileOffsetH=%u, dwFileOffsetL=%u, dwNumberOfBytes=%u)\n",
          hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh,
          dwFileOffsetLow, dwNumberOfBytesToMap);

    pThread = InternalGetCurrentThread();

    palError = InternalMapViewOfFile(
        pThread,
        hFileMappingObject,
        dwDesiredAccess,
        dwFileOffsetHigh,
        dwFileOffsetLow,
        dwNumberOfBytesToMap,
        &pvMappedBaseAddress
        );

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }

    LOGEXIT( "MapViewOfFile returning %p.\n", pvMappedBaseAddress );
    return pvMappedBaseAddress;
}

/*++
Function:
  UnmapViewOfFile

See MSDN doc.
--*/
BOOL
UnmapViewOfFile(
         const void * lpBaseAddress)
{
    PAL_ERROR palError;
    CPalThread *pThread;
    
    ENTRY("UnmapViewOfFile(lpBaseAddress=%p)\n", lpBaseAddress);

    pThread = InternalGetCurrentThread();

    palError = InternalUnmapViewOfFile(pThread, lpBaseAddress);

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }
    
    LOGEXIT( "UnmapViewOfFile returning %s.\n", (NO_ERROR == palError) ? "TRUE" : "FALSE" );
    return (NO_ERROR == palError);
}

PAL_ERROR
CorUnix::InternalMapViewOfFile(
    CPalThread *pThread,
    HANDLE hFileMappingObject,
    uint32_t dwDesiredAccess,
    uint32_t dwFileOffsetHigh,
    uint32_t dwFileOffsetLow,
    size_t dwNumberOfBytesToMap,
    void * *ppvBaseAddress
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pMappingObject = NULL;
    CFileMappingImmutableData *pImmutableData = NULL;
    CFileMappingProcessLocalData *pProcessLocalData = NULL;
    IDataLock *pProcessLocalDataLock = NULL;
    void * pvBaseAddress = NULL;

    /* Sanity checks */
    if ( MAPContainsInvalidFlags( dwDesiredAccess ) )
    {
        ASSERT( "dwDesiredAccess can be one of FILE_MAP_WRITE, FILE_MAP_READ,"
               " FILE_MAP_COPY or FILE_MAP_ALL_ACCESS.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto InternalMapViewOfFileExit;
    }

    if ( 0 != dwFileOffsetHigh || 0 != dwFileOffsetLow )
    {
        ASSERT( "dwFileOffsetHigh and dwFileOffsetLow are always 0.\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto InternalMapViewOfFileExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFileMappingObject,
        &aotFileMapping,
        dwDesiredAccess,
        &pMappingObject
        );

    if (NO_ERROR != palError)
    {
        ERROR( "Unable to reference handle %p.\n",hFileMappingObject );
        goto InternalMapViewOfFileExit;
    }

    palError = pMappingObject->GetImmutableData(
        reinterpret_cast<void**>(&pImmutableData)
        );

    if (NO_ERROR != palError)
    {
        ERROR( "Unable to obtain object immutable data");
        goto InternalMapViewOfFileExit;
    }

    palError = pMappingObject->GetProcessLocalData(
        pThread,
        ReadLock,
        &pProcessLocalDataLock,
        reinterpret_cast<void**>(&pProcessLocalData)
        );

    if (NO_ERROR != palError)
    {
        ERROR( "Unable to obtain object process local data");
        goto InternalMapViewOfFileExit;
    }
    
    /* If dwNumberOfBytesToMap is 0, we need to map the entire file.
     * mmap doesn't do the same thing as Windows in that case, though,
     * so we use the file size instead. */
    if (0 == dwNumberOfBytesToMap)
    {
        dwNumberOfBytesToMap = pImmutableData->MaxSize;
    }

    palError = MAPDesiredAccessAllowed(
        pImmutableData->flProtect,
        dwDesiredAccess,
        pImmutableData->dwDesiredAccessWhenOpened
        );

    if (NO_ERROR != palError)
    {
        goto InternalMapViewOfFileExit;
    }

    InternalEnterCriticalSection(pThread, &mapping_critsec);

    if (FILE_MAP_COPY == dwDesiredAccess)
    {
        int flags = MAP_PRIVATE;

#if !defined(__linux__)
        if (pProcessLocalData->UnixFd == -1)
        {
            flags |= MAP_ANON;
        }
#endif
        pvBaseAddress = mmap(
            NULL,
            dwNumberOfBytesToMap,
            PROT_READ|PROT_WRITE,
            flags,
            pProcessLocalData->UnixFd,
            0
            );
    }
    else
    {
        int32_t prot = MAPFileMapToMmapFlags(dwDesiredAccess);
        if (prot != -1)
        {
            int flags = MAP_SHARED;

#if !defined(__linux__)
            if (pProcessLocalData->UnixFd == -1)
            {
                flags |= MAP_ANON;
            }
#endif

            pvBaseAddress = mmap(
                NULL,
                dwNumberOfBytesToMap,
                prot,
                flags,
                pProcessLocalData->UnixFd,
                0
                );

        }
        else
        {
            ASSERT( "MapFileMapToMmapFlags failed!\n" );
            palError = ERROR_INTERNAL_ERROR;
            goto InternalMapViewOfFileLeaveCriticalSection;
        }
    }

    if (MAP_FAILED == pvBaseAddress
        )
    {
        ERROR( "mmap failed with code %s.\n", strerror( errno ) );
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto InternalMapViewOfFileLeaveCriticalSection;

    }

    {
        //
        // Allocate and fill out a new view structure, and add it to
        // the global list.
        //
        
        PMAPPED_VIEW_LIST pNewView = (PMAPPED_VIEW_LIST)malloc(sizeof(*pNewView));
        if (NULL != pNewView)
        {
            pNewView->lpAddress = pvBaseAddress;
            pNewView->NumberOfBytesToMap = dwNumberOfBytesToMap;
            pNewView->dwDesiredAccess = dwDesiredAccess;
            pNewView->pFileMapping = pMappingObject;
            pNewView->pFileMapping->AddReference();
            pNewView->lpPEBaseAddress = 0;
            InsertTailList(&MappedViewList, &pNewView->Link);

        }
        else
        {
            palError = ERROR_INTERNAL_ERROR;
        }

        if (NO_ERROR != palError)
        {
            if (-1 == munmap(pvBaseAddress, dwNumberOfBytesToMap))
            {
                ERROR("Unable to unmap the file. Expect trouble.\n");
                goto InternalMapViewOfFileLeaveCriticalSection;
            }
        }
    }
    
    TRACE( "Added %p to the list.\n", pvBaseAddress );
    *ppvBaseAddress = pvBaseAddress;

InternalMapViewOfFileLeaveCriticalSection:

    InternalLeaveCriticalSection(pThread, &mapping_critsec);

InternalMapViewOfFileExit:

    if (NULL != pProcessLocalDataLock)
    {
        pProcessLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pMappingObject)
    {
        pMappingObject->ReleaseReference(pThread);
    }

    return palError;
}


PAL_ERROR
CorUnix::InternalUnmapViewOfFile(
    CPalThread *pThread,
    const void * lpBaseAddress
    )
{
    PAL_ERROR palError = NO_ERROR;
    PMAPPED_VIEW_LIST pView = NULL;
    IPalObject *pMappingObject = NULL;

    InternalEnterCriticalSection(pThread, &mapping_critsec);

    pView = MAPGetViewForAddress(lpBaseAddress);
    if (NULL == pView)
    {
        ERROR("lpBaseAddress has to be the address returned by MapViewOfFile[Ex]");
        palError = ERROR_INVALID_HANDLE;
        goto InternalUnmapViewOfFileExit;
    }
    
    if (-1 == munmap((void *)lpBaseAddress, pView->NumberOfBytesToMap))
    {
        ASSERT( "Unable to unmap the memory. Error=%s.\n",
                strerror( errno ) );
        palError = ERROR_INTERNAL_ERROR;

        //
        // Even if the unmap fails we want to continue removing the
        // info for this view
        //
    }

    RemoveEntryList(&pView->Link);
    pMappingObject = pView->pFileMapping;
    free(pView);
    
InternalUnmapViewOfFileExit:

    InternalLeaveCriticalSection(pThread, &mapping_critsec);

    //
    // We can't dereference the file mapping object until after
    // we've released the mapping critical section, since it may
    // start going down its cleanup path and we don't want to make
    // any assumptions as to what locks that might grab...
    //

    if (NULL != pMappingObject)
    {
        pMappingObject->ReleaseReference(pThread);
    }

    return palError;
}

/*++
Function :
    MAPInitialize

    Initialize the critical sections.

Return value:
    TRUE if initialization succeeded
    FALSE otherwise
--*/
BOOL
MAPInitialize( void )
{
    TRACE( "Initialising the critical section.\n" );

    InternalInitializeCriticalSection(&mapping_critsec);

    InitializeListHead(&MappedViewList);

    return TRUE;
}

/*++
Function :
    MAPCleanup

    Deletes the critical sections. And all other necessary cleanup.

Note:
    This function is called after the handle manager is stopped. So
    there shouldn't be any call that will cause an access to the handle
    manager.

--*/
void MAPCleanup( void )
{
    TRACE( "Deleting the critical section.\n" );
    InternalDeleteCriticalSection(&mapping_critsec);
}

/*++
Function :
    MAPGetViewForAddress

    Returns the mapped view (if any) that is based at the passed in address.

    Callers to this function must hold mapping_critsec
--*/
static PMAPPED_VIEW_LIST MAPGetViewForAddress( const void * lpAddress )
{       
    if ( NULL == lpAddress )
    {
        ERROR( "lpAddress cannot be NULL\n" );
        return NULL;
    }

    for(LIST_ENTRY *pLink = MappedViewList.Flink;
        pLink != &MappedViewList;
        pLink = pLink->Flink)
    {
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        if (pView->lpAddress == lpAddress)
        {
            return pView;
        }
    }
            
    WARN( "No match found.\n" );
    
    return NULL;
}

/*++
Function :

    MAPDesiredAccessAllowed

    Determines if desired access is allowed based on the protection state.

    if dwDesiredAccess conflicts with flProtect then the error is
    ERROR_INVALID_PARAMETER, if the dwDesiredAccess conflicts with
    dwDesiredAccessWhenOpened, then the error code is ERROR_ACCESS_DENIED
--*/
static PAL_ERROR MAPDesiredAccessAllowed( uint32_t flProtect,
                                     uint32_t dwUserDesiredAccess,
                                     uint32_t dwDesiredAccessWhenOpened )
{
    TRACE( "flProtect=%d, dwUserDesiredAccess=%d, dwDesiredAccessWhenOpened=%d\n",
           flProtect, dwUserDesiredAccess, dwDesiredAccessWhenOpened );

    /* check flProtect parameters*/
    if ( FILE_MAP_READ!= dwUserDesiredAccess && PAGE_READONLY == flProtect )
    {
        ERROR( "map object is read-only, can't map a view with write access\n");
        return ERROR_INVALID_PARAMETER;
    }

    if ( FILE_MAP_WRITE == dwUserDesiredAccess && PAGE_READWRITE != flProtect )
    {
        ERROR( "map object not open read-write, can't map a view with write "
               "access.\n" );
        return ERROR_INVALID_PARAMETER;
    }

    if ( FILE_MAP_COPY == dwUserDesiredAccess  && PAGE_WRITECOPY != flProtect )
    {
        ERROR( "map object not open for copy-on-write, can't map copy-on-write "
               "view.\n" );
        return ERROR_INVALID_PARAMETER;
    }
    
    /* Check to see we don't confict with the desired access we
    opened the mapping object with. */
    if ( ( dwUserDesiredAccess == FILE_MAP_READ ) &&
        !( ( dwDesiredAccessWhenOpened == FILE_MAP_READ ) || 
           ( dwDesiredAccessWhenOpened == FILE_MAP_ALL_ACCESS ) ) ) 
    {
        ERROR( "dwDesiredAccess conflict : read access requested, object not "
               "opened with read access.\n" );
        return ERROR_ACCESS_DENIED;
    }
    if ( ( dwUserDesiredAccess & FILE_MAP_WRITE ) &&
        !( ( dwDesiredAccessWhenOpened == FILE_MAP_WRITE ) || 
           ( dwDesiredAccessWhenOpened == FILE_MAP_ALL_ACCESS ) ) ) 
    {
        ERROR( "dwDesiredAccess conflict : write access requested, object not "
               "opened with write access.\n" );
        return ERROR_ACCESS_DENIED;
    }
    if ( ( dwUserDesiredAccess == FILE_MAP_COPY ) &&
        !( dwDesiredAccessWhenOpened == FILE_MAP_COPY ) )
    {
        ERROR( "dwDesiredAccess conflict : copy-on-write access requested, "
               "object not opened with copy-on-write access.\n" );
        return ERROR_ACCESS_DENIED;
    }

    return NO_ERROR;
}

/*++
Function :
    MAPConvertProtectToAccess

    Converts the PAGE_READONLY type flags to FILE_MAP_READ flags.

--*/
static uint32_t MAPConvertProtectToAccess( uint32_t flProtect )
{
    if ( PAGE_READONLY == flProtect )
    {
        return FILE_MAP_READ;
    }
    if ( PAGE_READWRITE == flProtect )
    {
        return FILE_MAP_ALL_ACCESS;
    }
    if ( PAGE_WRITECOPY == flProtect )
    {
        return FILE_MAP_COPY;
    }

    ASSERT( "Unknown flag for flProtect. This line "
            "should not have been executed.\n " );
    return (uint32_t) -1;
}

/*++
Function :
    MAPConvertAccessToProtect

    Converts the FILE_MAP_READ type flags to PAGE_READONLY flags.
    Currently, this function only deals with the access flags recognized as valid 
    by MAPContainsInvalidFlags().

--*/
static uint32_t MAPConvertAccessToProtect(uint32_t flAccess)
{
    if (flAccess == FILE_MAP_ALL_ACCESS)
    {
        return PAGE_READWRITE;
    }
    else if ((flAccess == FILE_MAP_COPY) || (flAccess == FILE_MAP_WRITE))
    {
        return PAGE_WRITECOPY;
    }
    else if (flAccess == FILE_MAP_READ)
    {
        return PAGE_READONLY;
    }
    else if (flAccess == 0)
    {
        return PAGE_NOACCESS;
    }

    ASSERT("Unknown flag for flAccess.\n");
    return (uint32_t) -1;
}

/*++
Function :
    MAPFileMapToMmapFlags

    Converts the mapping flags to unix protection flags.
--*/
static int32_t MAPFileMapToMmapFlags( uint32_t flags )
{
    if ( FILE_MAP_READ == flags )
    {
        TRACE( "FILE_MAP_READ\n" );
        return PROT_READ;
    }
    else if ( FILE_MAP_WRITE == flags )
    {
        TRACE( "FILE_MAP_WRITE\n" );
        /* The limitation of x86 architecture
        means you cant have writable but not readable
        page. In Windows maps of FILE_MAP_WRITE can still be
        read from. */
        return PROT_WRITE | PROT_READ;
    }
    else if ( (FILE_MAP_READ|FILE_MAP_WRITE) == flags )
    {
        TRACE( "FILE_MAP_READ|FILE_MAP_WRITE\n" );
        return PROT_READ | PROT_WRITE;
    }
    else if( FILE_MAP_COPY == flags)
    {
        TRACE( "FILE_MAP_COPY\n");
        return PROT_READ | PROT_WRITE;
    } 

    ASSERT( "Unknown flag. This line should not have been executed.\n" );
    return -1;
}

/*++
Function :

    MAPGrowLocalFile

    Grows the file on disk to match the specified size.
    
--*/
static PAL_ERROR MAPGrowLocalFile( int32_t UnixFD, uint32_t NewSize )
{
    PAL_ERROR palError = NO_ERROR;
    int32_t  TruncateRetVal = -1;
    struct stat FileInfo;
    TRACE( "Entered MapGrowLocalFile (UnixFD=%d,NewSize%d)\n", UnixFD, NewSize );

    //
    // TODO: can we add configure flags to model the behavior of ftruncate
    // among our various target platforms? How much would that actually gain
    // us?
    //

    /* ftruncate is a standard function, but the behavior of enlarging files is
    non-standard.  So I will try to enlarge a file, and if that fails try the
    less efficent way.*/
    TruncateRetVal = ftruncate( UnixFD, NewSize );
    fstat( UnixFD, &FileInfo );

    if ( TruncateRetVal != 0 || FileInfo.st_size != (int) NewSize )
    {
        int32_t OrigSize;
        const uint32_t  BUFFER_SIZE = 128;
        uint8_t buf[BUFFER_SIZE];
        uint32_t x = 0;
        uint32_t CurrentPosition = 0;

        TRACE( "Trying the less efficent way.\n" );

        CurrentPosition = lseek( UnixFD, 0, SEEK_CUR );
        OrigSize = lseek( UnixFD, 0, SEEK_END );
        if ( OrigSize == -1 )
        {
            ERROR( "Unable to locate the EOF marker. Reason=%s\n",
                   strerror( errno ) );
            palError = ERROR_INTERNAL_ERROR;
            goto done;
        }

        if (NewSize <= (uint32_t) OrigSize)
        {
            return TRUE;
        }

        memset( buf, 0, BUFFER_SIZE );

        for ( x = 0; x < NewSize - OrigSize - BUFFER_SIZE; x += BUFFER_SIZE )
        {
            if ( write( UnixFD, (void *)buf, BUFFER_SIZE ) == -1 )
            {
                ERROR( "Unable to grow the file. Reason=%s\n", strerror( errno ) );
                if((errno == ENOSPC) || (errno == EDQUOT))
                {
                    palError = ERROR_DISK_FULL;
                }
                else
                {
                    palError = ERROR_INTERNAL_ERROR;
                }
                goto done;
            }
        }
        /* Catch any left overs. */
        if ( x != NewSize )
        {
            if ( write( UnixFD, (void *)buf, NewSize - OrigSize - x) == -1 )
            {
                ERROR( "Unable to grow the file. Reason=%s\n", strerror( errno ) );
                if((errno == ENOSPC) || (errno == EDQUOT))
                {
                    palError = ERROR_DISK_FULL;
                }
                else
                {
                    palError = ERROR_INTERNAL_ERROR;
                }
                goto done;
            }
        }

        /* restore the file pointer position */
        lseek( UnixFD, CurrentPosition, SEEK_SET );
    }

done:
    return palError;
}

/*++
Function :
    MAPContainsInvalidFlags

    Checks that only valid flags are in the parameter.
    
--*/
static BOOL MAPContainsInvalidFlags( uint32_t flags )
{

    if ( (flags == FILE_MAP_READ) ||
         (flags == FILE_MAP_WRITE) ||
         (flags == FILE_MAP_ALL_ACCESS) ||
         (flags == FILE_MAP_COPY) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*++
Function : 
    MAPProtectionToFileOpenFlags
    
    Converts the PAGE_* flags to the O_* flags.
 
    Returns the file open flags.
--*/
static int32_t MAPProtectionToFileOpenFlags( uint32_t flProtect )
{
    int32_t retVal = 0;
    switch(flProtect)
    {
    case PAGE_READONLY:
        retVal = O_RDONLY;
        break;
    case PAGE_READWRITE:
        retVal = O_RDWR;
        break;
    case PAGE_WRITECOPY:
        retVal = O_RDONLY;
        break;
    default:
        ASSERT("unexpected flProtect value %#x\n", flProtect);
        retVal = 0;
        break;
    }         
    return retVal;
}

/*++
Function :
    
    MAPIsRequestPermissible
    
        uint32_t flProtect     - The requested file mapping protection .
        file * pFileStruct  - The file structure containing all the information.

--*/
static BOOL MAPIsRequestPermissible( uint32_t flProtect, CFileProcessLocalData * pFileLocalData )
{
    if ( ( (flProtect == PAGE_READONLY || flProtect == PAGE_WRITECOPY) && 
           (pFileLocalData->open_flags_deviceaccessonly == TRUE || 
            pFileLocalData->open_flags & O_WRONLY) )
       )
    {
        /*
         * PAGE_READONLY or PAGE_WRITECOPY access to a file must at least be
         * readable. Contrary to what MSDN says, PAGE_WRITECOPY
         * only needs to be readable.
         */
        return FALSE;
    }
    else if ( flProtect == PAGE_READWRITE && !(pFileLocalData->open_flags & O_RDWR) )
    {
        /*
         * PAGE_READWRITE access to a file needs to be readable and writable 
         */
        return FALSE;
    }
    else
    {
        /* Action is permissible */
        return TRUE;
    }
}

// returns TRUE if we have information about the specified address
BOOL MAPGetRegionInfo(void * lpAddress,
                      PMEMORY_BASIC_INFORMATION lpBuffer)
{
    BOOL fFound = FALSE;
    CPalThread * pThread = InternalGetCurrentThread();
    
    InternalEnterCriticalSection(pThread, &mapping_critsec);

    for(LIST_ENTRY *pLink = MappedViewList.Flink;
        pLink != &MappedViewList;
        pLink = pLink->Flink)
    {
        uint32_t MappedSize;
        void * real_map_addr;
        size_t real_map_sz;
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        real_map_addr = pView->lpAddress;
        real_map_sz = pView->NumberOfBytesToMap;

        MappedSize = ((real_map_sz-1) & ~VIRTUAL_PAGE_MASK) + VIRTUAL_PAGE_SIZE; 
        if ( real_map_addr <= lpAddress && 
             (void *)((unsigned long)real_map_addr+MappedSize) > lpAddress )
        {
            if (lpBuffer)
            {
                size_t regionSize = MappedSize + (unsigned long) real_map_addr - 
                       ((unsigned long) lpAddress & ~VIRTUAL_PAGE_MASK);

                lpBuffer->BaseAddress = lpAddress;
                lpBuffer->AllocationProtect = 0;
                lpBuffer->RegionSize = regionSize;
                lpBuffer->State = MEM_COMMIT;
                lpBuffer->Protect = MAPConvertAccessToProtect(pView->dwDesiredAccess);
                lpBuffer->Type = MEM_MAPPED;
            }

            fFound = TRUE;
            break;
        }
    }

    InternalLeaveCriticalSection(pThread, &mapping_critsec);
    
    return fFound;
}