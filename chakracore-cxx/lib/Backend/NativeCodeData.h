//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#define NativeCodeDataNew(alloc, T, ...) AllocatorNew(NativeCodeData::AllocatorT<T>, alloc, T, __VA_ARGS__)
#define NativeCodeDataNewZ(alloc, T, ...) AllocatorNewZ(NativeCodeData::AllocatorT<T>, alloc, T, __VA_ARGS__)
#define NativeCodeDataNewArray(alloc, T, count) AllocatorNewArray(NativeCodeData::AllocatorT<NativeCodeData::Array<T>>, alloc, T, count)
#define NativeCodeDataNewArrayZ(alloc, T, count) AllocatorNewArrayZ(NativeCodeData::AllocatorT<NativeCodeData::Array<T>>, alloc, T, count)
#define NativeCodeDataNewPlusZ(size, alloc, T, ...) AllocatorNewPlusZ(NativeCodeData::AllocatorT<T>, alloc, size, T, __VA_ARGS__)

#define NativeCodeDataNewNoFixup(alloc, T, ...) AllocatorNew(NativeCodeData::AllocatorNoFixup<T>, alloc, T, __VA_ARGS__)
#define NativeCodeDataNewZNoFixup(alloc, T, ...) AllocatorNewZ(NativeCodeData::AllocatorNoFixup<T>, alloc, T, __VA_ARGS__)
#define NativeCodeDataNewArrayNoFixup(alloc, T, count) AllocatorNewArray(NativeCodeData::AllocatorNoFixup<NativeCodeData::Array<T>>, alloc, T, count)
#define NativeCodeDataNewArrayZNoFixup(alloc, T, count) AllocatorNewArrayZ(NativeCodeData::AllocatorNoFixup<NativeCodeData::Array<T>>, alloc, T, count)
#define NativeCodeDataNewPlusZNoFixup(size, alloc, T, ...) AllocatorNewPlusZ(NativeCodeData::AllocatorNoFixup<T>, alloc, size, T, __VA_ARGS__)

class NativeCodeData
{

public:

    struct DataChunk
    {
        unsigned int len;
        unsigned int allocIndex;
        unsigned int offset; // offset to the aggregated buffer
#if DBG
        const char* dataType;
#endif

        // todo: use union?
        void(*fixupFunc)(void* _this, NativeCodeData::DataChunk*);
        NativeDataFixupEntry *fixupList;

        DataChunk * next;
        char data[0];
    };

    struct DataChunkNoFixup
    {
        unsigned int len; // only used for memory management purposes.
        DataChunkNoFixup * next;
        char data[0];
    };

    NativeCodeData(DataChunk * chunkList);
    union
    {
        DataChunk * chunkList;
        DataChunkNoFixup * noFixupChunkList;
    };

public:
    template <class DataChunkT>
    static void DeleteChunkList(DataChunkT * chunkList);
public:
    class Allocator
    {
    public:
        static const bool FakeZeroLengthArray = false;

        Allocator();
        ~Allocator();

        char * Alloc(size_t requestedBytes);
        char * AllocZero(size_t requestedBytes);
        char * AllocLeaf(size_t requestedBytes);

        NativeCodeData * Finalize();
        void Free(void * buffer, size_t byteSize);

        union
        {
            DataChunk * chunkList;
            DataChunkNoFixup* noFixupChunkList;
        };
        DataChunk * lastChunkList; // used to maintain the allocation order in the list
        unsigned int totalSize;
        unsigned int allocCount;
    private:
#if DBG
        bool finalized;
#endif
    };

    // TODO (hanhossain): remove OOPJIT
    template<typename T>
    class Array
    {
    };

    template<typename T>
    class AllocatorNoFixup : public Allocator
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
        char * AllocLeaf(size_t requestedBytes)
        {
            return Alloc(requestedBytes);
        }
    };

    template<typename T>
    class AllocatorT : public Allocator
    {
        char* AddFixup(char* dataBlock)
        {
            return dataBlock;
        }

    public:
        char * Alloc(size_t requestedBytes)
        {
            return AddFixup(Allocator::Alloc(requestedBytes));
        }
        char * AllocZero(size_t requestedBytes)
        {
            return AddFixup(Allocator::AllocZero(requestedBytes));
        }
    };

    ~NativeCodeData();
};

enum DataDesc
{
    DataDesc_None,
    DataDesc_InlineeFrameRecord_ArgOffsets,
    DataDesc_InlineeFrameRecord_Constants,
    DataDesc_BailoutInfo_CotalOutParamCount,
    DataDesc_ArgOutOffsetInfo_StartCallOutParamCounts,
    DataDesc_ArgOutOffsetInfo_StartCallArgRestoreAdjustCounts,
    DataDesc_LowererMD_LoadFloatValue_Float,
    DataDesc_LowererMD_LoadFloatValue_Double,
    DataDesc_LowererMD_EmitLoadFloatCommon_Double,
    DataDesc_LowererMD_Simd128LoadConst,
};

template<DataDesc desc = DataDesc_None>
struct IntType
{
    int data;
};

template<DataDesc desc = DataDesc_None>
struct UIntType
{
    uint data;
};

template<DataDesc desc = DataDesc_None>
struct FloatType
{
    FloatType(float val) :data(val) {}
    float data;
};

template<DataDesc desc = DataDesc_None>
struct DoubleType
{
    DoubleType() {}
    DoubleType(double val) :data(val) {}
    double data;
};

template<DataDesc desc = DataDesc_None>
struct SIMDType
{
    SIMDType() {}
    SIMDType(AsmJsSIMDValue val) :data(val) {}
    AsmJsSIMDValue data;
};

template<DataDesc desc = DataDesc_None>
struct VarType
{
    Js::Var data;
    void Fixup(NativeCodeData::DataChunk* chunkList)
    {
        AssertMsg(false, "Please specialize Fixup method for this Var type or use no-fixup allocator");
    }
};

template<>
inline void VarType<DataDesc_InlineeFrameRecord_Constants>::Fixup(NativeCodeData::DataChunk* chunkList)
{
    AssertMsg(false, "InlineeFrameRecord::constants contains Var from main process, should not fixup");
}
