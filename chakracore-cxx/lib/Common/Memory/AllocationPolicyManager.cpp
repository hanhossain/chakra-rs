#include "AllocationPolicyManager.h"

AllocationPolicyManager::AllocationPolicyManager(bool needConcurrencySupport)
    : memoryLimit(static_cast<size_t>(-1)), currentMemory(0), supportConcurrency(needConcurrencySupport),
      context(nullptr), memoryAllocationCallback(nullptr) {}
AllocationPolicyManager::~AllocationPolicyManager() { assert(currentMemory == 0); }
std::unique_ptr<AllocationPolicyManager> AllocationPolicyManager::FfiNew(bool needConcurrencySupport) {
    return std::make_unique<AllocationPolicyManager>(needConcurrencySupport);
}
size_t AllocationPolicyManager::GetUsage() { return currentMemory; }
size_t AllocationPolicyManager::GetLimit() { return memoryLimit; }
void AllocationPolicyManager::SetLimit(size_t newLimit) { memoryLimit = newLimit; }
bool AllocationPolicyManager::RequestAlloc(size_t byteCount, bool externalAlloc) {
    if (supportConcurrency) {
        std::unique_lock auto_cs(cs);
        return RequestAllocImpl(byteCount, externalAlloc);
    } else {
        return RequestAllocImpl(byteCount, externalAlloc);
    }
}
void AllocationPolicyManager::ReportFailure(size_t byteCount) {
    if (supportConcurrency) {
        std::unique_lock auto_cs(cs);
        ReportFreeImpl(MemoryAllocateEvent::MemoryFailure, byteCount);
    } else {
        ReportFreeImpl(MemoryAllocateEvent::MemoryFailure, byteCount);
    }
}
void AllocationPolicyManager::ReportFree(size_t byteCount) {
    if (supportConcurrency) {
        std::unique_lock auto_cs(cs);
        ReportFreeImpl(MemoryAllocateEvent::MemoryFree, byteCount);
    } else {
        ReportFreeImpl(MemoryAllocateEvent::MemoryFree, byteCount);
    }
}
void AllocationPolicyManager::SetMemoryAllocationCallback(void *newContext,
                                                          PageAllocatorMemoryAllocationCallback callback) {
    this->memoryAllocationCallback = callback;

    if (callback == nullptr) {
        // doesn't make sense to have non-null context when the callback is nullptr.
        this->context = nullptr;
    } else {
        this->context = newContext;
    }
}
bool AllocationPolicyManager::RequestAllocImpl(size_t byteCount, bool externalAlloc) {
    size_t newCurrentMemory = currentMemory + byteCount;

    if (newCurrentMemory < currentMemory || newCurrentMemory > memoryLimit ||
        (memoryAllocationCallback != nullptr &&
         !memoryAllocationCallback(context, MemoryAllocateEvent::MemoryAllocate, byteCount))) {
        // oopjit number allocator allocated pages, we can't stop it from allocating so just increase the usage number
        if (externalAlloc) {
            currentMemory = newCurrentMemory;
            return true;
        }

        if (memoryAllocationCallback != nullptr) {
            memoryAllocationCallback(context, MemoryAllocateEvent::MemoryFailure, byteCount);
        }

        return false;
    } else {
        currentMemory = newCurrentMemory;
        return true;
    }
}
void AllocationPolicyManager::ReportFreeImpl(MemoryAllocateEvent allocationEvent, size_t byteCount) {
    assert(currentMemory >= byteCount);
    byteCount = std::min(byteCount, currentMemory);

    currentMemory = currentMemory - byteCount;

    if (memoryAllocationCallback != nullptr) {
        // The callback should be minimal, with no possibility of calling back to us.
        // Note that this can be called both in script or out of script.
        memoryAllocationCallback(context, allocationEvent, byteCount);
    }
}
