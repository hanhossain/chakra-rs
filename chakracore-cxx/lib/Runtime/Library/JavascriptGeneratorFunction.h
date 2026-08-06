//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Library/ScriptFunction.h"

namespace Js
{
    class GeneratorVirtualScriptFunction;

    class JavascriptGeneratorFunction : public ScriptFunctionBase
    {
    private:
        static FunctionInfo functionInfo;
        typename WriteBarrierFieldTypeTraits<GeneratorVirtualScriptFunction*>::Type scriptFunction;

    protected:
        DEFINE_VTABLE_CTOR(JavascriptGeneratorFunction, ScriptFunctionBase);
        friend class Js::CrossSiteObject<JavascriptGeneratorFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptGeneratorFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptGeneratorFunction>>::SetVirtualTable(this);
        };

        JavascriptGeneratorFunction(DynamicType* type, FunctionInfo* functionInfo, GeneratorVirtualScriptFunction* scriptFunction);

    public:
        JavascriptGeneratorFunction(DynamicType* type, GeneratorVirtualScriptFunction* scriptFunction);
        JavascriptGeneratorFunction(DynamicType* type);

        JavascriptString * GetDisplayNameImpl() const override;
        GeneratorVirtualScriptFunction* GetGeneratorVirtualScriptFunction() { return scriptFunction; }

        // Returns whether this function is exactly a JavascriptGeneratorFunction, not a JavascriptAsyncFunction
        static bool IsBaseGeneratorFunction(RecyclableObject* obj);
        inline static bool Test(JavascriptFunction *obj)
        {
            return VirtualTableInfo<JavascriptGeneratorFunction>::HasVirtualTable(obj)
                || VirtualTableInfo<CrossSiteObject<JavascriptGeneratorFunction>>::HasVirtualTable(obj);
        }

        static JavascriptGeneratorFunction* New(ScriptContext* scriptContext, GeneratorVirtualScriptFunction* scriptFunction);
        static JavascriptGeneratorFunction* OP_NewScGenFunc(FrameDisplay* environment, FunctionInfoPtrPtr infoRef);
        static JavascriptGeneratorFunction* OP_NewScGenFuncHomeObj(FrameDisplay* environment, FunctionInfoPtrPtr infoRef, Var homeObj);
        static Var EntryGeneratorFunctionImplementation(RecyclableObject* function, CallInfo callInfo, ...);
        static uint32_t GetOffsetOfScriptFunction() { return offsetof(JavascriptGeneratorFunction, scriptFunction); }

        void SetScriptFunction(GeneratorVirtualScriptFunction* scriptFunction) {
            this->scriptFunction = scriptFunction;
        }

        Var GetHomeObj() const override;
        void SetHomeObj(Var homeObj) override;
        void SetComputedNameVar(Var computedNameVar) override;
        Var GetComputedNameVar() const override;
        bool IsAnonymousFunction() const override;

        Var GetSourceString() const override;
        JavascriptString * EnsureSourceString() override;

        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;

        _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;

        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;

        BOOL IsWritable(PropertyId propertyId) override;
        BOOL IsEnumerable(PropertyId propertyId) override;
        bool IsGeneratorFunction() const override { return true; };

        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
        };

        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
        static Var NewInstanceRestrictedMode(RecyclableObject* function, CallInfo callInfo, ...);

    public:
        VTableValue DummyVirtualFunctionToHinderLinkerICF() const override
        {
            return VTableValue::VtableJavascriptGeneratorFunction;
        }
    };

    template <> bool VarIsImpl<JavascriptGeneratorFunction>(RecyclableObject* obj);

    class GeneratorVirtualScriptFunction : public ScriptFunction
    {
    private:
        friend class JavascriptGeneratorFunction;
        friend Var Js::JavascriptFunction::NewInstanceHelper(ScriptContext*, RecyclableObject*, CallInfo, ArgumentReader&, Js::JavascriptFunction::FunctionKind);

        typename WriteBarrierFieldTypeTraits<JavascriptGeneratorFunction*>::Type realFunction;

    protected:
        DEFINE_VTABLE_CTOR(GeneratorVirtualScriptFunction, ScriptFunction);

    public:
        GeneratorVirtualScriptFunction(FunctionProxy* proxy, ScriptFunctionType* deferredPrototypeType) : ScriptFunction(proxy, deferredPrototypeType) { }

        static uint32_t GetRealFunctionOffset() { return offsetof(GeneratorVirtualScriptFunction, realFunction); }

        JavascriptFunction * GetRealFunctionObject() override { return realFunction; }
        void SetRealGeneratorFunction(JavascriptGeneratorFunction* realFunction) { this->realFunction = realFunction; }

        VTableValue DummyVirtualFunctionToHinderLinkerICF() const override
        {
            return VTableValue::VtableJavascriptGeneratorFunction;
        }
    };

    typedef FunctionWithComputedName<GeneratorVirtualScriptFunction> GeneratorVirtualScriptFunctionWithComputedName;
}
