//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    //
    // This class is for Object.prototype. It performs special handling for __proto__ property.
    //
    class ObjectPrototypeObject : public DynamicObject
    {
    private:
        bool __proto__Enabled; // Now only used by diagnostics to decide display __proto__ or [prototype]

        DEFINE_VTABLE_CTOR(ObjectPrototypeObject, DynamicObject);
        friend class Js::CrossSiteObject<ObjectPrototypeObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ObjectPrototypeObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ObjectPrototypeObject>>::SetVirtualTable(this);
        };

    public:
        ObjectPrototypeObject(DynamicType* type);
        static ObjectPrototypeObject * New(Recycler * recycler, DynamicType * type);

        class EntryInfo
        {
        public:
            static FunctionInfo __proto__getter;
            static FunctionInfo __proto__setter;
        };

        static Var Entry__proto__getter(RecyclableObject* function, CallInfo callInfo, ...);
        static Var Entry__proto__setter(RecyclableObject* function, CallInfo callInfo, ...);

        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;

        // Indicates if __proto__ is enabled currently (note that it can be disabled and re-enabled),
        // only useful for diagnostics to decide displaying __proto__ or [prototype].
        bool is__proto__Enabled() const { return __proto__Enabled; }
        BOOL IsProtoImmutable() const override { return true; }

        void PostDefineOwnProperty__proto__(RecyclableObject* obj);
    };
}
