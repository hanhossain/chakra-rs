//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ForInObjectEnumerator
    {
    private:
        JavascriptStaticEnumerator enumerator;
        struct ShadowData
        {
            ShadowData(RecyclableObject * initObject, RecyclableObject * firstPrototype, Recycler * recycler);
            Field(RecyclableObject *) currentObject;
            Field(RecyclableObject *) firstPrototype;
            Field(BVSparse<Recycler>) propertyIds;
            typedef SListBase<Js::PropertyRecord const *, Recycler> _PropertyStringsListType;
            Field(_PropertyStringsListType) newPropertyStrings;
        } *shadowData;

        // States
        bool canUseJitFastPath; // used by GenerateFastBrBReturn and GenerateFastInlineHasOwnProperty
        bool enumeratingPrototype;

        BOOL TestAndSetEnumerated(PropertyId propertyId);
        BOOL InitializeCurrentEnumerator(RecyclableObject * object, EnumeratorCache * forInCache = nullptr);
        BOOL InitializeCurrentEnumerator(RecyclableObject * object, EnumeratorFlags flags, ScriptContext * requestContext, EnumeratorCache * forInCache);

    public:
        ForInObjectEnumerator(RecyclableObject* currentObject, ScriptContext * requestContext, bool enumSymbols = false);
        ~ForInObjectEnumerator() { Clear(); }

        ScriptContext * GetScriptContext() const { return enumerator.GetScriptContext(); }
        void Initialize(RecyclableObject* currentObject, ScriptContext * requestContext, bool enumSymbols = false, EnumeratorCache * forInCache = nullptr);
        void Clear();
        JavascriptString * MoveAndGetNext(PropertyId& propertyId);

        static RecyclableObject* GetFirstPrototypeWithEnumerableProperties(RecyclableObject* object, RecyclableObject** pFirstPrototype = nullptr);


        static uint32_t GetOffsetOfCanUseJitFastPath() { return offsetof(ForInObjectEnumerator, canUseJitFastPath); }
        static uint32_t GetOffsetOfEnumeratingPrototype() { return offsetof(ForInObjectEnumerator, enumeratingPrototype); }
        static uint32_t GetOffsetOfEnumeratorScriptContext() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfScriptContext(); }
        static uint32_t GetOffsetOfEnumeratorObject() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfObject(); }
        static uint32_t GetOffsetOfEnumeratorInitialType() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfInitialType(); }
        static uint32_t GetOffsetOfEnumeratorInitialPropertyCount() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfInitialPropertyCount(); }
        static uint32_t GetOffsetOfEnumeratorEnumeratedCount() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfEnumeratedCount(); }
        static uint32_t GetOffsetOfEnumeratorCachedData() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfCachedData(); }
        static uint32_t GetOffsetOfEnumeratorObjectIndex() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfObjectIndex(); }
        static uint32_t GetOffsetOfEnumeratorFlags() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfFlags(); }

        static uint32_t GetOffsetOfEnumeratorCurrentEnumerator() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfCurrentEnumerator(); }
        static uint32_t GetOffsetOfEnumeratorPrefixEnumerator() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfPrefixEnumerator(); }
        static uint32_t GetOffsetOfEnumeratorArrayEnumerator() { return offsetof(ForInObjectEnumerator, enumerator) + JavascriptStaticEnumerator::GetOffsetOfArrayEnumerator(); }

        static uint32_t GetOffsetOfShadowData() { return offsetof(ForInObjectEnumerator, shadowData); }
        static uint32_t GetOffsetOfStates()
        {
            CompileAssert(offsetof(ForInObjectEnumerator, enumeratingPrototype) == offsetof(ForInObjectEnumerator, canUseJitFastPath) + 1);
            return offsetof(ForInObjectEnumerator, canUseJitFastPath);
        }
    };
}
