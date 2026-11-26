#pragma once

#include "Base/ThreadContextFlags.h"

class ThreadContext;

class FfiThreadContext {
public:
    explicit FfiThreadContext(ThreadContext *thread_context);
    ~FfiThreadContext();

    void SetJSRTRuntime(bool isJsrtRuntime);
    void SetCurrentThreadId(uint32_t threadId);
    void SetThreadContextFlag(ThreadContextFlags threadContextFlag);
    void OptimizeForManyInstances(bool optimizeForManyInstances);

#if ENABLE_NATIVE_CODEGEN
    void EnableBgJit(bool enableBgJit);
#endif

    ThreadContext *GetThreadContext();
private:
    ThreadContext *thread_context_;
};
