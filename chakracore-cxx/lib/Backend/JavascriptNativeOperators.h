//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
#if ENABLE_NATIVE_CODEGEN
    template <typename T>
    class BranchDictionaryWrapper
    {
    public:
        class DictAllocator :public NativeCodeData::Allocator
        {
        public:
            char * Alloc(size_t requestedBytes)
            {
                char* dataBlock = Allocator::Alloc(requestedBytes);
                return dataBlock;
            }

            char * AllocZero(size_t requestedBytes)
            {
                char* dataBlock = Allocator::AllocZero(requestedBytes);
                return dataBlock;
            }
        };

        template <class TKey, class TValue>
        class SimpleDictionaryEntryWithFixUp : public JsUtil::SimpleDictionaryEntry<TKey, TValue>
        {
        public:
            void FixupWithRemoteKey(void* remoteKey)
            {
                this->key = (TKey)remoteKey;
            }
        };

        typedef JsUtil::BaseDictionary<T, void*, DictAllocator, PowerOf2SizePolicy, DefaultComparer, SimpleDictionaryEntryWithFixUp> BranchBaseDictionary;

        class BranchDictionary :public BranchBaseDictionary
        {
        public:
            BranchDictionary(DictAllocator* allocator, uint dictionarySize)
                : BranchBaseDictionary(allocator, dictionarySize)
            {
            }
        };

        BranchDictionaryWrapper(NativeCodeData::Allocator * allocator, uint dictionarySize, ArenaAllocator* remoteKeyAlloc) :
            defaultTarget(nullptr), dictionary((DictAllocator*)allocator, dictionarySize)
        {
            if (remoteKeyAlloc)
            {
                remoteKeys = AnewArrayZ(remoteKeyAlloc, void*, dictionarySize);
            }
            else
            {
                remoteKeys = nullptr;
            }
        }

        BranchDictionary dictionary;
        void* defaultTarget;
        void** remoteKeys;

        static BranchDictionaryWrapper* New(NativeCodeData::Allocator * allocator, uint dictionarySize, ArenaAllocator* remoteKeyAlloc)
        {
            return NativeCodeDataNew(allocator, BranchDictionaryWrapper, allocator, dictionarySize, remoteKeyAlloc);
        }

        void AddEntry(uint32_t offset, T key, void* remoteVar)
        {
            dictionary.AddNew(key, (void**)offset);
        }

        void Fixup(NativeCodeData::DataChunk* chunkList)
        {
        }
    };

    class JavascriptNativeOperators
    {
    public:
        static void * Op_SwitchStringLookUp(JavascriptString* str, Js::BranchDictionaryWrapper<Js::JavascriptString*>* stringDictionary, uintptr_t funcStart, uintptr_t funcEnd);

        static void TracePropertyEquivalenceCheck(const JitEquivalentTypeGuard* guard, const Type* type, const Type* refType, bool isEquivalent, uint failedPropertyIndex);
        static bool CheckIfTypeIsEquivalent(Type* type, JitEquivalentTypeGuard* guard);
        static bool CheckIfTypeIsEquivalentForFixedField(Type* type, JitEquivalentTypeGuard* guard);
        static bool CheckIfPolyTypeIsEquivalent(Type* type, JitPolyEquivalentTypeGuard* guard, uint8_t index);
        static bool CheckIfPolyTypeIsEquivalentForFixedField(Type* type, JitPolyEquivalentTypeGuard* guard, uint8_t index);
        static bool EquivalenceCheckHelper(Type* type, JitEquivalentTypeGuard* guard, intptr_t value);

        static Var OP_GetElementI_JIT_ExpectingNativeFloatArray(Var instance, Var index, ScriptContext *scriptContext);
        static Var OP_GetElementI_JIT_ExpectingVarArray(Var instance, Var index, ScriptContext *scriptContext);

        static Var OP_GetElementI_UInt32_ExpectingNativeFloatArray(Var instance, uint32_t aElementIndex, ScriptContext* scriptContext);
        static Var OP_GetElementI_UInt32_ExpectingVarArray(Var instance, uint32_t aElementIndex, ScriptContext* scriptContext);

        static Var OP_GetElementI_Int32_ExpectingNativeFloatArray(Var instance, int32_t aElementIndex, ScriptContext* scriptContext);
        static Var OP_GetElementI_Int32_ExpectingVarArray(Var instance, int32_t aElementIndex, ScriptContext* scriptContext);

#if DBG
        static void IntRangeCheckFailure();
#endif
    private:
        static bool IsStaticTypeObjTypeSpecEquivalent(const TypeEquivalenceRecord& equivalenceRecord, uint& failedIndex);
        static bool IsStaticTypeObjTypeSpecEquivalent(const EquivalentPropertyEntry *entry);
    };
#endif
};
