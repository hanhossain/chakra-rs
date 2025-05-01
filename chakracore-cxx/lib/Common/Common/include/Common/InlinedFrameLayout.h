#pragma once

struct InlinedFrameLayout
{
    Js::InlineeCallInfo     callInfo;
    Js::JavascriptFunction *function;
    Js::Var                 arguments;  // The arguments object.
    //Js::Var               argv[0];    // Here it would be embedded arguments array (callInfo.count elements)
    // but can't have 0-size arr in base class, so we define it in derived class.

    Js::Var* GetArguments()
    {
        return (Js::Var*)(this + 1);
    }

    template<class Fn>
    void MapArgs(Fn callback)
    {
        Js::Var* arguments = this->GetArguments();
        for (uint i = 0; i < callInfo.Count; i++)
        {
            callback(i, (Js::Var*)((uintptr_t*)arguments + i));
        }
    }

    InlinedFrameLayout* Next()
    {
        InlinedFrameLayout *next = (InlinedFrameLayout *)(this->GetArguments() + this->callInfo.Count);
        return next;
    }
};
