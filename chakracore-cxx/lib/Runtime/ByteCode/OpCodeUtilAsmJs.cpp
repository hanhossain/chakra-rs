//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifdef ASMJS_PLAT
namespace Js
{
    char16_t const * const OpCodeUtilAsmJs::OpCodeAsmJsNames[] =
    {
#define DEF_OP(x, y, ...) u###x,
#include "OpCodeListAsmJs.h"
#undef DEF_OP
    };

    char16_t const * const OpCodeUtilAsmJs::ExtendedOpCodeAsmJsNames[] =
    {
#define DEF_OP(x, y, ...) u###x,
#include "ExtendedOpCodeListAsmJs.h"
#undef DEF_OP
    };

    char16_t const * OpCodeUtilAsmJs::GetOpCodeName(OpCodeAsmJs op)
    {
        if (op <= Js::OpCodeAsmJs::MaxByteSizedOpcodes)
        {
            Assert(op < std::size(OpCodeAsmJsNames));
            return OpCodeAsmJsNames[(int)op];
        }
        else if (op < Js::OpCodeAsmJs::ByteCodeLast)
        {
            uint opIndex = op - (Js::OpCodeAsmJs::MaxByteSizedOpcodes + 1);
            Assert(opIndex < std::size(ExtendedOpCodeAsmJsNames));
            return ExtendedOpCodeAsmJsNames[opIndex];
        }
        return u"<NotAvail>";
    }


    OpLayoutTypeAsmJs const OpCodeUtilAsmJs::OpCodeAsmJsLayouts[] =
    {
#define DEF_OP(x, y, ...) OpLayoutTypeAsmJs::y,
#include "OpCodeListAsmJs.h"
    };

    OpLayoutTypeAsmJs const OpCodeUtilAsmJs::ExtendedOpCodeAsmJsLayouts[] =
    {
#define DEF_OP(x, y, ...) OpLayoutTypeAsmJs::y,
#include "ExtendedOpCodeListAsmJs.h"
    };

    OpLayoutTypeAsmJs OpCodeUtilAsmJs::GetOpCodeLayout(OpCodeAsmJs op)
    {
        if ((uint)op <= (uint)Js::OpCodeAsmJs::MaxByteSizedOpcodes)
        {
            Assert(op < std::size(OpCodeAsmJsLayouts));
            return OpCodeAsmJsLayouts[(uint)op];
        }

        uint opIndex = op - (Js::OpCodeAsmJs::MaxByteSizedOpcodes + 1);
        Assert(opIndex < std::size(ExtendedOpCodeAsmJsLayouts));
        return ExtendedOpCodeAsmJsLayouts[opIndex];
    }

    bool OpCodeUtilAsmJs::IsValidByteCodeOpcode(OpCodeAsmJs op)
    {
        // These OpCodes must have the same value for asm.js and normal javascript.
        // This static_assert will make sure to update both lists if any changes are made
        static_assert((uint)OpCodeAsmJs::EndOfBlock                 == (uint)OpCode::EndOfBlock);
        static_assert((uint)OpCodeAsmJs::ExtendedOpcodePrefix       == (uint)OpCode::ExtendedOpcodePrefix);
        static_assert((uint)OpCodeAsmJs::MediumLayoutPrefix         == (uint)OpCode::MediumLayoutPrefix);
        static_assert((uint)OpCodeAsmJs::ExtendedMediumLayoutPrefix == (uint)OpCode::ExtendedMediumLayoutPrefix);
        static_assert((uint)OpCodeAsmJs::LargeLayoutPrefix          == (uint)OpCode::LargeLayoutPrefix);
        static_assert((uint)OpCodeAsmJs::ExtendedLargeLayoutPrefix  == (uint)OpCode::ExtendedLargeLayoutPrefix);
        static_assert((uint)OpCodeAsmJs::Nop                        == (uint)OpCode::Nop);
        static_assert((uint)OpCodeAsmJs::InvalidOpCode              == (uint)OpCode::InvalidOpCode);

        static_assert((int)Js::OpCodeAsmJs::MaxByteSizedOpcodes + 1 + std::size(OpCodeUtilAsmJs::ExtendedOpCodeAsmJsLayouts) == (int)Js::OpCodeAsmJs::ByteCodeLast);
        return op < std::size(OpCodeAsmJsLayouts)
            || (op > Js::OpCodeAsmJs::MaxByteSizedOpcodes && op < Js::OpCodeAsmJs::ByteCodeLast);
    }

    bool OpCodeUtilAsmJs::IsValidOpcode(OpCodeAsmJs op)
    {
        return IsValidByteCodeOpcode(op)
            || (op > Js::OpCodeAsmJs::ByteCodeLast && op < Js::OpCodeAsmJs::Count);
    }

    void OpCodeUtilAsmJs::ConvertOpToProfiled(OpCodeAsmJs * op)
    {
        Assert(OpCodeAttrAsmJs::HasProfiledOp(*op));
        *op += 1;
        Assert(OpCodeAttrAsmJs::IsProfiledOp(*op));
    }
};
#endif
