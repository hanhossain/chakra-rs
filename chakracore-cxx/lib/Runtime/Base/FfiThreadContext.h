#pragma once

class ThreadContext;

class FfiThreadContext {
public:
    explicit FfiThreadContext(ThreadContext *thread_context);
    ~FfiThreadContext();

    void SetJSRTRuntime(bool isJsrtRuntime);
    void SetCurrentThreadId(uint32_t threadId);
    ThreadContext *GetThreadContext();
private:
    ThreadContext *thread_context_;
};
