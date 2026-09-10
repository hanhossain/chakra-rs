//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "chhelper.h"
#include "WScriptJsrt.h"

#include <print>

#include <pthread.h>
#include <rust/cxx.h>

#include "ChakraRtInterface.h"
#include "Helpers.h"
#include "HostConfigFlags.h"
#include "MessageQueue.h"
#include "chakra/Logger.h"

#ifdef __linux__
#include <sys/sysinfo.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#endif

unsigned int MessageBase::s_messageCount = 0;

static_assert(sizeof(ssize_t) == sizeof(long));

#define IfFailedGoLabel(expr, label) do { hr = (expr); if (FAILED(hr)) { goto label; } } while (FALSE)
#define IfFailGo(expr) IfFailedGoLabel(hr = (expr), Error)

static bool DummyJsSerializedScriptLoadUtf8Source(JsSourceContext sourceContext, JsValueRef *scriptBuffer,
                                                  JsParseScriptAttributes *parseAttributes)
{
    auto *scriptBody = reinterpret_cast<const rust::String *>(sourceContext);

    // sourceContext is source ptr, see RunScript below
    if (ChakraRTInterface::JsCreateExternalArrayBuffer(*scriptBody, nullptr, scriptBuffer) != JsNoError)
    {
        return false;
    }

    *parseAttributes = JsParseScriptAttributeNone;
    return true;
}

int32_t RunScript(const rust::Str fileName, const rust::String &contents,
                  JsValueRef bufferValue,
                  const rust::String &fullPath, JsValueRef parserStateCache)
{
    auto span = chakra::Span::create("RunScript");
    JsFinalizeCallback fileContentsFinalizeCallback = WScriptJsrt::FinalizeFree;
    int32_t hr = S_OK;
    MessageQueue *messageQueue = new MessageQueue();
    WScriptJsrt::AddMessageQueue(messageQueue);

    IfJsErrorFailLogLabel(
        ChakraRTInterface::JsSetPromiseContinuationCallback(WScriptJsrt::PromiseContinuationCallback, messageQueue),
        ErrorRunFinalize);

    JsErrorCode runScript;
    JsValueRef fname;
    IfJsErrorFailLogLabel(ChakraRTInterface::JsCreateString(fullPath, &fname), ErrorRunFinalize);

    if (bufferValue != nullptr)
    {
        // Now we can run our script, with this serializedCallbackInfo as the sourcecontext
        runScript = ChakraRTInterface::JsRunSerialized(bufferValue, DummyJsSerializedScriptLoadUtf8Source,
                                                       reinterpret_cast<JsSourceContext>(&contents),
                                                       // Use source ptr as sourceContext
                                                       fname, nullptr /*result*/);
    }
    else if (parserStateCache != nullptr)
    {
        JsValueRef scriptSource;
        IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer(contents, fileContentsFinalizeCallback, &scriptSource));

        runScript =
            ChakraRTInterface::JsRunScriptWithParserState(scriptSource, WScriptJsrt::GetNextSourceContext(), fname,
                                                          JsParseScriptAttributeNone, parserStateCache, nullptr);
    }
    else if (HostConfigFlags::flags.Module)
    {
        // TODO (hanhossain): convert to rust::String
        runScript = WScriptJsrt::ModuleEntryPoint(contents, static_cast<std::string>(fullPath));
    }
    else // bufferValue == nullptr && parserStateCache == nullptr
    {
        JsValueRef scriptSource;
        IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer(contents, fileContentsFinalizeCallback, &scriptSource));

        runScript = ChakraRTInterface::JsRun(scriptSource, WScriptJsrt::GetNextSourceContext(), fname,
                                             JsParseScriptAttributeNone, nullptr /*result*/);
    }

    if (runScript != JsNoError)
    {
        WScriptJsrt::PrintException(fileName, runScript);
    }
    else
    {
        // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
        // because setTimeout can add scripts to execute.
        do
        {
            IfFailGo(messageQueue->ProcessAll(fileName));
        }
        while (!messageQueue->IsEmpty());
    }

    if (false)
    {
    ErrorRunFinalize:
    }
Error:
    if (messageQueue != nullptr)
    {
        messageQueue->RemoveAll();
        // clean up possible pinned exception object on exit to avoid potential leak
        bool hasException;
        if (ChakraRTInterface::JsHasException(&hasException) == JsNoError && hasException)
        {
            JsValueRef exception = JS_INVALID_REFERENCE;
            ChakraRTInterface::JsGetAndClearException(&exception);
        }
        delete messageQueue;
    }

    // We only call RunScript() once, safe to Uninitialize()
    WScriptJsrt::Uninitialize();

    return hr;
}
