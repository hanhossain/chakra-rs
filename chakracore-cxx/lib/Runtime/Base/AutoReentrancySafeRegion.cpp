#include "AutoReentrancySafeRegion.h"
AutoReentrancySafeRegion::AutoReentrancySafeRegion(ThreadContext *threadContext)
    : thread_context_(threadContext),
      saved_is_in_reentrancy_safe_region_(thread_context_->GetIsInReentrancySafeRegion()) {
    thread_context_->SetIsInReentrancySafeRegion(true);
}

AutoReentrancySafeRegion::~AutoReentrancySafeRegion() {
    thread_context_->SetIsInReentrancySafeRegion(saved_is_in_reentrancy_safe_region_);
}
