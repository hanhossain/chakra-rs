#pragma once

class ThreadContext;

class AutoReentrancySafeRegion {
    ThreadContext * thread_context_;
    bool saved_is_in_reentrancy_safe_region_;
public:
    explicit AutoReentrancySafeRegion(ThreadContext *threadContext);
    ~AutoReentrancySafeRegion();
};
