//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#include <rust/cxx.h>

#include "Core/ICustomConfigFlags.h"
class HostConfigFlags : public ICustomConfigFlags
{
public:
    bool UseParserStateCache; bool UseParserStateCacheIsEnabled;
    BSTR Serialized; bool SerializedIsEnabled;
    bool OOPJIT; bool OOPJITIsEnabled;
    bool IgnoreScriptErrorCode; bool IgnoreScriptErrorCodeIsEnabled;
    bool MuteHostErrorMsg; bool MuteHostErrorMsgIsEnabled;
    bool TraceHostCallback; bool TraceHostCallbackIsEnabled;
    bool Test262; bool Test262IsEnabled;
    bool Module; bool ModuleIsEnabled;

    static HostConfigFlags flags;
    static rust::Vec<rust::String> vargsVal;

    virtual bool ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser) override;
    virtual void PrintUsage() override;
    static void PrintUsageString();
    static void SetHostArgs(const rust::Vec<rust::String> &hostArgs);

private:
    int nDummy;
    HostConfigFlags();

    template <typename T>
    void Parse(ICmdLineArgsParser * parser, T * value);
};
