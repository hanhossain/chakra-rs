//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    shmobjectmanager.hpp

Abstract:
    Shared memory based object manager



--*/

#ifndef _PAL_SHMOBJECTMANAGER_HPP_
#define _PAL_SHMOBJECTMANAGER_HPP_

#include "pal/corunix.hpp"
#include "pal/handlemgr.hpp"
#include "pal/list.h"
#include "shmobject.hpp"

namespace CorUnix
{
    class CSharedMemoryObjectManager : public IPalObjectManager
    {
    protected:
        CRITICAL_SECTION m_csListLock;
        bool m_fListLockInitialized;
        LIST_ENTRY m_leNamedObjects;
        LIST_ENTRY m_leAnonymousObjects;

        CSimpleHandleManager m_HandleManager;

        PAL_ERROR
        ImportSharedObjectIntoProcess(CPalThread *pthr, CObjectType *pot, CObjectAttributes *poa,
                                      SHMPTR shmSharedObjectData, SHMObjData *psmod, bool fAddRefSharedData,
                                      CSharedMemoryObject **ppshmobj);

    public:
        CSharedMemoryObjectManager() : m_fListLockInitialized(FALSE) {};

        virtual ~CSharedMemoryObjectManager() {};

        PAL_ERROR
        Initialize(void);

        PAL_ERROR
        Shutdown(CPalThread *pthr);

        PAL_ERROR
        ConvertRemoteHandleToLocal(CPalThread *pthr, RHANDLE rhRemote, HANDLE *phLocal);

        //
        // IPalObjectManager routines
        //

        PAL_ERROR
        AllocateObject(CPalThread *pthr, CObjectType *pot, CObjectAttributes *poa, IPalObject **ppobjNew) override;

        PAL_ERROR
        RegisterObject(CPalThread *pthr, IPalObject *pobjToRegister, CAllowedObjectTypes *paot,
                       uint32_t dwRightsRequested, HANDLE *pHandle, IPalObject **ppobjRegistered) override;

        PAL_ERROR
        LocateObject(CPalThread *pthr, CPalString *psObjectToLocate, CAllowedObjectTypes *paot,
                     IPalObject **ppobj) override;

        PAL_ERROR
        ObtainHandleForObject(CPalThread *pthr, IPalObject *pobj, uint32_t dwRightsRequested, bool fInheritHandle,
                              IPalProcess *pProcessForHandle, // IN, OPTIONAL
                              HANDLE *pNewHandle) override;

        PAL_ERROR
        RevokeHandle(CPalThread *pthr, HANDLE hHandleToRevoke) override;

        PAL_ERROR
        ReferenceObjectByHandle(CPalThread *pthr, HANDLE hHandleToReference, CAllowedObjectTypes *paot,
                                IPalObject **ppobj) override;

        PAL_ERROR
        ReferenceMultipleObjectsByHandleArray(CPalThread *pthr, HANDLE rghHandlesToReference[], uint32_t dwHandleCount,
                                              CAllowedObjectTypes *paot, IPalObject *rgpobjs[]) override;
    };
} // namespace CorUnix

#endif // _PAL_SHMOBJECTMANAGER_HPP_
