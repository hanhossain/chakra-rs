//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include <rust/cxx.h>

#include "Memory/MemoryLogger.h"
#include "Memory/ForcedMemoryConstraints.h"
#include "Core/ICustomConfigFlags.h"
#include "Core/CmdParser.h"
#include "Core/ConfigParser.h"

#include <filesystem>
#include <unistd.h>

ConfigParser ConfigParser::s_moduleConfigParser {};

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
    s_moduleConfigParser.ProcessConfiguration();
    // 'parser' destructor post-processes some configuration
}

void ConfigParser::ProcessConfiguration()
{
    [[maybe_unused]] bool hasOutput = false;

#ifdef ENABLE_TRACE
    // TODO (hanhossain): remove
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
