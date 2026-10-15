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

    void broadcast() const;
    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content() const;
    HANDLE get_received_broadcast_event() const;
    HANDLE get_shutdown_event() const;
};
