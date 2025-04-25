//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#include "Interface/CommonDefines.h"
#include "CollectionFlags.h"
#include "RecyclerWaitReason.h"
#include "Common/Tick.h"
#include "AllocatorTelemetryStats.h"
#include "HeapBucketStats.h"
#include "DataStructures/SList.h"

namespace Memory
{
    class Recycler;
    class IdleDecommitPageAllocator;
    class RecyclerTelemetryInfo;

    // struct that defines an interface with host on how we can get key data stats
    class RecyclerTelemetryHostInterface
    {
    public:
        virtual LPFILETIME GetLastScriptExecutionEndTime() const = 0;
        virtual bool TransmitGCTelemetryStats(RecyclerTelemetryInfo& rti) = 0;
        virtual bool TransmitTelemetryError(const RecyclerTelemetryInfo& rti, const char* msg) = 0;
        virtual bool TransmitHeapUsage(size_t totalHeapBytes, size_t usedHeapBytes, double heapUsedRatio) = 0;
        virtual bool IsTelemetryProviderEnabled() const = 0;
        virtual bool IsThreadBound() const = 0;
        virtual uint32_t GetCurrentScriptThreadID() const = 0;
        virtual uint GetClosedContextCount() const = 0;
    };

    class RecyclerTelemetryInfo
    {
    };

}

