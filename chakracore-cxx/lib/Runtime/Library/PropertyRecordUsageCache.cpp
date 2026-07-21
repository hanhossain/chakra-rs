//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/// Number of string or symbol cache hits per miss needed to be worth using cache
constexpr int PropertyCacheMissPenalty = 10;

/// Point at which we disable string or symbol property cache
constexpr int PropertyCacheMissThreshold = -100;

/// Point at which we try to start using string or symbol cache after giving up
constexpr int PropertyCacheMissReset = -5000;

namespace Js
{
    PropertyRecordUsageCache::PropertyRecordUsageCache() :
        propertyRecord(nullptr),
        ldElemInlineCache(nullptr),
        stElemInlineCache(nullptr),
        hitRate(0)
    {
        // Required due to DEFINE_VTABLE_CTOR on parent, but useless
    }

    PropertyRecordUsageCache::PropertyRecordUsageCache(StaticType* type, const PropertyRecord* propertyRecord) :
        propertyRecord(propertyRecord),
        ldElemInlineCache(nullptr),
        stElemInlineCache(nullptr),
        hitRate(0)
    {
        // TODO: in future, might be worth putting these inline to avoid extra allocations. PIC copy API needs to be updated to support this though
        this->ldElemInlineCache = ScriptContextPolymorphicInlineCache::New(MinPropertyStringInlineCacheSize, type->GetLibrary());
        this->stElemInlineCache = ScriptContextPolymorphicInlineCache::New(MinPropertyStringInlineCacheSize, type->GetLibrary());
    }

    PolymorphicInlineCache* PropertyRecordUsageCache::GetLdElemInlineCache() const
    {
        return this->ldElemInlineCache;
    }

    PolymorphicInlineCache* PropertyRecordUsageCache::GetStElemInlineCache() const
    {
        return this->stElemInlineCache;
    }

    bool PropertyRecordUsageCache::ShouldUseCache() const
    {
        return this->hitRate > PropertyCacheMissThreshold;
    }

    void PropertyRecordUsageCache::RegisterCacheMiss()
    {
        this->hitRate -= PropertyCacheMissPenalty;
        if (this->hitRate < PropertyCacheMissReset)
        {
            this->hitRate = 0;
        }
    }

    PolymorphicInlineCache * PropertyRecordUsageCache::CreateBiggerPolymorphicInlineCache(bool isLdElem)
    {
        PolymorphicInlineCache * polymorphicInlineCache = isLdElem ? GetLdElemInlineCache() : GetStElemInlineCache();
        ScriptContext * scriptContext = polymorphicInlineCache->GetScriptContext();
        Assert(polymorphicInlineCache && polymorphicInlineCache->CanAllocateBigger());
        uint16 polymorphicInlineCacheSize = polymorphicInlineCache->GetSize();
        uint16 newPolymorphicInlineCacheSize = PolymorphicInlineCache::GetNextSize(polymorphicInlineCacheSize);
        Assert(newPolymorphicInlineCacheSize > polymorphicInlineCacheSize);
        PolymorphicInlineCache * newPolymorphicInlineCache = ScriptContextPolymorphicInlineCache::New(newPolymorphicInlineCacheSize, scriptContext->GetLibrary());
        polymorphicInlineCache->CopyTo(this->propertyRecord->GetPropertyId(), scriptContext, newPolymorphicInlineCache);
        if (isLdElem)
        {
            this->ldElemInlineCache = newPolymorphicInlineCache;
        }
        else
        {
            this->stElemInlineCache = newPolymorphicInlineCache;
        }
        if (PHASE_VERBOSE_TRACE1(Js::PolymorphicInlineCachePhase) || PHASE_TRACE1(PropertyCachePhase))
        {
            Output::Print(u"PropertyRecordUsageCache '%s' : Bigger PIC, oldSize = %d, newSize = %d\n", GetString(), polymorphicInlineCacheSize, newPolymorphicInlineCacheSize);
        }
        return newPolymorphicInlineCache;
    }
}
