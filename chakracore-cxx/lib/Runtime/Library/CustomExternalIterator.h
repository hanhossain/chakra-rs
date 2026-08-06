//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

namespace Js
{
    typedef void(*InitIteratorFunction)(Var, Var);
    typedef bool(*NextFunction)(Var, Var *, Var *);

    enum class ExternalIteratorKind
    {
        External_Keys,
        External_Values,
        External_KeyAndValue,
    };

    class ExternalIteratorCreatorFunction : public RuntimeFunction
    {
    protected:
        friend class Js::CrossSiteObject<ExternalIteratorCreatorFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ExternalIteratorCreatorFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ExternalIteratorCreatorFunction>>::SetVirtualTable(this);
        };
        DEFINE_VTABLE_CTOR(ExternalIteratorCreatorFunction, RuntimeFunction);

    public:
        ExternalIteratorCreatorFunction(DynamicType* type,
            FunctionInfo* functionInfo,
            JavascriptTypeId typeId,
            uint byteCount,
            Var prototypeForIterator, InitIteratorFunction initFunction, NextFunction nextFunction);

        BOOL IsExternalFunction() override { return TRUE; }

        static Var EntryExternalEntries(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryExternalKeys(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryExternalValues(RecyclableObject* function, CallInfo callInfo, ...);

        void ThrowIfNotValidObject(Var instance);
        static Var CreateCustomExternalIterator(Var instance, ExternalIteratorCreatorFunction* function, ExternalIteratorKind kind);

        static Var CreateFunction(JavascriptLibrary *library,
            JavascriptTypeId typeId,
            JavascriptMethod entryPoint,
            uint byteCount,
            Var prototypeForIterator, InitIteratorFunction initFunction, NextFunction nextFunction);

    public:
        typename WriteBarrierFieldTypeTraits<JavascriptTypeId>::Type m_externalTypeId;
        typename WriteBarrierFieldTypeTraits<uint>::Type m_extraByteCount;
        typename WriteBarrierFieldTypeTraits<InitIteratorFunction>::Type m_initFunction;
        typename WriteBarrierFieldTypeTraits<NextFunction>::Type m_nextFunction;
        typename WriteBarrierFieldTypeTraits<Var>::Type m_prototypeForIterator;

        friend class JavascriptLibrary;
    };

    class JavascriptExternalIteratorNextFunction : public RuntimeFunction
    {
    protected:
        friend class Js::CrossSiteObject<JavascriptExternalIteratorNextFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptExternalIteratorNextFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptExternalIteratorNextFunction>>::SetVirtualTable(this);
        };
        DEFINE_VTABLE_CTOR(JavascriptExternalIteratorNextFunction, RuntimeFunction);

        typename WriteBarrierFieldTypeTraits<JavascriptTypeId>::Type m_externalTypeId;

        JavascriptExternalIteratorNextFunction(DynamicType* type, FunctionInfo* functionInfo, JavascriptTypeId typeId);
    public:
        BOOL IsExternalFunction() override { return TRUE; }
        JavascriptTypeId GetExternalTypeId() const { return m_externalTypeId; }

        static JavascriptExternalIteratorNextFunction* CreateFunction(JavascriptLibrary *library, JavascriptTypeId typeId, JavascriptMethod entryPoint);

        friend class JavascriptLibrary;
    };

    class CustomExternalIterator : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<ExternalIteratorKind>::Type m_kind;
        typename WriteBarrierFieldTypeTraits<JavascriptTypeId>::Type m_externalTypeId;
        typename WriteBarrierFieldTypeTraits<NextFunction>::Type m_nextFunction;

    protected:
        DEFINE_VTABLE_CTOR(CustomExternalIterator, DynamicObject);
        friend class Js::CrossSiteObject<CustomExternalIterator>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<CustomExternalIterator>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<CustomExternalIterator>>::SetVirtualTable(this);
        };

    public:
        CustomExternalIterator(DynamicType* type, ExternalIteratorKind kind, JavascriptTypeId typeId, NextFunction nextFunction);

        static Var CreateNextFunction(JavascriptLibrary *library, JavascriptTypeId typeId);
        static Var EntryNext(RecyclableObject* function, CallInfo callInfo, ...);
    };

    template <> bool VarIsImpl<CustomExternalIterator>(RecyclableObject* obj);

}
