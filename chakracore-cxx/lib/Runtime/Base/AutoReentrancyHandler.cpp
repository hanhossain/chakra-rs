#include "AutoReentrancyHandler.h"
AutoReentrancyHandler::AutoReentrancyHandler(ThreadContext *thread_context)
    : thread_context_(thread_context),
      saved_reentrancy_safe_or_handled_(thread_context_->GetReentrancySafeOrHandled()) {
    thread_context_->SetReentrancySafeOrHandled(true);
}
AutoReentrancyHandler::~AutoReentrancyHandler() {
    thread_context_->SetReentrancySafeOrHandled(saved_reentrancy_safe_or_handled_);
}
