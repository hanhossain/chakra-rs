//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeThreadData.h"

#include <utility>

#include "ChakraRtInterface.h"
#include "WScriptJsrt.h"
#include "chakra/Logger.h"
#include "chakracore-sys/src/wscript_jsrt.rs.h"

RuntimeThreadData::RuntimeThreadData() :
    hThread(nullptr),
    sharedContent_(nullptr),
    receiveBroadcastCallbackFunc(nullptr),
    leaving_(false)
{
    this->hevntReceivedBroadcast = CreateEventW(FALSE, FALSE);
    this->hevntShutdown = CreateEventW(TRUE, FALSE);
}

RuntimeThreadData::~RuntimeThreadData()
{
    if (!children_.empty()) {
        std::vector<HANDLE> childrenHandles;

        for (const auto &child : children_) {
            childrenHandles.push_back(child->hThread);
            SetEvent(child->hevntShutdown);
        }

        uint32_t waitRet = WaitForMultipleObjects(childrenHandles.size(), childrenHandles.data(), TRUE, INFINITE);
        assert(waitRet == WAIT_OBJECT_0);

        children_.clear();
    }

    CloseHandle(this->hevntReceivedBroadcast);
    CloseHandle(this->hevntShutdown);
    CloseHandle(this->hThread);
}

std::shared_ptr<RuntimeThreadData> RuntimeThreadData::New() {
    return std::make_shared<RuntimeThreadData>();
}

uint32_t RuntimeThreadData::ThreadProc(JsRuntimeHandle runtime) const {
    auto span = chakra::Span::create("RuntimeThreadData::ThreadProc");

    // loop waiting for work;

    while (true)
    {
        HANDLE handles[] = { this->hevntReceivedBroadcast, this->hevntShutdown };
        uint32_t waitRet = WaitForMultipleObjects(std::size(handles), handles, false, INFINITE);

        if (waitRet == WAIT_OBJECT_0)
        {
            JsValueRef args[3];
            ChakraRTInterface::JsGetGlobalObject(&args[0]);
            {
                auto parent = parent_.lock();
                ChakraRTInterface::JsCreateSharedArrayBufferWithSharedContent(parent->get_shared_content(), &args[1]);
            }
            ChakraRTInterface::JsDoubleToNumber(1, &args[2]);

            if (this->receiveBroadcastCallbackFunc)
            {
                ChakraRTInterface::JsCallFunction(this->receiveBroadcastCallbackFunc, args, 3, nullptr);
            }
        }

        if (waitRet == WAIT_OBJECT_0 + 1 || leaving_)
        {
            if (this->receiveBroadcastCallbackFunc)
            {
                ChakraRTInterface::JsRelease(this->receiveBroadcastCallbackFunc, nullptr);
            }
            ChakraRTInterface::JsSetCurrentContext(nullptr);
            ChakraRTInterface::JsDisposeRuntime(runtime);
            return 0;
        }
        else if (waitRet != WAIT_OBJECT_0)
        {
            assert(false);
            break;
        }
    }

    ChakraRTInterface::JsSetCurrentContext(nullptr);
    ChakraRTInterface::JsDisposeRuntime(runtime);
    return 0;
}

void RuntimeThreadData::set_leaving(bool leaving)
{
    leaving_ = leaving;
}

void RuntimeThreadData::enqueue_report_to_parent(rust::String report)
{
    auto parent = parent_.lock();
    if (parent)
    {
        std::unique_lock lease{parent->csReportQ_};
        parent->reportQ_.push_back(std::move(report));
    }
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

void RuntimeThreadData::broadcast_to_children()
{
    for (const auto &child : children_)
    {
        SetEvent(child->hevntReceivedBroadcast);
    }
}

void RuntimeThreadData::set_shared_content(JsSharedArrayBufferContentHandle shared_content)
{
    sharedContent_ = shared_content;
}

JsSharedArrayBufferContentHandle RuntimeThreadData::get_shared_content()
{
    return sharedContent_;
}
JsValueRef RuntimeThreadData::get_receive_broadcast_callback_func() const
{
    return receiveBroadcastCallbackFunc;
}

void RuntimeThreadData::set_receive_broadcast_callback_func(JsValueRef value)
{
    receiveBroadcastCallbackFunc = value;
}

void RuntimeThreadData::add_child(const std::shared_ptr<RuntimeThreadData> &child) {
    children_.push_back(child);
}
void RuntimeThreadData::set_parent(const std::weak_ptr<RuntimeThreadData> &parentThread) {
    parent_ = parentThread;
}
void RuntimeThreadData::set_thread_handle(HANDLE thread) {
    hThread = thread;
}
