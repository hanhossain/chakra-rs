//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "ChakraCore.h"

#include <condition_variable>
#include <list>
#include <optional>
#include <semaphore>
#include <rust/cxx.h>

class RuntimeThreadData
{
public:
    RuntimeThreadData();
    RuntimeThreadData(rust::String initialSource);
    ~RuntimeThreadData();
    HANDLE hevntReceivedBroadcast;
    HANDLE hevntShutdown;
    HANDLE hThread;

    JsSharedArrayBufferContentHandle sharedContent_;
    JsValueRef receiveBroadcastCallbackFunc;


    JsRuntimeHandle runtime;
    JsContextRef context;



    RuntimeThreadData* parent;

    std::list<RuntimeThreadData*> children;

    uint32_t ThreadProc();
    void set_leaving(bool leaving);
    void set_initial_script_completed();
    void reset_initial_script_completed();
    void wait_initial_script_completed();
    void enqueue_report_to_parent(rust::String report);
    bool dequeue_report(rust::String &report);
    void broadcast_to_children();
    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content();
    JsValueRef get_receive_broadcast_callback_func() const;
    void set_receive_broadcast_callback_func(JsValueRef value);

private:
    bool initial_script_completed_{};
    std::condition_variable initial_script_completed_cv_;
    std::mutex initial_script_completed_mtx_;

    std::mutex csReportQ_{};
    std::list<rust::String> reportQ_{};
    bool leaving_;
    rust::String initialSource_;
};

struct RuntimeThreadLocalData
{
    // can't use ctor/dtor because it's not supported in VS2012
    // error C2483: 'threadLocalData' : object with constructor or destructor cannot be declared 'thread' 
    void Initialize(RuntimeThreadData* threadData);
    void Uninitialize();
    RuntimeThreadData* threadData;
};

RuntimeThreadLocalData& GetRuntimeThreadLocalData();

RuntimeThreadData &GetCurrentRuntimeThreadData(int &dummy);
