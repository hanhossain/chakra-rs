//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef ENABLE_TEST_HOOKS
#include <oaidl.h>
interface ICustomConfigFlags;

struct TestHooks
{
    typedef HRESULT(*SetConfigFlagsPtr)(int argc, LPWSTR argv[], ICustomConfigFlags* customConfigFlags);
    typedef HRESULT(*SetConfigFilePtr)(LPWSTR strConfigFile);
    typedef HRESULT(*PrintConfigFlagsUsageStringPtr)(void);
    typedef HRESULT(*SetAssertToConsoleFlagPtr)(bool flag);
    typedef HRESULT(*SetEnableCheckMemoryLeakOutputPtr)(bool flag);
    typedef void(* NotifyUnhandledExceptionPtr)(PEXCEPTION_POINTERS exceptionInfo);
    typedef int(*LogicalStringCompareImpl)(const char16_t* p1, int p1size, const char16_t* p2, int p2size);

    SetConfigFlagsPtr pfSetConfigFlags;
    SetConfigFilePtr  pfSetConfigFile;
    PrintConfigFlagsUsageStringPtr pfPrintConfigFlagsUsageString;
    SetAssertToConsoleFlagPtr pfSetAssertToConsoleFlag;
    SetEnableCheckMemoryLeakOutputPtr pfSetEnableCheckMemoryLeakOutput;
    LogicalStringCompareImpl pfLogicalCompareStringImpl;

    // Javasscript Bigint hooks
    typedef digit_t(*AddDigit)(digit_t a, digit_t b, digit_t* carry);
    typedef digit_t(*SubDigit)(digit_t a, digit_t b, digit_t* borrow);
    typedef digit_t(*MulDigit)(digit_t a, digit_t b, digit_t* high);
    AddDigit pfAddDigit;
    SubDigit pfSubDigit;
    MulDigit pfMulDigit;

#define FLAG(type, name, description, defaultValue, ...) FLAG_##type##(name)
#define FLAG_String(name) \
    bool (*pfIsEnabled##name##Flag)(); \
    HRESULT (*pfGet##name##Flag)(BSTR *flag); \
    HRESULT (*pfSet##name##Flag)(BSTR flag);
#define FLAG_Boolean(name) \
    bool (*pfIsEnabled##name##Flag)(); \
    HRESULT (*pfGet##name##Flag)(bool *flag); \
    HRESULT (*pfSet##name##Flag)(bool flag);
#define FLAG_Number(name) \
    bool (*pfIsEnabled##name##Flag)(); \
    HRESULT (*pfGet##name##Flag)(int *flag); \
    HRESULT (*pfSet##name##Flag)(int flag);
    // skipping other types
#define FLAG_Phases(name)
#define FLAG_NumberSet(name)
#define FLAG_NumberPairSet(name)
#define FLAG_NumberTrioSet(name)
#define FLAG_NumberRange(name)
#include "Interface/ConfigFlagsList.h"
#undef FLAG
#undef FLAG_String
#undef FLAG_Boolean
#undef FLAG_Number
#undef FLAG_Phases
#undef FLAG_NumberSet
#undef FLAG_NumberPairSet
#undef FLAG_NumberTrioSet
#undef FLAG_NumberRange

    NotifyUnhandledExceptionPtr pfnNotifyUnhandledException;
};

typedef HRESULT(*OnChakraCoreLoadedPtr)(TestHooks &testHooks);
HRESULT OnChakraCoreLoaded(OnChakraCoreLoadedPtr pfChakraCoreLoaded = NULL);

#endif
