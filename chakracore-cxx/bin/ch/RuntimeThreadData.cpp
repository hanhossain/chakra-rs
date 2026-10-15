//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeThreadData.h"

#include "ChakraRtInterface.h"
#include "WScriptJsrt.h"
#include "chakracore-sys/src/wscript_jsrt.rs.h"

RuntimeThreadData::RuntimeThreadData() : RuntimeThreadData(nullptr) {}

RuntimeThreadData::RuntimeThreadData(HANDLE thread) :
    hThread(thread),
    sharedContent_(nullptr),
    receiveBroadcastCallbackFunc(nullptr)
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

        uint32_t waitRet = WaitForMultipleObjects(childrenHandles.size(), childrenHandles.data(), true);
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

std::shared_ptr<RuntimeThreadData> RuntimeThreadData::NewWithHandle(HANDLE thread) {
    return std::make_shared<RuntimeThreadData>(thread);
}

void RuntimeThreadData::broadcast_to_children() const {
    for (const auto &child : children_)
    {
        SetEvent(child->hevntReceivedBroadcast);
    }
}

void RuntimeThreadData::set_shared_content(JsSharedArrayBufferContentHandle shared_content)
{
    sharedContent_ = shared_content;
}

JsSharedArrayBufferContentHandle RuntimeThreadData::get_shared_content() const {
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
HANDLE RuntimeThreadData::get_received_broadcast_event() const {
    return hevntReceivedBroadcast;
}
HANDLE RuntimeThreadData::get_shutdown_event() const {
    return hevntShutdown;
}
