//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <print>
#include <rust/cxx.h>
#include <chakracore-sys/src/chhelper.rs.h>
#include <chakracore-sys/src/str_helper.rs.h>

HostConfigFlags HostConfigFlags::flags;
rust::Vec<rust::String> HostConfigFlags::vargsVal;

template <>
void HostConfigFlags::Parse<bool>(ICmdLineArgsParser * parser, bool * value)
{
    *value = parser->GetCurrentBoolean();
}

template <>
void HostConfigFlags::Parse<int>(ICmdLineArgsParser * parser, int* value)
{
    try
    {
        *value = parser->GetCurrentInt();
    }
    catch (...)
    {
        // Don't do anything, *value will remain its default value.
    }
}

template <>
void HostConfigFlags::Parse<BSTR>(ICmdLineArgsParser * parser, BSTR * bstr)
{
    if (*bstr != NULL)
    {
        SysFreeString(*bstr);
    }
    *bstr = parser->GetCurrentString();
    if (*bstr == NULL)
    {
        *bstr = SysAllocString(u"");
    }
}

HostConfigFlags::HostConfigFlags() :
    UseParserStateCache(false), UseParserStateCacheIsEnabled(false),
    Serialized(nullptr), SerializedIsEnabled(false),
    OOPJIT(false), OOPJITIsEnabled(false),
    IgnoreScriptErrorCode(false), IgnoreScriptErrorCodeIsEnabled(false),
    MuteHostErrorMsg(false), MuteHostErrorMsgIsEnabled(false),
    TraceHostCallback(false), TraceHostCallbackIsEnabled(false),
    Test262(false), Test262IsEnabled(false),
    Module(false), ModuleIsEnabled(false),
    nDummy(0)
{
}

bool HostConfigFlags::ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser)
{
    const auto flagStringsNormalized = chakra_rs::str_helper::to_lowercase(flagsString);
    if (chakra_rs::str_helper::to_lowercase(u"UseParserStateCache") == flagStringsNormalized)
    {
        this->UseParserStateCacheIsEnabled = true;
        Parse<bool>(parser, &this->UseParserStateCache);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"Serialized") == flagStringsNormalized)
    {
        this->SerializedIsEnabled = true;
        Parse<BSTR>(parser, &this->Serialized);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"OOPJIT") == flagStringsNormalized)
    {
        this->OOPJITIsEnabled = true;
        Parse<bool>(parser, &this->OOPJIT);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"IgnoreScriptErrorCode") == flagStringsNormalized)
    {
        this->IgnoreScriptErrorCodeIsEnabled = true;
        Parse<bool>(parser, &this->IgnoreScriptErrorCode);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"MuteHostErrorMsg") == flagStringsNormalized)
    {
        this->MuteHostErrorMsgIsEnabled = true;
        Parse<bool>(parser, &this->MuteHostErrorMsg);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"TraceHostCallback") == flagStringsNormalized)
    {
        this->TraceHostCallbackIsEnabled = true;
        Parse<bool>(parser, &this->TraceHostCallback);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"Test262") == flagStringsNormalized)
    {
        this->Test262IsEnabled = true;
        Parse<bool>(parser, &this->Test262);
        return true;
    }
    if (chakra_rs::str_helper::to_lowercase(u"Module") == flagStringsNormalized)
    {
        this->ModuleIsEnabled = true;
        Parse<bool>(parser, &this->Module);
        return true;
    }
    return false;
}

void HostConfigFlags::PrintUsageString()
{
    std::println("{:>20}          \t{}", "UseParserStateCache", "\"Create parser state cache while parsing and use it during script execution\"");
    std::println("{:>20}          \t{}", "Serialized", "\"If source is UTF8, deserializes from bytecode file\"");
    std::println("{:>20}          \t{}", "OOPJIT", "\"Run JIT in a separate process\"");
    std::println("{:>20}          \t{}", "IgnoreScriptErrorCode", "\"Don't return error code on script error\"");
    std::println("{:>20}          \t{}", "MuteHostErrorMsg", "\"Mute host error output, e.g. module load failures\"");
    std::println("{:>20}          \t{}", "TraceHostCallback", "\"Output traces for host callbacks\"");
    std::println("{:>20}          \t{}", "Test262", "\"load Test262 harness\"");
    std::println("{:>20}          \t{}", "Module", "\"load the script as a module\"");
}

void HostConfigFlags::SetHostArgs(const rust::Vec<rust::String> &hostArgs)
{
    HostConfigFlags::vargsVal = hostArgs;
}

void HostConfigFlags::PrintUsage()
{
    chakra_rs::chhelper::print_usage();

    std::println();
    std::println("Host Config Flags: ");
    std::println();
    HostConfigFlags::PrintUsageString();
    TestHooks::PrintConfigFlagsUsageString();
}
