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

        explicit JavascriptExceptionObject(const bool isPendingExceptionObject);
        JavascriptExceptionObject(Var object, ScriptContext * scriptContext, JavascriptExceptionContext* exceptionContextIn, bool isPendingExceptionObject = false);

        JavascriptExceptionObject(JavascriptExceptionObject&& other) noexcept;

        JavascriptExceptionObject& operator=(JavascriptExceptionObject&& other) noexcept;

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

#ifdef ENABLE_SCRIPT_DEBUGGING
        void SetDebuggerSkip(bool skip)
        {
            isDebuggerSkip = skip;
        }

        bool IsDebuggerSkip()
        {
            return isDebuggerSkip;
        }

        int GetByteCodeOffsetAfterDebuggerSkip()
        {
            return byteCodeOffsetAfterDebuggerSkip;
        }

        void SetByteCodeOffsetAfterDebuggerSkip(int offset)
        {
            byteCodeOffsetAfterDebuggerSkip = offset;
        }

        void SetDebuggerHasLogged(bool has)
        {
            hasDebuggerLogged = has;
        }

        bool HasDebuggerLogged()
        {
            return hasDebuggerLogged;
        }

        void SetIsFirstChance(bool is)
        {
            isFirstChance = is;
        }

        bool IsFirstChanceException()
        {
            return isFirstChance;
        }

        void SetIsExceptionCaughtInNonUserCode(bool is)
        {
            isExceptionCaughtInNonUserCode = is;
        }

        bool IsExceptionCaughtInNonUserCode()
        {
            return isExceptionCaughtInNonUserCode;
        }

        void SetIgnoreAdvanceToNextStatement(bool is)
        {
            ignoreAdvanceToNextStatement = is;
        }

        bool IsIgnoreAdvanceToNextStatement()
        {
            return ignoreAdvanceToNextStatement;
        }
#endif

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
        static void Insert(WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type* head, JavascriptExceptionObject* item);
        static void Remove(WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type* head, JavascriptExceptionObject* item);

    private:
        WriteBarrierFieldTypeTraits<Var>::Type thrownObject = nullptr;
        WriteBarrierFieldTypeTraits<ScriptContext *>::Type scriptContext = nullptr;

#ifdef ENABLE_SCRIPT_DEBUGGING
        WriteBarrierFieldTypeTraits<int>::Type byteCodeOffsetAfterDebuggerSkip = Constants::InvalidByteCodeOffset;
#endif

        WriteBarrierFieldTypeTraits<bool>::Type tag : 1 = true; // Tag the low bit to prevent possible GC false references
        WriteBarrierFieldTypeTraits<bool>::Type isPendingExceptionObject : 1 = false;

#ifdef ENABLE_SCRIPT_DEBUGGING
        WriteBarrierFieldTypeTraits<bool>::Type isDebuggerSkip : 1 = false;
        WriteBarrierFieldTypeTraits<bool>::Type hasDebuggerLogged : 1 = false;
        WriteBarrierFieldTypeTraits<bool>::Type isFirstChance : 1 = false; // Mentions whether the current exception is a handled exception or not
        WriteBarrierFieldTypeTraits<bool>::Type isExceptionCaughtInNonUserCode : 1 = false; // Mentions if in the caller chain the exception will be handled by the non-user code.
        WriteBarrierFieldTypeTraits<bool>::Type ignoreAdvanceToNextStatement : 1 = false;  // This will be set when user had setnext while sitting on the exception
                                                // So the exception eating logic shouldn't try and advance to next statement again.
#endif

        WriteBarrierFieldTypeTraits<HostWrapperCreateFuncType, _no_write_barrier_policy, _no_write_barrier_policy>::Type hostWrapperCreateFunc = nullptr;

        WriteBarrierFieldTypeTraits<JavascriptExceptionContext>::Type exceptionContext {};
#if ENABLE_DEBUG_STACK_BACK_TRACE
        typename WriteBarrierFieldTypeTraits<StackBackTrace*>::Type stackBackTrace;
        static const int StackToSkip = 2;
        static const int StackTraceDepth = 30;
#endif

        WriteBarrierFieldTypeTraits<JavascriptExceptionObject*>::Type next = nullptr;  // to temporarily store list of throwing exceptions

        PREVENT_COPY(JavascriptExceptionObject)
    };
}
