//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "ChakraCore.h"
#include "JsrtThreadService.h"

class JsrtContext;

class JsrtRuntime
{
    friend class JsrtContext;

public:
    JsrtRuntime(ThreadContext * threadContext, bool useIdle, bool dispatchExceptions);
    ~JsrtRuntime();

    ThreadContext * GetThreadContext() { return this->threadContext; }

    JsRuntimeHandle ToHandle() { return static_cast<JsRuntimeHandle>(this); }
    static JsrtRuntime * FromHandle(JsRuntimeHandle runtimeHandle)
    {
        JsrtRuntime * runtime = static_cast<JsrtRuntime *>(runtimeHandle);
        runtime->threadContext->ValidateThreadContext();
        return runtime;
    }
    static void Uninitialize();

    bool UseIdle() const { return useIdle; }
    unsigned int Idle();

    bool DispatchExceptions() const { return dispatchExceptions; }

    void CloseContexts();
    void SetBeforeCollectCallback(JsBeforeCollectCallback beforeCollectCallback, void * callbackContext);
    void SetBeforeSweepCallback(JsBeforeSweepCallback beforeCollectCallback, void * callbackContext);

    void SetSerializeByteCodeForLibrary(bool set) { serializeByteCodeForLibrary = set; }
    bool IsSerializeByteCodeForLibrary() const { return serializeByteCodeForLibrary; }

private:
    static void RecyclerCollectCallbackStatic(void * context, RecyclerCollectCallBackFlags flags);

private:
    ThreadContext * threadContext;
    AllocationPolicyManager* allocationPolicyManager;
    JsrtContext * contextList;
    ThreadContext::CollectCallBack * collectCallback;
    JsBeforeCollectCallback beforeCollectCallback;
    JsBeforeSweepCallback beforeSweepCallback;
    void * beforeSweepCallbackContext;
    JsrtThreadService threadService;
    void * beforeCollectCallbackContext;
    bool useIdle;
    bool dispatchExceptions;
    bool serializeByteCodeForLibrary;
};
