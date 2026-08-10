//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// We need real JITManager code when on windows.
// Otherwise we use a dummy JITManager which disables OOP JIT to reduce code noise.

class JITManager
{
public:
private:
    static JITManager s_jitManager;
};
