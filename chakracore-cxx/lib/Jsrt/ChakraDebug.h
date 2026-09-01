//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
/// \mainpage Chakra Hosting Debugging API Reference
///
/// Chakra is Microsoft's JavaScript engine. It is an integral part of Internet Explorer but can
/// also be hosted independently by other applications. This reference describes the APIs available
/// to applications to debug JavaScript.

/// \file
/// \brief The Chakra hosting debugging API.
///
/// This file contains a flat C API layer. This is the API exported by ChakraCore.dll.

#ifndef _CHAKRADEBUG_H_
#define _CHAKRADEBUG_H_

    /// <summary>
    ///     Debug events reported from ChakraCore engine.
    /// </summary>
    typedef enum _JsDiagDebugEvent
    {
        /// <summary>
        ///     Indicates a new script being compiled, this includes script, eval, new function.
        /// </summary>
        JsDiagDebugEventSourceCompile = 0,
        /// <summary>
        ///     Indicates compile error for a script.
        /// </summary>
        JsDiagDebugEventCompileError = 1,
        /// <summary>
        ///     Indicates a break due to a breakpoint.
        /// </summary>
        JsDiagDebugEventBreakpoint = 2,
        /// <summary>
        ///     Indicates a break after completion of step action.
        /// </summary>
        JsDiagDebugEventStepComplete = 3,
        /// <summary>
        ///     Indicates a break due to debugger statement.
        /// </summary>
        JsDiagDebugEventDebuggerStatement = 4,
        /// <summary>
        ///     Indicates a break due to async break.
        /// </summary>
        JsDiagDebugEventAsyncBreak = 5,
        /// <summary>
        ///     Indicates a break due to a runtime script exception.
        /// </summary>
        JsDiagDebugEventRuntimeException = 6
    } JsDiagDebugEvent;

    /// <summary>
    ///     Break on Exception attributes.
    /// </summary>
    typedef enum _JsDiagBreakOnExceptionAttributes
    {
        /// <summary>
        ///     Don't break on any exception.
        /// </summary>
        JsDiagBreakOnExceptionAttributeNone = 0x0,
        /// <summary>
        ///     Break on uncaught exception.
        /// </summary>
        JsDiagBreakOnExceptionAttributeUncaught = 0x1,
        /// <summary>
        ///     Break on first chance exception.
        /// </summary>
        JsDiagBreakOnExceptionAttributeFirstChance = 0x2
    } JsDiagBreakOnExceptionAttributes;

    /// <summary>
    ///     Stepping types.
    /// </summary>
    typedef enum _JsDiagStepType
    {
        /// <summary>
        ///     Perform a step operation to next statement.
        /// </summary>
        JsDiagStepTypeStepIn = 0,
        /// <summary>
        ///     Perform a step out from the current function.
        /// </summary>
        JsDiagStepTypeStepOut = 1,
        /// <summary>
        ///     Perform a single step over after a debug break if the next statement is a function call, else behaves as a stepin.
        /// </summary>
        JsDiagStepTypeStepOver = 2,
        /// <summary>
        ///     Perform a single step back to the previous statement (only applicable in TTD mode).
        /// </summary>
        JsDiagStepTypeStepBack = 3,
        /// <summary>
        ///     Perform a reverse continue operation (only applicable in TTD mode).
        /// </summary>
        JsDiagStepTypeReverseContinue = 4,
        /// <summary>
        ///     Perform a forward continue operation. Clears any existing step value.
        /// </summary>
        JsDiagStepTypeContinue = 5
    } JsDiagStepType;

    /// <summary>
    ///     User implemented callback routine for debug events.
    /// </summary>
    /// <remarks>
    ///     Use <c>JsDiagStartDebugging</c> to register the callback.
    /// </remarks>
    /// <param name="debugEvent">The type of JsDiagDebugEvent event.</param>
    /// <param name="eventData">Additional data related to the debug event.</param>
    /// <param name="callbackState">The state passed to <c>JsDiagStartDebugging</c>.</param>
    typedef void (* JsDiagDebugEventCallback)(_In_ JsDiagDebugEvent debugEvent, _In_ JsValueRef eventData, _In_opt_ void* callbackState);
#endif // _CHAKRADEBUG_H_
