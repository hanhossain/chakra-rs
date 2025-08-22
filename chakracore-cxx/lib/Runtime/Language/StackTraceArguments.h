//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
namespace Js {
    class StackTraceArguments
    {
    private:
        static const unsigned long MaxNumberOfDisplayedArgumentsInStack = 20; // 1 << (3*MaxNumberOfDisplayedArgumentsInStack + 1) must fit in unsigned long (or you have to change it's type)
        static const unsigned long fCallerIsGlobal = 1ull << (3*MaxNumberOfDisplayedArgumentsInStack + 1);
        static const unsigned long fTooManyArgs = 1ull << (3*MaxNumberOfDisplayedArgumentsInStack);

        static unsigned long ObjectToTypeCode(Js::Var object);
        static JavascriptString *TypeCodeToTypeName(unsigned typeCode, ScriptContext *scriptContext);

        // We use 3 bits to store the value type. If we add another type, we need to use more bits.
        enum valueTypes
        {
            nullValue = 0,
            undefinedValue = 1,
            booleanValue = 2,
            stringValue = 3,
            nanValue = 4,
            numberValue = 5,
            symbolValue = 6,
            objectValue = 7
        };
        unsigned long types;

    public:
        HRESULT ToString(LPCWSTR functionName, Js::ScriptContext *scriptContext, _In_ LPCWSTR* outResult) const;
        void Init(const JavascriptStackWalker &walker);
        StackTraceArguments() : types(fCallerIsGlobal) {}
    };
}
