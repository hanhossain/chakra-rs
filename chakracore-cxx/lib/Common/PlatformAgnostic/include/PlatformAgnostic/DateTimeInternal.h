//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "PlatformAgnostic/CommonTypedefs.h"

namespace PlatformAgnostic
{
namespace DateTime
{
    struct YMD;

    #define DateTimeTicks_PerSecond 1000.0
    #define DateTimeTicks_PerMinute (60 * DateTimeTicks_PerSecond)
    #define DateTimeTicks_PerHour   (60 * DateTimeTicks_PerMinute)
    #define DateTimeTicks_PerDay (DateTimeTicks_PerHour * 24)
    #define DateTimeTicks_PerLargestTZOffset (DateTimeTicks_PerDay + 1)
    #define DateTimeTicks_PerNonLeapYear (DateTimeTicks_PerDay * 365)
    #define DateTimeTicks_PerSafeEndOfYear (DateTimeTicks_PerNonLeapYear - DateTimeTicks_PerLargestTZOffset)

    typedef void* DaylightTimeHelperPlatformData;

    #define __CC_PA_TIMEZONE_ABVR_NAME_LENGTH 32
    struct UtilityPlatformData
    {
        // cache always the last date's zone
        WCHAR standardName[__CC_PA_TIMEZONE_ABVR_NAME_LENGTH];
        size_t standardNameLength;
    };

    class HiresTimerPlatformData
    {
    public:
        double    cacheSysTime;
        unsigned long cacheTick;
        unsigned long previousDifference;

        HiresTimerPlatformData():cacheSysTime(0), cacheTick(-1), previousDifference(0) { }
        void Reset() { /* dummy method for interface compatiblity */ }
    };


} // namespace DateTime
} // namespace PlatformAgnostic
