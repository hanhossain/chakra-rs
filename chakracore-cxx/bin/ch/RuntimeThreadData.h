//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "ChakraCore.h"

#include <condition_variable>
#include <memory>
#include <rust/cxx.h>

class RuntimeThreadData
{
public:
    RuntimeThreadData();
    ~RuntimeThreadData();
    static std::shared_ptr<RuntimeThreadData> New();
    HANDLE hevntReceivedBroadcast;
    HANDLE hevntShutdown;
    HANDLE hThread;

    JsSharedArrayBufferContentHandle sharedContent_;
    JsValueRef receiveBroadcastCallbackFunc;
    std::weak_ptr<RuntimeThreadData> parent_;

    std::vector<std::shared_ptr<RuntimeThreadData>> children_;

    uint32_t ThreadProc(
        JsRuntimeHandle runtime,
        rust::Fn<void(const std::weak_ptr<RuntimeThreadData> &parent, const JsValueRef &receiveBroadcastCallbackFunc)>
            onReceiveBroadcast,
        rust::Fn<void(const JsValueRef &receiveBroadcastCallbackFunc, JsRuntimeHandle runtime)> onShutdown) const;
    void set_leaving(bool leaving);
    void broadcast_to_children() const;
    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content() const;
    JsValueRef get_receive_broadcast_callback_func() const;
    void set_receive_broadcast_callback_func(JsValueRef value);
    void add_child(const std::shared_ptr<RuntimeThreadData> &child);
    void set_parent(const std::weak_ptr<RuntimeThreadData> &parentThread);
    void set_thread_handle(HANDLE thread);

private:
    bool leaving_;
};
