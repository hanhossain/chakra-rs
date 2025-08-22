//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

#if DBG_DUMP
#include "Language/JavascriptStackWalker.h"

char16_t* DumpCallStack(uint frameCount) { return DumpCallStackFull(frameCount, /*print*/ true); }

char16_t* DumpCallStackFull(uint frameCount, bool print)
{
    Js::ScriptContext* scriptContext = ThreadContext::GetContextForCurrentThread()->GetScriptContextList();
    Js::JavascriptStackWalker walker(scriptContext);

    char16_t buffer[512];
    Js::StringBuilder<ArenaAllocator> sb(scriptContext->GeneralAllocator());
    uint fc = 0;
    while (walker.Walk())
    {
        void * codeAddr = walker.GetCurrentCodeAddr();
        if (walker.IsJavascriptFrame())
        {
            char16_t debugStringBuffer[MAX_FUNCTION_BODY_DEBUG_STRING_SIZE];
            StringCchPrintf(buffer, _countof(buffer), _u("0x%p  "), codeAddr);
            sb.AppendSz(buffer);
            // Found a JavascriptFunction.  Dump its name and parameters.
            Js::JavascriptFunction *jsFunc = walker.GetCurrentFunction();

            Js::FunctionBody * jsBody = jsFunc->GetFunctionBody();
            const Js::CallInfo callInfo = walker.GetCallInfo();
            const char16_t* sourceFileName = _u("NULL");
            uint32_t line = 0; int32_t column = 0;
            walker.GetSourcePosition(&sourceFileName, &line, &column);

            StringCchPrintf(buffer, _countof(buffer), _u("%s [%s] (0x%p, Args=%u"), jsBody->GetDisplayName(), jsBody->GetDebugNumberSet(debugStringBuffer), jsFunc,
                callInfo.Count);
            sb.AppendSz(buffer);

            for (uint i = 0; i < callInfo.Count; i++)
            {
                StringCchPrintf(buffer, _countof(buffer), _u(", 0x%p"), walker.GetJavascriptArgs(false /*boxArgs*/)[i]);
                sb.AppendSz(buffer);
            }
            StringCchPrintf(buffer, _countof(buffer), _u(")[%s (%u, %d)]\n"), sourceFileName, line + 1, column + 1);
            sb.AppendSz(buffer);
            fc++;
            if(fc >= frameCount)
            {
                break;
            }
       }
    }
    sb.AppendCppLiteral(_u("----------------------------------------------------------------------\n"));
    char16_t* stack = sb.Detach();
    if(print)
    {
        Output::Print(stack);
    }
    return stack;
}
#endif
