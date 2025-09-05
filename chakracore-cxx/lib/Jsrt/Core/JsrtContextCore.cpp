//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Runtime.h"

#include "Library/DelayFreeArrayBufferHelper.h"
#include "Base/ThreadContext.h"

#include "Base/StackProber.h"
#include "Base/ScriptContextProfiler.h"

#include "Language/JavascriptConversion.h"

#include "Base/ScriptContextOptimizationOverrideInfo.h"
#include "Base/ScriptContextBase.h"
#include "Base/ScriptContextInfo.h"
#include "Base/ScriptContext.h"
#include "Base/LeaveScriptObject.h"
#include "Base/PropertyRecord.h"

#include "ByteCode/ByteCodeReader.h"
#include "Language/TaggedInt.h"

#include "Library/RootObjectBase.h"
#include "Library/GlobalObject.h"

#include "Library/LiteralString.h"
#include "Library/ConcatString.h"
#include "Library/CompoundString.h"
#include "Library/PropertyRecordUsageCache.h"
#include "Library/PropertyString.h"
#include "Library/SingleCharString.h"

#include "Library/JavascriptTypedNumber.h"
#include "Library/SparseArraySegment.h"
#include "Library/JavascriptError.h"
#include "Library/JavascriptArray.h"
#include "Library/JavascriptSymbol.h"

#include "Library/AtomicsObject.h"
#include "Library/DetachedStateBase.h"
#include "Library/ArrayBuffer.h"
#include "Library/SharedArrayBuffer.h"
#include "Library/TypedArray.h"
#include "Library/JavascriptBoolean.h"
#include "Library/JavascriptBigInt.h"
#include "Library/WebAssemblyEnvironment.h"
#include "Library/WebAssemblyTable.h"
#include "Library/WebAssemblyMemory.h"
#include "Library/WebAssemblyModule.h"
#include "Library/WebAssembly.h"
#include "JsrtContext.h"
#include "SCACorePch.h"
#include "JsrtContextCore.h"

JsrtContext *JsrtContext::New(JsrtRuntime * runtime)
{
    return JsrtContextCore::New(runtime);
}

/* static */
bool JsrtContext::Is(void * ref)
{
    return VirtualTableInfo<JsrtContextCore>::HasVirtualTable(ref);
}

void JsrtContext::OnScriptLoad(Js::JavascriptFunction * scriptFunction, Js::Utf8SourceInfo* utf8SourceInfo, CompileScriptException* compileException)
{
    ((JsrtContextCore *)this)->OnScriptLoad(scriptFunction, utf8SourceInfo, compileException);
}

#if ENABLE_TTD
void JsrtContext::OnScriptLoad_TTDCallback(Js::FunctionBody* body, Js::Utf8SourceInfo* utf8SourceInfo, CompileScriptException* compileException, bool notify)
{
    JsrtContextCore* rcvr = ((JsrtContextCore*)this);

    JsrtDebugManager* jsrtDebugManager = rcvr->GetRuntime()->GetJsrtDebugManager();
    if(jsrtDebugManager != nullptr)
    {
        jsrtDebugManager->ReportScriptCompile_TTD(body, utf8SourceInfo, compileException, notify);
    }
}

void JsrtContext::OnReplayDisposeContext_TTDCallback(FinalizableObject* jsrtCtx)
{
    ((JsrtContextCore *)jsrtCtx)->Dispose(false);
}
#endif

JsrtContextCore::JsrtContextCore(JsrtRuntime * runtime) :
    JsrtContext(runtime)
{
    EnsureScriptContext();
    Link();
}

/* static */
JsrtContextCore *JsrtContextCore::New(JsrtRuntime * runtime)
{
    return RecyclerNewFinalized(runtime->GetThreadContext()->EnsureRecycler(), JsrtContextCore, runtime);
}

void JsrtContextCore::Dispose(bool isShutdown)
{
    if (this->GetJavascriptLibrary())
    {
        Unlink();
        this->SetJavascriptLibrary(nullptr);
    }
}

void JsrtContextCore::Finalize(bool isShutdown)
{
}

Js::ScriptContext* JsrtContextCore::EnsureScriptContext()
{
    Assert(this->GetJavascriptLibrary() == nullptr);

    ThreadContext* localThreadContext = this->GetRuntime()->GetThreadContext();

    AutoPtr<Js::ScriptContext> newScriptContext(Js::ScriptContext::New(localThreadContext));

    newScriptContext->Initialize();

    hostContext = HeapNew(ChakraCoreHostScriptContext, newScriptContext);
    newScriptContext->SetHostScriptContext(hostContext);

    this->SetJavascriptLibrary(newScriptContext.Detach()->GetLibrary());

    Js::JavascriptLibrary *library = this->GetScriptContext()->GetLibrary();
    Assert(library != nullptr);
    localThreadContext->GetRecycler()->RootRelease(library->GetGlobalObject());

    library->GetEvalFunctionObject()->SetEntryPoint(&Js::GlobalObject::EntryEval);
    library->GetFunctionConstructor()->SetEntryPoint(&Js::JavascriptFunction::NewInstance);

    return this->GetScriptContext();
}

void JsrtContextCore::OnScriptLoad(Js::JavascriptFunction * scriptFunction, Js::Utf8SourceInfo* utf8SourceInfo, CompileScriptException* compileException)
{
#ifdef ENABLE_SCRIPT_DEBUGGING
    JsrtDebugManager* jsrtDebugManager = this->GetRuntime()->GetJsrtDebugManager();
    if (jsrtDebugManager != nullptr)
    {
        jsrtDebugManager->ReportScriptCompile(scriptFunction, utf8SourceInfo, compileException);
    }
#endif
}

int32_t ChakraCoreHostScriptContext::FetchImportedModule(Js::ModuleRecordBase* referencingModule, LPCOLESTR specifier, Js::ModuleRecordBase** dependentModuleRecord)
{
    if (fetchImportedModuleCallback == nullptr)
    {
        return E_INVALIDARG;
    }
    Js::JavascriptString* specifierVar = Js::JavascriptString::NewCopySz(specifier, GetScriptContext());
    JsModuleRecord dependentRecord = JS_INVALID_REFERENCE;
    {
        AUTO_NO_EXCEPTION_REGION;
        JsErrorCode errorCode = fetchImportedModuleCallback(referencingModule, specifierVar, &dependentRecord);
        if (errorCode == JsNoError)
        {
            *dependentModuleRecord = static_cast<Js::ModuleRecordBase*>(dependentRecord);
            return NOERROR;
        }
    }
    return E_INVALIDARG;
}

int32_t ChakraCoreHostScriptContext::FetchImportedModuleFromScript(JsSourceContext dwReferencingSourceContext, LPCOLESTR specifier, Js::ModuleRecordBase** dependentModuleRecord)
{
    if (fetchImportedModuleFromScriptCallback == nullptr)
    {
        return E_INVALIDARG;
    }
    Js::JavascriptString* specifierVar = Js::JavascriptString::NewCopySz(specifier, GetScriptContext());
    JsModuleRecord dependentRecord = JS_INVALID_REFERENCE;
    {
        AUTO_NO_EXCEPTION_REGION;
        JsErrorCode errorCode = fetchImportedModuleFromScriptCallback(dwReferencingSourceContext, specifierVar, &dependentRecord);
        if (errorCode == JsNoError)
        {
            *dependentModuleRecord = static_cast<Js::ModuleRecordBase*>(dependentRecord);
            return NOERROR;
        }
    }
    return E_INVALIDARG;
}

int32_t ChakraCoreHostScriptContext::NotifyHostAboutModuleReady(Js::ModuleRecordBase* referencingModule, Js::Var exceptionVar)
{
    if (notifyModuleReadyCallback == nullptr)
    {
        return E_INVALIDARG;
    }
    {
        AUTO_NO_EXCEPTION_REGION;
        JsErrorCode errorCode = notifyModuleReadyCallback(referencingModule, exceptionVar);
        if (errorCode == JsNoError)
        {
            return NOERROR;
        }
    }
    return E_INVALIDARG;
}

int32_t ChakraCoreHostScriptContext::InitializeImportMeta(Js::ModuleRecordBase* referencingModule, Js::Var importMetaObject)
{
    if (initializeImportMetaCallback == nullptr)
    {
        return E_INVALIDARG;
    }
    {
        AUTO_NO_EXCEPTION_REGION;
        JsErrorCode errorCode = initializeImportMetaCallback(referencingModule, importMetaObject);
        if (errorCode == JsNoError)
        {
            return NOERROR;
        }
    }
    return E_INVALIDARG;
}

bool ChakraCoreHostScriptContext::ReportModuleCompletion(Js::ModuleRecordBase* module, Js::Var exception)
{
    if (reportModuleCompletionCallback == nullptr)
    {
        return false;
    }
    {
        AUTO_NO_EXCEPTION_REGION;
        JsErrorCode errorCode = reportModuleCompletionCallback(module, exception);
        if (errorCode == JsNoError)
        {
            return true;
        }
    }
    return false;
}

ChakraCoreStreamWriter::~ChakraCoreStreamWriter()
{
    HeapDelete(m_serializerCore);
}

byte * ChakraCoreStreamWriter::ExtendBuffer(byte *oldBuffer, size_t newSize, size_t *allocatedSize)
{
    m_data = this->reallocateBufferMemory(this->callbackState, oldBuffer, newSize, allocatedSize);
    m_length = newSize;

    if (m_data == nullptr)
    {
        // free(m_data);
        OutOfMemory_unrecoverable_error();
    }
    return m_data;
}

bool ChakraCoreStreamWriter::WriteHostObject(void* data)
{
    return this->writeHostObject(this->callbackState, data);
}

void ChakraCoreStreamWriter::SetSerializer(Js::SCACore::Serializer *s)
{
    m_serializerCore = s;
}

void ChakraCoreStreamWriter::WriteRawBytes(const void* source, size_t length)
{
    Assert(m_serializerCore);
    m_serializerCore->WriteRawBytes(source, length);
}

bool ChakraCoreStreamWriter::WriteValue(JsValueRef root)
{
    Assert(m_serializerCore);
    return m_serializerCore->WriteValue((Js::Var)root);
}

bool ChakraCoreStreamWriter::ReleaseData(byte** data, size_t *dataLength)
{
    if (m_data)
    {
        Assert(m_serializerCore);
        return m_serializerCore->Release(data, dataLength);
    }
    return false;
}

bool ChakraCoreStreamWriter::DetachArrayBuffer()
{
    Assert(m_serializerCore);
    return m_serializerCore->DetachArrayBuffer();
}

JsErrorCode ChakraCoreStreamWriter::SetTransferableVars(JsValueRef *transferableVars, size_t transferableVarsCount)
{
    Assert(m_serializerCore);
    int32_t hr = m_serializerCore->SetTransferableVars((Js::Var *)transferableVars, transferableVarsCount);
    if (hr == S_OK)
    {
        return JsNoError;
    }
    else if (hr == E_SCA_TRANSFERABLE_UNSUPPORTED)
    {
        return JsTransferableNotSupported;
    }
    else if (hr == E_SCA_TRANSFERABLE_NEUTERED)
    {
        return JsTransferableAlreadyDetached;
    }
    return JsSerializerNotSupported;
}

void ChakraCoreStreamWriter::FreeSelf()
{
    HeapDelete(this);
}

ChakraHostDeserializerHandle::~ChakraHostDeserializerHandle()
{
    HeapDelete(m_deserializer);
}

void ChakraHostDeserializerHandle::SetDeserializer(Js::SCACore::Deserializer *deserializer)
{
    m_deserializer = deserializer;
}

bool ChakraHostDeserializerHandle::ReadRawBytes(size_t length, void **data)
{
    Assert(m_deserializer);
    return m_deserializer->ReadRawBytes(length, data);
}

bool ChakraHostDeserializerHandle::ReadBytes(size_t length, void **data)
{
    Assert(m_deserializer);
    return m_deserializer->ReadBytes(length, data);
}

JsValueRef ChakraHostDeserializerHandle::ReadValue()
{
    Assert(m_deserializer);
    return m_deserializer->ReadValue();
}

JsErrorCode ChakraHostDeserializerHandle::SetTransferableVars(JsValueRef *transferableVars, size_t transferableVarsCount)
{
    Assert(m_deserializer);
    int32_t hr = m_deserializer->SetTransferableVars((Js::Var *)transferableVars, transferableVarsCount);
    if (hr == S_OK)
    {
        return JsNoError;
    }
    else if (hr == E_SCA_TRANSFERABLE_UNSUPPORTED)
    {
        return JsTransferableNotSupported;
    }
    else if (hr == E_SCA_TRANSFERABLE_NEUTERED)
    {
        return JsTransferableAlreadyDetached;
    }
    return JsSerializerNotSupported;
}


void ChakraHostDeserializerHandle::FreeSelf()
{
    HeapDelete(this);
}

Js::Var ChakraHostDeserializerHandle::ReadHostObject()
{
    return (Js::Var)this->readHostObject(this->callbackState);
}

