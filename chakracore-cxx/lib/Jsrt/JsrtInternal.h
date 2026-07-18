//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "JsrtExceptionBase.h"
#include "Exceptions/EvalDisabledException.h"

#include <functional>

// JSRT Unsafe mode leaves runtime health-state responsibility to the host
#ifndef JSRT_VERIFY_RUNTIME_STATE
#define JSRT_MAYBE_TRUE false
#else
#define JSRT_MAYBE_TRUE true
#endif

#define PARAM_NOT_NULL(p) \
    if (p == nullptr) \
    { \
        return JsErrorNullArgument; \
    }

#define VALIDATE_JSREF(p) \
    if (p == JS_INVALID_REFERENCE) \
    { \
        return JsErrorInvalidArgument; \
    } \

#define MARSHAL_OBJECT(p, scriptContext) \
        Js::RecyclableObject* __obj = Js::VarTo<Js::RecyclableObject>(p); \
        if (__obj->GetScriptContext() != scriptContext) \
        { \
            if(__obj->GetScriptContext()->GetThreadContext() != scriptContext->GetThreadContext()) \
            {  \
                return JsErrorWrongRuntime;  \
            }  \
            p = Js::CrossSite::MarshalVar(scriptContext, __obj); \
        }

#define VALIDATE_INCOMING_RUNTIME_HANDLE(p) \
        { \
            if (p == JS_INVALID_RUNTIME_HANDLE) \
            { \
                return JsErrorInvalidArgument; \
            } \
        }

#define VALIDATE_INCOMING_PROPERTYID(p) \
          { \
            if (p == JS_INVALID_REFERENCE || \
                    Js::IsInternalPropertyId(((Js::PropertyRecord *)p)->GetPropertyId())) \
            { \
                return JsErrorInvalidArgument; \
            } \
        }

#define VALIDATE_INCOMING_REFERENCE(p, scriptContext) \
        { \
            VALIDATE_JSREF(p); \
            if (Js::VarIs<Js::RecyclableObject>(p)) \
            { \
                MARSHAL_OBJECT(p, scriptContext)   \
            } \
        }

#define VALIDATE_INCOMING_OBJECT(p, scriptContext) \
        { \
            VALIDATE_JSREF(p); \
            if (!Js::JavascriptOperators::IsObject(p)) \
            { \
                return JsErrorArgumentNotObject; \
            } \
            MARSHAL_OBJECT(p, scriptContext)   \
        }

#define VALIDATE_INCOMING_RECYCLABLE(p, scriptContext) \
{ \
    VALIDATE_JSREF(p); \
    if (!Js::VarIs<Js::RecyclableObject>(p)) \
    { \
        return JsErrorInvalidArgument; \
    } \
    MARSHAL_OBJECT(p, scriptContext)   \
}

#define VALIDATE_INCOMING_OBJECT_OR_NULL(p, scriptContext) \
        { \
            VALIDATE_JSREF(p); \
            if (!Js::JavascriptOperators::IsObjectOrNull(p)) \
            { \
                return JsErrorArgumentNotObject; \
            } \
            MARSHAL_OBJECT(p, scriptContext)   \
        }

#define VALIDATE_INCOMING_FUNCTION(p, scriptContext) \
        { \
            VALIDATE_JSREF(p); \
            if (!Js::VarIs<Js::JavascriptFunction>(p)) \
            { \
                return JsErrorInvalidArgument; \
            } \
            MARSHAL_OBJECT(p, scriptContext)   \
        }

inline JsErrorCode GlobalAPIWrapper(const std::function<JsErrorCode()>& fn)
{
    JsErrorCode errCode = JsNoError;
    try
    {
        // For now, treat this like an out of memory; consider if we should do something else here.

        AUTO_NESTED_HANDLED_EXCEPTION_TYPE((ExceptionType)(ExceptionType_OutOfMemory | ExceptionType_StackOverflow));

        errCode = fn();

        // These are error codes that should only be produced by the wrappers and should never
        // be produced by the internal calls.
        Assert(errCode != JsErrorFatal &&
            errCode != JsErrorNoCurrentContext &&
            errCode != JsErrorInExceptionState &&
            errCode != JsErrorInDisabledState &&
            errCode != JsErrorOutOfMemory &&
            errCode != JsErrorScriptException &&
            errCode != JsErrorScriptTerminated);
    }
    CATCH_STATIC_JAVASCRIPT_EXCEPTION_OBJECT(errCode)

    CATCH_OTHER_EXCEPTIONS(errCode)

    return errCode;
}

inline JsErrorCode GlobalAPIWrapper_NoRecord(std::function<JsErrorCode()> fn)
{
    return GlobalAPIWrapper([&fn]() -> JsErrorCode
    {
        return fn();
    });
}

JsErrorCode CheckContext(JsrtContext *currentContext, bool verifyRuntimeState, bool allowInObjectBeforeCollectCallback = false);

template <bool verifyRuntimeState>
JsErrorCode ContextAPIWrapper_Core(const std::function<JsErrorCode(Js::ScriptContext *)>& fn)
{
    JsrtContext *currentContext = JsrtContext::GetCurrent();
    JsErrorCode errCode = CheckContext(currentContext, verifyRuntimeState);

    if(errCode != JsNoError)
    {
        return errCode;
    }

    Js::ScriptContext *scriptContext = currentContext->GetScriptContext();
    try
    {
        AUTO_NESTED_HANDLED_EXCEPTION_TYPE((ExceptionType)(ExceptionType_OutOfMemory | ExceptionType_JavascriptException));

        // Enter script
        BEGIN_ENTER_SCRIPT(scriptContext, true, true, true)
        {
            errCode = fn(scriptContext);
        }
        END_ENTER_SCRIPT

            // These are error codes that should only be produced by the wrappers and should never
            // be produced by the internal calls.
            Assert(errCode != JsErrorFatal &&
                errCode != JsErrorNoCurrentContext &&
                errCode != JsErrorInExceptionState &&
                errCode != JsErrorInDisabledState &&
                errCode != JsErrorOutOfMemory &&
                errCode != JsErrorScriptException &&
                errCode != JsErrorScriptTerminated);
    }
    catch(Js::OutOfMemoryException)
    {
        errCode = JsErrorOutOfMemory;
    }
    catch(const Js::JavascriptException& err)
    {
        scriptContext->GetThreadContext()->SetRecordedException(err.GetAndClear());
        errCode = JsErrorScriptException;
    }
    catch(Js::ScriptAbortException)
    {
        Assert(scriptContext->GetThreadContext()->GetRecordedException() == nullptr);
        scriptContext->GetThreadContext()->SetRecordedException(scriptContext->GetThreadContext()->GetPendingTerminatedErrorObject());
        errCode = JsErrorScriptTerminated;
    }
    catch(Js::EvalDisabledException)
    {
        errCode = JsErrorScriptEvalDisabled;
    }

    CATCH_OTHER_EXCEPTIONS(errCode)

    return errCode;
}

template <bool verifyRuntimeState>
JsErrorCode ContextAPIWrapper(std::function<JsErrorCode(Js::ScriptContext *)> fn)
{
    JsErrorCode errCode = ContextAPIWrapper_Core<verifyRuntimeState>([&fn](Js::ScriptContext* scriptContext) -> JsErrorCode
    {
        return fn(scriptContext);
    });

    return errCode;
}

template <bool verifyRuntimeState>
JsErrorCode ContextAPIWrapper_NoRecord(std::function<JsErrorCode(Js::ScriptContext *)> fn)
{
    return ContextAPIWrapper_Core<verifyRuntimeState>([&fn](Js::ScriptContext* scriptContext) -> JsErrorCode
    {
        return fn(scriptContext);
    });
}

// allowInObjectBeforeCollectCallback only when current API is guaranteed not to do recycler allocation.
inline JsErrorCode ContextAPINoScriptWrapper_Core(const std::function<JsErrorCode(Js::ScriptContext *)>& fn, bool allowInObjectBeforeCollectCallback = false,
    bool scriptExceptionAllowed = false)
{
    JsrtContext *currentContext = JsrtContext::GetCurrent();
    JsErrorCode errCode = CheckContext(currentContext, /*verifyRuntimeState*/JSRT_MAYBE_TRUE,
        allowInObjectBeforeCollectCallback);

    if(errCode != JsNoError)
    {
        return errCode;
    }

    Js::ScriptContext *scriptContext = currentContext->GetScriptContext();
    try
    {
        // For now, treat this like an out of memory; consider if we should do something else here.

        AUTO_NESTED_HANDLED_EXCEPTION_TYPE((ExceptionType)(ExceptionType_OutOfMemory | ExceptionType_StackOverflow));

        errCode = fn(scriptContext);

        // These are error codes that should only be produced by the wrappers and should never
        // be produced by the internal calls.
        Assert(errCode != JsErrorFatal &&
            errCode != JsErrorNoCurrentContext &&
            errCode != JsErrorInExceptionState &&
            errCode != JsErrorInDisabledState &&
            errCode != JsErrorOutOfMemory &&
            (scriptExceptionAllowed || errCode != JsErrorScriptException) &&
            errCode != JsErrorScriptTerminated);
    }
    CATCH_STATIC_JAVASCRIPT_EXCEPTION_OBJECT(errCode)

    catch(const Js::JavascriptException& err)
    {
        AssertMsg(false, "Should never get JavascriptExceptionObject for ContextAPINoScriptWrapper.");
        scriptContext->GetThreadContext()->SetRecordedException(err.GetAndClear());
        errCode = JsErrorScriptException;
    }

    catch(Js::ScriptAbortException)
    {
        Assert(scriptContext->GetThreadContext()->GetRecordedException() == nullptr);
        scriptContext->GetThreadContext()->SetRecordedException(scriptContext->GetThreadContext()->GetPendingTerminatedErrorObject());
        errCode = JsErrorScriptTerminated;
    }

    CATCH_OTHER_EXCEPTIONS(errCode)

    return errCode;
}

inline JsErrorCode ContextAPINoScriptWrapper(std::function<JsErrorCode(Js::ScriptContext *)> fn, bool allowInObjectBeforeCollectCallback = false, bool scriptExceptionAllowed = false)
{
    JsErrorCode errCode = ContextAPINoScriptWrapper_Core([&fn](Js::ScriptContext* scriptContext) -> JsErrorCode
    {
        return fn(scriptContext);
    }, allowInObjectBeforeCollectCallback, scriptExceptionAllowed);

    return errCode;
}

inline JsErrorCode ContextAPINoScriptWrapper_NoRecord(std::function<JsErrorCode(Js::ScriptContext *)> fn, bool allowInObjectBeforeCollectCallback = false, bool scriptExceptionAllowed = false)
{
    return ContextAPINoScriptWrapper_Core([&fn](Js::ScriptContext* scriptContext) -> JsErrorCode
    {
        return fn(scriptContext);
    }, allowInObjectBeforeCollectCallback, scriptExceptionAllowed);
}

inline JsErrorCode SetContextAPIWrapper(JsrtContext* newContext, const std::function<JsErrorCode(Js::ScriptContext *)>& fn)
{
    JsrtContext* oldContext = JsrtContext::GetCurrent();
    Js::ScriptContext* scriptContext = newContext->GetScriptContext();

    JsErrorCode errorCode = JsNoError;
    try
    {
        // For now, treat this like an out of memory; consider if we should do something else here.

        AUTO_NESTED_HANDLED_EXCEPTION_TYPE((ExceptionType)(ExceptionType_OutOfMemory | ExceptionType_StackOverflow | ExceptionType_JavascriptException));
        if (JsrtContext::TrySetCurrent(newContext))
        {
            // Enter script
            BEGIN_ENTER_SCRIPT(scriptContext, true, true, true)
            {
                errorCode = fn(scriptContext);
            }
            END_ENTER_SCRIPT
        }
        else
        {
            errorCode = JsErrorWrongThread;
        }

        // These are error codes that should only be produced by the wrappers and should never
        // be produced by the internal calls.
        Assert(errorCode != JsErrorFatal &&
            errorCode != JsErrorNoCurrentContext &&
            errorCode != JsErrorInExceptionState &&
            errorCode != JsErrorInDisabledState &&
            errorCode != JsErrorOutOfMemory &&
            errorCode != JsErrorScriptException &&
            errorCode != JsErrorScriptTerminated);
    }
    catch (Js::OutOfMemoryException)
    {
        errorCode = JsErrorOutOfMemory;
    }
    catch (const Js::JavascriptException& err)
    {
        scriptContext->GetThreadContext()->SetRecordedException(err.GetAndClear());
        errorCode = JsErrorScriptException;
    }
    catch (Js::ScriptAbortException)
    {
        Assert(scriptContext->GetThreadContext()->GetRecordedException() == nullptr);
        scriptContext->GetThreadContext()->SetRecordedException(scriptContext->GetThreadContext()->GetPendingTerminatedErrorObject());
        errorCode = JsErrorScriptTerminated;
    }
    catch (Js::EvalDisabledException)
    {
        errorCode = JsErrorScriptEvalDisabled;
    }
    catch (Js::StackOverflowException)
    {
        return JsErrorOutOfMemory;
    }
    CATCH_OTHER_EXCEPTIONS(errorCode)
    AUTO_NESTED_HANDLED_EXCEPTION_TYPE((ExceptionType)(ExceptionType_OutOfMemory | ExceptionType_StackOverflow));
    JsrtContext::TrySetCurrent(oldContext);
    return errorCode;
}

void HandleScriptCompileError(Js::ScriptContext * scriptContext, CompileScriptException * se, const char16_t * sourceUrl = nullptr);

#if DBG
#define _PREPARE_RETURN_NO_EXCEPTION __debugCheckNoException.hasException = false;
#else
#define _PREPARE_RETURN_NO_EXCEPTION
#endif

#define BEGIN_JSRT_NO_EXCEPTION  BEGIN_NO_EXCEPTION
#define END_JSRT_NO_EXCEPTION    END_NO_EXCEPTION return JsNoError;
#define RETURN_NO_EXCEPTION(x)   _PREPARE_RETURN_NO_EXCEPTION return x
