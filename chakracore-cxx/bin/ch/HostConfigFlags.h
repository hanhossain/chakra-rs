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
    static std::vector<std::u16string> vargsVal;
    static void(*pfnPrintUsage)();

    static void HandleArgsFlag(std::vector<std::u16string> &vargs);

    virtual bool ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser) override;
    virtual void PrintUsage() override;
    static void PrintUsageString();

private:
    int nDummy;
    HostConfigFlags();

    template <typename T>
    void Parse(ICmdLineArgsParser * parser, T * value);
};
