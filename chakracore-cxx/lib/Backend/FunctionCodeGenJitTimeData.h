//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if ENABLE_NATIVE_CODEGEN
// forward ref
struct FunctionBodyDataIDL;

namespace Js
{
    // - Data generated for jitting purposes
    // - Recycler-allocated, lifetime is from when a code gen work item is added to the jit queue, to when jitting is complete
    //     - Also keeps the function body and inlinee function bodies alive while jitting.
    class FunctionCodeGenJitTimeData
    {
    private:
        typename WriteBarrierFieldTypeTraits<FunctionInfo *>::Type const functionInfo;

        // Point's to an entry point if the work item needs the entry point alive- null for cases where the entry point isn't used
        typename WriteBarrierFieldTypeTraits<EntryPointInfo *>::Type const entryPointInfo;

        // These cloned inline caches are guaranteed to have stable data while jitting, but will be collectible after jitting
        typename WriteBarrierFieldTypeTraits<ObjTypeSpecFldInfoArray>::Type objTypeSpecFldInfoArray;

        // Globally ordered list of all object type specialized property access information (monomorphic and polymorphic caches combined).
        typename WriteBarrierFieldTypeTraits<uint>::Type globalObjTypeSpecFldInfoCount;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<ObjTypeSpecFldInfo*>::Type*>::Type globalObjTypeSpecFldInfoArray;

        // There will be a non-null entry for each profiled call site where a function is to be inlined
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData*>::Type*>::Type inlinees;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData*>::Type*>::Type ldFldInlinees;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData*>::Type*>::Type callbackInlinees;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData*>::Type*>::Type callApplyTargetInlinees;
        
        typename WriteBarrierFieldTypeTraits<RecyclerWeakReference<FunctionBody>*>::Type weakFuncRef;

        typename WriteBarrierFieldTypeTraits<PolymorphicInlineCacheInfoIDL*>::Type inlineeInfo;
        typename WriteBarrierFieldTypeTraits<PolymorphicInlineCacheInfoIDL*>::Type selfInfo;
        typename WriteBarrierFieldTypeTraits<PolymorphicInlineCacheIDL*>::Type polymorphicInlineCaches;

        // current value of global this object, may be changed in case of script engine invalidation
        typename WriteBarrierFieldTypeTraits<Var>::Type globalThisObject;

        // Number of functions that are to be inlined (this is not the length of the 'inlinees' array above, includes getter setter inlinee count)
        typename WriteBarrierFieldTypeTraits<uint>::Type inlineeCount;
        // Number of counts of getter setter to be inlined. This is not an exact count as inline caches are shared and we have no way of knowing
        // accurate count.
        typename WriteBarrierFieldTypeTraits<uint>::Type ldFldInlineeCount;

        // For polymorphic call site we will have linked list of FunctionCodeGenJitTimeData
        // Each is differentiated by id starting from 0, 1
        typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData *>::Type next;
        typename WriteBarrierFieldTypeTraits<bool>::Type isInlined;

        // This indicates the function is aggressively Inlined(see NativeCodeGenerator::TryAggressiveInlining) .
        typename WriteBarrierFieldTypeTraits<bool>::Type isAggressiveInliningEnabled;

        // The profiled iterations need to be determined at the time of gathering code gen data on the main thread
        typename WriteBarrierFieldTypeTraits<const uint16>::Type profiledIterations;

        FunctionCodeGenJitTimeData(FunctionInfo *const functionInfo, EntryPointInfo *const entryPoint, Var globalThis, uint16 profiledIterations, bool isInlined = true);

#ifdef FIELD_ACCESS_STATS
    public:
        typename WriteBarrierFieldTypeTraits<FieldAccessStatsPtr>::Type inlineCacheStats;

        void EnsureInlineCacheStats(Recycler* recycler);
        void AddInlineeInlineCacheStats(FunctionCodeGenJitTimeData* inlineeJitTimeData);
#endif

    public:

        static FunctionCodeGenJitTimeData* New(Recycler* recycler, FunctionInfo *const functionInfo, EntryPointInfo *const entryPoint, bool isInlined = true);

    public:
        typename WriteBarrierFieldTypeTraits<BVFixed *>::Type inlineesBv;

        typename WriteBarrierFieldTypeTraits<Js::PropertyId*>::Type sharedPropertyGuards;
        typename WriteBarrierFieldTypeTraits<uint>::Type sharedPropertyGuardCount;

        FunctionInfo *GetFunctionInfo() const;
        FunctionBody *GetFunctionBody() const;
        Var GetGlobalThisObject() const;
        FunctionBodyDataIDL *GetJITBody() const;
        FunctionCodeGenJitTimeData *GetNext() const { return next; }

        const ObjTypeSpecFldInfoArray* GetObjTypeSpecFldInfoArray() const { return &this->objTypeSpecFldInfoArray; }
        ObjTypeSpecFldInfoArray* GetObjTypeSpecFldInfoArray() { return &this->objTypeSpecFldInfoArray; }
        EntryPointInfo* GetEntryPointInfo() const { return this->entryPointInfo; }

    public:
        const FunctionCodeGenJitTimeData *GetInlinee(const ProfileId profiledCallSiteId) const;
        const FunctionCodeGenJitTimeData *GetLdFldInlinee(const InlineCacheIndex inlineCacheIndex) const;
        const FunctionCodeGenJitTimeData * GetCallbackInlinee(const ProfileId profiledCallSiteId) const;
        const FunctionCodeGenJitTimeData * GetCallApplyTargetInlinee(const ProfileId callApplyCallSiteId) const;
        FunctionCodeGenJitTimeData *AddInlinee(
            Recycler *const recycler,
            const ProfileId profiledCallSiteId,
            FunctionInfo *const inlinee,
            bool isInlined = true);
        uint InlineeCount() const;
        uint LdFldInlineeCount() const;
        bool IsLdFldInlineePresent() const { return ldFldInlineeCount != 0; }

        RecyclerWeakReference<FunctionBody> *GetWeakFuncRef() const { return this->weakFuncRef; }
        void SetWeakFuncRef(RecyclerWeakReference<FunctionBody> *weakFuncRef)
        {
            Assert(this->weakFuncRef == nullptr || weakFuncRef == nullptr || this->weakFuncRef == weakFuncRef);
            this->weakFuncRef = weakFuncRef;
        }

        void SetPolymorphicInlineInfo(PolymorphicInlineCacheInfoIDL* inlineeInfo, PolymorphicInlineCacheInfoIDL* selfInfo, PolymorphicInlineCacheIDL* polymorphicInlineCaches)
        {
            this->inlineeInfo = inlineeInfo;
            this->selfInfo = selfInfo;
            this->polymorphicInlineCaches = polymorphicInlineCaches;
        }

        FunctionCodeGenJitTimeData *AddLdFldInlinee(
            Recycler *const recycler,
            const InlineCacheIndex inlineCacheIndex,
            FunctionInfo *const inlinee);

        FunctionCodeGenJitTimeData * AddCallbackInlinee(
            Recycler *const recycler,
            const ProfileId profiledCallSiteId,
            FunctionInfo *const inlinee);

        FunctionCodeGenJitTimeData * AddCallApplyTargetInlinee(
            Recycler *const recycler,
            const ProfileId profiledCallSiteId,
            const ProfileId callApplyCallSiteId,
            FunctionInfo *const inlinee);

        bool IsPolymorphicCallSite(const ProfileId profiledCallSiteId) const;
        // This function walks all the chained jittimedata and returns the one which match the functionInfo.
        // This can return null, if the functionInfo doesn't match.
        const FunctionCodeGenJitTimeData *GetJitTimeDataFromFunctionInfo(FunctionInfo *polyFunctioInfoy) const;

        uint GetGlobalObjTypeSpecFldInfoCount() const { return this->globalObjTypeSpecFldInfoCount; }
        typename WriteBarrierFieldTypeTraits<ObjTypeSpecFldInfo*>::Type* GetGlobalObjTypeSpecFldInfoArray() const {return this->globalObjTypeSpecFldInfoArray; }

        ObjTypeSpecFldInfo* GetGlobalObjTypeSpecFldInfo(uint propertyInfoId) const
        {
            Assert(this->globalObjTypeSpecFldInfoArray != nullptr && propertyInfoId < this->globalObjTypeSpecFldInfoCount);
            return this->globalObjTypeSpecFldInfoArray[propertyInfoId];
        }

        void SetGlobalObjTypeSpecFldInfo(uint propertyInfoId, ObjTypeSpecFldInfo* info) const
        {
            Assert(this->globalObjTypeSpecFldInfoArray != nullptr && propertyInfoId < this->globalObjTypeSpecFldInfoCount);
            this->globalObjTypeSpecFldInfoArray[propertyInfoId] = info;
        }

        void SetGlobalObjTypeSpecFldInfoArray(typename WriteBarrierFieldTypeTraits<ObjTypeSpecFldInfo*>::Type* array, uint count)
        {
            Assert(array != nullptr);
            this->globalObjTypeSpecFldInfoArray = array;
            this->globalObjTypeSpecFldInfoCount = count;
        }

        bool GetIsInlined() const
        {
            return isInlined;
        }
        bool GetIsAggressiveInliningEnabled() const
        {
            return isAggressiveInliningEnabled;
        }
        void SetIsAggressiveInliningEnabled()
        {
            isAggressiveInliningEnabled = true;
        }

        void SetupRecursiveInlineeChain(
            Recycler *const recycler,
            const ProfileId profiledCallSiteId)
        {
            if (!inlinees)
            {
                inlinees = RecyclerNewArrayZ(recycler, typename WriteBarrierFieldTypeTraits<FunctionCodeGenJitTimeData *>::Type, GetFunctionBody()->GetProfiledCallSiteCount());
            }
            inlinees[profiledCallSiteId] = this;
            inlineeCount++;
        }

        uint16 GetProfiledIterations() const;

        PREVENT_COPY(FunctionCodeGenJitTimeData)
    };
}
#endif

