//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{

    class GlobalObject : public RootObjectBase
    {
    public:
        typename WriteBarrierFieldTypeTraits<JavascriptLibrary*>::Type library;
    private:
        DEFINE_VTABLE_CTOR(GlobalObject, RootObjectBase);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(GlobalObject);
        GlobalObject(DynamicType * type, ScriptContext* scriptContext);

        static int const InitialCapacity = 52;
        static int const InlineSlotCapacity = 52;
    public:
        static GlobalObject * New(ScriptContext * scriptContext);

        JavascriptLibrary* GetLibrary() const {return library; }

        void Initialize(ScriptContext * scriptContext);

        int32_t SetDirectHostObject(RecyclableObject* hostObject, RecyclableObject* secureHostObject);
        RecyclableObject* GetDirectHostObject();
        RecyclableObject* GetSecureDirectHostObject();
        Var ToThis();

        BOOL ReserveGlobalProperty(PropertyId propertyId);
        BOOL IsReservedGlobalProperty(PropertyId propertyId);

        Var ExecuteEvalParsedFunction(ScriptFunction *pfuncScript, FrameDisplay* environment, Var &varThis, ScriptContext *scriptContext);

        class EntryInfo
        {
        public:
            static FunctionInfo Eval;
            static FunctionInfo ParseInt;
            static FunctionInfo ParseFloat;
            static FunctionInfo IsNaN;
            static FunctionInfo IsFinite;
            static FunctionInfo DecodeURI;
            static FunctionInfo DecodeURIComponent;
            static FunctionInfo EncodeURI;
            static FunctionInfo EncodeURIComponent;
            static FunctionInfo Escape;
            static FunctionInfo UnEscape;
            static FunctionInfo CollectGarbage;

            static FunctionInfo ChWriteTraceEvent;
        };

        static Var EntryEval(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEvalRestrictedMode(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryParseInt(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryParseFloat(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryIsNaN(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryIsFinite(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryDecodeURI(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryDecodeURIComponent(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEncodeURI(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEncodeURIComponent(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEscape(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryUnEscape(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryCollectGarbage(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryChWriteTraceEvent(RecyclableObject* function, CallInfo callInfo, ...);

        static void ValidateSyntax(ScriptContext* scriptContext, const char16_t *source, int sourceLength, bool isGenerator, bool isAsync, void (Parser::*validateSyntax)());
        static void UpdateThisForEval(Var &varThis, ModuleID moduleID, ScriptContext *scriptContext) ;
        static ScriptFunction* DefaultEvalHelper(ScriptContext* scriptContext, const char16_t *source, int sourceLength, ModuleID moduleID, uint32_t grfscr, LPCOLESTR pszTitle, BOOL registerDocument, BOOL isIndirect, BOOL strictMode);
        typedef ScriptFunction* (*EvalHelperType)(ScriptContext* scriptContext, const char16_t *source, int sourceLength, ModuleID moduleID, uint32_t grfscr, LPCOLESTR pszTitle, BOOL registerDocument, BOOL isIndirect, BOOL strictMode);
        typename WriteBarrierFieldTypeTraits<EvalHelperType, _no_write_barrier_policy, _no_write_barrier_policy>::Type EvalHelper;

        static Var EntryEvalHelper(ScriptContext* scriptContext, RecyclableObject* function, Arguments& args);
        static Var VEval(JavascriptLibrary* library, FrameDisplay* environment, ModuleID moduleID, bool isStrictMode, bool isIndirect,
            Arguments& args, bool isLibraryCode, bool registerDocument, uint32_t additionalGrfscr, ScriptContext* debugEvalScriptContext = nullptr);

        virtual PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        virtual BOOL HasOwnProperty(PropertyId propertyId) override;
        virtual BOOL HasOwnPropertyNoHostObject(PropertyId propertyId) override;
        virtual BOOL UseDynamicObjectForNoHostObjectAccess() override { return TRUE; }
        virtual PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        virtual BOOL InitPropertyInEval(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        virtual BOOL InitPropertyScoped(PropertyId propertyId, Var value) override;
        virtual BOOL InitFuncScoped(PropertyId propertyId, Var value) override;
        virtual BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        virtual BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        virtual BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        virtual BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
        virtual PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual PropertyQueryFlags HasItemQuery(uint32_t index) override;
        virtual BOOL HasOwnItem(uint32_t index) override;
        virtual PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        virtual PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        virtual DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override;
        _Check_return_ _Success_(return) virtual BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext* requestContext) override;
        virtual BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        virtual BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override;
        virtual BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags) override;
        virtual DescriptorFlags GetSetter(PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;

        virtual BOOL Equals(Var other, BOOL* value, ScriptContext * requestContext) override;
        virtual BOOL StrictEquals(Var other, BOOL* value, ScriptContext * requestContext) override;
        virtual BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        virtual BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;

        virtual BOOL HasRootProperty(PropertyId propertyId) override;
        virtual BOOL GetRootProperty(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL GetRootPropertyReference(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL SetRootProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        virtual DescriptorFlags GetRootSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual BOOL DeleteRootProperty(PropertyId propertyId, PropertyOperationFlags flags) override;

        BOOL SetExistingProperty(PropertyId propertyId, Var value, PropertyValueInfo* info, BOOL *setAttempted);
        BOOL SetExistingRootProperty(PropertyId propertyId, Var value, PropertyValueInfo* info, BOOL *setAttempted);
    private:
        static BOOL MatchPatternHelper(JavascriptString *propertyName, JavascriptString *pattern, ScriptContext *scriptContext);

    private:
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type directHostObject;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type secureDirectHostObject;

        typedef JsUtil::BaseHashSet<PropertyId, Recycler, PowerOf2SizePolicy> ReservedPropertiesHashSet;
        typename WriteBarrierFieldTypeTraits<ReservedPropertiesHashSet *>::Type reservedProperties;
    };

    template <> inline bool VarIsImpl<GlobalObject>(RecyclableObject* obj)
    {
        return obj->GetTypeId() == TypeIds_GlobalObject;
    }
}
