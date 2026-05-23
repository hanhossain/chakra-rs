//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#include "Core/ICustomConfigFlags.h"
class HostConfigFlags : public ICustomConfigFlags
{
public:
    BSTR GenerateLibraryByteCodeHeader; bool GenerateLibraryByteCodeHeaderIsEnabled;
    bool GenerateParserStateCache; bool GenerateParserStateCacheIsEnabled;
    bool UseParserStateCache; bool UseParserStateCacheIsEnabled;
    int InspectMaxStringLength; bool InspectMaxStringLengthIsEnabled;
    BSTR Serialized; bool SerializedIsEnabled;
    bool OOPJIT; bool OOPJITIsEnabled;
    bool EnsureCloseJITServer; bool EnsureCloseJITServerIsEnabled;
    bool IgnoreScriptErrorCode; bool IgnoreScriptErrorCodeIsEnabled;
    bool MuteHostErrorMsg; bool MuteHostErrorMsgIsEnabled;
    bool TraceHostCallback; bool TraceHostCallbackIsEnabled;
    bool Test262; bool Test262IsEnabled;
    bool Module; bool ModuleIsEnabled;
    bool TrackRejectedPromises; bool TrackRejectedPromisesIsEnabled;
    BSTR CustomConfigFile; bool CustomConfigFileIsEnabled;
    bool ExecuteWithBgParse; bool ExecuteWithBgParseIsEnabled;

    static HostConfigFlags flags;
    static const char16_t** argsVal;
    static int argsCount;
    static void(*pfnPrintUsage)();

    static void HandleArgsFlag(int& argc, _Inout_updates_to_(argc, argc) char16_t* argv[]);
    static void RemoveArg(int& argc, _Inout_updates_to_(argc, argc) char16_t* argv[], int index);
    static int FindArg(int argc, _In_reads_(argc) char16_t* argv[], const char16_t * targetArg, size_t targetArgLen);

    template <class Func> static int FindArg(int argc, _In_reads_(argc) char16_t* argv[], Func func);
    template <int LEN> static int FindArg(int argc, _In_reads_(argc) char16_t* argv[], const char16_t(&targetArg)[LEN]);

    virtual bool ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser) override;
    virtual void PrintUsage() override;
    static void PrintUsageString();

private:
    int nDummy;
    HostConfigFlags();

    template <typename T>
    void Parse(ICmdLineArgsParser * parser, T * value);
};

// Find an arg in the arg list that satisfies func. Return the arg index if found.
template <class Func>
int HostConfigFlags::FindArg(int argc, _In_reads_(argc) char16_t * argv[], Func func)
{
    for (int i = 1; i < argc; ++i)
    {
        if (func(argv[i]))
        {
            return i;
        }
    }

    return -1;
}

template <int LEN>
int HostConfigFlags::FindArg(int argc, _In_reads_(argc) char16_t * argv[], const char16_t(&targetArg)[LEN])
{
    return FindArg(argc, argv, targetArg, LEN - 1); // -1 to exclude null terminator
}
