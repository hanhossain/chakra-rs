//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#include <chakracore-sys/src/config.rs.h>

class HostConfigFlags
{
public:
    static chakra_rs::ConfigContext configContext;

    static void SetConfig(const chakra_rs::ConfigContext &config);
    static const chakra_rs::ConfigContext &GetConfig();
};
