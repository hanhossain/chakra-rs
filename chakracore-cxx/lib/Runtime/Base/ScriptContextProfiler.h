//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Core/ProfileInstrument.h"

class ServerScriptContext;
class NativeCodeGenerator;
namespace Js
{
    class ScriptContextProfiler
    {
#ifdef PROFILE_EXEC
        friend class ::NativeCodeGenerator;
        friend class ::ServerScriptContext;

    public:
        ScriptContextProfiler();
        ~ScriptContextProfiler();

        bool IsInitialized() const { return profilerArena != nullptr; }
        void Initialize(PageAllocator * pageAllocator, Recycler * recycler);

        uint32_t AddRef();
        uint32_t Release();

        Profiler * GetProfiler() { return profiler; }
        Profiler * GetBackgroundRecyclerProfiler() { return backgroundRecyclerProfiler; }
        void ProfileBegin(Js::Phase phase);
        void ProfileEnd(Js::Phase phase);
        void ProfileSuspend(Js::Phase, Js::Profiler::SuspendRecord * suspendRecord);
        void ProfileResume(Js::Profiler::SuspendRecord * suspendRecord);
        void ProfilePrint(Js::Phase phase);
        void ProfileMerge(ScriptContextProfiler * profiler);
        void ProfilePrint();
    private:
        ArenaAllocator * profilerArena;
        ArenaAllocator * backgroundRecyclerProfilerArena;
        Profiler * profiler;
        Profiler * backgroundRecyclerProfiler;
        uint refcount;
        Recycler * recycler;
        PageAllocator *pageAllocator;
        ScriptContextProfiler *next;

#endif
    };
};
