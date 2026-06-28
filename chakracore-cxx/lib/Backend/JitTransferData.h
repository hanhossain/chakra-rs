//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class EntryPointInfo;
    class JitEquivalentTypeGuard;
    struct CtorCacheGuardTransferEntry;
    struct TypeGuardTransferEntry;
};

struct TypeGuardTransferData
{
    typename WriteBarrierFieldTypeTraits<unsigned int>::Type propertyGuardCount;
    typename WriteBarrierFieldTypeTraits<TypeGuardTransferEntryIDL*, _no_write_barrier_policy, _no_write_barrier_policy>::Type entries;
};

struct CtorCacheTransferData
{
    typename WriteBarrierFieldTypeTraits<unsigned int>::Type ctorCachesCount;
    typename WriteBarrierFieldTypeTraits<CtorCacheTransferEntryIDL **, _no_write_barrier_policy, _no_write_barrier_policy>::Type entries;
};

class JitTransferData
{
    friend Js::EntryPointInfo;

private:
    typedef JsUtil::BaseHashSet<void*, Recycler, PowerOf2SizePolicy> TypeRefSet;
    typename WriteBarrierFieldTypeTraits<TypeRefSet*>::Type jitTimeTypeRefs;

    typename WriteBarrierFieldTypeTraits<PinnedTypeRefsIDL*>::Type runtimeTypeRefs;

    typename WriteBarrierFieldTypeTraits<int>::Type propertyGuardCount;
    // This is a dynamically sized array of dynamically sized TypeGuardTransferEntries.  It's heap allocated by the JIT
    // thread and lives until entry point is installed, at which point it is explicitly freed.
    typename WriteBarrierFieldTypeTraits<Js::TypeGuardTransferEntry*, _no_write_barrier_policy, _no_write_barrier_policy>::Type propertyGuardsByPropertyId;
    typename WriteBarrierFieldTypeTraits<size_t>::Type propertyGuardsByPropertyIdPlusSize;

    // This is a dynamically sized array of dynamically sized CtorCacheGuardTransferEntry.  It's heap allocated by the JIT
    // thread and lives until entry point is installed, at which point it is explicitly freed.
    typename WriteBarrierFieldTypeTraits<Js::CtorCacheGuardTransferEntry*, _no_write_barrier_policy, _no_write_barrier_policy>::Type ctorCacheGuardsByPropertyId;
    typename WriteBarrierFieldTypeTraits<size_t>::Type ctorCacheGuardsByPropertyIdPlusSize;

    typename WriteBarrierFieldTypeTraits<int>::Type equivalentTypeGuardCount;
    typename WriteBarrierFieldTypeTraits<int>::Type lazyBailoutPropertyCount;
    // This is a dynamically sized array of JitEquivalentTypeGuards. It's heap allocated by the JIT thread and lives
    // until entry point is installed, at which point it is explicitly freed. We need it during installation so as to
    // swap the cache associated with each guard from the heap to the recycler (so the types in the cache are kept alive).
    typename WriteBarrierFieldTypeTraits<Js::JitEquivalentTypeGuard**, _no_write_barrier_policy, _no_write_barrier_policy>::Type equivalentTypeGuards;
    typename WriteBarrierFieldTypeTraits<Js::PropertyId*, _no_write_barrier_policy, _no_write_barrier_policy>::Type lazyBailoutProperties;
    typename WriteBarrierFieldTypeTraits<NativeCodeData*, _no_write_barrier_policy, _no_write_barrier_policy>::Type jitTransferRawData;
    typename WriteBarrierFieldTypeTraits<EquivalentTypeGuardOffsets*, _no_write_barrier_policy, _no_write_barrier_policy>::Type equivalentTypeGuardOffsets;
    typename WriteBarrierFieldTypeTraits<TypeGuardTransferData>::Type typeGuardTransferData;
    typename WriteBarrierFieldTypeTraits<CtorCacheTransferData>::Type ctorCacheTransferData;

    typename WriteBarrierFieldTypeTraits<bool>::Type falseReferencePreventionBit;
    typename WriteBarrierFieldTypeTraits<bool>::Type isReady;

public:
    JitTransferData() :
        jitTimeTypeRefs(nullptr), runtimeTypeRefs(nullptr),
        propertyGuardCount(0), propertyGuardsByPropertyId(nullptr), propertyGuardsByPropertyIdPlusSize(0),
        ctorCacheGuardsByPropertyId(nullptr), ctorCacheGuardsByPropertyIdPlusSize(0),
        equivalentTypeGuardCount(0), equivalentTypeGuards(nullptr), jitTransferRawData(nullptr),
        falseReferencePreventionBit(true), isReady(false), lazyBailoutProperties(nullptr), lazyBailoutPropertyCount(0) {}

    void SetRawData(NativeCodeData* rawData) { jitTransferRawData = rawData; }

    void AddJitTimeTypeRef(void* typeRef, Recycler* recycler);

    int GetRuntimeTypeRefCount() { return this->runtimeTypeRefs ? this->runtimeTypeRefs->count : 0; }
    void** GetRuntimeTypeRefs() { return this->runtimeTypeRefs ? (void**)this->runtimeTypeRefs->typeRefs : nullptr; }
    void SetRuntimeTypeRefs(PinnedTypeRefsIDL* pinnedTypeRefs) { this->runtimeTypeRefs = pinnedTypeRefs; }

    Js::JitEquivalentTypeGuard** GetEquivalentTypeGuards() const { return this->equivalentTypeGuards; }
    void SetEquivalentTypeGuards(Js::JitEquivalentTypeGuard** guards, int count)
    {
        this->equivalentTypeGuardCount = count;
        this->equivalentTypeGuards = guards;
    }
    void SetLazyBailoutProperties(Js::PropertyId* properties, int count)
    {
        this->lazyBailoutProperties = properties;
        this->lazyBailoutPropertyCount = count;
    }
    void SetEquivalentTypeGuardOffsets(EquivalentTypeGuardOffsets* offsets)
    {
        equivalentTypeGuardOffsets = offsets;
    }
    void SetTypeGuardTransferData(JITOutputIDL* data)
    {
        typeGuardTransferData.entries = data->typeGuardEntries;
        typeGuardTransferData.propertyGuardCount = data->propertyGuardCount;
    }
    void SetCtorCacheTransferData(JITOutputIDL * data)
    {
        ctorCacheTransferData.entries = data->ctorCacheEntries;
        ctorCacheTransferData.ctorCachesCount = data->ctorCachesCount;
    }

    bool GetIsReady() { return this->isReady; }
    void SetIsReady() { this->isReady = true; }

    void RecordTypeGuards(int propertyGuardCount, Js::TypeGuardTransferEntry* typeGuardTransferRecord, size_t typeGuardTransferPlusSize);
    void RecordCtorCacheGuards(Js::CtorCacheGuardTransferEntry* ctorCacheTransferRecord, size_t ctorCacheTransferPlusSize);

    void Cleanup();
private:
    void EnsureJitTimeTypeRefs(Recycler* recycler);
};
