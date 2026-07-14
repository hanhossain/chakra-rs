//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCommonPch.h"
#include "RejitReason.h"
#include <iterator>

const char *const RejitReasonNames[] =
{
    #define REJIT_REASON(n) "" STRINGIZE(n) "",
    #include "RejitReasons.h"
    #undef REJIT_REASON
};
const char* const GetRejitReasonName(RejitReason reason)
{
    byte reasonIndex = static_cast<byte>(reason);
    Assert(reasonIndex < NumRejitReasons);
    return RejitReasonNames[reasonIndex];
}

const uint NumRejitReasons = std::size(RejitReasonNames);
