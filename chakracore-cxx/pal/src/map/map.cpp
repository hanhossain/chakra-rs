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
#include "pal/malloc.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include "rt/ntimage.h"
#include <pal_endian.h>

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

CRITICAL_SECTION mapping_critsec PAL_GLOBAL;
LIST_ENTRY MappedViewList PAL_GLOBAL;

static PAL_ERROR MAPGrowLocalFile(int32_t, uint32_t);
static PMAPPED_VIEW_LIST MAPGetViewForAddress( const void * );
static PAL_ERROR MAPDesiredAccessAllowed( uint32_t, uint32_t, uint32_t );

static int32_t MAPProtectionToFileOpenFlags( uint32_t );
static BOOL MAPIsRequestPermissible( uint32_t, CFileProcessLocalData * );
static BOOL MAPContainsInvalidFlags( uint32_t );
static uint32_t MAPConvertProtectToAccess( uint32_t );
static int32_t MAPFileMapToMmapFlags( uint32_t );
static uint32_t MAPMmapProtToAccessFlags( int prot );
#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
static NativeMapHolder * NewNativeMapHolder(CPalThread *pThread, void * address, SIZE_T size,
                                     SIZE_T offset, long init_ref_count);
static int32_t NativeMapHolderAddRef(NativeMapHolder * thisPMH);
static int32_t NativeMapHolderRelease(CPalThread *pThread, NativeMapHolder * thisPMH);
static PMAPPED_VIEW_LIST FindSharedMappingReplacement(CPalThread *pThread, dev_t deviceNum, ino_t inodeNum,
                                                      SIZE_T size, SIZE_T offset);
#endif

static PAL_ERROR
MAPRecordMapping(
    IPalObject *pMappingObject,
    void *pPEBaseAddress,
    void *addr,
    size_t len,
    int prot
    );

static PAL_ERROR
MAPmmapAndRecord(
    IPalObject *pMappingObject,
    void *pPEBaseAddress,
    void *addr,
    size_t len,
    int prot,
    int flags,
    int fd,
    off_t offset,
    void * *ppvBaseAddress
    );

#if !HAVE_MMAP_DEV_ZERO
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

CObjectType CorUnix::otFileMapping PAL_GLOBAL(
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

CAllowedObjectTypes aotFileMapping PAL_GLOBAL (otiFileMapping);

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

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    struct stat st;

    if (0 == fstat(pProcessLocalData->UnixFd, &st))
    {
        pProcessLocalData->MappedFileDevNum = st.st_dev;
        pProcessLocalData->MappedFileInodeNum = st.st_ino;
    }
    else
    {
        ERROR("Couldn't get inode info for fd=%d to be stored in mapping object\n", pProcessLocalData->UnixFd);
    }
#endif

ExitFileMappingInitializationRoutine:    

    return palError;
}

/*++
Function:
  CreateFileMappingA

Note:
  File mapping are used to do inter-process communication.

See MSDN doc.
--*/
HANDLE
CreateFileMappingA(
                    HANDLE hFile,
                    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
                    uint32_t flProtect,
                    uint32_t dwMaximumSizeHigh,
                    uint32_t dwMaximumSizeLow,
                    LPCSTR lpName)
{
    HANDLE hFileMapping = NULL;
    CPalThread *pThread = NULL;
    PAL_ERROR palError = NO_ERROR;

    PERF_ENTRY(CreateFileMappingA);
    ENTRY("CreateFileMappingA(hFile=%p, lpAttributes=%p, flProtect=%#x, "
          "dwMaxSizeH=%d, dwMaxSizeL=%d, lpName=%p (%s))\n",
          hFile, lpFileMappingAttributes, flProtect, 
          dwMaximumSizeHigh, dwMaximumSizeLow,
          lpName?lpName:"NULL",
          lpName?lpName:"NULL");

    pThread = InternalGetCurrentThread();

    if (lpName != nullptr)
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
    }
    else
    {
        palError = InternalCreateFileMapping(
            pThread,
            hFile,
            lpFileMappingAttributes,
            flProtect,
            dwMaximumSizeHigh,
            dwMaximumSizeLow,
            NULL,
            &hFileMapping
            );
    }


    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

    pThread->SetLastError(palError);

    LOGEXIT( "CreateFileMappingA returns HANDLE %p. \n", hFileMapping );
    PERF_EXIT(CreateFileMappingA);
    return hFileMapping;
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
                LPCWSTR lpName)
{
    HANDLE hFileMapping = NULL;
    CPalThread *pThread = NULL;
    PAL_ERROR palError = NO_ERROR;
    
    PERF_ENTRY(CreateFileMappingW);
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
    PERF_EXIT(CreateFileMappingW);
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
    LPCWSTR lpName,
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

#if HAVE_MMAP_DEV_ZERO

        UnixFd = InternalOpen(pImmutableData->szFileName, O_RDWR);
        if ( -1 == UnixFd )
        {
            ERROR( "Unable to open the file.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }

#else //!HAVE_MMAP_DEV_ZERO

        UnixFd = -1;  /* will pass MAP_ANON to mmap() instead */

#endif //!HAVE_MMAP_DEV_ZERO

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

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    if (-1 == UnixFd)
    {
        pLocalData->MappedFileDevNum = (dev_t)-1; /* there is no standard NO_DEV */
        pLocalData->MappedFileInodeNum = NO_INO;
    }
    else
    {
        struct stat st;

        if (0 == fstat(UnixFd, &st))
        {
            pLocalData->MappedFileDevNum = st.st_dev;
            pLocalData->MappedFileInodeNum = st.st_ino;
        }
        else
        {
            ERROR("Couldn't get inode info for fd=%d to be stored in mapping object\n", UnixFd);
            palError = ERROR_INTERNAL_ERROR;
            goto ExitInternalCreateFileMapping;
        }
    }
#endif

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
  OpenFileMappingA

See MSDN doc.
--*/
HANDLE
OpenFileMappingA(
          uint32_t dwDesiredAccess,
          BOOL bInheritHandle,
          LPCSTR lpName)
{
    HANDLE hFileMapping = NULL;
    CPalThread *pThread = NULL;
    PAL_ERROR palError = NO_ERROR;

    PERF_ENTRY(OpenFileMappingA);
    ENTRY("OpenFileMappingA(dwDesiredAccess=%u, bInheritHandle=%d, lpName=%p (%s)\n",
          dwDesiredAccess, bInheritHandle, lpName?lpName:"NULL", lpName?lpName:"NULL");

    pThread = InternalGetCurrentThread();

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
    LOGEXIT( "OpenFileMappingA returning %p\n", hFileMapping );
    PERF_EXIT(OpenFileMappingA);
    return hFileMapping;
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
          LPCWSTR lpName)
{
    HANDLE hFileMapping = NULL;
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    
    PERF_ENTRY(OpenFileMappingW);
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
    PERF_EXIT(OpenFileMappingW);
    return hFileMapping;
}

PAL_ERROR
CorUnix::InternalOpenFileMapping(
    CPalThread *pThread,
    uint32_t dwDesiredAccess,
    BOOL bInheritHandle,
    LPCWSTR lpName,
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
           SIZE_T dwNumberOfBytesToMap)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    void * pvMappedBaseAddress = NULL;

    PERF_ENTRY(MapViewOfFile);
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
    PERF_EXIT(MapViewOfFile);
    return pvMappedBaseAddress;
}

void *
MapViewOfFileEx(
           HANDLE hFileMappingObject,
           uint32_t dwDesiredAccess,
           uint32_t dwFileOffsetHigh,
           uint32_t dwFileOffsetLow,
           SIZE_T dwNumberOfBytesToMap,
           void * lpBaseAddress)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    void * pvMappedBaseAddress = NULL;

    PERF_ENTRY(MapViewOfFileEx);
    ENTRY("MapViewOfFileEx(hFileMapping=%p, dwDesiredAccess=%u, "
          "dwFileOffsetH=%u, dwFileOffsetL=%u, dwNumberOfBytes=%u, lpBaseAddress=%p)\n",
          hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh,
          dwFileOffsetLow, dwNumberOfBytesToMap, lpBaseAddress);

    pThread = InternalGetCurrentThread();

    if (lpBaseAddress == NULL)
    {
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
    }
    else
    {
        // TODO: Figure out if we can support mapping at a specific address on Linux.
        pThread->SetLastError(ERROR_INVALID_PARAMETER);       
    }

    LOGEXIT( "MapViewOfFileEx returning %p.\n", pvMappedBaseAddress );
    PERF_EXIT(MapViewOfFileEx);
    return pvMappedBaseAddress;
}

/*++
Function:
  FlushViewOfFile

See MSDN doc.
--*/
BOOL
FlushViewOfFile(
     void * lpBaseAddress,
     SIZE_T dwNumberOfBytesToFlush)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pThread = NULL;
    PMAPPED_VIEW_LIST pView = NULL;
    BOOL fResult = TRUE;

    PERF_ENTRY(FlushViewOfFile);
    ENTRY("FlushViewOfFile(lpBaseAddress=%p, dwNumberOfBytesToFlush=%u)\n",
          lpBaseAddress, dwNumberOfBytesToFlush);

    pThread = InternalGetCurrentThread();

    InternalEnterCriticalSection(pThread, &mapping_critsec);

    pView = MAPGetViewForAddress(lpBaseAddress);
    if (NULL == pView)
    {
        ERROR("lpBaseAddress has to be the address returned by MapViewOfFile[Ex]");
        palError = ERROR_INVALID_HANDLE;
        goto Exit;
    }

    if (dwNumberOfBytesToFlush == 0)
    {
        dwNumberOfBytesToFlush = pView->NumberOfBytesToMap;
    }

    // <ROTORTODO>we should only use MS_SYNC if the file has been opened
    // with FILE_FLAG_WRITE_THROUGH
    if (msync(lpBaseAddress, dwNumberOfBytesToFlush, MS_SYNC) == -1)
    {
        if (errno == EINVAL)
        {
            WARN("msync failed; %s\n", strerror(errno));
            palError = ERROR_INVALID_PARAMETER;
        }
        else if (errno == EIO)
        {
            WARN("msync failed; %s\n", strerror(errno));
            palError = ERROR_WRITE_FAULT;
        }
        else
        {
            ERROR("msync failed; %s\n", strerror(errno));
            palError = ERROR_INTERNAL_ERROR;
        }
    }

Exit:
    InternalLeaveCriticalSection(pThread, &mapping_critsec);

    if (NO_ERROR != palError)
    {
        fResult = FALSE;
        pThread->SetLastError(palError);
    }

    LOGEXIT("FlushViewOfFile returning %d.\n", fResult);
    PERF_EXIT(FlushViewOfFile);
    return fResult;
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
    
    PERF_ENTRY(UnmapViewOfFile);
    ENTRY("UnmapViewOfFile(lpBaseAddress=%p)\n", lpBaseAddress);

    pThread = InternalGetCurrentThread();

    palError = InternalUnmapViewOfFile(pThread, lpBaseAddress);

    if (NO_ERROR != palError)
    {
        pThread->SetLastError(palError);
    }
    
    LOGEXIT( "UnmapViewOfFile returning %s.\n", (NO_ERROR == palError) ? "TRUE" : "FALSE" );
    PERF_EXIT(UnmapViewOfFile);
    return (NO_ERROR == palError);
}

PAL_ERROR
CorUnix::InternalMapViewOfFile(
    CPalThread *pThread,
    HANDLE hFileMappingObject,
    uint32_t dwDesiredAccess,
    uint32_t dwFileOffsetHigh,
    uint32_t dwFileOffsetLow,
    SIZE_T dwNumberOfBytesToMap,
    void * *ppvBaseAddress
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pMappingObject = NULL;
    CFileMappingImmutableData *pImmutableData = NULL;
    CFileMappingProcessLocalData *pProcessLocalData = NULL;
    IDataLock *pProcessLocalDataLock = NULL;
#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    PMAPPED_VIEW_LIST pReusedMapping = NULL;
#endif
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

#if !HAVE_MMAP_DEV_ZERO
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

#if !HAVE_MMAP_DEV_ZERO
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

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
            if ((MAP_FAILED == pvBaseAddress) && (ENOMEM == errno))
            {
                /* Search in list of MAPPED_MEMORY_INFO for a shared mapping 
                   with the same inode number
                */
                TRACE("Mmap() failed with errno=ENOMEM probably for multiple mapping "
                      "limitation. Searching for a replacement among existing mappings\n");

                pReusedMapping = FindSharedMappingReplacement(
                    pThread,
                    pProcessLocalData->MappedFileDevNum,
                    pProcessLocalData->MappedFileInodeNum,
                    dwNumberOfBytesToMap,
                    0
                    );
                
                if (pReusedMapping)
                {
                    int ret;

                    TRACE("Mapping @ %p {sz=%d offs=%d} fully "
                          "contains the requested one {sz=%d offs=%d}: reusing it\n",
                          pReusedMapping->pNMHolder->address,
                          (int)pReusedMapping->pNMHolder->size,
                          (int)pReusedMapping->pNMHolder->offset,
                          dwNumberOfBytesToMap, 0);

                    /* Let's check the mapping's current protection */
                    ret = mprotect(pReusedMapping->pNMHolder->address,
                                   pReusedMapping->pNMHolder->size,
                                   prot | PROT_CHECK);
                    if (0 != ret)
                    {               
                        /* We need to raise the protection to the desired 
                           one. That will give write access to any read-only
                           mapping sharing this native mapping, but there is 
                           no way around this problem on systems that do not
                           allow more than one mapping per file region, per
                           process */
                        TRACE("Raising protections on mapping @ %p to 0x%x\n",
                              pReusedMapping->pNMHolder->address, prot);
                        ret = mprotect(pReusedMapping->pNMHolder->address,
                                   pReusedMapping->pNMHolder->size,
                                   prot);
                    }

                    if (ret != 0) 
                    {
                        ERROR( "Failed setting protections on reused mapping\n");

                        NativeMapHolderRelease(pThread, pReusedMapping->pNMHolder);
                        InternalFree(pReusedMapping);
                        pReusedMapping = NULL;
                    }
                }
            }
#endif // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
        }
        else
        {
            ASSERT( "MapFileMapToMmapFlags failed!\n" );
            palError = ERROR_INTERNAL_ERROR;
            goto InternalMapViewOfFileLeaveCriticalSection;
        }
    }

    if (MAP_FAILED == pvBaseAddress
#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
         &&  (pReusedMapping == NULL)
#endif // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
        )
    {
        ERROR( "mmap failed with code %s.\n", strerror( errno ) );
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto InternalMapViewOfFileLeaveCriticalSection;

    }

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    if (pReusedMapping != NULL)
    {
        //
        // Add a reference to the file mapping object the reused mapping
        // points to (note that it may be different than the object this
        // call was actually made against) and add the view to the global
        // list. All other initialization took place in
        // FindSharedMappingReplacement
        //
        
        pvBaseAddress = pReusedMapping->lpAddress;
        pReusedMapping->pFileMapping->AddReference();
        InsertTailList(&MappedViewList, &pReusedMapping->Link);
    }
    else 
#endif // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    {
        //
        // Allocate and fill out a new view structure, and add it to
        // the global list.
        //
        
        PMAPPED_VIEW_LIST pNewView = (PMAPPED_VIEW_LIST)InternalMalloc(sizeof(*pNewView));
        if (NULL != pNewView)
        {
            pNewView->lpAddress = pvBaseAddress;
            pNewView->NumberOfBytesToMap = dwNumberOfBytesToMap;
            pNewView->dwDesiredAccess = dwDesiredAccess;
            pNewView->pFileMapping = pMappingObject;
            pNewView->pFileMapping->AddReference();
            pNewView->lpPEBaseAddress = 0;
            InsertTailList(&MappedViewList, &pNewView->Link);

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
            pNewView->MappedFileDevNum = pProcessLocalData->MappedFileDevNum;
            pNewView->MappedFileInodeNum = pProcessLocalData->MappedFileInodeNum;
            
            pNewView->pNMHolder = NewNativeMapHolder(
                pThread,
                pvBaseAddress, 
                dwNumberOfBytesToMap,
                0,
                1
                );

            if (NULL == pNewView->pNMHolder)
            {
                pNewView->pFileMapping->ReleaseReference(pThread);
                RemoveEntryList(&pNewView->Link);
                InternalFree(pNewView);
                palError = ERROR_INTERNAL_ERROR;
            }
#endif // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
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
    
#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
    NativeMapHolderRelease(pThread, pView->pNMHolder);
    pView->pNMHolder = NULL;
#else
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
#endif

    RemoveEntryList(&pView->Link);
    pMappingObject = pView->pFileMapping;
    InternalFree(pView);
    
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
    MAPMmapProtToAccessFlags

    Converts unix protection flags to file access flags.
    We ignore PROT_EXEC.
--*/
static uint32_t MAPMmapProtToAccessFlags( int prot )
{
    uint32_t flAccess = 0; // default: no access

    if (PROT_NONE == prot)
    {
        flAccess = 0;
    }
    else if ( ((PROT_READ | PROT_WRITE) & prot) == (PROT_READ | PROT_WRITE) )
    {
        flAccess = FILE_MAP_ALL_ACCESS;
    }
    else if ( (PROT_WRITE & prot) == PROT_WRITE )
    {
        flAccess = FILE_MAP_WRITE;
    }
    else if ( (PROT_READ & prot) == PROT_READ )
    {
        flAccess = FILE_MAP_READ;
    }
    else
    {
        ASSERT( "Unknown Unix protection flag\n" );
    }

    return flAccess;
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
        SIZE_T real_map_sz;
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS
        real_map_addr = pView->pNMHolder->address;
        real_map_sz = pView->pNMHolder->size;
#else
        real_map_addr = pView->lpAddress;
        real_map_sz = pView->NumberOfBytesToMap;
#endif

        MappedSize = ((real_map_sz-1) & ~VIRTUAL_PAGE_MASK) + VIRTUAL_PAGE_SIZE; 
        if ( real_map_addr <= lpAddress && 
             (void *)((UINT_PTR)real_map_addr+MappedSize) > lpAddress )
        {
            if (lpBuffer)
            {
                SIZE_T regionSize = MappedSize + (UINT_PTR) real_map_addr - 
                       ((UINT_PTR) lpAddress & ~VIRTUAL_PAGE_MASK);

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

#if ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS

//
// Callers of FindSharedMappingReplacement must hold mapping_critsec
//

static PMAPPED_VIEW_LIST FindSharedMappingReplacement(
    CPalThread *pThread,
    dev_t deviceNum,
    ino_t inodeNum,
    SIZE_T size, 
    SIZE_T offset)
{
    PMAPPED_VIEW_LIST pNewView = NULL;
    
    if (size == 0)
    {
        ERROR("Mapping size cannot be NULL\n");
        return NULL;
    }

    for (LIST_ENTRY *pLink = MappedViewList.Flink;
         pLink != &MappedViewList;
         pLink = pLink->Flink)
    {
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        if (pView->MappedFileDevNum != deviceNum
            || pView->MappedFileInodeNum != inodeNum
            || pView->dwDesiredAccess == FILE_MAP_COPY)
        {
            continue;
        }

        //
        // This is a shared mapping for the same indoe / device. Now, check
        // to see if it overlaps with the range for the new view
        //

        SIZE_T real_map_offs = pView->pNMHolder->offset;
        SIZE_T real_map_sz = pView->pNMHolder->size;

        if (real_map_offs <= offset
            && real_map_offs+real_map_sz >= offset)
        {
            //
            // The views overlap. Even if this view is not reusable for the
            // new once the search is over, as on
            // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS systems there
            // cannot be shared mappings of two overlapping regions of the
            // same file, in the same process. Therefore, whether this view
            // is reusable or not we cannot mmap the requested region of
            // the specified file.
            //

            if (real_map_offs+real_map_sz >= offset+size)
            {
                /* The new desired mapping is fully contained in the 
                   one just found: we can reuse this one */

                pNewView = (PMAPPED_VIEW_LIST)InternalMalloc(sizeof(MAPPED_VIEW_LIST));
                if (pNewView)
                {
                    memcpy(pNewView, pView, sizeof(*pNewView));
                    NativeMapHolderAddRef(pNewView->pNMHolder);
                    pNewView->lpAddress = (void*)((char*)pNewView->pNMHolder->address +
                        offset - pNewView->pNMHolder->offset);
                    pNewView->NumberOfBytesToMap = size; 
                }
                else
                {
                    ERROR("No memory for new MAPPED_VIEW_LIST node\n");
                }
            }

            break;
        }
    }

    TRACE ("FindSharedMappingReplacement returning %p\n", pNewView);
    return pNewView;
}

static NativeMapHolder * NewNativeMapHolder(CPalThread *pThread, void * address, SIZE_T size,
                                     SIZE_T offset, long init_ref_count)
{
    NativeMapHolder * pThisMapHolder;
    
    if (init_ref_count < 0)
    {
        ASSERT("Negative initial reference count for new map holder\n");
        return NULL;
    }
	
    pThisMapHolder = 
        (NativeMapHolder *)InternalMalloc(sizeof(NativeMapHolder));
        
    if (pThisMapHolder)
    {
        pThisMapHolder->ref_count = init_ref_count;
        pThisMapHolder->address = address;
        pThisMapHolder->size = size;
        pThisMapHolder->offset = offset;
    }
    
    return pThisMapHolder;
}

static int32_t NativeMapHolderAddRef(NativeMapHolder * thisNMH)
{
    int32_t ret = InterlockedIncrement(&thisNMH->ref_count);
    return ret;
}

static int32_t NativeMapHolderRelease(CPalThread *pThread, NativeMapHolder * thisNMH)
{
    int32_t ret = InterlockedDecrement(&thisNMH->ref_count);
    if (ret == 0)
    {
        if (-1 == munmap(thisNMH->address, thisNMH->size))
        {
            ASSERT( "Unable to unmap memory. Error=%s.\n",
                    strerror( errno ) );
        }
        else
        {
            TRACE( "Successfully unmapped %p (size=%lu)\n", 
                   thisNMH->address, (unsigned long)thisNMH->size);
        }
        InternalFree (thisNMH);
    }
    else if (ret < 0)
    {
        ASSERT( "Negative reference count for map holder %p"
                " {address=%p, size=%lu}\n", thisNMH->address, 
                (unsigned long)thisNMH->size);
    }

    return ret;
}

#endif // ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS

// Record a mapping in the MappedViewList list.
// This call assumes the mapping_critsec has already been taken.
static PAL_ERROR
MAPRecordMapping(
    IPalObject *pMappingObject,
    void *pPEBaseAddress,
    void *addr,
    size_t len,
    int prot
    )
{
    if (pPEBaseAddress == NULL)
    {
        return ERROR_INTERNAL_ERROR;
    }

    PAL_ERROR palError = NO_ERROR;
    PMAPPED_VIEW_LIST pNewView;
    pNewView = (PMAPPED_VIEW_LIST)InternalMalloc(sizeof(*pNewView));
    if (NULL != pNewView)
    {
        pNewView->lpAddress = addr;
        pNewView->NumberOfBytesToMap = len;
        pNewView->dwDesiredAccess = MAPMmapProtToAccessFlags(prot);
        pMappingObject->AddReference();
        pNewView->pFileMapping = pMappingObject;
        pNewView->lpPEBaseAddress = pPEBaseAddress;
        InsertTailList(&MappedViewList, &pNewView->Link);

        TRACE_(LOADER)("Added address %p, size 0x%x, to the mapped file list.\n", addr, len);
    }
    else
    {
        palError = ERROR_INTERNAL_ERROR;
    }

    return palError;
}

// Do the actual mmap() call, and record the mapping in the MappedViewList list.
// This call assumes the mapping_critsec has already been taken.
static PAL_ERROR
MAPmmapAndRecord(
    IPalObject *pMappingObject,
    void *pPEBaseAddress,
    void *addr,
    size_t len,
    int prot,
    int flags,
    int fd,
    off_t offset,
    void * *ppvBaseAddress
    )
{
    _ASSERTE(pPEBaseAddress != NULL);

    PAL_ERROR palError = NO_ERROR;
    void * pvBaseAddress = NULL;

    pvBaseAddress = mmap(addr, len, prot, flags, fd, offset);
    if (MAP_FAILED == pvBaseAddress)
    {
        ERROR_(LOADER)( "mmap failed with code %d: %s.\n", errno, strerror( errno ) );
        palError = FILEGetLastErrorFromErrno();
    }
    else
    {
        palError = MAPRecordMapping(pMappingObject, pPEBaseAddress, pvBaseAddress, len, prot);
        if (NO_ERROR != palError)
        {
            if (-1 == munmap(pvBaseAddress, len))
            {
                ERROR_(LOADER)("Unable to unmap the file. Expect trouble.\n");
            }
        }
        else
        {
            *ppvBaseAddress = pvBaseAddress;
        }
    }

    return palError;
}

/*++
    MAPMapPEFile -

    Map a PE format file into memory like Windows LoadLibrary() would do.
    Doesn't apply base relocations if the function is relocated.

Parameters:
    IN hFile - file to map

Return value:
    non-NULL - the base address of the mapped image
    NULL - error, with last error set.
--*/

void * MAPMapPEFile(HANDLE hFile)
{
    PAL_ERROR palError = 0;
    IPalObject *pFileObject = NULL;
    IDataLock *pLocalDataLock = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    CPalThread *pThread = InternalGetCurrentThread();
    void * loadedBase = NULL;
    IMAGE_DOS_HEADER * loadedHeader = NULL;
    void * retval;
#if _DEBUG
    bool forceRelocs = false;
#endif

    ENTRY("MAPMapPEFile (hFile=%p)\n", hFile);

    //Step 0: Verify values, find internal pal data structures.
    if (INVALID_HANDLE_VALUE == hFile)
    {
        ERROR_(LOADER)( "Invalid file handle\n" );
        palError = ERROR_INVALID_HANDLE;
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
        ERROR_(LOADER)( "ReferenceObjectByHandle failed\n" );
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
        ERROR_(LOADER)( "GetProcessLocalData failed\n" );
        goto done;
    }

    int fd;
    fd = pLocalData->unix_fd;
    //Step 1: Read the PE headers and reserve enough space for the whole image somewhere.
    IMAGE_DOS_HEADER dosHeader;
    IMAGE_NT_HEADERS ntHeader;
    errno = 0;
    if (0 != lseek(fd, 0, SEEK_SET))
    {
        palError = FILEGetLastErrorFromErrno();
        ERROR_(LOADER)( "lseek failed\n" );
        goto done;
    }
    if (sizeof(dosHeader) != read(fd, &dosHeader, sizeof(dosHeader)))
    {
        palError = FILEGetLastErrorFromErrno();
        ERROR_(LOADER)( "reading dos header failed\n" );
        goto done;
    }
    if (dosHeader.e_lfanew != lseek(fd, dosHeader.e_lfanew, SEEK_SET))
    {
        palError = FILEGetLastErrorFromErrno();
        goto done;
    }
    if (sizeof(ntHeader) != read(fd, &ntHeader, sizeof(ntHeader)))
    {
        palError = FILEGetLastErrorFromErrno();
        goto done;
    }

    if ((VAL16(IMAGE_DOS_SIGNATURE) != VAL16(dosHeader.e_magic))
        || (VAL32(IMAGE_NT_SIGNATURE) != VAL32(ntHeader.Signature))
        || (VAL16(IMAGE_NT_OPTIONAL_HDR_MAGIC) != VAL16(ntHeader.OptionalHeader.Magic) ) )
    {
        ERROR_(LOADER)( "Magic number mismatch\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    //this code requires that the file alignment be the same as the page alignment
    if (ntHeader.OptionalHeader.FileAlignment < VIRTUAL_PAGE_SIZE)
    {
        ERROR_(LOADER)( "Optional header file alignment is bad\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

    //This doesn't read the entire NT header (the optional header technically has a variable length.  But I
    //don't need more directories.

    //I now know how big the file is.  Reserve enough address space for the whole thing.  Try to get the
    //preferred base.  Create the intial mapping as "no access".  We'll use that for the guard pages in the
    //"holes" between sections.
    SIZE_T preferredBase, virtualSize;
    preferredBase = ntHeader.OptionalHeader.ImageBase;
    virtualSize = ntHeader.OptionalHeader.SizeOfImage;

    // Validate the image header
    if (   (preferredBase == 0)
        || (virtualSize == 0)
        || (preferredBase + virtualSize < preferredBase)    // Does the image overflow?
        )
    {
        ERROR_(LOADER)( "image is corrupt\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto done;
    }

#if _DEBUG
    char * envVar;
    envVar = getenv("PAL_ForceRelocs");
    if (envVar && strlen(envVar) > 0)
    {
        forceRelocs = true;
        TRACE_(LOADER)("Forcing rebase of image\n");
    }
    void * pForceRelocBase;
    pForceRelocBase = NULL;
    if (forceRelocs)
    {
        //if we're forcing relocs, create an anonymous mapping at the preferred base.  Only create the
        //mapping if we can create it at the specified address.
        pForceRelocBase = mmap( (void*)preferredBase, VIRTUAL_PAGE_SIZE, PROT_NONE, MAP_ANON|MAP_FIXED, -1, 0 );
        if (pForceRelocBase == MAP_FAILED)
        {
            TRACE_(LOADER)("Attempt to take preferred base of %p to force relocation failed\n", (void*)preferredBase);
            forceRelocs = false;
        }
        else if ((void*)preferredBase != pForceRelocBase)
        {
            TRACE_(LOADER)("Attempt to take preferred base of %p to force relocation failed; actually got %p\n", (void*)preferredBase, pForceRelocBase);
        }
    }
#endif // _DEBUG

    // The first mmap mapping covers the entire file but just reserves space. Subsequent mappings cover
    // individual parts of the file, and actually map pages in. Note that according to the mmap() man page, "A
    // successful mmap deletes any previous mapping in the allocated address range." Also, "If a MAP_FIXED
    // request is successful, the mapping established by mmap() replaces any previous mappings for the process' pages
    // in the range from addr to addr + len." Thus, we will record a series of mappings here, one for the header
    // and each of the sections, as well as all the space between them that we give PROT_NONE protections.

    // We're going to start adding mappings to the mapping list, so take the critical section
    InternalEnterCriticalSection(pThread, &mapping_critsec);

#if !defined(_AMD64_)
    loadedBase = mmap((void*)preferredBase, virtualSize, PROT_NONE, MAP_ANON, -1, 0);
#else // defined(_AMD64_)    
    // MAC64 requires we pass MAP_SHARED (or MAP_PRIVATE) flags - otherwise, the call is failed. 
    // Refer to mmap documentation at http://www.manpagez.com/man/2/mmap/ for details.
    loadedBase = mmap((void*)preferredBase, virtualSize, PROT_NONE, MAP_ANON|MAP_PRIVATE, -1, 0);
#endif // !defined(_AMD64_)

    if (MAP_FAILED == loadedBase)
    {
        ERROR_(LOADER)( "mmap failed with code %d: %s.\n", errno, strerror( errno ) );
        palError = FILEGetLastErrorFromErrno();
        loadedBase = NULL; // clear it so we don't try to use it during clean-up
        goto doneReleaseMappingCriticalSection;
    }

    // All subsequent mappings of the PE file will be in the range [loadedBase, loadedBase + virtualSize)

#if _DEBUG
    if (forceRelocs)
    {
        _ASSERTE(((SIZE_T)loadedBase) != preferredBase);
        munmap(pForceRelocBase, VIRTUAL_PAGE_SIZE); // now that we've forced relocation, let the original address mapping go
    }
    if (((SIZE_T)loadedBase) != preferredBase)
    {
        TRACE_(LOADER)("Image rebased from preferredBase of %p to loadedBase of %p\n", preferredBase, loadedBase);
    }
    else
    {
        TRACE_(LOADER)("Image loaded at preferred base %p\n", loadedBase);
    }
#endif // _DEBUG

    //we have now reserved memory (potentially we got rebased).  Walk the PE sections and map each part
    //separately.

    size_t headerSize;
    headerSize = VIRTUAL_PAGE_SIZE; // if there are lots of sections, this could be wrong

    //first, map the PE header to the first page in the image.  Get pointers to the section headers
    palError = MAPmmapAndRecord(pFileObject, loadedBase,
                    loadedBase, headerSize, PROT_READ, MAP_FILE|MAP_PRIVATE|MAP_FIXED, fd, 0,
                    (void**)&loadedHeader);
    if (NO_ERROR != palError)
    {
        ERROR_(LOADER)( "mmap of PE header failed\n" );
        goto doneReleaseMappingCriticalSection;
    }

    TRACE_(LOADER)("PE header loaded @ %p\n", loadedHeader);
    _ASSERTE(loadedHeader == loadedBase); // we already preallocated the space, and we used MAP_FIXED, so we should have gotten this address
    IMAGE_SECTION_HEADER * firstSection;
    firstSection = (IMAGE_SECTION_HEADER*)(((char *)loadedHeader)
                                           + loadedHeader->e_lfanew
                                           + offsetof(IMAGE_NT_HEADERS, OptionalHeader)
                                           + VAL16(ntHeader.FileHeader.SizeOfOptionalHeader));
    unsigned numSections;
    numSections = ntHeader.FileHeader.NumberOfSections;

    // Validation
    char* sectionHeaderEnd;
    sectionHeaderEnd = (char*)firstSection + numSections * sizeof(IMAGE_SECTION_HEADER);
    if (   ((void*)firstSection < loadedBase)
        || ((char*)firstSection > sectionHeaderEnd)
        || (sectionHeaderEnd > (char*)loadedBase + virtualSize)
        )
    {
        ERROR_(LOADER)( "image is corrupt\n" );
        palError = ERROR_INVALID_PARAMETER;
        goto doneReleaseMappingCriticalSection;
    }

    void* prevSectionBase;
    prevSectionBase = loadedBase; // the first "section" for our purposes is the header
    size_t prevSectionSizeInMemory;
    prevSectionSizeInMemory = headerSize;
    for (unsigned i = 0; i < numSections; ++i)
    {
        //for each section, map the section of the file to the correct virtual offset.  Gather the
        //protection bits from the PE file and convert them to the correct mmap PROT_* flags.
        void * sectionData;
        int prot = 0;
        IMAGE_SECTION_HEADER &currentHeader = firstSection[i];

        void* sectionBase = (char*)loadedBase + currentHeader.VirtualAddress;

        // Validate the section header
        if (   (sectionBase < loadedBase)                                                           // Did computing the section base overflow?
            || ((char*)sectionBase + currentHeader.SizeOfRawData < (char*)sectionBase)              // Does the section overflow?
            || ((char*)sectionBase + currentHeader.SizeOfRawData > (char*)loadedBase + virtualSize) // Does the section extend past the end of the image as the header stated?
            || ((char*)prevSectionBase + prevSectionSizeInMemory > sectionBase)                     // Does this section overlap the previous one?
            )
        {
            ERROR_(LOADER)( "section %d is corrupt\n", i );
            palError = ERROR_INVALID_PARAMETER;
            goto doneReleaseMappingCriticalSection;
        }
        if (currentHeader.Misc.VirtualSize > currentHeader.SizeOfRawData)
        {
            ERROR_(LOADER)( "no support for zero-padded sections, section %d\n", i );
            palError = ERROR_INVALID_PARAMETER;
            goto doneReleaseMappingCriticalSection;
        }

        // Is there space between the previous section and this one? If so, add a PROT_NONE mapping to cover it.
        if ((char*)prevSectionBase + prevSectionSizeInMemory < sectionBase)
        {
            char* gapBase = (char*)prevSectionBase + prevSectionSizeInMemory;
            palError = MAPRecordMapping(pFileObject,
                            loadedBase,
                            (void*)gapBase,
                            (char*)sectionBase - gapBase,
                            PROT_NONE);
            if (NO_ERROR != palError)
            {
                ERROR_(LOADER)( "recording gap section before section %d failed\n", i );
                goto doneReleaseMappingCriticalSection;
            }
        }

        //Don't discard these sections.  We need them to verify PE files
        //if (currentHeader.Characteristics & IMAGE_SCN_MEM_DISCARDABLE)
        //    continue;
        if (currentHeader.Characteristics & IMAGE_SCN_MEM_EXECUTE)
            prot |= PROT_EXEC;
        if (currentHeader.Characteristics & IMAGE_SCN_MEM_READ)
            prot |= PROT_READ;
        if (currentHeader.Characteristics & IMAGE_SCN_MEM_WRITE)
            prot |= PROT_WRITE;

        palError = MAPmmapAndRecord(pFileObject, loadedBase,
                        sectionBase,
                        currentHeader.SizeOfRawData,
                        prot,
                        MAP_FILE|MAP_PRIVATE|MAP_FIXED,
                        fd,
                        currentHeader.PointerToRawData,
                        &sectionData);
        if (NO_ERROR != palError)
        {
            ERROR_(LOADER)( "mmap of section %d failed\n", i );
            goto doneReleaseMappingCriticalSection;
        }

#if _DEBUG
        {
            // Ensure null termination of section name (which is allowed to not be null terminated if exactly 8 characters long)
            char sectionName[9];
            sectionName[8] = '\0';
            memcpy(sectionName, currentHeader.Name, sizeof(currentHeader.Name));
            TRACE_(LOADER)("Section %d '%s' (header @ %p) loaded @ %p (RVA: 0x%x, SizeOfRawData: 0x%x, PointerToRawData: 0x%x)\n",
                i, sectionName, &currentHeader, sectionData, currentHeader.VirtualAddress, currentHeader.SizeOfRawData, currentHeader.PointerToRawData);
        }
#endif // _DEBUG

        prevSectionBase = sectionBase;
        prevSectionSizeInMemory = (currentHeader.SizeOfRawData + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK; // round up to page boundary
    }

    // Is there space after the last section and before the end of the mapped image? If so, add a PROT_NONE mapping to cover it.
    char* imageEnd;
    imageEnd = (char*)loadedBase + virtualSize; // actually, points just after the mapped end
    if ((char*)prevSectionBase + prevSectionSizeInMemory < imageEnd)
    {
        char* gapBase = (char*)prevSectionBase + prevSectionSizeInMemory;
        palError = MAPRecordMapping(pFileObject,
                        loadedBase,
                        (void*)gapBase,
                        imageEnd - gapBase,
                        PROT_NONE);
        if (NO_ERROR != palError)
        {
            ERROR_(LOADER)( "recording end of image gap section failed\n" );
            goto doneReleaseMappingCriticalSection;
        }
    }

    palError = ERROR_SUCCESS;

doneReleaseMappingCriticalSection:

    InternalLeaveCriticalSection(pThread, &mapping_critsec);

done:

    if (NULL != pLocalDataLock)
    {
        pLocalDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pFileObject)
    {
        pFileObject->ReleaseReference(pThread);
    }

    if (palError == ERROR_SUCCESS)
    {
        retval = loadedBase;
        LOGEXIT("MAPMapPEFile returns %p\n", retval);
    }
    else
    {
        retval = NULL;
        LOGEXIT("MAPMapPEFile error: %d\n", palError);

        // If we had an error, and had mapped anything, we need to unmap it
        if (loadedBase != NULL)
        {
            MAPUnmapPEFile(loadedBase);
        }
    }
    return retval;
}

/*++
Function :
    MAPUnmapPEFile - unmap a PE file, and remove it from the recorded list of PE files mapped

    returns TRUE if successful, FALSE otherwise
--*/
BOOL MAPUnmapPEFile(const void * lpAddress)
{
    TRACE_(LOADER)("MAPUnmapPEFile(lpAddress=%p)\n", lpAddress);

    if ( NULL == lpAddress )
    {
        ERROR_(LOADER)( "lpAddress cannot be NULL\n" );
        return FALSE;
    }

    BOOL retval = TRUE;
    CPalThread * pThread = InternalGetCurrentThread();
    InternalEnterCriticalSection(pThread, &mapping_critsec);
    PLIST_ENTRY pLink, pLinkNext, pLinkLocal = NULL;
    unsigned nPESections = 0;

    // Look through the entire MappedViewList for all mappings associated with the
    // PE file with base address 'lpAddress'. We want to unmap all the memory
    // and then release the file mapping object. Unfortunately, based on the comment
    // in CorUnix::InternalUnmapViewOfFile(), we can't release the file mapping object
    // while within the mapping critical section. So, we unlink all the elements from the
    // main list while in the critical section, and then run through this local list
    // doing the real work after releasing the main list critical section. The order
    // of the unmapping doesn't matter, so we don't fully set all the list link pointers,
    // only a minimal set.

    for(pLink = MappedViewList.Flink;
        pLink != &MappedViewList;
        pLink = pLinkNext)
    {
        pLinkNext = pLink->Flink;
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        if (pView->lpPEBaseAddress == lpAddress) // this entry is associated with the PE file
        {
            ++nPESections; // for debugging, check that we see at least one

            RemoveEntryList(&pView->Link);
            pView->Link.Flink = pLinkLocal; // the local list is singly-linked, NULL terminated
            pLinkLocal = &pView->Link;
        }
    }

#if _DEBUG
    if (nPESections == 0)
    {
        ERROR_(LOADER)( "MAPUnmapPEFile called to unmap a file that was not in the PE file mapping list\n" );
    }
#endif // _DEBUG

    InternalLeaveCriticalSection(pThread, &mapping_critsec);

    // Now, outside the critical section, do the actual unmapping work

    for(pLink = pLinkLocal;
        pLink != NULL;
        pLink = pLinkNext)
    {
        pLinkNext = pLink->Flink;
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        // remove pView mapping from the list
        if (-1 == munmap(pView->lpAddress, pView->NumberOfBytesToMap))
        {
            // Emit an error message in a trace, but continue trying to do the rest
            ERROR_(LOADER)("Unable to unmap the file. Expect trouble.\n");
            retval = FALSE;
        }

        IPalObject* pFileObject = pView->pFileMapping;
        if (NULL != pFileObject)
        {
            pFileObject->ReleaseReference(pThread);
        }
        InternalFree(pView); // this leaves pLink dangling
    }

    TRACE_(LOADER)("MAPUnmapPEFile returning %d\n", retval);
    return retval;
}
