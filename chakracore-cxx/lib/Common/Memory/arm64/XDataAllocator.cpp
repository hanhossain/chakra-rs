//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"

// ARM64 TODO: this is nearly identical to AMD64, consider merging in the future

// This one works only for ARM64
#if !defined(_M_ARM64)
CompileAssert(false)
#endif

#include "XDataAllocator.h"
#include "Core/DelayLoadLibrary.h"

#include "PlatformAgnostic/AssemblyCommon.h" // __REGISTER_FRAME / __DEREGISTER_FRAME

XDataAllocator::XDataAllocator(uint8_t* address, uint size) :
    freeList(nullptr),
    start(address),
    current(address),
    size(size)
{
    Assert(size > 0);
    Assert(address != nullptr);
}

bool XDataAllocator::Initialize(void* segmentStart, void* segmentEnd)
{
    Assert(segmentEnd > segmentStart);
    return true;
}

XDataAllocator::~XDataAllocator()
{
    current = nullptr;
    ClearFreeList();
}

void XDataAllocator::Delete()
{
    HeapDelete(this);
}

bool XDataAllocator::Alloc(size_t functionStart, uint32_t functionSize,
    ushort pdataCount, ushort xdataSize, SecondaryAllocation* allocation)
{
    XDataAllocation* xdata = static_cast<XDataAllocation*>(allocation);
    Assert(start != nullptr);
    Assert(current != nullptr);
    Assert(current >= start);
    Assert(xdataSize <= XDATA_SIZE);
    Assert(pdataCount == 1);

    // Allocate a new xdata entry
    if((End() - current) >= XDATA_SIZE)
    {
        xdata->address = current;
        current += XDATA_SIZE;
    } // try allocating from the free list
    else if(freeList)
    {
        auto entry = freeList;
        xdata->address = entry->address;
        this->freeList = entry->next;
        HeapDelete(entry);
    }
    else
    {
        xdata->address = nullptr;
        OUTPUT_TRACE(Js::XDataAllocatorPhase, _u("No space for XDATA.\n"));
    }

    if (xdata->address)
    {
        ClearHead(xdata->address);  // mark empty .eh_frame
    }

    return xdata->address != nullptr;
}

void XDataAllocator::Release(const SecondaryAllocation& allocation)
{
    const XDataAllocation& xdata = static_cast<const XDataAllocation&>(allocation);
    Assert(allocation.address);
    // Add it to free list
    auto freed = HeapNewNoThrowStruct(XDataAllocationEntry);
    if(freed)
    {
        freed->address = xdata.address;
        freed->next = this->freeList;
        this->freeList = freed;
    }
}

bool XDataAllocator::CanAllocate()
{
    return ((End() - current) >= XDATA_SIZE) || this->freeList;
}

void XDataAllocator::ClearFreeList()
{
    XDataAllocationEntry* next = this->freeList;
    XDataAllocationEntry* entry;
    while(next)
    {
        entry = next;
        next = entry->next;
        entry->address = nullptr;
        HeapDelete(entry);
    }
    this->freeList = NULL;
}

/* static */
void XDataAllocator::Register(XDataAllocation * xdataInfo, size_t functionStart, uint32_t functionSize)
{
    Assert(ReadHead(xdataInfo->address));  // should be non-empty .eh_frame
    __REGISTER_FRAME(xdataInfo->address);
}

/* static */
void XDataAllocator::Unregister(XDataAllocation * xdataInfo)
{
    Assert(ReadHead(xdataInfo->address));  // should be non-empty .eh_frame
    __DEREGISTER_FRAME(xdataInfo->address);
}
