//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include <rust/cxx.h>

#include "Memory/ForcedMemoryConstraints.h"
#include "Core/ICustomConfigFlags.h"
#include "Core/CmdParser.h"
#include "Core/ConfigParser.h"

#include <filesystem>
#include <unistd.h>

ConfigParser ConfigParser::s_moduleConfigParser {};

void ConfigParser::ParseOnModuleLoad(CmdLineArgsParser& parser)
{
    s_moduleConfigParser.ProcessConfiguration();
    // 'parser' destructor post-processes some configuration
}

void ConfigParser::ProcessConfiguration()
{
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
