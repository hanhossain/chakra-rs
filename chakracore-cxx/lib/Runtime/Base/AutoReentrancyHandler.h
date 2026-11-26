#pragma once

class ThreadContext;

class AutoReentrancyHandler {
    ThreadContext *thread_context_;
    bool saved_reentrancy_safe_or_handled_;
public:
    explicit AutoReentrancyHandler(ThreadContext *thread_context);
    ~AutoReentrancyHandler();
};
