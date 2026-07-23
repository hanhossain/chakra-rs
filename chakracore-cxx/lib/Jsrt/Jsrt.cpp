//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Library/JavascriptProxy.h"
#include "JsrtInternal.h"
#include "JsrtExternalObject.h"
#include "JsrtExternalArrayBuffer.h"
#include "jsrtHelper.h"

#include "JsrtSourceHolder.h"
#include "ByteCode/ByteCodeSerializer.h"
#include "Common/ByteSwap.h"
#include "Library/DataView.h"
#include "Base/ThreadContextTlsEntry.h"
#include "Codex/Utf8Helper.h"

// Parser Includes
#include "cmperr.h"     // For ERRnoMemory
#include "screrror.h"   // For CompileScriptException

#include "TestHooksRt.h"

using namespace chakracore::jsrt;

JsErrorCode RunScriptWithParserStateCore(
    _In_ uint32_t dwBgParseCookie,
    _In_ JsValueRef script,
    _In_ JsSourceContext sourceContext,
    _In_ char16_t *url,
    _In_ JsParseScriptAttributes parseAttributes,
    _In_ JsValueRef parserState,
    _In_ bool parseOnly,
    _Out_ JsValueRef *result
);

struct CodexHeapAllocatorInterface
{
public:
    static void* allocate(size_t size)
    {
        return HeapNewArray(char, size);
    }

    static void free(void* ptr, size_t count)
    {
        HeapDeleteArray(count, (char*) ptr);
    }
};

JsErrorCode CheckContext(JsrtContext *currentContext, bool verifyRuntimeState,
    bool allowInObjectBeforeCollectCallback)
{
    if (currentContext == nullptr)
    {
        return JsErrorNoCurrentContext;
    }

    // We don't need parameter check if it's checked in previous wrapper.
    if (verifyRuntimeState)
    {
        Js::ScriptContext *scriptContext = currentContext->GetScriptContext();
        Assert(scriptContext != nullptr);
        Recycler *recycler = scriptContext->GetRecycler();
        ThreadContext *threadContext = scriptContext->GetThreadContext();

        if (recycler && recycler->IsHeapEnumInProgress())
        {
            return JsErrorHeapEnumInProgress;
        }
        else if (!allowInObjectBeforeCollectCallback &&
            recycler && recycler->IsInObjectBeforeCollectCallback())
        {
            return JsErrorInObjectBeforeCollectCallback;
        }
        else if (threadContext->IsExecutionDisabled())
        {
            return JsErrorInDisabledState;
        }
        else if (scriptContext->IsInProfileCallback())
        {
            return JsErrorInProfileCallback;
        }
        else if (threadContext->IsInThreadServiceCallback())
        {
            return JsErrorInThreadServiceCallback;
        }

        // Make sure we don't have an outstanding exception.
        if (scriptContext->GetThreadContext()->GetRecordedException() != nullptr)
        {
            return JsErrorInExceptionState;
        }
    }

    return JsNoError;
}

/////////////////////

//A create context function that we can funnel to for regular and record or debug aware creation
JsErrorCode CreateContextCore(_In_ JsRuntimeHandle runtimeHandle, _In_ bool inRecordMode, _In_ bool activelyRecording, _In_ bool inReplayMode, _Out_ JsContextRef *newContext)
{
    JsrtRuntime * runtime = JsrtRuntime::FromHandle(runtimeHandle);
    ThreadContext * threadContext = runtime->GetThreadContext();

    if(threadContext->GetRecycler() && threadContext->GetRecycler()->IsHeapEnumInProgress())
    {
        return JsErrorHeapEnumInProgress;
    }
    else if(threadContext->IsInThreadServiceCallback())
    {
        return JsErrorInThreadServiceCallback;
    }

    ThreadContextScope scope(threadContext);

    if(!scope.IsValid())
    {
        return JsErrorWrongThread;
    }

    JsrtContext * context = JsrtContext::New(runtime);

#ifdef ENABLE_SCRIPT_DEBUGGING
    JsrtDebugManager* jsrtDebugManager = runtime->GetJsrtDebugManager();

    if(jsrtDebugManager != nullptr)
    {
        // JsDiagStartDebugging was called
        threadContext->GetDebugManager()->SetLocalsDisplayFlags(Js::DebugManager::LocalsDisplayFlags::LocalsDisplayFlags_NoGroupMethods);

        Js::ScriptContext* scriptContext = context->GetScriptContext();

        Js::DebugContext* debugContext = scriptContext->GetDebugContext();
        debugContext->SetHostDebugContext(jsrtDebugManager);

        if (!jsrtDebugManager->IsDebugEventCallbackSet())
        {
            // JsDiagStopDebugging was called so we need to be in SourceRunDownMode
            debugContext->SetDebuggerMode(Js::DebuggerMode::SourceRundown);
        }
        else
        {
            // Set Debugging mode
            scriptContext->InitializeDebugging();
            Js::ProbeContainer* probeContainer = debugContext->GetProbeContainer();
            probeContainer->InitializeInlineBreakEngine(jsrtDebugManager);
            probeContainer->InitializeDebuggerScriptOptionCallback(jsrtDebugManager);
        }
    }
#endif

    *newContext = (JsContextRef)context;
    return JsNoError;
}

//A create runtime function that we can funnel to for regular and record or debug aware creation
JsErrorCode CreateRuntimeCore(_In_ JsRuntimeAttributes attributes,
                              _In_opt_ JsThreadServiceCallback threadService,
                              _Out_ JsRuntimeHandle *runtimeHandle)
{
    VALIDATE_ENTER_CURRENT_THREAD();

    PARAM_NOT_NULL(runtimeHandle);
    *runtimeHandle = nullptr;

    JsErrorCode runtimeResult = GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        const JsRuntimeAttributes JsRuntimeAttributesAll =
            (JsRuntimeAttributes)(
            JsRuntimeAttributeDisableBackgroundWork |
            JsRuntimeAttributeAllowScriptInterrupt |
            JsRuntimeAttributeEnableIdleProcessing |
            JsRuntimeAttributeDisableEval |
            JsRuntimeAttributeDisableNativeCodeGeneration |
            JsRuntimeAttributeDisableExecutablePageAllocation |
            JsRuntimeAttributeEnableExperimentalFeatures |
            JsRuntimeAttributeDispatchSetExceptionsToDebugger |
            JsRuntimeAttributeDisableFatalOnOOM
            | JsRuntimeAttributeSerializeLibraryByteCode
        );

        Assert((attributes & ~JsRuntimeAttributesAll) == 0);
        if ((attributes & ~JsRuntimeAttributesAll) != 0)
        {
            return JsErrorInvalidArgument;
        }
        CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 0, nullptr);
        AllocationPolicyManager * policyManager = HeapNew(AllocationPolicyManager, (attributes & JsRuntimeAttributeDisableBackgroundWork) == 0);
        bool enableExperimentalFeatures = (attributes & JsRuntimeAttributeEnableExperimentalFeatures) != 0;
        ThreadContext * threadContext = HeapNew(ThreadContext, policyManager, threadService, enableExperimentalFeatures);

        if ((attributes & JsRuntimeAttributeDisableBackgroundWork) != 0)
        {
            threadContext->OptimizeForManyInstances(true);
#if ENABLE_NATIVE_CODEGEN
            threadContext->EnableBgJit(false);
#endif
        }

        if (!threadContext->IsRentalThreadingEnabledInJSRT()
            || Js::Configuration::Global.flags.DisableRentalThreading
            )
        {
            threadContext->SetIsThreadBound();
        }

        if (attributes & JsRuntimeAttributeAllowScriptInterrupt)
        {
            threadContext->SetThreadContextFlag(ThreadContextFlagCanDisableExecution);
        }

        if (attributes & JsRuntimeAttributeDisableEval)
        {
            threadContext->SetThreadContextFlag(ThreadContextFlagEvalDisabled);
        }

        if (attributes & JsRuntimeAttributeDisableNativeCodeGeneration)
        {
            threadContext->SetThreadContextFlag(ThreadContextFlagNoJIT);
        }

        if (attributes & JsRuntimeAttributeDisableExecutablePageAllocation)
        {
            threadContext->SetThreadContextFlag(ThreadContextFlagNoJIT);
            threadContext->SetThreadContextFlag(ThreadContextFlagNoDynamicThunks);
        }

        if (attributes & JsRuntimeAttributeDisableFatalOnOOM)
        {
            threadContext->SetThreadContextFlag(ThreadContextFlagDisableFatalOnOOM);
        }

        if (Js::Configuration::Global.flags.PrimeRecycler)
        {
            threadContext->EnsureRecycler()->Prime();
        }

        bool enableIdle = (attributes & JsRuntimeAttributeEnableIdleProcessing) == JsRuntimeAttributeEnableIdleProcessing;
        bool dispatchExceptions = (attributes & JsRuntimeAttributeDispatchSetExceptionsToDebugger) == JsRuntimeAttributeDispatchSetExceptionsToDebugger;

        JsrtRuntime * runtime = HeapNew(JsrtRuntime, threadContext, enableIdle, dispatchExceptions);
        threadContext->SetCurrentThreadId(ThreadContext::NoThread);
        *runtimeHandle = runtime->ToHandle();

        return JsNoError;
    });

    return runtimeResult;
}

/////////////////////

JsErrorCode chakracore::jsrt::JsCreateRuntime(_In_ JsRuntimeAttributes attributes, _In_opt_ JsThreadServiceCallback threadService, _Out_ JsRuntimeHandle *runtimeHandle)
{
    return CreateRuntimeCore(attributes,
                             threadService, runtimeHandle
                             /*optSnapInterval*/
                             /*optLogLength*/);
}

template <CollectionFlags flags>
JsErrorCode JsCollectGarbageCommon(JsRuntimeHandle runtimeHandle)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

        ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();

        if (threadContext->GetRecycler() && threadContext->GetRecycler()->IsHeapEnumInProgress())
        {
            return JsErrorHeapEnumInProgress;
        }
        else if (threadContext->IsInThreadServiceCallback())
        {
            return JsErrorInThreadServiceCallback;
        }

        ThreadContextScope scope(threadContext);

        if (!scope.IsValid())
        {
            return JsErrorWrongThread;
        }

        Recycler* recycler = threadContext->EnsureRecycler();
        if (flags & CollectOverride_SkipStack)
        {
            Recycler::AutoEnterExternalStackSkippingGCMode autoGC(recycler);
            recycler->CollectNow<flags>();
        }
        else
        {
            recycler->CollectNow<flags>();
        }
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCollectGarbage(_In_ JsRuntimeHandle runtimeHandle)
{
    return JsCollectGarbageCommon<CollectNowExhaustive>(runtimeHandle);
}

JsErrorCode JsPrivateCollectGarbageSkipStack(_In_ JsRuntimeHandle runtimeHandle)
{
    return JsCollectGarbageCommon<CollectNowExhaustiveSkipStack>(runtimeHandle);
}

JsErrorCode JsPrivateDetachArrayBuffer(_In_ JsValueRef ref, _Out_ void** detachedState)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode
    {
        VALIDATE_JSREF(ref);
        *detachedState = Js::JavascriptOperators::DetachVarAndGetState(ref, false /*queueForDelayFree*/);
        return JsNoError;
    });
}

JsErrorCode JsPrivateFreeDetachedArrayBuffer(_In_ void* detachedState)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode
    {
        auto state = reinterpret_cast<Js::ArrayBufferDetachedStateBase*>(detachedState);
        state->CleanUp();
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsDisposeRuntime(_In_ JsRuntimeHandle runtimeHandle)
{
    return GlobalAPIWrapper_NoRecord([&] () -> JsErrorCode {
        VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

        JsrtRuntime * runtime = JsrtRuntime::FromHandle(runtimeHandle);
        ThreadContext * threadContext = runtime->GetThreadContext();
        ThreadContextScope scope(threadContext);

        // We should not dispose if the runtime is being used.
        if (!scope.IsValid() ||
            scope.WasInUse() ||
            (threadContext->GetRecycler() && threadContext->GetRecycler()->IsHeapEnumInProgress()))
        {
            return JsErrorRuntimeInUse;
        }
        else if (threadContext->IsInThreadServiceCallback())
        {
            return JsErrorInThreadServiceCallback;
        }

        // Invoke and clear the callbacks while the contexts and runtime are still available
        {
            Recycler* recycler = threadContext->GetRecycler();
            if (recycler != nullptr)
            {
                recycler->ClearObjectBeforeCollectCallbacks();
            }
        }
#ifdef ENABLE_SCRIPT_DEBUGGING
        if (runtime->GetJsrtDebugManager() != nullptr)
        {
            runtime->GetJsrtDebugManager()->ClearDebuggerObjects();
        }
#endif
        Js::ScriptContext *scriptContext;
        for (scriptContext = threadContext->GetScriptContextList(); scriptContext; scriptContext = scriptContext->next)
        {
#ifdef ENABLE_SCRIPT_DEBUGGING
            if (runtime->GetJsrtDebugManager() != nullptr)
            {
                runtime->GetJsrtDebugManager()->ClearDebugDocument(scriptContext);
            }
#endif
            scriptContext->MarkForClose();
        }

        // Close any open Contexts.
        // We need to do this before recycler shutdown, because ScriptEngine->Close won't work then.
        runtime->CloseContexts();

#ifdef ENABLE_SCRIPT_DEBUGGING
        runtime->DeleteJsrtDebugManager();
#endif

        runtime->SetBeforeCollectCallback(nullptr, nullptr);
        threadContext->CloseForJSRT();
        HeapDelete(threadContext);

        HeapDelete(runtime);

        scope.Invalidate();

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsAddRef(_In_ JsRef ref, _Out_opt_ unsigned int *count)
{
    VALIDATE_JSREF(ref);
    if (count != nullptr)
    {
        *count = 0;
    }

    if (Js::TaggedNumber::Is(ref))
    {
        // The count is always one because these are never collected
        if (count)
        {
            *count = 1;
        }
        return JsNoError;
    }

    if (JsrtContext::Is(ref))
    {
        return GlobalAPIWrapper_NoRecord([&] () -> JsErrorCode
        {
            Recycler * recycler = static_cast<JsrtContext *>(ref)->GetRuntime()->GetThreadContext()->GetRecycler();
            recycler->RootAddRef(ref, count);
            return JsNoError;
        });
    }
    else
    {
        ThreadContext* threadContext = ThreadContext::GetContextForCurrentThread();
        if (threadContext == nullptr)
        {
            return JsErrorNoCurrentContext;
        }
        Recycler * recycler = threadContext->GetRecycler();
        return GlobalAPIWrapper([&] () -> JsErrorCode
        {
            // Note, some references may live in arena-allocated memory, so we need to do this check
            if (!recycler->IsValidObject(ref))
            {
                return JsNoError;
            }

            recycler->RootAddRef(ref, count);
            return JsNoError;
        });
    }
}

JsErrorCode chakracore::jsrt::JsRelease(_In_ JsRef ref, _Out_opt_ unsigned int *count)
{
    VALIDATE_JSREF(ref);
    if (count != nullptr)
    {
        *count = 0;
    }

    if (Js::TaggedNumber::Is(ref))
    {
        // The count is always one because these are never collected
        if (count)
        {
            *count = 1;
        }
        return JsNoError;
    }

    if (JsrtContext::Is(ref))
    {
        return GlobalAPIWrapper_NoRecord([&] () -> JsErrorCode
        {
            Recycler * recycler = static_cast<JsrtContext *>(ref)->GetRuntime()->GetThreadContext()->GetRecycler();
            recycler->RootRelease(ref, count);
            return JsNoError;
        });
    }
    else
    {
        ThreadContext* threadContext = ThreadContext::GetContextForCurrentThread();
        if (threadContext == nullptr)
        {
            return JsErrorNoCurrentContext;
        }
        Recycler * recycler = threadContext->GetRecycler();
        return GlobalAPIWrapper([&]() -> JsErrorCode
        {
            // Note, some references may live in arena-allocated memory, so we need to do this check
            if (!recycler->IsValidObject(ref))
            {
                return JsNoError;
            }

            recycler->RootRelease(ref, count);

            return JsNoError;
        });
    }
}

JsErrorCode chakracore::jsrt::JsSetObjectBeforeCollectCallback(_In_ JsRef ref, _In_opt_ void *callbackState, _In_ JsObjectBeforeCollectCallback objectBeforeCollectCallback)
{
    VALIDATE_JSREF(ref);

    if (Js::TaggedNumber::Is(ref))
    {
        return JsErrorInvalidArgument;
    }

    if (JsrtContext::Is(ref))
    {
        return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode
        {
            ThreadContext* threadContext = static_cast<JsrtContext *>(ref)->GetRuntime()->GetThreadContext();
            Recycler * recycler = threadContext->GetRecycler();
            recycler->SetObjectBeforeCollectCallback(ref, reinterpret_cast<Recycler::ObjectBeforeCollectCallback>(objectBeforeCollectCallback), callbackState,
                reinterpret_cast<Recycler::ObjectBeforeCollectCallbackWrapper>(JsrtCallbackState::ObjectBeforeCallectCallbackWrapper), threadContext);
            return JsNoError;
        });
    }
    else
    {
        ThreadContext* threadContext = ThreadContext::GetContextForCurrentThread();
        if (threadContext == nullptr)
        {
            return JsErrorNoCurrentContext;
        }
        Recycler * recycler = threadContext->GetRecycler();
        return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode
        {
            if (!recycler->IsValidObject(ref))
            {
                return JsErrorInvalidArgument;
            }

            recycler->SetObjectBeforeCollectCallback(ref, reinterpret_cast<Recycler::ObjectBeforeCollectCallback>(objectBeforeCollectCallback), callbackState,
                reinterpret_cast<Recycler::ObjectBeforeCollectCallbackWrapper>(JsrtCallbackState::ObjectBeforeCallectCallbackWrapper), threadContext);
            return JsNoError;
        });
    }
}

JsErrorCode chakracore::jsrt::JsCreateContext(_In_ JsRuntimeHandle runtimeHandle, _Out_ JsContextRef *newContext)
{
    return GlobalAPIWrapper([&]() -> JsErrorCode {
        PARAM_NOT_NULL(newContext);
        VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

        bool inRecord = false;
        bool activelyRecording = false;
        bool inReplay = false;

        return CreateContextCore(runtimeHandle, inRecord, activelyRecording, inReplay, newContext);
    });
}

JsErrorCode chakracore::jsrt::JsGetCurrentContext(_Out_ JsContextRef *currentContext)
{
    PARAM_NOT_NULL(currentContext);

    BEGIN_JSRT_NO_EXCEPTION
    {
      *currentContext = (JsContextRef)JsrtContext::GetCurrent();
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsSetCurrentContext(_In_opt_ JsContextRef newContext)
{
    VALIDATE_ENTER_CURRENT_THREAD();

    return GlobalAPIWrapper([&] () -> JsErrorCode {
        JsrtContext *currentContext = JsrtContext::GetCurrent();
        Recycler* recycler = currentContext != nullptr ? currentContext->GetScriptContext()->GetRecycler() : nullptr;

        if (currentContext && recycler->IsHeapEnumInProgress())
        {
            return JsErrorHeapEnumInProgress;
        }
        else if (currentContext && currentContext->GetRuntime()->GetThreadContext()->IsInThreadServiceCallback())
        {
            return JsErrorInThreadServiceCallback;
        }

        if (!JsrtContext::TrySetCurrent((JsrtContext *)newContext))
        {
            return JsErrorWrongThread;
        }

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetContextOfObject(_In_ JsValueRef object, _Out_ JsContextRef *context)
{
    VALIDATE_JSREF(object);
    PARAM_NOT_NULL(context);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::VarIs<Js::RecyclableObject>(object))
        {
            RETURN_NO_EXCEPTION(JsErrorArgumentNotObject);
        }
        Js::RecyclableObject* obj = Js::VarTo<Js::RecyclableObject>(object);
        *context = (JsContextRef)obj->GetScriptContext()->GetLibrary()->GetJsrtContext();
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetContextData(_In_ JsContextRef context, _Out_ void **data)
{
    VALIDATE_JSREF(context);
    PARAM_NOT_NULL(data);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!JsrtContext::Is(context))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        *data = static_cast<JsrtContext *>(context)->GetExternalData();
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsSetContextData(_In_ JsContextRef context, _In_ void *data)
{
    VALIDATE_JSREF(context);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!JsrtContext::Is(context))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        static_cast<JsrtContext *>(context)->SetExternalData(data);
    }
    END_JSRT_NO_EXCEPTION
}

void HandleScriptCompileError(Js::ScriptContext * scriptContext, CompileScriptException * se, const char16_t * sourceUrl)
{
    int32_t hr = se->ei.scode;
    if (hr == E_OUTOFMEMORY || hr == VBSERR_OutOfMemory || hr == ERRnoMemory)
    {
        Js::Throw::OutOfMemory();
    }
    else if (hr == E_ABORT)
    {
        Js::JavascriptOperators::ScriptAbort();
    }

    Js::JavascriptError* error = Js::JavascriptError::CreateFromCompileScriptException(scriptContext, se, sourceUrl);

    Js::JavascriptExceptionObject * exceptionObject = RecyclerNew(scriptContext->GetRecycler(),
        Js::JavascriptExceptionObject, error, scriptContext, nullptr);

    scriptContext->GetThreadContext()->SetRecordedException(exceptionObject);
}

JsErrorCode chakracore::jsrt::JsGetUndefinedValue(_Out_ JsValueRef *undefinedValue)
{
    return ContextAPINoScriptWrapper_NoRecord([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(undefinedValue);

        *undefinedValue = scriptContext->GetLibrary()->GetUndefined();

        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsGetNullValue(_Out_ JsValueRef *nullValue)
{
    return ContextAPINoScriptWrapper_NoRecord([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(nullValue);

        *nullValue = scriptContext->GetLibrary()->GetNull();

        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsGetTrueValue(_Out_ JsValueRef *trueValue)
{
    return ContextAPINoScriptWrapper_NoRecord([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(trueValue);

        *trueValue = scriptContext->GetLibrary()->GetTrue();

        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsGetFalseValue(_Out_ JsValueRef *falseValue)
{
    return ContextAPINoScriptWrapper_NoRecord([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(falseValue);

        *falseValue = scriptContext->GetLibrary()->GetFalse();

        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsBoolToBoolean(_In_ bool value, _Out_ JsValueRef *booleanValue)
{
    return ContextAPINoScriptWrapper([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(booleanValue);

        *booleanValue = value ? scriptContext->GetLibrary()->GetTrue() : scriptContext->GetLibrary()->GetFalse();

        ;

        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsBooleanToBool(_In_ JsValueRef value, _Out_ bool *boolValue)
{
    VALIDATE_JSREF(value);
    PARAM_NOT_NULL(boolValue);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::VarIs<Js::JavascriptBoolean>(value))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        *boolValue = Js::VarTo<Js::JavascriptBoolean>(value)->GetValue() ? true : false;
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsConvertValueToBoolean(_In_ JsValueRef value, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(value, scriptContext);
        PARAM_NOT_NULL(result);

        if (Js::JavascriptConversion::ToBool((Js::Var)value, scriptContext))
        {
            *result = scriptContext->GetLibrary()->GetTrue();
        }
        else
        {
            *result = scriptContext->GetLibrary()->GetFalse();
        }

        //It is either true or false which we always track so no need to store result identity

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetValueType(_In_ JsValueRef value, _Out_ JsValueType *type)
{
    VALIDATE_JSREF(value);
    PARAM_NOT_NULL(type);

    BEGIN_JSRT_NO_EXCEPTION
    {
        Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(value);
        switch (typeId)
        {
        case Js::TypeIds_Undefined:
            *type = JsUndefined;
            break;
        case Js::TypeIds_Null:
            *type = JsNull;
            break;
        case Js::TypeIds_Boolean:
            *type = JsBoolean;
            break;
        case Js::TypeIds_Integer:
        case Js::TypeIds_Number:
        case Js::TypeIds_Int64Number:
        case Js::TypeIds_UInt64Number:
            *type = JsNumber;
            break;
        case Js::TypeIds_String:
            *type = JsString;
            break;
        case Js::TypeIds_Function:
            *type = JsFunction;
            break;
        case Js::TypeIds_Error:
            *type = JsError;
            break;
        case Js::TypeIds_Array:
        case Js::TypeIds_NativeIntArray:
#if ENABLE_COPYONACCESS_ARRAY
        case Js::TypeIds_CopyOnAccessNativeIntArray:
#endif
        case Js::TypeIds_NativeFloatArray:
        case Js::TypeIds_ES5Array:
            *type = JsArray;
            break;
        case Js::TypeIds_Symbol:
            *type = JsSymbol;
            break;
        case Js::TypeIds_ArrayBuffer:
            *type = JsArrayBuffer;
            break;
        case Js::TypeIds_DataView:
            *type = JsDataView;
            break;
        default:
            if (Js::TypedArrayBase::Is(typeId))
            {
                *type = JsTypedArray;
            }
            else
            {
                *type = JsObject;
            }
            break;
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsDoubleToNumber(_In_ double dbl, _Out_ JsValueRef *asValue)
{
    PARAM_NOT_NULL(asValue);
    //If number is not heap allocated then we don't need to record/track the creation for time-travel
    if (Js::JavascriptNumber::TryToVarFastWithCheck(dbl, asValue))
    {
      return JsNoError;
    }

    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        *asValue = Js::JavascriptNumber::ToVarNoCheck(dbl, scriptContext);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsIntToNumber(_In_ int intValue, _Out_ JsValueRef *asValue)
{
    PARAM_NOT_NULL(asValue);
    //If number is not heap allocated then we don't need to record/track the creation for time-travel
    if (Js::JavascriptNumber::TryToVarFast(intValue, asValue))
    {
        return JsNoError;
    }

    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
#if !INT32VAR
        ;
#endif

        *asValue = Js::JavascriptNumber::ToVar(intValue, scriptContext);

#if !INT32VAR
        ;
#endif

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsNumberToDouble(_In_ JsValueRef value, _Out_ double *asDouble)
{
    VALIDATE_JSREF(value);
    PARAM_NOT_NULL(asDouble);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (Js::TaggedInt::Is(value))
        {
            *asDouble = Js::TaggedInt::ToDouble(value);
        }
        else if (Js::JavascriptNumber::Is_NoTaggedIntCheck(value))
        {
            *asDouble = Js::JavascriptNumber::GetValue(value);
        }
        else
        {
            *asDouble = 0;
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsNumberToInt(_In_ JsValueRef value, _Out_ int *asInt)
{
    VALIDATE_JSREF(value);
    PARAM_NOT_NULL(asInt);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (Js::TaggedInt::Is(value))
        {
            *asInt = Js::TaggedInt::ToInt32(value);
        }
        else if (Js::JavascriptNumber::Is_NoTaggedIntCheck(value))
        {
            *asInt = Js::JavascriptConversion::ToInt32(Js::JavascriptNumber::GetValue(value));
        }
        else
        {
            *asInt = 0;
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsConvertValueToNumber(_In_ JsValueRef value, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(value, scriptContext);
        PARAM_NOT_NULL(result);

        *result = (JsValueRef)Js::JavascriptOperators::ToNumber((Js::Var)value, scriptContext);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetStringLength(_In_ JsValueRef value, _Out_ int *length)
{
    VALIDATE_JSREF(value);
    PARAM_NOT_NULL(length);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::VarIs<Js::JavascriptString>(value))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        *length = Js::VarTo<Js::JavascriptString>(value)->GetLengthAsSignedInt();
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode JsPointerToString(_In_reads_(stringLength) const char16_t *stringValue, _In_ size_t stringLength, _Out_ JsValueRef *string)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(stringValue);
        PARAM_NOT_NULL(string);

        if (!Js::IsValidCharCount(stringLength))
        {
            Js::JavascriptError::ThrowOutOfMemoryError(scriptContext);
        }

        *string = Js::JavascriptString::NewCopyBuffer(stringValue, static_cast<charcount_t>(stringLength), scriptContext);

        ;

        return JsNoError;
    });
}

// TODO: The annotation of stringPtr is wrong.  Need to fix definition in chakrart.h
// The warning is '*stringPtr' could be '0' : this does not adhere to the specification for the function 'JsStringToPointer'.
#pragma warning(suppress:6387)
JsErrorCode JsStringToPointer(_In_ JsValueRef stringValue, _Outptr_result_buffer_(*stringLength) const char16_t **stringPtr, _Out_ size_t *stringLength)
{
    VALIDATE_JSREF(stringValue);
    PARAM_NOT_NULL(stringPtr);
    *stringPtr = nullptr;
    PARAM_NOT_NULL(stringLength);
    *stringLength = 0;

    if (!Js::VarIs<Js::JavascriptString>(stringValue))
    {
        return JsErrorInvalidArgument;
    }

    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        Js::JavascriptString *jsString = Js::VarTo<Js::JavascriptString>(stringValue);

        *stringPtr = jsString->GetSz();
        *stringLength = jsString->GetLength();
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsConvertValueToString(_In_ JsValueRef value, _Out_ JsValueRef *result)
{
    PARAM_NOT_NULL(result);
    *result = nullptr;

    if (value != nullptr && Js::VarIs<Js::JavascriptString>(value))
    {
        return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
            ;
            VALIDATE_INCOMING_REFERENCE(value, scriptContext);

            *result = value;

            ;

            return JsNoError;
        });
    }

    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;
        VALIDATE_INCOMING_REFERENCE(value, scriptContext);

        *result = (JsValueRef) Js::JavascriptConversion::ToString((Js::Var)value, scriptContext);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetGlobalObject(_Out_ JsValueRef *globalObject)
{
    return ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(globalObject);

        *globalObject = (JsValueRef)scriptContext->GetGlobalObject();
        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsCreateObject(_Out_ JsValueRef *object)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(object);

        *object = scriptContext->GetLibrary()->CreateObject();

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateExternalObjectWithPrototype(_In_opt_ void *data,
    _In_opt_ JsFinalizeCallback finalizeCallback,
    _In_opt_ JsValueRef prototype,
    _Out_ JsValueRef *object)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(object);

        Js::RecyclableObject * prototypeObject = nullptr;
        if (prototype != JS_INVALID_REFERENCE)
        {
            VALIDATE_INCOMING_OBJECT(prototype, scriptContext);
            prototypeObject = Js::VarTo<Js::RecyclableObject>(prototype);
        }

        *object = JsrtExternalObject::Create(data, 0, finalizeCallback, prototypeObject, scriptContext, nullptr);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateExternalObject(_In_opt_ void *data, _In_opt_ JsFinalizeCallback finalizeCallback, _Out_ JsValueRef *object)
{
    return JsCreateExternalObjectWithPrototype(data, finalizeCallback, JS_INVALID_REFERENCE, object);
}

JsErrorCode chakracore::jsrt::JsConvertValueToObject(_In_ JsValueRef value, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(value, scriptContext);
        PARAM_NOT_NULL(result);

        *result = (JsValueRef)Js::JavascriptOperators::ToObject((Js::Var)value, scriptContext);
        Assert(*result == nullptr || !Js::CrossSite::NeedMarshalVar(*result, scriptContext));

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetPrototype(_In_ JsValueRef object, _Out_ JsValueRef *prototypeObject)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        PARAM_NOT_NULL(prototypeObject);

        *prototypeObject = (JsValueRef)Js::JavascriptOperators::OP_GetPrototype(object, scriptContext);
        Assert(*prototypeObject == nullptr || !Js::CrossSite::NeedMarshalVar(*prototypeObject, scriptContext));

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsSetPrototype(_In_ JsValueRef object, _In_ JsValueRef prototypeObject)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_OBJECT_OR_NULL(prototypeObject, scriptContext);

        // We're not allowed to set this.
        if (object == scriptContext->GetLibrary()->GetObjectPrototype())
        {
            return JsErrorInvalidArgument;
        }

        Js::JavascriptObject::ChangePrototype(Js::VarTo<Js::RecyclableObject>(object), Js::VarTo<Js::RecyclableObject>(prototypeObject), true, scriptContext);

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsInstanceOf(_In_ JsValueRef object, _In_ JsValueRef constructor, _Out_ bool *result) {
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(object, scriptContext);
        VALIDATE_INCOMING_REFERENCE(constructor, scriptContext);
        PARAM_NOT_NULL(result);

        Js::Var value = Js::JavascriptOperators::OP_IsInst(object, constructor, scriptContext, nullptr);
        *result = !!Js::VarTo<Js::JavascriptBoolean>(value)->GetValue();

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetExtensionAllowed(_In_ JsValueRef object, _Out_ bool *value)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        PARAM_NOT_NULL(value);
        *value = false;

        *value = Js::VarTo<Js::RecyclableObject>(object)->IsExtensible() != 0;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsPreventExtension(_In_ JsValueRef object)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);

        Js::VarTo<Js::RecyclableObject>(object)->PreventExtensions();

        return JsNoError;
    });
}

JsErrorCode JsHasOwnPropertyCommon(Js::ScriptContext * scriptContext, _In_ JsValueRef object,
    _In_ const Js::PropertyRecord * propertyRecord, _Out_ bool *hasOwnProperty, _In_opt_ Js::PropertyString * propString)
{
    *hasOwnProperty = Js::JavascriptOperators::OP_HasOwnProperty(object,
        propertyRecord->GetPropertyId(), scriptContext, propString) != 0;

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsHasOwnProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId,
    _Out_ bool *hasOwnProperty)
{
    return ContextAPIWrapper<true>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(hasOwnProperty);
        *hasOwnProperty = false;

        return JsHasOwnPropertyCommon(scriptContext, object,
            (const Js::PropertyRecord *)propertyId, hasOwnProperty, nullptr);
    });
}

static JsErrorCode InternalGetPropertyRecord(Js::ScriptContext * scriptContext,
    Js::RecyclableObject * key, _Out_ const Js::PropertyRecord ** propertyRecord)
{
    Assert(propertyRecord != nullptr);
    *propertyRecord = nullptr;

    switch(key->GetTypeId())
    {
    case Js::TypeIds_String:
        scriptContext->GetOrAddPropertyRecord(Js::VarTo<Js::JavascriptString>(key),
            (Js::PropertyRecord const **)propertyRecord);
        break;
    case Js::TypeIds_Symbol:
        *propertyRecord = Js::VarTo<Js::JavascriptSymbol>(key)->GetValue();
        break;
    default:
        return JsErrorInvalidArgument;
    };

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsObjectHasOwnProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId, _Out_ bool *hasOwnProperty)
{
    return ContextAPIWrapper<true>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        PARAM_NOT_NULL(hasOwnProperty);
        *hasOwnProperty = false;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        return JsHasOwnPropertyCommon(scriptContext, object, propertyRecord, hasOwnProperty, Js::VarIs<Js::PropertyString>(propertyId) ? (Js::PropertyString*)propertyId : nullptr);
    });
}

static JsErrorCode JsGetPropertyCommon(Js::ScriptContext * scriptContext,
    _In_ Js::RecyclableObject * object,
    _In_ const Js::PropertyRecord * propertyRecord, _Out_ JsValueRef *value)
{
    AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

    *value = Js::JavascriptOperators::GetPropertyNoCache(object, propertyRecord->GetPropertyId(), scriptContext);
    Assert(*value == nullptr || !Js::CrossSite::NeedMarshalVar(*value, scriptContext));

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsGetProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId, _Out_ JsValueRef *value)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(value);
        *value = nullptr;

        Js::RecyclableObject * instance = Js::VarTo<Js::RecyclableObject>(object);
        JsErrorCode err = JsGetPropertyCommon(scriptContext, instance, (const Js::PropertyRecord *)propertyId,
             value);

        ;

        return err;
    });
}

JsErrorCode chakracore::jsrt::JsObjectGetProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId, _Out_ JsValueRef *value)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        PARAM_NOT_NULL(value);
        *value = nullptr;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Assert(propertyRecord != nullptr);

        Js::RecyclableObject * instance = Js::VarTo<Js::RecyclableObject>(object);
        return JsGetPropertyCommon(scriptContext, instance, propertyRecord, value);
    });
}

static JsErrorCode JsGetOwnPropertyDescriptorCommon(Js::ScriptContext * scriptContext,
    _In_ JsValueRef object, _In_ const Js::PropertyRecord * propertyRecord, _Out_ JsValueRef *propertyDescriptor)
{
    AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

    Js::PropertyDescriptor propertyDescriptorValue;
    if (Js::JavascriptOperators::GetOwnPropertyDescriptor(Js::VarTo<Js::RecyclableObject>(object),
        propertyRecord->GetPropertyId(), scriptContext, &propertyDescriptorValue))
    {
        *propertyDescriptor = Js::JavascriptOperators::FromPropertyDescriptor(propertyDescriptorValue, scriptContext);
    }
    else
    {
        *propertyDescriptor = scriptContext->GetLibrary()->GetUndefined();
    }
    Assert(*propertyDescriptor == nullptr || !Js::CrossSite::NeedMarshalVar(*propertyDescriptor, scriptContext));

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsGetOwnPropertyDescriptor(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId, _Out_ JsValueRef *propertyDescriptor)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(propertyDescriptor);
        *propertyDescriptor = nullptr;

        JsErrorCode err = JsGetOwnPropertyDescriptorCommon(scriptContext, object, (const Js::PropertyRecord *)propertyId,
            propertyDescriptor);

        ;

        return err;
    });
}

JsErrorCode chakracore::jsrt::JsObjectGetOwnPropertyDescriptor(_In_ JsValueRef object, _In_ JsValueRef propertyId, _Out_ JsValueRef *propertyDescriptor)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        PARAM_NOT_NULL(propertyDescriptor);
        *propertyDescriptor = nullptr;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Assert(propertyRecord != nullptr);

        return JsGetOwnPropertyDescriptorCommon(scriptContext, object, propertyRecord, propertyDescriptor);
    });
}

static JsErrorCode JsSetPropertyCommon(Js::ScriptContext * scriptContext, _In_ JsValueRef object,
    _In_ const Js::PropertyRecord * propertyRecord, _In_ JsValueRef value, _In_ bool useStrictRules)
{
    AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

    Js::JavascriptOperators::OP_SetProperty(object, propertyRecord->GetPropertyId(),
        value, scriptContext, nullptr, useStrictRules ? Js::PropertyOperation_StrictMode : Js::PropertyOperation_None);

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsSetProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId, _In_ JsValueRef value, _In_ bool useStrictRules)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        VALIDATE_INCOMING_REFERENCE(value, scriptContext);

        return JsSetPropertyCommon(scriptContext, object, (const Js::PropertyRecord *)propertyId,
            value, useStrictRules);
    });
}

JsErrorCode chakracore::jsrt::JsObjectSetProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId, _In_ JsValueRef value, _In_ bool useStrictRules)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        VALIDATE_INCOMING_REFERENCE(value, scriptContext);

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Assert(propertyRecord != nullptr);

        return JsSetPropertyCommon(scriptContext, object, propertyRecord, value, useStrictRules);
    });
}

JsErrorCode chakracore::jsrt::JsHasProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId, _Out_ bool *hasProperty)
{
    VALIDATE_JSREF(object);
    if (!Js::JavascriptOperators::IsObject(object)) return JsErrorArgumentNotObject;

    auto internalHasProperty = [&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(hasProperty);
        *hasProperty = false;

        Js::RecyclableObject * instance = Js::VarTo<Js::RecyclableObject>(object);
        *hasProperty = Js::JavascriptOperators::HasProperty(instance, ((Js::PropertyRecord *)propertyId)->GetPropertyId()) != 0;

        return JsNoError;
    };

    Js::RecyclableObject* robject = Js::VarTo<Js::RecyclableObject>(object);
    Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(robject);
    while (typeId != Js::TypeIds_Null && typeId != Js::TypeIds_Proxy)
    {
        robject = robject->GetPrototype();
        typeId = Js::JavascriptOperators::GetTypeId(robject);
    }

    if (typeId == Js::TypeIds_Proxy)
    {
        return ContextAPIWrapper<JSRT_MAYBE_TRUE>(internalHasProperty);
    }
    else if (typeId == Js::TypeIds_Object)
    {
        // CEOs can also have traps so we would want the Enter/Leave semantics for those.
        Js::CustomExternalWrapperObject * externalWrapper = Js::JavascriptOperators::TryFromVar<Js::CustomExternalWrapperObject>(object);
        if (externalWrapper)
        {
            return ContextAPIWrapper<JSRT_MAYBE_TRUE>(internalHasProperty);
        }
    }

    return ContextAPINoScriptWrapper(internalHasProperty);
}

JsErrorCode chakracore::jsrt::JsObjectHasProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId, _Out_ bool *hasProperty)
{
    VALIDATE_JSREF(object);
    if (!Js::JavascriptOperators::IsObject(object)) return JsErrorArgumentNotObject;

    auto internalHasProperty = [&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;
        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        PARAM_NOT_NULL(hasProperty);
        *hasProperty = false;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Js::RecyclableObject * instance = Js::VarTo<Js::RecyclableObject>(object);
        *hasProperty = Js::JavascriptOperators::HasProperty(instance, propertyRecord->GetPropertyId()) != 0;

        return JsNoError;
    };

    Js::RecyclableObject* robject = Js::VarTo<Js::RecyclableObject>(object);
    Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(robject);
    while (typeId != Js::TypeIds_Null && typeId != Js::TypeIds_Proxy)
    {
        robject = robject->GetPrototype();
        typeId = Js::JavascriptOperators::GetTypeId(robject);
    }

    if (typeId == Js::TypeIds_Proxy)
    {
        return ContextAPIWrapper<JSRT_MAYBE_TRUE>(internalHasProperty);
    }
    else if (typeId == Js::TypeIds_Object)
    {
        // CEOs can also have traps so we would want the Enter/Leave semantics for those.
        Js::CustomExternalWrapperObject * externalWrapper = Js::JavascriptOperators::TryFromVar<Js::CustomExternalWrapperObject>(object);
        if (externalWrapper)
        {
            return ContextAPIWrapper<JSRT_MAYBE_TRUE>(internalHasProperty);
        }
    }

    return ContextAPINoScriptWrapper(internalHasProperty);
}

static JsErrorCode JsDeletePropertyCommon(Js::ScriptContext * scriptContext, _In_ JsValueRef object,
    _In_ const Js::PropertyRecord * propertyRecord, _In_ bool useStrictRules, _Out_ JsValueRef *result)
{
    AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

    *result = Js::JavascriptOperators::OP_DeleteProperty((Js::Var)object,
        propertyRecord->GetPropertyId(),
        scriptContext, useStrictRules ? Js::PropertyOperation_StrictMode : Js::PropertyOperation_None);

    Assert(*result == nullptr || !Js::CrossSite::NeedMarshalVar(*result, scriptContext));

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsDeleteProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId,
    _In_ bool useStrictRules, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(result);
        *result = nullptr;

        JsErrorCode err = JsDeletePropertyCommon(scriptContext, object, (const Js::PropertyRecord *)propertyId,
            useStrictRules, result);

        ;

        return err;
    });
}

JsErrorCode chakracore::jsrt::JsObjectDeleteProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId,
    _In_ bool useStrictRules, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        PARAM_NOT_NULL(result);
        *result = nullptr;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Assert(propertyRecord != nullptr);

        return JsDeletePropertyCommon(scriptContext, object, propertyRecord,
            useStrictRules, result);
    });
}

static JsErrorCode JsDefinePropertyCommon(Js::ScriptContext * scriptContext, _In_ JsValueRef object,
    _In_ const Js::PropertyRecord *propertyRecord, _In_ JsValueRef propertyDescriptor,
    _Out_ bool *result)
{
    AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

    Js::PropertyDescriptor propertyDescriptorValue;
    if (!Js::JavascriptOperators::ToPropertyDescriptor(propertyDescriptor, &propertyDescriptorValue, scriptContext))
    {
        return JsErrorInvalidArgument;
    }

    *result = Js::JavascriptOperators::DefineOwnPropertyDescriptor(
        Js::VarTo<Js::RecyclableObject>(object), propertyRecord->GetPropertyId(),
        propertyDescriptorValue, true, scriptContext) != 0;

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsDefineProperty(_In_ JsValueRef object, _In_ JsPropertyIdRef propertyId,
    _In_ JsValueRef propertyDescriptor, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        VALIDATE_INCOMING_OBJECT(propertyDescriptor, scriptContext);
        PARAM_NOT_NULL(result);
        *result = false;

        return JsDefinePropertyCommon(scriptContext, object, (const Js::PropertyRecord *)propertyId,
            propertyDescriptor, result);
    });
}

JsErrorCode chakracore::jsrt::JsObjectDefinePropertyFull(
    _In_ JsValueRef object,
    _In_ JsValueRef key,
    _In_opt_ JsValueRef value,
    _In_opt_ JsValueRef getter,
    _In_opt_ JsValueRef setter,
    _In_ bool writable,
    _In_ bool enumerable,
    _In_ bool configurable,
    _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;
        AssertMsg(scriptContext->GetThreadContext()->IsScriptActive(), "Caller is expected to be under ContextAPIWrapper!");

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(key, scriptContext);
        PARAM_NOT_NULL(result);
        *result = false;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(key), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        Js::PropertyDescriptor propertyDescriptor;
        if (value)
        {
            propertyDescriptor.SetValue(value);
        }
        if (getter)
        {
            propertyDescriptor.SetGetter(getter);
        }
        if (setter)
        {
            propertyDescriptor.SetSetter(setter);
        }
        if (writable)
        {
            propertyDescriptor.SetWritable(writable);
        }
        if (enumerable)
        {
            propertyDescriptor.SetEnumerable(enumerable);
        }
        if (configurable)
        {
            propertyDescriptor.SetConfigurable(configurable);
        }

        *result = Js::JavascriptOperators::DefineOwnPropertyDescriptor(
            Js::VarTo<Js::RecyclableObject>(object), propertyRecord->GetPropertyId(),
            propertyDescriptor, true, scriptContext) != 0;
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsObjectDefineProperty(_In_ JsValueRef object, _In_ JsValueRef propertyId,
    _In_ JsValueRef propertyDescriptor, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_RECYCLABLE(propertyId, scriptContext);
        VALIDATE_INCOMING_OBJECT(propertyDescriptor, scriptContext);
        PARAM_NOT_NULL(result);
        *result = false;

        const Js::PropertyRecord *propertyRecord = nullptr;
        JsErrorCode errorValue = InternalGetPropertyRecord(scriptContext,
            Js::VarTo<Js::RecyclableObject>(propertyId), &propertyRecord);

        if (errorValue != JsNoError)
        {
            return errorValue;
        }

        return JsDefinePropertyCommon(scriptContext, object, propertyRecord, propertyDescriptor, result);
    });
}

JsErrorCode chakracore::jsrt::JsGetOwnPropertyNames(_In_ JsValueRef object, _Out_ JsValueRef *propertyNames)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        PARAM_NOT_NULL(propertyNames);
        *propertyNames = nullptr;

        *propertyNames = Js::JavascriptOperators::GetOwnPropertyNames(object, scriptContext);
        Assert(*propertyNames == nullptr || !Js::CrossSite::NeedMarshalVar(*propertyNames, scriptContext));

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetOwnPropertySymbols(_In_ JsValueRef object, _Out_ JsValueRef *propertySymbols)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        PARAM_NOT_NULL(propertySymbols);

        *propertySymbols = Js::JavascriptOperators::GetOwnPropertySymbols(object, scriptContext);
        Assert(*propertySymbols == nullptr || !Js::CrossSite::NeedMarshalVar(*propertySymbols, scriptContext));

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateArray(_In_ unsigned int length, _Out_ JsValueRef *result)
{
    return ContextAPINoScriptWrapper([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(result);
        *result = nullptr;

        *result = scriptContext->GetLibrary()->CreateArray(length);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateArrayBuffer(_In_ unsigned int byteLength, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(result);

        Js::JavascriptLibrary* library = scriptContext->GetLibrary();
        *result = library->CreateArrayBuffer(byteLength);

        ;
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateExternalArrayBuffer(void *data, _In_ unsigned int byteLength,
    _In_opt_ JsFinalizeCallback finalizeCallback, _In_opt_ void *callbackState, _Out_ JsValueRef *result)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(result);

        if (data == nullptr && byteLength > 0)
        {
            return JsErrorInvalidArgument;
        }

        Js::JavascriptLibrary* library = scriptContext->GetLibrary();
        *result = Js::JsrtExternalArrayBuffer::New(
            reinterpret_cast<uint8_t*>(data),
            byteLength,
            finalizeCallback,
            callbackState,
            library->GetArrayBufferType());

        ;
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateTypedArray(_In_ JsTypedArrayType arrayType, _In_ JsValueRef baseArray, _In_ unsigned int byteOffset,
    _In_ unsigned int elementLength, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        if (baseArray != JS_INVALID_REFERENCE)
        {
            VALIDATE_INCOMING_REFERENCE(baseArray, scriptContext);
        }
        PARAM_NOT_NULL(result);

        Js::JavascriptLibrary* library = scriptContext->GetLibrary();

        const bool fromArrayBuffer = (baseArray != JS_INVALID_REFERENCE && Js::VarIs<Js::ArrayBuffer>(baseArray));

        if (byteOffset != 0 && !fromArrayBuffer)
        {
            return JsErrorInvalidArgument;
        }

        if (elementLength != 0 && !(baseArray == JS_INVALID_REFERENCE || fromArrayBuffer))
        {
            return JsErrorInvalidArgument;
        }

        Js::JavascriptFunction* constructorFunc = nullptr;
        Js::Var values[4] =
        {
            library->GetUndefined(),
            baseArray != nullptr ? baseArray : Js::JavascriptNumber::ToVar(elementLength, scriptContext)
        };
        if (fromArrayBuffer)
        {
            values[2] = Js::JavascriptNumber::ToVar(byteOffset, scriptContext);
            values[3] = Js::JavascriptNumber::ToVar(elementLength, scriptContext);
        }

        Js::CallInfo info(Js::CallFlags_New, fromArrayBuffer ? 4 : 2);
        Js::Arguments args(info, values);

        switch (arrayType)
        {
        case JsArrayTypeInt8:
            constructorFunc = library->GetInt8ArrayConstructor();
            break;
        case JsArrayTypeUint8:
            constructorFunc = library->GetUint8ArrayConstructor();
            break;
        case JsArrayTypeUint8Clamped:
            constructorFunc = library->GetUint8ClampedArrayConstructor();
            break;
        case JsArrayTypeInt16:
            constructorFunc = library->GetInt16ArrayConstructor();
            break;
        case JsArrayTypeUint16:
            constructorFunc = library->GetUint16ArrayConstructor();
            break;
        case JsArrayTypeInt32:
            constructorFunc = library->GetInt32ArrayConstructor();
            break;
        case JsArrayTypeUint32:
            constructorFunc = library->GetUint32ArrayConstructor();
            break;
        case JsArrayTypeFloat32:
            constructorFunc = library->GetFloat32ArrayConstructor();
            break;
        case JsArrayTypeFloat64:
            constructorFunc = library->GetFloat64ArrayConstructor();
            break;
        default:
            return JsErrorInvalidArgument;
        }

        BEGIN_SAFE_REENTRANT_CALL(scriptContext->GetThreadContext())
        {
            *result = Js::JavascriptFunction::CallAsConstructor(constructorFunc, /* overridingNewTarget = */nullptr, args, scriptContext);
        }
        END_SAFE_REENTRANT_CALL
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateDataView(_In_ JsValueRef arrayBuffer, _In_ unsigned int byteOffset, _In_ unsigned int byteLength, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(arrayBuffer, scriptContext);
        PARAM_NOT_NULL(result);

        if (!Js::VarIs<Js::ArrayBuffer>(arrayBuffer))
        {
            return JsErrorInvalidArgument;
        }

        Js::JavascriptLibrary* library = scriptContext->GetLibrary();
        *result = library->CreateDataView(Js::VarTo<Js::ArrayBuffer>(arrayBuffer), byteOffset, byteLength);
        return JsNoError;
    });
}


static_assert(static_cast<uint8_t>(JsArrayTypeUint8)         - Js::TypeIds_Uint8Array        == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeUint8Clamped)  - Js::TypeIds_Uint8ClampedArray == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeInt16)         - Js::TypeIds_Int16Array        == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeUint16)        - Js::TypeIds_Uint16Array       == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeInt32)         - Js::TypeIds_Int32Array        == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeUint32)        - Js::TypeIds_Uint32Array       == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeFloat32)       - Js::TypeIds_Float32Array      == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);
static_assert(static_cast<uint8_t>(JsArrayTypeFloat64)       - Js::TypeIds_Float64Array      == static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array);

inline JsTypedArrayType GetTypedArrayType(Js::TypeId typeId)
{
    Assert(Js::TypedArrayBase::Is(typeId));
    return static_cast<JsTypedArrayType>(typeId + (static_cast<uint8_t>(JsArrayTypeInt8) - Js::TypeIds_Int8Array));
}

JsErrorCode chakracore::jsrt::JsGetTypedArrayInfo(_In_ JsValueRef typedArray, _Out_opt_ JsTypedArrayType *arrayType, _Out_opt_ JsValueRef *arrayBuffer,
    _Out_opt_ unsigned int *byteOffset, _Out_opt_ unsigned int *byteLength)
{
    VALIDATE_JSREF(typedArray);

    BEGIN_JSRT_NO_EXCEPTION
    {
        const Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(typedArray);

        if (!Js::TypedArrayBase::Is(typeId))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        if (arrayType != nullptr) {
            *arrayType = GetTypedArrayType(typeId);
        }

        Js::TypedArrayBase* typedArrayBase = Js::VarTo<Js::TypedArrayBase>(typedArray);
        if (arrayBuffer != nullptr) {
            *arrayBuffer = typedArrayBase->GetArrayBuffer();
        }

        if (byteOffset != nullptr) {
            *byteOffset = typedArrayBase->GetByteOffset();
        }

        if (byteLength != nullptr) {
            *byteLength = typedArrayBase->GetByteLength();
        }
    }

    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetArrayBufferStorage(_In_ JsValueRef instance, _Outptr_result_bytebuffer_(*bufferLength) uint8_t **buffer,
    _Out_ unsigned int *bufferLength)
{
    VALIDATE_JSREF(instance);
    PARAM_NOT_NULL(buffer);
    PARAM_NOT_NULL(bufferLength);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::VarIs<Js::ArrayBuffer>(instance))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        Js::ArrayBuffer* arrayBuffer = Js::VarTo<Js::ArrayBuffer>(instance);
        *buffer = arrayBuffer->GetBuffer();
        *bufferLength = arrayBuffer->GetByteLength();
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetTypedArrayStorage(_In_ JsValueRef instance, _Outptr_result_bytebuffer_(*bufferLength) uint8_t **buffer,
    _Out_ unsigned int *bufferLength, _Out_opt_ JsTypedArrayType *typedArrayType, _Out_opt_ int *elementSize)
{
    VALIDATE_JSREF(instance);
    PARAM_NOT_NULL(buffer);
    PARAM_NOT_NULL(bufferLength);

    BEGIN_JSRT_NO_EXCEPTION
    {
        const Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(instance);
        if (!Js::TypedArrayBase::Is(typeId))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        Js::TypedArrayBase* typedArrayBase = Js::VarTo<Js::TypedArrayBase>(instance);
        *buffer = typedArrayBase->GetByteBuffer();
        *bufferLength = typedArrayBase->GetByteLength();

        if (typedArrayType)
        {
            *typedArrayType = GetTypedArrayType(typeId);
        }

        if (elementSize)
        {
            switch (typeId)
            {
                case Js::TypeIds_Int8Array:
                    *elementSize = sizeof(int8_t);
                    break;
                case Js::TypeIds_Uint8Array:
                    *elementSize = sizeof(uint8_t);
                    break;
                case Js::TypeIds_Uint8ClampedArray:
                    *elementSize = sizeof(uint8_t);
                    break;
                case Js::TypeIds_Int16Array:
                    *elementSize = sizeof(int16);
                    break;
                case Js::TypeIds_Uint16Array:
                    *elementSize = sizeof(uint16);
                    break;
                case Js::TypeIds_Int32Array:
                    *elementSize = sizeof(int32_t);
                    break;
                case Js::TypeIds_Uint32Array:
                    *elementSize = sizeof(uint32_t);
                    break;
                case Js::TypeIds_Float32Array:
                    *elementSize = sizeof(float);
                    break;
                case Js::TypeIds_Float64Array:
                    *elementSize = sizeof(double);
                    break;
                default:
                    AssertMsg(FALSE, "invalid typed array type");
                    *elementSize = 1;
                    RETURN_NO_EXCEPTION(JsErrorFatal);
            }
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetDataViewStorage(_In_ JsValueRef instance, _Outptr_result_bytebuffer_(*bufferLength) uint8_t **buffer, _Out_ unsigned int *bufferLength)
{
    VALIDATE_JSREF(instance);
    PARAM_NOT_NULL(buffer);
    PARAM_NOT_NULL(bufferLength);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::VarIs<Js::DataView>(instance))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        Js::DataView* dataView = Js::VarTo<Js::DataView>(instance);
        *buffer = dataView->GetArrayBuffer()->GetBuffer() + dataView->GetByteOffset();
        *bufferLength = dataView->GetLength();
    }
    END_JSRT_NO_EXCEPTION
}


JsErrorCode chakracore::jsrt::JsCreateSymbol(_In_ JsValueRef description, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        PARAM_NOT_NULL(result);
        *result = nullptr;

        Js::JavascriptString* descriptionString;

        if (description != JS_INVALID_REFERENCE)
        {
            VALIDATE_INCOMING_REFERENCE(description, scriptContext);
            descriptionString = Js::JavascriptConversion::ToString(description, scriptContext);
        }
        else
        {
            descriptionString = scriptContext->GetLibrary()->GetEmptyString();
        }

        *result = scriptContext->GetLibrary()->CreateSymbol(descriptionString);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsHasIndexedProperty(_In_ JsValueRef object, _In_ JsValueRef index, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_REFERENCE(index, scriptContext);
        PARAM_NOT_NULL(result);
        *result = false;

        *result = Js::JavascriptOperators::OP_HasItem((Js::Var)object, (Js::Var)index, scriptContext) != 0;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetIndexedProperty(_In_ JsValueRef object, _In_ JsValueRef index, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_REFERENCE(index, scriptContext);
        PARAM_NOT_NULL(result);
        *result = nullptr;

        *result = (JsValueRef)Js::JavascriptOperators::OP_GetElementI((Js::Var)object, (Js::Var)index, scriptContext);

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsSetIndexedProperty(_In_ JsValueRef object, _In_ JsValueRef index, _In_ JsValueRef value)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_REFERENCE(index, scriptContext);
        VALIDATE_INCOMING_REFERENCE(value, scriptContext);

        Js::JavascriptOperators::OP_SetElementI((Js::Var)object, (Js::Var)index, (Js::Var)value, scriptContext);

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsDeleteIndexedProperty(_In_ JsValueRef object, _In_ JsValueRef index)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);
        VALIDATE_INCOMING_REFERENCE(index, scriptContext);

        Js::JavascriptOperators::OP_DeleteElementI((Js::Var)object, (Js::Var)index, scriptContext);

        return JsNoError;
    });
}

template <class T, bool clamped = false> struct TypedArrayTypeTraits { static const JsTypedArrayType cTypedArrayType; };
template<> struct TypedArrayTypeTraits<int8_t> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeInt8; };
template<> struct TypedArrayTypeTraits<uint8_t, false> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeUint8; };
template<> struct TypedArrayTypeTraits<uint8_t, true> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeUint8Clamped; };
template<> struct TypedArrayTypeTraits<int16> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeInt16; };
template<> struct TypedArrayTypeTraits<uint16> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeUint16; };
template<> struct TypedArrayTypeTraits<int32_t> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeInt32; };
template<> struct TypedArrayTypeTraits<uint32_t> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeUint32; };
template<> struct TypedArrayTypeTraits<float> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeFloat32; };
template<> struct TypedArrayTypeTraits<double> { static const JsTypedArrayType cTypedArrayType = JsTypedArrayType::JsArrayTypeFloat64; };

template <class T, bool clamped = false>
Js::ArrayObject* CreateTypedArray(Js::ScriptContext *scriptContext, void* data, unsigned int length)
{
    Js::JavascriptLibrary* library = scriptContext->GetLibrary();

    Js::ArrayBufferBase* arrayBuffer = RecyclerNew(
        scriptContext->GetRecycler(),
        Js::ExternalArrayBuffer,
        reinterpret_cast<uint8_t*>(data),
        length * sizeof(T),
        library->GetArrayBufferType());

    return static_cast<Js::ArrayObject*>(Js::TypedArray<T, clamped>::Create(arrayBuffer, 0, length, library));
}

template <class T, bool clamped = false>
void GetObjectArrayData(Js::ArrayObject* objectArray, void** data, JsTypedArrayType* arrayType, uint* length)
{
    Js::TypedArray<T, clamped>* typedArray = Js::VarTo<Js::TypedArray<T, clamped>>(objectArray);
    *data = typedArray->GetArrayBuffer()->GetBuffer();
    *arrayType = TypedArrayTypeTraits<T, clamped>::cTypedArrayType;
    *length = typedArray->GetLength();
}

JsErrorCode chakracore::jsrt::JsSetIndexedPropertiesToExternalData(
    _In_ JsValueRef object,
    _In_ void* data,
    _In_ JsTypedArrayType arrayType,
    _In_ unsigned int elementLength)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_OBJECT(object, scriptContext);

        // Don't support doing this on array or array-like object
        Js::TypeId typeId = Js::JavascriptOperators::GetTypeId(object);
        if (!Js::DynamicType::Is(typeId)
            || Js::DynamicObject::IsAnyArrayTypeId(typeId)
            || (typeId >= Js::TypeIds_TypedArrayMin && typeId <= Js::TypeIds_TypedArrayMax)
            || typeId == Js::TypeIds_ArrayBuffer
            || typeId == Js::TypeIds_DataView
            || Js::VarTo<Js::RecyclableObject>(object)->IsExternal()
            )
        {
            return JsErrorInvalidArgument;
        }

        if (data == nullptr && elementLength > 0)
        {
            return JsErrorInvalidArgument;
        }

        Js::ArrayObject* newTypedArray = nullptr;
        switch (arrayType)
        {
        case JsArrayTypeInt8:
            newTypedArray = CreateTypedArray<int8_t>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeUint8:
            newTypedArray = CreateTypedArray<uint8_t>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeUint8Clamped:
            newTypedArray = CreateTypedArray<uint8_t, true>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeInt16:
            newTypedArray = CreateTypedArray<int16>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeUint16:
            newTypedArray = CreateTypedArray<uint16>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeInt32:
            newTypedArray = CreateTypedArray<int32_t>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeUint32:
            newTypedArray = CreateTypedArray<uint32_t>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeFloat32:
            newTypedArray = CreateTypedArray<float>(scriptContext, data, elementLength);
            break;
        case JsArrayTypeFloat64:
            newTypedArray = CreateTypedArray<double>(scriptContext, data, elementLength);
            break;
        default:
            return JsErrorInvalidArgument;
        }

        Js::DynamicObject* dynamicObject = Js::VarTo<Js::DynamicObject>(object);
        dynamicObject->SetObjectArray(newTypedArray);

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsHasIndexedPropertiesExternalData(_In_ JsValueRef object, _Out_ bool *value)
{
    VALIDATE_JSREF(object);
    PARAM_NOT_NULL(value);

    BEGIN_JSRT_NO_EXCEPTION
    {
        *value = false;

        if (Js::DynamicType::Is(Js::JavascriptOperators::GetTypeId(object)))
        {
            Js::DynamicObject* dynamicObject = Js::UnsafeVarTo<Js::DynamicObject>(object);
            Js::ArrayObject* objectArray = dynamicObject->GetObjectArray();
            *value = (objectArray && !Js::DynamicObject::IsAnyArray(objectArray));
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetIndexedPropertiesExternalData(
    _In_ JsValueRef object,
    _Out_ void** buffer,
    _Out_ JsTypedArrayType* arrayType,
    _Out_ unsigned int* elementLength)
{
    VALIDATE_JSREF(object);
    PARAM_NOT_NULL(buffer);
    PARAM_NOT_NULL(arrayType);
    PARAM_NOT_NULL(elementLength);

    BEGIN_JSRT_NO_EXCEPTION
    {
        if (!Js::DynamicType::Is(Js::JavascriptOperators::GetTypeId(object)))
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        *buffer = nullptr;
        *arrayType = JsTypedArrayType();
        *elementLength = 0;

        Js::DynamicObject* dynamicObject = Js::UnsafeVarTo<Js::DynamicObject>(object);
        Js::ArrayObject* objectArray = dynamicObject->GetObjectArray();
        if (!objectArray)
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }

        switch (Js::JavascriptOperators::GetTypeId(objectArray))
        {
        case Js::TypeIds_Int8Array:
            GetObjectArrayData<int8_t>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Uint8Array:
            GetObjectArrayData<uint8_t>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Uint8ClampedArray:
            GetObjectArrayData<uint8_t, true>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Int16Array:
            GetObjectArrayData<int16>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Uint16Array:
            GetObjectArrayData<uint16>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Int32Array:
            GetObjectArrayData<int32_t>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Uint32Array:
            GetObjectArrayData<uint32_t>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Float32Array:
            GetObjectArrayData<float>(objectArray, buffer, arrayType, elementLength);
            break;
        case Js::TypeIds_Float64Array:
            GetObjectArrayData<double>(objectArray, buffer, arrayType, elementLength);
            break;
        default:
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsLessThan(_In_ JsValueRef object1, _In_ JsValueRef object2, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(object1, scriptContext);
        VALIDATE_INCOMING_REFERENCE(object2, scriptContext);
        PARAM_NOT_NULL(result);

        *result = Js::JavascriptOperators::Less((Js::Var)object1, (Js::Var)object2, scriptContext) != 0;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsLessThanOrEqual(_In_ JsValueRef object1, _In_ JsValueRef object2, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(object1, scriptContext);
        VALIDATE_INCOMING_REFERENCE(object2, scriptContext);
        PARAM_NOT_NULL(result);

        *result = Js::JavascriptOperators::LessEqual((Js::Var)object1, (Js::Var)object2, scriptContext) != 0;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsEquals(_In_ JsValueRef object1, _In_ JsValueRef object2, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(object1, scriptContext);
        VALIDATE_INCOMING_REFERENCE(object2, scriptContext);
        PARAM_NOT_NULL(result);

        *result = Js::JavascriptOperators::Equal((Js::Var)object1, (Js::Var)object2, scriptContext) != 0;
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsStrictEquals(_In_ JsValueRef object1, _In_ JsValueRef object2, _Out_ bool *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(object1, scriptContext);
        VALIDATE_INCOMING_REFERENCE(object2, scriptContext);
        PARAM_NOT_NULL(result);

        *result = Js::JavascriptOperators::StrictEqual((Js::Var)object1, (Js::Var)object2, scriptContext) != 0;
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsHasExternalData(_In_ JsValueRef object, _Out_ bool *value)
{
    VALIDATE_JSREF(object);
    PARAM_NOT_NULL(value);

    BEGIN_JSRT_NO_EXCEPTION
    {
        while (Js::VarIs<Js::JavascriptProxy>(object))
        {
            object = Js::UnsafeVarTo<Js::JavascriptProxy>(object);
        }
        *value = (Js::VarIs<JsrtExternalObject>(object)
            || Js::VarIs<Js::CustomExternalWrapperObject>(object)
            );
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsGetExternalData(_In_ JsValueRef object, _Out_ void **data)
{
    VALIDATE_JSREF(object);
    PARAM_NOT_NULL(data);

    BEGIN_JSRT_NO_EXCEPTION
    {
        while (Js::VarIs<Js::JavascriptProxy>(object))
        {
            object = Js::UnsafeVarTo<Js::JavascriptProxy>(object)->GetTarget();
        }
        if (Js::VarIs<JsrtExternalObject>(object))
        {
            *data = Js::UnsafeVarTo<JsrtExternalObject>(object)->GetSlotData();
        }
        else if (Js::VarIs<Js::CustomExternalWrapperObject>(object))
        {
            *data = Js::UnsafeVarTo<Js::CustomExternalWrapperObject>(object)->GetSlotData();
        }
        else
        {
            *data = nullptr;
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsSetExternalData(_In_ JsValueRef object, _In_opt_ void *data)
{
    VALIDATE_JSREF(object);

    BEGIN_JSRT_NO_EXCEPTION
    {
        while (Js::VarIs<Js::JavascriptProxy>(object))
        {
            object = Js::UnsafeVarTo<Js::JavascriptProxy>(object)->GetTarget();
        }
        if (Js::VarIs<JsrtExternalObject>(object))
        {
            Js::UnsafeVarTo<JsrtExternalObject>(object)->SetSlotData(data);
        }
        else if (Js::VarIs<Js::CustomExternalWrapperObject>(object))
        {
            Js::UnsafeVarTo<Js::CustomExternalWrapperObject>(object)->SetSlotData(data);
        }
        else
        {
            RETURN_NO_EXCEPTION(JsErrorInvalidArgument);
        }
    }
    END_JSRT_NO_EXCEPTION
}

JsErrorCode chakracore::jsrt::JsCallFunction(_In_ JsValueRef function, _In_reads_(cargs) JsValueRef *args, _In_ ushort cargs, _Out_opt_ JsValueRef *result)
{
    if(result != nullptr)
    {
        *result = nullptr;
    }

    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        VALIDATE_INCOMING_FUNCTION(function, scriptContext);

        if(cargs == 0 || args == nullptr)
        {
            return JsErrorInvalidArgument;
        }

        for(int index = 0; index < cargs; index++)
        {
            VALIDATE_INCOMING_REFERENCE(args[index], scriptContext);
        }

        Js::JavascriptFunction *jsFunction = Js::VarTo<Js::JavascriptFunction>(function);
        Js::CallInfo callInfo(cargs);
        Js::Arguments jsArgs(callInfo, reinterpret_cast<Js::Var *>(args));

        Js::Var varResult = jsFunction->CallRootFunction(jsArgs, scriptContext, true);
        if(result != nullptr)
        {
            *result = varResult;
            Assert(*result == nullptr || !Js::CrossSite::NeedMarshalVar(*result, scriptContext));
        }

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsConstructObject(_In_ JsValueRef function, _In_reads_(cargs) JsValueRef *args, _In_ ushort cargs, _Out_ JsValueRef *result)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_FUNCTION(function, scriptContext);
        PARAM_NOT_NULL(result);
        *result = nullptr;

        if (cargs == 0 || args == nullptr)
        {
            return JsErrorInvalidArgument;
        }

        for (int index = 0; index < cargs; index++)
        {
            VALIDATE_INCOMING_REFERENCE(args[index], scriptContext);
        }

        Js::JavascriptFunction *jsFunction = Js::VarTo<Js::JavascriptFunction>(function);
        Js::CallInfo callInfo(Js::CallFlags::CallFlags_New, cargs);
        Js::Arguments jsArgs(callInfo, reinterpret_cast<Js::Var *>(args));

        BEGIN_SAFE_REENTRANT_CALL(scriptContext->GetThreadContext())
        {
            *result = Js::JavascriptFunction::CallAsConstructor(jsFunction, /* overridingNewTarget = */nullptr, jsArgs, scriptContext);
        }
        END_SAFE_REENTRANT_CALL
        Assert(*result == nullptr || !Js::CrossSite::NeedMarshalVar(*result, scriptContext));

        ;

        return JsNoError;
    });
}

typedef struct JsNativeFunctionWrapperHolder
{
    typename WriteBarrierFieldTypeTraits<void *, _no_write_barrier_policy, _no_write_barrier_policy>::Type callbackState;
    typename WriteBarrierFieldTypeTraits<JsNativeFunction, _no_write_barrier_policy, _no_write_barrier_policy>::Type nativeFunction;
}JsNativeFunctionWrapperHolder;

JsValueRef CALLBACK JsNativeFunctionWrapper(JsValueRef callee, JsValueRef *arguments, unsigned short argumentCount, JsNativeFunctionInfo *info, void *wrapperData)
{
    JsNativeFunctionWrapperHolder *wrapperHolder = static_cast<JsNativeFunctionWrapperHolder*>(wrapperData);
    JsValueRef result = wrapperHolder->nativeFunction(callee, info->isConstructCall, arguments, argumentCount, wrapperHolder->callbackState);
    return result;
}

template <bool wrapNativeFunction, class T>
JsErrorCode JsCreateEnhancedFunctionHelper(_In_ T nativeFunction, _In_opt_ JsValueRef metadata, _In_opt_ void *callbackState, _Out_ JsValueRef *function)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        ;
        PARAM_NOT_NULL(nativeFunction);
        PARAM_NOT_NULL(function);
        *function = nullptr;

        Js::StdCallJavascriptMethod method;

        if (wrapNativeFunction)
        {
            JsNativeFunctionWrapperHolder *wrapperHolder = RecyclerNewStruct(scriptContext->GetRecycler(), JsNativeFunctionWrapperHolder);
            wrapperHolder->callbackState = callbackState;
            wrapperHolder->nativeFunction = (JsNativeFunction)nativeFunction;
            callbackState = wrapperHolder;
            method = (Js::StdCallJavascriptMethod)JsNativeFunctionWrapper;
        }
        else
        {
            method = (Js::StdCallJavascriptMethod)nativeFunction;
        }

        if (metadata != JS_INVALID_REFERENCE)
        {
            VALIDATE_INCOMING_REFERENCE(metadata, scriptContext);
            metadata = Js::JavascriptConversion::ToString(metadata, scriptContext);
        }
        else
        {
            metadata = scriptContext->GetLibrary()->GetEmptyString();
        }

        Js::JavascriptExternalFunction *externalFunction = scriptContext->GetLibrary()->CreateStdCallExternalFunction(method, metadata, callbackState);
        *function = (JsValueRef)externalFunction;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateEnhancedFunction(_In_ JsEnhancedNativeFunction nativeFunction, _In_opt_ JsValueRef metadata, _In_opt_ void *callbackState, _Out_ JsValueRef *function)
{
    return JsCreateEnhancedFunctionHelper<false>(nativeFunction, metadata, callbackState, function);
}

JsErrorCode chakracore::jsrt::JsCreateFunction(_In_ JsNativeFunction nativeFunction, _In_opt_ void *callbackState, _Out_ JsValueRef *function)
{
    return JsCreateEnhancedFunctionHelper<true>(nativeFunction, JS_INVALID_REFERENCE, callbackState, function);
}

JsErrorCode chakracore::jsrt::JsCreateNamedFunction(_In_ JsValueRef name, _In_ JsNativeFunction nativeFunction, _In_opt_ void *callbackState, _Out_ JsValueRef *function)
{
    return JsCreateEnhancedFunctionHelper<true>(nativeFunction, name, callbackState, function);
}

void SetErrorMessage(Js::ScriptContext *scriptContext, Js::JavascriptError *newError, JsValueRef message)
{
    // ECMA262 #sec-error-message
    if (!Js::JavascriptOperators::IsUndefined(message))
    {
        Js::JavascriptString *messageStr = nullptr;
        if (Js::VarIs<Js::JavascriptString>(message))
        {
            messageStr = Js::VarTo<Js::JavascriptString>(message);
        }
        else
        {
            messageStr = Js::JavascriptConversion::ToString(message, scriptContext);
        }

        Js::PropertyDescriptor desc;
        desc.SetValue(messageStr);
        desc.SetWritable(true);
        desc.SetEnumerable(false);
        desc.SetConfigurable(true);
        Js::JavascriptOperators::SetPropertyDescriptor(newError, Js::PropertyIds::message, desc);
    }
}

JsErrorCode chakracore::jsrt::JsCreateError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateRangeError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateRangeError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateReferenceError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateReferenceError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateSyntaxError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateSyntaxError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateTypeError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateTypeError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsCreateURIError(_In_ JsValueRef message, _Out_ JsValueRef *error)
{
    return ContextAPIWrapper<JSRT_MAYBE_TRUE>([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(message, scriptContext);
        PARAM_NOT_NULL(error);
        *error = nullptr;

        Js::JavascriptError *newError = scriptContext->GetLibrary()->CreateURIError();
        SetErrorMessage(scriptContext, newError, message);
        *error = newError;

        ;

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsHasException(_Out_ bool *hasException)
{
    PARAM_NOT_NULL(hasException);
    *hasException = false;

    JsrtContext *currentContext = JsrtContext::GetCurrent();

    if (currentContext == nullptr)
    {
        return JsErrorNoCurrentContext;
    }

    Js::ScriptContext *scriptContext = currentContext->GetScriptContext();
    Assert(scriptContext != nullptr);
    Recycler *recycler = scriptContext->GetRecycler();
    ThreadContext *threadContext = scriptContext->GetThreadContext();

#ifndef JSRT_VERIFY_RUNTIME_STATE
    if (recycler && recycler->IsInObjectBeforeCollectCallback())
    {
        return JsErrorInObjectBeforeCollectCallback;
    }
#endif

    if (recycler && recycler->IsHeapEnumInProgress())
    {
        return JsErrorHeapEnumInProgress;
    }
    else if (threadContext->IsInThreadServiceCallback())
    {
        return JsErrorInThreadServiceCallback;
    }

    if (threadContext->IsExecutionDisabled())
    {
        return JsErrorInDisabledState;
    }

    *hasException = scriptContext->HasRecordedException();

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsGetAndClearException(_Out_ JsValueRef *exception)
{
    PARAM_NOT_NULL(exception);
    *exception = nullptr;

    JsrtContext *currentContext = JsrtContext::GetCurrent();

    if (currentContext == nullptr)
    {
        return JsErrorNoCurrentContext;
    }

    Js::ScriptContext *scriptContext = currentContext->GetScriptContext();
    Assert(scriptContext != nullptr);

    if (scriptContext->GetRecycler() && scriptContext->GetRecycler()->IsHeapEnumInProgress())
    {
        return JsErrorHeapEnumInProgress;
    }
    else if (scriptContext->GetThreadContext()->IsInThreadServiceCallback())
    {
        return JsErrorInThreadServiceCallback;
    }

    if (scriptContext->GetThreadContext()->IsExecutionDisabled())
    {
        return JsErrorInDisabledState;
    }

    int32_t hr = S_OK;
    Js::JavascriptExceptionObject *recordedException = nullptr;

    BEGIN_TRANSLATE_OOM_TO_HRESULT
      if (scriptContext->HasRecordedException())
      {
          recordedException = scriptContext->GetAndClearRecordedException();
      }
    END_TRANSLATE_OOM_TO_HRESULT(hr)

    if (hr == E_OUTOFMEMORY)
    {
        recordedException = scriptContext->GetThreadContext()->GetRecordedException();
    }
    if (recordedException == nullptr)
    {
        return JsErrorInvalidArgument;
    }

    *exception = recordedException->GetThrownObject(nullptr);

    if (*exception == nullptr)
    {
        return JsErrorInvalidArgument;
    }

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsSetException(_In_ JsValueRef exception)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext* scriptContext) -> JsErrorCode {
        JsrtContext * context = JsrtContext::GetCurrent();
        JsrtRuntime * runtime = context->GetRuntime();

        ;

        VALIDATE_INCOMING_REFERENCE(exception, scriptContext);

        Js::JavascriptExceptionObject *exceptionObject;
        exceptionObject = RecyclerNew(scriptContext->GetRecycler(), Js::JavascriptExceptionObject, exception, scriptContext, nullptr);

        scriptContext->RecordException(exceptionObject, runtime->DispatchExceptions());

        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetRuntimeMemoryUsage(_In_ JsRuntimeHandle runtimeHandle, _Out_ size_t * memoryUsage)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);
    PARAM_NOT_NULL(memoryUsage);
    *memoryUsage = 0;

    ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
    AllocationPolicyManager * allocPolicyManager = threadContext->GetAllocationPolicyManager();

    *memoryUsage = allocPolicyManager->GetUsage();

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsSetRuntimeMemoryLimit(_In_ JsRuntimeHandle runtimeHandle, _In_ size_t memoryLimit)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

    ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
    AllocationPolicyManager * allocPolicyManager = threadContext->GetAllocationPolicyManager();

    allocPolicyManager->SetLimit(memoryLimit);

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsGetRuntimeMemoryLimit(_In_ JsRuntimeHandle runtimeHandle, _Out_ size_t * memoryLimit)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);
    PARAM_NOT_NULL(memoryLimit);
    *memoryLimit = 0;

    ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
    AllocationPolicyManager * allocPolicyManager = threadContext->GetAllocationPolicyManager();

    *memoryLimit = allocPolicyManager->GetLimit();

    return JsNoError;
}

static_assert(JsMemoryAllocate == (_JsMemoryEventType) AllocationPolicyManager::MemoryAllocateEvent::MemoryAllocate);
static_assert(JsMemoryFree == (_JsMemoryEventType) AllocationPolicyManager::MemoryAllocateEvent::MemoryFree);
static_assert(JsMemoryFailure == (_JsMemoryEventType) AllocationPolicyManager::MemoryAllocateEvent::MemoryFailure);
static_assert(JsMemoryFailure == (_JsMemoryEventType) AllocationPolicyManager::MemoryAllocateEvent::MemoryMax);

JsErrorCode chakracore::jsrt::JsSetRuntimeMemoryAllocationCallback(_In_ JsRuntimeHandle runtime, _In_opt_ void *callbackState, _In_ JsMemoryAllocationCallback allocationCallback)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtime);

    ThreadContext* threadContext = JsrtRuntime::FromHandle(runtime)->GetThreadContext();
    AllocationPolicyManager * allocPolicyManager = threadContext->GetAllocationPolicyManager();

    allocPolicyManager->SetMemoryAllocationCallback(callbackState, (AllocationPolicyManager::PageAllocatorMemoryAllocationCallback)allocationCallback);

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsSetRuntimeBeforeCollectCallback(_In_ JsRuntimeHandle runtime, _In_opt_ void *callbackState, _In_ JsBeforeCollectCallback beforeCollectCallback)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        VALIDATE_INCOMING_RUNTIME_HANDLE(runtime);

        JsrtRuntime::FromHandle(runtime)->SetBeforeCollectCallback(beforeCollectCallback, callbackState);
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsDisableRuntimeExecution(_In_ JsRuntimeHandle runtimeHandle)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

    ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
    if (!threadContext->TestThreadContextFlag(ThreadContextFlagCanDisableExecution))
    {
        return JsErrorCannotDisableExecution;
    }

    if (threadContext->GetRecycler() && threadContext->GetRecycler()->IsHeapEnumInProgress())
    {
        return JsErrorHeapEnumInProgress;
    }
    else if (threadContext->IsInThreadServiceCallback())
    {
        return JsErrorInThreadServiceCallback;
    }

    threadContext->DisableExecution();
    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsEnableRuntimeExecution(_In_ JsRuntimeHandle runtimeHandle)
{
    return GlobalAPIWrapper_NoRecord([&] () -> JsErrorCode {
        VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);

        ThreadContext * threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
        if (!threadContext->TestThreadContextFlag(ThreadContextFlagCanDisableExecution))
        {
            return JsNoError;
        }

        if (threadContext->GetRecycler() && threadContext->GetRecycler()->IsHeapEnumInProgress())
        {
            return JsErrorHeapEnumInProgress;
        }
        else if (threadContext->IsInThreadServiceCallback())
        {
            return JsErrorInThreadServiceCallback;
        }

        ThreadContextScope scope(threadContext);

        if (!scope.IsValid())
        {
            return JsErrorWrongThread;
        }

        threadContext->EnableExecution();
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsIsRuntimeExecutionDisabled(_In_ JsRuntimeHandle runtimeHandle, _Out_ bool *isDisabled)
{
    VALIDATE_INCOMING_RUNTIME_HANDLE(runtimeHandle);
    PARAM_NOT_NULL(isDisabled);
    *isDisabled = false;

    ThreadContext* threadContext = JsrtRuntime::FromHandle(runtimeHandle)->GetThreadContext();
    *isDisabled = threadContext->IsExecutionDisabled();
    return JsNoError;
}

inline JsErrorCode JsGetPropertyIdFromNameInternal(_In_z_ const char16_t *name, size_t cPropertyNameLength, _Out_ JsPropertyIdRef *propertyId)
{
    return ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext * scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(name);
        PARAM_NOT_NULL(propertyId);
        *propertyId = nullptr;

        if (cPropertyNameLength <= INT_MAX)
        {
            scriptContext->GetOrAddPropertyRecord(name, static_cast<int>(cPropertyNameLength), (Js::PropertyRecord const **)propertyId);

            return JsNoError;
        }
        else
        {
            return JsErrorOutOfMemory;
        }
    });
}

JsErrorCode JsGetPropertyIdFromName(_In_z_ const char16_t *name, _Out_ JsPropertyIdRef *propertyId)
{
    return JsGetPropertyIdFromNameInternal(name, std::u16string(name).length(), propertyId);
}

JsErrorCode chakracore::jsrt::JsGetPropertyIdFromSymbol(_In_ JsValueRef symbol, _Out_ JsPropertyIdRef *propertyId)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_REFERENCE(symbol, scriptContext);
        PARAM_NOT_NULL(propertyId);
        *propertyId = nullptr;

        if (!Js::VarIs<Js::JavascriptSymbol>(symbol))
        {
            return JsErrorPropertyNotSymbol;
        }

        *propertyId = (JsPropertyIdRef)Js::VarTo<Js::JavascriptSymbol>(symbol)->GetValue();
        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode chakracore::jsrt::JsGetSymbolFromPropertyId(_In_ JsPropertyIdRef propertyId, _Out_ JsValueRef *symbol)
{
    return ContextAPINoScriptWrapper([&](Js::ScriptContext * scriptContext) -> JsErrorCode {
        ;

        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(symbol);
        *symbol = nullptr;

        Js::PropertyRecord const * propertyRecord = (Js::PropertyRecord const *)propertyId;
        if (!propertyRecord->IsSymbol())
        {
            return JsErrorPropertyNotSymbol;
        }

        *symbol = scriptContext->GetSymbol(propertyRecord);
        return JsNoError;
    });
}

#pragma prefast(suppress:6101, "Prefast doesn't see through the lambda")
JsErrorCode JsGetPropertyNameFromId(_In_ JsPropertyIdRef propertyId, const char16_t **name)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        VALIDATE_INCOMING_PROPERTYID(propertyId);
        PARAM_NOT_NULL(name);
        *name = nullptr;

        Js::PropertyRecord const * propertyRecord = (Js::PropertyRecord const *)propertyId;

        if (propertyRecord->IsSymbol())
        {
            return JsErrorPropertyNotString;
        }

        *name = propertyRecord->GetBuffer();
        return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsGetPropertyIdType(_In_ JsPropertyIdRef propertyId, _Out_ JsPropertyIdType* propertyIdType)
{
    return GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        VALIDATE_INCOMING_PROPERTYID(propertyId);

        Js::PropertyRecord const * propertyRecord = (Js::PropertyRecord const *)propertyId;

        if (propertyRecord->IsSymbol())
        {
            *propertyIdType = JsPropertyIdTypeSymbol;
        }
        else
        {
            *propertyIdType = JsPropertyIdTypeString;
        }
        return JsNoError;
    });
}


JsErrorCode chakracore::jsrt::JsGetRuntime(_In_ JsContextRef context, _Out_ JsRuntimeHandle *runtime)
{
    VALIDATE_JSREF(context);
    PARAM_NOT_NULL(runtime);

    *runtime = nullptr;

    if (!JsrtContext::Is(context))
    {
        return JsErrorInvalidArgument;
    }

    *runtime = static_cast<JsrtContext *>(context)->GetRuntime();
    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsIdle(_Out_opt_ unsigned int *nextIdleTick)
{
    PARAM_NOT_NULL(nextIdleTick);

    return ContextAPINoScriptWrapper_NoRecord([&] (Js::ScriptContext * scriptContext) -> JsErrorCode {

            *nextIdleTick = 0;

            if (scriptContext->GetThreadContext()->GetRecycler() && scriptContext->GetThreadContext()->GetRecycler()->IsHeapEnumInProgress())
            {
                return JsErrorHeapEnumInProgress;
            }
            else if (scriptContext->GetThreadContext()->IsInThreadServiceCallback())
            {
                return JsErrorInThreadServiceCallback;
            }

            JsrtContext * context = JsrtContext::GetCurrent();
            JsrtRuntime * runtime = context->GetRuntime();

            if (!runtime->UseIdle())
            {
                return JsErrorIdleNotEnabled;
            }

            unsigned int ticks = runtime->Idle();

            *nextIdleTick = ticks;

            return JsNoError;
    });
}

JsErrorCode chakracore::jsrt::JsSetPromiseContinuationCallback(_In_opt_ JsPromiseContinuationCallback promiseContinuationCallback, _In_opt_ void *callbackState)
{
    return ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext * scriptContext) -> JsErrorCode {
        scriptContext->GetLibrary()->SetNativeHostPromiseContinuationFunction((Js::JavascriptLibrary::PromiseContinuationCallback)promiseContinuationCallback, callbackState);
        return JsNoError;
    },
    /*allowInObjectBeforeCollectCallback*/true);
}

JsErrorCode RunScriptCore(JsValueRef scriptSource, const byte *script, size_t cb,
    LoadScriptFlag loadScriptFlag, JsSourceContext sourceContext,
    const char16_t *sourceUrl, bool parseOnly, JsParseScriptAttributes parseAttributes,
    bool isSourceModule, JsValueRef *result)
{
    Js::JavascriptFunction *scriptFunction;
    CompileScriptException se;

    JsErrorCode errorCode = ContextAPINoScriptWrapper([&](Js::ScriptContext * scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(script);
        PARAM_NOT_NULL(sourceUrl);

        SourceContextInfo * sourceContextInfo = scriptContext->GetSourceContextInfo(sourceContext, nullptr);

        if (sourceContextInfo == nullptr)
        {
            sourceContextInfo = scriptContext->CreateSourceContextInfo(sourceContext, sourceUrl, std::u16string(sourceUrl).length(), nullptr);
        }

        const int chsize = (loadScriptFlag & LoadScriptFlag_Utf8Source) ?
            sizeof(utf8char_t) : sizeof(char16_t);

        SRCINFO si = {
            /* sourceContextInfo   */ sourceContextInfo,
            /* dlnHost             */ 0,
            /* ulColumnHost        */ 0,
            /* lnMinHost           */ 0,
            /* ichMinHost          */ 0,
            /* ichLimHost          */ static_cast<uint32_t>(cb / chsize), // OK to truncate since this is used to limit sourceText in debugDocument/compilation errors.
            /* ulCharOffset        */ 0,
            /* mod                 */ kmodGlobal,
            /* grfsi               */ 0
        };

        Js::Utf8SourceInfo* utf8SourceInfo = nullptr;
        if (result != nullptr)
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_Expression);
        }
        bool isLibraryCode = (parseAttributes & JsParseScriptAttributeLibraryCode) == JsParseScriptAttributeLibraryCode;
        bool isStrictMode = (parseAttributes & JsParseScriptAttributeStrictMode) == JsParseScriptAttributeStrictMode;
        if (isLibraryCode)
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_LibraryCode);
        }
        if (isSourceModule)
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_Module);
        }
        if (isStrictMode)
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_StrictMode);
        }

        scriptFunction = scriptContext->LoadScript(script, cb,
            &si, &se, &utf8SourceInfo,
            Js::Constants::GlobalCode, loadScriptFlag, scriptSource);

        JsrtContext * context = JsrtContext::GetCurrent();
        context->OnScriptLoad(scriptFunction, utf8SourceInfo, &se);

        return JsNoError;
    });

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    return ContextAPIWrapper<false>([&](Js::ScriptContext* scriptContext) -> JsErrorCode {
        if (scriptFunction == nullptr)
        {
            ;

            HandleScriptCompileError(scriptContext, &se, sourceUrl);
            return JsErrorScriptCompile;
        }

        if (parseOnly)
        {
            PARAM_NOT_NULL(result);
            *result = scriptFunction;
        }
        else
        {
            Js::Arguments args(0, nullptr);
            Js::Var varThis;
            if (PHASE_FORCE1(Js::EvalCompilePhase))
            {
                varThis = Js::JavascriptOperators::OP_GetThis(scriptContext->GetLibrary()->GetUndefined(), kmodGlobal, scriptContext);
                args.Info.Flags = (Js::CallFlags)Js::CallFlags::CallFlags_Eval;
                args.Info.Count = 1;
                args.Values = &varThis;
            }

            Js::Var varResult = scriptFunction->CallRootFunction(args, scriptContext, true);
            if (result != nullptr)
            {
                *result = varResult;
            }
        }
        return JsNoError;
    });
}

JsErrorCode RunScriptCore(const char *script, JsSourceContext sourceContext,
    const char *sourceUrl, bool parseOnly, JsParseScriptAttributes parseAttributes,
    bool isSourceModule, JsValueRef *result)
{
    utf8::NarrowToWide url((const char *)sourceUrl);
    if (!url)
    {
        return JsErrorOutOfMemory;
    }

    return RunScriptCore(nullptr, reinterpret_cast<const byte*>(script), strlen(script),
        LoadScriptFlag_Utf8Source, sourceContext, url, parseOnly, parseAttributes,
        isSourceModule, result);
}

JsErrorCode RunScriptCore(const char16_t *script, JsSourceContext sourceContext,
    const char16_t *sourceUrl, bool parseOnly, JsParseScriptAttributes parseAttributes,
    bool isSourceModule, JsValueRef *result)
{
    return RunScriptCore(nullptr, reinterpret_cast<const byte*>(script),
        std::u16string(script).length() * sizeof(char16_t),
        LoadScriptFlag_None, sourceContext, sourceUrl, parseOnly,
        parseAttributes, isSourceModule, result);
}

JsErrorCode GetScriptBufferDetails(
    _In_ JsValueRef scriptVal,
    _In_ JsParseScriptAttributes parseAttributes,
    _Out_ LoadScriptFlag* scriptFlag,
    _Out_ size_t* cb,
    _Out_ const byte** script)
{
    PARAM_NOT_NULL(scriptFlag);
    PARAM_NOT_NULL(cb);
    PARAM_NOT_NULL(script);

    *scriptFlag = LoadScriptFlag_None;
    *cb = 0;
    *script = nullptr;

    const bool isExternalArray = Js::VarIs<Js::ArrayBuffer>(scriptVal);
    const bool isString = !isExternalArray && Js::VarIs<Js::JavascriptString>(scriptVal);
    if (!isExternalArray && !isString)
    {
        return JsErrorInvalidArgument;
    }
    const bool isUtf8 = !isString && !(parseAttributes & JsParseScriptAttributeArrayBufferIsUtf16Encoded);

    *script = isExternalArray ?
        ((Js::ExternalArrayBuffer*)(scriptVal))->GetBuffer() :
        (const byte*)((Js::JavascriptString*)(scriptVal))->GetSz();
    *cb = isExternalArray ?
        ((Js::ExternalArrayBuffer*)(scriptVal))->GetByteLength() :
        ((Js::JavascriptString*)(scriptVal))->GetSizeInBytes();

    if (isExternalArray && isUtf8)
    {
        *scriptFlag = (LoadScriptFlag)(LoadScriptFlag_ExternalArrayBuffer | LoadScriptFlag_Utf8Source);
    }
    else if (isUtf8)
    {
        *scriptFlag = (LoadScriptFlag)(LoadScriptFlag_Utf8Source);
    }
    else
    {
        *scriptFlag = LoadScriptFlag_None;
    }

    return JsNoError;
}

JsErrorCode JsSerializeScriptCore(const byte *script, size_t cb,
    LoadScriptFlag loadScriptFlag, uint8_t *functionTable, int functionTableSize,
    unsigned char *buffer, unsigned int *bufferSize, JsValueRef scriptSource)
{
    Js::JavascriptFunction *function;
    CompileScriptException se;

    JsErrorCode errorCode = ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(script);
        PARAM_NOT_NULL(bufferSize);

        if (*bufferSize > 0)
        {
            PARAM_NOT_NULL(buffer);
            memset((buffer),0,(*bufferSize));
        }

        if (scriptContext->IsScriptContextInDebugMode())
        {
            return JsErrorCannotSerializeDebugScript;
        }

        SourceContextInfo * sourceContextInfo = scriptContext->GetSourceContextInfo(JS_SOURCE_CONTEXT_NONE, nullptr);
        Assert(sourceContextInfo != nullptr);
        sourceContextInfo->nextLocalFunctionId = 0;

        const int chsize = (loadScriptFlag & LoadScriptFlag_Utf8Source) ? sizeof(utf8char_t) : sizeof(char16_t);
        SRCINFO si = {
            /* sourceContextInfo   */ sourceContextInfo,
            /* dlnHost             */ 0,
            /* ulColumnHost        */ 0,
            /* lnMinHost           */ 0,
            /* ichMinHost          */ 0,
            /* ichLimHost          */ static_cast<uint32_t>(cb / chsize), // OK to truncate since this is used to limit sourceText in debugDocument/compilation errors.
            /* ulCharOffset        */ 0,
            /* mod                 */ kmodGlobal,
            /* grfsi               */ 0
        };
        bool isSerializeByteCodeForLibrary = false;
        isSerializeByteCodeForLibrary = JsrtContext::GetCurrent()->GetRuntime()->IsSerializeByteCodeForLibrary();

        Js::Utf8SourceInfo* sourceInfo = nullptr;
        loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_disableDeferredParse);
        if (isSerializeByteCodeForLibrary)
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_isByteCodeBufferForLibrary);
        }
        else
        {
            loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_Expression);
        }
        function = scriptContext->LoadScript(script, cb, &si, &se, &sourceInfo,
            Js::Constants::GlobalCode, loadScriptFlag, scriptSource);
        return JsNoError;
    });

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    return ContextAPIWrapper_NoRecord<false>([&](Js::ScriptContext* scriptContext) -> JsErrorCode {
        if (function == nullptr)
        {
            HandleScriptCompileError(scriptContext, &se);
            return JsErrorScriptCompile;
        }
        // Could we have a deserialized function in this case?
        // If we are going to serialize it, a check isn't to expensive
        if (CONFIG_FLAG(ForceSerialized) && function->GetFunctionProxy() != nullptr) {
            function->GetFunctionProxy()->EnsureDeserialized();
        }
        Js::FunctionBody *functionBody = function->GetFunctionBody();
        const Js::Utf8SourceInfo *sourceInfo = functionBody->GetUtf8SourceInfo();
        size_t cSourceCodeLength = sourceInfo->GetCbLength(u"JsSerializeScript");

        // truncation of code length can lead to accessing random memory. Reject the call.
        if (cSourceCodeLength > UINT_MAX)
        {
            return JsErrorOutOfMemory;
        }

        LPCUTF8 utf8Code = sourceInfo->GetSource(u"JsSerializeScript");
        uint32_t dwFlags = 0;
        dwFlags = JsrtContext::GetCurrent()->GetRuntime()->IsSerializeByteCodeForLibrary() ? GENERATE_BYTE_CODE_BUFFER_LIBRARY : 0;

        BEGIN_TEMP_ALLOCATOR(tempAllocator, scriptContext, u"ByteCodeSerializer");
        // We cast buffer size to uint32_t* because on Windows, uint32_t = unsigned long = unsigned int
        // On 64-bit clang on linux, this is not true, unsigned long is larger than unsigned int
        // However, the PAL defines uint32_t for us on linux as unsigned int so the cast is safe here.
        int32_t hr = Js::ByteCodeSerializer::SerializeToBuffer(scriptContext,
            tempAllocator, static_cast<uint32_t>(cSourceCodeLength), utf8Code,
            functionBody, functionBody->GetHostSrcInfo(), &buffer,
            (uint32_t*) bufferSize, dwFlags);
        END_TEMP_ALLOCATOR(tempAllocator, scriptContext);

        if (SUCCEEDED(hr))
        {
            return JsNoError;
        }
        else
        {
            return JsErrorScriptCompile;
        }
    });
}

JsErrorCode JsSerializeScript(_In_z_ const char16_t *script, _Out_writes_to_opt_(*bufferSize,
    *bufferSize) unsigned char *buffer,
    _Inout_ unsigned int *bufferSize)
{
    return JsSerializeScriptCore((const byte*)script, std::u16string(script).length() * sizeof(char16_t),
        LoadScriptFlag_None, nullptr, 0, buffer, bufferSize, nullptr);
}

template <typename TLoadCallback, typename TUnloadCallback>
JsErrorCode RunSerializedScriptCore(
    TLoadCallback scriptLoadCallback, TUnloadCallback scriptUnloadCallback,
    JsSourceContext scriptLoadSourceContext, // only used by scriptLoadCallback
    unsigned char *buffer, Js::ArrayBuffer* bufferVal,
    JsSourceContext sourceContext, const char16_t *sourceUrl,
    uint32_t bgParseCookie,
    bool parseOnly, bool useParserStateCache, JsValueRef *result,
    uint sourceIndex)
{
    Js::JavascriptFunction *function;
    JsErrorCode errorCode = ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext *scriptContext) -> JsErrorCode {

        if (result != nullptr)
        {
            *result = nullptr;
        }

        if (bgParseCookie == 0)
        {
            PARAM_NOT_NULL(buffer);
        }
        else
        {
            Assert(buffer == nullptr);
        }

        PARAM_NOT_NULL(sourceUrl);
        Js::ISourceHolder *sourceHolder = nullptr;
        SRCINFO *hsi = nullptr;

        PARAM_NOT_NULL(scriptLoadCallback);
        PARAM_NOT_NULL(scriptUnloadCallback);
        typedef Js::JsrtSourceHolder<TLoadCallback, TUnloadCallback> TSourceHolder;


        if (!useParserStateCache || bgParseCookie != 0)
        {
            sourceIndex = Js::Constants::InvalidSourceIndex;

            sourceHolder = RecyclerNewFinalized(scriptContext->GetRecycler(), TSourceHolder,
                scriptLoadCallback, scriptUnloadCallback, scriptLoadSourceContext, bufferVal);

            SourceContextInfo *sourceContextInfo = scriptContext->GetSourceContextInfo(sourceContext, nullptr);

            if (sourceContextInfo == nullptr)
            {
                sourceContextInfo = scriptContext->CreateSourceContextInfo(sourceContext, sourceUrl,
                    std::u16string(sourceUrl).length(), nullptr);
            }

            SRCINFO si = {
                /* sourceContextInfo   */ sourceContextInfo,
                /* dlnHost             */ 0,
                /* ulColumnHost        */ 0,
                /* lnMinHost           */ 0,
                /* ichMinHost          */ 0,
                /* ichLimHost          */ 0, // xplat-todo: need to compute this?
                /* ulCharOffset        */ 0,
                /* mod                 */ kmodGlobal,
                /* grfsi               */ 0
            };

            hsi = scriptContext->AddHostSrcInfo(&si);
        }
        else
        {
            Assert(sourceIndex != Js::Constants::InvalidSourceIndex);
        }

        int32_t hr;

        typename WriteBarrierFieldTypeTraits<Js::FunctionBody*>::Type functionBody = nullptr;

        if (bgParseCookie == 0)
        {
            uint32_t flags = 0;

            if (!scriptContext->IsProfiling())
            {
                flags = fscrAllowFunctionProxy;
            }
            if (useParserStateCache && !CONFIG_FLAG(ForceSerialized))
            {
                flags |= fscrCreateParserState;
            }

            hr = Js::ByteCodeSerializer::DeserializeFromBuffer(scriptContext, flags, sourceHolder,
                hsi, buffer, nullptr, &functionBody, sourceIndex);
        }
        else
        {
            size_t srcLength = 0;
            Js::FunctionBody* functionBodyLocal = nullptr;
            hr = BGParseManager::GetBGParseManager()->GetParseResults(
                scriptContext,
                bgParseCookie,
                nullptr, // pszSrc
                hsi,
                &functionBodyLocal,
                nullptr, // pse
                srcLength,
                nullptr, // utf8sourceinfo
                sourceIndex
            );
            
            if (hr == S_OK)
            {
                functionBody = functionBodyLocal;
            }
        }

        if (FAILED(hr))
        {
            return JsErrorBadSerializedScript;
        }

        function = scriptContext->GetLibrary()->CreateScriptFunction(functionBody);

        JsrtContext * context = JsrtContext::GetCurrent();
        context->OnScriptLoad(function, functionBody->GetUtf8SourceInfo(), nullptr);

        return JsNoError;
    });

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    return ContextAPIWrapper_NoRecord<false>([&](Js::ScriptContext* scriptContext) -> JsErrorCode {
        if (parseOnly)
        {
            PARAM_NOT_NULL(result);
            *result = function;
        }
        else
        {
            Js::Var varResult = function->CallRootFunction(Js::Arguments(0, nullptr), scriptContext, true);
            if (result != nullptr)
            {
                *result = varResult;
            }
        }
        return JsNoError;
    });
}

static void DummyScriptUnloadCallback(_In_ JsSourceContext sourceContext)
{
    // Do nothing
}

/////////////////////

template <class CopyFunc>
JsErrorCode WriteStringCopy(
    JsValueRef value,
    int start,
    int length,
    _Out_opt_ size_t* written,
    const CopyFunc& copyFunc)
{
    if (written)
    {
        *written = 0;  // init to 0 for default
    }

    const char16_t* str = nullptr;
    size_t strLength = 0;
    JsErrorCode errorCode = JsStringToPointer(value, &str, &strLength);
    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    if (start < 0 || (size_t)start > strLength)
    {
        return JsErrorInvalidArgument;  // start out of range, no chars written
    }

    size_t count = min(static_cast<size_t>(length), strLength - start);
    if (count == 0)
    {
        return JsNoError;  // no chars written
    }

    errorCode = copyFunc(str + start, count, written);
    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    if (written)
    {
        *written = count;
    }

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsCopyStringUtf16(
    _In_ JsValueRef value,
    _In_ int start,
    _In_ int length,
    _Out_opt_ uint16_t* buffer,
    _Out_opt_ size_t* written)
{
    PARAM_NOT_NULL(value);
    VALIDATE_JSREF(value);

    return WriteStringCopy(value, start, length, written,
        [buffer](const char16_t* src, size_t count, size_t *needed)
        {
            if (buffer)
            {
                memmove(buffer, src, sizeof(char16_t) * count);
            }
            return JsNoError;
        });
}

JsErrorCode chakracore::jsrt::JsCopyString(
    _In_ JsValueRef value,
    _Out_opt_ char* buffer,
    _In_ size_t bufferSize,
    _Out_opt_ size_t* length)
{
    PARAM_NOT_NULL(value);
    VALIDATE_JSREF(value);

    const char16_t* str = nullptr;
    size_t strLength = 0;
    JsErrorCode errorCode = JsStringToPointer(value, &str, &strLength);
    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    utf8::WideToNarrow utf8Str(str, strLength, buffer, bufferSize);
    if (length)
    {
        *length = utf8Str.Length();
    }

    return JsNoError;
}

inline JsErrorCode CompileRun(
    JsValueRef scriptVal,
    JsSourceContext sourceContext,
    JsValueRef sourceUrl,
    JsParseScriptAttributes parseAttributes,
    _Out_ JsValueRef *result,
    bool parseOnly)
{
    PARAM_NOT_NULL(scriptVal);
    VALIDATE_JSREF(scriptVal);
    PARAM_NOT_NULL(sourceUrl);

    bool isExternalArray = Js::VarIs<Js::ArrayBuffer>(scriptVal),
        isString = false;
    bool isUtf8 = !(parseAttributes & JsParseScriptAttributeArrayBufferIsUtf16Encoded);

    LoadScriptFlag scriptFlag = LoadScriptFlag_None;
    const byte* script;
    size_t cb;
    const char16_t *url;

    if (isExternalArray)
    {
        script = ((Js::ExternalArrayBuffer*)(scriptVal))->GetBuffer();

        cb = ((Js::ExternalArrayBuffer*)(scriptVal))->GetByteLength();

        scriptFlag = (LoadScriptFlag)(isUtf8 ?
            LoadScriptFlag_ExternalArrayBuffer | LoadScriptFlag_Utf8Source :
            LoadScriptFlag_ExternalArrayBuffer);
    }
    else
    {
        isString = Js::VarIs<Js::JavascriptString>(scriptVal);
        if (!isString)
        {
            return JsErrorInvalidArgument;
        }
    }

    JsErrorCode error = GlobalAPIWrapper_NoRecord([&]() -> JsErrorCode {
        if (isString)
        {
            Js::JavascriptString* jsString = Js::VarTo<Js::JavascriptString>(scriptVal);
            script = (const byte*)jsString->GetSz();

            // JavascriptString is 2 bytes (char16_t/char16_t)
            cb = jsString->GetLength() * sizeof(char16_t);
        }

        if (!Js::VarIs<Js::JavascriptString>(sourceUrl))
        {
            return JsErrorInvalidArgument;
        }

        url = Js::VarTo<Js::JavascriptString>(sourceUrl)->GetSz();

        return JsNoError;

    });

    if (error != JsNoError)
    {
        return error;
    }

    return RunScriptCore(scriptVal, script, cb, scriptFlag,
        sourceContext, url, parseOnly, parseAttributes, false, result);
}

JsErrorCode chakracore::jsrt::JsParse(
    _In_ JsValueRef scriptVal,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _In_ JsParseScriptAttributes parseAttributes,
    _Out_ JsValueRef *result)
{
    return CompileRun(scriptVal, sourceContext, sourceUrl, parseAttributes,
        result, true);
}

JsErrorCode chakracore::jsrt::JsRun(
    _In_ JsValueRef scriptVal,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _In_ JsParseScriptAttributes parseAttributes,
    _Out_ JsValueRef *result)
{
    return CompileRun(scriptVal, sourceContext, sourceUrl, parseAttributes,
        result, false);
}

JsErrorCode chakracore::jsrt::JsCreatePropertyId(
    _In_z_ const char *name,
    _In_ size_t length,
    _Out_ JsPropertyIdRef *propertyId)
{
    PARAM_NOT_NULL(name);
    utf8::NarrowToWide wname(name, length);
    if (!wname)
    {
        return JsErrorOutOfMemory;
    }

    return JsGetPropertyIdFromNameInternal(wname, wname.Length(), propertyId);
}

JsErrorCode chakracore::jsrt::JsCopyPropertyId(
    _In_ JsPropertyIdRef propertyId,
    _Out_ char* buffer,
    _In_ size_t bufferSize,
    _Out_ size_t* length)
{
    PARAM_NOT_NULL(propertyId);

    const char16_t* str = nullptr;
    JsErrorCode errorCode = JsGetPropertyNameFromId(propertyId, &str);

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    utf8::WideToNarrow utf8Str(str);
    if (!buffer)
    {
        if (length)
        {
            *length = utf8Str.Length();
        }
    }
    else
    {
        size_t count = min(bufferSize, utf8Str.Length());
        // Try to copy whole characters if buffer size insufficient
        auto maxFitChars = utf8::ByteIndexIntoCharacterIndex(
            (LPCUTF8)(const char*)utf8Str, count,
            utf8::DecodeOptions::doChunkedEncoding);
        count = utf8::CharacterIndexToByteIndex(
            (LPCUTF8)(const char*)utf8Str, utf8Str.Length(), maxFitChars);

        memmove(buffer, utf8Str, sizeof(char) * count);
        if (length)
        {
            *length = count;
        }
    }

    return JsNoError;
}

JsErrorCode chakracore::jsrt::JsSerialize(
    _In_ JsValueRef scriptVal,
    _Out_ JsValueRef *bufferVal,
    _In_ JsParseScriptAttributes parseAttributes)
{
    PARAM_NOT_NULL(scriptVal);
    PARAM_NOT_NULL(bufferVal);
    VALIDATE_JSREF(scriptVal);

    *bufferVal = nullptr;

    const byte* script = nullptr;
    size_t cb = 0;
    LoadScriptFlag scriptFlag = LoadScriptFlag_None;

    JsErrorCode errorCode = GetScriptBufferDetails(scriptVal, parseAttributes,
        &scriptFlag, &cb, &script);

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    unsigned int bufferSize = 0;
    errorCode = JsSerializeScriptCore(script, cb, scriptFlag, nullptr,
        0, nullptr, &bufferSize, scriptVal);

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    if (bufferSize == 0)
    {
        return JsErrorScriptCompile;
    }

    if ((errorCode = JsCreateArrayBuffer(bufferSize, bufferVal)) == JsNoError)
    {
        byte* buffer = ((Js::ArrayBuffer*)(*bufferVal))->GetBuffer();
        errorCode = JsSerializeScriptCore(script, cb, scriptFlag, nullptr,
            0, buffer, &bufferSize, scriptVal);
    }

    return errorCode;
}

JsErrorCode chakracore::jsrt::JsParseSerialized(
    _In_ JsValueRef bufferVal,
    _In_ JsSerializedLoadScriptCallback scriptLoadCallback,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _Out_ JsValueRef *result)
{
    PARAM_NOT_NULL(bufferVal);
    PARAM_NOT_NULL(sourceUrl);

    const char16_t *url;

    if (Js::VarIs<Js::JavascriptString>(sourceUrl))
    {
        url = ((Js::JavascriptString*)(sourceUrl))->GetSz();
    }
    else
    {
        return JsErrorInvalidArgument;
    }

    // JsParseSerialized only accepts ArrayBuffer (incl. ExternalArrayBuffer)
    if (!Js::VarIs<Js::ArrayBuffer>(bufferVal))
    {
        return JsErrorInvalidArgument;
    }

    Js::ArrayBuffer* arrayBuffer = Js::VarTo<Js::ArrayBuffer>(bufferVal);
    byte* buffer = arrayBuffer->GetBuffer();

    return RunSerializedScriptCore(
      scriptLoadCallback, DummyScriptUnloadCallback,
      sourceContext,// use the same user provided sourceContext as scriptLoadSourceContext
      buffer, arrayBuffer, sourceContext, url, 0, true, false, result, Js::Constants::InvalidSourceIndex);
}

JsErrorCode chakracore::jsrt::JsRunSerialized(
    _In_ JsValueRef bufferVal,
    _In_ JsSerializedLoadScriptCallback scriptLoadCallback,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _Out_ JsValueRef *result)
{
    PARAM_NOT_NULL(bufferVal);
    const char16_t *url;

    if (sourceUrl && Js::VarIs<Js::JavascriptString>(sourceUrl))
    {
        url = ((Js::JavascriptString*)(sourceUrl))->GetSz();
    }
    else
    {
        return JsErrorInvalidArgument;
    }

    // JsParseSerialized only accepts ArrayBuffer (incl. ExternalArrayBuffer)
    if (!Js::VarIs<Js::ArrayBuffer>(bufferVal))
    {
        return JsErrorInvalidArgument;
    }

    Js::ArrayBuffer* arrayBuffer = Js::VarTo<Js::ArrayBuffer>(bufferVal);
    byte* buffer = arrayBuffer->GetBuffer();

    return RunSerializedScriptCore(
        scriptLoadCallback, DummyScriptUnloadCallback,
        sourceContext, // use the same user provided sourceContext as scriptLoadSourceContext
        buffer, arrayBuffer, sourceContext, url, 0, false, false, result, Js::Constants::InvalidSourceIndex);
}


JsErrorCode chakracore::jsrt::JsCopyStringOneByte(
    _In_ JsValueRef value,
    _In_ int start,
    _In_ int length,
    _Out_opt_ char* buffer,
    _Out_opt_ size_t* written)
{
    PARAM_NOT_NULL(value);
    VALIDATE_JSREF(value);
    return WriteStringCopy(value, start, length, written,
        [buffer](const char16_t* src, size_t count, size_t *needed)
    {
        if (buffer)
        {
            for (size_t i = 0; i < count; i++)
            {
                buffer[i] = (char)src[i];
            }
        }
        return JsNoError;
    });
}

JsErrorCode JsSerializeParserStateCore(
    _In_z_ const byte* script,
    _In_ size_t cb,
    _In_ LoadScriptFlag loadScriptFlag,
    _Out_writes_to_opt_(*bufferSize, *bufferSize) unsigned char *buffer,
    _Inout_ unsigned int *bufferSize)
{
    Js::JavascriptFunction *function;
    CompileScriptException se;

    return ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        PARAM_NOT_NULL(script);
        PARAM_NOT_NULL(bufferSize);

        if (*bufferSize > 0)
        {
            PARAM_NOT_NULL(buffer);
            memset((buffer),0,(*bufferSize));
        }

        if (scriptContext->IsScriptContextInDebugMode())
        {
            return JsErrorCannotSerializeDebugScript;
        }

        SourceContextInfo * sourceContextInfo = scriptContext->GetSourceContextInfo(JS_SOURCE_CONTEXT_NONE, nullptr);
        Assert(sourceContextInfo != nullptr);
        sourceContextInfo->nextLocalFunctionId = 0;

        const int chsize = (loadScriptFlag & LoadScriptFlag_Utf8Source) ?
            sizeof(utf8char_t) : sizeof(char16_t);

        SRCINFO si = {
            /* sourceContextInfo   */ sourceContextInfo,
            /* dlnHost             */ 0,
            /* ulColumnHost        */ 0,
            /* lnMinHost           */ 0,
            /* ichMinHost          */ 0,
            /* ichLimHost          */ static_cast<uint32_t>(cb / chsize), // OK to truncate since this is used to limit sourceText in debugDocument/compilation errors.
            /* ulCharOffset        */ 0,
            /* mod                 */ kmodGlobal,
            /* grfsi               */ 0
        };

        Js::Utf8SourceInfo* sourceInfo = nullptr;
        loadScriptFlag = (LoadScriptFlag)(loadScriptFlag | LoadScriptFlag_CreateParserState);

        BEGIN_TEMP_ALLOCATOR(tempAllocator, scriptContext, u"ByteCodeSerializer");
        // We cast buffer size to uint32_t* because on Windows, uint32_t = unsigned long = unsigned int
        // On 64-bit clang on linux, this is not true, unsigned long is larger than unsigned int
        // However, the PAL defines uint32_t for us on linux as unsigned int so the cast is safe here.
        int32_t hr = scriptContext->SerializeParserState(script, cb, &si, &se, &sourceInfo,
            Js::Constants::GlobalCode, loadScriptFlag, &buffer, (uint32_t*)bufferSize, tempAllocator, &function, nullptr);
        END_TEMP_ALLOCATOR(tempAllocator, scriptContext);

        if (function == nullptr)
        {
            HandleScriptCompileError(scriptContext, &se);
            return JsErrorScriptCompile;
        }

        Js::FunctionBody *functionBody = function->GetFunctionBody();
        sourceInfo = functionBody->GetUtf8SourceInfo();
        size_t cSourceCodeLength = sourceInfo->GetCbLength(u"JsSerializeParserState");

        // truncation of code length can lead to accessing random memory. Reject the call.
        if (cSourceCodeLength > UINT_MAX)
        {
            return JsErrorOutOfMemory;
        }

        if (SUCCEEDED(hr))
        {
            return JsNoError;
        }
        else
        {
            return JsErrorScriptCompile;
        }
    });
}

JsErrorCode chakracore::jsrt::JsSerializeParserState(
    _In_ JsValueRef scriptVal,
    _Out_ JsValueRef *bufferVal,
    _In_ JsParseScriptAttributes parseAttributes)
{
    PARAM_NOT_NULL(scriptVal);
    PARAM_NOT_NULL(bufferVal);
    VALIDATE_JSREF(scriptVal);

    *bufferVal = nullptr;

    const byte* script = nullptr;
    size_t cb = 0;
    LoadScriptFlag scriptFlag = LoadScriptFlag_None;

    JsErrorCode errorCode = GetScriptBufferDetails(scriptVal, parseAttributes,
        &scriptFlag, &cb, &script);

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    unsigned int bufferSize = 0;
    errorCode = JsSerializeParserStateCore(script, cb, scriptFlag, nullptr,
        &bufferSize);

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    if (bufferSize == 0)
    {
        return JsErrorScriptCompile;
    }

    if ((errorCode = JsCreateArrayBuffer(bufferSize, bufferVal)) == JsNoError)
    {
        byte* buffer = ((Js::ArrayBuffer*)(*bufferVal))->GetBuffer();
        errorCode = JsSerializeParserStateCore(script, cb, scriptFlag, buffer,
            &bufferSize);
    }

    return errorCode;
}

static bool DummyScriptLoadSourceCallbackForRunScriptWithParserState(
    JsSourceContext sourceContext,
    _Out_ JsValueRef *value,
    _Out_ JsParseScriptAttributes *parseAttributes)
{
    *value = nullptr;
    *parseAttributes = JsParseScriptAttributeNone;
    return true;
}

JsErrorCode RunScriptWithParserStateCore(
    _In_ uint32_t dwBgParseCookie,
    _In_ JsValueRef script,
    _In_ JsSourceContext sourceContext,
    _In_ char16_t *url,
    _In_ JsParseScriptAttributes parseAttributes,
    _In_ JsValueRef parserState,
    _In_ bool parseOnly,
    _Out_ JsValueRef *result
)
{
    PARAM_NOT_NULL(script);

    if (dwBgParseCookie == 0)
    {
        PARAM_NOT_NULL(parserState);
    }

    uint sourceIndex = 0;

    JsErrorCode errorCode = ContextAPINoScriptWrapper_NoRecord([&](Js::ScriptContext *scriptContext) -> JsErrorCode {
        const byte* bytes;
        size_t cb;
        LoadScriptFlag loadScriptFlag;

        JsErrorCode errorCode = GetScriptBufferDetails(script, parseAttributes, &loadScriptFlag, &cb, &bytes);
        if (errorCode != JsNoError)
        {
            return errorCode;
        }

        SourceContextInfo* sourceContextInfo = scriptContext->GetSourceContextInfo(sourceContext, nullptr);

        if (sourceContextInfo == nullptr)
        {
            sourceContextInfo = scriptContext->CreateSourceContextInfo(sourceContext, url, std::u16string(url).length(), nullptr);
        }

        const int chsize = (loadScriptFlag & LoadScriptFlag_Utf8Source) ?
            sizeof(utf8char_t) : sizeof(char16_t);

        SRCINFO si = {
            /* sourceContextInfo   */ sourceContextInfo,
            /* dlnHost             */ 0,
            /* ulColumnHost        */ 0,
            /* lnMinHost           */ 0,
            /* ichMinHost          */ 0,
            /* ichLimHost          */ static_cast<uint32_t>(cb / chsize), // OK to truncate since this is used to limit sourceText in debugDocument/compilation errors.
            /* ulCharOffset        */ 0,
            /* mod                 */ kmodGlobal,
            /* grfsi               */ 0
        };

        Js::Utf8SourceInfo* utf8SourceInfo = nullptr;
        scriptContext->MakeUtf8SourceInfo(bytes, cb, &si, &utf8SourceInfo, loadScriptFlag, script);

        if (utf8SourceInfo == nullptr)
        {
            return JsErrorInvalidArgument;
        }

        uint32_t grfscr = scriptContext->GetParseFlags(loadScriptFlag, utf8SourceInfo, sourceContextInfo);
        utf8SourceInfo->SetParseFlags(grfscr);

        if ((loadScriptFlag & LoadScriptFlag_Utf8Source) != LoadScriptFlag_Utf8Source)
        {
            sourceIndex = scriptContext->SaveSourceNoCopy(utf8SourceInfo, static_cast<charcount_t>(utf8SourceInfo->GetCchLength()), /*isCesu8*/ true);
        }
        else
        {
            // TODO: This length may not be correct because we could have actually parsed a different number of characters
            sourceIndex = scriptContext->SaveSourceNoCopy(utf8SourceInfo, static_cast<charcount_t>(utf8SourceInfo->GetCchLength()), /* isCesu8*/ false);
        }

        return JsNoError;
    });

    if (errorCode != JsNoError)
    {
        return errorCode;
    }

    Js::ArrayBuffer* arrayBuffer = nullptr;
    byte* buffer = nullptr;
    if (dwBgParseCookie == 0)
    {
        if (!Js::VarIs<Js::ArrayBuffer>(parserState))
        {
            return JsErrorInvalidArgument;
        }

        arrayBuffer = Js::VarTo<Js::ArrayBuffer>(parserState);
        buffer = arrayBuffer->GetBuffer();
    }

    JsSerializedLoadScriptCallback dummy = DummyScriptLoadSourceCallbackForRunScriptWithParserState;

    return RunSerializedScriptCore(
        dummy, DummyScriptUnloadCallback,
        sourceContext, // use the same user provided sourceContext as scriptLoadSourceContext
        buffer, arrayBuffer, sourceContext, url, dwBgParseCookie, parseOnly, true, result, sourceIndex);
}

JsErrorCode chakracore::jsrt::JsRunScriptWithParserState(
    _In_ JsValueRef script,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _In_ JsParseScriptAttributes parseAttributes,
    _In_ JsValueRef parserState,
    _Out_ JsValueRef *result)
{
    char16_t *url = nullptr;
    if (sourceUrl && Js::VarIs<Js::JavascriptString>(sourceUrl))
    {
        url = const_cast<char16_t*>(((Js::JavascriptString*)(sourceUrl))->GetSz());
        return RunScriptWithParserStateCore(0, script, sourceContext, url, parseAttributes, parserState, false, result);
    }
    else
    {
        return JsErrorInvalidArgument;
    }
}

JsErrorCode chakracore::jsrt::JsDeserializeParserState(
    _In_ JsValueRef script,
    _In_ JsSourceContext sourceContext,
    _In_ JsValueRef sourceUrl,
    _In_ JsParseScriptAttributes parseAttributes,
    _In_ JsValueRef parserState,
    _Out_ JsValueRef * result)
{
    char16_t *url = nullptr;
    if (sourceUrl && Js::VarIs<Js::JavascriptString>(sourceUrl))
    {
        url = const_cast<char16_t*>(((Js::JavascriptString*)(sourceUrl))->GetSz());
        return RunScriptWithParserStateCore(0, script, sourceContext, url, parseAttributes, parserState, true, result);
    }
    else
    {
        return JsErrorInvalidArgument;
    }
}

JsErrorCode chakracore::jsrt::JsExecuteBackgroundParse_Experimental(
    _In_ uint32_t dwBgParseCookie,
    _In_ JsValueRef script,
    _In_ JsSourceContext sourceContext,
    _In_ char16_t *url,
    _In_ JsParseScriptAttributes parseAttributes,
    _In_ JsValueRef parserState,
    _Out_ JsValueRef *result)
{
    int32_t hr = BGParseManager::GetBGParseManager()->GetInputFromCookie(dwBgParseCookie, nullptr, nullptr, &url);
    if (hr == S_OK)
    {
        return RunScriptWithParserStateCore(
            dwBgParseCookie,
            script,
            sourceContext,
            url,
            parseAttributes,
            parserState,
            false,
            result
        );
    }
    else
    {
        return JsErrorFatal;
    }
}
