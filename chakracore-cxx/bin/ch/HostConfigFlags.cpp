//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "HostConfigFlags.h"

#include <chakracore-sys/src/chhelper.rs.h>
#include <print>
#include <rust/cxx.h>

#include "TestHooks.h"

HostConfigFlags HostConfigFlags::flags;
rust::Vec<rust::String> HostConfigFlags::vargsVal;
chakra_rs::ConfigContext HostConfigFlags::configContext;

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
    TestHooks::PrintConfigFlagsUsageString();
}
