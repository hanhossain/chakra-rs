//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeThreadData.h"

#include <format>
#include <utility>

#include "ChakraRtInterface.h"
#include "Helpers.h"
#include "WScriptJsrt.h"
#include "chakra/Logger.h"
#include "chakracore-sys/src/wscript_jsrt.rs.h"

#define IfFailedGoLabel(expr, label) do { hr = (expr); if (FAILED(hr)) { goto label; } } while (FALSE)
#define IfFailGo(expr) IfFailedGoLabel(hr = (expr), Error)

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

RuntimeThreadData &GetCurrentRuntimeThreadData([[maybe_unused]] int &dummy)
{
    return *threadLocalData.threadData;
}

RuntimeThreadData::RuntimeThreadData(rust::String initialSource) :
    semaphore(std::nullopt),
    hThread(nullptr),
    sharedContent(nullptr),
    receiveBroadcastCallbackFunc(nullptr),
    runtime(nullptr),
    context(nullptr),
    parent(nullptr),
    leaving_(false),
    initialSource_(std::move(initialSource))
{
    this->hevntReceivedBroadcast = CreateEventW(FALSE, FALSE);
    this->hevntShutdown = CreateEventW(TRUE, FALSE);
}

RuntimeThreadData::RuntimeThreadData() : RuntimeThreadData(rust::String{})
{
}

RuntimeThreadData::~RuntimeThreadData()
{
    CloseHandle(this->hevntReceivedBroadcast);
    CloseHandle(this->hevntShutdown);
    CloseHandle(this->hThread);
}

uint32_t RuntimeThreadData::ThreadProc()
{
    auto span = chakra::Span::create("RuntimeThreadData::ThreadProc");
    JsValueRef scriptSource;
    JsValueRef fname;
    const char* fullPath = "agent source";
    int32_t hr = S_OK;

    threadLocalData.Initialize(this);

    IfJsErrorFailLog(ChakraRTInterface::JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
    IfJsErrorFailLog(ChakraRTInterface::JsCreateContext(runtime, &context));
    IfJsErrorFailLog(ChakraRTInterface::JsSetCurrentContext(context));

    try
    {
        chakra_rs::WScript::initialize();
    }
    catch (const rust::Error &err)
    {
        chakra::Logger::error(std::format("Caught exception from rust: {}", err.what()));
        IfFailGo(E_FAIL);
    }

    IfJsErrorFailLog(ChakraRTInterface::JsCreateExternalArrayBuffer(initialSource_, nullptr, &scriptSource));

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

        if (waitRet == WAIT_OBJECT_0 + 1 || leaving_)
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

void RuntimeThreadData::set_leaving(bool leaving)
{
    leaving_ = leaving;
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

void RuntimeThreadData::enqueue_report(rust::String report)
{
    std::unique_lock lease{csReportQ_};
    reportQ_.push_back(std::move(report));
}

bool RuntimeThreadData::dequeue_report(rust::String &report)
{
    std::unique_lock lease{csReportQ_};
    if (reportQ_.empty())
    {
        return false;
    }

    report = reportQ_.front();
    reportQ_.pop_front();
    return true;
}
