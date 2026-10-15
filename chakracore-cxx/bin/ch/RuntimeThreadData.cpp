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
    sharedContent_(nullptr)
{
    this->hevntReceivedBroadcast = CreateEventW(FALSE, FALSE);
    this->hevntShutdown = CreateEventW(TRUE, FALSE);
}

RuntimeThreadData::~RuntimeThreadData()
{
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

void RuntimeThreadData::broadcast() const {
    SetEvent(hevntReceivedBroadcast);
}

void RuntimeThreadData::set_shared_content(JsSharedArrayBufferContentHandle shared_content)
{
    sharedContent_ = shared_content;
}

JsSharedArrayBufferContentHandle RuntimeThreadData::get_shared_content() const {
    return sharedContent_;
}

HANDLE RuntimeThreadData::get_received_broadcast_event() const {
    return hevntReceivedBroadcast;
}
HANDLE RuntimeThreadData::get_shutdown_event() const {
    return hevntShutdown;
}
