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
    explicit RuntimeThreadData(HANDLE thread);
    ~RuntimeThreadData();
    static std::shared_ptr<RuntimeThreadData> New();
    static std::shared_ptr<RuntimeThreadData> NewWithHandle(HANDLE thread);
    HANDLE hevntReceivedBroadcast;
    HANDLE hevntShutdown;
    HANDLE hThread;

    JsSharedArrayBufferContentHandle sharedContent_;
    JsValueRef receiveBroadcastCallbackFunc;
    std::weak_ptr<RuntimeThreadData> parent_;

    std::vector<std::shared_ptr<RuntimeThreadData>> children_;

    void broadcast_to_children() const;
    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content() const;
    JsValueRef get_receive_broadcast_callback_func() const;
    void set_receive_broadcast_callback_func(JsValueRef value);
    void add_child(const std::shared_ptr<RuntimeThreadData> &child);
    void set_parent(const std::weak_ptr<RuntimeThreadData> &parentThread);
    std::weak_ptr<RuntimeThreadData> get_parent() const;
    HANDLE get_received_broadcast_event() const;
    HANDLE get_shutdown_event() const;
};
