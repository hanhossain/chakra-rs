//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"
#include "Output.h"

// Initialization order
//  AB AutoSystemInfo
//  AD PerfCounter
//  AE PerfCounterSet
//  AM Output/Configuration
//  AN MemProtectHeap
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AR LeakReport
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAM")

bool                Output::s_useDebuggerWindow = false;
CriticalSection     Output::s_critsect;
AutoFILE            Output::s_outputFile; // Create a separate output file that is not thread-local.
#ifdef ENABLE_TRACE
Js::ILogger*        Output::s_inMemoryLogger = nullptr;
#ifdef STACK_BACK_TRACE
Js::IStackTraceHelper* Output::s_stackTraceHelper = nullptr;
#endif
unsigned int Output::s_traceEntryId = 0;
#endif

thread_local FILE*    Output::s_file = nullptr;
thread_local size_t   Output::s_Column  = 0;
thread_local uint16_t     Output::s_color = 0;
thread_local bool     Output::s_hasColor = false;
thread_local bool     Output::s_capture = false;

thread_local bool     Output::hasDoneAlignPrefixForThisLine = false;
thread_local bool     Output::usingCustomAlignAndPrefix = false;
thread_local size_t   Output::align = 0;
thread_local const char16_t* Output::prefix = nullptr;

#define MAX_OUTPUT_BUFFER_SIZE 10 * 1024 * 1024  // 10 MB maximum before we force a flush

size_t
Output::VerboseNote(const char16_t * format, ...)
{
#ifdef ENABLE_TRACE
    if (Js::Configuration::Global.flags.Verbose)
    {
        AutoCriticalSection autocs(&s_critsect);
        va_list argptr;
        va_start(argptr, format);
        size_t size = vfwprintf(stdout, format, argptr);
        fflush(stdout);
        va_end(argptr);
        return size;
    }
#endif
    return 0;
}

#ifdef ENABLE_TRACE
size_t
Output::Trace(Js::Phase phase, const char16_t *form, ...)
{
    size_t retValue = 0;

    if(Js::Configuration::Global.flags.Trace.IsEnabled(phase))
    {
        va_list argptr;
        va_start(argptr, form);
        retValue += Output::VTrace(u"%s: ", Js::PhaseNames[static_cast<int>(phase)], form, argptr);
        va_end(argptr);
    }

    return retValue;
}

size_t
Output::Trace2(Js::Phase phase, const char16_t *form, ...)
{
    size_t retValue = 0;

    if (Js::Configuration::Global.flags.Trace.IsEnabled(phase))
    {
        va_list argptr;
        va_start(argptr, form);
        retValue += Output::VPrint(form, argptr);
        va_end(argptr);
    }

    return retValue;
}

size_t
Output::TraceWithPrefix(Js::Phase phase, const char16_t prefix[], const char16_t *form, ...)
{
    size_t retValue = 0;

    if (Js::Configuration::Global.flags.Trace.IsEnabled(phase))
    {
        va_list argptr;
        va_start(argptr, form);
        char16_t prefixValue[512];
        _snwprintf_s(prefixValue, _countof(prefixValue), _TRUNCATE, u"%s: %s: ", Js::PhaseNames[static_cast<int>(phase)], prefix);
        retValue += Output::VTrace(u"%s", prefixValue, form, argptr);
        va_end(argptr);
    }

    return retValue;
}

size_t
Output::TraceWithFlush(Js::Phase phase, const char16_t *form, ...)
{
    size_t retValue = 0;

    if(Js::Configuration::Global.flags.Trace.IsEnabled(phase))
    {
        va_list argptr;
        va_start(argptr, form);
        retValue += Output::VTrace(u"%s:", Js::PhaseNames[static_cast<int>(phase)], form, argptr);
        Output::Flush();
        va_end(argptr);
    }

    return retValue;
}

size_t
Output::TraceWithFlush(Js::Flag flag, const char16_t *form, ...)
{
    size_t retValue = 0;

    if (Js::Configuration::Global.flags.IsEnabled(flag))
    {
        va_list argptr;
        va_start(argptr, form);
        retValue += Output::VTrace(u"[-%s]::", Js::FlagNames[static_cast<int>(flag)], form, argptr);
        Output::Flush();
        va_end(argptr);
    }

    return retValue;
}

size_t
Output::VTrace(const char16_t* shortPrefixFormat, const char16_t* prefix, const char16_t *form, va_list argptr)
{
    size_t retValue = 0;

    retValue += Output::Print(shortPrefixFormat, prefix);
    retValue += Output::VPrint(form, argptr);

#ifdef STACK_BACK_TRACE
    // Print stack trace.
    if (s_stackTraceHelper)
    {
        const uint32_t c_framesToSkip = 2; // Skip 2 frames -- Output::VTrace and Output::Trace.
        const uint32_t c_frameCount = 10;  // TODO: make it configurable.
        const char16_t callStackPrefix[] = u"call stack:";
        if (s_inMemoryLogger)
        {
            // Trace just addresses of functions, avoid symbol info as it takes too much memory.
            // One line for whole stack trace for easier parsing on the jd side.
            const size_t c_msgCharCount = _countof(callStackPrefix) + (1 + sizeof(void*) * 2) * c_frameCount; // 2 hexadecimal digits per byte + 1 for space.
            char16_t callStackMsg[c_msgCharCount];
            void* frames[c_frameCount];
            size_t start = 0;
            size_t temp;

            temp = _snwprintf_s(callStackMsg, _countof(callStackMsg), _TRUNCATE, u"%s", callStackPrefix);
            Assert(temp != -1);
            start += temp;

            uint32_t framesObtained = s_stackTraceHelper->GetStackTrace(c_framesToSkip, c_frameCount, frames);
            Assert(framesObtained <= c_frameCount);
            for (uint32_t i = 0; i < framesObtained && i < c_frameCount; ++i)
            {
                Assert(_countof(callStackMsg) >= start);
                temp = _snwprintf_s(callStackMsg + start, _countof(callStackMsg) - start, _TRUNCATE, u" %p", frames[i]);
                Assert(temp != -1);
                start += temp;
            }

            retValue += Output::Print(u"%s\n", callStackMsg);
        }
        else
        {
            // Trace with full symbol info.
            retValue += Output::Print(u"%s\n", callStackPrefix);
            retValue += s_stackTraceHelper->PrintStackTrace(c_framesToSkip, c_frameCount);
        }
    }
#endif

    return retValue;
}

#ifdef BGJIT_STATS
size_t
Output::TraceStats(Js::Phase phase, const char16_t *form, ...)
{
    if(PHASE_STATS1(phase))
    {
        va_list argptr;
        va_start(argptr, form);
        size_t ret_val = Output::VPrint(form, argptr);
        va_end(argptr);
        return ret_val;
    }
    return 0;
}
#endif
#endif // ENABLE_TRACE

///----------------------------------------------------------------------------
///
/// Output::Print
///
///     Print the given format string.
///
///
///----------------------------------------------------------------------------

size_t
Output::Print(const char16_t *form, ...)
{
    va_list argptr;
    va_start(argptr, form);
    size_t ret_val = Output::VPrint(form, argptr);
    va_end(argptr);
    return ret_val;
}

size_t
Output::Print(int column, const char16_t *form, ...)
{
    Output::SkipToColumn(column);
    va_list argptr;
    va_start(argptr, form);
    size_t ret_val = Output::VPrint(form, argptr);
    va_end(argptr);
    return ret_val;
}

size_t
Output::VPrint(const char16_t *form, va_list argptr)
{
    char16_t buf[2048];
    size_t size;

    size = _vsnwprintf_s(buf, _countof(buf), _TRUNCATE, form, argptr);
    if(size == -1)
    {
        size = _countof(buf) - 1;  // characters written, excluding the terminating null
    }
    return Output::PrintBuffer(buf, size);
}

//
// buf: a null terminated string
// size: characters in buf, excluding the terminating null ==> wcslen(buf)
//
size_t
Output::PrintBuffer(const char16_t * buf, size_t size)
{
    // Handle custom line prefixing
    bool internallyAllocatedBuffer = false;
    if (usingCustomAlignAndPrefix)
    {
        if (hasDoneAlignPrefixForThisLine && wcschr(buf, '\n') == nullptr)
        {
            // no newlines, and we've already prefixed this line, so nothing to do
        }
        else
        {
            size_t newbufsize = size + align;
            char16_t* newbuf = (char16_t*)calloc(newbufsize, sizeof(char16_t));
            AssertOrFailFastMsg(newbuf != nullptr, "Ran out of memory while printing output");
            internallyAllocatedBuffer = true;
            const char16_t* currentReadIndex = buf;
            char16_t* currentWriteIndex = newbuf;
            auto ensureSpace = [&currentWriteIndex, &newbuf, &newbufsize](size_t numCharsWantToWrite)
            {
                size_t charsWritten = (currentWriteIndex - newbuf); // pointer subtraction is number of elements of pointed type between pointers
                size_t remaining = newbufsize - charsWritten;
                if (numCharsWantToWrite + 1 > remaining)
                {
                    char16_t* tempbuf = (char16_t*)realloc(newbuf, newbufsize * sizeof(char16_t) * 2);
                    AssertOrFailFastMsg(tempbuf != nullptr, "Ran out of memory while printing output");
                    newbuf = tempbuf;
                    newbufsize = newbufsize * 2;
                    currentWriteIndex = newbuf + charsWritten;
                }
            };
            const size_t prefixlength = wcslen(prefix);
            size_t oldS_Column = Output::s_Column;
            while (currentReadIndex < buf + size)
            {
                if (!hasDoneAlignPrefixForThisLine)
                {
                    // attempt to write the alignment
                    {
                        unsigned int alignspacesneeded = 1; // always put at least one space
                        if (oldS_Column < align)
                        {
                            alignspacesneeded = (unsigned int)(align - oldS_Column);
                        }
                        ensureSpace(alignspacesneeded);
                        for (unsigned int i = 0; i < alignspacesneeded; i++)
                        {
                            *(currentWriteIndex++) = ' ';
                        }
                    }
                    // attempt to write the prefix
                    ensureSpace(prefixlength);
                    js_wmemcpy_s(currentWriteIndex, (newbuf + newbufsize) - currentWriteIndex, Output::prefix, prefixlength);
                    currentWriteIndex += prefixlength;
                    oldS_Column = align + prefixlength;
                    hasDoneAlignPrefixForThisLine = true;
                }
                const char16_t* endOfLine = wcschr(currentReadIndex, '\n');
                size_t charsToCopy = 0;
                if (endOfLine != nullptr)
                {
                    charsToCopy = (endOfLine - currentReadIndex) + 1; // We want to grab the newline character as part of this line
                    oldS_Column = 0; // We're ending this line, and want the next to be calculated properly
                    hasDoneAlignPrefixForThisLine = false; // The next line will need this
                }
                else
                {
                    charsToCopy = (buf + size) - currentReadIndex; // the rest of the input buffer
                    oldS_Column += charsToCopy; // Will be reset anyway later on
                }
                ensureSpace(endOfLine - currentReadIndex);
                js_wmemcpy_s(currentWriteIndex, (newbuf + newbufsize) - currentWriteIndex, currentReadIndex, charsToCopy);
                currentReadIndex += charsToCopy;
                currentWriteIndex += charsToCopy;
            }
            // null terminate becuase there's no real reason not to
            ensureSpace(1);
            *(currentWriteIndex++) = '\0';
            buf = newbuf;
            size = (currentWriteIndex - newbuf) - 1; // not counting the terminator here though, to align with vsnwprintf_s's behavior
        }
    }
    Output::s_Column += size;
    const char16_t * endbuf = wcschr(buf, '\n');
    while (endbuf != nullptr)
    {
        Output::s_Column = size - (endbuf - buf) - 1;
        endbuf = wcschr(endbuf + 1, '\n');
    }

    bool useConsoleOrFile = true;
    if (!Output::s_capture)
    {
        if (Output::s_useDebuggerWindow)
        {
            OutputDebugStringW(buf);
            useConsoleOrFile = false;
        }
#ifdef ENABLE_TRACE
        if (Output::s_inMemoryLogger)
        {
            s_inMemoryLogger->Write(buf);
            useConsoleOrFile = false;
        }
#endif
    }

    if (useConsoleOrFile)
    {
        if (s_file == nullptr || Output::s_capture)
        {
            DirectPrint(buf);
        }
        else
        {
            fwprintf_s(Output::s_file, u"%s", buf);
        }

        if(s_outputFile != nullptr && !Output::s_capture)
        {
            fwprintf_s(s_outputFile, u"%s", buf);
        }
    }

    Output::Flush();

    return size;
}

void Output::Flush()
{
    if (s_capture)
    {
        return;
    }
    if(s_outputFile != nullptr)
    {
        fflush(s_outputFile);
    }
    fflush(NULL);
}

void Output::DirectPrint(char16_t const * string)
{
    AutoCriticalSection autocs(&s_critsect);

    fwprintf(stdout, u"%s", string);
}
///----------------------------------------------------------------------------
///
/// Output::SkipToColumn
///
///     Inserts spaces up to the column passed in.
///
///----------------------------------------------------------------------------

void
Output::SkipToColumn(size_t column)
{
    size_t columnbias = 0;
    // If we're using a custom alignment and prefix, we want to do this relative to that
    if (usingCustomAlignAndPrefix)
    {
        // If we've already added the alignment and prefix, we need to add the alignment to our column number here
        columnbias = align + wcslen(prefix);
    }
    size_t reference = 0;
    if (Output::s_Column > columnbias)
    {
        reference = Output::s_Column - columnbias;
    }
    if (column <= reference)
    {
        Output::Print(u" ");
        return;
    }

    // compute distance to our destination

    size_t dist = column - reference;

    // Print at least one space
    while (dist > 0)
    {
        Output::Print(u" ");
        dist--;
    }
}

FILE*
Output::GetFile()
{
    return Output::s_file;
}

FILE*
Output::SetFile(FILE *file)
{
    Output::Flush();
    FILE *oldfile = Output::s_file;
    Output::s_file = file;
    return oldfile;
}

void
Output::SetOutputFile(FILE* file)
{
    if(s_outputFile != nullptr)
    {
        AssertMsg(false, "Output file is being set twice.");
    }
    else
    {
        s_outputFile = file;
    }
}

FILE*
Output::GetOutputFile()
{
    return s_outputFile;
}

#ifdef ENABLE_TRACE
void
Output::SetInMemoryLogger(Js::ILogger* logger)
{
    AssertMsg(s_inMemoryLogger == nullptr, "This cannot be called more than once.");
    s_inMemoryLogger = logger;
}

#ifdef STACK_BACK_TRACE
void
Output::SetStackTraceHelper(Js::IStackTraceHelper* helper)
{
    AssertMsg(s_stackTraceHelper == nullptr, "This cannot be called more than once.");
    s_stackTraceHelper = helper;
}
#endif

#endif // ENABLE_TRACE

//
// Sets the foreground color and returns the old color. Returns 0 on failure
//

uint16_t
Output::SetConsoleForeground(uint16_t color)
{
    AutoCriticalSection autocs(&s_critsect);
    return 0;
}

void
Output::CaptureStart()
{
    Assert(!s_capture);
    Output::Flush();
    s_capture = true;
}

char16_t *
Output::CaptureEnd()
{
    Assert(s_capture);
    s_capture = false;
    char16_t * returnBuffer = nullptr;
    return returnBuffer;
}

void
Output::SetAlignAndPrefix(unsigned int align, const char16_t *prefix)
{
    Output::hasDoneAlignPrefixForThisLine = false;
    Output::usingCustomAlignAndPrefix = true;
    Output::prefix = prefix;
    Output::align = align;
}
void
Output::ResetAlignAndPrefix()
{
    Output::hasDoneAlignPrefixForThisLine = false;
    Output::usingCustomAlignAndPrefix = false;
    Output::prefix = nullptr;
    Output::align = 0;
}
