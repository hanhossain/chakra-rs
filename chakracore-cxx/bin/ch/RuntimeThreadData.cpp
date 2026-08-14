//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "RuntimeThreadData.h"

#include "ChakraRtInterface.h"
#include "Helpers.h"
#include "WScriptJsrt.h"

void RuntimeThreadLocalData::Initialize(RuntimeThreadData* threadData)
{
    this->threadData = threadData;
}

void RuntimeThreadLocalData::Uninitialize()
{
}

thread_local RuntimeThreadLocalData threadLocalData;

RuntimeThreadLocalData& GetRuntimeThreadLocalData()
{
    return threadLocalData;
}

RuntimeThreadData::RuntimeThreadData() :
    semaphore(std::nullopt),
    hThread(nullptr),
    sharedContent(nullptr),
    receiveBroadcastCallbackFunc(nullptr),
    runtime(nullptr),
    context(nullptr),
    parent(nullptr),
    leaving(false)
{
    this->hevntReceivedBroadcast = CreateEventW(FALSE, FALSE);
    this->hevntShutdown = CreateEventW(TRUE, FALSE);

    InitializeCriticalSection(&csReportQ);
}

RuntimeThreadData::~RuntimeThreadData()
{
    CloseHandle(this->hevntReceivedBroadcast);
    CloseHandle(this->hevntShutdown);
    CloseHandle(this->hThread);
    DeleteCriticalSection(&csReportQ);
}

uint32_t RuntimeThreadData::ThreadProc()
{
    JsValueRef scriptSource;
    JsValueRef fname;
    const char* fullPath = "agent source";
    int32_t hr = S_OK;

    threadLocalData.Initialize(this);

    IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
    IfJsErrorFailLog(ChakraRTInterface::JsCreateContext(runtime, &context));
    IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));


    if (!WScriptJsrt::Initialize())
    {
        IfFailGo(E_FAIL);
    }


    IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer((void*)this->initialSource.c_str(),
        (unsigned int)this->initialSource.size(), nullptr, nullptr, &scriptSource));


    ChakraRTInterface::JsCreateString(fullPath, strlen(fullPath), &fname);

    ChakraRTInterface::JsRun(scriptSource, WScriptJsrt::GetNextSourceContext(), fname, JsParseScriptAttributeNone, nullptr);

    this->parent->set_initial_script_completed();

    // loop waiting for work;

    while (true)
    {
        HANDLE handles[] = { this->hevntReceivedBroadcast, this->hevntShutdown };
        uint32_t waitRet = WaitForMultipleObjects(std::size(handles), handles, false, INFINITE);

        if (waitRet == WAIT_OBJECT_0)
        {
            JsValueRef args[3];
            ChakraRTInterface::JsGetGlobalObject(&args[0]);
            ChakraRTInterface::JsCreateSharedArrayBufferWithSharedContent(this->parent->sharedContent, &args[1]);
            ChakraRTInterface::JsDoubleToNumber(1, &args[2]);

            // notify the parent we received the data
            parent->semaphore->release();

            if (this->receiveBroadcastCallbackFunc)
            {
                ChakraRTInterface::JsCallFunction(this->receiveBroadcastCallbackFunc, args, 3, nullptr);
            }
        }

        if (waitRet == WAIT_OBJECT_0 + 1 || this->leaving)
        {
            WScriptJsrt::Uninitialize();

            if (this->receiveBroadcastCallbackFunc)
            {
                ChakraRTInterface::JsRelease(this->receiveBroadcastCallbackFunc, nullptr);
            }
            ChakraRTInterface::JsSetCurrentContext(nullptr);
            ChakraRTInterface::JsDisposeRuntime(runtime);

            threadLocalData.Uninitialize();
            return 0;
        }
        else if (waitRet != WAIT_OBJECT_0)
        {
            assert(false);
            break;
        }
    }

Error:

    ChakraRTInterface::JsSetCurrentContext(nullptr);
    ChakraRTInterface::JsDisposeRuntime(runtime);
    threadLocalData.Uninitialize();
    return 0;
}

void RuntimeThreadData::set_initial_script_completed()
{
    {
        std::lock_guard lock(initial_script_completed_mtx_);
        initial_script_completed_ = true;
    }
    initial_script_completed_cv_.notify_all();
}

void RuntimeThreadData::reset_initial_script_completed()
{
    std::unique_lock lock(initial_script_completed_mtx_);
    initial_script_completed_ = false;
}

void RuntimeThreadData::wait_initial_script_completed()
{
    std::unique_lock lock(initial_script_completed_mtx_);
    initial_script_completed_cv_.wait(lock, [this] { return initial_script_completed_; });
}
