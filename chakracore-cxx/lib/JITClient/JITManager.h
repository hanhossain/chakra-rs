//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// We need real JITManager code when on windows or explict ENABLE_OOP_NATIVE_CODEGEN.
// Otherwise we use a dummy JITManager which disables OOP JIT to reduce code noise.

enum class RemoteCallType
{
    CodeGen,
    ThunkCreation,
    HeapQuery,
    StateUpdate,
    MemFree
};

#if ENABLE_OOP_NATIVE_CODEGEN
class JITManager
{
public:
    HRESULT ConnectRpcServer(HANDLE jitProcessHandle, __in_opt void* serverSecurityDescriptor, UUID connectionUuid);

    bool IsConnected() const;
    bool IsJITServer() const;
    void SetIsJITServer();
    bool IsOOPJITEnabled() const;
    void EnableOOPJIT();
    void SetJITFailed(HRESULT hr);
    bool HasJITFailed() const;

    HRESULT InitializeThreadContext(
        ThreadContextDataIDL * data,
        PPTHREADCONTEXT_HANDLE threadContextInfoAddress,
        intptr_t * prereservedRegionAddr,
        intptr_t * jitThunkAddr);

    HRESULT CleanupThreadContext(
        __inout PPTHREADCONTEXT_HANDLE threadContextInfoAddress);

    HRESULT UpdatePropertyRecordMap(
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        __in_opt BVSparseNodeIDL * updatedPropsBVHead);

    HRESULT DecommitInterpreterBufferManager(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        boolean asmJsThunk);

    HRESULT NewInterpreterThunkBlock(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        InterpreterThunkInputIDL * thunkInput,
        InterpreterThunkOutputIDL * thunkOutput);

    HRESULT AddModuleRecordInfo(
            /* [in] */ PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
            /* [in] */ unsigned int moduleId,
            /* [in] */ intptr_t localExportSlotsAddr);

    HRESULT SetWellKnownHostTypeId(
        PTHREADCONTEXT_HANDLE threadContextRoot,
        int typeId);

    HRESULT InitializeScriptContext(
        ScriptContextDataIDL * data,
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress);

    HRESULT CleanupScriptContext(
        __inout PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress);

    HRESULT CloseScriptContext(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress);

    HRESULT FreeAllocation(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        intptr_t codeAddress);

    HRESULT SetIsPRNGSeeded(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        boolean value);

    HRESULT IsNativeAddr(
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        intptr_t address,
        boolean * result);

    HRESULT RemoteCodeGenCall(
        CodeGenWorkItemIDL *workItemData,
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        JITOutputIDL *jitData);

#if DBG
    HRESULT IsInterpreterThunkAddr(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        intptr_t address,
        boolean asmjsThunk,
        boolean * result);
#endif

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    static HRESULT DeserializeRPCData(
        _In_reads_(bufferSize) const byte* buffer,
        _In_ uint bufferSize,
        _Out_ CodeGenWorkItemIDL **workItemData
    );

    static HRESULT SerializeRPCData(
        _In_ CodeGenWorkItemIDL *workItemData,
        _Out_ size_t* bufferSize,
        _Outptr_result_buffer_(*bufferSize) const byte** outBuffer
    );
#endif

    HRESULT Shutdown();


    static JITManager * GetJITManager();
    static bool HandleServerCallResult(HRESULT hr, RemoteCallType callType);
private:
    JITManager();
    ~JITManager();

    HRESULT CreateBinding(
        HANDLE serverProcessHandle,
        __in_opt void* serverSecurityDescriptor,
        UUID* connectionUuid,
        RPC_BINDING_HANDLE* bindingHandle);

    HRESULT ConnectProcess(RPC_BINDING_HANDLE rpcBindingHandle);

    RPC_BINDING_HANDLE m_rpcBindingHandle;
    UUID m_jitConnectionId;
    bool m_oopJitEnabled;
    bool m_isJITServer;
    HRESULT m_failingHRESULT;
    CriticalSection m_cs;

    static JITManager s_jitManager;

};

#else  // !ENABLE_OOP_NATIVE_CODEGEN
class JITManager
{
public:
    HRESULT ConnectRpcServer(HANDLE jitProcessHandle, __in_opt void* serverSecurityDescriptor, UUID connectionUuid)
        { Assert(false); return E_FAIL; }

    bool IsConnected() const { return false; }
    bool IsJITServer() const { return false; }
    void SetIsJITServer() { Assert(false); }
    bool IsOOPJITEnabled() const { return false; }
    void EnableOOPJIT() { Assert(false); }
    void SetJITFailed(HRESULT hr) { Assert(false); }

    HRESULT InitializeThreadContext(
        ThreadContextDataIDL * data,
        PPTHREADCONTEXT_HANDLE threadContextInfoAddress,
        intptr_t *prereservedRegionAddr,
        intptr_t * jitThunkAddr)
        { Assert(false); return E_FAIL; }

    HRESULT DecommitInterpreterBufferManager(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        boolean asmJsThunk)
        { Assert(false); return E_FAIL; }

    HRESULT CleanupThreadContext(
        __inout PPTHREADCONTEXT_HANDLE threadContextInfoAddress)
        { Assert(false); return E_FAIL; }

    HRESULT UpdatePropertyRecordMap(
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        __in_opt BVSparseNodeIDL * updatedPropsBVHead)
        { Assert(false); return E_FAIL; }

    HRESULT AddModuleRecordInfo(
            /* [in] */ PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
            /* [in] */ unsigned int moduleId,
            /* [in] */ intptr_t localExportSlotsAddr)
        { Assert(false); return E_FAIL; }

    HRESULT SetWellKnownHostTypeId(
        PTHREADCONTEXT_HANDLE threadContextRoot,
        int typeId)
        { Assert(false); return E_FAIL; }

    HRESULT InitializeScriptContext(
        ScriptContextDataIDL * data,
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
        { Assert(false); return E_FAIL; }

    HRESULT CleanupScriptContext(
        __inout PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
        { Assert(false); return E_FAIL; }

    HRESULT CloseScriptContext(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
        { Assert(false); return E_FAIL; }

    HRESULT FreeAllocation(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        intptr_t codeAddress)
        { Assert(false); return E_FAIL; }

    HRESULT SetIsPRNGSeeded(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        boolean value)
        { Assert(false); return E_FAIL; }

    HRESULT IsNativeAddr(
        PTHREADCONTEXT_HANDLE threadContextInfoAddress,
        intptr_t address,
        boolean * result)
        { Assert(false); return E_FAIL; }

    HRESULT RemoteCodeGenCall(
        CodeGenWorkItemIDL *workItemData,
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        JITOutputIDL *jitData)
        { Assert(false); return E_FAIL; }

#if DBG
    HRESULT IsInterpreterThunkAddr(
        PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
        intptr_t address,
        boolean asmjsThunk,
        boolean * result)
        { Assert(false); return E_FAIL; }
#endif

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    static HRESULT DeserializeRPCData(
        _In_reads_(bufferSize) const byte* buffer,
        _In_ uint bufferSize,
        _Out_ CodeGenWorkItemIDL **workItemData
    ) { *workItemData = nullptr; return E_NOTIMPL; }

    static HRESULT SerializeRPCData(
        _In_ CodeGenWorkItemIDL *workItemData,
        _Out_ size_t* bufferSize,
        _Outptr_result_buffer_(*bufferSize) const byte** outBuffer
    ) { *bufferSize = 0; *outBuffer = nullptr; return E_NOTIMPL; }
#endif

    HRESULT Shutdown()
        { Assert(false); return E_FAIL; }

    static JITManager * GetJITManager()
        { return &s_jitManager; }
    static bool HandleServerCallResult(HRESULT hr, RemoteCallType callType) { Assert(UNREACHED); }
private:
    static JITManager s_jitManager;
};
#endif  // !ENABLE_OOP_NATIVE_CODEGEN
