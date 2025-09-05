//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"

#include "Memory/MemoryLogger.h"
#include "Memory/ForcedMemoryConstraints.h"
#include "Core/ICustomConfigFlags.h"
#include "Core/CmdParser.h"
#include "Core/ConfigParser.h"

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

void ConfigParser::ParseOnModuleLoad(CmdLineArgsParser& parser, HANDLE hmod)
{
    Assert(!s_moduleConfigParser.HasReadConfig());

    s_moduleConfigParser.ParseRegistry(parser);
    s_moduleConfigParser.ParseConfig(hmod, parser);
    s_moduleConfigParser.ProcessConfiguration(hmod);
    // 'parser' destructor post-processes some configuration
}

// TODO (hanhossain): remove
void ConfigParser::ParseRegistry(CmdLineArgsParser &parser)
{
}

// TODO (hanhossain): remove
void ConfigParser::ParseRegistryKey(HKEY hk, CmdLineArgsParser &parser)
{
}

void ConfigParser::ParseConfig(HANDLE hmod, CmdLineArgsParser &parser, const char16_t* strCustomConfigFile)
{
#if defined(ENABLE_DEBUG_CONFIG_OPTIONS) && CONFIG_PARSE_CONFIG_FILE
    Assert(!_hasReadConfig || strCustomConfigFile != nullptr);
    _hasReadConfig = true;

    const char16_t* configFileName = strCustomConfigFile;
    const char16_t* configFileExt = u""; /* in the custom config case,
                                             ext is expected to be passed
                                             in as part of the filename */

    if (configFileName == nullptr)
    {
        configFileName = _configFileName;
        configFileExt = u".config";
    }

    int err = 0;
    char16_t modulename[_MAX_PATH];
    char16_t filename[_MAX_PATH];

    GetModuleFileName((HMODULE)hmod, modulename, _MAX_PATH);
    char16_t drive[_MAX_DRIVE];
    char16_t dir[_MAX_DIR];

    _wsplitpath_s(modulename, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
    _wmakepath_s(filename, drive, dir, configFileName, configFileExt);

    FILE* configFile;
    // Two-pathed for a couple reasons
    // 1. PAL doesn't like the ccs option passed in.
    // 2. _wfullpath is not implemented in the PAL.
    // Instead, on xplat, we'll check the HOME directory to see if there is
    // a config file there that we can use
    if (_wfopen_s(&configFile, filename, u"r") != 0 || configFile == nullptr)
    {
        char16_t homeDir[MAX_PATH];

        if (GetEnvironmentVariable(u"HOME", homeDir, MAX_PATH) == 0)
        {
            return;
        }
        
        char16_t configFileFullName[MAX_PATH];

        StringCchPrintf(configFileFullName, MAX_PATH, u"%s/%s%s", homeDir, configFileName, configFileExt);
        if (_wfopen_s(&configFile, configFileFullName, u"r") != 0 || configFile == nullptr)
        {
            return;
        }
    }

    char16_t configBuffer[MaxTokenSize];
    int index = 0;

#define ReadChar(file) fgetc(file)
#define UnreadChar(c, file) ungetc(c, file)
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

        if (this->_flags.rawInputFromConfigFileIndex < sizeof(this->_flags.rawInputFromConfigFile) / sizeof(this->_flags.rawInputFromConfigFile[0]))
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
#endif
}

void ConfigParser::ProcessConfiguration(HANDLE hmod)
{
#if defined(ENABLE_DEBUG_CONFIG_OPTIONS)
    bool hasOutput = false;
    char16_t modulename[_MAX_PATH];

    GetModuleFileName((HMODULE)hmod, modulename, _MAX_PATH);

    if (Js::Configuration::Global.flags.IsEnabled(Js::OutputFileFlag)
        && Js::Configuration::Global.flags.OutputFile != nullptr)
    {
        SetOutputFile(Js::Configuration::Global.flags.OutputFile, Js::Configuration::Global.flags.OutputFileOpenMode);
        hasOutput = true;
    }

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
        ConfigParserAPI::DisplayInitialOutput(modulename);

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
#endif

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

int32_t ConfigParser::SetOutputFile(const char16_t* outputFile, const char16_t* openMode)
{
    // If present, replace the {PID} token with the process ID
    const char16_t* pidStr = nullptr;
    char16_t buffer[_MAX_PATH];
    if ((pidStr = wcsstr(outputFile, u"{PID}")) != nullptr)
    {
        size_t pidStartPosition = pidStr - outputFile;

        char16_t* pDest = buffer;
        size_t bufferLen = _MAX_PATH;

        // Copy the filename before the {PID} token
        wcsncpy_s(pDest, bufferLen, outputFile, pidStartPosition);
        pDest += pidStartPosition;
        bufferLen = bufferLen - pidStartPosition;

        // Copy the PID
        _itow_s(GetCurrentProcessId(), pDest, /*bufferSize=*/_MAX_PATH - pidStartPosition, /*radix=*/10);
#pragma prefast(suppress: 26014, "ultow string length is smaller than 256")
        pDest += wcslen(pDest);
        bufferLen = bufferLen - wcslen(pDest);

        // Copy the rest of the string.
#pragma prefast(suppress: 26014, "Overwriting pDset's null terminator is intentional since the string being copied is null terminated")
        wcscpy_s(pDest, bufferLen, outputFile + pidStartPosition + /*length of {PID}*/ 5);

        outputFile = buffer;
    }

    char16_t fileName[_MAX_PATH];
    char16_t moduleName[_MAX_PATH];
    PlatformAgnostic::SystemInfo::GetBinaryLocation(moduleName, _MAX_PATH);
    _wsplitpath_s(moduleName, nullptr, 0, nullptr, 0, fileName, _MAX_PATH, nullptr, 0);
    if (_wcsicmp(fileName, u"WWAHost") == 0 ||
        _wcsicmp(fileName, u"ByteCodeGenerator") == 0 ||
        _wcsicmp(fileName, u"spartan") == 0 ||
        _wcsicmp(fileName, u"spartan_edge") == 0 ||
        _wcsnicmp(fileName, u"MicrosoftEdge", wcslen(u"MicrosoftEdge")) == 0)
    {

        // we need to output to %temp% directory in wwa. we don't have permission otherwise.
        if (GetEnvironmentVariable(u"temp", fileName, _MAX_PATH) != 0)
        {
            wcscat_s(fileName, _MAX_PATH, u"\\");
            const char16_t * fileNameOnly = wcsrchr(outputFile, u'\\');
            // if outputFile is full path we just need filename, discard the path
            wcscat_s(fileName, _MAX_PATH, fileNameOnly == nullptr ? outputFile : fileNameOnly);
        }
        else
        {
            AssertMsg(FALSE, "Get temp environment failed");
        }
        outputFile = fileName;
    }

    FILE *fp;
    if ((fp = _wfsopen(outputFile, openMode, _SH_DENYWR)) != nullptr)
    {
        Output::SetOutputFile(fp);
        return S_OK;
    }

    AssertMsg(false, "Could not open file for logging output.");
    return E_FAIL;
}
