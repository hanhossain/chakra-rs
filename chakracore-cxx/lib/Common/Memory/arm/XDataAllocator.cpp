//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// This one works only for ARM
#include "CommonMemoryPch.h"
#if !defined(_M_ARM)
CompileAssert(false)
#endif

#include "XDataAllocator.h"
#include "Core/DelayLoadLibrary.h"

#include "PlatformAgnostic/AssemblyCommon.h" // __REGISTER_FRAME / __DEREGISTER_FRAME

XDataAllocator::XDataAllocator(BYTE* address, uint size)
{
    Assert(size == 0);
}


void XDataAllocator::Delete()
{
    HeapDelete(this);
}

bool XDataAllocator::Initialize(void* segmentStart, void* segmentEnd)
{
    return true;
}

bool XDataAllocator::Alloc(size_t functionStart, DWORD functionSize, ushort pdataCount, ushort xdataSize, SecondaryAllocation* allocation)
{
    XDataAllocation* xdata = static_cast<XDataAllocation*>(allocation);
    Assert(pdataCount > 0);
    Assert(xdataSize >= 0);
    Assert(xdata);

    DWORD size = GetAllocSize(pdataCount, xdataSize);
    BYTE* alloc = HeapNewNoThrowArray(BYTE, size);
    if (alloc != nullptr)
    {
        xdata->address = alloc;
        xdata->xdataSize = xdataSize;
        xdata->pdataCount = pdataCount;

        return true; //success
    }
    return false; //fail;
}


void XDataAllocator::Release(const SecondaryAllocation& allocation)
{
    const XDataAllocation& xdata = static_cast<const XDataAllocation&>(allocation);
    if(xdata.address  != nullptr)
    {
        HeapDeleteArray(GetAllocSize(xdata.pdataCount, xdata.xdataSize), xdata.address);
    }
}

/* static */
void XDataAllocator::Register(XDataAllocation * xdataInfo, DWORD functionStart, DWORD functionSize)
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

bool XDataAllocator::CanAllocate()
{
    return true;
}
