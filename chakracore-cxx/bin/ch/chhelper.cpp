//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "chhelper.h"
#include "WScriptJsrt.h"

#include <filesystem>
#include <print>

#include <chakracore-sys/src/logger.rs.h>
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

static int32_t CreateRuntime(JsRuntimeHandle *runtime, const JsRuntimeAttributes jsrtAttributes)
{
    int32_t hr = E_FAIL;

    // On Posix, malloc optimistically returns a non-null address without
    // checking if it's actually able to back that allocation in memory
    // Upon use of that address however, if the address space for that allocation
    // can't be committed, the process is killed
    // See the man page for malloc
    //
    // In order to avoid having to deal with this, we set the memory limit for the
    // runtime to the size of the physical memory on the system
    // TODO:
    // We could move the following into its own platform agnostic API
    // but in this case, this is a one-time call thats not applicable
    // on Windows so decided to leave as is
    // Additionally, we can probably do better than just limit to the physical memory
    // size

#if defined(__APPLE__) || defined(__linux__)
    size_t memoryLimit;
#ifdef __APPLE__
    int totalRamHW[] = {CTL_HW, HW_MEMSIZE};
    size_t length = sizeof(memoryLimit);
    if (sysctl(totalRamHW, 2, &memoryLimit, &length, NULL, 0) == -1)
    {
        memoryLimit = 0;
    }
#else
    struct sysinfo sysInfo;
    if (sysinfo(&sysInfo) == -1)
    {
        memoryLimit = 0;
    }
    else
    {
        memoryLimit = sysInfo.totalram;
    }
#endif // __APPLE__
#endif // __APPLE__ || __linux

    IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(jsrtAttributes, nullptr, runtime));

    IfJsErrorFailLog(ChakraRTInterface::JsSetRuntimeMemoryLimit(*runtime, memoryLimit));

    hr = S_OK;
Error:
    return hr;
}

int32_t GetParserStateBuffer(const rust::String &fileContents, JsFinalizeCallback fileContentsFinalizeCallback,
                             JsValueRef *parserStateBuffer)
{
    int32_t hr = S_OK;
    JsValueRef scriptSource = nullptr;

    IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer(fileContents, fileContentsFinalizeCallback, &scriptSource));
    IfJsErrorFailLog(
        ChakraRTInterface::JsSerializeParserState(scriptSource, parserStateBuffer, JsParseScriptAttributeNone));

Error:
    return hr;
}

int32_t CreateParserStateAndRunScript(const rust::Str fileName,
                                      const rust::String &contents,
                                      const rust::String &fullPath, JsRuntimeHandle &chRuntime,
                                      const JsRuntimeAttributes jsrtAttributes)
{
    auto span = chakra::Span::create("CreateParserStateAndRunScript");
    int32_t hr = S_OK;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef context = JS_INVALID_REFERENCE, current = JS_INVALID_REFERENCE;
    JsValueRef bufferVal;

    // We don't want this to free fileContents when it completes, so the finalizeCallback is nullptr
    IfFailedGoLabel(GetParserStateBuffer(contents, nullptr, &bufferVal), ErrorRunFinalize);

    // Bytecode buffer is created in one runtime and will be executed on different runtime.
    IfFailedGoLabel(CreateRuntime(&runtime, jsrtAttributes), ErrorRunFinalize);
    chRuntime = runtime;

    IfJsErrorFailLogLabel(ChakraRTInterface::JsCreateContext(runtime, &context), ErrorRunFinalize);
    IfJsErrorFailLogLabel(ChakraRTInterface::JsGetCurrentContext(&current), ErrorRunFinalize);
    IfJsErrorFailLogLabel(ChakraRTInterface::JsSetCurrentContext(context), ErrorRunFinalize);

    // Initialized the WScript object on the new context
    if (!WScriptJsrt::Initialize())
    {
        IfFailedGoLabel(E_FAIL, ErrorRunFinalize);
    }

    // This is our last call to use fileContents, so pass in the finalizeCallback
    IfFailGo(RunScript(fileName, contents, nullptr, fullPath, bufferVal));

    if (false)
    {
    ErrorRunFinalize:
    }
Error:
    if (current != JS_INVALID_REFERENCE)
    {
        ChakraRTInterface::JsSetCurrentContext(current);
    }

    if (runtime != JS_INVALID_RUNTIME_HANDLE)
    {
        ChakraRTInterface::JsDisposeRuntime(runtime);
    }

    return hr;
}

int32_t CreateAndRunSerializedScript(const rust::Str fileName,
                                     const rust::String &contents,
                                     const rust::String &fullPath, JsRuntimeHandle &runtime,
                                     JsValueRef bufferVal)
{
    auto span = chakra::Span::create("CreateAndRunSerializedScript");
    int32_t hr = S_OK;
    JsContextRef context = JS_INVALID_REFERENCE, current = JS_INVALID_REFERENCE;

    IfJsErrorFailLogLabel(ChakraRTInterface::JsCreateContext(runtime, &context), ErrorRunFinalize);
    IfJsErrorFailLogLabel(ChakraRTInterface::JsGetCurrentContext(&current), ErrorRunFinalize);
    IfJsErrorFailLogLabel(ChakraRTInterface::JsSetCurrentContext(context), ErrorRunFinalize);

    // Initialized the WScript object on the new context
    if (!WScriptJsrt::Initialize())
    {
        IfFailedGoLabel(E_FAIL, ErrorRunFinalize);
    }

    // This is our last call to use fileContents, so pass in the finalizeCallback
    IfFailGo(RunScript(fileName, contents, bufferVal, fullPath, nullptr));

    if (false)
    {
    ErrorRunFinalize:
    }
Error:
    if (current != JS_INVALID_REFERENCE)
    {
        ChakraRTInterface::JsSetCurrentContext(current);
    }

    if (runtime != JS_INVALID_RUNTIME_HANDLE)
    {
        ChakraRTInterface::JsDisposeRuntime(runtime);
    }

    return hr;
}
