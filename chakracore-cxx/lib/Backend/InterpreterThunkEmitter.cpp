//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Backend.h"

#if ENABLE_NATIVE_CODEGEN

namespace {

// The definitions in this anonymous namespace must be constexpr to allow OACR to conclude that certain operations
// in InterpreterThunkEmitter::EncodeInterpreterThunk are safe.  Because constexpr requires that the declaration
// and the definition appear at the same place (i.e., no forward declarations), this means that we either have
// to move all 5 definitions of InterpreterThunk into the header file, or we have to make InterpreterThunkSize
// public.  The latter option seems the less objectionable, so that's what I've done here.

#ifdef _M_X64
constexpr uint8_t FunctionInfoOffset = 7;
constexpr uint8_t FunctionProxyOffset = 11;
constexpr uint8_t DynamicThunkAddressOffset = 15;
constexpr uint8_t CallBlockStartAddrOffset = 25;
constexpr uint8_t ThunkSizeOffset = 39;
constexpr uint8_t ErrorOffset = 48;
constexpr uint8_t ThunkAddressOffset = 61;

constexpr uint8_t PrologSize = 60;
constexpr uint8_t StackAllocSize = 0x0;

constexpr uint8_t InterpreterThunk[InterpreterThunkEmitter::InterpreterThunkSize] = {
    0x55,                                                       // push   rbp                   // Prolog - setup the stack frame
    0x48, 0x89, 0xe5,                                           // mov    rbp, rsp
    0x48, 0x8b, 0x47, 0x00,                                     // mov    rax, qword ptr [rdi + FunctionInfoOffset]
    0x48, 0x8B, 0x48, 0x00,                                     // mov    rcx, qword ptr [rax+FunctionProxyOffset]
    0x48, 0x8B, 0x51, 0x00,                                     // mov    rdx, qword ptr [rcx+DynamicThunkAddressOffset]
                                                                                                // Range Check for Valid call target
    0x48, 0x83, 0xE2, 0xF8,                                     // and    rdx, 0xfffffffffffffff8   // Force 8 byte alignment
    0x48, 0x89, 0xd1,                                           // mov    rcx, rdx
    0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov    rax, CallBlockStartAddress
    0x48, 0x29, 0xc1,                                           // sub    rcx, rax
    0x48, 0x81, 0xf9, 0x00, 0x00, 0x00, 0x00,                   // cmp    rcx, ThunkSize
    0x76, 0x09,                                                 // jbe    safe
    0x48, 0xc7, 0xc1, 0x00, 0x00, 0x00, 0x00,                   // mov    rcx, errorcode
    0xcd, 0x29,                                                 // int    29h       <-- xplat TODO: just to exit

    // safe:
    0x48, 0x8d, 0x7c, 0x24, 0x10,                               // lea    rdi, [rsp+0x10]
    0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov    rax, <thunk>          // stack already 16-byte aligned
    0xff, 0xe2,                                                 // jmp    rdx
    0xcc                                                        // int    3                     // for alignment to size of 8
};

constexpr uint8_t Epilog[] = {
    0x5d,                                                       // pop    rbp
    0xc3                                                        // ret
};
#elif defined(_M_ARM)

constexpr uint8_t ThunkAddressOffset = 8;
constexpr uint8_t FunctionInfoOffset = 18;
constexpr uint8_t FunctionProxyOffset = 22;
constexpr uint8_t DynamicThunkAddressOffset = 26;
constexpr uint8_t CallBlockStartAddressInstrOffset = 42;
constexpr uint8_t CallThunkSizeInstrOffset = 54;
constexpr uint8_t ErrorOffset = 64;

constexpr uint8_t InterpreterThunk[InterpreterThunkEmitter::InterpreterThunkSize] = {
    0x0F, 0xB4,                                                      // push        {r0-r3}
    0x2D, 0xE9, 0x00, 0x48,                                          // push        {r11,lr}
    0xEB, 0x46,                                                      // mov         r11,sp
    0x00, 0x00, 0x00, 0x00,                                          // movw        r1,ThunkAddress
    0x00, 0x00, 0x00, 0x00,                                          // movt        r1,ThunkAddress
    0xD0, 0xF8, 0x00, 0x20,                                          // ldr.w       r2,[r0,#0x00]
    0xD2, 0xF8, 0x00, 0x00,                                          // ldr.w       r0,[r2,#0x00]
    0xD0, 0xF8, 0x00, 0x30,                                          // ldr.w       r3,[r0,#0x00]
    0x4F, 0xF6, 0xF9, 0x70,                                          // mov         r0,#0xFFF9
    0xCF, 0xF6, 0xFF, 0x70,                                          // movt        r0,#0xFFFF
    0x03, 0xEA, 0x00, 0x03,                                          // and         r3,r3,r0
    0x18, 0x46,                                                      // mov         r0, r3
    0x00, 0x00, 0x00, 0x00,                                          // movw        r12, CallBlockStartAddress
    0x00, 0x00, 0x00, 0x00,                                          // movt        r12, CallBlockStartAddress
    0xA0, 0xEB, 0x0C, 0x00,                                          // sub         r0, r12
    0x00, 0x00, 0x00, 0x00,                                          // mov         r12, ThunkSize
    0x60, 0x45,                                                      // cmp         r0, r12
    0x02, 0xD9,                                                      // bls         $safe
    0x4F, 0xF0, 0x00, 0x00,                                          // mov         r0, errorcode
    0xFB, 0xDE,                                                      // Equivalent to int 0x29

    //$safe:
    0x02, 0xA8,                                                      // add         r0,sp,#8
    0x18, 0x47                                                       // bx          r3
};

constexpr uint8_t JmpOffset = 2;

constexpr uint8_t Call[] = {
    0x88, 0x47,                                                      // blx         r1
    0x00, 0x00, 0x00, 0x00,                                          // b.w         epilog
    0xFE, 0xDE,                                                      // int         3       ;Required for alignment
};

constexpr uint8_t Epilog[] = {
    0x5D, 0xF8, 0x04, 0xBB,                                         // pop         {r11}
    0x5D, 0xF8, 0x14, 0xFB                                          // ldr         pc,[sp],#0x14
};
#elif defined(_M_ARM64)

constexpr uint8_t FunctionInfoOffset = 8;
constexpr uint8_t FunctionProxyOffset = 12;
constexpr uint8_t DynamicThunkAddressOffset = 16;
constexpr uint8_t ThunkAddressOffset = 20;

//TODO: saravind :Implement Range Check for ARM64
constexpr uint8_t InterpreterThunk[InterpreterThunkEmitter::InterpreterThunkSize] = {
    0xFD, 0x7B, 0xBF, 0xA9,                                         //stp         fp, lr, [sp, #-16]!   ;Prologue
    0xFD, 0x03, 0x00, 0x91,                                         //mov         fp, sp                ;update frame pointer to the stack pointer
    0x02, 0x00, 0x40, 0xF9,                                         //ldr         x2, [x0, #0x00]       ;offset will be replaced with Offset of FunctionInfo
    0x40, 0x00, 0x40, 0xF9,                                         //ldr         x0, [x2, #0x00]       ;offset will be replaced with Offset of FunctionProxy
    0x03, 0x00, 0x40, 0xF9,                                         //ldr         x3, [x0, #0x00]       ;offset will be replaced with offset of DynamicInterpreterThunk
                                                                    //Following 4 MOV Instrs are to move the 64-bit address of the InterpreterThunk address into register x1.
    0x00, 0x00, 0x00, 0x00,                                         //movz        x1, #0x00             ;This is overwritten with the actual thunk address(16 - 0 bits) move
    0x00, 0x00, 0x00, 0x00,                                         //movk        x1, #0x00, lsl #16    ;This is overwritten with the actual thunk address(32 - 16 bits) move
    0x00, 0x00, 0x00, 0x00,                                         //movk        x1, #0x00, lsl #32    ;This is overwritten with the actual thunk address(48 - 32 bits) move
    0x00, 0x00, 0x00, 0x00,                                         //movk        x1, #0x00, lsl #48    ;This is overwritten with the actual thunk address(64 - 48 bits) move
    0xE0, 0x43, 0x00, 0x91,                                         //add         x0, sp, #16
    0x60, 0x00, 0x1F, 0xD6,                                         //br          x3
    0xCC, 0xCC, 0xCC, 0xCC                                          //int 3 for 8byte alignment
};

constexpr uint8_t JmpOffset = 4;

constexpr uint8_t Call[] = {
    0x20, 0x00, 0x3f, 0xd6,                                         // blr         x1
    0x00, 0x00, 0x00, 0x00                                          // b           epilog
};

constexpr uint8_t Epilog[] = {
    0xfd, 0x7b, 0xc1, 0xa8,                                         // ldp         fp, lr, [sp], #16
    0xc0, 0x03, 0x5f, 0xd6                                          // ret
};
#else // x86

constexpr uint8_t FunctionInfoOffset = 8;
constexpr uint8_t FunctionProxyOffset = 11;
constexpr uint8_t DynamicThunkAddressOffset = 14;
constexpr uint8_t CallBlockStartAddrOffset = 21;
constexpr uint8_t ThunkSizeOffset = 26;
constexpr uint8_t ErrorOffset = 33;
constexpr uint8_t ThunkAddressOffset = 44;

constexpr uint8_t InterpreterThunk[InterpreterThunkEmitter::InterpreterThunkSize] = {
    0x55,                                                           //   push        ebp                ;Prolog - setup the stack frame
    0x8B, 0xEC,                                                     //   mov         ebp,esp
    0x8B, 0x45, 0x08,                                               //   mov         eax, uint32_t ptr [ebp+8]
    0x8B, 0x40, 0x00,                                               //   mov         eax, uint32_t ptr [eax+FunctionInfoOffset]
    0x8B, 0x40, 0x00,                                               //   mov         eax, uint32_t ptr [eax+FunctionProxyOffset]
    0x8B, 0x48, 0x00,                                               //   mov         ecx, uint32_t ptr [eax+DynamicThunkAddressOffset]
                                                                    //   Range Check for Valid call target
    0x83, 0xE1, 0xF8,                                               //   and         ecx, 0FFFFFFF8h
    0x8b, 0xc1,                                                     //   mov         eax, ecx
    0x2d, 0x00, 0x00, 0x00, 0x00,                                   //   sub         eax, CallBlockStartAddress
    0x3d, 0x00, 0x00, 0x00, 0x00,                                   //   cmp         eax, ThunkSize
    0x76, 0x07,                                                     //   jbe         short $safe
    0xb9, 0x00, 0x00, 0x00, 0x00,                                   //   mov         ecx, errorcode
    0xCD, 0x29,                                                     //   int         29h

    //$safe
    0x8D, 0x45, 0x08,                                               //   lea         eax, ebp+8
    0x50,                                                           //   push        eax
    0xB8, 0x00, 0x00, 0x00, 0x00,                                   //   mov         eax, <thunk>
    0xFF, 0xE1,                                                     //   jmp         ecx
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC                              //   int 3 for 8byte alignment
};

constexpr uint8_t Epilog[] = {
    0x5D,                                                         // pop ebp
    0xC3                                                          // ret
};
#endif

#if defined(_M_X64) || defined(_M_IX86)
constexpr uint8_t JmpOffset = 3;

constexpr uint8_t Call[] = {
    0xFF, 0xD0,                                                // call       rax
    0xE9, 0x00, 0x00, 0x00, 0x00,                              // jmp        [offset]
    0xCC,                                                      // int 3      ;for alignment to size of 8 we are adding this
};

#endif

constexpr uint8_t HeaderSize = sizeof(InterpreterThunk);

} // anonymous namespace

const uint8_t InterpreterThunkEmitter::ThunkSize = sizeof(Call);

InterpreterThunkEmitter::InterpreterThunkEmitter(Js::ScriptContext* context, ArenaAllocator* allocator, CustomHeap::InProcCodePageAllocators * codePageAllocators, bool isAsmInterpreterThunk) :
    emitBufferManager(allocator, codePageAllocators, /*scriptContext*/ nullptr, nullptr, u"Interpreter thunk buffer", GetCurrentProcess()),
    scriptContext(context),
    allocator(allocator),
    thunkCount(0),
    thunkBuffer(nullptr),
    isAsmInterpreterThunk(isAsmInterpreterThunk)
{
}

SListBase<ThunkBlock>* 
InterpreterThunkEmitter::GetThunkBlocksList()
{
    return &thunkBlocks;
}

//
// Returns the next thunk. Batch allocated PageCount pages of thunks and issue them one at a time
//
uint8_t* InterpreterThunkEmitter::GetNextThunk(void ** ppDynamicInterpreterThunk)
{
    Assert(ppDynamicInterpreterThunk);
    Assert(*ppDynamicInterpreterThunk == nullptr);

    if(thunkCount == 0)
    {
        if(!this->freeListedThunkBlocks.Empty())
        {
            return AllocateFromFreeList(ppDynamicInterpreterThunk);
        }
        if (!NewThunkBlock())
        {
#ifdef ASMJS_PLAT
            return this->isAsmInterpreterThunk ? (uint8_t*)&Js::InterpreterStackFrame::StaticInterpreterAsmThunk : (uint8_t*)&Js::InterpreterStackFrame::StaticInterpreterThunk;
#else
            Assert(!this->isAsmInterpreterThunk);
            return (uint8_t*)&Js::InterpreterStackFrame::StaticInterpreterThunk;
#endif
        }
    }

    Assert(this->thunkBuffer != nullptr);
    uint8_t* thunk = this->thunkBuffer;
#if _M_ARM
    thunk = (uint8_t*)((uint32_t)thunk | 0x01);
#endif
    *ppDynamicInterpreterThunk = thunk + HeaderSize + ((--thunkCount) * ThunkSize);
#if _M_ARM
    AssertMsg(((uintptr_t)(*ppDynamicInterpreterThunk) & 0x6) == 0, "Not 8 byte aligned?");
#else
    AssertMsg(((uintptr_t)(*ppDynamicInterpreterThunk) & 0x7) == 0, "Not 8 byte aligned?");
#endif
    return thunk;
}

//
// Interpreter thunks have an entrypoint at the beginning of the page boundary. Each function has a unique thunk return address
// and this function can convert to the unique thunk return address to the beginning of the page which corresponds with the entrypoint
//
void* InterpreterThunkEmitter::ConvertToEntryPoint(void * dynamicInterpreterThunk)
{
    Assert(dynamicInterpreterThunk != nullptr);
    void* entryPoint = (void*)((size_t)dynamicInterpreterThunk & (~((size_t)(BlockSize) - 1)));

#if _M_ARM
    entryPoint = (uint8_t*)((uint32_t)entryPoint | 0x01);
#endif
    return entryPoint;
}

bool InterpreterThunkEmitter::NewThunkBlock()
{
    if (this->scriptContext->GetConfig()->IsNoDynamicThunks())
    {
        return false;
    }

#ifdef ENABLE_OOP_NATIVE_CODEGEN
    if (CONFIG_FLAG(ForceStaticInterpreterThunk))
    {
        return false;
    }

    if (JITManager::GetJITManager()->IsOOPJITEnabled())
    {
        return NewOOPJITThunkBlock();
    }
#endif

    Assert(this->thunkCount == 0);
    uint8_t* buffer;

    EmitBufferAllocation<VirtualAllocWrapper, PreReservedVirtualAllocWrapper> * allocation = emitBufferManager.AllocateBuffer(BlockSize, &buffer);
    if (allocation == nullptr)
    {
        Js::Throw::OutOfMemory();
    }
    if (!emitBufferManager.ProtectBufferWithExecuteReadWriteForInterpreter(allocation))
    {
        Js::Throw::OutOfMemory();
    }

#if PDATA_ENABLED
    PRUNTIME_FUNCTION pdataStart = nullptr;
    intptr_t epilogEnd = 0;
#endif

    uint32_t count = this->thunkCount;
    FillBuffer(
        this->scriptContext->GetThreadContext(),
        this->isAsmInterpreterThunk,
        (intptr_t)buffer,
        BlockSize,
        buffer,
#if PDATA_ENABLED
        &pdataStart,
        &epilogEnd,
#endif
        &count
    );

    if (!emitBufferManager.CommitBufferForInterpreter(allocation, buffer, BlockSize))
    {
        Js::Throw::OutOfMemory();
    }

    // Call to set VALID flag for CFG check
    uint8_t* callTarget = buffer;
#ifdef _M_ARM
    // We want to allow the actual callable value, so thumb-tag the address
    callTarget = (uint8_t*)((uintptr_t)buffer | 0x1);
#endif
    ThreadContext::GetContextForCurrentThread()->SetValidCallTargetForCFG(callTarget);

    // Update object state only at the end when everything has succeeded - and no exceptions can be thrown.
    auto block = this->thunkBlocks.PrependNode(allocator, buffer, count);
#if PDATA_ENABLED
    void* pdataTable;
    PDataManager::RegisterPdata((PRUNTIME_FUNCTION)pdataStart, (size_t)buffer, (size_t)epilogEnd, &pdataTable);
    block->SetPdata(pdataTable);
#else
    Unused(block);
#endif
    this->thunkBuffer = buffer;
    this->thunkCount = count;
    return true;
}

#ifdef ENABLE_OOP_NATIVE_CODEGEN
bool InterpreterThunkEmitter::NewOOPJITThunkBlock()
{
    PSCRIPTCONTEXT_HANDLE remoteScriptContext = this->scriptContext->GetRemoteScriptAddr();
    if (!JITManager::GetJITManager()->IsConnected())
    {
        return false;
    }
    InterpreterThunkInputIDL thunkInput;
    thunkInput.asmJsThunk = this->isAsmInterpreterThunk;

    InterpreterThunkOutputIDL thunkOutput;
    int32_t hr = JITManager::GetJITManager()->NewInterpreterThunkBlock(remoteScriptContext, &thunkInput, &thunkOutput);
    if (!JITManager::HandleServerCallResult(hr, RemoteCallType::ThunkCreation))
    {
        return false;
    }

    uint8_t* buffer = (uint8_t*)thunkOutput.mappedBaseAddr;

    if (!CONFIG_FLAG(OOPCFGRegistration))
    {
        uint8_t* callTarget = buffer;
#ifdef _M_ARM
        // Need to register the thumb-tagged call target for CFG
        callTarget = (uint8_t*)((uintptr_t)callTarget | 0x1);
#endif
        this->scriptContext->GetThreadContext()->SetValidCallTargetForCFG(callTarget);
    }

    // Update object state only at the end when everything has succeeded - and no exceptions can be thrown.
    auto block = this->thunkBlocks.PrependNode(allocator, buffer, thunkOutput.thunkCount);
#if PDATA_ENABLED
    void* pdataTable;
    PDataManager::RegisterPdata((PRUNTIME_FUNCTION)thunkOutput.pdataTableStart, (size_t)thunkOutput.mappedBaseAddr, (size_t)thunkOutput.epilogEndAddr, &pdataTable);
    block->SetPdata(pdataTable);
#else
    Unused(block);
#endif

    this->thunkBuffer = (uint8_t*)thunkOutput.mappedBaseAddr;
    this->thunkCount = thunkOutput.thunkCount;
    return true;
}
#endif

/* static */
void InterpreterThunkEmitter::FillBuffer(
    _In_ ThreadContextInfo * threadContext,
    _In_ bool asmJsThunk,
    _In_ intptr_t finalAddr,
    _In_ size_t bufferSize,
    _Out_writes_bytes_all_(BlockSize) uint8_t* buffer,
#if PDATA_ENABLED
    _Out_ PRUNTIME_FUNCTION * pdataTableStart,
    _Out_ intptr_t * epilogEndAddr,
#endif
    _Out_ uint32_t * thunkCount
    )
{
#ifdef _M_X64
    PrologEncoder prologEncoder;
    prologEncoder.EncodeSmallProlog(PrologSize, StackAllocSize);
    uint32_t pdataSize = prologEncoder.SizeOfPData();
#elif defined(_M_ARM32_OR_ARM64)
    uint32_t pdataSize = sizeof(RUNTIME_FUNCTION);
#else
    uint32_t pdataSize = 0;
#endif
    uint32_t bytesRemaining = BlockSize;
    uint32_t bytesWritten = 0;
    uint32_t thunks = 0;
    uint32_t epilogSize = sizeof(Epilog);
    const uint8_t *epilog = Epilog;
    const uint8_t *header = InterpreterThunk;

    intptr_t interpreterThunk;

    // the static interpreter thunk invoked by the dynamic emitted thunk
#ifdef ASMJS_PLAT
    if (asmJsThunk)
    {
        interpreterThunk = ShiftAddr(threadContext, &Js::InterpreterStackFrame::InterpreterAsmThunk);
    }
    else
#endif
    {
        interpreterThunk = ShiftAddr(threadContext, &Js::InterpreterStackFrame::InterpreterThunk);
    }


    uint8_t * currentBuffer = buffer;
    // Ensure there is space for PDATA at the end
    uint8_t* pdataStart = currentBuffer + (BlockSize - Math::Align(pdataSize, EMIT_BUFFER_ALIGNMENT));
    uint8_t* epilogStart = pdataStart - Math::Align(epilogSize, EMIT_BUFFER_ALIGNMENT);

    // Ensure there is space for PDATA at the end
    intptr_t finalPdataStart = finalAddr + (BlockSize - Math::Align(pdataSize, EMIT_BUFFER_ALIGNMENT));
    intptr_t finalEpilogStart = finalPdataStart - Math::Align(epilogSize, EMIT_BUFFER_ALIGNMENT);

    // Copy the thunk buffer and modify it.
    js_memcpy_s(currentBuffer, bytesRemaining, header, HeaderSize);
    EncodeInterpreterThunk(currentBuffer, finalAddr, finalEpilogStart, epilogSize, interpreterThunk);
    currentBuffer += HeaderSize;
    bytesRemaining -= HeaderSize;

    // Copy call buffer
    uint32_t callSize = sizeof(Call);
    while (currentBuffer < epilogStart - callSize)
    {
        js_memcpy_s(currentBuffer, bytesRemaining, Call, callSize);
#if _M_ARM
        int offset = (epilogStart - (currentBuffer + JmpOffset));
        Assert(offset >= 0);
        uint32_t encodedOffset = EncoderMD::BranchOffset_T2_24(offset);
        uint32_t encodedBranch = /*opcode=*/ 0x9000F000 | encodedOffset;
        Emit(currentBuffer, JmpOffset, encodedBranch);
#elif _M_ARM64
        long offset = (epilogStart - (currentBuffer + JmpOffset));
        Assert(offset >= 0);
        uint32_t encodedOffset = EncoderMD::BranchOffset_26(offset);
        uint32_t encodedBranch = /*opcode=*/ 0x14000000 | encodedOffset;
        Emit(currentBuffer, JmpOffset, encodedBranch);
#else
        // jump requires an offset from the end of the jump instruction.
        int offset = (int)(epilogStart - (currentBuffer + JmpOffset + sizeof(int)));
        Assert(offset >= 0);
        Emit(currentBuffer, JmpOffset, offset);
#endif
        currentBuffer += callSize;
        bytesRemaining -= callSize;
        thunks++;
    }

    // Fill any gap till start of epilog
    bytesWritten = FillDebugBreak(currentBuffer, (uint32_t)(epilogStart - currentBuffer));
    bytesRemaining -= bytesWritten;
    currentBuffer += bytesWritten;

    // Copy epilog
    bytesWritten = CopyWithAlignment(currentBuffer, bytesRemaining, epilog, epilogSize, EMIT_BUFFER_ALIGNMENT);
    currentBuffer += bytesWritten;
    bytesRemaining -= bytesWritten;

    // Generate and register PDATA
#if PDATA_ENABLED
    uint8_t* epilogEnd = epilogStart + epilogSize;
    uint32_t functionSize = (uint32_t)(epilogEnd - buffer);
    Assert(pdataStart == currentBuffer);
#ifdef _M_X64
    Assert(bytesRemaining >= pdataSize);
    uint8_t* pdata = prologEncoder.Finalize(buffer, functionSize, pdataStart);
    bytesWritten = CopyWithAlignment(pdataStart, bytesRemaining, pdata, pdataSize, EMIT_BUFFER_ALIGNMENT);
#elif defined(_M_ARM32_OR_ARM64)
    RUNTIME_FUNCTION pdata;
    GeneratePdata(buffer, functionSize, &pdata);
    bytesWritten = CopyWithAlignment(pdataStart, bytesRemaining, (const uint8_t*)&pdata, pdataSize, EMIT_BUFFER_ALIGNMENT);
#endif
    *pdataTableStart = (PRUNTIME_FUNCTION)finalPdataStart;
    *epilogEndAddr = finalEpilogStart;
#endif
    *thunkCount = thunks;
}

#if _M_ARM
void InterpreterThunkEmitter::EncodeInterpreterThunk(
    __in_bcount(InterpreterThunkSize) uint8_t* thunkBuffer,
    const intptr_t thunkBufferStartAddress,
    const intptr_t epilogStart,
    const uint32_t epilogSize,
    const intptr_t interpreterThunk)
{
    // Encode MOVW
    uint32_t lowerThunkBits = (uint32)interpreterThunk & 0x0000FFFF;
    uint32_t movW = EncodeMove(/*Opcode*/ 0x0000F240, /*register*/1, lowerThunkBits);
    Emit(thunkBuffer,ThunkAddressOffset, movW);

    // Encode MOVT
    uint32_t higherThunkBits = ((uint32)interpreterThunk & 0xFFFF0000) >> 16;
    uint32_t movT = EncodeMove(/*Opcode*/ 0x0000F2C0, /*register*/1, higherThunkBits);
    Emit(thunkBuffer, ThunkAddressOffset + sizeof(movW), movT);

    // Encode LDR - Load of function Body
    thunkBuffer[FunctionInfoOffset] = Js::JavascriptFunction::GetOffsetOfFunctionInfo();
    thunkBuffer[FunctionProxyOffset] = Js::FunctionInfo::GetOffsetOfFunctionProxy();

    // Encode LDR - Load of interpreter thunk number
    thunkBuffer[DynamicThunkAddressOffset] = Js::FunctionBody::GetOffsetOfDynamicInterpreterThunk();

    // Encode MOVW R12, CallBlockStartAddress
    uintptr_t callBlockStartAddress = (uintptr_t)thunkBufferStartAddress + HeaderSize;
    uint totalThunkSize = (uint)(epilogStart - callBlockStartAddress);

    uint32_t lowerCallBlockStartAddress = callBlockStartAddress & 0x0000FFFF;
    uint32_t movWblockStart = EncodeMove(/*Opcode*/ 0x0000F240, /*register*/12, lowerCallBlockStartAddress);
    Emit(thunkBuffer,CallBlockStartAddressInstrOffset, movWblockStart);

    // Encode MOVT R12, CallBlockStartAddress
    uint32_t higherCallBlockStartAddress = (callBlockStartAddress & 0xFFFF0000) >> 16;
    uint32_t movTblockStart = EncodeMove(/*Opcode*/ 0x0000F2C0, /*register*/12, higherCallBlockStartAddress);
    Emit(thunkBuffer, CallBlockStartAddressInstrOffset + sizeof(movWblockStart), movTblockStart);

    //Encode MOV R12, CallBlockSize
    uint32_t movBlockSize = EncodeMove(/*Opcode*/ 0x0000F240, /*register*/12, (uint32_t)totalThunkSize);
    Emit(thunkBuffer, CallThunkSizeInstrOffset, movBlockSize);

    Emit(thunkBuffer, ErrorOffset, (uint8_t) FAST_FAIL_INVALID_ARG);
}

uint32_t InterpreterThunkEmitter::EncodeMove(uint32_t opCode, int reg, uint32_t imm16)
{
    uint32_t encodedMove = reg << 24;
#if _M_ARM
    uint32_t encodedImm = 0;
    EncoderMD::EncodeImmediate16(imm16, &encodedImm);
    encodedMove |= encodedImm;
#elif _M_ARM64
    // ToDo (SaAgarwa) - From Aaron change. Validate for ARM64
    encodedMove |= (imm16 & 0xFFFF) << 5;
#endif
    AssertMsg((encodedMove & opCode) == 0, "Any bits getting overwritten?");
    encodedMove |= opCode;
    return encodedMove;
}

void InterpreterThunkEmitter::GeneratePdata(_In_ const uint8_t* entryPoint, _In_ const uint32_t functionSize, _Out_ RUNTIME_FUNCTION* function)
{
    function->BeginAddress   = 0x1;                        // Since our base address is the start of the function - this is offset from the base address
    function->Flag           = 1;                          // Packed unwind data is used
    function->FunctionLength = functionSize / 2;
    function->Ret            = 0;                          // Return via Pop
    function->H              = 1;                          // Homes parameters
    function->Reg            = 7;                          // No saved registers - R11 is the frame pointer - not considered here
    function->R              = 1;                          // No registers are being saved.
    function->L              = 1;                          // Save/restore LR register
    function->C              = 1;                          // Frame pointer chain in R11 established
    function->StackAdjust    = 0;                          // Stack allocation for the function
}

#elif _M_ARM64
void InterpreterThunkEmitter::EncodeInterpreterThunk(
    __in_bcount(InterpreterThunkSize) uint8_t* thunkBuffer,
    const intptr_t thunkBufferStartAddress,
    const intptr_t epilogStart,
    const uint32_t epilogSize,
    const intptr_t interpreterThunk)
{
    int addrOffset = ThunkAddressOffset;

    // Following 4 MOV Instrs are to move the 64-bit address of the InterpreterThunk address into register x1.

    // Encode MOVZ (movz        x1, #<interpreterThunk 16-0 bits>)
    uint32_t lowerThunkBits = (unsigned long)interpreterThunk & 0x0000FFFF;
    uint32_t movZ = EncodeMove(/*Opcode*/ 0xD2800000, /*register x1*/1, lowerThunkBits); // no shift; hw = 00
    Emit(thunkBuffer,addrOffset, movZ);
    static_assert(sizeof(movZ) == 4, "movZ has to be 32-bit encoded");
    addrOffset+= sizeof(movZ);

    // Encode MOVK (movk        x1, #<interpreterThunk 32-16 bits>, lsl #16)
    uint32_t higherThunkBits = ((unsigned long)interpreterThunk & 0xFFFF0000) >> 16;
    uint32_t movK = EncodeMove(/*Opcode*/ 0xF2A00000, /*register x1*/1, higherThunkBits); // left shift 16 bits; hw = 01
    Emit(thunkBuffer, addrOffset, movK);
    static_assert(sizeof(movK) == 4, "movK has to be 32-bit encoded");
    addrOffset+= sizeof(movK);

    // Encode MOVK (movk        x1, #<interpreterThunk 48-32 bits>, lsl #16)
    higherThunkBits = ((unsigned long)interpreterThunk & 0xFFFF00000000) >> 32;
    movK = EncodeMove(/*Opcode*/ 0xF2C00000, /*register x1*/1, higherThunkBits); // left shift 32 bits; hw = 02
    Emit(thunkBuffer, addrOffset, movK);
    addrOffset += sizeof(movK);

    // Encode MOVK (movk        x1, #<interpreterThunk 64-48 bits>, lsl #16)
    higherThunkBits = ((unsigned long)interpreterThunk & 0xFFFF000000000000) >> 48;
    movK = EncodeMove(/*Opcode*/ 0xF2E00000, /*register x1*/1, higherThunkBits); // left shift 48 bits; hw = 03
    Emit(thunkBuffer, addrOffset, movK);

    // Encode LDR - Load of function Body
    uint32_t offsetOfFunctionInfo = Js::JavascriptFunction::GetOffsetOfFunctionInfo();
    AssertMsg(offsetOfFunctionInfo % 8 == 0, "Immediate offset for LDR must be 8 byte aligned");
    AssertMsg(offsetOfFunctionInfo < 0x8000, "Immediate offset for LDR must be less than 0x8000");
    *(uint32_t *)&thunkBuffer[FunctionInfoOffset] |= (offsetOfFunctionInfo / 8) << 10;

    uint32_t offsetOfFunctionProxy = Js::FunctionInfo::GetOffsetOfFunctionProxy();
    AssertMsg(offsetOfFunctionProxy % 8 == 0, "Immediate offset for LDR must be 8 byte aligned");
    AssertMsg(offsetOfFunctionProxy < 0x8000, "Immediate offset for LDR must be less than 0x8000");
    *(uint32_t *)&thunkBuffer[FunctionProxyOffset] |= (offsetOfFunctionProxy / 8) << 10;

    // Encode LDR - Load of interpreter thunk number
    uint32_t offsetOfDynamicInterpreterThunk = Js::FunctionBody::GetOffsetOfDynamicInterpreterThunk();
    AssertMsg(offsetOfDynamicInterpreterThunk % 8 == 0, "Immediate offset for LDR must be 8 byte aligned");
    AssertMsg(offsetOfDynamicInterpreterThunk < 0x8000, "Immediate offset for LDR must be less than 0x8000");
    *(uint32_t *)&thunkBuffer[DynamicThunkAddressOffset] |= (offsetOfDynamicInterpreterThunk / 8) << 10;
}

uint32_t InterpreterThunkEmitter::EncodeMove(uint32_t opCode, int reg, uint32_t imm16)
{
    uint32_t encodedMove = reg << 0;
#if _M_ARM
    uint32_t encodedImm = 0;
    EncoderMD::EncodeImmediate16(imm16, &encodedImm);
    encodedMove |= encodedImm;
#elif _M_ARM64
    // ToDo (SaAgarwa) - From Aaron change. Validate for ARM64
    encodedMove |= (imm16 & 0xFFFF) << 5;
#endif
    AssertMsg((encodedMove & opCode) == 0, "Any bits getting overwritten?");
    encodedMove |= opCode;
    return encodedMove;
}

void InterpreterThunkEmitter::GeneratePdata(_In_ const uint8_t* entryPoint, _In_ const uint32_t functionSize, _Out_ RUNTIME_FUNCTION* function)
{
    function->BeginAddress = 0x0;               // Since our base address is the start of the function - this is offset from the base address
    function->Flag = 1;                         // Packed unwind data is used
    function->FunctionLength = functionSize / 4;
    function->RegF = 0;                         // number of non-volatile FP registers (d8-d15) saved in the canonical stack location
    function->RegI = 0;                         // number of non-volatile INT registers (r19-r28) saved in the canonical stack location
    function->H = 1;                            // Homes parameters
    // (indicating whether the function "homes" the integer parameter registers (r0-r7) by storing them at the very start of the function)

    function->CR = 3;                           // chained function, a store/load pair instruction is used in prolog/epilog <r29,lr>
    function->FrameSize = 5;                    // the number of bytes of stack that is allocated for this function divided by 16
}
#else
void InterpreterThunkEmitter::EncodeInterpreterThunk(
    __in_bcount(InterpreterThunkSize) uint8_t* thunkBuffer,
    const intptr_t thunkBufferStartAddress,
    const intptr_t epilogStart,
    const uint32_t epilogSize,
    const intptr_t interpreterThunk)
{
    Emit(thunkBuffer, ThunkAddressOffset, (uintptr_t)interpreterThunk);
    thunkBuffer[DynamicThunkAddressOffset] = Js::FunctionBody::GetOffsetOfDynamicInterpreterThunk();
    thunkBuffer[FunctionInfoOffset] = Js::JavascriptFunction::GetOffsetOfFunctionInfo();
    thunkBuffer[FunctionProxyOffset] = Js::FunctionInfo::GetOffsetOfFunctionProxy();
    Emit(thunkBuffer, CallBlockStartAddrOffset, (uintptr_t) thunkBufferStartAddress + HeaderSize);
    uint totalThunkSize = (uint)(epilogStart - (thunkBufferStartAddress + HeaderSize));
    Emit(thunkBuffer, ThunkSizeOffset, totalThunkSize);
    Emit(thunkBuffer, ErrorOffset, (uint8_t) FAST_FAIL_INVALID_ARG);
}
#endif

/*static*/
uint32_t InterpreterThunkEmitter::FillDebugBreak(_Out_writes_bytes_all_(count) uint8_t* dest, _In_ uint32_t count)
{
#if defined(_M_ARM)
    Assert(count % 2 == 0);
#elif defined(_M_ARM64)
    Assert(count % 4 == 0);
#endif
    CustomHeap::FillDebugBreak(dest, count);
    return count;
}

/*static*/
uint32_t InterpreterThunkEmitter::CopyWithAlignment(
    _Out_writes_bytes_all_(sizeInBytes) uint8_t* dest,
    _In_ const uint32_t sizeInBytes,
    _In_reads_bytes_(srcSize) const uint8_t* src,
    _In_ const uint32_t srcSize,
    _In_ const uint32_t alignment)
{
    js_memcpy_s(dest, sizeInBytes, src, srcSize);
    dest += srcSize;

    uint32_t alignPad = Math::Align(srcSize, alignment) - srcSize;
    Assert(alignPad <= (sizeInBytes - srcSize));
    if(alignPad > 0 && alignPad <= (sizeInBytes - srcSize))
    {
        FillDebugBreak(dest, alignPad);
        return srcSize + alignPad;
    }
    return srcSize;
}

#if DBG
bool
InterpreterThunkEmitter::IsInHeap(void* address)
{
#ifdef ENABLE_OOP_NATIVE_CODEGEN
    if (JITManager::GetJITManager()->IsOOPJITEnabled())
    {
        PSCRIPTCONTEXT_HANDLE remoteScript = this->scriptContext->GetRemoteScriptAddr(false);
        if (!remoteScript || !JITManager::GetJITManager()->IsConnected())
        {
            // this method is used in asserts to validate whether an entry point is valid
            // in case JIT process crashed, let's just say true to keep asserts from firing
            return true;
        }
        boolean result;
        int32_t hr = JITManager::GetJITManager()->IsInterpreterThunkAddr(remoteScript, (intptr_t)address, this->isAsmInterpreterThunk, &result);
        if (!JITManager::HandleServerCallResult(hr, RemoteCallType::HeapQuery))
        {
            return true;
        }
        return result != FALSE;
    }
    else
#endif
    {
        return emitBufferManager.IsInHeap(address);
    }
}
#endif

// We only decommit at close because there might still be some
// code running here.
// The destructor of emitBufferManager will cause the eventual release.
void InterpreterThunkEmitter::Close()
{
#if PDATA_ENABLED
    auto unregisterPdata = ([&] (const ThunkBlock& block)
    {
        PDataManager::UnregisterPdata((PRUNTIME_FUNCTION) block.GetPdata());
    });
    thunkBlocks.Iterate(unregisterPdata);
    freeListedThunkBlocks.Iterate(unregisterPdata);
#endif

    this->thunkBlocks.Clear(allocator);
    this->freeListedThunkBlocks.Clear(allocator);

#ifdef ENABLE_OOP_NATIVE_CODEGEN
    if (JITManager::GetJITManager()->IsOOPJITEnabled())
    {
        PSCRIPTCONTEXT_HANDLE remoteScript = this->scriptContext->GetRemoteScriptAddr(false);
        if (remoteScript && JITManager::GetJITManager()->IsConnected())
        {
            JITManager::GetJITManager()->DecommitInterpreterBufferManager(remoteScript, this->isAsmInterpreterThunk);
        }
    }
    else
#endif
    {
        emitBufferManager.Decommit();
    }


    this->thunkBuffer = nullptr;
    this->thunkCount = 0;
}

void InterpreterThunkEmitter::Release(uint8_t* thunkAddress, bool addtoFreeList)
{
    if(!addtoFreeList)
    {
        return;
    }

    auto predicate = ([=] (const ThunkBlock& block)
    {
        return block.Contains(thunkAddress);
    });

    ThunkBlock* block = freeListedThunkBlocks.Find(predicate);
    if(!block)
    {
        block = thunkBlocks.MoveTo(&freeListedThunkBlocks, predicate);
    }

    // if EnsureFreeList fails in an OOM scenario - we just leak the thunks
    if(block && block->EnsureFreeList(allocator))
    {
        block->Release(thunkAddress);
    }
}

uint8_t* InterpreterThunkEmitter::AllocateFromFreeList(void ** ppDynamicInterpreterThunk )
{
    ThunkBlock& block = this->freeListedThunkBlocks.Head();
    uint8_t* thunk = block.AllocateFromFreeList();
#if _M_ARM
    thunk = (uint8_t*)((uint32_t)thunk | 0x01);
#endif
    if(block.IsFreeListEmpty())
    {
        this->freeListedThunkBlocks.MoveHeadTo(&this->thunkBlocks);
    }
    *ppDynamicInterpreterThunk = thunk;
    uint8_t* entryPoint = block.GetStart();
#if _M_ARM
    entryPoint = (uint8_t*)((uint32_t)entryPoint | 0x01);
#endif
    return entryPoint;
}


bool ThunkBlock::Contains(uint8_t* address) const
{
    bool contains = address >= start && address < (start + InterpreterThunkEmitter::BlockSize);
    return contains;
}

void ThunkBlock::Release(uint8_t* address)
{
    Assert(Contains(address));
    Assert(this->freeList);

    BVIndex index = FromThunkAddress(address);
    this->freeList->Set(index);
}

uint8_t* ThunkBlock::AllocateFromFreeList()
{
    Assert(this->freeList);
    BVIndex index = this->freeList->GetNextBit(0);
    uint8_t* address = ToThunkAddress(index);
    this->freeList->Clear(index);
    return address;
}

BVIndex ThunkBlock::FromThunkAddress(uint8_t* address)
{
    uint index = ((uint)(address - start) - HeaderSize) / InterpreterThunkEmitter::ThunkSize;
    Assert(index < this->thunkCount);
    return index;
}

uint8_t* ThunkBlock::ToThunkAddress(BVIndex index)
{
    Assert(index < this->thunkCount);
    uint8_t* address = start + HeaderSize + InterpreterThunkEmitter::ThunkSize * index;
    return address;
}

bool ThunkBlock::EnsureFreeList(ArenaAllocator* allocator)
{
    if(!this->freeList)
    {
        this->freeList = BVFixed::NewNoThrow(this->thunkCount, allocator);
    }
    return this->freeList != nullptr;
}

bool ThunkBlock::IsFreeListEmpty() const
{
    Assert(this->freeList);
    return this->freeList->IsAllClear();
}

#endif  // ENABLE_NATIVE_CODEGEN
