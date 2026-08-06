//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once


namespace Js
{
    class JavascriptRegExpConstructor : public RuntimeFunction
    {
        friend class RegexHelper;
    private:
        static PropertyId const specialPropertyIds[];
        static PropertyId const specialnonEnumPropertyIds[];
        static PropertyId const specialEnumPropertyIds[];
        static const int NumCtorCaptures = 10;

        friend class Js::CrossSiteObject<JavascriptRegExpConstructor>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptRegExpConstructor>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptRegExpConstructor>>::SetVirtualTable(this);
        };
    protected:
        //To prevent lastMatch from being cleared from cross-site marshalling
        DEFINE_VTABLE_CTOR_MEMBER_INIT(JavascriptRegExpConstructor, RuntimeFunction, lastMatch);

    public:
        JavascriptRegExpConstructor(DynamicType* type, ConstructorCache* cache);

        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override;
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL IsEnumerable(PropertyId propertyId) override;
        BOOL IsConfigurable(PropertyId propertyId) override;
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        BOOL GetSpecialNonEnumerablePropertyName(uint32_t index, Var *propertyName, ScriptContext * requestContext);
        uint GetSpecialNonEnumerablePropertyCount() const;
        PropertyId const * GetSpecialNonEnumerablePropertyIds() const;
        BOOL GetSpecialEnumerablePropertyName(uint32_t index, JavascriptString ** propertyName, ScriptContext * requestContext);
        uint GetSpecialEnumerablePropertyCount() const;
        PropertyId const * GetSpecialEnumerablePropertyIds() const;
        BOOL GetSpecialPropertyName(uint32_t index, JavascriptString ** propertyName, ScriptContext * requestContext) override;
        uint GetSpecialPropertyCount() const override;
        PropertyId const * GetSpecialPropertyIds() const override;
        UnifiedRegex::RegexPattern* GetLastPattern() const { return lastPattern; }

    private:
        bool GetPropertyBuiltIns(PropertyId propertyId, Var* value, BOOL* result);
        bool SetPropertyBuiltIns(PropertyId propertyId, Var value, BOOL* result);
        void SetLastMatch(UnifiedRegex::RegexPattern* lastPattern, JavascriptString* lastInput, UnifiedRegex::GroupInfo lastMatch);
        void InvalidateLastMatch(UnifiedRegex::RegexPattern* lastPattern, JavascriptString* lastInput);

        void EnsureValues();

        typename WriteBarrierFieldTypeTraits<UnifiedRegex::RegexPattern*>::Type lastPattern;
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type lastInput;
        typename WriteBarrierFieldTypeTraits<UnifiedRegex::GroupInfo>::Type lastMatch;
        typename WriteBarrierFieldTypeTraits<bool>::Type invalidatedLastMatch; // true if last match must be recalculated before use
        typename WriteBarrierFieldTypeTraits<bool>::Type reset; // true if following fields must be recalculated from above before first use
        typename WriteBarrierFieldTypeTraits<Var>::Type lastParen;
        typename WriteBarrierFieldTypeTraits<Var>::Type lastIndex;
        typename WriteBarrierFieldTypeTraits<Var>::Type index;
        typename WriteBarrierFieldTypeTraits<Var>::Type leftContext;
        typename WriteBarrierFieldTypeTraits<Var>::Type rightContext;
        typename WriteBarrierFieldTypeTraits<Var>::Type captures[NumCtorCaptures];
    };

    class JavascriptRegExpConstructorProperties
    {
    public:
        static bool IsSpecialProperty(PropertyId id)
        {
            switch (id)
            {
                case PropertyIds::input:
                case PropertyIds::$_:

                case PropertyIds::lastMatch:
                case PropertyIds::$Ampersand:

                case PropertyIds::lastParen:
                case PropertyIds::$Plus:

                case PropertyIds::leftContext:
                case PropertyIds::$BackTick:

                case PropertyIds::rightContext:
                case PropertyIds::$Tick:

                case PropertyIds::$1:
                case PropertyIds::$2:
                case PropertyIds::$3:
                case PropertyIds::$4:
                case PropertyIds::$5:
                case PropertyIds::$6:
                case PropertyIds::$7:
                case PropertyIds::$8:
                case PropertyIds::$9:
                case PropertyIds::index:
                    return true;
            }
            return false;
        }
    };

} // namespace Js
