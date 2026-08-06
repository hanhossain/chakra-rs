//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Language/ModuleRecordBase.h"

namespace Js
{
    class ModuleNamespace : public DynamicObject
    {
    public:
        friend class ModuleNamespaceEnumerator;
        typedef JsUtil::BaseDictionary<PropertyId, ModuleNameRecord, RecyclerLeafAllocator, PowerOf2SizePolicy> UnambiguousExportMap;
        typedef JsUtil::BaseDictionary<const PropertyRecord*, SimpleDictionaryPropertyDescriptor<BigPropertyIndex>, RecyclerNonLeafAllocator,
            DictionarySizePolicy<PowerOf2Policy, 1>, PropertyRecordStringHashComparer, PropertyMapKeyTraits<const PropertyRecord*>::template Entry>
            SimplePropertyDescriptorMap;
    protected:
        DEFINE_VTABLE_CTOR(ModuleNamespace, DynamicObject);
        friend class Js::CrossSiteObject<ModuleNamespace>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ModuleNamespace>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ModuleNamespace>>::SetVirtualTable(this);
        };

    protected:
        ModuleNamespace(ModuleRecordBase* moduleRecord, DynamicType * type);
        static ModuleNamespace* New(ModuleRecordBase* moduleRecord);
    public:

        class EntryInfo
        {
        };

        static ModuleNamespace* GetModuleNamespace(ModuleRecordBase* moduleRecord);
        void Initialize();
        ListForListIterator* GetSortedExportedNames() { return this->sortedExportedNames; }

        PropertyId GetPropertyId(BigPropertyIndex index) override;
        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        BOOL HasOwnProperty(PropertyId propertyId) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL GetInternalProperty(Var instance, PropertyId internalPropertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { return FALSE; }
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { return FALSE; }
        BOOL SetInternalProperty(PropertyId internalPropertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { return FALSE; }
        DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override { return DescriptorFlags::None; }
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override { return DescriptorFlags::None; }
        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = nullptr) override { Assert(false); return FALSE; }
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override { return false; }
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
#if ENABLE_FIXED_FIELDS
        virtual BOOL IsFixedProperty(PropertyId propertyId) override { return false; }
#endif
        PropertyQueryFlags HasItemQuery(uint32_t index) override { return PropertyQueryFlags::Property_NotFound; }
        BOOL HasOwnItem(uint32_t index) override { return false; }
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { return PropertyQueryFlags::Property_NotFound; }
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { return PropertyQueryFlags::Property_NotFound; }
        DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override { *setterValue = nullptr; return DescriptorFlags::None; }
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override { return false; }
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override { return true; }
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override { return false; }
        _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext* requestContext) override { return FALSE; };
        BOOL IsWritable(PropertyId propertyId) override;
        BOOL IsConfigurable(PropertyId propertyId) override;
        BOOL IsEnumerable(PropertyId propertyId) override;
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override { return false; }
        BOOL SetWritable(PropertyId propertyId, BOOL value) override { return false; }
        BOOL IsProtoImmutable() const override { return true; }
        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override { return false; }
        BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override { return false; }
        BOOL IsExtensible() override { return false; };
        BOOL PreventExtensions() override { return true; }
        BOOL Seal() override { return false; }
        BOOL Freeze() override { return false; }
        BOOL IsSealed() override { return true; }
        BOOL IsFrozen() override { return true; }
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;

        void RemoveFromPrototype(ScriptContext * requestContext, bool * allProtoCachesInvalidated) override { Assert(false); }
        void AddToPrototype(ScriptContext * requestContext, bool * allProtoCachesInvalidated) override { Assert(false); }
        void SetPrototype(RecyclableObject* newPrototype) override { Assert(false); return; }

    private:
        typename WriteBarrierFieldTypeTraits<ModuleRecordBase*>::Type moduleRecord;
        typename WriteBarrierFieldTypeTraits<UnambiguousExportMap*>::Type unambiguousNonLocalExports;
        typename WriteBarrierFieldTypeTraits<SimplePropertyDescriptorMap*>::Type propertyMap;   // local exports.
        typename WriteBarrierFieldTypeTraits<ListForListIterator*>::Type sortedExportedNames;   // sorted exported names for both local and indirect exports; excludes symbols.
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<Var>::Type*>::Type nsSlots;

        void SetNSSlotsForModuleNS(typename WriteBarrierFieldTypeTraits<Var>::Type* nsSlot) { this->nsSlots = nsSlot; }
        Var GetNSSlot(BigPropertyIndex propertyIndex);
        void AddUnambiguousNonLocalExport(PropertyId exportId, ModuleNameRecord* nonLocalExportNameRecord);
        UnambiguousExportMap* GetUnambiguousNonLocalExports() const { return unambiguousNonLocalExports; }

        // Methods used by NamespaceEnumerator;
        BOOL FindNextProperty(BigPropertyIndex& index, JavascriptString** propertyString, PropertyId* propertyId, PropertyAttributes* attributes, ScriptContext * requestContext) const;
    };

    template <> inline bool VarIsImpl<ModuleNamespace>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_ModuleNamespace;
    }
}
