//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Common/vtregistry.h"

// static
template <typename T>
inline long VirtualTableInfo<T>::RegisterVirtualTable()
{
    long vtable = *(long const*)&T(VirtualTableInfoCtorValue);

#if DBG
#if ENABLE_VALIDATE_VTABLE_CTOR
    //printf("m_vtableMapHash->Add(VirtualTableInfo<%s>::Address, dummy);\n", typeid(T).name());
#endif
    if (T::RegisterVTable)
    {
        VirtualTableRegistry::Add(vtable, typeid(T).name());
    }
#endif

    return vtable;
}

template <typename T>
inline void VirtualTableInfo<T>::SetVirtualTable(void * ptr)
{
    VirtualTableInfoBase::SetVirtualTable(ptr, Address);
}
