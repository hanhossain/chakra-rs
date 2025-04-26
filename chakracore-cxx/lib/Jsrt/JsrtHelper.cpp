//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "JsrtPch.h"

#if !defined(__ANDROID__)
#include <pthread.h>
#endif

#include "jsrtHelper.h"
#include "Base/ThreadContextTlsEntry.h"

#include "Base/VTuneChakraProfile.h"

#ifdef DYNAMIC_PROFILE_STORAGE
#include "Language/DynamicProfileStorage.h"
#endif

#include "Core/ConfigParser.h"
#include "Base/ThreadBoundThreadContextManager.h"

#ifdef CHAKRA_STATIC_LIBRARY
bool ConfigParserAPI::FillConsoleTitle(__ecount(cchBufferSize) LPWSTR buffer, size_t cchBufferSize, __in LPWSTR moduleName)
{
    return false;
}

void ConfigParserAPI::DisplayInitialOutput(__in LPWSTR moduleName)
{
}

LPCWSTR JsUtil::ExternalApi::GetFeatureKeyName()
{
    return _u("");
}
#endif // CHAKRA_STATIC_LIBRARY

JsrtCallbackState::JsrtCallbackState(ThreadContext* currentThreadContext)
{
    if (currentThreadContext == nullptr)
    {
        originalThreadContext = ThreadContext::GetContextForCurrentThread();
    }
    else
    {
        originalThreadContext = currentThreadContext;
    }
    originalJsrtContext = JsrtContext::GetCurrent();
    Assert(originalJsrtContext == nullptr || originalThreadContext == originalJsrtContext->GetScriptContext()->GetThreadContext());
}

JsrtCallbackState::~JsrtCallbackState()
{
    if (originalJsrtContext != nullptr)
    {
        if (originalJsrtContext != JsrtContext::GetCurrent())
        {
            // This shouldn't fail as the context was previously set on the current thread.
            bool isSet = JsrtContext::TrySetCurrent(originalJsrtContext);
            if (!isSet)
            {
                Js::Throw::FatalInternalError();
            }
        }
    }
    else
    {
        bool isSet = ThreadContextTLSEntry::TrySetThreadContext(originalThreadContext);
        if (!isSet)
        {
            Js::Throw::FatalInternalError();
        }
    }
}

void JsrtCallbackState::ObjectBeforeCallectCallbackWrapper(JsObjectBeforeCollectCallback callback, void* object, void* callbackState, void* threadContext)
{
    JsrtCallbackState scope(reinterpret_cast<ThreadContext*>(threadContext));
    callback(object, callbackState);
}

    void ChakraBinaryAutoSystemInfoInit(AutoSystemInfo * autoSystemInfo)
    {
        autoSystemInfo->buildDateHash = JsUtil::CharacterBuffer<char>::StaticGetHashCode(__DATE__, _countof(__DATE__));
        autoSystemInfo->buildTimeHash = JsUtil::CharacterBuffer<char>::StaticGetHashCode(__TIME__, _countof(__TIME__));
    }

    static pthread_key_t s_threadLocalDummy;
    static THREAD_LOCAL bool s_threadWasEntered = false;

    void DISPOSE_CHAKRA_CORE_THREAD(void *_)
    {
        free(_);
        ThreadBoundThreadContextManager::DestroyContextAndEntryForCurrentThread();
    }

    bool InitializeProcess()
    {
        pthread_key_create(&s_threadLocalDummy, DISPOSE_CHAKRA_CORE_THREAD);

    // setup the cleanup
    // we do not track the main thread. When it exits do the cleanup below
    atexit([]() {
        ThreadBoundThreadContextManager::DestroyContextAndEntryForCurrentThread();

#ifdef DYNAMIC_PROFILE_STORAGE
        DynamicProfileStorage::Uninitialize();
#endif
        JsrtRuntime::Uninitialize();

        // thread-bound entrypoint should be able to get cleanup correctly, however tlsentry
        // for current thread might be left behind if this thread was initialized.
        ThreadContextTLSEntry::CleanupThread();
        ThreadContextTLSEntry::CleanupProcess();
    });

        PAL_InitializeChakraCore();

        HMODULE mod = GetModuleHandleW(NULL);
        AutoSystemInfo::SaveModuleFileName(mod);

    #if defined(_M_IX86) && !defined(__clang__)
        // Enable SSE2 math functions in CRT if SSE2 is available
    #pragma prefast(suppress:6031, "We don't require SSE2, but will use it if available")
        _set_SSE2_enable(TRUE);
    #endif

        {
            CmdLineArgsParser parser;
            ConfigParser::ParseOnModuleLoad(parser, mod);
        }

    #if defined(ENABLE_JS_ETW)
        EtwTrace::Register();
    #endif
    #ifdef VTUNE_PROFILING
        VTuneChakraProfile::Register();
    #endif
#if PERFMAP_TRACE_ENABLED
        PlatformAgnostic::PerfTrace::Register();
#endif

        ValueType::Initialize();
        ThreadContext::GlobalInitialize();

    #ifdef DYNAMIC_PROFILE_STORAGE
        DynamicProfileStorage::Initialize();
    #endif

        return true;
    }

    void VALIDATE_ENTER_CURRENT_THREAD()
    {
        // We do also initialize the process part here
        // This is thread safe by the standard
        // Let's hope compiler doesn't fail
        static bool _has_init = InitializeProcess();

        if (!_has_init) // do not assert this.
        {
            abort();
        }

        if (s_threadWasEntered) return;
        s_threadWasEntered = true;

    #ifdef HEAP_TRACK_ALLOC
        HeapAllocator::InitializeThread();
    #endif

        // put something into key to make sure destructor is going to be called
        pthread_setspecific(s_threadLocalDummy, malloc(1));
    }
