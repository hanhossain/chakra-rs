//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class NullTypeHandlerBase : public DynamicTypeHandler
    {
    protected:
        NullTypeHandlerBase(bool isPrototype) :
            DynamicTypeHandler(0, 0, 0, DefaultFlags | IsLockedFlag | MayBecomeSharedFlag | IsSharedFlag | (isPrototype ? IsPrototypeFlag : 0)),
            isPrototype(isPrototype) {}

        NullTypeHandlerBase(NullTypeHandlerBase* typeHandler, bool isPrototype) :
            DynamicTypeHandler(typeHandler), isPrototype(typeHandler->isPrototype)
        {}

        DEFINE_VTABLE_CTOR_NO_REGISTER(NullTypeHandlerBase, DynamicTypeHandler);

        typename WriteBarrierFieldTypeTraits<bool, _no_write_barrier_policy, _no_write_barrier_policy>::Type isPrototype;

    public:
        DEFINE_GETCPPNAME();

    public:
        BOOL IsLockable() const override { return true; }
        BOOL IsSharable() const override { return true; }
        int GetPropertyCount() override;
        PropertyId GetPropertyId(ScriptContext* scriptContext, PropertyIndex index) override;
        PropertyId GetPropertyId(ScriptContext* scriptContext, BigPropertyIndex index) override;
        BOOL FindNextProperty(ScriptContext* scriptContext, PropertyIndex& index, JavascriptString** propertyString,
            PropertyId* propertyId, PropertyAttributes* attributes, Type* type, DynamicType *typeToEnumerate, EnumeratorFlags flags, DynamicObject* instance, PropertyValueInfo* info) override;
        PropertyIndex GetPropertyIndex(PropertyRecord const* propertyRecord) override;
#if ENABLE_NATIVE_CODEGEN
        virtual bool GetPropertyEquivalenceInfo(PropertyRecord const* propertyRecord, PropertyEquivalenceInfo& info) override;
        virtual bool IsObjTypeSpecEquivalent(const Type* type, const TypeEquivalenceRecord& record, uint& failedPropertyIndex) override;
        virtual bool IsObjTypeSpecEquivalent(const Type* type, const EquivalentPropertyEntry* entry) override;
#endif
        BOOL HasProperty(DynamicObject* instance, PropertyId propertyId, __out_opt bool *noRedecl = nullptr, _Inout_opt_ PropertyValueInfo* info = nullptr) override;
        BOOL HasProperty(DynamicObject* instance, JavascriptString* propertyNameString) override;
        BOOL GetProperty(DynamicObject* instance, Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL GetProperty(DynamicObject* instance, Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(DynamicObject* instance, JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetInternalProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(DynamicObject* instance, PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL IsEnumerable(DynamicObject* instance, PropertyId propertyId) override;
        BOOL IsWritable(DynamicObject* instance, PropertyId propertyId) override;
        BOOL IsConfigurable(DynamicObject* instance, PropertyId propertyId) override;
        BOOL SetEnumerable(DynamicObject* instance, PropertyId propertyId, BOOL value) override;
        BOOL SetWritable(DynamicObject* instance, PropertyId propertyId, BOOL value) override;
        BOOL SetConfigurable(DynamicObject* instance, PropertyId propertyId, BOOL value) override;
        BOOL SetAccessors(DynamicObject* instance, PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override;
        BOOL PreventExtensions(DynamicObject *instance) override;
        BOOL Seal(DynamicObject* instance) override;
        BOOL SetPropertyWithAttributes(DynamicObject* instance, PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;
        BOOL SetAttributes(DynamicObject* instance, PropertyId propertyId, PropertyAttributes attributes) override;
        BOOL GetAttributesWithPropertyIndex(DynamicObject * instance, PropertyId propertyId, BigPropertyIndex index, PropertyAttributes * attributes) override;

        void SetAllPropertiesToUndefined(DynamicObject* instance, bool invalidateFixedFields) override {};
        void MarshalAllPropertiesToScriptContext(DynamicObject* instance, ScriptContext* targetScriptContext, bool invalidateFixedFields) override {};
        DynamicTypeHandler * ConvertToTypeWithItemAttributes(DynamicObject* instance) override;

        void SetIsPrototype(DynamicObject* instance) override;

#if DBG
        bool SupportsPrototypeInstances() const override { return this->isPrototype; }
        bool RespectsIsolatePrototypes() const override { return false; }
        bool RespectsChangeTypeOnProto() const override { return false; }
#endif

    private:
        template <typename T>
        T* ConvertToTypeHandler(DynamicObject* instance);

        SimpleTypeHandler<1>* ConvertToSimpleType(DynamicObject* instance);
        SimpleDictionaryTypeHandler * ConvertToSimpleDictionaryType(DynamicObject* instance);
        DictionaryTypeHandler * ConvertToDictionaryType(DynamicObject* instance);
        ES5ArrayTypeHandler * ConvertToES5ArrayType(DynamicObject* instance);

        BOOL AddProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags, SideEffects possibleSideEffects);
        BOOL FreezeImpl(DynamicObject* instance, bool isConvertedType) override;
    };

    template <bool IsPrototypeTemplate>
    class NullTypeHandler : public NullTypeHandlerBase
    {
    public:
        DEFINE_GETCPPNAME();

    private:
        NullTypeHandler() : NullTypeHandlerBase(IsPrototypeTemplate) {}
        NullTypeHandler(NullTypeHandler * typeHandler) : NullTypeHandlerBase(typeHandler) {}

        DEFINE_VTABLE_CTOR_NO_REGISTER(NullTypeHandler, NullTypeHandlerBase);

        static NullTypeHandler defaultInstance;

    public:
        static NullTypeHandler * GetDefaultInstance();
        DynamicTypeHandler * Clone(Recycler * recycler) override;

#if DBG_DUMP
    public:
        void Dump(unsigned indent = 0) const override;
#endif
    };
}
