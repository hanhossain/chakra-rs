//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

// strings for builtin names
#define HASH_NAME(name, hashCS, hashCI) \
    const StaticSymLen<sizeof(#name)> g_ssym_##name = \
    { \
        hashCS, \
        sizeof(#name) - 1, \
        u###name \
    }; \
    static_assert(offsetof(StaticSymLen<sizeof(#name)>, luHash) == offsetof(StaticSym, luHash)); \
    static_assert(offsetof(StaticSymLen<sizeof(#name)>, cch) == offsetof(StaticSym, cch)); \
    static_assert(offsetof(StaticSymLen<sizeof(#name)>, sz) == offsetof(StaticSym, sz)); \

#include "objnames.h"
#undef HASH_NAME
