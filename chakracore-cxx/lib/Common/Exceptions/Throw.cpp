//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "CommonExceptionsPch.h"

#ifndef USING_PAL_STDLIB
// === C Runtime Header Files ===
#pragma warning(push)
#pragma warning(disable: 4995) /* 'function': name was marked as #pragma deprecated */
#include <strsafe.h>
#pragma warning(pop)
#endif

#include "StackOverflowException.h"
#include "AsmJsParseException.h"
#include "OutOfMemoryException.h"
#include "NotImplementedException.h"

// Header files required before including ConfigFlagsTable.h

#include "EnumHelp.h"
#include "Common/MathUtil.h"
#include "Core/AllocSizeMath.h"
#include "Core/FaultInjection.h"

#include "Core/BasePtr.h"
#include "Core/AutoFile.h"
#include "Core/Output.h"

// Memory Management
namespace Memory {
    class ArenaAllocator;
}
using namespace Memory;
#include "Memory/Allocator.h"
#include "Memory/HeapAllocator.h"
#include "Memory/RecyclerPointers.h"

// Data structure
#include "DataStructures/Comparer.h"
#include "DataStructures/SizePolicy.h"
#include "DataStructures/SList.h"
#include "DataStructures/KeyValuePair.h"
#include "DataStructures/DefaultContainerLockPolicy.h"
#include "DataStructures/BaseDictionary.h"
#include "Core/ConfigFlagsTable.h"

#include "Core/StackBackTrace.h"

namespace Js {
    void Throw::FatalInternalError(HRESULT hr)
    {
        int scenario = 2;
        ReportFatalException(NULL, hr, Fatal_Internal_Error, scenario);
    }

    void Throw::FatalInternalErrorEx(int scenario)
    {
        ReportFatalException(NULL, E_FAIL, Fatal_Internal_Error, scenario);
    }

    void Throw::FatalInternalGlobalizationError()
    {
        AssertMsg(false, "Failure in initializing Globalization library");
        int scenario = 2;
        ReportFatalException(NULL, E_FAIL, Fatal_Internal_Error, scenario);
    }

    void Throw::FatalProjectionError()
    {
        RaiseException((DWORD)DBG_TERMINATE_PROCESS, EXCEPTION_NONCONTINUABLE, 0, NULL);
    }

#ifdef ENABLE_JS_BUILTINS
    void Throw::FatalJsBuiltInError()
    {
        AssertMsg(false, "Could not initialize JsBuiltIns!");
        ReportFatalException(NULL, E_UNEXPECTED, Fatal_JsBuiltIn_Error, 0);
    }
#endif

#if ENABLE_JS_REENTRANCY_CHECK
    void Throw::FatalJsReentrancyError()
    {
        AssertMsg(false, "Js reentrancy error!!");
        ReportFatalException(NULL, E_UNEXPECTED, Fatal_JsReentrancy_Error, 0);
    }
#endif

    void Throw::InternalError()
    {
        AssertOrFailFastMsg(false, "Internal error!!");
    }

    void Throw::OutOfMemory()
    {
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
        if (CONFIG_FLAG(PrintSystemException))
        {
            Output::Print(_u("SystemException: OutOfMemory\n"));
            Output::Flush();
        }
#endif
        if (JsUtil::ExternalApi::RaiseOutOfMemoryIfScriptActive())
        {
            AssertMsg(false, "We shouldn't be here");
        }
        throw OutOfMemoryException();
    }

#if !defined(_M_IX86) && defined(_WIN32)
    void Throw::XDataRegistrationError(HRESULT hr, ULONG_PTR funcStart)
    {
        ULONG_PTR            imageBase = 0;
        RUNTIME_FUNCTION  *runtimeFunction = RtlLookupFunctionEntry(funcStart, &imageBase, nullptr);

        if (JsUtil::ExternalApi::GetCurrentThreadContextId())
        {
            XDataRegistration_unrecoverable_error(hr, (ULONG_PTR)runtimeFunction);
        }
        throw OutOfMemoryException();
    }
#endif

    void Throw::StackOverflow(ScriptContext *scriptContext, PVOID returnAddress)
    {
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
        if (CONFIG_FLAG(PrintSystemException))
        {
            Output::Print(_u("SystemException: StackOverflow\n"));
            Output::Flush();
        }
#endif
        if (JsUtil::ExternalApi::RaiseStackOverflowIfScriptActive(scriptContext, returnAddress))
        {
            AssertMsg(false, "We shouldn't be here");
        }
        throw StackOverflowException();
    }

    void Throw::NotImplemented()
    {
        AssertMsg(false, "This functionality is not yet implemented");

        throw NotImplementedException();
    }

#if DBG
    // After assert the program should terminate. Sometime we saw the program continue somehow
    // log the existence of assert for debugging.
    void Throw::LogAssert()
    {
        IsInAssert = true;
    }

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
#define CHAKRA_ASSERT_CAPTION _u("CHAKRA ASSERT")
#endif

    bool Throw::ReportAssert(__in LPCSTR fileName, uint lineNumber, __in LPCSTR error, __in LPCSTR message)
    {
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
        if (Js::Configuration::Global.flags.IsEnabled(Js::AssertBreakFlag))
        {
            DebugBreak();
            return false;
        }
        if (Js::Configuration::Global.flags.IsEnabled(Js::AssertIgnoreFlag))
        {
            return true;
        }
#endif
        if (AssertsToConsole)
        {
            fprintf(stderr, "ASSERTION %u: (%s, line %u) %s\n Failure: %s\n", GetCurrentProcessId(), fileName, lineNumber, message, error);
            fflush(stderr);
            return false;
        }

        // The following code is applicable only when we are hosted in an
        // GUI environment
#if defined(ENABLE_DEBUG_CONFIG_OPTIONS) && defined(_WIN32)
        // Then if DumpOncrashFlag is not specified it directly returns,
        // otherwise if will raise a non-continuable exception, generate the dump and terminate the process.
        // the popup message box might be useful when testing in IE
        if (Js::Configuration::Global.flags.AssertPopUp)
        {
            char16 buff[1024];

            swprintf_s(buff, _countof(buff), _u("%S (%u)\n%S\n%S"), fileName, lineNumber, message, error);
            buff[_countof(buff)-1] = 0;

            int ret = MessageBox(nullptr, buff, CHAKRA_ASSERT_CAPTION, MB_ABORTRETRYIGNORE);

            switch (ret)
            {
            case IDIGNORE:
                return true;
            case IDABORT:
                Throw::FatalInternalError();
            default:
                return false;
            }
        }
#endif
        return false;
    }

#endif

} // namespace Js
