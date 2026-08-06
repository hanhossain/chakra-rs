//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class RuntimeFunction : public JavascriptFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(RuntimeFunction, JavascriptFunction);
        friend class Js::CrossSiteObject<RuntimeFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<RuntimeFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<RuntimeFunction>>::SetVirtualTable(this);
        };
        RuntimeFunction(DynamicType * type);
    public:
        RuntimeFunction(DynamicType * type, FunctionInfo * functionInfo);
        RuntimeFunction(DynamicType * type, FunctionInfo * functionInfo, ConstructorCache* cache);

        void SetFunctionNameId(Var nameId);

        // This is for cached source string for the function. Possible values are:
        // NULL; initialized for anonymous methods.
        // propertyId in Int31 format; this is used for fastDOM function as well as library function
        // JavascriptString: composed using functionname from the propertyId, or fixed string for anonymous functions.
        // NOTE: This has a side-effect that after toString() is called for the first time on a built-in function the functionNameId gets replaced with a string like "function foo() { native code }".
        // As a result any code like debugger(F12) that shows the functionNameId to the user will need to pre-process this string as it may not be desirable to use this as-is in some cases.
        // See RuntimeFunction::EnsureSourceString() for details.
        typename WriteBarrierFieldTypeTraits<bool>::Type isDisplayString;
        typename WriteBarrierFieldTypeTraits<Var>::Type functionNameId;
        Var GetSourceString() const override { return functionNameId; }
        JavascriptString * EnsureSourceString() override;
    };
};
