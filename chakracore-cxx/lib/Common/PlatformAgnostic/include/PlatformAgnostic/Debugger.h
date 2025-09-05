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

class IActiveScriptProfilerCallback
{
public:
  int32_t Initialize(uint32_t ctx)
  {
      return S_OK;
  }

  int32_t Shutdown(int32_t _)
  {
      return S_OK;
  }

  int32_t Release()
  {
      return S_OK;
  }

  int32_t QueryInterface(IActiveScriptProfilerCallback **_)
  {
      return S_OK;
  }

  int32_t ScriptCompiled(PROFILER_TOKEN scriptId, PROFILER_SCRIPT_TYPE type, IUnknown *ctx)
  {
      return S_OK;
  }

  int32_t FunctionCompiled(PROFILER_TOKEN functionId, PROFILER_TOKEN scriptId,
      const char16_t* pwszFunctionName, const char16_t* pwszFunctionNameHint, IUnknown *ctx)
  {
      return S_OK;
  }

  int32_t OnFunctionEnter(PROFILER_TOKEN scriptId, PROFILER_TOKEN functionId)
  {
      return S_OK;
  }

  int32_t OnFunctionExit(PROFILER_TOKEN scriptId, PROFILER_TOKEN functionId)
  {
      return S_OK;
  }

  // IActiveScriptProfilerCallback2
  int32_t OnFunctionEnterByName(const char16_t *functionName, PROFILER_SCRIPT_TYPE _)
  {
      return S_OK;
  }

  int32_t OnFunctionExitByName(const char16_t *functionName, PROFILER_SCRIPT_TYPE _)
  {
      return S_OK;
  }

  // IActiveScriptProfilerCallback3
  int32_t AddRef()
  {
      return S_OK;
  }

  int32_t SetWebWorkerId(PROFILER_TOKEN _)
  {
      return S_OK;
  }
};

#define IActiveScriptProfilerCallback2 IActiveScriptProfilerCallback
#define IActiveScriptProfilerCallback3 IActiveScriptProfilerCallback
