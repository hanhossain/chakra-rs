//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagStartDebugging(
    _In_ JsRuntimeHandle runtimeHandle,
    _In_ JsDiagDebugEventCallback debugEventCallback,
    _In_opt_ void* callbackState)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagStopDebugging(
    _In_ JsRuntimeHandle runtimeHandle,
    _Out_opt_ void** callbackState)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetScripts(
    _Out_ JsValueRef *scriptsArray)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetSource(
    _In_ unsigned int scriptId,
    _Out_ JsValueRef *source)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetBreakpoints(
    _Out_ JsValueRef *breakpoints)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagSetBreakpoint(
    _In_ unsigned int scriptId,
    _In_ unsigned int lineNumber,
    _In_ unsigned int columnNumber,
    _Out_ JsValueRef *breakpoint)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagRemoveBreakpoint(
    _In_ unsigned int breakpointId)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagSetBreakOnException(
    _In_ JsRuntimeHandle runtimeHandle,
    _In_ JsDiagBreakOnExceptionAttributes exceptionAttributes)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetBreakOnException(
    _In_ JsRuntimeHandle runtimeHandle,
    _Out_ JsDiagBreakOnExceptionAttributes* exceptionAttributes)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagSetStepType(
    _In_ JsDiagStepType stepType)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetFunctionPosition(
    _In_ JsValueRef function,
    _Out_ JsValueRef *functionPosition)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetStackTrace(
    _Out_ JsValueRef *stackTrace)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetStackProperties(
    _In_ unsigned int stackFrameIndex,
    _Out_ JsValueRef *properties)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetProperties(
    _In_ unsigned int objectHandle,
    _In_ unsigned int fromCount,
    _In_ unsigned int totalCount,
    _Out_ JsValueRef *propertiesObject)
{
    return JsErrorCategoryUsage;
}

// TODO (hanhossain): remove
JsErrorCode chakracore::jsrt::JsDiagGetObjectFromHandle(
    _In_ unsigned int objectHandle,
    _Out_ JsValueRef *handleObject)
{
    return JsErrorCategoryUsage;
}