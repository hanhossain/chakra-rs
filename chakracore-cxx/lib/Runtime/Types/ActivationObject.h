//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    struct FuncCacheEntry
    {
        typename WriteBarrierFieldTypeTraits<ScriptFunction *>::Type func;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type type;
    };

    class ActivationObject : public DynamicObject
    {
    protected:
        DEFINE_VTABLE_CTOR(ActivationObject, DynamicObject);
        friend class Js::CrossSiteObject<ActivationObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ActivationObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ActivationObject>>::SetVirtualTable(this);
        };
    public:
        ActivationObject(DynamicType * type) : DynamicObject(type)
        {}

        BOOL HasOwnPropertyCheckNoRedecl(PropertyId propertyId) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetInternalProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL EnsureProperty(PropertyId propertyId) override;
        BOOL EnsureNoRedeclProperty(PropertyId propertyId) override;
        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
        BOOL InitFuncScoped(PropertyId propertyId, Var value) override;
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override;
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
    };

    template <> bool VarIsImpl<ActivationObject>(RecyclableObject* instance);

    // A block-ActivationObject is a scope for an ES6 block that should only receive block-scoped inits,
    // including function, let, and const.
    class BlockActivationObject : public ActivationObject
    {
    private:
        DEFINE_VTABLE_CTOR(BlockActivationObject, ActivationObject);
        friend class Js::CrossSiteObject<BlockActivationObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<BlockActivationObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<BlockActivationObject>>::SetVirtualTable(this);
        };
    public:
        BlockActivationObject(DynamicType * type) : ActivationObject(type) {}

        BOOL EnsureProperty(PropertyId propertyId) override;
        BOOL EnsureNoRedeclProperty(PropertyId propertyId) override;
        BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
        BOOL InitFuncScoped(PropertyId propertyId, Var value) override;

        BlockActivationObject* Clone(ScriptContext *scriptContext);
    };

    template <> bool VarIsImpl<BlockActivationObject>(RecyclableObject* instance);

    // A pseudo-ActivationObject is a scope like a "catch" scope that shouldn't receive var inits.
    class PseudoActivationObject : public ActivationObject
    {
    private:
        DEFINE_VTABLE_CTOR(PseudoActivationObject, ActivationObject);
        friend class Js::CrossSiteObject<PseudoActivationObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<PseudoActivationObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<PseudoActivationObject>>::SetVirtualTable(this);
        };
    public:
        PseudoActivationObject(DynamicType * type) : ActivationObject(type) {}

        BOOL EnsureProperty(PropertyId propertyId) override;
        BOOL EnsureNoRedeclProperty(PropertyId propertyId) override;
        BOOL InitFuncScoped(PropertyId propertyId, Var value) override;
        BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
    };

    template <> bool VarIsImpl<PseudoActivationObject>(RecyclableObject* instance);

    class ConsoleScopeActivationObject : public ActivationObject
    {
    private:
        DEFINE_VTABLE_CTOR(ConsoleScopeActivationObject, ActivationObject);
        friend class Js::CrossSiteObject<ConsoleScopeActivationObject>;
        virtual void MarshalToScriptContext(Js::ScriptContext *scriptContext)
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ConsoleScopeActivationObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ConsoleScopeActivationObject>>::SetVirtualTable(this);
        };
    public:
        ConsoleScopeActivationObject(DynamicType * type) : ActivationObject(type) {}

        // A dummy function to have a different vtable
        virtual void DummyVirtualFunc(void)
        {
            AssertMsg(false, "ConsoleScopeActivationObject::DummyVirtualFunc function should never be called");
        }
    };

    template <> bool VarIsImpl<ConsoleScopeActivationObject>(RecyclableObject* instance);

    class ActivationObjectEx : public ActivationObject
    {
    private:
        DEFINE_VTABLE_CTOR(ActivationObjectEx, ActivationObject);
        friend class Js::CrossSiteObject<ActivationObjectEx>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ActivationObjectEx>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ActivationObjectEx>>::SetVirtualTable(this);
        };

        void GetPropertyCore(PropertyValueInfo *info, ScriptContext *requestContext);
    public:
        ActivationObjectEx(
            DynamicType * type, ScriptFunction *func, uint cachedFuncCount, uint firstFuncSlot, uint lastFuncSlot)
            : ActivationObject(type),
              parentFunc(func),
              cachedFuncCount(cachedFuncCount),
              firstFuncSlot(firstFuncSlot),
              lastFuncSlot(lastFuncSlot),
              committed(false)
        {
            if (cachedFuncCount != 0)
            {
                cache[0].func = nullptr;
            }
        }

        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var *value, PropertyValueInfo *info, ScriptContext *requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var *value, PropertyValueInfo *info, ScriptContext *requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var *value, PropertyValueInfo *info, ScriptContext *requestContext) override;
        void InvalidateCachedScope() override;

        bool IsCommitted() const { return committed; }
        void SetCommit(bool set) { committed = set; }
        ScriptFunction *GetParentFunc() const { return parentFunc; }
        uint GetFirstFuncSlot() const { return firstFuncSlot; }
        uint GetLastFuncSlot() const { return lastFuncSlot; }
        bool HasCachedFuncs() const { return cachedFuncCount != 0 && cache[0].func != nullptr; }

        void SetCachedFunc(uint i, ScriptFunction *func);

        FuncCacheEntry *GetFuncCacheEntry(uint i)
        {
            Assert(i < cachedFuncCount);
            return &cache[i];
        }

        static uint32_t GetOffsetOfCache() { return offsetof(ActivationObjectEx, cache); }
        static uint32_t GetOffsetOfCommitFlag() { return offsetof(ActivationObjectEx, committed); }
        static uint32_t GetOffsetOfParentFunc() { return offsetof(ActivationObjectEx, parentFunc); }

        static const PropertyId *GetCachedScopeInfo(const PropertyIdArray *propIds);

        // Cached scope info:
        // [0] - cached func count
        // [1] - first func slot
        // [2] - first var slot
        // [3] - literal object reference

        static PropertyId GetCachedFuncCount(const PropertyIdArray *propIds)
        {
            AssertOrFailFast(propIds->extraSlots > 0);
            return ActivationObjectEx::GetCachedScopeInfo(propIds)[0];
        }

        static PropertyId GetFirstFuncSlot(const PropertyIdArray *propIds)
        {
            AssertOrFailFast(propIds->extraSlots > 1);
            return ActivationObjectEx::GetCachedScopeInfo(propIds)[1];
        }

        static PropertyId GetFirstVarSlot(const PropertyIdArray *propIds)
        {
            AssertOrFailFast(propIds->extraSlots > 2);
            return ActivationObjectEx::GetCachedScopeInfo(propIds)[2];
        }

        static PropertyId GetLiteralObjectRef(const PropertyIdArray *propIds)
        {
            AssertOrFailFast(propIds->extraSlots > 3);
            return ActivationObjectEx::GetCachedScopeInfo(propIds)[3];
        }

        static byte ExtraSlotCount() { return 4; }

    private:
        typename WriteBarrierFieldTypeTraits<ScriptFunction *>::Type parentFunc;
        typename WriteBarrierFieldTypeTraits<uint>::Type cachedFuncCount;
        typename WriteBarrierFieldTypeTraits<uint>::Type firstFuncSlot;
        typename WriteBarrierFieldTypeTraits<uint>::Type lastFuncSlot;
        typename WriteBarrierFieldTypeTraits<bool>::Type committed;
        typename WriteBarrierFieldTypeTraits<FuncCacheEntry>::Type cache[1];
    };

    template <> bool VarIsImpl<ActivationObjectEx>(RecyclableObject* instance);
};
