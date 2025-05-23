//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DataStructures/BitVector.h"

namespace Memory
{

#define PreReservedHeapTrace(...) \
{ \
    OUTPUT_TRACE(Js::PreReservedHeapAllocPhase, __VA_ARGS__); \
}


class SectionAllocWrapper;
class PreReservedSectionAllocWrapper;

/*
* VirtualAllocWrapper is just a delegator class to call VirtualAlloc and VirtualFree.
*/
class VirtualAllocWrapper
{
public:
    void *  AllocPages(void * lpAddress, size_t pageCount, uint32_t allocationType, uint32_t protectFlags, bool isCustomHeapAllocation);
    BOOL    Free(void * lpAddress, size_t dwSize, uint32_t dwFreeType);
    void *  AllocLocal(void * lpAddress, size_t dwSize) { return lpAddress; }
    BOOL    FreeLocal(void * lpAddress) { return true; }
    bool    GetFileInfo(void * address, HANDLE* fileHandle, void ** baseAddress) { return true; }

    static VirtualAllocWrapper Instance;  // single instance
private:
    VirtualAllocWrapper() {}
};

#if ENABLE_NATIVE_CODEGEN
/*
* PreReservedVirtualAllocWrapper class takes care of Reserving a large memory region initially
* and then committing mem regions for the size requested.
* Committed pages are being tracked with a bitVector.
* Committing memory outside of the preReserved Memory region is not handled by this allocator
*/

class PreReservedVirtualAllocWrapper
{
public:
#if TARGET_32
    static const uint PreReservedAllocationSegmentCount = 256; // (256 * 64K) == 16 MB, if 64k is the AllocationGranularity
#else // TARGET_64
    static const uint PreReservedAllocationSegmentCount = 4096; //(4096 * 64K) == 256MB, if 64k is the AllocationGranularity
#endif

public:
    PreReservedVirtualAllocWrapper();
    ~PreReservedVirtualAllocWrapper();
    void *      AllocPages(void * lpAddress, size_t pageCount, uint32_t allocationType, uint32_t protectFlags, bool isCustomHeapAllocation);
    BOOL        Free(void * lpAddress,  size_t dwSize, uint32_t dwFreeType);
    void *  AllocLocal(void * lpAddress, size_t dwSize) { return lpAddress; }
    BOOL    FreeLocal(void * lpAddress) { return true; }
    bool    GetFileInfo(void * address, HANDLE* fileHandle, void ** baseAddress) { return true; }

    bool        IsInRange(void * address);
    static bool IsInRange(void * regionStart, void * address);
    void *      EnsurePreReservedRegion();

    void *      GetPreReservedEndAddress();
    static void * GetPreReservedEndAddress(void * regionStart);

#if DBG_DUMP || defined(ENABLE_IR_VIEWER)
    bool        IsPreReservedEndAddress(void * address)
    {
        return IsPreReservedRegionPresent() && address == GetPreReservedEndAddress();
    }
#endif
private:
    void *      EnsurePreReservedRegionInternal();
    bool        IsPreReservedRegionPresent();
    void *      GetPreReservedStartAddress();

    BVStatic<PreReservedAllocationSegmentCount>     freeSegments;
    void *                                          preReservedStartAddress;
    CriticalSection                                 cs;
};

#endif

#if defined(ENABLE_JIT_CLAMP)

class AutoEnableDynamicCodeGen
{
public:
    AutoEnableDynamicCodeGen(bool enable = true);
    ~AutoEnableDynamicCodeGen();

private:
    bool enabled;

    typedef
    BOOL
    (WINAPI *PSET_THREAD_INFORMATION_PROC)(
        _In_ HANDLE                   hThread,
        _In_ THREAD_INFORMATION_CLASS ThreadInformationClass,
        _In_reads_bytes_(ThreadInformationSize) void * ThreadInformation,
        _In_ uint32_t                    ThreadInformationSize
    );

    typedef
    BOOL
    (WINAPI *PGET_THREAD_INFORMATION_PROC)(
        _In_ HANDLE                   hThread,
        _In_ THREAD_INFORMATION_CLASS ThreadInformationClass,
        _Out_writes_bytes_(ThreadInformationSize) void * ThreadInformation,
        _In_ uint32_t                    ThreadInformationSize
    );

    static PSET_THREAD_INFORMATION_PROC SetThreadInformationProc;
    static PGET_THREAD_INFORMATION_PROC GetThreadInformationProc;
    static PROCESS_MITIGATION_DYNAMIC_CODE_POLICY processPolicy;
    static CriticalSection processPolicyCS;
    static volatile bool processPolicyObtained;
};
#endif

template<typename TVirtualAlloc = VirtualAllocWrapper>
class PageSegmentBase;

template<typename TVirtualAlloc = VirtualAllocWrapper>
class SegmentBase;

template<typename TVirtualAlloc = VirtualAllocWrapper, typename TSegment = SegmentBase<TVirtualAlloc>, typename TPageSegment = PageSegmentBase<TVirtualAlloc>>
class PageAllocatorBase;

typedef PageAllocatorBase<> PageAllocator;
typedef PageSegmentBase<> PageSegment;
typedef SegmentBase<> Segment;
}
