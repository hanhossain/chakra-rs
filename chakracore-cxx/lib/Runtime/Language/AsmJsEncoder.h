//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#ifdef ASMJS_PLAT
namespace Js
{
    class AsmJsEncoder
    {
        struct EncoderRelocLabel;
        struct EncoderReloc
        {
            static void New( EncoderRelocLabel* label, uint8_t* _patchAddr, uint8_t* _pc, ArenaAllocator* allocator );
            uint8_t* patchAddr;
            uint8_t* pc;
            EncoderReloc* next;
        };
        struct EncoderRelocLabel
        {
            EncoderRelocLabel() :labelSeen( false ), relocList( nullptr ){}
            EncoderRelocLabel(uint8_t* _pc) :labelSeen( true ), pc(_pc), relocList( nullptr ){}
            bool labelSeen : 1;
            uint8_t* pc;
            EncoderReloc* relocList;
        };
        // the key is the bytecode address
        typedef JsUtil::BaseDictionary<int, EncoderRelocLabel, ArenaAllocator> RelocLabelMap;
        const byte* ip;
        ByteCodeReader mReader;
        uint32_t mEncodeBufferSize;
        uint8_t* mEncodeBuffer;
        uint8_t* mPc;
        PageAllocator* mPageAllocator;
        InProcCodeGenAllocators* mForegroundAllocators;
        FunctionBody* mFunctionBody;
        RelocLabelMap* mRelocLabelMap;
        ArenaAllocator* mLocalAlloc;
        // Byte offset of first int and double
        int mIntOffset, mDoubleOffset, mFloatOffset;
#ifdef ENABLE_WASM_SIMD
        int mSimdOffset;
#endif
        // architecture dependant data to build templatized JIT
        void* mTemplateData;
    public:
        void* Encode( FunctionBody* functionBody );
        void* GetTemplateData() { return mTemplateData; }
        inline PageAllocator* GetPageAllocator() const{return mPageAllocator;}
        inline void SetPageAllocator( PageAllocator* val ){mPageAllocator = val;}
        inline InProcCodeGenAllocators* GetCodeGenAllocator() const{return mForegroundAllocators;}
        inline void SetCodeGenAllocator( InProcCodeGenAllocators* val ){mForegroundAllocators = val;}
        FunctionBody* GetFunctionBody() { return mFunctionBody; }

    private:
        void ApplyRelocs();
        void AddReloc( const int labelOffset, uint8_t* patchAddr );
        uint32_t GetEncodeBufferSize(FunctionBody* functionBody);
        AsmJsFunctionInfo* GetAsmJsFunctionInfo(){ return mFunctionBody->GetAsmJsFunctionInfo(); }
        AsmJsFunctionInfo* GetAsmJsFunctionInfoWithLock() { return mFunctionBody->GetAsmJsFunctionInfoWithLock(); }
        bool ReadOp();
        template<LayoutSize T> void ReadOpTemplate( OpCodeAsmJs op );

        template<typename T> int GetOffset() const;

        template<typename T> int CalculateOffset(int stackLocation) { return stackLocation*sizeof(T)+GetOffset<T>(); }


        void OP_Label( const OpLayoutEmpty* playout );
        template <class T> void OP_Br( const T* playout );
        template <class T> void OP_BrEq( const T* playout );
        template <class T> void OP_BrEqConst( const T* playout );
        template <class T> void OP_BrTrue( const T* playout );
        template <class T> void OP_Empty( const T* playout );
        template <class T> void OP_CheckHeap(const T* playout);
        template <class T> void Op_LdSlot_Db( const T* playout );
        template <class T> void Op_LdSlot_Int(const T* playout);
        template <class T> void Op_LdSlot_Flt(const T* playout);
        template <class T> void Op_StSlot_Db( const T* playout );
        template <class T> void Op_StSlot_Int(const T* playout);
        template <class T> void Op_StSlot_Flt(const T* playout);
        template <class T> void Op_LdConst_Int(const T* playout);

        template <class T> void Op_LdArr     ( const T* playout );
        template <class T> void Op_LdArrConst( const T* playout );
        template <class T> void Op_StArr     ( const T* playout );
        template <class T> void Op_StArrConst( const T* playout );

        template <class T> void OP_SetReturnInt( const T* playout );
        template <class T> void OP_SetReturnDouble(const T* playout);
        template <class T> void OP_SetReturnFloat(const T* playout);
        template <class T> void OP_SetFroundInt(const T* playout);
        template <class T> void OP_SetFroundDb(const T* playout);
        template <class T> void OP_SetFroundFlt(const T* playout);
        template <class T> void Op_Float_To_Int(const T* playout);
        template <class T> void Op_Float_To_Db(const T* playout);
        template <class T> void Op_UInt_To_Db(const T* playout);
        template <class T> void Op_Int_To_Db( const T* playout );
        template <class T> void Op_Db_To_Int( const T* playout );
        template <class T> void Op_LdAddr_Db( const T* playout );
        template <class T> void OP_LdSlot( const T* playout );

        template <class T> void OP_StartCall( const T* playout );
        template <class T> void OP_Call( const T* playout );
        template <class T> void OP_ArgOut_Db( const T* playout );
        template <class T> void OP_ArgOut_Int(const T* playout);
        template <class T> void OP_Conv_VTD( const T* playout );
        template <class T> void OP_Conv_VTI( const T* playout );
        template <class T> void OP_Conv_VTF( const T* playout );

        template <class T> void OP_I_StartCall( const T* playout );
        template <class T> void OP_I_Call( const T* playout );
        template <class T> void OP_I_ArgOut_Db( const T* playout );
        template <class T> void OP_I_ArgOut_Int(const T* playout);
        template <class T> void OP_I_ArgOut_Flt(const T* playout);

        template <class T> void OP_AsmJsLoopBody(const T* playout);

#ifdef ENABLE_WASM_SIMD
        template <class T> void OP_Simd128_LdF4(const T* playout);
        template <class T> void OP_Simd128_LdI4(const T* playout);
        template <class T> void OP_Simd128_LdD2(const T* playout);

        template <class T> void OP_Simd128_LdSlotF4(const T* playout);
        template <class T> void OP_Simd128_LdSlotI4(const T* playout);
        template <class T> void OP_Simd128_LdSlotD2(const T* playout);
        template <class T> void OP_Simd128_StSlotF4(const T* playout);
        template <class T> void OP_Simd128_StSlotI4(const T* playout);
        template <class T> void OP_Simd128_StSlotD2(const T* playout);

        template <class T> void OP_Simd128_FloatsToF4(const T* playout);
        template <class T> void OP_Simd128_IntsToI4(const T* playout);
        template <class T> void OP_Simd128_DoublesToD2(const T* playout);

        template <class T> void OP_Simd128_ReturnF4(const T* playout);
        template <class T> void OP_Simd128_ReturnI4(const T* playout);
        template <class T> void OP_Simd128_ReturnD2(const T* playout);

        template <class T> void OP_Simd128_SplatF4(const T* playout);
        template <class T> void OP_Simd128_SplatI4(const T* playout);
        template <class T> void OP_Simd128_SplatD2(const T* playout);

        template <class T> void OP_Simd128_FromFloat64x2F4(const T* playout);
        template <class T> void OP_Simd128_FromInt32x4F4(const T* playout);
        template <class T> void OP_Simd128_FromFloat32x4I4(const T* playout);
        template <class T> void OP_Simd128_FromFloat64x2I4(const T* playout);
        template <class T> void OP_Simd128_FromFloat32x4D2(const T* playout);
        template <class T> void OP_Simd128_FromInt32x4D2(const T* playout);

        template <class T> void OP_Simd128_FromFloat32x4BitsD2(const T* playout);
        template <class T> void OP_Simd128_FromInt32x4BitsD2(const T* playout);
        template <class T> void OP_Simd128_FromFloat32x4BitsI4(const T* playout);
        template <class T> void OP_Simd128_FromFloat64x2BitsI4(const T* playout);
        template <class T> void OP_Simd128_FromFloat64x2BitsF4(const T* playout);
        template <class T> void OP_Simd128_FromInt32x4BitsF4(const T* playout);

        template <class T> void OP_Simd128_AbsF4(const T* playout);
        template <class T> void OP_Simd128_AbsD2(const T* playout);
        template <class T> void OP_Simd128_NegF4(const T* playout);
        template <class T> void OP_Simd128_NegI4(const T* playout);
        template <class T> void OP_Simd128_NegD2(const T* playout);
        template <class T> void OP_Simd128_RcpF4(const T* playout);
        template <class T> void OP_Simd128_RcpD2(const T* playout);
        template <class T> void OP_Simd128_RcpSqrtF4(const T* playout);
        template <class T> void OP_Simd128_RcpSqrtD2(const T* playout);
        template <class T> void OP_Simd128_SqrtF4(const T* playout);
        template <class T> void OP_Simd128_SqrtD2(const T* playout);

        template <class T> void OP_Simd128_NotF4(const T* playout);
        template <class T> void OP_Simd128_NotI4(const T* playout);
        template <class T> void OP_Simd128_AddF4(const T* playout);
        template <class T> void OP_Simd128_AddI4(const T* playout);
        template <class T> void OP_Simd128_AddD2(const T* playout);
        template <class T> void OP_Simd128_SubF4(const T* playout);
        template <class T> void OP_Simd128_SubI4(const T* playout);
        template <class T> void OP_Simd128_SubD2(const T* playout);
        template <class T> void OP_Simd128_MulF4(const T* playout);
        template <class T> void OP_Simd128_MulI4(const T* playout);
        template <class T> void OP_Simd128_MulD2(const T* playout);
        template <class T> void OP_Simd128_DivF4(const T* playout);
        template <class T> void OP_Simd128_DivD2(const T* playout);

        template <class T> void OP_Simd128_MinF4(const T* playout);
        template <class T> void OP_Simd128_MinD2(const T* playout);
        template <class T> void OP_Simd128_MaxF4(const T* playout);
        template <class T> void OP_Simd128_MaxD2(const T* playout);

        template <class T> void OP_Simd128_LtF4(const T* playout);
        template <class T> void OP_Simd128_LtI4(const T* playout);
        template <class T> void OP_Simd128_LtD2(const T* playout);
        template <class T> void OP_Simd128_GtF4(const T* playout);
        template <class T> void OP_Simd128_GtI4(const T* playout);
        template <class T> void OP_Simd128_GtD2(const T* playout);
        template <class T> void OP_Simd128_LtEqF4(const T* playout);
        template <class T> void OP_Simd128_LtEqD2(const T* playout);
        template <class T> void OP_Simd128_GtEqF4(const T* playout);
        template <class T> void OP_Simd128_GtEqD2(const T* playout);
        template <class T> void OP_Simd128_EqF4(const T* playout);
        template <class T> void OP_Simd128_EqI4(const T* playout);
        template <class T> void OP_Simd128_EqD2(const T* playout);
        template <class T> void OP_Simd128_NeqF4(const T* playout);
        template <class T> void OP_Simd128_NeqD2(const T* playout);

        template <class T> void OP_Simd128_AndF4(const T* playout);
        template <class T> void OP_Simd128_AndI4(const T* playout);
        template <class T> void OP_Simd128_OrF4(const T* playout);
        template <class T> void OP_Simd128_OrI4(const T* playout);
        template <class T> void OP_Simd128_XorF4(const T* playout);
        template <class T> void OP_Simd128_XorI4(const T* playout);

        template <class T> void OP_Simd128_SelectF4(const T* playout);
        template <class T> void OP_Simd128_SelectI4(const T* playout);
        template <class T> void OP_Simd128_SelectD2(const T* playout);

        template <class T> void OP_Simd128_ExtractLaneI4(const T* playout);
        template <class T> void OP_Simd128_ExtractLaneF4(const T* playout);
        template <class T> void OP_Simd128_ReplaceLaneI4(const T* playout);
        template <class T> void OP_Simd128_ReplaceLaneF4(const T* playout);

        template <class T> void OP_Simd128_I_ArgOutF4(const T* playout);
        template <class T> void OP_Simd128_I_ArgOutI4(const T* playout);
        template <class T> void OP_Simd128_I_ArgOutD2(const T* playout);
#endif // ENABLE_WASM_SIMD
    };
}

#endif
