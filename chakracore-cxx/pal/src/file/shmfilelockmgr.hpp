//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    shmfilelockmgr.hpp

Abstract:
    Shared memory based file lock manager



--*/

#ifndef _PAL_SHMFILELOCKMGR_H_
#define _PAL_SHMFILELOCKMGR_H_

#include "pal/corunix.hpp"
#include "pal/shm.hpp"

namespace CorUnix
{
    #define SHARE_MODE_NOT_INITALIZED 0xFFFFFFFF

    typedef struct
    {
        SHMPTR unix_filename;
        SHMPTR fileLockedRgns;    
        uint32_t refCount;
        SHMPTR next;
        SHMPTR prev;
        uint32_t  share_mode; /* FILE_SHARE_READ, FILE_SHARE_WRITE,
                              FILE_SHARE_DELETE,  0 ( not shared ) or
                              SHARE_MODE_NOT_INITALIZED */
        int nbReadAccess;  /* used to keep track of the minimal
                              access permissions */
        int nbWriteAccess;
    } SHMFILELOCKS;

    typedef enum
    {
        USER_LOCK_RGN, /* Used only for user locks (LockFile or UnlockFile call) */
        RDWR_LOCK_RGN  /* Used to distinguish between the user locks and the internal 
                          locks made when  reading, writing or truncating file */
    } LOCK_TYPE;

    typedef struct
    {
        uint32_t processId;
        void * pvControllerInstance;
        UINT64 lockRgnStart; 
        UINT64 nbBytesLocked;
        LOCK_TYPE lockType;

        SHMPTR next;
    } SHMFILELOCKRGNS;
    
    class CSharedMemoryFileLockMgr : public IFileLockManager
    {
    public:
        
        virtual
        PAL_ERROR
        GetLockControllerForFile(
            CPalThread *pThread,                // IN, OPTIONAL
            LPCSTR szFileName,
            uint32_t dwAccessRights,
            uint32_t dwShareMode,
            IFileLockController **ppLockController  // OUT
            );

        virtual
        PAL_ERROR
        GetFileShareModeForFile(
            LPCSTR szFileName,
            uint32_t* pdwShareMode);
    };

    class CSharedMemoryFileLockController : public IFileLockController
    {
    private:
        uint32_t m_dwAccessRights;
        SHMPTR m_shmFileLocks;
    protected:
        virtual ~CSharedMemoryFileLockController()
        {
        };
        
    public:

        CSharedMemoryFileLockController(
            uint32_t dwAccessRights,
            SHMPTR shmFileLocks
            )
            :
            m_dwAccessRights(dwAccessRights),
            m_shmFileLocks(shmFileLocks)
        {
        };

        virtual
        PAL_ERROR
        GetTransactionLock(
            CPalThread *pThread,                // IN, OPTIONAL
            FileTransactionLockType eLockType,
            uint32_t dwOffsetLow,
            uint32_t dwOffsetHigh,
            uint32_t nNumberOfBytesToLockLow,
            uint32_t nNumberOfBytesToLockHigh,
            IFileTransactionLock **ppTransactionLock    // OUT
            );

        virtual
        PAL_ERROR
        CreateFileLock(
            CPalThread *pThread,                // IN, OPTIONAL
            uint32_t dwOffsetLow,
            uint32_t dwOffsetHigh,
            uint32_t nNumberOfBytesToLockLow,
            uint32_t nNumberOfBytesToLockHigh,
            FileLockExclusivity eFileLockExclusivity,
            FileLockWaitMode eFileLockWaitMode
            );

        virtual
        PAL_ERROR
        ReleaseFileLock(
            CPalThread *pThread,                // IN, OPTIONAL
            uint32_t dwOffsetLow,
            uint32_t dwOffsetHigh,
            uint32_t nNumberOfBytesToUnlockLow,
            uint32_t nNumberOfBytesToUnlockHigh
            );

        virtual
        void
        ReleaseController();
    };

    class CSharedMemoryFileTransactionLock : public IFileTransactionLock
    {
    private:

        SHMPTR m_shmFileLocks;
        void * m_pvControllerInstance;
        UINT64 m_lockRgnStart;
        UINT64 m_nbBytesToLock;
    protected:
        virtual ~CSharedMemoryFileTransactionLock()
        {
        };
        
    public:

        CSharedMemoryFileTransactionLock(
            SHMPTR shmFileLocks,
            void * pvControllerInstance,
            UINT64 lockRgnStart,
            UINT64 nbBytesToLock
            )
            :
            m_shmFileLocks(shmFileLocks),
            m_pvControllerInstance(pvControllerInstance),
            m_lockRgnStart(lockRgnStart),
            m_nbBytesToLock(nbBytesToLock)
        {
        };

        virtual
        void
        ReleaseLock();
    };
}

#endif /* _PAL_SHMFILELOCKMGR_H_ */

