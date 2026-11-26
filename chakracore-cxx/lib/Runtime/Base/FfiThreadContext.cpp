#include "FfiThreadContext.h"

#include "ThreadContext.h"

FfiThreadContext::FfiThreadContext(ThreadContext *thread_context) : thread_context_(thread_context) {}

FfiThreadContext::~FfiThreadContext() { delete thread_context_; }
void FfiThreadContext::SetJSRTRuntime(bool isJsrtRuntime) { thread_context_->SetJSRTRuntime(isJsrtRuntime); }
void FfiThreadContext::SetCurrentThreadId(uint32_t threadId) { thread_context_->SetCurrentThreadId(threadId); }
void FfiThreadContext::SetThreadContextFlag(ThreadContextFlags threadContextFlag) { thread_context_->SetThreadContextFlag(threadContextFlag); }
void FfiThreadContext::OptimizeForManyInstances(bool optimizeForManyInstances) { thread_context_->OptimizeForManyInstances(optimizeForManyInstances); }

#if ENABLE_NATIVE_CODEGEN
void FfiThreadContext::EnableBgJit(bool enableBgJit) { thread_context_->EnableBgJit(enableBgJit); }
#endif

ThreadContext *FfiThreadContext::GetThreadContext() { return thread_context_; }
