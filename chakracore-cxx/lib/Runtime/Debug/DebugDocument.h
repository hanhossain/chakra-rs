//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

struct StatementSpan
{
    int32_t ich;
    int32_t cch;
};

// A Document in Engine means a file, eval code or new function code. For each of these there is a Utf8SourceInfo.
// DebugDocument relates debug operations such as adding/remove breakpoints to a specific Utf8SourceInfo.

namespace Js
{
    class DebugDocument
    {
    public:
        DebugDocument(Utf8SourceInfo* utf8SourceInfo, Js::FunctionBody* functionBody);
        ~DebugDocument();
        virtual void CloseDocument();

        int32_t SetBreakPoint(int32_t ibos, BREAKPOINT_STATE bps);
        BreakpointProbe* SetBreakPoint(StatementLocation statement, BREAKPOINT_STATE bps);
        void RemoveBreakpointProbe(BreakpointProbe *probe);
        void ClearAllBreakPoints(void);

#if ENABLE_TTD
        BreakpointProbe* SetBreakPoint_TTDWbpId(long bpId, StatementLocation statement);
#endif

        BreakpointProbe* FindBreakpoint(StatementLocation statement);
        bool FindBPStatementLocation(uint32_t bpId, StatementLocation * statement);

        BOOL GetStatementSpan(int32_t ibos, StatementSpan* pBos);
        BOOL GetStatementLocation(int32_t ibos, StatementLocation* plocation);

        virtual bool HasDocumentText() const
        {
            Assert(false);
            return false;
        }
        virtual void* GetDocumentText() const
        {
            Assert(false);
            return nullptr;
        };

        Js::FunctionBody * GetFunctionBodyAt(int32_t ibos);

        Utf8SourceInfo* GetUtf8SourceInfo() { return this->utf8SourceInfo; }

    private:
        Utf8SourceInfo* utf8SourceInfo;
        RecyclerRootPtr<Js::FunctionBody> functionBody;
        BreakpointProbeList* m_breakpointList;

        BreakpointProbeList* NewBreakpointList(ArenaAllocator* arena);
        BreakpointProbeList* GetBreakpointList();

        BOOL HasLineBreak(int32_t _start, int32_t _end);
    };
}
