//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    enum class EnumeratorFlags : byte
    {
        None                = 0x0,
        EnumNonEnumerable   = 0x1,
        EnumSymbols         = 0x2,
        SnapShotSemantics   = 0x4,
        UseCache            = 0x8,
        EphemeralReference  = 0x10
    };
    ENUM_CLASS_HELPERS(EnumeratorFlags, byte);

    class JavascriptStaticEnumerator 
    {
    protected:
        friend class ForInObjectEnumerator;
        Field(DynamicObjectPropertyEnumerator) propertyEnumerator;
        Field(JavascriptEnumerator*) currentEnumerator;
        Field(JavascriptEnumerator*) prefixEnumerator;
        Field(JavascriptEnumerator*) arrayEnumerator;

        JavascriptString * MoveAndGetNextFromEnumerator(PropertyId& propertyId, PropertyAttributes* attributes);
    public:
        JavascriptStaticEnumerator() { Clear(EnumeratorFlags::None, nullptr); }
        bool Initialize(JavascriptEnumerator * prefixEnumerator, ArrayObject * arrayToEnumerate, DynamicObject* objectToEnumerate, EnumeratorFlags flags, ScriptContext * requestContext, EnumeratorCache * enumeratorCache);
        bool IsNullEnumerator() const;
        bool CanUseJITFastPath() const;
        ScriptContext * GetScriptContext() const { return propertyEnumerator.GetScriptContext(); }
        EnumeratorFlags GetFlags() const { return propertyEnumerator.GetFlags(); }

        void Clear(EnumeratorFlags flags, ScriptContext * requestContext);
        void Reset();
        uint32_t GetCurrentItemIndex();
        JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes* attributes = nullptr);

        static uint32_t GetOffsetOfCurrentEnumerator() { return offsetof(JavascriptStaticEnumerator, currentEnumerator); }
        static uint32_t GetOffsetOfPrefixEnumerator() { return offsetof(JavascriptStaticEnumerator, prefixEnumerator); }
        static uint32_t GetOffsetOfArrayEnumerator() { return offsetof(JavascriptStaticEnumerator, arrayEnumerator); }
        static uint32_t GetOffsetOfScriptContext() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfScriptContext(); }
        static uint32_t GetOffsetOfInitialType() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfInitialType(); }
        static uint32_t GetOffsetOfObject() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfObject(); }
        static uint32_t GetOffsetOfObjectIndex() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfObjectIndex(); }
        static uint32_t GetOffsetOfInitialPropertyCount() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfInitialPropertyCount(); }
        static uint32_t GetOffsetOfEnumeratedCount() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfEnumeratedCount(); }
        static uint32_t GetOffsetOfCachedData() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfCachedData(); }
        static uint32_t GetOffsetOfFlags() { return offsetof(JavascriptStaticEnumerator, propertyEnumerator) + DynamicObjectPropertyEnumerator::GetOffsetOfFlags(); }


    };

    

} // namespace Js
