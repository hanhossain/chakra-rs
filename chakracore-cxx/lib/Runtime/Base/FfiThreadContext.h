#pragma once

class ThreadContext;

class FfiThreadContext {
public:
    explicit FfiThreadContext(ThreadContext *thread_context);
    ~FfiThreadContext();

    void SetJSRTRuntime(bool isJsrtRuntime);
    ThreadContext *GetThreadContext();
private:
    ThreadContext *thread_context_;
};
