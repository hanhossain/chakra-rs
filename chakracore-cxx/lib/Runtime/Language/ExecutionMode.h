//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

enum class ExecutionMode : uint8_t
{
#define EXECUTION_MODE(name) name,
#include "ExecutionModes.h"
#undef EXECUTION_MODE
};

ENUM_CLASS_HELPERS(ExecutionMode, uint8_t);

extern const char *ExecutionModeName(const ExecutionMode executionMode);
