//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/synchobjects.hpp

Abstract:
    Header file for synchronization manager and controllers



--*/

#ifndef _SINCHOBJECTS_HPP_
#define _SINCHOBJECTS_HPP_

#include "corunix.hpp"
#include "threadinfo.hpp"
#include "shm.hpp"
#include "list.h"

#include <pthread.h>

#define SharedID SHMPTR
#define SharedPoolId size_t
#define DefaultSharedPool ((size_t)0)
#define NULLSharedID ((SHMPTR)NULL)
#define SharedIDToPointer(shID) SHMPTR_TO_TYPED_PTR(void *, shID)
#define SharedIDToTypePointer(TYPE,shID) SHMPTR_TO_TYPED_PTR(TYPE, shID)
#define RawSharedObjectAlloc(szSize, shPoolId) SHMalloc(szSize)
#define RawSharedObjectFree(shID) SHMfree(shID)
    
namespace CorUnix
{   
    uint32_t InternalWaitForMultipleObjectsEx(
        CPalThread * pthrCurrent,
        uint32_t nCount,
        const HANDLE *lpHandles,
        BOOL bWaitAll,
        uint32_t dwMilliseconds,
        BOOL bAlertable);
    
    PAL_ERROR InternalSleepEx(
        CPalThread * pthrCurrent,
        uint32_t dwMilliseconds,
        BOOL bAlertable);
        
    enum THREAD_STATE
    {
        TS_IDLE,
        TS_STARTING,
        TS_RUNNING,
        TS_FAILED,
        TS_DONE,
    };

    // forward declarations
    struct _ThreadWaitInfo;
    struct _WaitingThreadsListNode;
    class CSynchData;
    
    typedef struct _WaitingThreadsListNode * PWaitingThreadsListNode;
    typedef struct _OwnedObjectsListNode * POwnedObjectsListNode;
    typedef struct _ThreadApcInfoNode * PThreadApcInfoNode;
        
    typedef struct _ThreadWaitInfo
    {   
        WaitType wtWaitType;
        WaitDomain wdWaitDomain;
        int32_t lObjCount;
        int32_t lSharedObjCount;
        CPalThread * pthrOwner;        
        PWaitingThreadsListNode rgpWTLNodes[MAXIMUM_WAIT_OBJECTS];
        
        _ThreadWaitInfo() : wtWaitType(SingleObject), wdWaitDomain(LocalWait), 
                            lObjCount(0), lSharedObjCount(0), 
                            pthrOwner(NULL) {}
    } ThreadWaitInfo;

    typedef struct _ThreadNativeWaitData 
    {
#if !SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
        pthread_mutex_t     mutex;
        pthread_cond_t      cond;  
        int                 iPred;
#else // SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
        int                 iPipeRd;
        int                 iPipeWr;
#endif // SYNCHMGR_PIPE_BASED_THREAD_BLOCKING

        uint32_t               dwObjectIndex;
        ThreadWakeupReason  twrWakeupReason;
        bool                fInitialized;

        _ThreadNativeWaitData() : 
#if !SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
            iPred(0), 
#else // SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
            iPipeRd(-1),
            iPipeWr(-1),
#endif // SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
            dwObjectIndex(0), 
            twrWakeupReason(WaitSucceeded), 
            fInitialized(false)
        {
        }

        ~_ThreadNativeWaitData();
    } ThreadNativeWaitData;

    class CThreadSynchronizationInfo : public CThreadInfoInitializer
    {
        friend class CPalSynchronizationManager;
        friend class CSynchWaitController;

        THREAD_STATE          m_tsThreadState; 
        SharedID              m_shridWaitAwakened;
        Volatile<int32_t>        m_lLocalSynchLockCount;
        Volatile<int32_t>        m_lSharedSynchLockCount;
        LIST_ENTRY            m_leOwnedObjsList;

        ThreadNativeWaitData  m_tnwdNativeData;
        ThreadWaitInfo        m_twiWaitInfo;

#ifdef SYNCHMGR_SUSPENSION_SAFE_CONDITION_SIGNALING
        static const int      PendingSignalingsArraySize = 10;
        int32_t                  m_lPendingSignalingCount;
        CPalThread *          m_rgpthrPendingSignalings[PendingSignalingsArraySize];
        LIST_ENTRY            m_lePendingSignalingsOverflowList;
#endif // SYNCHMGR_SUSPENSION_SAFE_CONDITION_SIGNALING

    public:

        CThreadSynchronizationInfo();
        virtual ~CThreadSynchronizationInfo();

        //
        // CThreadInfoInitializer methods
        //
        virtual PAL_ERROR InitializePreCreate(void);
        
        virtual PAL_ERROR InitializePostCreate(
            CPalThread *pthrCurrent,
            SIZE_T threadId,
            uint32_t dwLwpId
            );

        THREAD_STATE GetThreadState(void)
        {
            return m_tsThreadState;
        };

        void SetThreadState(THREAD_STATE tsThreadState)
        {
            m_tsThreadState = tsThreadState;
        };

        ThreadNativeWaitData * GetNativeData()
        {
            return &m_tnwdNativeData;
        }

#if SYNCHMGR_SUSPENSION_SAFE_CONDITION_SIGNALING && !SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
        PAL_ERROR RunDeferredThreadConditionSignalings();
#endif // SYNCHMGR_SUSPENSION_SAFE_CONDITION_SIGNALING && !SYNCHMGR_PIPE_BASED_THREAD_BLOCKING
    
        // NOTE: the following methods provide non-synchronized access to 
        //       the list of owned objects for this thread. Any thread 
        //       accessing this list MUST own the appropriate 
        //       synchronization lock(s).
        void AddObjectToOwnedList(POwnedObjectsListNode pooln);
        void RemoveObjectFromOwnedList(POwnedObjectsListNode pooln);
        POwnedObjectsListNode RemoveFirstObjectFromOwnedList(void);

        // The following methods provide access to the native wait lock for 
        // those implementations that need a lock to protect the support for 
        // native thread blocking (e.g.: pthread conditions)
        void AcquireNativeWaitLock(void);
        void ReleaseNativeWaitLock(void);
        bool TryAcquireNativeWaitLock(void);
    };

    class CThreadApcInfo : public CThreadInfoInitializer
    {
        friend class CPalSynchronizationManager;

        PThreadApcInfoNode m_ptainHead;
        PThreadApcInfoNode m_ptainTail;
        
    public:
        CThreadApcInfo() :
            m_ptainHead(NULL),
            m_ptainTail(NULL)
        {
        }
    };
        
    class CPalSynchMgrController
    {
    public:
        static IPalSynchronizationManager * CreatePalSynchronizationManager();

        static PAL_ERROR PrepareForShutdown(void);
        
        static PAL_ERROR Shutdown(CPalThread *pthrCurrent, bool fFullCleanup);
    };
}

#endif // _SINCHOBJECTS_HPP_

