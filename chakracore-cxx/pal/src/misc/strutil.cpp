//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    strutil.cpp

Abstract:
    Various string-related utility functions



--*/

#include "pal/corunix.hpp"
#include "pal/thread.hpp"
#include <new>
#include "pal/dbgmsg.h"

SET_DEFAULT_DEBUG_CHANNEL(PAL);

using namespace CorUnix;

/*++
Function:
  CPalString::CopyString

  Copies a CPalString into a new (empty) instance, allocating buffer space
  as necessary

Parameters:
  psSource -- the string to copy from
--*/

PAL_ERROR
CPalString::CopyString(
    CPalString *psSource
    )
{
    PAL_ERROR palError = NO_ERROR;

    if (0 != psSource->GetStringLength())
    {
        pwsz_ = std::u16string(psSource->pwsz_);
    }

    return palError;
}
