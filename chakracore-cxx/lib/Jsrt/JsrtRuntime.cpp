//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "JsrtRuntime.h"
#include "jsrtHelper.h"
#include "Base/ThreadContextTlsEntry.h"
#include "Base/ThreadBoundThreadContextManager.h"
JsrtRuntime::JsrtRuntime(std::unique_ptr<ThreadContext> threadContext)
{
    this->threadContext = std::move(threadContext);
    this->threadContext->SetJSRTRuntime(true);
}

JsrtRuntime::~JsrtRuntime() {
    ThreadContextScope scope{threadContext.get()};

    // Invoke and clear the callbacks while the contexts and runtime are still available
    {
        Recycler *recycler = threadContext->GetRecycler();
        if (recycler != nullptr) {
            recycler->ClearObjectBeforeCollectCallbacks();
        }
    }

    threadContext->MarkContextsForClose();

    // Close any open Contexts.
    // We need to do this before recycler shutdown, because ScriptEngine->Close won't work then.
    CloseContexts();
    threadContext.reset();
    scope.Invalidate();
}

// This is called at process detach.
// threadcontext created from runtime should not be destroyed in ThreadBoundThreadContext
// we should clean them up at process detach only as runtime can be used in other threads
// even after the current physical thread was destroyed.
// This is called after ThreadBoundThreadContext are cleaned up, so the remaining items
// in the globalthreadContext linklist should be for jsrt only.
void JsrtRuntime::Uninitialize()
{
    ThreadContext* currentThreadContext = ThreadContext::GetThreadContextList();
    ThreadContext* tmpThreadContext;
    while (currentThreadContext)
    {
        Assert(!currentThreadContext->IsScriptActive());
        tmpThreadContext = currentThreadContext;
        currentThreadContext = currentThreadContext->Next();

        // xplat-todo: Cleanup staticlib shutdown. This only shuts down threads.
        // Other closing contexts / finalizers having trouble with current
        // runtime/context.
        RentalThreadContextManager::DestroyThreadContext(tmpThreadContext);
    }
}

void JsrtRuntime::Link(JsrtContext *context) {
    contextList.push_front(context);
}
void JsrtRuntime::Unlink(JsrtContext *context) {
    contextList.remove(context);
}

void JsrtRuntime::CloseContexts()
{
    for (auto *context : contextList) {
        context->DisposeNoUnlink();
    }
    contextList.clear();
}