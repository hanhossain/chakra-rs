//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#define ACTIVPROF_E_PROFILER_PRESENT        0x0200
#define ACTIVPROF_E_PROFILER_ABSENT         0x0201
#define ACTIVPROF_E_UNABLE_TO_APPLY_ACTION  0x0202
#define PROFILER_TOKEN uint

typedef enum {
    PROFILER_SCRIPT_TYPE_USER,
    PROFILER_SCRIPT_TYPE_DYNAMIC,
    PROFILER_SCRIPT_TYPE_NATIVE,
    PROFILER_SCRIPT_TYPE_DOM
} PROFILER_SCRIPT_TYPE;

typedef enum {
    PROFILER_EVENT_MASK_TRACE_SCRIPT_FUNCTION_CALL = 0x00000001,
    PROFILER_EVENT_MASK_TRACE_NATIVE_FUNCTION_CALL = 0x00000002,
    PROFILER_EVENT_MASK_TRACE_DOM_FUNCTION_CALL    = 0x00000004,
    PROFILER_EVENT_MASK_TRACE_ALL =
    PROFILER_EVENT_MASK_TRACE_SCRIPT_FUNCTION_CALL |
    PROFILER_EVENT_MASK_TRACE_NATIVE_FUNCTION_CALL,
    PROFILER_EVENT_MASK_TRACE_ALL_WITH_DOM = PROFILER_EVENT_MASK_TRACE_ALL |
    PROFILER_EVENT_MASK_TRACE_DOM_FUNCTION_CALL
} PROFILER_EVENT_MASK;

interface IEnumDebugCodeContexts : IUnknown
{
    // int32_t Next( ..

    // int32_t Skip( ..

    // int32_t Reset();

    // int32_t Clone( ..
};

interface IDebugDocumentInfo : IUnknown
{
    int32_t GetName(char* dn, BSTR *name);

    int32_t GetDocumentClassId(CLSID *dclsid);
};

interface IDebugDocument : IDebugDocumentInfo
{
};

interface IDebugDocumentContext : IUnknown
{
    int32_t GetDocument(IDebugDocument **doc);

    int32_t EnumCodeContexts(IEnumDebugCodeContexts **dctx);
};
