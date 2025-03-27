//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"

//----------------------------------------
// Default operator new/delete overrides
//----------------------------------------

// Update:
//
// Overriding global operator new/delete causes problems in host.
// Do not use global operator new/delete.
//
// To support memory tracking, use explicit HeapNew/HeapDelete.
//

// TODO (hanhossain): remove file