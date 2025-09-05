//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Common/Jobs.h"
#include "Common/Event.h"

// This files contains the declarations of BGParseManager and BGParseWorkItem and build upon the Job and
// JobManager classes that do work on background threads. This enables the host to offload parser work
// from the UI and execution that doesn't have strict thread dependencies. Thus, both classes are
// multi-threaded; please see the definition of each function for expectations of which thread executes
// the function.
//
// There are up to 3 threads involved per background parse. Here are the relevant BGParseManager functions
// called from these three threads:
//
//      Background/Network          JobProcessor                UI/Executing 
//      Thread                      Thread                      Thread
//              |                       |                           |
//      QueueBackgroundParse            |                           |
//              |                       |                           |
//              |                   Process                         |
//              |                       |                           |
//              |                       |                       GetParseResults
//              .                       .                           .
// Note that the thread queueing the work can also be the UIT thread. Also, note that GetParseResults may
// block the calling thread until the JobProcessor thread finishes processing the BGParseWorkItem that
// contains the results.


// Forward Declarations
class BGParseWorkItem;
namespace Js
{
    struct Utf8SourceInfo;
    typedef void * Var;
    class JavascriptFunction;
}


// BGParseManager is the primary interface for background parsing. It uses a cookie to publicly track the data
// involved per parse request.
class BGParseManager sealed : public JsUtil::WaitableJobManager
{
public:
    BGParseManager();
    ~BGParseManager();

    static BGParseManager* GetBGParseManager();
    static void DeleteBGParseManager();
    static uint32_t GetNextCookie();
    static uint32_t IncCompleted();
    static uint32_t IncFailed();

    int32_t QueueBackgroundParse(LPCUTF8 pszSrc, size_t cbLength, char16_t *fullPath, uint32_t* dwBgParseCookie);
    int32_t GetInputFromCookie(uint32_t cookie, LPCUTF8* ppszSrc, size_t* pcbLength, char16_t** sourceUrl);
    int32_t GetParseResults(
        Js::ScriptContext* scriptContextUI,
        uint32_t cookie,
        LPCUTF8 pszSrc,
        SRCINFO const * pSrcInfo,
        Js::FunctionBody** ppFunc,
        CompileScriptException* pse,
        size_t& srcLength,
        Js::Utf8SourceInfo* utf8SourceInfo,
        uint& sourceIndex
    );
    bool DiscardParseResults(uint32_t cookie, void* buffer);

    virtual bool Process(JsUtil::Job *const job, JsUtil::ParallelThreadData *threadData) override;
    virtual void JobProcessed(JsUtil::Job *const job, const bool succeeded) override;
    virtual void JobProcessing(JsUtil::Job* job) override;

    // Defines needed for jobs.inl
    BGParseWorkItem* GetJob(BGParseWorkItem* workitem);
    bool WasAddedToJobProcessor(JsUtil::Job *const job) const;

private:
    BGParseWorkItem * FindJob(uint32_t dwCookie, bool waitForResults, bool removeJob);

    // BGParseWorkItem job can be in one of 3 states, based on which linked list it is in:
    // - queued - JobProcessor::jobs
    // - processing - BGParseManager::workitemsProcessing
    // - processed - BGParseManager::workitemsProcessed
    JsUtil::DoublyLinkedList<BGParseWorkItem> workitemsProcessing;
    JsUtil::DoublyLinkedList<BGParseWorkItem> workitemsProcessed;

    static uint32_t s_lastCookie;
    static uint32_t s_completed;
    static uint32_t s_failed;
    static BGParseManager* s_BGParseManager;
    static CriticalSection s_staticMemberLock;
};

// BGParseWorkItem is a helper class to BGParseManager that caches the input data from the calling thread
// to parse on the background thread and caches serialized bytecode so that bytecode can be deserialized
// on the appropriate thread.
class BGParseWorkItem sealed : public JsUtil::Job
{
public:
    BGParseWorkItem(
        BGParseManager* manager,
        const byte* script,
        size_t cb,
        char16_t *fullPath
    );
    ~BGParseWorkItem();

    void ParseUTF8Core(Js::ScriptContext* scriptContext);
    int32_t DeserializeParseResults(
        Js::ScriptContext* scriptContextUI,
        LPCUTF8 pszSrc,
        SRCINFO const * pSrcInfo,
        Js::Utf8SourceInfo* utf8SourceInfo,
        Js::FunctionBody** functionBodyReturn,
        size_t& srcLength,
        uint& sourceIndex
    );
    void TransferCSE(CompileScriptException* pse);

    void CreateCompletionEvent();
    void WaitForCompletion();
    void JobProcessed(const bool succeeded);

    void Discard() { discarded = true; }
    bool IsDiscarded() const { return discarded; }

    uint32_t GetCookie() const { return cookie; }
    const byte* GetScriptSrc() const { return script; }
    size_t GetScriptLength() const { return cb; }
    char16_t* GetScriptPath() const { return path; }

private:
    // This cookie is the public identifier for this parser work
    uint32_t cookie;

    // Input data
    const byte* script;
    size_t cb;
    BSTR path;

    // Parse state
    CompileScriptException cse;
    int32_t parseHR;
    size_t parseSourceLength;
    Event* complete;
    
    // True when this workitem was discarded while processing. This instance
    // will free itself after it has been processed.
    bool discarded;

    // Output data
    byte * bufferReturn;
    uint32_t  bufferReturnBytes;
};
