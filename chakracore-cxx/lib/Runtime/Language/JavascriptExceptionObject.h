//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    const uint32_t  ExceptionParameters = 1;
    const int    ExceptionObjectIndex = 0;

    class JavascriptExceptionContext;

    class JavascriptExceptionObject
    {
    public:
        typedef Var (*HostWrapperCreateFuncType)(Var var, ScriptContext * sourceScriptContext, ScriptContext * destScriptContext);

        JavascriptExceptionObject(Var object, ScriptContext * scriptContext, JavascriptExceptionContext* exceptionContextIn, bool isPendingExceptionObject = false) :
            thrownObject(object),
            isPendingExceptionObject(isPendingExceptionObject),
            scriptContext(scriptContext), tag(true),
            hostWrapperCreateFunc(nullptr),
            next(nullptr)
        {
            if (exceptionContextIn)
            {
                exceptionContext = *exceptionContextIn;
            }
            else
            {
                memset(&exceptionContext, 0, sizeof(exceptionContext));
            }
#if ENABLE_DEBUG_STACK_BACK_TRACE
            this->stackBackTrace = nullptr;
#endif
        }

        Var GetThrownObject(ScriptContext * requestingScriptContext);

        // ScriptContext can be NULL in case of OOM exception.
        ScriptContext * GetScriptContext() const
        {
            return scriptContext;
        }

        FunctionBody * GetFunctionBody() const;
        JavascriptFunction* GetFunction() const
        {
            return exceptionContext.ThrowingFunction();
        }

        const JavascriptExceptionContext* GetExceptionContext() const
        {
            return &exceptionContext;
        }
#if ENABLE_DEBUG_STACK_BACK_TRACE
        void FillStackBackTrace();
#endif

        void FillError(JavascriptExceptionContext& exceptionContext, ScriptContext *scriptContext, HostWrapperCreateFuncType hostWrapperCreateFunc = NULL);
        void ClearError();

        void SetHostWrapperCreateFunc(HostWrapperCreateFuncType hostWrapperCreateFunc)
        {
            this->hostWrapperCreateFunc = hostWrapperCreateFunc;
        }

        uint32_t GetByteCodeOffset()
        {
            return exceptionContext.ThrowingFunctionByteCodeOffset();
        }

        void ReplaceThrownObject(Var object)
        {
            AssertMsg(VarIs<RecyclableObject>(object), "Why are we replacing a non recyclable thrown object?");
            AssertMsg(this->GetScriptContext() != VarTo<RecyclableObject>(object)->GetScriptContext() || this->thrownObject != object, "If replaced thrownObject is from same context what's the need to replace?");
            this->thrownObject = object;
        }

        void SetThrownObject(Var object)
        {
            // Only pending exception object and generator return exception use this API.
            Assert(this->isPendingExceptionObject);
            this->thrownObject = object;
        }
        JavascriptExceptionObject* CloneIfStaticExceptionObject(ScriptContext* scriptContext);

        void ClearStackTrace()
        {
            exceptionContext.SetStackTrace(NULL);
        }

        bool IsPendingExceptionObject() const { return isPendingExceptionObject; }

    private:
        friend class ::ThreadContext;
        static void Insert(typename WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type* head, JavascriptExceptionObject* item);
        static void Remove(typename WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type* head, JavascriptExceptionObject* item);

    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type      thrownObject;
        typename WriteBarrierFieldTypeTraits<ScriptContext *>::Type scriptContext;


        typename WriteBarrierFieldTypeTraits<const bool>::Type tag : 1;               // Tag the low bit to prevent possible GC false references
        typename WriteBarrierFieldTypeTraits<bool>::Type       isPendingExceptionObject : 1;

        typename WriteBarrierFieldTypeTraits<HostWrapperCreateFuncType, _no_write_barrier_policy, _no_write_barrier_policy>::Type hostWrapperCreateFunc;

        typename WriteBarrierFieldTypeTraits<JavascriptExceptionContext>::Type exceptionContext;
#if ENABLE_DEBUG_STACK_BACK_TRACE
        typename WriteBarrierFieldTypeTraits<StackBackTrace*>::Type stackBackTrace;
        static const int StackToSkip = 2;
        static const int StackTraceDepth = 30;
#endif

        typename WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type next;  // to temporarily store list of throwing exceptions

        PREVENT_COPY(JavascriptExceptionObject)
    };
}
