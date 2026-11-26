#pragma once

class JsReentLock {
    ThreadContext *m_threadContext;
#ifndef NDEBUG
    Js::Var m_arrayObject;
    Js::Var m_arrayObject2; // This is for adding the second object in the mutation equation.
#endif
    bool m_savedNoJsReentrancy;

public:
    JsReentLock(ThreadContext *threadContext);
    ~JsReentLock();

    void unlock();
    void relock();

#ifndef NDEBUG
    void setObjectForMutation(Js::Var object);
    void setSecondObjectForMutation(Js::Var object);
#endif
};
