#pragma once

class ThreadContext;

// This is for protecting a region of code, where we can't recover and be consistent upon failures (mainly due to OOM
// and SO). FailFast on that.
class AutoDisableInterrupt {
public:
    explicit AutoDisableInterrupt(ThreadContext *threadContext, bool explicitCompletion = true);
    ~AutoDisableInterrupt();
    void RequireExplicitCompletion();
    void Completed();
private:
    ThreadContext * m_threadContext;
    bool m_operationCompleted;
    bool m_interruptDisableState;
    bool m_explicitCompletion;
};
