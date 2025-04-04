//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#if !defined(_M_X64)
CompileAssert(false)
#endif
#pragma once

namespace Memory
{
#define XDATA_SIZE (0x80)

struct XDataAllocation : public SecondaryAllocation
{
    XDataAllocation()
    {}

    bool IsFreed() const
    {
        return address == nullptr;
    }
    void Free()
    {
        address = nullptr;
    }
};

//
// Allocates xdata and pdata entries for x64 architecture.
//
// xdata
// ------
// x64 architecture requires the xdata to be within 32-bit address range of the jitted code itself
// Hence, for every page segment we have an instance of the xdata allocator that allocates
// xdata entries in some specified non-executable region at the end of the page segment.
//
// pdata
// -------
// XDataAllocator also manages the pdata entries for a the page segment range. It allocates the table of pdata entries
// on the heap to do that.
//
class XDataAllocator sealed : public SecondaryAllocator
{
// -------- Private members ---------/
private:
    struct XDataAllocationEntry : XDataAllocation
    {
        XDataAllocationEntry* next;
    };
    uint8_t* start;
    uint8_t* current;
    uint  size;

    XDataAllocationEntry* freeList;

// --------- Public functions ---------/
public:
    XDataAllocator(uint8_t* address, uint size);
    virtual ~XDataAllocator();

    bool Initialize(void* segmentStart, void* segmentEnd);
    void Delete();
    bool Alloc(size_t functionStart, uint32_t functionSize, ushort pdataCount, ushort xdataSize, SecondaryAllocation* allocation);
    void Release(const SecondaryAllocation& address);
    bool CanAllocate();

    static void Register(XDataAllocation * xdataInfo, size_t functionStart, uint32_t functionSize);
    static void Unregister(XDataAllocation * xdataInfo);

// -------- Private helpers ---------/
private:
    uint8_t* End() { return start + size; }

    // Read .eh_frame data head (length record). 0 means empty.
    static uint32 ReadHead(const void* p)
    {
        return *reinterpret_cast<const uint32*>(p);
    }

    // Clear .eh_frame data head (length record). Set to 0 to mark empty.
    static void ClearHead(void* p)
    {
        *reinterpret_cast<uint32*>(p) = 0;
    }

    void ClearFreeList();
};
}
