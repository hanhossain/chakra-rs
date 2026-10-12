//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "ChakraCore.h"

#include <condition_variable>
#include <list>
#include <rust/cxx.h>

namespace chakra_rs {
    struct WScript;
}

class RuntimeThreadData
{
public:
    RuntimeThreadData();
    RuntimeThreadData(rust::String initialSource);
    ~RuntimeThreadData();
    static std::shared_ptr<RuntimeThreadData> NewWithInitialSource(const rust::String &initialSource);
    static std::shared_ptr<RuntimeThreadData> New();
    void uninitialize();
    HANDLE hevntReceivedBroadcast;
    HANDLE hevntShutdown;
    HANDLE hThread;

    JsSharedArrayBufferContentHandle sharedContent_;
    JsValueRef receiveBroadcastCallbackFunc;

    JsRuntimeHandle runtime;
    JsContextRef context;

    std::weak_ptr<RuntimeThreadData> parent_;

    std::vector<std::shared_ptr<RuntimeThreadData>> children_;

    uint32_t ThreadProc(const chakra_rs::WScript &wscript);
    void set_leaving(bool leaving);
    void set_initial_script_completed();
    void wait_initial_script_completed();
    void enqueue_report_to_parent(rust::String report);
    bool dequeue_report(rust::String &report);
    void broadcast_to_children();
    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content();
    JsValueRef get_receive_broadcast_callback_func() const;
    void set_receive_broadcast_callback_func(JsValueRef value);
    void add_child(const std::shared_ptr<RuntimeThreadData> &child);
    void set_parent(const std::weak_ptr<RuntimeThreadData> &parentThread);
    void set_thread_handle(HANDLE thread);

private:
    bool initial_script_completed_{};
    std::condition_variable initial_script_completed_cv_;
    std::mutex initial_script_completed_mtx_;

    std::mutex csReportQ_{};
    std::list<rust::String> reportQ_{};
    bool leaving_;
    rust::String initialSource_;
};
