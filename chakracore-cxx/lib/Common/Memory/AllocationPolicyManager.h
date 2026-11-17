//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//
// AllocationPolicyManager allows a caller/host to disallow new page allocations
// and to track current page usage.
//
#pragma once
#include <cstddef>
#include <memory>
#include <mutex>

// NOTE: For now, we are only tracking reserved page count.
// Consider whether we should also (or maybe only) track committed page count.

class AllocationPolicyManager {
public:
    enum MemoryAllocateEvent {
        MemoryAllocate = 0,
        MemoryFree = 1,
        MemoryFailure = 2,
        MemoryMax = 2,
    };
    typedef bool (*PageAllocatorMemoryAllocationCallback)(void *context,
                                                          AllocationPolicyManager::MemoryAllocateEvent allocationEvent,
                                                          std::size_t allocationSize);

private:
    size_t memoryLimit;
    size_t currentMemory;
    bool supportConcurrency;
    std::recursive_mutex cs;
    void *context;
    PageAllocatorMemoryAllocationCallback memoryAllocationCallback;

public:
    AllocationPolicyManager(bool needConcurrencySupport);

    ~AllocationPolicyManager();
    static std::unique_ptr<AllocationPolicyManager> FfiNew(bool needConcurrencySupport);

    size_t GetUsage();

    size_t GetLimit();

    void SetLimit(size_t newLimit);

    bool RequestAlloc(size_t byteCount, bool externalAlloc = false);

    void ReportFailure(size_t byteCount);

    void ReportFree(size_t byteCount);

    void SetMemoryAllocationCallback(void *newContext, PageAllocatorMemoryAllocationCallback callback);

private:
    bool RequestAllocImpl(size_t byteCount, bool externalAlloc = false);

    void ReportFreeImpl(MemoryAllocateEvent allocationEvent, size_t byteCount);
};
