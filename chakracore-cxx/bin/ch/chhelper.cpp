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

#include <chakracore-sys/src/chhelper.rs.h>

unsigned int MessageBase::s_messageCount = 0;

static_assert(sizeof(ssize_t) == sizeof(long));

#define IfFailedGoLabel(expr, label) do { hr = (expr); if (FAILED(hr)) { goto label; } } while (FALSE)
#define IfFailGo(expr) IfFailedGoLabel(hr = (expr), Error)

int32_t RunScript(const rust::Str fileName, const rust::String &contents,
                  JsValueRef bufferValue,
                  const rust::String &fullPath, JsValueRef parserStateCache,
                  const std::unique_ptr<MessageQueue> &messageQueue, JsValueRef fname)
{
    auto span = chakra::Span::create("RunScript");
    JsFinalizeCallback fileContentsFinalizeCallback = WScriptJsrt::FinalizeFree;
    int32_t hr = S_OK;

    JsErrorCode runScript;

    if (bufferValue != nullptr)
    {
        // Now we can run our script, with this serializedCallbackInfo as the sourcecontext
        runScript = chakra_rs::chhelper::run_serialized(bufferValue, contents, fname);
    }
    else if (parserStateCache != nullptr)
    {
        JsValueRef scriptSource;
        IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer(contents, fileContentsFinalizeCallback, &scriptSource));

        runScript =
            ChakraRTInterface::JsRunScriptWithParserState(scriptSource, WScriptJsrt::GetNextSourceContext(), fname,
                                                          JsParseScriptAttributeNone, parserStateCache, nullptr);
    }
    else if (HostConfigFlags::coreConfig.module)
    {
        runScript = WScriptJsrt::ModuleEntryPoint(contents, fullPath);
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

Error:
    return hr;
}
