//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "PlatformAgnostic/DateTimeInternal.h"

namespace PlatformAgnostic
{
namespace DateTime
{

    class HiResTimer
    {
    private:
        HiresTimerPlatformData data;

    public:
        double Now();
        double GetSystemTime();

        void Reset() { data.Reset(); }
    };

    class Utility
    {
        UtilityPlatformData data;
    public:
        const WCHAR *GetStandardName(size_t *nameLength,
                                     // xplat implementation needs an actual
                                     // date for the zone abbr.
                                     const DateTime::YMD *ymd = NULL);
        const WCHAR *GetDaylightName(size_t *nameLength,
                                     const DateTime::YMD *ymd = NULL);
    };

    // Decomposed date (Year-Month-Date).
    struct YMD
    {
        int                 year; // year
        int                 yt;   // year type: wkd of Jan 1 (plus 7 if a leap year).
        int                 mon;  // month (0 to 11)
        int                 mday; // day in month (0 to 30)
        int                 yday; // day in year (0 to 365)
        int                 wday; // week day (0 to 6)
        int                 time; // time of day (in milliseconds: 0 to 86399999)

        void ToSystemTime(SYSTEMTIME *sys)
        {
            sys->wYear = (uint16_t)year;
            sys->wMonth = (uint16_t)(mon + 1);
            sys->wDay =(uint16_t)(mday + 1);
            int t = time;
            sys->wMilliseconds = (uint16_t)(t % 1000);
            t /= 1000;
            sys->wSecond = (uint16_t)(t % 60);
            t /= 60;
            sys->wMinute = (uint16_t)(t % 60);
            t /= 60;
            sys->wHour = (uint16_t)t;
        }
    };

    class DaylightTimeHelper
    {
    CLANG_WNO_BEGIN("-Wunused-private-field")
        DaylightTimeHelperPlatformData data;
    CLANG_WNO_END

    public:
        double UtcToLocal(double utcTime, int &bias, int &offset, bool &isDaylightSavings);
        double LocalToUtc(double time);
    };

} // namespace DateTime
} // namespace PlatformAgnostic
