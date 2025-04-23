//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#if DYNAMIC_INTERPRETER_THUNK
#define DefaultEntryThunk Js::InterpreterStackFrame::DelayDynamicInterpreterThunk
#define AsmJsDefaultEntryThunk Js::InterpreterStackFrame::AsmJsDelayDynamicInterpreterThunk
#else
#define DefaultEntryThunk Js::InterpreterStackFrame::InterpreterThunk
#endif

#define ProfileEntryThunk Js::ScriptContext::DebugProfileProbeThunk

#define DefaultDeferredParsingThunk Js::JavascriptFunction::DeferredParsingThunk
#if defined(ENABLE_SCRIPT_PROFILING) || defined(ENABLE_SCRIPT_DEBUGGING)

#define ProfileDeferredParsingThunk Js::ScriptContext::ProfileModeDeferredParsingThunk
#endif

#define DefaultDeferredDeserializeThunk Js::JavascriptFunction::DeferredDeserializeThunk
#define ProfileDeferredDeserializeThunk Js::ScriptContext::ProfileModeDeferredDeserializeThunk
#if ENABLE_NATIVE_CODEGEN
class NativeCodeGenerator;
class ThreadContext;
struct CodeGenWorkItem;
class NativeCodeData;
class ThreadContextInfo;
class StackSym;
class Func;
struct InlinedFrameLayout;

typedef intptr_t IntConstType;
typedef uintptr_t  UIntConstType;
typedef double  FloatConstType;

#include "EmitBuffer.h"
#include "InterpreterThunkEmitter.h"
#include "BackendOpCodeAttr.h"
#include "BackendOpCodeAttrAsmJs.h"
#include "CodeGenNumberAllocator.h"
#include "NativeCodeData.h"
#include "JnHelperMethod.h"
#include "IRType.h"
#include "InlineeFrameInfo.h"
#include "CodeGenAllocators.h"

NativeCodeGenerator * NewNativeCodeGenerator(Js::ScriptContext * nativeCodeGen);
void DeleteNativeCodeGenerator(NativeCodeGenerator * nativeCodeGen);
void CloseNativeCodeGenerator(NativeCodeGenerator* nativeCodeGen);
bool IsClosedNativeCodeGenerator(NativeCodeGenerator* nativeCodeGen);
void SetProfileModeNativeCodeGen(NativeCodeGenerator *pNativeCodeGen, BOOL fSet);
void UpdateNativeCodeGeneratorForDebugMode(NativeCodeGenerator* nativeCodeGen);

CriticalSection *GetNativeCodeGenCriticalSection(NativeCodeGenerator *pNativeCodeGen);
bool TryReleaseNonHiPriWorkItem(Js::ScriptContext* scriptContext, CodeGenWorkItem* workItem);
void NativeCodeGenEnterScriptStart(NativeCodeGenerator * nativeCodeGen);
void FreeNativeCodeGenAllocation(Js::ScriptContext* scriptContext, Js::JavascriptMethod codeAddress, Js::JavascriptMethod thunkAddress);
InProcCodeGenAllocators* GetForegroundAllocator(NativeCodeGenerator * nativeCodeGen, PageAllocator* pageallocator);
void GenerateFunction(NativeCodeGenerator * nativeCodeGen, Js::FunctionBody * functionBody, Js::ScriptFunction * function = NULL);
void GenerateLoopBody(NativeCodeGenerator * nativeCodeGen, Js::FunctionBody * functionBody, Js::LoopHeader * loopHeader, Js::EntryPointInfo* entryPointInfo, uint localCount, Js::Var localSlots[]);
#ifdef ENABLE_PREJIT
void GenerateAllFunctions(NativeCodeGenerator * nativeCodeGen, Js::FunctionBody * fn);
#endif
#ifdef IR_VIEWER
Js::Var RejitIRViewerFunction(NativeCodeGenerator *nativeCodeGen, Js::FunctionBody *fn, Js::ScriptContext *scriptContext);
#endif
#ifdef ALLOW_JIT_REPRO
HRESULT JitFromEncodedWorkItem(NativeCodeGenerator *nativeCodeGen, _In_reads_(bufferSize) const byte* buffer, _In_ uint bufferSize);
#endif

BOOL IsIntermediateCodeGenThunk(Js::JavascriptMethod codeAddress);
BOOL IsAsmJsCodeGenThunk(Js::JavascriptMethod codeAddress);
typedef Js::JavascriptMethod (*CheckCodeGenFunction)(Js::ScriptFunction * function);
CheckCodeGenFunction GetCheckCodeGenFunction(Js::JavascriptMethod codeAddress);
Js::JavascriptMethod GetCheckCodeGenThunk();
#ifdef ASMJS_PLAT
Js::JavascriptMethod GetCheckAsmJsCodeGenThunk();
#endif

uint GetBailOutRegisterSaveSlotCount();
uint GetBailOutReserveSlotCount();

#if DBG
void CheckIsExecutable(Js::RecyclableObject * function, Js::JavascriptMethod entryPoint);
#endif

#ifdef PROFILE_EXEC
namespace Js
{
class ScriptContextProfiler;
};
void CreateProfilerNativeCodeGen(NativeCodeGenerator * nativeCodeGen, Js::ScriptContextProfiler * profiler);
void ProfilePrintNativeCodeGen(NativeCodeGenerator * nativeCodeGen);
void SetProfilerFromNativeCodeGen(NativeCodeGenerator * toNativeCodeGen, NativeCodeGenerator * fromNativeCodeGen);
#endif

void DeleteNativeCodeData(NativeCodeData * data);
#else
inline BOOL IsIntermediateCodeGenThunk(Js::JavascriptMethod codeAddress) { return false; }
inline BOOL IsAsmJsCodeGenThunk(Js::JavascriptMethod codeAddress) { return false; }
#endif

#if _M_X64
extern "C" void * amd64_ReturnFromCallWithFakeFrame();
#endif
