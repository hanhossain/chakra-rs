//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// Common definitions used outside parser so that we don't have to include the whole Parser.h.

#pragma once
#include "Memory/ArenaAllocator.h"

namespace Js
{
    typedef int32_t  ByteCodeLabel;       // Size of this match the offset size in layouts
    typedef uint32_t RegSlot;
    typedef uint8_t  RegSlot_OneByte;
    typedef int8_t   RegSlot_OneSByte;
    typedef int16  RegSlot_TwoSByte;
    typedef uint16 RegSlot_TwoByte;
}

enum ErrorTypeEnum
{
    kjstError,
    kjstEvalError,
    kjstRangeError,
    kjstReferenceError,
    kjstSyntaxError,
    kjstTypeError,
    kjstURIError,
    kjstAggregateError,
    kjstWebAssemblyCompileError,
    kjstWebAssemblyRuntimeError,
    kjstWebAssemblyLinkError,
    kjstCustomError,
};

class ParseNode;
class ParseNodeFnc;
typedef ParseNode *ParseNodePtr;

struct Ident;
struct PidRefStack;
typedef Ident *IdentPtr;

struct ModuleImportOrExportEntry
{
    IdentPtr     moduleRequest;
    IdentPtr     importName;
    IdentPtr     localName;
    IdentPtr     exportName;
    PidRefStack* pidRefStack;
    charcount_t  offset;
};

typedef SList<ModuleImportOrExportEntry, ArenaAllocator> ModuleImportOrExportEntryList;
typedef SList<IdentPtr, ArenaAllocator> IdentPtrList;
typedef JsUtil::BaseHashSet<IdentPtr, ArenaAllocator, PowerOf2SizePolicy> IdentPtrSet;

//
// Below was moved from scrutil.h to share with chakradiag.
//
#define HR(sc) ((int32_t)(sc))
