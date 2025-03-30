//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"
#include "Core/EtwTraceCore.h"

#ifdef ENABLE_JS_ETW
extern "C" {
    ETW_INLINE
        void EtwCallback(
        uint32_t controlCode,
        PVOID callbackContext)
    {
        EtwCallbackApi::OnSessionChange(controlCode, callbackContext);
    }
}

bool EtwTraceCore::s_registered = false;

//
// Registers the ETW provider - this is usually done on Jscript DLL load
// After registration, we will receive callbacks when ETW tracing is enabled/disabled.
//
void EtwTraceCore::Register()
{
    if (!s_registered)
    {
        s_registered = true;

        JS_ETW(EventRegisterMicrosoft_JScript());

        // This will be used to distinguish the provider we are getting the callback for.
        PROVIDER_JSCRIPT9_Context.RegistrationHandle = Microsoft_JScriptHandle;
    }
}

//
// Unregister to ensure we do not get callbacks.
//
void EtwTraceCore::UnRegister()
{
    if (s_registered)
    {
        s_registered = false;

        JS_ETW(EventUnregisterMicrosoft_JScript());
    }
}

#endif // ENABLE_JS_ETW
