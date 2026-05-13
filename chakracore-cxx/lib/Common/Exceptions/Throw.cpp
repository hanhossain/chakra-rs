//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "CommonExceptionsPch.h"
#include "Exceptions/Throw.h"
#include "Core/Assertions.h"

#include "StackOverflowException.h"
#include "AsmJsParseException.h"
#include "OutOfMemoryException.h"
#include "NotImplementedException.h"

// Header files required before including ConfigFlagsTable.h

#include <unistd.h>

#include "Interface/EnumHelp.h"
#include "Common/MathUtil.h"
#include "Core/AllocSizeMath.h"

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
    void Throw::FatalInternalError(int32_t hr)
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
        RaiseException((uint32_t)DBG_TERMINATE_PROCESS, EXCEPTION_NONCONTINUABLE, 0, NULL);
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
            Output::Print(u"SystemException: OutOfMemory\n");
            Output::Flush();
        }
#endif
        if (JsUtil::ExternalApi::RaiseOutOfMemoryIfScriptActive())
        {
            AssertMsg(false, "We shouldn't be here");
        }
        throw OutOfMemoryException();
    }

    void Throw::StackOverflow(ScriptContext *scriptContext, void * returnAddress)
    {
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
        if (CONFIG_FLAG(PrintSystemException))
        {
            Output::Print(u"SystemException: StackOverflow\n");
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
#define CHAKRA_ASSERT_CAPTION u"CHAKRA ASSERT"
#endif

    bool Throw::ReportAssert(const char * fileName, uint lineNumber, const char * error, const char * message)
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
            fprintf(stderr, "ASSERTION %u: (%s, line %u) %s\n Failure: %s\n", getpid(), fileName, lineNumber, message, error);
            fflush(stderr);
            return false;
        }

        return false;
    }

#endif

} // namespace Js
