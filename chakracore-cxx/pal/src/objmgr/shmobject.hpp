//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    shmobject.hpp

Abstract:
    Shared memory based object



--*/

#ifndef _PAL_SHMOBJECT_HPP
#define _PAL_SHMOBJECT_HPP

#include "palobjbase.hpp"
#include "pal/shm.hpp"

extern "C"
{
#include "pal/list.h"
}

namespace CorUnix
{
    class CSimpleSharedMemoryLock : public IDataLock
    {
    public:

        void
        AcquireLock(
            IDataLock **ppDataLock
        )
        {
            SHMLock();
            *ppDataLock = static_cast<IDataLock*>(this);
        };

        void ReleaseLock([[maybe_unused]] CPalThread *pthr) override
        {
            SHMRelease();
        };
    };

    typedef struct _SHMObjData
    {
        SHMPTR shmPrevObj;
        SHMPTR shmNextObj;
        BOOL fAddedToList;
        
        SHMPTR shmObjName;
        SHMPTR shmObjImmutableData;

        int32_t lProcessRefCount;
        uint32_t dwNameLength;

        PalObjectTypeId eTypeId;

        void * pvSynchData;
    } SHMObjData;

    class CSharedMemoryObject : public CPalObjectBase
    {
    protected:

        //
        // Entry on the process's named or anonymous object list
        //

        LIST_ENTRY m_le;

        //
        // The lock that guards access to that list
        //

        CRITICAL_SECTION *m_pcsObjListLock;

        //
        // m_fSharedDataDereferenced will be TRUE if DereferenceSharedData
        // has already been called. (N.B. -- this is a int32_t instead of a bool
        // because it is passed to InterlockedExchange). If the shared data blob
        // should be freed in the object's destructor (i.e., SHMfree should be
        // called on the appropriate SHMPTRs) DereferenceSharedData will
        // set m_fDeleteSharedData to TRUE.
        //

        int32_t m_fSharedDataDereferenced;
        int32_t m_fDeleteSharedData;

        static
        void
        FreeSharedDataAreas(
            SHMPTR shmObjData
            );

        bool
        DereferenceSharedData();

        virtual
        void
        AcquireObjectDestructionLock(
            CPalThread *pthr
            );

        virtual
        bool
        ReleaseObjectDestructionLock(
            CPalThread *pthr,
            bool fDestructionPending
            );
        
        virtual ~CSharedMemoryObject();

    public:

        //
        // Constructor used for new object
        //

        CSharedMemoryObject(
            CObjectType *pot,
            CRITICAL_SECTION *pcsObjListLock
            )
            :
            CPalObjectBase(pot),
            m_pcsObjListLock(pcsObjListLock),
            m_fSharedDataDereferenced(FALSE),
            m_fDeleteSharedData(FALSE)
        {
            InitializeListHead(&m_le);
        };

        virtual
        PAL_ERROR
        Initialize(CPalThread *pthr);

        PLIST_ENTRY
        GetObjectListLink(
            void
            )
        {
            return &m_le;
        }

        //
        // Clients of this object -- in particular, CSharedMemoryObjectManager
        // -- can't use CONTAINING_RECORD directly, since they don't have
        // access to m_Link.
        //

        //
        // IPalObject routines
        //

        virtual
        PAL_ERROR
        GetSynchStateController(
            CPalThread *pthr,
            ISynchStateController **ppStateController
            );

        virtual
        PAL_ERROR
        GetObjectSynchData(
            void **ppvSynchData
            );

    };

    class CSharedMemoryWaitableObject : public CSharedMemoryObject
    {
    protected:

        void *m_pvSynchData;

        virtual ~CSharedMemoryWaitableObject();
        
    public:

        CSharedMemoryWaitableObject(
            CObjectType *pot,
            CRITICAL_SECTION *pcsObjListLock
            )
            :
            CSharedMemoryObject(pot, pcsObjListLock),
            m_pvSynchData(NULL)
        {
        };

        virtual
        PAL_ERROR
        Initialize(CPalThread *pthr);

        //
        // IPalObject routines
        //

        virtual
        PAL_ERROR
        GetSynchStateController(
            CPalThread *pthr,
            ISynchStateController **ppStateController
            );

        virtual
        PAL_ERROR
        GetObjectSynchData(
            void **ppvSynchData
            );
    };

}

#endif // _PAL_SHMOBJECT_HPP

