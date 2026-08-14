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
#include <string>

class RuntimeThreadData
{
public:
    RuntimeThreadData();
    ~RuntimeThreadData();
    HANDLE hevntReceivedBroadcast;
    HANDLE hevntShutdown;
    std::optional<std::binary_semaphore> semaphore;
    HANDLE hThread;

    JsSharedArrayBufferContentHandle sharedContent;
    JsValueRef receiveBroadcastCallbackFunc;


    JsRuntimeHandle runtime;
    JsContextRef context;


    std::string initialSource;

    RuntimeThreadData* parent;
    
    std::list<RuntimeThreadData*> children;

    CRITICAL_SECTION csReportQ;
    std::list<std::string> reportQ;

    bool leaving;


    uint32_t ThreadProc();
    void set_initial_script_completed();
    void reset_initial_script_completed();
    void wait_initial_script_completed();

private:
    bool initial_script_completed_;
    std::condition_variable initial_script_completed_cv_;
    std::mutex initial_script_completed_mtx_;
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
