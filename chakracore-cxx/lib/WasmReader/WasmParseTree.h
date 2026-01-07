//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{
    const uint16 EXTENDED_OFFSET = 256;
    namespace Simd {
        const size_t VEC_WIDTH = 4;
        typedef uint32_t simdvec [VEC_WIDTH]; //TODO: maybe we should pull in SIMDValue?
        const size_t MAX_LANES = 16;
        void EnsureSimdIsEnabled();
        bool IsEnabled();
    }

    namespace Threads
    {
        bool IsEnabled();
    };

    namespace WasmNontrapping
    {
        bool IsEnabled();
    };

    namespace SignExtends
    {
        bool IsEnabled();
    };

    namespace WasmTypes
    {
        enum WasmType
        {
            // based on binary format encoding values
            Void = 0,
            I32 = 1,
            I64 = 2,
            F32 = 3,
            F64 = 4,
#ifdef ENABLE_WASM_SIMD
            V128 = 5,
#endif
            Limit,
            Ptr,
            Any,

            FirstLocalType = I32,
            AllLocalTypes = 
                  1 << I32
                | 1 << I64
                | 1 << F32
                | 1 << F64
#ifdef ENABLE_WASM_SIMD
                | 1 << V128
#endif
        };

        namespace SwitchCaseChecks
        {
            template<WasmType... T>
            struct bv;

            template<>
            struct bv<>
            {
                static constexpr uint value = 0;
            };

            template<WasmType... K>
            struct bv<Limit, K...>
            {
                static constexpr uint value = bv<K...>::value;
            };

            template<WasmType K1, WasmType... K>
            struct bv<K1, K...>
            {
                static constexpr uint value = (1 << K1) | bv<K...>::value;
            };
        }

#ifdef ENABLE_WASM_SIMD
#define WASM_V128_CHECK_TYPE Wasm::WasmTypes::V128
#else
#define WASM_V128_CHECK_TYPE Wasm::WasmTypes::Limit
#endif

        template<WasmType... T>
        __declspec(noreturn) void CompileAssertCases()
        {
            CompileAssertMsg(SwitchCaseChecks::bv<T...>::value == AllLocalTypes, "WasmTypes missing in switch-case");
            AssertOrFailFastMsg(UNREACHED, "The WasmType case should have been handled");
        }

        template<WasmType... T>
        void CompileAssertCasesNoFailFast()
        {
            CompileAssertMsg(SwitchCaseChecks::bv<T...>::value == AllLocalTypes, "WasmTypes missing in switch-case");
            AssertMsg(UNREACHED, "The WasmType case should have been handled");
        }

        extern const char16_t* const strIds[Limit];

        bool IsLocalType(WasmTypes::WasmType type);
        uint32_t GetTypeByteSize(WasmType type);
        const char16_t* GetTypeName(WasmType type);
    }
    typedef WasmTypes::WasmType Local;

    enum class ExternalKinds: uint8_t
    {
        Function = 0,
        Table = 1,
        Memory = 2,
        Global = 3,
        Limit
    };

    namespace FunctionIndexTypes
    {
        enum Type
        {
            Invalid = -1,
            ImportThunk,
            Function,
            Import
        };
        bool CanBeExported(Type funcType);
    }

    namespace GlobalReferenceTypes
    {
        enum Type
        {
            Invalid, Const, LocalReference, ImportedReference
        };
    }

    struct WasmOpCodeSignatures
    {
#define WASM_SIGNATURE(id, nTypes, ...) static const WasmTypes::WasmType id[nTypes]; DebugOnly(static const int n##id = nTypes;)
#include "WasmBinaryOpCodes.h"
    };

    enum WasmOp : uint16
    {
#define WASM_OPCODE(opname, opcode, ...) wb##opname = opcode,
// Add prefix to the enum to get a compiler error if there is a collision between operators and prefixes
#define WASM_PREFIX(name, value, ...) prefix##name = value,
#include "WasmBinaryOpCodes.h"
    };

    struct WasmConstLitNode
    {
        union
        {
            float f32;
            double f64;
            int32 i32;
            long i64;
            Simd::simdvec v128;
        };
    };

    struct WasmShuffleNode
    {
        uint8_t indices[Simd::MAX_LANES];
    };

    struct WasmLaneNode
    {
        uint index;
    };

    struct WasmVarNode
    {
        uint32_t num;
    };

    struct WasmMemOpNode
    {
        uint32_t offset;
        uint8_t alignment;
    };

    struct WasmBrNode
    {
        uint32_t depth;
    };

    struct WasmBrTableNode
    {
        uint32_t numTargets;
        uint32_t* targetTable;
        uint32_t defaultTarget;
    };

    struct WasmCallNode
    {
        uint32_t num; // function id
        FunctionIndexTypes::Type funcType;
    };

    struct WasmBlock
    {
    private:
        bool isSingleResult;
        union
        {
            WasmTypes::WasmType singleResult;
            uint32_t signatureId;
        };
    public:
        bool IsSingleResult() const { return isSingleResult; }
        void SetSignatureId(uint32_t id)
        {
            isSingleResult = false;
            signatureId = id;
        }
        uint32_t GetSignatureId() const 
        {
            Assert(!isSingleResult);
            return signatureId;
        }
        void SetSingleResult(WasmTypes::WasmType type)
        {
            isSingleResult = true;
            singleResult = type;
        }
        WasmTypes::WasmType GetSingleResult() const
        {
            Assert(isSingleResult);
            return singleResult;
        }
    };

    struct WasmNode
    {
        WasmOp op;
        union
        {
            WasmBlock block;
            WasmBrNode br;
            WasmBrTableNode brTable;
            WasmCallNode call;
            WasmConstLitNode cnst;
            WasmMemOpNode mem;
            WasmVarNode var;
            WasmLaneNode lane;
            WasmShuffleNode shuffle;
        };
    };

    struct WasmExport
    {
        uint32_t index;
        uint32_t nameLength;
        const char16_t* name;
        ExternalKinds kind;
    };

    struct WasmImport
    {
        ExternalKinds kind;
        uint32_t modNameLen;
        const char16_t* modName;
        uint32_t importNameLen;
        const char16_t* importName;
    };

    struct CustomSection
    {
        const char16_t* name;
        charcount_t nameLength;
        const byte* payload;
        uint32_t payloadSize;
    };
}
