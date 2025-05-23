//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef STACK_BACK_TRACE

#include <execinfo.h>
#define CaptureStackBackTrace(FramesToSkip, FramesToCapture, buffer, hash) \
    backtrace(buffer, FramesToCapture)

class StackBackTrace
{
public:
    static const uint32_t DefaultFramesToCapture = 30;
    static StackBackTrace * Capture(char* buffer, size_t bufSize, uint32_t framesToSkip = 0);
    template <typename TAllocator>
        static StackBackTrace * Capture(TAllocator * alloc, uint32_t framesToSkip = 0, uint32_t framesToCapture = DefaultFramesToCapture);

    template <typename TAllocator>
        static StackBackTrace * Create(TAllocator * alloc, uint32_t framesToCaptureLater = DefaultFramesToCapture);
    size_t Print();
    template<typename Fn>void Map(Fn fn);   // The Fn is expected to be: void Fn(void*).
    uint32_t Capture(uint32_t framesToSkip);
    uint32_t GetRequestedFrameCount() { return this->requestedFramesToCapture; }
    template <typename TAllocator>
    void Delete(TAllocator * alloc);
private:
    // We want to skip at lease the StackBackTrace::Capture and the constructor frames
    static const uint32_t BaseFramesToSkip = 2;

    StackBackTrace(uint32_t framesToSkip, uint32_t framesToCapture);
    StackBackTrace(uint32_t framesToCapture);

    uint32_t requestedFramesToCapture;
    uint32_t framesCount;
    void * stackBackTrace[];
};

template <typename TAllocator>
StackBackTrace *
StackBackTrace::Capture(TAllocator * alloc, uint32_t framesToSkip, uint32_t framesToCapture)
{
    return AllocatorNewPlusZ(TAllocator, alloc, sizeof(void *) * framesToCapture, StackBackTrace, framesToSkip, framesToCapture);
}

template <typename TAllocator>
StackBackTrace* StackBackTrace::Create(TAllocator * alloc, uint32_t framesToCaptureLater)
{
    return AllocatorNewPlusZ(TAllocator, alloc, sizeof(void *)* framesToCaptureLater, StackBackTrace, framesToCaptureLater);
}

template <typename TAllocator>
void StackBackTrace::Delete(TAllocator * alloc)
{
    AllocatorDeletePlus(TAllocator, alloc, sizeof(void *)* requestedFramesToCapture, this);
}

template <typename Fn>
void StackBackTrace::Map(Fn fn)
{
    for (uint32_t i = 0; i < this->framesCount; ++i)
    {
        fn(this->stackBackTrace[i]);
    }
}

class StackBackTraceNode
{
public:
    template <typename TAllocator>
    static void Prepend(TAllocator * allocator, StackBackTraceNode *& head, StackBackTrace * stackBackTrace)
    {
        head = AllocatorNew(TAllocator, allocator, StackBackTraceNode, stackBackTrace, head);
    }

    template <typename TAllocator>
    static void DeleteAll(TAllocator * allocator, StackBackTraceNode *& head)
    {
        StackBackTraceNode * curr = head;
        while (curr != nullptr)
        {
            StackBackTraceNode * next = curr->next;
            curr->stackBackTrace->Delete(allocator);
            AllocatorDelete(TAllocator, allocator, curr);
            curr = next;
        }
        head = nullptr;
    }

    static void PrintAll(StackBackTraceNode * head)
    {
        // We want to print them tail first because that is the first stack trace we added

        // Reverse the list
        StackBackTraceNode * curr = head;
        StackBackTraceNode * prev = nullptr;
        while (curr != nullptr)
        {
            StackBackTraceNode * next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        // print and reverse again.
        curr = prev;
        prev = nullptr;
        while (curr != nullptr)
        {
            curr->stackBackTrace->Print();
            StackBackTraceNode * next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        Assert(prev == head);
    }
private:
    StackBackTraceNode(StackBackTrace * stackBackTrace, StackBackTraceNode * next) : stackBackTrace(stackBackTrace), next(next) {};
    StackBackTrace * stackBackTrace;
    StackBackTraceNode * next;
};


//
// In memory TraceRing
//

uint _trace_ring_next_id();

//
// A buffer of size "T[count]", dynamically allocated (!useStatic) or
// statically embedded (useStatic).
//
template <class T, int32_t count, bool useStatic>
struct _TraceRingBuffer
{
    T* buf;
    _TraceRingBuffer() { buf = HeapNewArray(T, count); }
    ~_TraceRingBuffer() { HeapDeleteArray(count, buf); }
};
template <class T, int32_t count>
struct _TraceRingBuffer<T, count, true>
{
    T buf[count];
};

//
// A trace ring frame, consisting of id, header, and optionally stack
//
template <class Header, uint STACK_FRAMES>
struct _TraceRingFrame
{
    uint id;  // unique id in order
    Header header;
    void* stack[STACK_FRAMES];
};

//
// Trace code execution using an in-memory ring buffer. Capture each trace
// point with a frame, which contains of custom data and optional stack trace.
// Useful for instrumenting source code to track execution.
//
//  Header:     Custom header data type to capture in each frame. Used to
//              capture execution state at a trace point.
//  COUNT:      Number of frames to keep in the ring buffer. When the buffer
//              is filled up, capture will start over from the beginning.
//  STACK_FRAMES:
//              Number of stack frames to capture for each trace frame.
//              This can be 0, only captures header data without stack trace.
//  USE_STATIC_BUFFER:
//              Use embedded buffer instead of dynamically allocate. This may
//              be helpful to avoid static data initialization problem.
//  SKIP_TOP_FRAMES:
//              Top stack frames to skip for each capture (only on windows).
//
//  Usage: Following captures the last 100 stacks that changes
//  scriptContext->debuggerMode:
//          struct DebugModeChange {
//              ScriptContext* scriptContext;
//              DebuggerMode debuggerMode;
//          };
//          static TraceRing<DebugModeChange, 100> s_ev;
//      Call at every debuggerMode change point:
//          DebugModeChange e = { scriptContext, debuggerMode };
//          s_ev.Capture(e);
//
//  Examine trace frame i with its call stack in debugger:
//  gdb:
//      p s_ev.buf[i]
//  windbg:
//      ?? &s_ev.buf[i]
//      dds/dqs [above address]
//
template <class Header, int32_t COUNT,
          uint STACK_FRAMES = 30,
          bool USE_STATIC_BUFFER = false,
          uint SKIP_TOP_FRAMES = 1>
class TraceRing:
    protected _TraceRingBuffer<_TraceRingFrame<Header, STACK_FRAMES>, COUNT, USE_STATIC_BUFFER>
{
protected:
    int32_t cur;

public:
    TraceRing()
    {
        cur = (uint)-1;
    }

    template <class HeaderFunc>
    void Capture(const HeaderFunc& writeHeader)
    {
        int32_t i = InterlockedIncrement(&cur);
        if (i >= COUNT)
        {
            InterlockedCompareExchange(&cur, i % COUNT, i);
            i %= COUNT;
        }

        auto* frame = &this->buf[i];
        *frame = {};
        frame->id = _trace_ring_next_id();
        writeHeader(&frame->header);
        if (STACK_FRAMES > 0)
        {
            CaptureStackBackTrace(SKIP_TOP_FRAMES, STACK_FRAMES, frame->stack, nullptr);
        }
    }

    void Capture(const Header& header)
    {
        Capture([&](Header* h)
        {
            *h = header;
        });
    }

    // Capture a trace (no header data, stack only)
    void Capture()
    {
        Capture([&](Header* h) { });
    }
};

#endif  // STACK_BACK_TRACE
