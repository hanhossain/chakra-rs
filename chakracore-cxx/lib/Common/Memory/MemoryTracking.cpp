//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"
#include "Memory/MemoryTracking.h"

// These are empty stubs here but DLL can supply an .OBJ with an implementation

void ArenaMemoryTracking::Activate()
{
    // Called to activate arena memory tracking
}

// ArenaMemoryTracking stubs
void ArenaMemoryTracking::ArenaCreated(Allocator *arena,  LPCWSTR name)
{
    // Called when arena is created.
}

void ArenaMemoryTracking::ArenaDestroyed(Allocator *arena)
{
    // Called when arena is destroyed
}

void ArenaMemoryTracking::ReportAllocation(Allocator *arena, void *address, size_t size)
{
    // Called when size bytes at address are allocated
}

void ArenaMemoryTracking::ReportReallocation(Allocator *arena, void *address, size_t existingSize, size_t newSize)
{
    // Called when a reallocation where newSize < existingSize.

    // This will only be called if newSize < existingSize.

    // This is to inform a tracking that a realloc is taking place and ReportFree() will be called on address + newSize soon
    // and the ReportFree for address will report newSize instead of existing size
}

void ArenaMemoryTracking::ReportFree(Allocator *arena, void *address, size_t size)
{
    // Called when the when size bytes at address are freed. address was either reported by ReportAllocation() or as a
    // result of a buffer being split reported by ReportReallocation().

    // IMPORTANT: ReportFree() will always be called after ReportReallocation() to report the newly free memory of address + newSize.
}

void ArenaMemoryTracking::ReportFreeAll(Allocator *arena)
{
    // Called when all the arena memory currently allocated is bulk freed.
}

void RecyclerMemoryTracking::Activate()
{
    // Called to active recycler memory tracking
}

// RecyclerMemoryTracking stubs
bool RecyclerMemoryTracking::IsActive()
{
    // Should return when tracking is active. This is used to force ReportFree() calls. Without this ReportFree() is only called on
    // finalizable memory which is only part of the memory allocated in the recycler.

    return false;
}

void RecyclerMemoryTracking::ReportRecyclerCreate(Recycler * recycler)
{
    // Called when a recycler is created.
}

void RecyclerMemoryTracking::ReportRecyclerDestroy(Recycler * recycler)
{
    // Called when a recycler is freed.
}

void RecyclerMemoryTracking::ReportAllocation(Recycler * recycler, void *address, size_t size)
{
    // Called when size bytes at address are allocated from the recycler.
}

void RecyclerMemoryTracking::ReportFree(Recycler * recycler, void *address, size_t size)
{
    // Called when size bytes at address are freed.
}

void RecyclerMemoryTracking::ReportUnallocated(Recycler * recycler, void* address, void *endAddress, size_t sizeCat)
{
    // Even though the memory is not really allocated between address and endAddress,
    // the recycler initially treats it as allocated and a ReportFree() will be called on it even
    // though ReportAllocation() is never called. This can be treated as equivalent of the parent
    // requesting the following be performed:
    //
    //  while (address + sizeCat <= endAddress)
    //  {
    //    ReportFree(address, sizeCat);
    //    address += sizeCat;
    //  }
    //
    // if address where a (char *)
}

// PageTracking stubs
void PageTracking::Activate()
{
    // Called to activate page allocator tracking
}

void PageTracking::PageAllocatorCreated(PageAllocator *pageAllocator)
{
    // Called when a page allocator is created.
}

void PageTracking::PageAllocatorDestroyed(PageAllocator *pageAllocator)
{
    // Called when a page allocator is destroyed.
}

void PageTracking::ReportAllocation(PageAllocator *pageAllocator, void *address, size_t size)
{
    // Called when size bytes are allocated at address.
}

void PageTracking::ReportFree(PageAllocator *pageAllocator, void *address, size_t size)
{
    // Called when size bytes are freed at address.
}

