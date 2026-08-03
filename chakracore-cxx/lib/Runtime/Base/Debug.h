//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if DBG_DUMP
char16_t* DumpCallStackFull(uint frameCount = -1, bool print = true);
#endif

#define OUTPUT_PRINT(FunctionBody) \
    Output::Print(u"Function %s (#%d.%u, #%u) ", (FunctionBody)->GetDisplayName(), \
            (int)(FunctionBody)->GetSourceContextId(), (FunctionBody)->GetLocalFunctionId(), (FunctionBody)->GetFunctionNumber());

#define OUTPUT_TRACE2(Phase, FunctionBody, ...) \
   if(Js::Configuration::Global.flags.Trace.IsEnabled((Phase))) \
   { \
        char16_t prefixValue[512]; \
        swprintf_s(prefixValue, u"Function %s (#%d.%u, #%u)", (FunctionBody)->GetDisplayName(), \
            (int)(FunctionBody)->GetSourceContextId(), (FunctionBody)->GetLocalFunctionId(), (FunctionBody)->GetFunctionNumber()); \
        Output::TraceWithPrefix((Phase), prefixValue, __VA_ARGS__); \
   }
#define OUTPUT_TRACE_FUNC(Phase, Func, ...) \
   if(PHASE_TRACE((Phase), (Func))) \
      { \
        char16_t prefixValue[512]; \
        swprintf_s(prefixValue, u"%s (#%d.%u, #%u)", (Func)->GetJITFunctionBody()->GetDisplayName(), \
           (int)(Func)->GetJITFunctionBody()->GetSourceContextId(), (Func)->GetWorkItem()->GetJITTimeInfo()->GetLocalFunctionId(), (Func)->GetJITFunctionBody()->GetFunctionNumber()); \
        Output::TraceWithPrefix((Phase), prefixValue, __VA_ARGS__); \
      }