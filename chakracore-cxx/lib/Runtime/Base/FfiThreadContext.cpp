#include "FfiThreadContext.h"

#include "ThreadContext.h"

FfiThreadContext::FfiThreadContext(ThreadContext *thread_context) : thread_context_(thread_context) {}

FfiThreadContext::~FfiThreadContext() { delete thread_context_; }
void FfiThreadContext::SetJSRTRuntime(bool isJsrtRuntime) { thread_context_->SetJSRTRuntime(isJsrtRuntime); }
void FfiThreadContext::SetCurrentThreadId(uint32_t threadId) { thread_context_->SetCurrentThreadId(threadId); }
ThreadContext *FfiThreadContext::GetThreadContext() { return thread_context_; }
