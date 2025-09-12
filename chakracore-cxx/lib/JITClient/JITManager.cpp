//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "JITClientPch.h"
#include "Core/GlobalSecurityPolicy.h"
#include "Core/ConfigFlagsTable.h"

_Must_inspect_result_
_Ret_maybenull_ _Post_writable_byte_size_(size)
void * midl_user_allocate(
    size_t size)
{
    return (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size));
}

void midl_user_free(_Pre_maybenull_ _Post_invalid_ void * ptr)
{
    if (ptr != NULL)
    {
        HeapFree(GetProcessHeap(), NULL, ptr);
    }
}

JITManager JITManager::s_jitManager = JITManager();

JITManager::JITManager() :
    m_rpcBindingHandle(nullptr),
    m_oopJitEnabled(false),
    m_isJITServer(false),
    m_failingHRESULT(S_OK),
    m_jitConnectionId()
{
}

JITManager::~JITManager()
{
    if (m_rpcBindingHandle)
    {
        RpcBindingFree(&m_rpcBindingHandle);
    }
}

/* static */
JITManager * 
JITManager::GetJITManager()
{
    return &s_jitManager;
}

typedef struct _CHAKRA_RPC_SECURITY_QOS_V5 {
    unsigned long Version;
    unsigned long Capabilities;
    unsigned long IdentityTracking;
    unsigned long ImpersonationType;
    unsigned long AdditionalSecurityInfoType;
    union
    {
        RPC_HTTP_TRANSPORT_CREDENTIALS_W* HttpCredentials;
    } u;
    void* Sid;
    unsigned int EffectiveOnly;
    void* ServerSecurityDescriptor;
} CHAKRA_RPC_SECURITY_QOS_V5;

// This routine creates a binding with the server.
int32_t
JITManager::CreateBinding(
    HANDLE serverProcessHandle,
    __in_opt void * serverSecurityDescriptor,
    UUID * connectionUuid,
    RPC_BINDING_HANDLE * bindingHandle)
{
    Assert(IsOOPJITEnabled());

    RPC_STATUS status;
    uint32_t attemptCount = 0;
    uint32_t sleepInterval = 100; // in milliseconds
    RPC_BINDING_HANDLE localBindingHandle;
    RPC_BINDING_HANDLE_TEMPLATE_V1 bindingTemplate;
    RPC_BINDING_HANDLE_SECURITY_V1_W bindingSecurity;

    CHAKRA_RPC_SECURITY_QOS_V5 securityQOS;
    ZeroMemory(&securityQOS, sizeof(CHAKRA_RPC_SECURITY_QOS_V5));
    securityQOS.Capabilities = RPC_C_QOS_CAPABILITIES_DEFAULT;
    securityQOS.IdentityTracking = RPC_C_QOS_IDENTITY_DYNAMIC;
    securityQOS.ImpersonationType = RPC_C_IMP_LEVEL_IDENTIFY;
    securityQOS.Version = AutoSystemInfo::Data.IsWin8OrLater() ? 5 : 4;
    securityQOS.ServerSecurityDescriptor = serverSecurityDescriptor;

    ZeroMemory(&bindingTemplate, sizeof(bindingTemplate));
    bindingTemplate.Version = 1;
    bindingTemplate.ProtocolSequence = RPC_PROTSEQ_LRPC;
    bindingTemplate.StringEndpoint = NULL;
    memcpy_s(&bindingTemplate.ObjectUuid, sizeof(UUID), connectionUuid, sizeof(UUID));
    bindingTemplate.Flags |= RPC_BHT_OBJECT_UUID_VALID;

    ZeroMemory(&bindingSecurity, sizeof(bindingSecurity));
    bindingSecurity.Version = 1;
    bindingSecurity.AuthnLevel = RPC_C_AUTHN_LEVEL_PKT_PRIVACY;
    bindingSecurity.AuthnSvc = RPC_C_AUTHN_KERNEL;
    bindingSecurity.SecurityQos = (RPC_SECURITY_QOS*)&securityQOS;

    status = RpcBindingCreate(&bindingTemplate, &bindingSecurity, NULL, &localBindingHandle);
    if (status != RPC_S_OK)
    {
        return HRESULT_FROM_WIN32(status);
    }

    // We keep attempting to connect to the server with increasing wait intervals in between.
    // This will wait close to 5 minutes before it finally gives up.
    do
    {
        uint32_t waitStatus;

        status = RpcBindingBind(NULL, localBindingHandle, ClientIChakraJIT_v0_0_c_ifspec);
        if (status == RPC_S_OK)
        {
            break;
        }
        else if (status == EPT_S_NOT_REGISTERED)
        {
            // The Server side has not finished registering the RPC Server yet.
            // We should only breakout if we have reached the max attempt count.
            if (attemptCount > 600)
            {
                break;
            }
        }
        else
        {
            // Some unknown error occurred. We are not going to retry for arbitrary errors.
            break;
        }

        // When we come to this point, it means the server has not finished registration yet.
        // We should wait for a while and then reattempt to bind.
        waitStatus = WaitForSingleObject(serverProcessHandle, sleepInterval);
        if (waitStatus == WAIT_OBJECT_0)
        {
            // The server process died for some reason. No need to reattempt.
            status = RPC_S_SERVER_UNAVAILABLE;
            break;
        }
        else if (waitStatus == WAIT_TIMEOUT)
        {
            // Not an error. the server is still alive and we should reattempt.
        }
        else
        {
            Assert(waitStatus == WAIT_FAILED);
#ifdef DBG
            LPWSTR messageBuffer = nullptr;
            uint32_t errorNumber = GetLastError();
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, errorNumber, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
            Output::Print(u"Last error was 0x%x (%s)", errorNumber, messageBuffer);
            LocalFree(messageBuffer);
#endif
            // wait operation failed for an unknown reason.
            Assert(false);
            status = HRESULT_FROM_WIN32(waitStatus);
            break;
        }

        attemptCount++;
        if (sleepInterval < 500)
        {
            sleepInterval += 100;
        }
    } while (status != RPC_S_OK); // redundant check, but compiler would not allow true here.

    *bindingHandle = localBindingHandle;

    return HRESULT_FROM_WIN32(status);
}

bool
JITManager::IsJITServer() const
{
    return m_isJITServer;
}

void
JITManager::SetIsJITServer()
{
    m_isJITServer = true;
    m_oopJitEnabled = true;
}

bool
JITManager::IsConnected() const
{
    Assert(IsOOPJITEnabled());
    return m_rpcBindingHandle != nullptr && !HasJITFailed();
}

void
JITManager::EnableOOPJIT()
{
    m_oopJitEnabled = true;

    if (CONFIG_FLAG(OOPCFGRegistration))
    {
        // Since this client has enabled OOPJIT, perform the one-way policy update
        // that will disable SetProcessValidCallTargets from being invoked.
        GlobalSecurityPolicy::DisableSetProcessValidCallTargets();
    }
}

void
JITManager::SetJITFailed(int32_t hr)
{
    Assert(hr != S_OK);
    m_failingHRESULT = hr;
}

bool
JITManager::HasJITFailed() const
{
    return m_failingHRESULT != S_OK;
}

bool
JITManager::IsOOPJITEnabled() const
{
    return m_oopJitEnabled;
}

#pragma prefast(push)
#pragma prefast(disable:__WARNING_RELEASING_UNHELD_LOCK_MEDIUM_CONFIDENCE, "Lock is correctly acquired and released by RAII class AutoCriticalSection")
#pragma prefast(disable:__WARNING_CALLER_FAILING_TO_HOLD, "Lock is correctly acquired and released by RAII class AutoCriticalSection")
int32_t
JITManager::ConnectRpcServer(HANDLE jitProcessHandle, __in_opt void* serverSecurityDescriptor, UUID connectionUuid)
{
    // We might be trying to connect from multiple threads simultaneously
    AutoCriticalSection cs(&m_cs);

    Assert(IsOOPJITEnabled());
    if (m_rpcBindingHandle != nullptr)
    {
        return S_OK;
    }

    int32_t hr = E_FAIL;

    RPC_BINDING_HANDLE bindingHandle;
    hr = CreateBinding(jitProcessHandle, serverSecurityDescriptor, &connectionUuid, &bindingHandle);
    if (FAILED(hr))
    {
        goto FailureCleanup;
    }


    hr = ConnectProcess(bindingHandle);
    HandleServerCallResult(hr, RemoteCallType::StateUpdate);

    // Only store the binding handle after JIT handshake, so other threads do not prematurely think we are ready to JIT
    m_rpcBindingHandle = bindingHandle;
    m_jitConnectionId = connectionUuid;

    return hr;

FailureCleanup:
    if (m_rpcBindingHandle)
    {
        RpcBindingFree(&m_rpcBindingHandle);
        m_rpcBindingHandle = nullptr;
    }

    return hr;
}
#pragma prefast(pop)

int32_t
JITManager::Shutdown()
{
    // this is special case of shutdown called when runtime process is a parent of the server process
    // used for console host type scenarios
    int32_t hr = S_OK;
    Assert(IsOOPJITEnabled());
    Assert(m_rpcBindingHandle != nullptr);

    RpcTryExcept
    {
        ClientShutdown(m_rpcBindingHandle);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    m_rpcBindingHandle = nullptr;

    return hr;
}

int32_t
JITManager::ConnectProcess(RPC_BINDING_HANDLE rpcBindingHandle)
{
    Assert(IsOOPJITEnabled());
    int32_t hr = E_FAIL;

    if (AutoSystemInfo::Data.IsWin8Point1OrLater())
    {
        HANDLE processHandle = nullptr;
        // RPC handle marshalling is only available on 8.1+
        if (!DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &processHandle, 0, false, DUPLICATE_SAME_ACCESS))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        RpcTryExcept
        {
            hr = ClientConnectProcessWithProcessHandle(
                rpcBindingHandle,
                processHandle,
                (intptr_t)AutoSystemInfo::Data.GetChakraBaseAddr(),
                (intptr_t)AutoSystemInfo::Data.GetCRTHandle());
        }
            RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
        {
            hr = HRESULT_FROM_WIN32(RpcExceptionCode());
        }
        RpcEndExcept;

        if (processHandle)
        {
            CloseHandle(processHandle);
        }
    }
    else
    {
        RpcTryExcept
        {
            hr = ClientConnectProcess(
                rpcBindingHandle,
                (intptr_t)AutoSystemInfo::Data.GetChakraBaseAddr(),
                (intptr_t)AutoSystemInfo::Data.GetCRTHandle());
        }
            RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
        {
            hr = HRESULT_FROM_WIN32(RpcExceptionCode());
        }
        RpcEndExcept;
    }

    return hr;
}

int32_t
JITManager::InitializeThreadContext(
    ThreadContextDataIDL * data,
    PPTHREADCONTEXT_HANDLE threadContextInfoAddress,
    intptr_t * prereservedRegionAddr,
    intptr_t * jitThunkAddr)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientInitializeThreadContext(
            m_rpcBindingHandle,
            data,
            threadContextInfoAddress,
            prereservedRegionAddr,
            jitThunkAddr);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::CleanupThreadContext(
    __inout PPTHREADCONTEXT_HANDLE threadContextInfoAddress)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientCleanupThreadContext(m_rpcBindingHandle, threadContextInfoAddress);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::SetIsPRNGSeeded(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    boolean value)
{
    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientSetIsPRNGSeeded(m_rpcBindingHandle, scriptContextInfoAddress, value);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;

}

int32_t
JITManager::DecommitInterpreterBufferManager(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    boolean asmJsThunk)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientDecommitInterpreterBufferManager(m_rpcBindingHandle, scriptContextInfoAddress, asmJsThunk);
    }
        RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::NewInterpreterThunkBlock(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    InterpreterThunkInputIDL * thunkInput,
    InterpreterThunkOutputIDL * thunkOutput)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientNewInterpreterThunkBlock(m_rpcBindingHandle, scriptContextInfoAddress, thunkInput, thunkOutput);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t 
JITManager::AddModuleRecordInfo(
    /* [in] */ PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    /* [in] */ unsigned int moduleId,
    /* [in] */ intptr_t localExportSlotsAddr)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientAddModuleRecordInfo(m_rpcBindingHandle, scriptContextInfoAddress, moduleId, localExportSlotsAddr);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}


int32_t
JITManager::SetWellKnownHostTypeId(
    PTHREADCONTEXT_HANDLE threadContextRoot,
    int typeId)
{

    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientSetWellKnownHostTypeId(m_rpcBindingHandle, threadContextRoot, typeId);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;

}

int32_t
JITManager::UpdatePropertyRecordMap(
    PTHREADCONTEXT_HANDLE threadContextInfoAddress,
    __in_opt BVSparseNodeIDL * updatedPropsBVHead)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientUpdatePropertyRecordMap(m_rpcBindingHandle, threadContextInfoAddress, updatedPropsBVHead);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::InitializeScriptContext(
    ScriptContextDataIDL * data,
    PTHREADCONTEXT_HANDLE threadContextInfoAddress,
    PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientInitializeScriptContext(m_rpcBindingHandle, data, threadContextInfoAddress, scriptContextInfoAddress);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::CleanupScriptContext(
    __inout PPSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientCleanupScriptContext(m_rpcBindingHandle, scriptContextInfoAddress);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::CloseScriptContext(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientCloseScriptContext(m_rpcBindingHandle, scriptContextInfoAddress);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::FreeAllocation(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    intptr_t codeAddress)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientFreeAllocation(m_rpcBindingHandle, scriptContextInfoAddress, codeAddress);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::IsNativeAddr(
    PTHREADCONTEXT_HANDLE threadContextInfoAddress,
    intptr_t address,
    boolean * result)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientIsNativeAddr(m_rpcBindingHandle, threadContextInfoAddress, address, result);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

int32_t
JITManager::RemoteCodeGenCall(
    CodeGenWorkItemIDL *workItemData,
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    JITOutputIDL *jitData)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientRemoteCodeGen(m_rpcBindingHandle, scriptContextInfoAddress, workItemData, jitData);
    }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}

#if DBG
int32_t
JITManager::IsInterpreterThunkAddr(
    PSCRIPTCONTEXT_HANDLE scriptContextInfoAddress,
    intptr_t address,
    boolean asmjsThunk,
    boolean * result)
{
    Assert(IsOOPJITEnabled());

    int32_t hr = E_FAIL;
    RpcTryExcept
    {
        hr = ClientIsInterpreterThunkAddr(m_rpcBindingHandle, scriptContextInfoAddress, address, asmjsThunk, result);
    }
        RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
    {
        hr = HRESULT_FROM_WIN32(RpcExceptionCode());
    }
    RpcEndExcept;

    return hr;
}
#endif

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
int32_t
JITManager::DeserializeRPCData(
    _In_reads_(bufferSize) const byte* buffer,
    _In_ uint bufferSize,
    _Out_ CodeGenWorkItemIDL **workItemData
)
{
    RPC_STATUS status = RPC_S_OK;
    handle_t marshalHandle = nullptr;
    *workItemData = nullptr;
    __try
    {
        RpcTryExcept
        {
            status = MesDecodeBufferHandleCreate((char*)buffer, bufferSize, &marshalHandle);
            if (status != RPC_S_OK)
            {
                return HRESULT_FROM_WIN32(status);
            }

            pCodeGenWorkItemIDL_Decode(
                marshalHandle,
                workItemData);
        }
        RpcExcept(I_RpcExceptionFilter(RpcExceptionCode()))
        {
            status = RpcExceptionCode();
        }
        RpcEndExcept;
    }
    __finally
    {
        MesHandleFree(marshalHandle);
    }
    return HRESULT_FROM_WIN32(status);
}

int32_t
JITManager::SerializeRPCData(_In_ CodeGenWorkItemIDL *workItemData, _Out_ size_t* bufferSize, _Outptr_result_buffer_(*bufferSize) const byte** outBuffer)
{
    handle_t marshalHandle = nullptr;
    *bufferSize = 0;
    *outBuffer = nullptr;
    RPC_STATUS status = RPC_S_OK;
    __try
    {
        RpcTryExcept
        {
            char* data = nullptr;
            unsigned long encodedSize;
            status = MesEncodeDynBufferHandleCreate(
                &data,
                &encodedSize,
                &marshalHandle);
            if (status != RPC_S_OK)
            {
                return HRESULT_FROM_WIN32(status);
            }

            MIDL_ES_CODE encodeType = MES_ENCODE;
#if TARGET_64
            encodeType = MES_ENCODE_NDR64;
            // We only support encode syntax NDR64, however MesEncodeDynBufferHandleCreate doesn't allow to specify it
            status = MesBufferHandleReset(
                marshalHandle,
                MES_DYNAMIC_BUFFER_HANDLE,
                encodeType,
                &data,
                0,
                &encodedSize
            );
            if (status != RPC_S_OK)
            {
                return HRESULT_FROM_WIN32(status);
            }
#endif

            // Calculate how big we need to create the buffer
            size_t tmpBufSize = pCodeGenWorkItemIDL_AlignSize(marshalHandle, &workItemData);
            size_t alignedBufSize = Math::Align<size_t>(tmpBufSize, 16);
            data = HeapNewNoThrowArray(char, alignedBufSize);
            if (!data)
            {
                // Ran out of memory
                return E_OUTOFMEMORY;
            }

            // Reset the buffer handle to a fixed buffer
            status = MesBufferHandleReset(
                marshalHandle,
                MES_FIXED_BUFFER_HANDLE,
                encodeType,
                &data,
                (unsigned long)alignedBufSize,
                &encodedSize
            );
            if (status != RPC_S_OK)
            {
                return HRESULT_FROM_WIN32(status);
            }

            pCodeGenWorkItemIDL_Encode(
                marshalHandle,
                &workItemData);
            *bufferSize = alignedBufSize;
            *outBuffer = (byte*)data;
        }
        RpcExcept(I_RpcExceptionFilter(RpcExceptionCode()))
        {
            status = RpcExceptionCode();
        }
        RpcEndExcept;
    }
    __finally
    {
        MesHandleFree(marshalHandle);
    }

    return HRESULT_FROM_WIN32(status);
}
#endif
