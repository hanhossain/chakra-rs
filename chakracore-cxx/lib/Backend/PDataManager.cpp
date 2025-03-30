//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Backend.h"

// Conditionally-compiled on x64 and arm/arm64
#if PDATA_ENABLED
// ----------------------------------------------------------------------------
//  non-windows x64 unwind uses .eh_frame
// ----------------------------------------------------------------------------

void PDataManager::RegisterPdata(RUNTIME_FUNCTION* pdataStart,
    _In_ const size_t functionStart, _In_ const size_t functionEnd,
    _Out_ void ** pdataTable, uint32_t entryCount, uint32_t maxEntryCount)
{
    __REGISTER_FRAME(pdataStart);
    *pdataTable = pdataStart;
}

void PDataManager::UnregisterPdata(RUNTIME_FUNCTION* pdata)
{
    __DEREGISTER_FRAME(pdata);
}

#endif  // PDATA_ENABLED
