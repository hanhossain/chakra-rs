//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// We need real JITManager code when on windows.
// Otherwise we use a dummy JITManager which disables OOP JIT to reduce code noise.

enum class RemoteCallType
{
    CodeGen,
    ThunkCreation,
    HeapQuery,
    StateUpdate,
    MemFree
};

class JITManager
{
public:
    int32_t SetWellKnownHostTypeId(
        PTHREADCONTEXT_HANDLE threadContextRoot,
        int typeId)
        { Assert(false); return E_FAIL; }

    int32_t CleanupScriptContext(
        PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
        { Assert(false); return E_FAIL; }

    int32_t CloseScriptContext(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
        { Assert(false); return E_FAIL; }

    int32_t FreeAllocation(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        intptr_t codeAddress)
        { Assert(false); return E_FAIL; }

    int32_t SetIsPRNGSeeded(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        boolean value)
        { Assert(false); return E_FAIL; }

    int32_t IsNativeAddr(
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        intptr_t address,
        boolean * result)
        { Assert(false); return E_FAIL; }

    static int32_t DeserializeRPCData(
        _In_reads_(bufferSize) const byte* buffer,
        _In_ uint bufferSize,
        _Out_ CodeGenWorkItemIDL **workItemData
    ) { *workItemData = nullptr; return E_NOTIMPL; }

    static int32_t SerializeRPCData(
        _In_ CodeGenWorkItemIDL *workItemData,
        _Out_ size_t* bufferSize,
        _Outptr_result_buffer_(*bufferSize) const byte** outBuffer
    ) { *bufferSize = 0; *outBuffer = nullptr; return E_NOTIMPL; }

    int32_t Shutdown()
        { Assert(false); return E_FAIL; }

    static JITManager * GetJITManager()
        { return &s_jitManager; }
    static bool HandleServerCallResult(int32_t hr, RemoteCallType callType) { Assert(UNREACHED); }
private:
    static JITManager s_jitManager;
};
