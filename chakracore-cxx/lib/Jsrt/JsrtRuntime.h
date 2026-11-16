//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "ChakraCore.h"

#include <list>
#include <memory>

class JsrtContext;
class ThreadContext;

/// <summary>
///     A Chakra runtime.
/// </summary>
/// <remarks>
///     <para>
///     Each Chakra runtime has its own independent execution engine, JIT compiler, and garbage
///     collected heap. As such, each runtime is completely isolated from other runtimes.
///     </para>
///     <para>
///     Runtimes can be used on any thread, but only one thread can call into a runtime at any
///     time.
///     </para>
/// </remarks>
class JsrtRuntime
{
public:
    JsrtRuntime(std::unique_ptr<ThreadContext> threadContext);
    ~JsrtRuntime();

    ThreadContext * GetThreadContext() { return this->threadContext.get(); }

    static void Uninitialize();

    // JsrtContext *GetFirstContext() { return contextList; }
    // void SetFirstContext(JsrtContext *context) { contextList = context; }

    void Link(JsrtContext *context);
    void Unlink(JsrtContext *context);

private:
    void CloseContexts();
    std::unique_ptr<ThreadContext> threadContext;
    std::list<JsrtContext *> contextList{};
};
