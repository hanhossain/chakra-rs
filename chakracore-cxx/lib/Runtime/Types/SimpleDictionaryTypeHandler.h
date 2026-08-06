//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//
// This type handler supports storage of a map of property Id to index along with
// property descriptors for every property.
//
// It can be shared across objects as long as no properties are
// added, deleted or changed. Once the property structure is changed the type is marked as
// non-shared. In non-shared state, any property structural changes can happen without creating
// a new type or handler.
//
// Type transition to DictionaryTypeHandler happens on the use of setters.
//

#pragma once
#include "Interface/TemplateParameter.h"

namespace Js
{
    template <typename TMapKey>
    struct PropertyMapKeyTraits
    {
    };

    template <>
    struct PropertyMapKeyTraits<const PropertyRecord*>
    {
        template <typename TKey, typename TValue>
        class Entry : public JsUtil::SimpleDictionaryEntry<TKey, TValue> { };

        static bool IsStringTypeHandler() { return false; };
    };

    template <>
    struct PropertyMapKeyTraits<JavascriptString*>
    {
        template <typename TKey, typename TValue>
        class Entry : public JsUtil::DictionaryEntry<TKey, TValue> { };

        static bool IsStringTypeHandler() { return true; };
    };

    // Template parameters:
    // - TPropertyIndex: property index type: PropertyIndex, BigPropertyIndex, etc.
    // - TMapKey: key type for property map: PropertyRecord* const, JavascriptString*
    // - IsNotExtensibleSupported: whether the following features are supported preventExtensions, seal, freeze.
    template <typename TPropertyIndex, typename TMapKey, bool IsNotExtensibleSupported>
    class SimpleDictionaryTypeHandlerBase : public DynamicTypeHandler
    {
    private:
        friend class NullTypeHandlerBase;
        friend class DeferredTypeHandlerBase;
        friend class PathTypeHandlerBase;
        template<size_t size>
        friend class SimpleTypeHandler;

        template <typename _TPropertyIndex, typename _TMapKey, bool _IsNotExtensibleSupported> friend class SimpleDictionaryTypeHandlerBase;

        // Explicit non leaf allocator now that the key is non-leaf
        typedef JsUtil::BaseDictionary<TMapKey, SimpleDictionaryPropertyDescriptor<TPropertyIndex>, RecyclerNonLeafAllocator, DictionarySizePolicy<PowerOf2Policy, 1>, PropertyRecordStringHashComparer, PropertyMapKeyTraits<TMapKey>::template Entry>
            SimplePropertyDescriptorMap;
        typedef SimplePropertyDescriptorMap PropertyDescriptorMapType; // alias used by diagnostics

    protected:
        typename WriteBarrierFieldTypeTraits<SimplePropertyDescriptorMap*>::Type propertyMap;

    private:
        typename WriteBarrierFieldTypeTraits<RecyclerWeakReference<DynamicObject>*>::Type singletonInstance;

    protected:
        typename WriteBarrierFieldTypeTraits<bool>::Type _gc_tag : 1;  // Tag the low bit to prevent possible GC false references
        // Determines whether this instance is actually a SimpleDictionaryUnorderedTypeHandler
        typename WriteBarrierFieldTypeTraits<bool>::Type isUnordered : 1;
        // Tracks if an InternalPropertyRecord or symbol has been added to this type; will prevent conversion to string-keyed type handler
        typename WriteBarrierFieldTypeTraits<bool>::Type hasNamelessPropertyId : 1;
    private:
        // Number of deleted properties in the property map
        typename WriteBarrierFieldTypeTraits<byte>::Type numDeletedProperties;
        typename WriteBarrierFieldTypeTraits<TPropertyIndex>::Type nextPropertyIndex;

    public:
        DEFINE_GETCPPNAME();

    protected:
        SimpleDictionaryTypeHandlerBase(Recycler * recycler);
        SimpleDictionaryTypeHandlerBase(Recycler * recycler, int slotCapacity, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false);
        SimpleDictionaryTypeHandlerBase(ScriptContext * scriptContext, SimplePropertyDescriptor const* propertyDescriptors, int propertyCount, int slotCapacity, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false, bool isInitialized = false);
        SimpleDictionaryTypeHandlerBase(Recycler* recycler, int slotCapacity, int propertyCapacity, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false);
        SimpleDictionaryTypeHandlerBase(Recycler* recycler, SimpleDictionaryTypeHandlerBase * typeHandler);
        DEFINE_VTABLE_CTOR_NO_REGISTER(SimpleDictionaryTypeHandlerBase, DynamicTypeHandler);

        typedef PropertyIndexRanges<TPropertyIndex> PropertyIndexRangesType;
        static const TPropertyIndex MaxPropertyIndexSize = PropertyIndexRangesType::MaxValue;
        static const TPropertyIndex NoSlots = PropertyIndexRangesType::NoSlots;

    public:
        typedef TPropertyIndex PropertyIndexType;

        // Create a new type handler for a future DynamicObject. This is for public usage. "initialCapacity" indicates desired slotCapacity, subject to alignment round up.
        static SimpleDictionaryTypeHandlerBase * New(Recycler * recycler, int initialCapacity, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false);

        // Create a new type handler for a future DynamicObject. This is for public usage. "propertyCount" indicates desired slotCapacity, subject to alignment round up.
        static SimpleDictionaryTypeHandlerBase * New(ScriptContext * scriptContext, SimplePropertyDescriptor const* propertyDescriptors, int propertyCount, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false);

#if ENABLE_FIXED_FIELDS
        static SimpleDictionaryTypeHandlerBase * NewInitialized(ScriptContext * scriptContext, SimplePropertyDescriptor const* propertyDescriptors, int propertyCount, uint16 inlineSlotCapacity, uint16 offsetOfInlineSlots, bool isLocked = false, bool isShared = false);
#endif

        static DynamicType* CreateTypeForNewScObject(ScriptContext* scriptContext, DynamicType* type, const Js::PropertyIdArray *propIds, bool shareType, bool check__proto__);

        DynamicTypeHandler * Clone(Recycler * recyler) override;
        BOOL IsStringTypeHandler() const override { return PropertyMapKeyTraits<TMapKey>::IsStringTypeHandler(); }

        BOOL IsLockable() const override { return true; }
        BOOL IsSharable() const override { return true; }

        int GetPropertyCount() override;

        PropertyId GetPropertyId(ScriptContext* scriptContext, PropertyIndex index) override;
        PropertyId GetPropertyId(ScriptContext* scriptContext, BigPropertyIndex index) override;
        PropertyIndex GetPropertyIndex(const PropertyRecord* propertyRecord) override;
#if ENABLE_NATIVE_CODEGEN
        virtual bool GetPropertyEquivalenceInfo(PropertyRecord const* propertyRecord, PropertyEquivalenceInfo& info) override;
        virtual bool IsObjTypeSpecEquivalent(const Type* type, const TypeEquivalenceRecord& record, uint& failedPropertyIndex) override;
        virtual bool IsObjTypeSpecEquivalent(const Type* type, const EquivalentPropertyEntry* entry) override;
#endif

        BOOL FindNextProperty(ScriptContext* scriptContext, PropertyIndex& index, JavascriptString** propertyString,
            PropertyId* propertyId, PropertyAttributes* attributes, Type* type, DynamicType *typeToEnumerate, EnumeratorFlags flags, DynamicObject* instance, PropertyValueInfo* info) override;
        BOOL FindNextProperty(ScriptContext* scriptContext, BigPropertyIndex& index, JavascriptString** propertyString,
            PropertyId* propertyId, PropertyAttributes* attributes, Type* type, DynamicType *typeToEnumerate, EnumeratorFlags flags, DynamicObject* instance, PropertyValueInfo* info) override;

        BOOL HasProperty(DynamicObject* instance, PropertyId propertyId, bool *noRedecl = nullptr, _Inout_opt_ PropertyValueInfo* info = nullptr) override;
        BOOL HasProperty(DynamicObject* instance, JavascriptString* propertyNameString) override;
        BOOL GetProperty(DynamicObject* instance, Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL GetProperty(DynamicObject* instance, Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(DynamicObject* instance, JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetInternalProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags) override;
        DescriptorFlags GetSetter(DynamicObject* instance, PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(DynamicObject* instance, JavascriptString* propertyNameString, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL DeleteProperty(DynamicObject* instance, PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(DynamicObject* instance, JavascriptString* propertyNameString, PropertyOperationFlags flags) override;

        PropertyIndex GetRootPropertyIndex(const PropertyRecord* propertyRecord) override;

        BOOL HasRootProperty(DynamicObject* instance, PropertyId propertyId, bool *noRedecl, bool *pDeclaredProperty = nullptr, bool *pNonconfigurableProperty = nullptr) override;
        BOOL GetRootProperty(DynamicObject* instance, Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetRootProperty(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        DescriptorFlags GetRootSetter(DynamicObject* instance, PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL DeleteRootProperty(DynamicObject* instance, PropertyId propertyId, PropertyOperationFlags flags) override;

        BOOL IsSimpleDictionaryTypeHandler() const override { return TRUE;}
#if DBG
        bool IsLetConstGlobal(DynamicObject* instance, PropertyId propertyId) override;
#endif
        bool NextLetConstGlobal(int& index, RootObjectBase* instance, const PropertyRecord** propertyRecord, Var* value, bool* isConst) override;

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
        BOOL IsSealed(DynamicObject* instance) override;
        BOOL IsFrozen(DynamicObject* instance) override;
        BOOL SetAttributes(DynamicObject* instance, PropertyId propertyId, PropertyAttributes attributes) override;
        BOOL GetAttributesWithPropertyIndex(DynamicObject * instance, PropertyId propertyId, BigPropertyIndex index, PropertyAttributes * attributes) override;
        BOOL SetItem(DynamicObject* instance, uint32_t index, Var value, PropertyOperationFlags flags) override;

        void SetAllPropertiesToUndefined(DynamicObject* instance, bool invalidateFixedFields) override;
        void MarshalAllPropertiesToScriptContext(DynamicObject* instance, ScriptContext* targetScriptContext, bool invalidateFixedFields) override;
        DynamicTypeHandler * ConvertToTypeWithItemAttributes(DynamicObject* instance) override;

        void SetIsPrototype(DynamicObject* instance) override;

#if DBG
        bool SupportsPrototypeInstances() const override { return true; }
        bool CanStorePropertyValueDirectly(const DynamicObject* instance, PropertyId propertyId, bool allowLetConst) override;
#endif

#if ENABLE_FIXED_FIELDS
        virtual void DoShareTypeHandler(ScriptContext* scriptContext) override;
        virtual BOOL IsFixedProperty(const DynamicObject* instance, PropertyId propertyId) override;
        virtual bool HasSingletonInstance() const override;
        virtual bool TryUseFixedProperty(PropertyRecord const * propertyRecord, Var * pProperty, FixedPropertyKind propertyType, ScriptContext * requestContext) override;
        virtual bool TryUseFixedAccessor(PropertyRecord const * propertyRecord, Var * pAccessor, FixedPropertyKind propertyType, bool getter, ScriptContext * requestContext) override;

#if DBG
        virtual bool CheckFixedProperty(PropertyRecord const * propertyRecord, Var * pProperty, ScriptContext * requestContext) override;
        virtual bool HasAnyFixedProperties() const override;
#endif

        virtual void DumpFixedFields() const override;
        static void TraceFixedFieldsBeforeTypeHandlerChange(
            const char16_t* oldTypeHandlerName, const char16_t* newTypeHandlerName,
            DynamicObject* instance, DynamicTypeHandler* oldTypeHandler, DynamicType* oldType, RecyclerWeakReference<DynamicObject>* oldSingletonInstanceBefore);
        static void TraceFixedFieldsAfterTypeHandlerChange(
            DynamicObject* instance, DynamicTypeHandler* oldTypeHandler, DynamicTypeHandler* newTypeHandler,
            DynamicType* oldType, RecyclerWeakReference<DynamicObject>* oldSingletonInstanceBefore);
        static void TraceFixedFieldsBeforeSetIsProto(
            DynamicObject* instance, DynamicTypeHandler* oldTypeHandler, DynamicType* oldType, RecyclerWeakReference<DynamicObject>* oldSingletonInstanceBefore);
        static void TraceFixedFieldsAfterSetIsProto(
            DynamicObject* instance, DynamicTypeHandler* oldTypeHandler, DynamicTypeHandler* newTypeHandler,
            DynamicType* oldType, RecyclerWeakReference<DynamicObject>* oldSingletonInstanceBefore);

    private:

        template <bool allowNonExistent, bool markAsUsed>
        bool TryGetFixedProperty(PropertyRecord const * propertyRecord, Var * pProperty, FixedPropertyKind propertyType, ScriptContext * requestContext);

        template <typename TPropertyKey>
        void InvalidateFixedField(const TPropertyKey propertyKey, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor, ScriptContext* scriptContext);
    public:
        RecyclerWeakReference<DynamicObject>* GetSingletonInstance() const override
        { Assert(HasSingletonInstanceOnlyIfNeeded()); return this->singletonInstance; }

        virtual void SetSingletonInstanceUnchecked(RecyclerWeakReference<DynamicObject>* instance) override
        {
            Assert(!GetIsShared());
            Assert(this->singletonInstance == nullptr);
            this->singletonInstance = instance;
        }

        virtual void ClearSingletonInstance() override
        {
            Assert(HasSingletonInstanceOnlyIfNeeded());
            this->singletonInstance = nullptr;
        }

#if DBG
        bool HasSingletonInstanceOnlyIfNeeded() const
        {
            return AreSingletonInstancesNeeded() || this->singletonInstance == nullptr;
        }
#endif
#endif
    private:
        typedef SimpleDictionaryTypeHandlerBase<BigPropertyIndex, TMapKey, false> BigSimpleDictionaryTypeHandler;

#if ENABLE_NATIVE_CODEGEN
        template <bool doLock>
        bool IsObjTypeSpecEquivalentImpl(const Type* type, const EquivalentPropertyEntry *entry);
#endif
        void SetIsPrototype(DynamicObject* instance, bool hasNewType);
        bool SupportsSwitchingToUnordered(const ScriptContext *const scriptContext) const;
        SimpleDictionaryUnorderedTypeHandler<TPropertyIndex, TMapKey, IsNotExtensibleSupported> *AsUnordered();
        void SetNumDeletedProperties(const byte n);

        template <typename U, typename UMapKey>
        U* ConvertToTypeHandler(DynamicObject* instance);

        template <typename TPropertyKey>
        void Add(TPropertyKey propertyKey, PropertyAttributes attributes, ScriptContext *const scriptContext);
        template <typename TPropertyKey>
        void Add(TPropertyKey propertyKey, PropertyAttributes attributes, bool isInitialized, bool isFixed, bool usedAsFixed, ScriptContext *const scriptContext);
        template <typename TPropertyKey>
        void Add(TPropertyIndex propertyIndex, TPropertyKey propertyKey, PropertyAttributes attributes, ScriptContext *const scriptContext);
        template <typename TPropertyKey>
        void Add(TPropertyIndex propertyIndex, TPropertyKey propertyKey, PropertyAttributes attributes, bool isInitialized, bool isFixed, bool usedAsFixed, ScriptContext *const scriptContext);
        DictionaryTypeHandlerBase<TPropertyIndex>* ConvertToDictionaryType(DynamicObject* instance);
        ES5ArrayTypeHandlerBase<TPropertyIndex>* ConvertToES5ArrayType(DynamicObject* instance);
        SimpleDictionaryTypeHandlerBase* ConvertToNonSharedSimpleDictionaryType(DynamicObject* instance);
        template <typename NewTPropertyIndex, typename NewTMapKey, bool NewIsNotExtensibleSupported> SimpleDictionaryUnorderedTypeHandler<NewTPropertyIndex, NewTMapKey, NewIsNotExtensibleSupported>* ConvertToSimpleDictionaryUnorderedTypeHandler(DynamicObject* instance);
        BOOL SetAttribute(DynamicObject* instance, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor, PropertyAttributes attribute);
        BOOL ClearAttribute(DynamicObject* instance, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor, PropertyAttributes attribute);
        void EnsureSlotCapacity(DynamicObject * instance);
        template <typename TPropertyKey>
        BOOL AddProperty(DynamicObject* instance, TPropertyKey propertyKey, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags, SideEffects possibleSideEffects);
        BOOL FreezeImpl(DynamicObject* instance, bool isConvertedType) override;

        template <bool allowLetConstGlobal>
        inline BOOL HasProperty_Internal(DynamicObject* instance, PropertyId propertyId, bool *noRedecl, _Inout_opt_ PropertyValueInfo* info, bool *pDeclaredProperty, bool *pNonconfigurableProperty);
        template <bool allowLetConstGlobal>
        inline PropertyIndex GetPropertyIndex_Internal(const PropertyRecord* propertyRecord);
        template <bool allowLetConstGlobal>
        inline BOOL GetProperty_Internal(DynamicObject* instance, Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext);
        template <bool allowLetConstGlobal>
        inline BOOL SetProperty_Internal(DynamicObject* instance, PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info);
        template <bool allowLetConstGlobal>
        inline DescriptorFlags GetSetter_Internal(DynamicObject* instance, PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext);
        template <bool allowLetConstGlobal>
        inline BOOL DeleteProperty_Internal(DynamicObject* instance, PropertyId propertyId, PropertyOperationFlags flags);

        template <bool allowLetConstGlobal>
        inline BOOL GetPropertyFromDescriptor(DynamicObject* instance, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor, Var* value, PropertyValueInfo* info);
        template <bool allowLetConstGlobal, typename TPropertyKey>
        inline BOOL SetPropertyFromDescriptor(DynamicObject* instance, PropertyId propertyId, TPropertyKey propertyKey, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor, Var value, PropertyOperationFlags flags, PropertyValueInfo* info);
        template <bool allowLetConstGlobal>
        inline DescriptorFlags GetSetterFromDescriptor(SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor);

        BOOL SetProperty_JavascriptString(DynamicObject* instance, JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info, TemplateParameter::Box<const PropertyRecord*>);
        BOOL SetProperty_JavascriptString(DynamicObject* instance, JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info, TemplateParameter::Box<JavascriptString*>);

        BigSimpleDictionaryTypeHandler* ConvertToBigSimpleDictionaryTypeHandler(DynamicObject* instance);
        void SetPropertyValueInfo(PropertyValueInfo* info, RecyclableObject* instance, SimpleDictionaryPropertyDescriptor<TPropertyIndex>* descriptor);
        void SetPropertyValueInfoNonFixed(PropertyValueInfo* info, RecyclableObject* instance, TPropertyIndex propIndex, PropertyAttributes attributes, InlineCacheFlags flags = InlineCacheNoFlags);

        BOOL PreventExtensionsInternal(DynamicObject* instance);
        BOOL SealInternal(DynamicObject* instance);
        BOOL FreezeInternal(DynamicObject* instance, bool isConvertedType = false);

        // This was added to work around not being able to specify partial template specialization of member function.
        BOOL FindNextProperty_BigPropertyIndex(ScriptContext* scriptContext, TPropertyIndex& index, JavascriptString** propertyString,
            PropertyId* propertyId, PropertyAttributes* attributes, Type* type, DynamicType *typeToEnumerate, EnumeratorFlags flags, DynamicObject* instance, PropertyValueInfo* info);

        template <bool check__proto__>
        static DynamicType* InternalCreateTypeForNewScObject(ScriptContext* scriptContext, DynamicType* type, const Js::PropertyIdArray *propIds, bool shareType);

#if DBG_DUMP
    public:
        void Dump(unsigned indent = 0) const override;
#endif
    };

}
