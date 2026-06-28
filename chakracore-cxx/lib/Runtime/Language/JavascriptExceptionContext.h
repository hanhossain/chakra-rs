//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js {

    class JavascriptExceptionContext
    {
    public:
        struct StackFrame
        {
        private:
            // Real script frames: functionBody, byteCodeOffset
            // Native library builtin (or potentially virtual) frames: name
            typename WriteBarrierFieldTypeTraits<FunctionBody*>::Type functionBody;
            union
            {
                typename WriteBarrierFieldTypeTraits<uint32_t>::Type byteCodeOffset;  // used for script functions        (functionBody != nullptr)
                typename WriteBarrierFieldTypeTraits<const char16_t *>::Type name;            // used for native/virtual frames   (functionBody == nullptr)
            };
            typename WriteBarrierFieldTypeTraits<StackTraceArguments>::Type argumentTypes;

        public:
            StackFrame() {}
            StackFrame(JavascriptFunction* func, const JavascriptStackWalker& walker, bool initArgumentTypes);
            StackFrame(const StackFrame& other)
                :functionBody(other.functionBody), name(other.name), argumentTypes(other.argumentTypes)
            {}
            StackFrame& operator=(const StackFrame& other)
            {
                functionBody = other.functionBody;
                name = other.name;
                argumentTypes = other.argumentTypes;
                return *this;
            }

            bool IsScriptFunction() const;
            FunctionBody* GetFunctionBody() const;
            uint32_t GetByteCodeOffset() const { return byteCodeOffset; }
            const char16_t* GetFunctionName() const;
            int32_t GetFunctionNameWithArguments(_In_ const char16_t* *outResult) const;
        };

        typedef JsUtil::List<StackFrame> StackTrace;

    public:
        JavascriptExceptionContext() :
            m_throwingFunction(nullptr),
            m_throwingFunctionByteCodeOffset(0),
            m_stackTrace(nullptr),
            m_originalStackTrace(nullptr)
        {
        }

        JavascriptFunction* ThrowingFunction() const { return m_throwingFunction; }
        uint32_t ThrowingFunctionByteCodeOffset() const { return m_throwingFunctionByteCodeOffset; }
        void SetThrowingFunction(JavascriptFunction* function, uint32_t byteCodeOffset, void * returnAddress);

        bool HasStackTrace() const { return m_stackTrace && m_stackTrace->Count() > 0; }
        StackTrace* GetStackTrace() const { return m_stackTrace; }
        void SetStackTrace(StackTrace *stackTrace) { m_stackTrace = stackTrace; }
        void SetOriginalStackTrace(StackTrace *stackTrace) { Assert(m_originalStackTrace == nullptr); m_originalStackTrace = stackTrace; }
        StackTrace* GetOriginalStackTrace() const { return m_originalStackTrace; }

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type m_throwingFunction;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_throwingFunctionByteCodeOffset;
        typename WriteBarrierFieldTypeTraits<StackTrace *>::Type m_stackTrace;
        typename WriteBarrierFieldTypeTraits<StackTrace *>::Type m_originalStackTrace;
    };
}
