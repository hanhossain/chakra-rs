#include "AutoDisableInterrupt.h"
AutoDisableInterrupt::AutoDisableInterrupt(ThreadContext *threadContext, bool explicitCompletion)
    : m_threadContext(threadContext), m_operationCompleted(false), m_interruptDisableState(false),
      m_explicitCompletion(explicitCompletion) {
}
AutoDisableInterrupt::~AutoDisableInterrupt() {
    if (m_explicitCompletion && !m_operationCompleted) {
        AssertOrFailFast(false);
    }
}
void AutoDisableInterrupt::RequireExplicitCompletion() { m_explicitCompletion = true; }
void AutoDisableInterrupt::Completed() { m_operationCompleted = true; }
