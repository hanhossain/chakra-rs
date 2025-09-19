//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

HostConfigFlags HostConfigFlags::flags;
char16_t** HostConfigFlags::argsVal;
int HostConfigFlags::argsCount;
void(*HostConfigFlags::pfnPrintUsage)();

template <>
void HostConfigFlags::Parse<bool>(ICmdLineArgsParser * parser, bool * value)
{
    *value = parser->GetCurrentBoolean();
}

template <>
void HostConfigFlags::Parse<int>(ICmdLineArgsParser * parser, int* value)
{
    try
    {
        *value = parser->GetCurrentInt();
    }
    catch (...)
    {
        // Don't do anything, *value will remain its default value.
    }
}

template <>
void HostConfigFlags::Parse<BSTR>(ICmdLineArgsParser * parser, BSTR * bstr)
{
    if (*bstr != NULL)
    {
        SysFreeString(*bstr);
    }
    *bstr = parser->GetCurrentString();
    if (*bstr == NULL)
    {
        *bstr = SysAllocString(u"");
    }
}

HostConfigFlags::HostConfigFlags() :
#define FLAG(Type, Name, Desc, Default) \
    Name##IsEnabled(false), \
    Name(Default),
#include "HostConfigFlagsList.h"
    nDummy(0)
{
}

bool HostConfigFlags::ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser)
{
#define FLAG(Type, Name, Desc, Default) \
    if (_wcsicmp(_u(#Name), flagsString) == 0) \
    { \
        this->Name##IsEnabled = true; \
        Parse<Type>(parser, &this->Name); \
        return true; \
    }
#include "HostConfigFlagsList.h"
    return false;
}

void HostConfigFlags::PrintUsageString()
{
#define FLAG(Type, Name, Desc, Default) \
    wprintf(u"%20ls          \t%ls\n", _u(#Name), _u(#Desc));
#include "HostConfigFlagsList.h"
}

void HostConfigFlags::PrintUsage()
{
    if (pfnPrintUsage)
    {
        pfnPrintUsage();
    }

    wprintf(u"\nHost Config Flags: \n\n");
    HostConfigFlags::PrintUsageString();
    ChakraRTInterface::PrintConfigFlagsUsageString();
}

int HostConfigFlags::FindArg(int argc, _In_reads_(argc) char16_t* argv[], const char16_t * targetArg, size_t targetArgLen)
{
    return FindArg(argc, argv, [=](const char16_t * arg) -> bool
    {
        return _wcsnicmp(arg, targetArg, targetArgLen) == 0;
    });
}

void HostConfigFlags::RemoveArg(int& argc, _Inout_updates_to_(argc, argc) char16_t* argv[], int index)
{
    Assert(index >= 0 && index < argc);
    for (int i = index + 1; i < argc; ++i)
    {
#pragma prefast(suppress:__WARNING_READ_OVERRUN, "Operation is safe but PREfast is difficult to convince")
        argv[i - 1] = argv[i];
    }
    --argc;
}

void HostConfigFlags::HandleArgsFlag(int& argc, _Inout_updates_to_(argc, argc) char16_t* argv[])
{
    const char16_t* argsFlag = u"-args";
    const char16_t* endArgsFlag = u"-endargs";
    int argsFlagLen = static_cast<int>(wcslen(argsFlag));
    int i;
    for (i = 1; i < argc; i++)
    {
        if (_wcsnicmp(argv[i], argsFlag, argsFlagLen) == 0)
        {
            break;
        }
    }
    int argsStart = ++i;
    for (; i < argc; i++)
    {
        if (_wcsnicmp(argv[i], endArgsFlag, argsFlagLen) == 0)
        {
            break;
        }
    }
    int argsEnd = i;

    int argsCount = argsEnd - argsStart;
    if (argsCount == 0)
    {
        return;
    }
    HostConfigFlags::argsVal = new char16_t*[argsCount];
    HostConfigFlags::argsCount = argsCount;
    int argIndex = argsStart;
    for (i = 0; i < argsCount; i++)
    {
        HostConfigFlags::argsVal[i] = argv[argIndex++];
    }

    argIndex = argsStart - 1;
    for (i = argsEnd + 1; i < argc; i++)
    {
        char16_t* temp = argv[argIndex];
        argv[argIndex] = argv[i];
        argv[i] = temp;
        argIndex++;
    }
    Assert(argIndex == argc - argsCount - 1 - (argsEnd < argc));
    argc = argIndex;
}
