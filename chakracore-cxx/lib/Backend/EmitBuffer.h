//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Memory/CustomHeap.h"

//---------------------------------------------------------------------------------
// One allocation chunk from CustomHeap + PData if needed, tracked as a linked list
//---------------------------------------------------------------------------------
template <typename TAlloc, typename TPreReservedAlloc>
struct EmitBufferAllocation
{
    CustomHeap::Allocation * allocation;
    size_t bytesUsed;
    size_t bytesCommitted;
    bool   inPrereservedRegion;
    bool   recorded;
    EmitBufferAllocation<TAlloc, TPreReservedAlloc> * nextAllocation;

    uint8_t * GetUnused() const            { return (uint8_t*) allocation->address + bytesUsed; }
    uint8_t * GetUncommitted() const       { return (uint8_t*) allocation->address + bytesCommitted; }
    size_t GetBytesUsed() const       { return bytesUsed; }

    // Truncation to uint32_t okay here
    uint32_t BytesFree() const    { return static_cast<uint32_t>(this->bytesCommitted - this->bytesUsed); }
};
typedef void* NativeMethod;

//----------------------------------------------------------------------------
// Emit buffer manager - manages allocation chunks from VirtualAlloc
//----------------------------------------------------------------------------
template <typename TAlloc, typename TPreReservedAlloc, class SyncObject = FakeCriticalSection>
class EmitBufferManager
{
    typedef EmitBufferAllocation<TAlloc, TPreReservedAlloc> TEmitBufferAllocation;
public:
    EmitBufferManager(ArenaAllocator * allocator, CustomHeap::CodePageAllocators<TAlloc, TPreReservedAlloc> * codePageAllocators, Js::ScriptContext * scriptContext, ThreadContextInfo * threadContext, LPCWSTR name, HANDLE processHandle);
    ~EmitBufferManager();

    // All the following methods are guarded with the SyncObject
    void Decommit();
    void Clear();

    TEmitBufferAllocation* AllocateBuffer(__in size_t bytes, __deref_bcount(bytes) uint8_t** ppBuffer, ushort pdataCount = 0, ushort xdataSize = 0, bool canAllocInPreReservedHeapPageSegment = false, bool isAnyJittedCode = false);
    bool CommitBuffer(TEmitBufferAllocation* allocation, __in const size_t destBufferBytes, __out_bcount(destBufferBytes) uint8_t* destBuffer, __in size_t bytes, __in_bcount(bytes) const uint8_t* sourceBuffer, __in uint32_t alignPad = 0);
    bool ProtectBufferWithExecuteReadWriteForInterpreter(TEmitBufferAllocation* allocation);
    bool CommitBufferForInterpreter(TEmitBufferAllocation* allocation, _In_reads_bytes_(bufferSize) uint8_t* pBuffer, _In_ size_t bufferSize);
    void CompletePreviousAllocation(TEmitBufferAllocation* allocation);
    bool FreeAllocation(void* address);
    void SetValidCallTarget(TEmitBufferAllocation* allocation, void* callTarget, bool isValid);
    //Ends here

    bool IsInHeap(void* address);

#if DBG_DUMP
    void DumpAndResetStats(char16 const * source);
#endif

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    void CheckBufferPermissions(TEmitBufferAllocation *allocation);
#endif

#if DBG
    bool IsBufferExecuteReadOnly(TEmitBufferAllocation * allocation);
#endif

    TEmitBufferAllocation * allocations;

private:
    void FreeAllocations(bool release);

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    bool CheckCommitFaultInjection();

    int commitCount;
#endif
    ArenaAllocator * allocator;
    Js::ScriptContext * scriptContext;
    ThreadContextInfo * threadContext;

    TEmitBufferAllocation * NewAllocation(size_t bytes, ushort pdataCount, ushort xdataSize, bool canAllocInPreReservedHeapPageSegment, bool isAnyJittedCode);
    TEmitBufferAllocation* GetBuffer(TEmitBufferAllocation *allocation, __in size_t bytes, __deref_bcount(bytes) uint8_t** ppBuffer);

    bool FinalizeAllocation(TEmitBufferAllocation *allocation, uint8_t* dstBuffer);
    CustomHeap::Heap<TAlloc, TPreReservedAlloc> allocationHeap;

    SyncObject  criticalSection;
    HANDLE processHandle;
#if DBG_DUMP

public:
    LPCWSTR name;
    size_t totalBytesCode;
    size_t totalBytesLoopBody;
    size_t totalBytesAlignment;
    size_t totalBytesCommitted;
    size_t totalBytesReserved;
#endif
};

typedef EmitBufferManager<VirtualAllocWrapper, PreReservedVirtualAllocWrapper, CriticalSection> InProcEmitBufferManagerWithlock;
typedef EmitBufferManager<VirtualAllocWrapper, PreReservedVirtualAllocWrapper, FakeCriticalSection> InProcEmitBufferManager;
#if ENABLE_OOP_NATIVE_CODEGEN
typedef EmitBufferManager<SectionAllocWrapper, PreReservedSectionAllocWrapper, CriticalSection> OOPEmitBufferManagerWithLock;
typedef EmitBufferManager<SectionAllocWrapper, PreReservedSectionAllocWrapper, FakeCriticalSection> OOPEmitBufferManager;
#endif
