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

#include "shmobject.hpp"
#include <new>
#include "pal/cs.hpp"
#include "pal/dbgmsg.h"
#include "chakra/Logger.h"

#include <stddef.h>

SET_DEFAULT_DEBUG_CHANNEL(PAL);

using namespace CorUnix;

/*++
Function:
  CSharedMemoryObject::Initialize

  Performs possibly-failing initialization for a newly-constructed
  object

Parameters:
  pthr -- thread data for calling thread
  poa -- the object attributes (e.g., name) for the object
--*/

PAL_ERROR
CSharedMemoryObject::Initialize(CPalThread *pthr)
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pthr);

    palError = CPalObjectBase::Initialize(pthr);
    if (NO_ERROR != palError)
    {
        goto InitializeExit;
    }

InitializeExit:

    LOGEXIT("CSharedMemoryObject::Initialize returns %d\n", palError);

    return palError;
}

/*++
Function:
  CSharedMemoryObject::FreeSharedDataItems

  Frees the shared memory structures referenced by the provided shared
  memory pointer

Parameters:
  shmObjData -- shared memory pointer to the structures to free
--*/

// static
void
CSharedMemoryObject::FreeSharedDataAreas(
    SHMPTR shmObjData
    )
{
    SHMObjData *psmod;

    assert(SHMNULL != shmObjData);

    SHMLock();
        
    psmod = SHMPTR_TO_TYPED_PTR(SHMObjData, shmObjData);
    assert(NULL != psmod);
    
    if (SHMNULL != psmod->shmObjImmutableData)
    {
        SHMfree(psmod->shmObjImmutableData);
    }

    if (SHMNULL != psmod->shmObjName)
    {
        SHMfree(psmod->shmObjName);
    }
    
    SHMfree(shmObjData);

    SHMRelease();

    LOGEXIT("CSharedMemoryObject::FreeSharedDataAreas\n");
}

/*++
Function:
  CSharedMemoryObject::AcquireObjectDestructionLock

  Acquires the lock that must be held when decrementing the object's
  reference count (and, if the count drops to 0, while removing the
  object from the object manager's lists).

Parameters:
  pthr -- thread data for the calling thread
--*/

void
CSharedMemoryObject::AcquireObjectDestructionLock(
    CPalThread *pthr
    )
{
    assert(NULL != pthr);
    
    InternalEnterCriticalSection(pthr, m_pcsObjListLock);

    LOGEXIT("CSharedMemoryObject::AcquireObjectDestructionLock\n");
}

/*++
Function:
  CSharedMemoryObject::ReleaseObjectDestructionLock

  Releases the lock acquired by AcquireObjectDestructionLock

Parameters:
  pthr -- thread data for the calling thread
  fDestructionPending -- if TRUE, the reference count for this
    object has dropped to 0; the object will be destroyed after
    this routine returns
--*/

bool
CSharedMemoryObject::ReleaseObjectDestructionLock(
    CPalThread *pthr,
    bool fDestructionPending
    )
{
    bool fCleanupSharedState = FALSE;

    assert(NULL != pthr);

    if (fDestructionPending)
    {
        RemoveEntryList(&m_le);
        fCleanupSharedState = DereferenceSharedData();
    }

    InternalLeaveCriticalSection(pthr, m_pcsObjListLock);

    LOGEXIT("CSharedMemoryObject::ReleaseObjectDestructionLock returns %d\n",
        fCleanupSharedState
        );

    return fCleanupSharedState;
}

/*++
Function:
  CSharedMemoryObject::DereferenceSharedData

  Called to decrement the global refcount (i.e., the count of
  the number of processes that have reference to the object) when
  the local reference to the object is being destroyed.

Return value:
  Returns TRUE if this process needs to clean up the object's shared
  data (i.e., the global refcount has dropped to 0, or the object
  is in the local domain)
--*/

bool
CSharedMemoryObject::DereferenceSharedData()
{

    int32_t fSharedDataAlreadDereferenced = InterlockedExchange(&m_fSharedDataDereferenced, TRUE);

    if (!fSharedDataAlreadDereferenced)
    {   
        //
        // If the object is local the shared data needs to be
        // deleted by definition
        //

        m_fDeleteSharedData = TRUE;
    }
    else
    {
        chakra::Logger::error("Multiple calls to DereferenceSharedData\n");
    }

    return m_fDeleteSharedData;
}

/*++
Function:
  CSharedMemoryObject::~CSharedMemoryObject

  Destructor; should only be called from ReleaseReference
--*/

CSharedMemoryObject::~CSharedMemoryObject()
{
    if (!m_fSharedDataDereferenced)
    {
        chakra::Logger::error("DereferenceSharedData not called before object destructor -- delete called directly?\n");
        DereferenceSharedData();
    }

    LOGEXIT("CSharedMemoryObject::~CSharedMemoryObject\n");
}

//
// C++ standard, 18.1.5 - offsetof requires a POD (plain old data) struct or
// union. Since offsetof is a macro, gcc doesn't actually check for improper
// use of offsetof, it keys off of the -> from NULL (which is also invalid for
// non-POD types by 18.1.5)
//
// As we have numerous examples of this behavior in our codebase,
// making an offsetof which doesn't use 0.
//
// PAL_safe_offsetof is a version of offsetof that protects against an
// overridden operator&
//

#define PAL_safe_offsetof(s,m) (static_cast<size_t>(reinterpret_cast<ptrdiff_t>(&reinterpret_cast<char&>((reinterpret_cast<s *>(64))->m)))-64)

/*++
Function:
  CSharedMemoryObject::GetSynchStateController

  Obtain a synchronization state controller for this object. Should
  never be called.

Parameters:
  pthr -- thread data for calling thread
  ppStateController -- on success, receives a pointer to the state controller
    instance
--*/

PAL_ERROR
CSharedMemoryObject::GetSynchStateController(
    CPalThread *pthr,
    ISynchStateController **ppStateController    // OUT
    )
{
    assert(NULL != pthr);
    assert(NULL != ppStateController);
    
    //
    // This is not a waitable object!
    //

    chakra::Logger::error("Attempt to obtain a synch state controller on a non-waitable object\n");
    return ERROR_INVALID_HANDLE;
}

/*++
Function:
  CSharedMemoryObject::GetObjectSynchData

  Obtain the synchronization data for this object. Should
  never be called.

Parameters:
  ppvSynchData -- on success, receives a pointer to the object's synch data
--*/

PAL_ERROR
CSharedMemoryObject::GetObjectSynchData(
    void **ppvSynchData             // OUT
    )
{
    assert(NULL != ppvSynchData);
    
    //
    // This is not a waitable object!!!
    //

    chakra::Logger::error("Attempt to obtain a synch data for a non-waitable object\n");
    return ERROR_INVALID_HANDLE;
}

/*++
Function:
  CSharedMemoryWaitableObject::Initialize

  Performs possibly-failing initialization for a newly-constructed
  object

Parameters:
  pthr -- thread data for calling thread
  poa -- the object attributes (e.g., name) for the object
--*/

PAL_ERROR
CSharedMemoryWaitableObject::Initialize(CPalThread *pthr)
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pthr);

    palError = CSharedMemoryObject::Initialize(pthr);
    if (NO_ERROR != palError)
    {
        goto InitializeExit;
    }

    //
    // Sanity check the passed in object type
    //

    assert(CObjectType::WaitableObject == m_pot->GetSynchronizationSupport());

    palError = g_pSynchronizationManager->AllocateObjectSynchData(m_pot, &m_pvSynchData);

InitializeExit:

    LOGEXIT("CSharedMemoryWaitableObject::Initialize returns %d\n", palError);

    return palError;
}

/*++
Function:
  CSharedMemoryWaitableObject::EnsureObjectIsShared

  If this object is not yet in the shared domain allocate the necessary
  shared memory structures for it and copy the object's data into those
  structures

Parameters:
  pthr -- thread data for the calling thread
--*/

/*++
Function:
  CSharedMemoryWaitableObject::~CSharedMemoryWaitableObject

  Destructor; should only be called from ReleaseReference
--*/

CSharedMemoryWaitableObject::~CSharedMemoryWaitableObject()
{
    if (!m_fSharedDataDereferenced)
    {
        chakra::Logger::error("DereferenceSharedData not called before object destructor -- delete called directly?\n");
        DereferenceSharedData();
    }
    
    if (NULL != m_pvSynchData && m_fDeleteSharedData)
    {
        g_pSynchronizationManager->FreeObjectSynchData(m_pvSynchData);
    }

    LOGEXIT("CSharedMemoryWaitableObject::~CSharedMemoryWaitableObject\n");
}

/*++
Function:
  CSharedMemoryWaitableObject::GetSynchStateController

  Obtain a synchronization state controller for this object.

Parameters:
  pthr -- thread data for calling thread
  ppStateController -- on success, receives a pointer to the state controller
    instance
--*/

PAL_ERROR
CSharedMemoryWaitableObject::GetSynchStateController(
    CPalThread *pthr,                // IN, OPTIONAL
    ISynchStateController **ppStateController    // OUT
    )
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pthr);
    assert(NULL != ppStateController);

    //
    // We need to grab the local synch lock before creating the controller
    // (otherwise we could get promoted after passing in our parameters)
    //

    g_pSynchronizationManager->AcquireProcessLock(pthr);
    
    palError = g_pSynchronizationManager->CreateSynchStateController(pthr, m_pot, m_pvSynchData, ppStateController);

    g_pSynchronizationManager->ReleaseProcessLock(pthr);

    LOGEXIT("CSharedMemoryWaitableObject::GetSynchStateController returns %d\n",
        palError
        );

    return palError;
}

/*++
Function:
  CSharedMemoryWaitableObject::GetSynchWaitController

  Obtain a synchronization wait controller for this object.

Parameters:
  pthr -- thread data for calling thread
  ppWaitController -- on success, receives a pointer to the wait controller
    instance
--*/

/*++
Function:
  CSharedMemoryWaitableObject::GetObjectSynchData

  Obtain the synchronization data for this object. This method should only
  be called by the synchronization manager

Parameters:
  ppvSynchData -- on success, receives a pointer to the object's synch data
--*/

PAL_ERROR
CSharedMemoryWaitableObject::GetObjectSynchData(
    void **ppvSynchData             // OUT
    )
{
    assert(NULL != ppvSynchData);
    
    *ppvSynchData = m_pvSynchData;

    LOGEXIT("CSharedMemoryWaitableObject::GetObjectSynchData returns %d\n",
        NO_ERROR
        );
    
    return NO_ERROR;
}

