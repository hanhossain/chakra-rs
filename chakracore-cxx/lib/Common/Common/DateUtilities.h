//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

using namespace PlatformAgnostic;

namespace Js
{
    // Global date constants
    extern const double g_kdblJanuary1st1970;
    extern const char16 g_rgpszDay[7][4];
    extern const char16 g_rgpszMonth[12][4];
    extern const char16 g_rgpszZone[8][4];

    // Utility methods to manipulate various date formats
    class DateUtilities
    {
        static const int64_t ticksPerSecond;
        static const int64_t ticksPerMinute;
        static const int64_t ticksPerHour;
        static const int64_t ticksPerDay;
        static const int64_t jsEpochTicks;

    public:

        static const int64_t ticksPerMillisecond;
        static const double ticksPerMillisecondDouble;
        static const int64_t jsEpochMilliseconds;
#ifdef INTL_WINGLOB
        static HRESULT ES5DateToWinRTDate(double es5Date, __out int64_t* pResult);
#endif
        static double TimeFromSt(SYSTEMTIME *pst);
        static double DayTimeFromSt(SYSTEMTIME *pst);
        static double TvFromDate(double year, double mon, double day, double time);
        static double DblModPos(double dbl, double dblDen);
        static double DayFromYear(double year);
        static int GetDayMinAndUpdateYear(int day, int &year);
        static bool FLeap(int year);

        static void GetYmdFromTv(double tv, DateTime::YMD *pymd);
        static void GetYearFromTv(double tv, int &year, int &yearType);
    };

}
