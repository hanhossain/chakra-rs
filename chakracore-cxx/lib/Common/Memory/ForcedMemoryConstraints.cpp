//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"

#include "Memory/ForcedMemoryConstraints.h"

void
ForcedMemoryConstraint::Apply()
{
    if (Js::Configuration::Global.flags.IsEnabled(Js::ForceFragmentAddressSpaceFlag))
    {
        FragmentAddressSpace(Js::Configuration::Global.flags.ForceFragmentAddressSpace);
    }
}

#pragma prefast(suppress:6262, "Where this function is call should have ample of stack space")
void ForcedMemoryConstraint::FragmentAddressSpace(size_t usableSize)
{
}

