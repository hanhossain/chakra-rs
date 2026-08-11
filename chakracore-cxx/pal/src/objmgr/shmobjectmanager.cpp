//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    shmobjectmgr.cpp

Abstract:
    Shared memory based object manager



--*/

#include <string>
#include "shmobjectmanager.hpp"
#include "shmobject.hpp"
#include "pal/cs.hpp"
#include "pal/thread.hpp"
#include "pal/procobj.hpp"
#include "pal/dbgmsg.h"
#include "chakra/Logger.h"

SET_DEFAULT_DEBUG_CHANNEL(PAL);

#include "pal/corunix.inl"

using namespace CorUnix;

IPalObjectManager * CorUnix::g_pObjectManager;

static
PAL_ERROR
CheckObjectTypeAndRights(
    IPalObject *pobj,
    CAllowedObjectTypes *paot
);

/*++
Function:
  CSharedMemoryObjectManager::Initialize

  Performs (possibly failing) startup tasks for the object manager

Parameters:
  None
--*/

PAL_ERROR
CSharedMemoryObjectManager::Initialize(
    void
    )
{
    PAL_ERROR palError = NO_ERROR;

    InitializeListHead(&m_leNamedObjects);
    InitializeListHead(&m_leAnonymousObjects);

    InternalInitializeCriticalSection(&m_csListLock);
    m_fListLockInitialized = TRUE;

    palError = m_HandleManager.Initialize();

    LOGEXIT("CSharedMemoryObjectManager::Initialize returns %d", palError);
    
    return palError;
}

/*++
Function:
  CSharedMemoryObjectManager::Shutdown

  Cleans up the object manager. This routine will call cleanup routines
  for all objects referenced by this process. After this routine is called
  no attempt should be made to access an IPalObject.

Parameters:
  pthr -- thread data for calling thread
--*/

/*++
Function:
  CSharedMemoryObjectManager::AllocateObject

  Allocates a new object instance of the specified type.

Parameters:
  pthr -- thread data for calling thread
  pot -- type of object to allocate
  poa -- attributes (name and SD) of object to allocate
  ppobjNew -- on success, receives a reference to the new object
--*/

PAL_ERROR
CSharedMemoryObjectManager::AllocateObject(CPalThread *pthr, CObjectType *pot,
                                           IPalObject **ppobjNew // OUT
)
{
    PAL_ERROR palError = NO_ERROR;
    CSharedMemoryObject *pshmobj = NULL;

    assert(NULL != pthr);
    assert(NULL != pot);
    assert(NULL != ppobjNew);

    if (CObjectType::WaitableObject == pot->GetSynchronizationSupport())
    {
        pshmobj = new CSharedMemoryWaitableObject(pot, &m_csListLock);
    }
    else
    {
        pshmobj = new CSharedMemoryObject(pot, &m_csListLock);
    }

    if (NULL != pshmobj)
    {
        palError = pshmobj->Initialize(pthr);
        if (NO_ERROR == palError)
        {
            *ppobjNew = static_cast<IPalObject*>(pshmobj);
        }
    }
    else
    {
        ERROR("Unable to allocate pshmobj\n");
        palError = ERROR_OUTOFMEMORY;
    }

    LOGEXIT("CSharedMemoryObjectManager::AllocateObject returns %d\n", palError);
    return palError;
}

/*++
Function:
  CSharedMemoryObjectManager::RegisterObject

  Registers a newly-allocated object instance. If the object to be registered
  has a name, and a previously registered object has the same name the new
  object will not be registered.

Distinguished return values:
  ERROR_ALREADY_EXISTS -- an object of a compatible type was already registered
    with the specified name
  ERROR_INVALID_HANDLE -- an object of an incompatible type was already
    registered with the specified name

Parameters:
  pthr -- thread data for calling thread
  pobjToRegister -- the object instance to register. This routine will always
    call ReleaseReference on this instance
  paot -- object types that are compatible with the new object instance
  dwRightsRequested -- requested access rights for the returned handle (ignored)
  pHandle -- on success, receives a handle to the registered object
  ppobjRegistered -- on success, receives a reference to the registered object
    instance.
--*/

PAL_ERROR
CSharedMemoryObjectManager::RegisterObject(
    CPalThread *pthr,
    IPalObject *pobjToRegister,
    CAllowedObjectTypes *paot,
    uint32_t dwRightsRequested,
    HANDLE *pHandle,                 // OUT
    IPalObject **ppobjRegistered     // OUT
    )
{
    PAL_ERROR palError = NO_ERROR;
    CSharedMemoryObject *pshmobj = static_cast<CSharedMemoryObject*>(pobjToRegister);
    BOOL fInherit = FALSE;

    assert(NULL != pthr);
    assert(NULL != pobjToRegister);
    assert(NULL != paot);
    assert(NULL != pHandle);
    assert(NULL != ppobjRegistered);

    InternalEnterCriticalSection(pthr, &m_csListLock);

    //
    // Place the object on the anonymous object list
    //

    InsertTailList(&m_leAnonymousObjects, pshmobj->GetObjectListLink());

    //
    // Obtain a handle for the new object
    //

    palError = ObtainHandleForObject(
        pthr,
        pobjToRegister,
        dwRightsRequested,
        fInherit,
        NULL, 
        pHandle
        );

    if (NO_ERROR == palError)
    {
        //
        // Transfer pobjToRegister reference to out param
        //

        *ppobjRegistered = pobjToRegister;
        pobjToRegister = NULL;
    }
        
    InternalLeaveCriticalSection(pthr, &m_csListLock);

    if (NULL != pobjToRegister)
    {
        pobjToRegister->ReleaseReference(pthr);
    }

    LOGEXIT("CSharedMemoryObjectManager::RegisterObject return %d\n", palError);

    return palError;
}

/*++
Function:
  CSharedMemoryObjectManager::ObtainHandleForObject

  Allocated a new handle for an object

Parameters:
  pthr -- thread data for calling thread
  pobj -- the object to allocate a handle for
  dwRightsRequired -- the access rights to grant the handle; currently ignored
  fInheritHandle -- true if the handle is inheritable; ignored for all but file
    objects that represent pipes
  pProcessForHandle -- the process the handle is to be used from; currently
    must be NULL
  pNewHandle -- on success, receives the newly allocated handle
--*/

PAL_ERROR   
CSharedMemoryObjectManager::ObtainHandleForObject(
    CPalThread *pthr,
    IPalObject *pobj,
    uint32_t dwRightsRequested,
    bool fInheritHandle,
    IPalProcess *pProcessForHandle,     // IN, OPTIONAL
    HANDLE *pNewHandle                  // OUT
    )
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pthr);
    assert(NULL != pobj);
    assert(NULL != pNewHandle);

    if (NULL != pProcessForHandle)
    {
        //
        // Not yet supported
        //

        chakra::Logger::error("Caller to ObtainHandleForObject provided a process\n");
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    palError = m_HandleManager.AllocateHandle(
        pthr,
        pobj,
        dwRightsRequested,
        fInheritHandle,
        pNewHandle
        );

    LOGEXIT("CSharedMemoryObjectManager::ObtainHandleForObject return %d\n", palError);

    return palError;    
}

/*++
Function:
  CSharedMemoryObjectManager::RevokeHandle

  Removes a handle from the process's handle table, which in turn releases
  the handle's reference on the object instance it refers to

Parameters:
  pthr -- thread data for calling thread
  hHandleToRevoke -- the handle to revoke
--*/

PAL_ERROR
CSharedMemoryObjectManager::RevokeHandle(
    CPalThread *pthr,
    HANDLE hHandleToRevoke
    )
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pthr);

    palError = m_HandleManager.FreeHandle(pthr, hHandleToRevoke);

    LOGEXIT("CSharedMemoryObjectManager::RevokeHandle returns %d\n", palError);

    return palError;
}

/*++
Function:
  CSharedMemoryObjectManager::ReferenceObjectByHandle

  Returns a referenced object instance that a handle refers to

Parameters:
  pthr -- thread data for calling thread
  hHandleToReference -- the handle to reference
  paot -- acceptable types for the underlying object
  dwRightsRequired -- the access rights that the handle must have been
    granted; currently ignored
  ppobj -- on success, receives a reference to the object instance
--*/

PAL_ERROR
CSharedMemoryObjectManager::ReferenceObjectByHandle(
    CPalThread *pthr,
    HANDLE hHandleToReference,
    CAllowedObjectTypes *paot,
    IPalObject **ppobj               // OUT
)
{
    PAL_ERROR palError;
    uint32_t dwRightsGranted;
    IPalObject *pobj;

    assert(NULL != pthr);
    assert(NULL != paot);
    assert(NULL != ppobj);

    palError = m_HandleManager.GetObjectFromHandle(
        pthr,
        hHandleToReference,
        &dwRightsGranted,
        &pobj
    );

    if (NO_ERROR == palError)
    {
        palError = CheckObjectTypeAndRights(
            pobj,
            paot
        );

        if (NO_ERROR == palError)
        {
            //
            // Transfer object reference to out parameter
            //

            *ppobj = pobj;
        }
        else
        {
            pobj->ReleaseReference(pthr);
        }
    }

    LOGEXIT("CSharedMemoryObjectManager::ReferenceObjectByHandle returns %d\n",
            palError
    );

    return palError;
}

/*++
Function:
  CSharedMemoryObjectManager::ReferenceObjectByHandleArray

  Returns the referenced object instances that an array of handles
  refer to.

Parameters:
  pthr -- thread data for calling thread
  rgHandlesToReference -- the array of handles to reference
  dwHandleCount -- the number of handles in the arrayu
  paot -- acceptable types for the underlying objects
  dwRightsRequired -- the access rights that the handles must have been
    granted; currently ignored
  rgpobjs -- on success, receives references to the object instances; will
    be empty on failures
--*/

PAL_ERROR
CSharedMemoryObjectManager::ReferenceMultipleObjectsByHandleArray(
    CPalThread *pthr,
    HANDLE rghHandlesToReference[],
    uint32_t dwHandleCount,
    CAllowedObjectTypes *paot,
    IPalObject *rgpobjs[]            // OUT (caller allocated)
)
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobj = NULL;
    uint32_t dwRightsGranted;
    uint32_t dw;

    assert(NULL != pthr);
    assert(NULL != rghHandlesToReference);
    assert(0 < dwHandleCount);
    assert(NULL != paot);
    assert(NULL != rgpobjs);

    m_HandleManager.Lock(pthr);

    for (dw = 0; dw < dwHandleCount; dw += 1)
    {        
        palError = m_HandleManager.GetObjectFromHandle(
            pthr,
            rghHandlesToReference[dw],
            &dwRightsGranted,
            &pobj
            );

        if (NO_ERROR == palError)
        {
            palError = CheckObjectTypeAndRights(
                pobj,
                paot
            );

            if (NO_ERROR == palError)
            {
                //
                // Transfer reference to out array
                //

                rgpobjs[dw] = pobj;
                pobj = NULL;
            }
        }

        if (NO_ERROR != palError)
        {
            break;
        }
    }

    //
    // The handle manager lock must be released before releasing
    // any object references, as ReleaseReference will acquire
    // the object manager list lock (which needs to be acquired before
    // the handle manager lock)
    //

    m_HandleManager.Unlock(pthr);

    if (NO_ERROR != palError)
    {
        //
        // dw's current value is the failing index, so we want
        // to free from dw - 1.
        //
        
        while (dw > 0)
        {
            rgpobjs[--dw]->ReleaseReference(pthr);
        }

        if (NULL != pobj)
        {
            pobj->ReleaseReference(pthr);
        }
    }

    LOGEXIT("CSharedMemoryObjectManager::ReferenceMultipleObjectsByHandleArray"
        " returns %d\n",
        palError
        );

    return palError;
}

static PalObjectTypeId RemotableObjectTypes[] =
    {otiManualResetEvent, otiAutoResetEvent, otiMutex, otiProcess};
    
static CAllowedObjectTypes aotRemotable __attribute__((init_priority(200))) (
    RemotableObjectTypes,
    sizeof(RemotableObjectTypes) / sizeof(RemotableObjectTypes[0])
    );

/*++
Function:
  CSharedMemoryObjectManager::ConvertRemoteHandleToLocal

  Given a "remote handle" creates a local handle that refers
  to the desired object. (Unlike PAL_RemoteHandleToLocal this method
  needs to access internal object manager state, so it's a member function.)

Parameters:
  pthr -- thread data for calling thread
  rhRemote -- the remote handle
  phLocal -- on success, receives the local handle
--*/

/*++
Function:
  CheckObjectTypeAndRights

  Helper routine that determines if:
  1) An object instance is of a specified type
  2) A set of granted access rights satisfies the required access rights
     (currently ignored)

Parameters:
  pobj -- the object instance whose type is to be checked
  paot -- the acceptable type for the object instance
  dwRightsGranted -- the granted access rights (ignored)
  dwRightsRequired -- the required access rights (ignored)
--*/

static
PAL_ERROR
CheckObjectTypeAndRights(
    IPalObject *pobj,
    CAllowedObjectTypes *paot
)
{
    PAL_ERROR palError = NO_ERROR;

    assert(NULL != pobj);
    assert(NULL != paot);

    if (paot->IsTypeAllowed(pobj->GetObjectType()->GetId()))
    {
#ifdef ENFORCE_OBJECT_ACCESS_RIGHTS

        //
        // This is where the access right check would occur if Win32 object
        // security were supported.
        //
        
        if ((dwRightsRequired & dwRightsGranted) != dwRightsRequired)
        {
            palError = ERROR_ACCESS_DENIED;
        }
#endif
    }
    else
    {
        palError = ERROR_INVALID_HANDLE;
    }

    LOGEXIT("CheckObjectTypeAndRights returns %d\n", palError);

    return palError;
}
    

