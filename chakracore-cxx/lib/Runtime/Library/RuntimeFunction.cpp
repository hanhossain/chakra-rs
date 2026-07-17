//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

namespace Js
{
    RuntimeFunction::RuntimeFunction(DynamicType * type)
        : JavascriptFunction(type), isDisplayString(false), functionNameId(nullptr)
    {}

    RuntimeFunction::RuntimeFunction(DynamicType * type, FunctionInfo * functionInfo)
        : JavascriptFunction(type, functionInfo), isDisplayString(false), functionNameId(nullptr)
    {}

    RuntimeFunction::RuntimeFunction(DynamicType * type, FunctionInfo * functionInfo, ConstructorCache* cache)
        : JavascriptFunction(type, functionInfo, cache), isDisplayString(false), functionNameId(nullptr)
    {}

    JavascriptString *
    RuntimeFunction::EnsureSourceString()
    {
        JavascriptLibrary* library = this->GetLibrary();
        ScriptContext * scriptContext = library->GetScriptContext();
        JavascriptString * retStr = nullptr;
        if (this->isDisplayString)
        {
            return VarTo<JavascriptString>(this->functionNameId);
        }

        if (this->functionNameId == nullptr)
        {
            retStr = library->GetFunctionDisplayString();
        }
        else
        {
            if (this->GetTypeHandler()->IsDeferredTypeHandler())
            {
                JavascriptString* functionName = nullptr;
                DebugOnly(bool status = ) this->GetFunctionName(&functionName);
                Assert(status);
                this->SetPropertyWithAttributes(PropertyIds::name, functionName, PropertyConfigurable, nullptr);
            }
            if (TaggedInt::Is(this->functionNameId))
            {
                // This has a side-effect where any other code (such as debugger) that uses functionNameId value will now get the value like "function foo() { native code }"
                // instead of just "foo". Alternative ways will need to be devised; if it's not desirable to use this full display name value in those cases.
                retStr = GetNativeFunctionDisplayString(scriptContext, scriptContext->GetPropertyString(TaggedInt::ToInt32(this->functionNameId)));
            }
            else
            {
                retStr = GetNativeFunctionDisplayString(scriptContext, VarTo<JavascriptString>(this->functionNameId));
            }
        }

        this->functionNameId = retStr;
        this->isDisplayString = true;

        return retStr;
    }

    void
    RuntimeFunction::SetFunctionNameId(Var nameId)
    {
        Assert(functionNameId == NULL);
        Assert(TaggedInt::Is(nameId) || Js::VarIs<Js::JavascriptString>(nameId));

        // We are only reference the propertyId, it needs to be tracked to stay alive
        Assert(!TaggedInt::Is(nameId) || this->GetScriptContext()->IsTrackedPropertyId(TaggedInt::ToInt32(nameId)));
        this->isDisplayString = false;
        this->functionNameId = nameId;
    }
};
