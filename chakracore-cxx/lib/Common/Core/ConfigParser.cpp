//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "CommonCorePch.h"
#include <rust/cxx.h>

#include "Memory/MemoryLogger.h"
#include "Memory/ForcedMemoryConstraints.h"
#include "Core/ICustomConfigFlags.h"
#include "Core/CmdParser.h"
#include "Core/ConfigParser.h"

#include <filesystem>
#include <unistd.h>

ConfigParser ConfigParser::s_moduleConfigParser(Js::Configuration::Global.flags);

#ifdef ENABLE_TRACE
class ArenaHost
{
    AllocationPolicyManager m_allocationPolicyManager;
    PageAllocator m_pageAllocator;
    ArenaAllocator m_allocator;

public:
    ArenaHost(__in_z const char16_t* arenaName) :
        m_allocationPolicyManager(/* needConcurrencySupport = */ true),
        m_pageAllocator(&m_allocationPolicyManager, Js::Configuration::Global.flags),
        m_allocator(arenaName, &m_pageAllocator, Js::Throw::OutOfMemory)
    {
    }
    ArenaAllocator* GetAllocator() { return &m_allocator; }
};

static ArenaHost s_arenaHost1(u"For Output::Trace (1)");
static ArenaHost s_arenaHost2(u"For Output::Trace (2)");

ArenaAllocator* GetOutputAllocator1()
{
    return s_arenaHost1.GetAllocator();
}

ArenaAllocator* GetOutputAllocator2()
{
    return s_arenaHost2.GetAllocator();
}
#endif

void ConfigParser::ParseOnModuleLoad(CmdLineArgsParser& parser)
{
    Assert(!s_moduleConfigParser.HasReadConfig());

    s_moduleConfigParser.ParseConfig(parser);
    s_moduleConfigParser.ProcessConfiguration();
    // 'parser' destructor post-processes some configuration
}

void ConfigParser::ParseConfig(CmdLineArgsParser &parser, const char16_t* strCustomConfigFile)
{
    Assert(!_hasReadConfig || strCustomConfigFile != nullptr);
    _hasReadConfig = true;

    const char16_t* configFileName = strCustomConfigFile;

    if (configFileName == nullptr)
    {
        configFileName = _configFileName;
    }

    int err = 0;

    const std::filesystem::path filename = configFileName;

    FILE* configFile;
    if (_wfopen_s(&configFile, filename.u16string().c_str(), u"r") != 0 || configFile == nullptr)
    {
        return;
    }

    char16_t configBuffer[MaxTokenSize];
    int index = 0;

#define ReadChar(file) std::getc(file)
#define UnreadChar(c, file) std::ungetc(c, file)
#define CharType int
#define EndChar EOF

    // We don't expect the token to overflow- if it does
    // the simplest thing to do would be to ignore the
    // read tokens
    // We could use _fwscanf_s here but the function
    // isn't implemented in the PAL and we'd have to deal with
    // wchar => char16_t impedance mismatch.
    while (index < MaxTokenSize)
    {
        CharType curChar = ReadChar(configFile);

        if (this->_flags.rawInputFromConfigFileIndex < std::size(this->_flags.rawInputFromConfigFile))
        {
            this->_flags.rawInputFromConfigFile[this->_flags.rawInputFromConfigFileIndex++] = curChar;
        }

        if (curChar == EndChar || isspace(curChar) || curChar == 0)
        {
            configBuffer[index] = 0;

            // Parse only if there's something in configBuffer
            if (index > 0 && (err = parser.Parse(configBuffer)) != 0)
            {
                break;
            }

            while(curChar != EndChar && (isspace(curChar) || curChar == 0))
            {
                curChar = ReadChar(configFile);
            }

            if (curChar == EndChar)
            {
                break;
            }
            else
            {
                UnreadChar(curChar, configFile);
            }

            index = 0;
        }
        else
        {
            // The expectation is that non-ANSI characters
            // are not used in the config- otherwise it will
            // be interpreted incorrectly here
            configBuffer[index++] = (char16_t) curChar;
        }
    }

#undef ReadChar
#undef UnreadChar
#undef CharType
#undef EndChar

    fclose(configFile);

    if (err !=0)
    {
        return;
    }
}

void ConfigParser::ProcessConfiguration()
{
    bool hasOutput = false;

    if (Js::Configuration::Global.flags.DebugWindow)
    {
        Output::UseDebuggerWindow();
        hasOutput = true;
    }

#ifdef ENABLE_TRACE
    if (CONFIG_FLAG(InMemoryTrace))
    {
        Output::SetInMemoryLogger(
            Js::MemoryLogger::Create(::GetOutputAllocator1(),
            CONFIG_FLAG(InMemoryTraceBufferSize) * 3));   // With stack each trace is 3 entries (header, msg, stack).
        hasOutput = true;
    }

#ifdef STACK_BACK_TRACE
    if (CONFIG_FLAG(TraceWithStack))
    {
        Output::SetStackTraceHelper(Js::StackTraceHelper::Create(::GetOutputAllocator2()));
    }
#endif // STACK_BACK_TRACE
#endif // ENABLE_TRACE

    if (hasOutput)
    {
        Output::Print(u"\n");

        Js::Configuration::Global.flags.VerboseDump();
        Output::Flush();
    }

    if (Js::Configuration::Global.flags.ForceSerialized)
    {
        // Can't generate or execute byte code under forced serialize
        Js::Configuration::Global.flags.GenerateByteCodeBufferReturnsCantGenerate = true;
        Js::Configuration::Global.flags.ExecuteByteCodeBufferReturnsInvalidByteCode = true;
    }

    ForcedMemoryConstraint::Apply();

#ifdef MEMSPECT_TRACKING
    bool all = false;
    if (Js::Configuration::Global.flags.Memspect.IsEnabled(Js::AllPhase))
    {
        all = true;
    }
    if (all || Js::Configuration::Global.flags.Memspect.IsEnabled(Js::RecyclerPhase))
    {
        RecyclerMemoryTracking::Activate();
    }
    if (all || Js::Configuration::Global.flags.Memspect.IsEnabled(Js::PageAllocatorPhase))
    {
        PageTracking::Activate();
    }
    if (all || Js::Configuration::Global.flags.Memspect.IsEnabled(Js::ArenaPhase))
    {
        ArenaMemoryTracking::Activate();
    }
#endif
}
