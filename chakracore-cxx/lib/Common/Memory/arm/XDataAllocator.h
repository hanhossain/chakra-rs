//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#if !defined(_M_ARM)
CompileAssert(false)
#endif

struct XDataAllocation sealed  : public SecondaryAllocation
{
    // ---- Methods ----- //
    XDataAllocation() :
        pdataCount(0)
        , functionTable(NULL)
        , xdataSize(0)
    {}

    RUNTIME_FUNCTION* GetPdataArray() const
    {
        return reinterpret_cast<RUNTIME_FUNCTION*>(address + xdataSize);
    }

    bool IsFreed() const
    {
        return address == nullptr;
    }

    void Free()
    {
        address = nullptr;
        pdataCount = 0;
        functionTable = nullptr;
        xdataSize = 0;
    }

    // ---- Data members ---- //
    ushort pdataCount;                   // ARM requires more than 1 pdata/function
    FunctionTableHandle functionTable;   // stores the handle to the growable function table
    ushort xdataSize;
};

//
// Allocates xdata and pdata entries for ARM architecture on the heap. They are freed when released.
//
//
class XDataAllocator sealed : public SecondaryAllocator
{
// --------- Public functions ---------/
public:
    XDataAllocator(uint8_t* address, uint size);

    bool Initialize(void* segmentStart, void* segmentEnd);
    void Delete();
    bool Alloc(size_t functionStart, uint32_t functionSize, ushort pdataCount, ushort xdataSize, SecondaryAllocation* allocation);
    void Release(const SecondaryAllocation& address);
    bool CanAllocate();
    static uint32_t GetAllocSize(ushort pdataCount, ushort xdataSize)
    {
        return sizeof(RUNTIME_FUNCTION) * pdataCount + xdataSize;
    }

    static void Register(XDataAllocation * xdataInfo, uint32_t functionStart, uint32_t functionSize);
    static void Unregister(XDataAllocation * xdataInfo);
    // Read .eh_frame data head (length record). 0 means empty.
    static uint32 ReadHead(const void* p)
    {
        return *reinterpret_cast<const uint32*>(p);
    }
};
