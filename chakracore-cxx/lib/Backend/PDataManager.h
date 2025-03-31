//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if PDATA_ENABLED

class PDataManager
{
public:
    static void RegisterPdata(RUNTIME_FUNCTION* pdataStart, _In_ const size_t functionStart, _In_ const size_t functionEnd, _Out_ void ** pdataTable, uint32_t entryCount = 1, uint32_t maxEntryCount = 1);
    static void UnregisterPdata(RUNTIME_FUNCTION* pdata);
};

#endif
