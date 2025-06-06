//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef ENABLE_FOUNDATION_OBJECT

#include "Windows.Foundation.Diagnostics.h"
using namespace Windows::Foundation::Diagnostics;

namespace Js
{
    class WindowsFoundationAdapter
    {
    private:
        AutoCOMPtr<IActivationFactory> asyncCausalityTracerActivationFactory;
        AutoCOMPtr<IAsyncCausalityTracerStatics> asyncCausalityTracerStatics;

        IActivationFactory* GetAsyncCausalityTracerActivationFactory(_In_ ScriptContext* scriptContext);
        IAsyncCausalityTracerStatics* GetAsyncCausalityTracerStatics(_In_ ScriptContext* scriptContext);
        DelayLoadWinRtString* GetWinRtStringLibrary(_In_ ScriptContext* scriptContext);
        DelayLoadWinRtFoundation* GetWinRtFoundationLibrary(_In_ ScriptContext* scriptContext);

    public:
        HRESULT TraceOperationCreation(
            _In_ ScriptContext* scriptContext,
            _In_ int32_t traceLevel,
            _In_ int32_t source,
            _In_ GUID platformId,
            _In_ UINT64 operationId,
            _In_z_ PCWSTR operationName,
            _In_ UINT64 relatedContext);

        HRESULT TraceOperationCompletion(
            _In_ ScriptContext* scriptContext,
            _In_ int32_t traceLevel,
            _In_ int32_t source,
            _In_ GUID platformId,
            _In_ UINT64 operationId,
            _In_ int32_t status);

        HRESULT TraceOperationRelation(
            _In_ ScriptContext* scriptContext,
            _In_ int32_t traceLevel,
            _In_ int32_t source,
            _In_ GUID platformId,
            _In_ UINT64 operationId,
            _In_ int32_t relation);

        HRESULT TraceSynchronousWorkStart(
            _In_ ScriptContext* scriptContext,
            _In_ int32_t traceLevel,
            _In_ int32_t source,
            _In_ GUID platformId,
            _In_ UINT64 operationId,
            _In_ int32_t work);

        HRESULT TraceSynchronousWorkCompletion(
            _In_ ScriptContext* scriptContext,
            _In_ int32_t traceLevel,
            _In_ int32_t source,
            _In_ int32_t work);
    };
}
#endif
