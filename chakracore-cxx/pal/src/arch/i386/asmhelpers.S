// -------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
// -------------------------------------------------------------------------------------------------------
// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

// ==++==
//

//
// ==--==
.intel_syntax noprefix
#include "unixasmmacros.inc"
#include "asmconstants.h"

LEAF_ENTRY GetCurrentSP, _TEXT
        mov rax, rsp
        add rax, 8
        ret
LEAF_END GetCurrentSP, _TEXT

LEAF_ENTRY GetCurrentIP, _TEXT
        mov rax, [rsp]
        ret
LEAF_END GetCurrentIP, _TEXT
