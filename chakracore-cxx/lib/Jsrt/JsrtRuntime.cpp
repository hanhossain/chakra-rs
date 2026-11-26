//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "JsrtRuntime.h"
#include "jsrtHelper.h"
#include "Base/ThreadContextTlsEntry.h"
#include "Base/ThreadBoundThreadContextManager.h"
#include "Base/FfiThreadContext.h"

JsrtRuntime::JsrtRuntime(FfiThreadContext *thread_context) : thread_context_(thread_context) {
    thread_context_->SetCurrentThreadId(ThreadContext::NoThread);
    thread_context_->SetJSRTRuntime(true);
}

JsrtRuntime::~JsrtRuntime() {
    ThreadContextScope scope{thread_context_->GetThreadContext()};

    // Invoke and clear the callbacks while the contexts and runtime are still available
    {
        Recycler *recycler = thread_context_->GetThreadContext()->GetRecycler();
        if (recycler != nullptr) {
            recycler->ClearObjectBeforeCollectCallbacks();
        }
    }

    thread_context_->GetThreadContext()->MarkContextsForClose();

    // Close any open Contexts.
    // We need to do this before recycler shutdown, because ScriptEngine->Close won't work then.
    CloseContexts();
    delete thread_context_;
    scope.Invalidate();
}

std::unique_ptr<JsrtRuntime> JsrtRuntime::FfiNew(std::unique_ptr<FfiThreadContext> thread_context) {
    return std::make_unique<JsrtRuntime>(thread_context.release());
}

ThreadContext *JsrtRuntime::GetThreadContext() { return thread_context_->GetThreadContext(); }

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