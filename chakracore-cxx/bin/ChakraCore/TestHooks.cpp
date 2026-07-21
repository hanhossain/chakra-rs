//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Runtime.h"

#include "Library/DelayFreeArrayBufferHelper.h"
#include "Base/ThreadContext.h"

#include "Base/StackProber.h"
#include "Base/ScriptContextProfiler.h"

#include "Language/JavascriptConversion.h"

#include "Base/ScriptContextOptimizationOverrideInfo.h"
#include "Base/ScriptContextBase.h"
#include "Base/ScriptContextInfo.h"
#include "Base/ScriptContext.h"
#include "Base/LeaveScriptObject.h"
#include "Base/PropertyRecord.h"

#include "ByteCode/ByteCodeReader.h"
#include "Language/TaggedInt.h"

#include "Library/RootObjectBase.h"
#include "Library/GlobalObject.h"

#include "Library/LiteralString.h"
#include "Library/ConcatString.h"
#include "Library/CompoundString.h"
#include "Library/PropertyRecordUsageCache.h"
#include "Library/PropertyString.h"
#include "Library/SingleCharString.h"

#include "Library/JavascriptTypedNumber.h"
#include "Library/SparseArraySegment.h"
#include "Library/JavascriptError.h"
#include "Library/JavascriptArray.h"
#include "Library/JavascriptSymbol.h"

#include "Library/AtomicsObject.h"
#include "Library/DetachedStateBase.h"
#include "Library/ArrayBuffer.h"
#include "Library/SharedArrayBuffer.h"
#include "Library/TypedArray.h"
#include "Library/JavascriptBoolean.h"
#include "Library/JavascriptBigInt.h"
#include "Library/WebAssemblyEnvironment.h"
#include "Library/WebAssemblyTable.h"
#include "Library/WebAssemblyMemory.h"
#include "Library/WebAssemblyModule.h"
#include "Library/WebAssembly.h"
#include "Core/ConfigParser.h"
#include "TestHooks.h"

namespace PlatformAgnostic
{
namespace UnicodeText
{
namespace Internal
{
// this is in place of including PlatformAgnostic/UnicodeTextInternal.h, which has template
// instantiations that upset Clang on macOS and Linux
int LogicalStringCompareImpl(const char16_t* p1, int p1size, const char16_t* p2, int p2size);
}
}
}

int32_t SetConfigFlags(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags)
{
    CmdLineArgsParser parser(customConfigFlags);
    if (parser.Parse(vargs) != 0)
    {
        return E_FAIL;
    }

    return S_OK;
}

int32_t SetConfigFile(char16_t* strConfigFile)
{
    CmdLineArgsParser parser;
    ConfigParser::ParseCustomConfigFile(parser, strConfigFile);
    return S_OK;
}

int32_t PrintConfigFlagsUsageString()
{
    Js::ConfigFlagsTable::PrintUsageString();
    return S_OK;
}

int32_t SetAssertToConsoleFlag(bool flag)
{
#ifdef DBG
    AssertsToConsole = flag;
#endif
    return S_OK;
}

int32_t OnChakraCoreLoaded()
{
    TestHooks testHooks =
    {
        SetConfigFlags,
        SetConfigFile,
        PrintConfigFlagsUsageString,
        SetAssertToConsoleFlag,
        PlatformAgnostic::UnicodeText::Internal::LogicalStringCompareImpl,

        //BigInt hooks
        Js::JavascriptBigInt::AddDigit,
        Js::JavascriptBigInt::SubDigit,
        Js::JavascriptBigInt::MulDigit,
    };
    return ChakraRTInterface::InitializeTestHooks(testHooks);
}
