//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "HostConfigFlags.h"

#include <chakracore-sys/src/chhelper.rs.h>
#include <chakracore-sys/src/str_helper.rs.h>
#include <print>
#include <rust/cxx.h>

#include "TestHooks.h"

HostConfigFlags HostConfigFlags::flags;
rust::Vec<rust::String> HostConfigFlags::vargsVal;
chakra_rs::ConfigContext HostConfigFlags::configContext;

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
    TraceHostCallback(false), TraceHostCallbackIsEnabled(false),
    Test262(false), Test262IsEnabled(false),
    nDummy(0)
{
}

bool HostConfigFlags::ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser)
{
    const auto flagStringsNormalized = chakra_rs::str_helper::to_lowercase(flagsString);
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
    return false;
}

void HostConfigFlags::PrintUsageString()
{
    std::println("{:>20}          \t{}", "TraceHostCallback", "\"Output traces for host callbacks\"");
    std::println("{:>20}          \t{}", "Test262", "\"load Test262 harness\"");
}

void HostConfigFlags::SetHostArgs(const rust::Vec<rust::String> &hostArgs, const chakra_rs::ConfigContext &config)
{
    HostConfigFlags::vargsVal = hostArgs;
    HostConfigFlags::configContext = config;
}

const chakra_rs::ConfigContext &HostConfigFlags::GetConfig()
{
    return HostConfigFlags::configContext;
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
