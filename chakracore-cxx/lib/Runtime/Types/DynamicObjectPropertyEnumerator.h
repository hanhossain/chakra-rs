//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class DynamicObjectPropertyEnumerator
    {
    private:
        Field(ScriptContext *) scriptContext;
        Field(DynamicObject *) object;
        Field(DynamicType *) initialType;              // for snapshot enumeration
        Field(BigPropertyIndex) objectIndex;
        Field(BigPropertyIndex) initialPropertyCount;
        Field(int) enumeratedCount;

        Field(EnumeratorFlags) flags;

        struct CachedData
        {
            Field(ScriptContext *) scriptContext;
            Field(Field(PropertyString*)*) strings;
            Field(BigPropertyIndex *) indexes;
            Field(PropertyAttributes *) attributes;
            Field(int) cachedCount;
            Field(int) propertyCount;
            Field(bool) completed;
            Field(bool) enumNonEnumerable;
            Field(bool) enumSymbols;
        };
        Field(CachedData *) cachedData;

        DynamicType * GetTypeToEnumerate() const;
        JavascriptString * MoveAndGetNextWithCache(PropertyId& propertyId, PropertyAttributes* attributes);
        JavascriptString * MoveAndGetNextNoCache(PropertyId& propertyId, PropertyAttributes * attributes);

        void Initialize(DynamicType * type, CachedData * data, Js::BigPropertyIndex initialPropertyCount);
        BigPropertyIndex PropertyIndexToPropertyEnumeration(BigPropertyIndex index) const { return object->GetTypeHandler()->PropertyIndexToPropertyEnumeration(index); }
    public:
        DynamicObject * GetObject() const { return object; }
        EnumeratorFlags GetFlags() const { return flags; }
        bool GetEnumNonEnumerable() const;
        bool GetEnumSymbols() const;
        bool GetSnapShotSemantics() const;
        bool GetUseCache() const;
        ScriptContext * GetScriptContext() const { return scriptContext; }

        bool Initialize(DynamicObject * object, EnumeratorFlags flags, ScriptContext * requestContext, EnumeratorCache * enumeratorCache);
        bool IsNullEnumerator() const;
        void Reset();
        void Clear(EnumeratorFlags flags, ScriptContext * requestContext);
        JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes * attributes);

        bool CanUseJITFastPath() const;
        static uint32_t GetOffsetOfScriptContext() { return offsetof(DynamicObjectPropertyEnumerator, scriptContext); }
        static uint32_t GetOffsetOfInitialType() { return offsetof(DynamicObjectPropertyEnumerator, initialType); }
        static uint32_t GetOffsetOfObject() { return offsetof(DynamicObjectPropertyEnumerator, object); }
        static uint32_t GetOffsetOfObjectIndex() { return offsetof(DynamicObjectPropertyEnumerator, objectIndex); }
        static uint32_t GetOffsetOfInitialPropertyCount() { return offsetof(DynamicObjectPropertyEnumerator, initialPropertyCount); }
        static uint32_t GetOffsetOfEnumeratedCount() { return offsetof(DynamicObjectPropertyEnumerator, enumeratedCount); }
        static uint32_t GetOffsetOfCachedData() { return offsetof(DynamicObjectPropertyEnumerator, cachedData); }
        static uint32_t GetOffsetOfFlags() { return offsetof(DynamicObjectPropertyEnumerator, flags);
        }
        static uint32_t GetOffsetOfCachedDataStrings() { return offsetof(CachedData, strings); }
        static uint32_t GetOffsetOfCachedDataIndexes() { return offsetof(CachedData, indexes); }
        static uint32_t GetOffsetOfCachedDataPropertyCount() { return offsetof(CachedData, propertyCount); }
        static uint32_t GetOffsetOfCachedDataCachedCount() { return offsetof(CachedData, cachedCount); }
        static uint32_t GetOffsetOfCachedDataPropertyAttributes() { return offsetof(CachedData, attributes); }
        static uint32_t GetOffsetOfCachedDataCompleted() { return offsetof(CachedData, completed); }
        static uint32_t GetOffsetOfCachedDataEnumNonEnumerable() { return offsetof(CachedData, enumNonEnumerable); }
    };
};
