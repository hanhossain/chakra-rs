//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

namespace Js
{
    DEFINE_RECYCLER_TRACKER_PERF_COUNTER(PropertyRecord);
    DEFINE_RECYCLER_TRACKER_WEAKREF_PERF_COUNTER(PropertyRecord);

    // Constructor for runtime-constructed PropertyRecords
    PropertyRecord::PropertyRecord(uint32_t byteCount, bool isNumeric, uint hash, bool isSymbol)
        : pid(Js::Constants::NoProperty), hash(hash), isNumeric(isNumeric), byteCount(byteCount), isBound(false), isSymbol(isSymbol)
    {
    }

    // Constructor for built-in PropertyRecords
    PropertyRecord::PropertyRecord(PropertyId pid, uint hash, bool isNumeric, uint32_t byteCount, bool isSymbol)
        : pid(pid), hash(hash), isNumeric(isNumeric), byteCount(byteCount), isBound(true), isSymbol(isSymbol)
    {
    }

    PropertyRecord::PropertyRecord(const char16_t* buffer, const int length, uint32_t bytelength, bool isSymbol)
        : pid(Js::Constants::NoProperty), isSymbol(isSymbol), byteCount(bytelength)
    {
        Assert(length >= 0 && buffer != nullptr);

        char16_t* target = (char16_t*)((PropertyRecord*)this + 1);
        isNumeric = (isSymbol || length > 10 || length <= 0) ? false : true;
        hash = CC_HASH_OFFSET_VALUE;

        for (int i = 0; i < length; i++)
        {
            const char16_t byte = buffer[i];
            if (isNumeric)
            {
                if (byte < u'0' || byte > u'9')
                  isNumeric = false;
            }

            CC_HASH_LOGIC(hash, byte);
            target[i] = byte;
        }
        target[length] = char16_t(0);

        if (isNumeric)
        {
            uint32 numericValue;
            isNumeric = Js::PropertyRecord::IsPropertyNameNumeric(this->GetBuffer(), this->GetLength(), &numericValue);
            if (isNumeric)
            {
                *(uint32 *)(this->GetBuffer() + this->GetLength() + 1) = numericValue;
                Assert(GetNumericValue() == numericValue);
            }
        }
    }

    void PropertyRecord::Finalize(bool isShutdown)
    {
        if (!isShutdown)
        {
            ThreadContext * tc = ThreadContext::GetContextForCurrentThread();
            Assert(tc);
            Assert(tc->IsActivePropertyId(this->GetPropertyId()));

            tc->InvalidatePropertyRecord(this);
        }
    }

    bool PropertyRecord::ShouldDisableWriteCache() const
    {
        // We can't cache stores to the 'prototype' property of function objects. We must go through the runtime and clear the constructor cache.
        // We could consider treating 'prototype' as an accessor on JavascriptFunction and friends, though this seems like it will grow the object.
        return pid == PropertyIds::prototype;
    }

#ifdef DEBUG
    // This is only used to assert that integer property names are not passed into
    // the GetSetter, GetProperty, SetProperty etc methods that take JavascriptString
    // instead of PropertyId.  It is expected that integer property names will go
    // through a fast path before reaching those APIs.
    bool PropertyRecord::IsPropertyNameNumeric(const char16_t* str, int length)
    {
        uint32 unused;
        return IsPropertyNameNumeric(str, length, &unused);
    }
#endif

    bool PropertyRecord::IsPropertyNameNumeric(const char16_t* str, int length, uint32* intVal)
    {
        return (Js::JavascriptOperators::TryConvertToUInt32(str, length, intVal) &&
            (*intVal != Js::JavascriptArray::InvalidIndex));
    }

    uint32 PropertyRecord::GetNumericValue() const
    {
        Assert(IsNumeric());
        return *(uint32 *)(this->GetBuffer() + this->GetLength() + 1);
    }

    // Initialize all Internal property records
#define INTERNALPROPERTY(name) \
    const BuiltInPropertyRecord<1> InternalPropertyRecords::name = { PropertyRecord((PropertyId)InternalPropertyIds::name, (uint)InternalPropertyIds::name, false, 0, false), u"" };
#include "Library/InternalPropertyList.h"

    const PropertyRecord* InternalPropertyRecords::GetInternalPropertyName(PropertyId propertyId)
    {
        Assert(IsInternalPropertyId(propertyId));

        switch (propertyId)
        {
#define INTERNALPROPERTY(name) \
            case InternalPropertyIds::name: \
                return InternalPropertyRecords::name;
#include "Library/InternalPropertyList.h"
        }

        Throw::FatalInternalError();
    }


    PropertyAttributes PropertyRecord::DefaultAttributesForPropertyId(PropertyId propertyId, bool __proto__AsDeleted)
    {
        switch (propertyId)
        {
        case PropertyIds::__proto__:
            if (__proto__AsDeleted)
            {
                //
                // If the property name is __proto__, it could be either [[prototype]] or ignored, or become a local
                // property depending on later environment and property value. To maintain enumeration order when it
                // becomes a local property, add the entry as deleted.
                //
                return PropertyDeletedDefaults;
            }
            return PropertyDynamicTypeDefaults;

        default:
            return PropertyDynamicTypeDefaults;
        }
    }

#if DBG_DUMP
    void PropertyRecord::Dump(unsigned indent) const
    {
        const auto padding(u"");
        const unsigned fieldIndent(indent + 2);

        Output::Print(u"%*sPropertyRecord (0x%p):\n", indent, padding, this);
        Output::Print(u"%*spid: %d\n", fieldIndent, padding, this->pid);
        Output::Print(u"%*shash: 0x%08x\n", fieldIndent, padding, this->hash);
        Output::Print(u"%*sisNumeric: %d\n", fieldIndent, padding, this->isNumeric);
        Output::Print(u"%*sIsBound: %d\n", fieldIndent, padding, this->isBound);
        Output::Print(u"%*sIsSymbol: %d\n", fieldIndent, padding, this->isSymbol);
        Output::Print(u"%*sbyteCount: %u\n", fieldIndent, padding, this->byteCount);
        if (this->isNumeric)
        {
            Output::Print(u"%*sbuffer (numeric): %u\n", fieldIndent, padding, this->GetNumericValue());
        }
        else
        {
            Output::Print(u"%*sbuffer (string): %s\n", fieldIndent, padding, this->GetBuffer());
        }
    }
#endif

    // Initialize all BuiltIn property records
    const BuiltInPropertyRecord<1> BuiltInPropertyRecords::EMPTY = { PropertyRecord(PropertyIds::_none, 0, false, 0, false), u"" };
#define ENTRY_INTERNAL_SYMBOL(n) const BuiltInPropertyRecord<ARRAYSIZE(u"<" _u(#n) u">")> BuiltInPropertyRecords::n = { PropertyRecord(PropertyIds::n, (uint)PropertyIds::n, false, (ARRAYSIZE(u"<" _u(#n) u">") - 1) * sizeof(char16_t), true), u"<" _u(#n) u">" };
#define ENTRY_SYMBOL(n, d) const BuiltInPropertyRecord<ARRAYSIZE(d)> BuiltInPropertyRecords::n = { PropertyRecord(PropertyIds::n, 0, false, (ARRAYSIZE(d) - 1) * sizeof(char16_t), true), d };
#define ENTRY2(n, s) const BuiltInPropertyRecord<ARRAYSIZE(s)> BuiltInPropertyRecords::n = { PropertyRecord(PropertyIds::n, 0, false, (ARRAYSIZE(s) - 1) * sizeof(char16_t), false), s };
#define ENTRY(n) ENTRY2(n, _u(#n))
#include "Base/JnDirectFields.h"
};

namespace JsUtil
{
    bool NoCaseComparer<Js::CaseInvariantPropertyListWithHashCode*>::Equals(_In_ Js::CaseInvariantPropertyListWithHashCode* list1, JsUtil::CharacterBuffer<char16_t> const& str)
    {
        Assert(list1 != nullptr);

        const RecyclerWeakReference<Js::PropertyRecord const>* propRecordWeakRef = list1->CompactEnd<true>();

        // If the lists are empty post-compaction, thats fine, we'll just remove them later
        if (propRecordWeakRef != nullptr)
        {
            const Js::PropertyRecord* prop = propRecordWeakRef->Get();

            // Since compaction returned this pointer, their strong refs should not be null
            Assert(prop);

            JsUtil::CharacterBuffer<char16_t> string(prop->GetBuffer(), prop->GetLength());

            return NoCaseComparer<JsUtil::CharacterBuffer<char16_t> >::Equals(string, str);
        }

        // If either of the property strings contains no entries, the two lists are not equivalent
        return false;
    }

    bool NoCaseComparer<Js::CaseInvariantPropertyListWithHashCode*>::Equals(_In_ Js::CaseInvariantPropertyListWithHashCode* list1, _In_ Js::CaseInvariantPropertyListWithHashCode* list2)
    {
        Assert(list1 != nullptr && list2 != nullptr);

        // If the two lists are the same, they're equal
        if (list1 == list2)
        {
            return true;
        }

        // If they don't have the same case invariant hash code, they're not equal
        if (list1->caseInvariantHashCode != list2->caseInvariantHashCode)
        {
            return false;
        }

        // Find a string from list 2
        // If it's the same when compared with a string from list 1 in a case insensitive way, they're equal
        const RecyclerWeakReference<Js::PropertyRecord const>* propRecordWeakRef = list2->CompactEnd<true>();

        if (propRecordWeakRef != nullptr)
        {
            const Js::PropertyRecord* prop = propRecordWeakRef->Get();

            // Since compaction returned this pointer, their strong refs should not be null
            Assert(prop);

            JsUtil::CharacterBuffer<char16_t> string(prop->GetBuffer(), prop->GetLength());

            return NoCaseComparer<Js::CaseInvariantPropertyListWithHashCode*>::Equals(list1, string);
        }

        return false;
    }

    uint NoCaseComparer<Js::CaseInvariantPropertyListWithHashCode*>::GetHashCode(_In_ Js::CaseInvariantPropertyListWithHashCode* list)
    {
        Assert(list != nullptr);

        if (list->caseInvariantHashCode == 0)
        {
            const RecyclerWeakReference<Js::PropertyRecord const>* propRecordWeakRef = list->CompactEnd<true>();

            if (propRecordWeakRef != nullptr)
            {
                const Js::PropertyRecord* prop = propRecordWeakRef->Get();

                Assert(prop);

                JsUtil::CharacterBuffer<char16_t> string(prop->GetBuffer(), prop->GetLength());

                list->caseInvariantHashCode = NoCaseComparer<JsUtil::CharacterBuffer<char16_t> >::GetHashCode(string);
            }
        }

        return list->caseInvariantHashCode;
    }
}
