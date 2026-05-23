//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <print>
#include <rust/cxx.h>
#include <chakra/src/lib.rs.h>

HostConfigFlags HostConfigFlags::flags;
const char16_t** HostConfigFlags::argsVal;
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
    GenerateLibraryByteCodeHeader(nullptr), GenerateLibraryByteCodeHeaderIsEnabled(false),
    GenerateParserStateCache(false), GenerateParserStateCacheIsEnabled(false),
    UseParserStateCache(false), UseParserStateCacheIsEnabled(false),
    InspectMaxStringLength(16), InspectMaxStringLengthIsEnabled(false),
    Serialized(nullptr), SerializedIsEnabled(false),
    OOPJIT(false), OOPJITIsEnabled(false),
    EnsureCloseJITServer(true), EnsureCloseJITServerIsEnabled(false),
    IgnoreScriptErrorCode(false), IgnoreScriptErrorCodeIsEnabled(false),
    MuteHostErrorMsg(false), MuteHostErrorMsgIsEnabled(false),
    TraceHostCallback(false), TraceHostCallbackIsEnabled(false),
    Test262(false), Test262IsEnabled(false),
    Module(false), ModuleIsEnabled(false),
    TrackRejectedPromises(false), TrackRejectedPromisesIsEnabled(false),
    CustomConfigFile(nullptr), CustomConfigFileIsEnabled(false),
    ExecuteWithBgParse(false), ExecuteWithBgParseIsEnabled(false),
    nDummy(0)
{
}

bool HostConfigFlags::ParseFlag(const char16_t* flagsString, ICmdLineArgsParser * parser)
{
    const auto flagsStringRust = rust::String(flagsString);
    if (chakra::strings_eq_insensitive(rust::String(u"GenerateLibraryByteCodeHeader"), flagsStringRust))
    {
        this->GenerateLibraryByteCodeHeaderIsEnabled = true;
        Parse<BSTR>(parser, &this->GenerateLibraryByteCodeHeader);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"GenerateParserStateCache"), flagsStringRust))
    {
        this->GenerateParserStateCacheIsEnabled = true;
        Parse<bool>(parser, &this->GenerateParserStateCache);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"UseParserStateCache"), flagsStringRust))
    {
        this->UseParserStateCacheIsEnabled = true;
        Parse<bool>(parser, &this->UseParserStateCache);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"InspectMaxStringLength"), flagsStringRust))
    {
        this->InspectMaxStringLengthIsEnabled = true;
        Parse<int>(parser, &this->InspectMaxStringLength);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"Serialized"), flagsStringRust))
    {
        this->SerializedIsEnabled = true;
        Parse<BSTR>(parser, &this->Serialized);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"OOPJIT"), flagsStringRust))
    {
        this->OOPJITIsEnabled = true;
        Parse<bool>(parser, &this->OOPJIT);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"EnsureCloseJITServer"), flagsStringRust))
    {
        this->EnsureCloseJITServerIsEnabled = true;
        Parse<bool>(parser, &this->EnsureCloseJITServer);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"IgnoreScriptErrorCode"), flagsStringRust))
    {
        this->IgnoreScriptErrorCodeIsEnabled = true;
        Parse<bool>(parser, &this->IgnoreScriptErrorCode);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"MuteHostErrorMsg"), flagsStringRust))
    {
        this->MuteHostErrorMsgIsEnabled = true;
        Parse<bool>(parser, &this->MuteHostErrorMsg);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"TraceHostCallback"), flagsStringRust))
    {
        this->TraceHostCallbackIsEnabled = true;
        Parse<bool>(parser, &this->TraceHostCallback);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"Test262"), flagsStringRust))
    {
        this->Test262IsEnabled = true;
        Parse<bool>(parser, &this->Test262);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"Module"), flagsStringRust))
    {
        this->ModuleIsEnabled = true;
        Parse<bool>(parser, &this->Module);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"TrackRejectedPromises"), flagsStringRust))
    {
        this->TrackRejectedPromisesIsEnabled = true;
        Parse<bool>(parser, &this->TrackRejectedPromises);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"CustomConfigFile"), flagsStringRust))
    {
        this->CustomConfigFileIsEnabled = true;
        Parse<BSTR>(parser, &this->CustomConfigFile);
        return true;
    }
    if (chakra::strings_eq_insensitive(rust::String(u"ExecuteWithBgParse"), flagsStringRust))
    {
        this->ExecuteWithBgParseIsEnabled = true;
        Parse<bool>(parser, &this->ExecuteWithBgParse);
        return true;
    }
    return false;
}

void HostConfigFlags::PrintUsageString()
{
    std::println("{:>20}          \t{}", "GenerateLibraryByteCodeHeader", "\"Generate bytecode header file from library code\"");
    std::println("{:>20}          \t{}", "GenerateParserStateCache", "\"Parse source file to create parser state cache and write it to file or console\"");
    std::println("{:>20}          \t{}", "UseParserStateCache", "\"Create parser state cache while parsing and use it during script execution\"");
    std::println("{:>20}          \t{}", "InspectMaxStringLength", "\"Max string length to dump in locals inspection\"");
    std::println("{:>20}          \t{}", "Serialized", "\"If source is UTF8, deserializes from bytecode file\"");
    std::println("{:>20}          \t{}", "OOPJIT", "\"Run JIT in a separate process\"");
    std::println("{:>20}          \t{}", "EnsureCloseJITServer", "\"JIT process will be force closed when ch is terminated\"");
    std::println("{:>20}          \t{}", "IgnoreScriptErrorCode", "\"Don't return error code on script error\"");
    std::println("{:>20}          \t{}", "MuteHostErrorMsg", "\"Mute host error output, e.g. module load failures\"");
    std::println("{:>20}          \t{}", "TraceHostCallback", "\"Output traces for host callbacks\"");
    std::println("{:>20}          \t{}", "Test262", "\"load Test262 harness\"");
    std::println("{:>20}          \t{}", "Module", "\"load the script as a module\"");
    std::println("{:>20}          \t{}", "TrackRejectedPromises", "\"Enable tracking of unhandled promise rejections\"");
    std::println("{:>20}          \t{}", "CustomConfigFile", "\"Custom config file to be used to pass in additional flags to Chakra\"");
    std::println("{:>20}          \t{}", "ExecuteWithBgParse", "\"Load script with bgparse (note: requires bgparse and parserstatecache be on as well)\"");
}

void HostConfigFlags::PrintUsage()
{
    if (pfnPrintUsage)
    {
        pfnPrintUsage();
    }

    std::println();
    std::println("Host Config Flags: ");
    std::println();
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
    constexpr std::u16string_view argsFlag = u"-args";
    constexpr std::u16string_view endArgsFlag = u"-endargs";
    int i;
    for (i = 1; i < argc; i++)
    {
        if (argv[i] == argsFlag)
        {
            break;
        }
    }

    int argsStart = ++i;
    for (; i < argc; i++)
    {
        if (argv[i] == endArgsFlag)
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
    HostConfigFlags::argsVal = new const char16_t*[argsCount];
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
