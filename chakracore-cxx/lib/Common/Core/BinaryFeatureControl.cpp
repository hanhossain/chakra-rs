//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"
#include "Core/BinaryFeatureControl.h"

bool
BinaryFeatureControl::RecyclerTest()
{
#ifdef RECYCLER_TEST
    return true;
#else
    return false;
#endif
}
