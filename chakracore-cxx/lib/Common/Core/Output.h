//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Core/AutoFile.h"

// xplat-todo: error: ISO C++ forbids forward references to 'enum' types
#if defined(ENABLE_TRACE) 
namespace Js
{
enum Flag: unsigned short;
enum Phase: unsigned short;
};
#endif

#if defined(ENABLE_DEBUG_CONFIG_OPTIONS) && defined(BGJIT_STATS)
#define OUTPUT_TRACE(Phase, ...) Output::Trace((Phase), __VA_ARGS__)
#define OUTPUT_TRACE_2(Phase, ...) Output::Trace2((Phase), __VA_ARGS__)
#define OUTPUT_VERBOSE_TRACE(Phase, ...) \
    if(Js::Configuration::Global.flags.Verbose) \
    { \
        OUTPUT_TRACE((Phase), __VA_ARGS__); \
    }
#define OUTPUT_STATS(Phase, ...) Output::TraceStats((Phase), __VA_ARGS__)
#define OUTPUT_VERBOSE_STATS(Phase, ...) \
    if(Js::Configuration::Global.flags.Verbose) \
    { \
        OUTPUT_STATS((Phase), __VA_ARGS__); \
    }
#define OUTPUT_FLUSH() Output::Flush()
#else
#define OUTPUT_TRACE(Phase, ...)
#define OUTPUT_TRACE_2(Phase, ...)
#define OUTPUT_VERBOSE_TRACE(Phase, ...)
#define OUTPUT_STATS(Phase, ...)
#define OUTPUT_VERBOSE_STATS(Phase, ...)
#define OUTPUT_FLUSH()
#endif

#if DBG
#define OUTPUT_TRACE_DEBUGONLY_ENABLED 1
#define OUTPUT_TRACE_DEBUGONLY(Phase, ...) Output::TraceWithFlush((Phase), __VA_ARGS__)
#define OUTPUT_TRACE_DEBUGONLY(Flag, ...) Output::TraceWithFlush((Flag), __VA_ARGS__)
#else
#define OUTPUT_TRACE_DEBUGONLY(Phase, ...)
#define OUTPUT_TRACE_FLAG_DEBUGONLY(Flag, ...)
#endif

namespace Js
{
    // Logging interfaces:
    // decouple implementation so that in common.lib we don't have dependency on memory.lib
    struct ILogger
    {
        virtual void Write(const char16* msg) = 0;
    };

#ifdef STACK_BACK_TRACE
    struct IStackTraceHelper
    {
        virtual size_t PrintStackTrace(uint32_t framesToSkip, uint32_t framesToCapture) = 0;  // Returns # of chars printed.
        virtual uint32_t GetStackTrace(uint32_t framesToSkip, uint32_t framesToCapture, void** stackFrames) = 0; // Returns # of frames captured.
    };
#endif
} // namespace Js.


class Output
{
public:
    static size_t VerboseNote(const char16 * format, ...);
#ifdef ENABLE_TRACE
    static size_t Trace(Js::Phase phase, const char16 *form, ...);
    static size_t Trace2(Js::Phase phase, const char16 *form, ...);
    static size_t TraceWithPrefix(Js::Phase phase, const char16 prefix[], const char16 *form, ...);
    static size_t TraceWithFlush(Js::Phase phase, const char16 *form, ...);
    static size_t TraceWithFlush(Js::Flag flag, const char16 *form, ...);
    static size_t TraceStats(Js::Phase phase, const char16 *form, ...);
    template<class Fn>
    static size_t
    TraceWithCallback(Js::Phase phase, Fn callback, const char16 *form, ...)
    {
        size_t retValue = 0;

        if(Js::Configuration::Global.flags.Trace.IsEnabled(phase))
        {
            va_list argptr;
            va_start(argptr, form);
            retValue = Output::Print(_u("%s:"), Js::PhaseNames[static_cast<int>(phase)]);
            retValue += Output::VPrint(form, argptr);
            retValue += Output::Print(_u("%s"), callback());
            va_end(argptr);
        }

        return retValue;
    }    
    static void     SetInMemoryLogger(Js::ILogger* logger);
#ifdef STACK_BACK_TRACE
    static void     SetStackTraceHelper(Js::IStackTraceHelper* helper);
#endif
    
#endif // ENABLE_TRACE
    static size_t Print(const char16 *form, ...);
    static size_t Print(int column, const char16 *form, ...);
    static size_t PrintBuffer(const char16 * buffer, size_t size);
    static size_t VPrint(const char16 *form, va_list argptr);
    static void     SkipToColumn(size_t column);
    static FILE*    SetFile(FILE *);
    static FILE*    GetFile();
    static void     SetOutputFile(FILE *);
    static FILE*    GetOutputFile();
    static void     UseDebuggerWindow() { s_useDebuggerWindow = true; }
    static void     Flush();

    static uint16_t     SetConsoleForeground(uint16_t color);
    static void     CaptureStart();
    static char16*  CaptureEnd();

    static void     SetAlignAndPrefix(unsigned int align, const char16 *prefix);
    static void     ResetAlignAndPrefix();

private:
    static void     DirectPrint(const char16 * string);

    static AutoFILE s_outputFile;
    static bool     s_useDebuggerWindow;
    static CriticalSection s_critsect;

#ifdef ENABLE_TRACE
    static Js::ILogger* s_inMemoryLogger;       // Used to trace into memory so that when process crashes, you can see tracing in crash dump file.
    static unsigned int s_traceEntryId;         // Sequential id of trace entry for rich output format.
#ifdef STACK_BACK_TRACE
    static Js::IStackTraceHelper* s_stackTraceHelper; // Used for capturing stack trace.
#endif
    static size_t VTrace(const char16* shortPrefixFormat, const char16* prefix, const char16 *form, va_list argptr);
#endif // ENABLE_TRACE

#define THREAD_ST THREAD_LOCAL

    THREAD_ST static bool hasDoneAlignPrefixForThisLine;
    THREAD_ST static bool usingCustomAlignAndPrefix;
    THREAD_ST static const char16* prefix;
    THREAD_ST static size_t align;

    THREAD_ST static bool s_capture;
    THREAD_ST static FILE * s_file;
    THREAD_ST static size_t s_Column;
    THREAD_ST static uint16_t s_color;
    THREAD_ST static bool s_hasColor;
};
