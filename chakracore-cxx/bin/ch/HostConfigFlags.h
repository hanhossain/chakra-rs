//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#include <rust/cxx.h>
#include <chakracore-sys/src/config.rs.h>

#include "Core/ICustomConfigFlags.h"
class HostConfigFlags : public ICustomConfigFlags
{
public:
    bool Test262; bool Test262IsEnabled;

    static HostConfigFlags flags;
    static rust::Vec<rust::String> vargsVal;
    static chakra_rs::ConfigContext configContext;

    virtual bool ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser) override;
    virtual void PrintUsage() override;
    static void PrintUsageString();
    static void SetHostArgs(const rust::Vec<rust::String> &hostArgs, const chakra_rs::ConfigContext &config);
    static const chakra_rs::ConfigContext &GetConfig();

private:
    int nDummy;
    HostConfigFlags();

    template <typename T>
    void Parse(ICmdLineArgsParser * parser, T * value);
};
