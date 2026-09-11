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
    static HostConfigFlags flags;
    static chakra_rs::ConfigContext configContext;

    void PrintUsage() override;
    static void SetConfig(const chakra_rs::ConfigContext &config);
    static const chakra_rs::ConfigContext &GetConfig();
};
