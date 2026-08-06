//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//  Implements JavascriptProxy.
//----------------------------------------------------------------------------
#pragma once

namespace Js
{
    // Host should keep the same object in cross-site scenario.
    class JavascriptProxy : public DynamicObject
    {
        friend class JavascriptOperators;
    protected:
        DEFINE_VTABLE_CTOR(JavascriptProxy, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptProxy>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptProxy>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptProxy>>::SetVirtualTable(this);
        };
    private:
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type handler;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type target;

        void RevokeObject();
    public:
        static const uint32_t MAX_STACK_CALL_ARGUMENT_COUNT = 20;
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo Revocable;
            static FunctionInfo Revoke;
        };
        typedef enum SetPropertyTrapKind {
            SetItemOnTaggedNumberKind,
            SetPropertyOnTaggedNumberKind,
            SetPropertyKind,
            SetItemKind,
            SetPropertyWPCacheKind,
        } SetPropertyTrapKind;

        enum KeysTrapKind {
            GetOwnPropertyNamesKind,
            GetOwnPropertySymbolKind,
            KeysKind
        };

        enum IntegrityLevel {
            IntegrityLevel_sealed,
            IntegrityLevel_frozen
        };

        JavascriptProxy(DynamicType * type);
        JavascriptProxy(DynamicType * type, ScriptContext * scriptContext, RecyclableObject* target, RecyclableObject* handler);

        // before recursively calling something on 'target' use this helper in case there is nesting of proxies.
        // the proxies could be deep nested and cause SO when processed recursively.
        static const JavascriptProxy* UnwrapNestedProxies(const JavascriptProxy* proxy);

#ifndef IsJsDiag
        RecyclableObject* GetTarget();
        RecyclableObject* GetHandler();
#else
        RecyclableObject* GetTarget() { return target; }
        RecyclableObject* GetHandler() { return handler; }
#endif
        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryRevocable(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryRevoke(RecyclableObject* function, CallInfo callInfo, ...);

        static Var FunctionCallTrap(RecyclableObject* function, CallInfo callInfo, ...);
        static JavascriptProxy* Create(ScriptContext* scriptContext, Arguments args);

        static BOOL GetOwnPropertyDescriptor(RecyclableObject* obj, PropertyId propertyId, ScriptContext* requestContext, PropertyDescriptor* propertyDescriptor);
        static BOOL DefineOwnPropertyDescriptor(RecyclableObject* obj, PropertyId propId, const PropertyDescriptor& descriptor, bool throwOnError, ScriptContext* requestContext, PropertyOperationFlags flags);

        static uint32_t GetOffsetOfTarget() { return offsetof(JavascriptProxy, target); }

        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        BOOL HasOwnProperty(PropertyId propertyId) override;
        BOOL HasOwnPropertyNoHostObject(PropertyId propertyId) override;
        BOOL HasOwnPropertyCheckNoRedecl(PropertyId propertyId) override;
        BOOL UseDynamicObjectForNoHostObjectAccess() override;
        DescriptorFlags GetSetter(PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL GetInternalProperty(Var instance, PropertyId internalPropertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetInternalProperty(PropertyId internalPropertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        BOOL EnsureProperty(PropertyId propertyId) override;
        BOOL EnsureNoRedeclProperty(PropertyId propertyId) override;
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;
        BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
        BOOL InitFuncScoped(PropertyId propertyId, Var value) override;
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
#if ENABLE_FIXED_FIELDS
        virtual BOOL IsFixedProperty(PropertyId propertyId) override;
#endif
        PropertyQueryFlags HasItemQuery(uint32_t index) override;
        BOOL HasOwnItem(uint32_t index) override;
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override;
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override;
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override;
        BOOL Equals(Var other, BOOL* value, ScriptContext* requestContext) override;
        BOOL StrictEquals(Var other, BOOL* value, ScriptContext* requestContext) override;
        BOOL IsWritable(PropertyId propertyId) override;
        BOOL IsConfigurable(PropertyId propertyId) override;
        BOOL IsEnumerable(PropertyId propertyId) override;
        BOOL IsExtensible() override;
        BOOL PreventExtensions() override;
        void ThrowIfCannotDefineProperty(PropertyId propId, const PropertyDescriptor& descriptor) override { }
        BOOL Seal() override;
        BOOL Freeze() override;
        BOOL IsSealed() override;
        BOOL IsFrozen() override;
        BOOL SetWritable(PropertyId propertyId, BOOL value) override;
        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override;
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override;
        BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override;
        BOOL GetSpecialPropertyName(uint32_t index, JavascriptString **propertyName,
                                    ScriptContext *requestContext) override
        {
            return false;
        }
        uint GetSpecialPropertyCount() const override { return 0; }
        PropertyId const * GetSpecialPropertyIds() const override { return nullptr; }
        BOOL HasInstance(Var instance, ScriptContext* scriptContext, IsInstInlineCache* inlineCache = NULL) override;
        // This is used for external object only; should not be called for proxy
        RecyclableObject * GetConfigurablePrototype(ScriptContext * requestContext) override;
        RecyclableObject * GetPrototypeSpecial() override;
        // for external object. don't need it here.
        Js::JavascriptString* GetClassName(ScriptContext * requestContext) override;

#if DBG
        bool CanStorePropertyValueDirectly(PropertyId propertyId, bool allowLetConst) override
        { Assert(false); return false; };
#endif

        void RemoveFromPrototype(ScriptContext * requestContext, bool * allProtoCachesInvalidated) override;
        void AddToPrototype(ScriptContext * requestContext, bool * allProtoCachesInvalidated) override;
        void SetPrototype(RecyclableObject* newPrototype) override;

        BOOL SetPrototypeTrap(RecyclableObject* newPrototype, bool showThrow, ScriptContext * requestContext);
        Var ToString(Js::ScriptContext* scriptContext);

        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        RecyclableObject * ToObject(ScriptContext * requestContext) override;
        Var GetTypeOfString(ScriptContext* requestContext) override;

        bool IsRevoked() const;
        BOOL SetPropertyTrap(Var receiver, SetPropertyTrapKind setPropertyTrapKind, PropertyId propertyId, Var newValue, ScriptContext* requestContext, PropertyOperationFlags propertyOperationFlags, BOOL skipPrototypeCheck = FALSE);
        BOOL SetPropertyTrap(Var receiver, SetPropertyTrapKind setPropertyTrapKind, Js::JavascriptString * propertyString, Var newValue, ScriptContext* requestContext, PropertyOperationFlags propertyOperationFlags);

        void PropertyIdFromInt(uint32_t index, PropertyRecord const** propertyRecord);

        JavascriptArray* PropertyKeysTrap(KeysTrapKind keysTrapKind, ScriptContext* requestContext);

        template <class Fn>
        void GetOwnPropertyKeysHelper(ScriptContext* scriptContext, RecyclableObject* trapResultArray, uint32_t len, JavascriptArray* trapResult,
            JsUtil::BaseDictionary<Js::PropertyId, bool, ArenaAllocator>& targetToTrapResultMap, Fn fn)
        {
            Var element = nullptr;
            const PropertyRecord* propertyRecord;
            uint32_t trapResultIndex = 0;
            PropertyId propertyId;
            for (uint32_t i = 0; i < len; i++)
            {
                if (!JavascriptOperators::GetItem(trapResultArray, i, &element, scriptContext) || // missing
                    !(VarIs<JavascriptString>(element) || VarIs<JavascriptSymbol>(element)))  // neither String nor Symbol
                {
                    JavascriptError::ThrowTypeError(scriptContext, JSERR_InconsistentTrapResult, u"ownKeys");
                }

                JavascriptConversion::ToPropertyKey(element, scriptContext, &propertyRecord, nullptr);
                propertyId = propertyRecord->GetPropertyId();

                if (propertyId != Constants::NoProperty)
                {
                    if (targetToTrapResultMap.AddNew(propertyId, true) == -1)
                    {
                        JavascriptError::ThrowTypeError(scriptContext, JSERR_DuplicateKeysFromOwnPropertyKeys);
                    }
                }

                if (fn(propertyRecord))
                {
                    trapResult->DirectSetItemAt(trapResultIndex++, element);
                }
            }
        }

        Var ConstructorTrap(Arguments args, ScriptContext* scriptContext, const Js::AuxArray<uint32_t> *spreadIndices);


        static PropertyId EnsureHandlerPropertyId(ScriptContext* scriptContext);

    private:
        JavascriptFunction* GetMethodHelper(PropertyId methodId, ScriptContext* requestContext);
        Var GetValueFromDescriptor(Var instance, PropertyDescriptor propertyDescriptor, ScriptContext* requestContext);
        static Var GetName(ScriptContext* requestContext, PropertyId propertyId);

        RecyclableObject* MarshalHandler(ScriptContext* requestContext) const
        {
            return static_cast<RecyclableObject*>(
                CrossSite::MarshalVar(requestContext, handler));
        }
        RecyclableObject* MarshalTarget(ScriptContext* requestContext) const
        {
            return static_cast<RecyclableObject*>(
                CrossSite::MarshalVar(requestContext, target));
        }

        static BOOL TestIntegrityLevel(IntegrityLevel integrityLevel, RecyclableObject* obj, ScriptContext* scriptContext);
        static BOOL SetIntegrityLevel(IntegrityLevel integrityLevel, RecyclableObject* obj, ScriptContext* scriptContext);

        template <class Fn, class GetPropertyIdFunc>
        BOOL HasPropertyTrap(Fn fn, GetPropertyIdFunc getPropertyId);

        template <class Fn, class GetPropertyIdFunc>
        BOOL GetPropertyTrap(Var instance, PropertyDescriptor* propertyDescriptor, Fn fn, GetPropertyIdFunc getPropertyId, ScriptContext* requestContext);

        BOOL GetPropertyDescriptorTrap(PropertyId propertyId, PropertyDescriptor* resultDescriptor, ScriptContext* requestContext);
    };

    template <> inline bool VarIsImpl<JavascriptProxy>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_Proxy;
    }
}
