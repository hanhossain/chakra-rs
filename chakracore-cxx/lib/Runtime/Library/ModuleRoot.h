//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    // A wrapper corresponds to a named item coming from the host.
    // it maintains the IDispatch* pointer of the named item.
    // this is used in setting up the scope for scoped operations. see javascriptoperators.cpp
    class ModuleRoot : public RootObjectBase
    {
    protected:
        DEFINE_VTABLE_CTOR(ModuleRoot, RootObjectBase);
        friend class Js::CrossSiteObject<ModuleRoot>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ModuleRoot>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ModuleRoot>>::SetVirtualTable(this);
        };

    public:
        ModuleRoot(DynamicType * type);
        void SetHostObject(ModuleID moduleID, HostObjectBase * hostObject);

        BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
        BOOL InitFuncScoped(PropertyId propertyId, Var value) override;
        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        BOOL HasOwnProperty(PropertyId propertyId) override;
        BOOL UseDynamicObjectForNoHostObjectAccess() override { return TRUE; }
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext* requestContext) override;
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags HasItemQuery(uint32_t index) override;
        BOOL HasOwnItem(uint32_t index) override;
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;

        BOOL EnsureProperty(PropertyId propertyId) override;

        BOOL HasRootProperty(PropertyId propertyId) override;
        BOOL GetRootProperty(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL GetRootPropertyReference(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetRootProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL DeleteRootProperty(PropertyId propertyId, PropertyOperationFlags flags) override;

        ModuleID GetModuleID() { return moduleID;}

    protected:
        // For module binder, there is only one IDispatch* associated with the name provided
        // by the host when we can IActiveScriptSite::GetItemInfo.
        ModuleID moduleID;
    };
}
