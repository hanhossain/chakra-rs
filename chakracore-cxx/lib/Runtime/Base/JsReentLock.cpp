#include "JsReentLock.h"

JsReentLock::JsReentLock(ThreadContext *threadContext)
#ifndef NDEBUG
    : m_arrayObject(nullptr), m_arrayObject2(nullptr)
#endif
{
    m_savedNoJsReentrancy = threadContext->GetNoJsReentrancy();
    threadContext->SetNoJsReentrancy(true);
    m_threadContext = threadContext;
}

JsReentLock::~JsReentLock() {
    m_threadContext->SetNoJsReentrancy(m_savedNoJsReentrancy);
}
void JsReentLock::unlock() { m_threadContext->SetNoJsReentrancy(m_savedNoJsReentrancy); }
void JsReentLock::relock() { m_threadContext->SetNoJsReentrancy(true); }

#ifndef NDEBUG
void JsReentLock::setObjectForMutation(Js::Var object)
{
    m_arrayObject = nullptr;

    if (object != nullptr && Js::DynamicObject::IsAnyArray(object))
    {
        m_arrayObject = object;
    }

    // Don't care about any other objects for now
}

void JsReentLock::setSecondObjectForMutation(Js::Var object)
{
    m_arrayObject2 = nullptr;

    if (object != nullptr && Js::DynamicObject::IsAnyArray(object))
    {
        m_arrayObject2 = object;
    }

    // Don't care about any other objects for now
}
#endif