//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if ENABLE_NATIVE_CODEGEN
class ThunkBlock
{
private:
#if PDATA_ENABLED
    void* registeredPdataTable;
#endif
    uint8_t*    start;
    BVFixed* freeList;
    uint32_t    thunkCount;

public:
    ThunkBlock(uint8_t* start, uint32_t thunkCount) :
        start(start),
        thunkCount(thunkCount),
        freeList(NULL)
#if PDATA_ENABLED
        , registeredPdataTable(NULL)
#endif
    {}

    bool Contains(uint8_t* address) const;
    void Release(uint8_t* address);
    bool EnsureFreeList(ArenaAllocator* allocator);
    uint8_t* AllocateFromFreeList();
    bool IsFreeListEmpty() const;
    uint8_t* GetStart() const { return start; }

#if PDATA_ENABLED
    void* GetPdata() const { return registeredPdataTable; }
    void SetPdata(void* pdata) { Assert(!this->registeredPdataTable); this->registeredPdataTable = pdata; }
#endif

private:
    BVIndex FromThunkAddress(uint8_t* address);
    uint8_t* ToThunkAddress(BVIndex index);
};

//
// Emits interpreter thunks such that each javascript function in the interpreter gets a unique return address on call.
// This unique address can be used to identify the function when a stackwalk is
// performed using ETW.
//
// On every call, we generate 1 thunk (size ~ 1 page) that is implemented as a jump table.
// The shape of the thunk is as follows:
//         1. Function prolog
//         2. Determine the thunk number from the JavascriptFunction object passed as first argument.
//         3. Calculate the address of the correct calling point and jump to it.
//         4. Make the call and jump to the epilog.
//         5. Function epilog.
//
class InterpreterThunkEmitter
{
public:
    // InterpreterThunkSize must be public to be accessible to all code in InterpreterThunkEmitter.cpp.
#ifdef _M_X64
    static constexpr size_t InterpreterThunkSize = 72;
#elif defined(_M_ARM)
    static constexpr size_t InterpreterThunkSize = 72;
#elif defined(_M_ARM64)
    static constexpr size_t InterpreterThunkSize = 48;
#else
    static constexpr size_t InterpreterThunkSize = 56;
#endif

private:
    /* ------- instance methods --------*/
    InProcEmitBufferManager emitBufferManager;
    SListBase<ThunkBlock> thunkBlocks;
    SListBase<ThunkBlock> freeListedThunkBlocks;
    bool isAsmInterpreterThunk; // To emit address of InterpreterAsmThunk or InterpreterThunk
    uint8_t*                thunkBuffer;
    ArenaAllocator*      allocator;
    Js::ScriptContext *  scriptContext;
    uint32_t thunkCount;                      // Count of thunks available in the current thunk block

    static constexpr uint8_t PageCount = 1;

    /* ------private helpers -----------*/
    bool NewThunkBlock();

#ifdef ENABLE_OOP_NATIVE_CODEGEN
    bool NewOOPJITThunkBlock();
#endif

    static void EncodeInterpreterThunk(
        __in_bcount(InterpreterThunkSize) uint8_t* thunkBuffer,
        const intptr_t thunkBufferStartAddress,
        const intptr_t epilogStart,
        const uint32_t epilogSize,
        const intptr_t interpreterThunk);
#if defined(_M_ARM32_OR_ARM64)
    static uint32_t EncodeMove(uint32_t opCode, int reg, uint32_t imm16);
    static void GeneratePdata(_In_ const uint8_t* entryPoint, _In_ const uint32_t functionSize, _Out_ RUNTIME_FUNCTION* function);
#endif

    /*-------static helpers ---------*/
    inline static uint32_t FillDebugBreak(_Out_writes_bytes_all_(count) uint8_t* dest, _In_ uint32_t count);
    inline static uint32_t CopyWithAlignment(_Out_writes_bytes_all_(sizeInBytes) uint8_t* dest, _In_ const uint32_t sizeInBytes, _In_reads_bytes_(srcSize) const uint8_t* src, _In_ const uint32_t srcSize, _In_ const uint32_t alignment);
    template<class T>
    inline static void Emit(__in_bcount(sizeof(T) + offset) uint8_t* dest, const uint32_t offset, const T value)
    {
        AssertMsg(*(T*) (dest + offset) == 0, "Overwriting an already existing opcode?");
        *(T*)(dest + offset) = value;
    };

    uint8_t* AllocateFromFreeList(void ** ppDynamicInterpreterThunk);
public:
    static const uint8_t ThunkSize;
    static constexpr uint BlockSize = AutoSystemInfo::PageSize * PageCount;
    static void* ConvertToEntryPoint(void * dynamicInterpreterThunk);

    InterpreterThunkEmitter(Js::ScriptContext * context, ArenaAllocator* allocator, CustomHeap::InProcCodePageAllocators * codePageAllocators, bool isAsmInterpreterThunk = false);
    uint8_t* GetNextThunk(void ** ppDynamicInterpreterThunk);
    SListBase<ThunkBlock>* GetThunkBlocksList();

    void Close();
    void Release(uint8_t* thunkAddress, bool addtoFreeList);
    // Returns true if the argument falls within the range managed by this buffer.
#if DBG
    bool IsInHeap(void* address);
#endif
    const InProcEmitBufferManager* GetEmitBufferManager() const
    {
        return &emitBufferManager;
    }

    static void FillBuffer(
        _In_ ThreadContextInfo * context,
        _In_ bool asmJsThunk,
        _In_ intptr_t finalAddr,
        _In_ size_t bufferSize,
        _Out_writes_bytes_all_(BlockSize) uint8_t* buffer,
#if PDATA_ENABLED
        _Out_ PRUNTIME_FUNCTION * pdataTableStart,
        _Out_ intptr_t * epilogEndAddr,
#endif
        _Out_ uint32_t * thunkCount
    );

};
#endif
