//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DbgController.js.h"

class Debugger
{
public:
    static Debugger* GetDebugger(JsRuntimeHandle runtime);
    static void CloseDebugger();
    static JsRuntimeHandle GetRuntime();
    bool StartDebugging(JsRuntimeHandle runtime);
    bool StopDebugging(JsRuntimeHandle runtime);
    bool HandleDebugEvent(JsDiagDebugEvent debugEvent, JsValueRef eventData);
    bool CompareOrWriteBaselineFile(LPCSTR fileName);
    bool SourceRunDown();
    bool DumpFunctionPosition(JsValueRef functionPosition);
    bool IsDetached() const { return m_isDetached; }
private:
    Debugger(JsRuntimeHandle runtime);
    ~Debugger();
    bool Initialize();
    JsRuntimeHandle m_runtime;
    JsContextRef m_context;
    bool m_isDetached;
    bool InstallDebugCallbacks(JsValueRef hostDebugObject);
    bool SetBaseline();
    bool SetInspectMaxStringLength();
    bool CallFunction(char const * functionName, JsValueRef *result, JsValueRef arg1 = JS_INVALID_REFERENCE, JsValueRef arg2 = JS_INVALID_REFERENCE);
    bool CallFunctionNoResult(char const * functionName, JsValueRef arg1 = JS_INVALID_REFERENCE, JsValueRef arg2 = JS_INVALID_REFERENCE);
public:
    static void DebugEventHandler(_In_ JsDiagDebugEvent debugEvent, _In_ JsValueRef eventData, _In_opt_ void* callbackState);
    static JsValueRef GetSource(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef SetBreakpoint(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetStackTrace(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetBreakpoints(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef RemoveBreakpoint(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef SetBreakOnException(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetBreakOnException(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef SetStepType(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetScripts(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetStackProperties(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetProperties(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef GetObjectFromHandle(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);
    static JsValueRef Evaluate(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState);

    static Debugger* debugger;
};
