//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Language/InterpreterStackFrame.h"

using namespace Js;

namespace
{
    // RAII helper to set the state of the generator to completed if an exception is 
    // thrown or if the save state InterpreterStackFrame is never created implying 
    // the generator is JITed and returned without ever yielding
    struct GeneratorStateHelper
    {
        using GeneratorState = JavascriptGenerator::GeneratorState;

        JavascriptGenerator* generator;
        bool didThrow;

        GeneratorStateHelper(JavascriptGenerator* generator) :
            generator(generator),
            didThrow(true)
        {
            generator->SetState(GeneratorState::Executing);
        }

        ~GeneratorStateHelper()
        {
            generator->SetState(IsDone() ? GeneratorState::Completed : GeneratorState::Suspended);
        }

        bool IsDone()
        {
            // If the generator is jit'd, we set its interpreter frame to nullptr at the
            // end right before the epilogue to signal that the generator has completed
            auto* frame = generator->GetFrame();

            if (didThrow || frame == nullptr)
                return true;

            int nextOffset = frame->GetReader()->GetCurrentOffset();
            int endOffset = frame->GetFunctionBody()->GetByteCode()->GetLength();

            if (nextOffset == endOffset - 1)
                return true;

            return false;
        }
    };
}

JavascriptGenerator::JavascriptGenerator(
    DynamicType* type,
    Arguments& args,
    ScriptFunction* scriptFunction) :
        DynamicObject(type),
        args(args),
        frame(nullptr),
        state(GeneratorState::SuspendedStart),
        scriptFunction(scriptFunction),
        resumeYieldObject(nullptr) {}

JavascriptGenerator* JavascriptGenerator::New(
    Recycler* recycler,
    DynamicType* generatorType,
    Arguments& args,
    ScriptFunction* scriptFunction)
{
    // InterpreterStackFrame takes a pointer to the args, so copy them to the recycler
    // heap and use that buffer for the generator's InterpreterStackFrame
    typename WriteBarrierFieldTypeTraits<Var>::Type* argValuesCopy = nullptr;

    if (args.Info.Count > 0)
    {
        argValuesCopy = RecyclerNewArray(recycler, typename WriteBarrierFieldTypeTraits<Var>::Type, args.Info.Count);
        CopyArray(argValuesCopy, args.Info.Count, args.Values, args.Info.Count);
    }

    Arguments heapArgs(args.Info, (Var*)argValuesCopy);

    if (CONFIG_FLAG(ForceSoftwareWriteBarrier))
    {
        JavascriptGenerator* obj = RecyclerNewFinalized(
            recycler,
            JavascriptGenerator,
            generatorType,
            heapArgs,
            scriptFunction);

        if (obj->args.Values != nullptr)
        {
            recycler->RegisterPendingWriteBarrierBlock(
                obj->args.Values,
                obj->args.Info.Count * sizeof(Var));

            recycler->RegisterPendingWriteBarrierBlock(
                &obj->args.Values,
                sizeof(Var*));
        }

        return obj;
    }

    return RecyclerNew(recycler, JavascriptGenerator, generatorType, heapArgs, scriptFunction);
}

template<>
bool Js::VarIsImpl<JavascriptGenerator>(RecyclableObject* obj)
{
    auto typeId = JavascriptOperators::GetTypeId(obj);
    return typeId == TypeIds_Generator || typeId == TypeIds_AsyncGenerator;
}

void JavascriptGenerator::SetFrame(InterpreterStackFrame* frame, size_t bytes)
{
    Assert(this->frame == nullptr);
    this->frame = frame;
    if (CONFIG_FLAG(ForceSoftwareWriteBarrier))
    {
        GetScriptContext()->GetRecycler()->RegisterPendingWriteBarrierBlock(frame, bytes);
    }
}

void JavascriptGenerator::SetFrameSlots(Js::RegSlot slotCount, typename WriteBarrierFieldTypeTraits<Var>::Type* frameSlotArray)
{
    AssertMsg(this->frame->GetFunctionBody()->GetLocalsCount() == slotCount, 
        "Unexpected mismatch in frame slot count for generated.");

    for (Js::RegSlot i = 0; i < slotCount; i++)
        GetFrame()->m_localSlots[i] = frameSlotArray[i];
}

void JavascriptGenerator::Finalize(bool isShutdown)
{
    if (CONFIG_FLAG(ForceSoftwareWriteBarrier) && !isShutdown)
    {
        auto* recycler = GetScriptContext()->GetRecycler();
        
        if (this->frame)
            recycler->UnRegisterPendingWriteBarrierBlock(this->frame);

        if (this->args.Values)
            recycler->UnRegisterPendingWriteBarrierBlock(this->args.Values);
    }
}

void JavascriptGenerator::ThrowIfExecuting(const char16_t* apiName)
{
    if (this->IsExecuting())
    {
        JavascriptError::ThrowTypeError(
            GetScriptContext(),
            JSERR_GeneratorAlreadyExecuting,
            apiName);
    }
}

Var JavascriptGenerator::CallGenerator(Var data, ResumeYieldKind resumeKind)
{
    Assert(!IsExecuting() && !IsCompleted());

    ScriptContext* scriptContext = this->GetScriptContext();
    JavascriptLibrary* library = scriptContext->GetLibrary();
    Var result = nullptr;

    if (this->frame)
    {
        // if the function already has a state it may be going to resume in the jit
        // if so copy any innerScopes into registers jit can access
        uint32_t innerScopeCount = this->scriptFunction->GetFunctionBody()->GetInnerScopeCount();
        for (uint32_t i = 0; i < innerScopeCount; ++i)
        {
            Js::RegSlot reg = this->scriptFunction->GetFunctionBody()->GetFirstInnerScopeRegister() + i;
            this->frame->SetNonVarReg(reg, this->frame->InnerScopeFromIndex(i));
        }
    }

    SetResumeYieldProperties(data, resumeKind);

    {
        Var thunkArgs[] = {this, this->resumeYieldObject};
        Arguments arguments(std::size(thunkArgs), thunkArgs);
        GeneratorStateHelper helper(this);

        try
        {
            BEGIN_SAFE_REENTRANT_CALL(scriptContext->GetThreadContext())
            {
                result = JavascriptFunction::CallFunction<1>(
                    this->scriptFunction,
                    this->scriptFunction->GetEntryPoint(),
                    arguments);
            }
            END_SAFE_REENTRANT_CALL
            helper.didThrow = false;
        }
        catch (const JavascriptException& err)
        {
            JavascriptExceptionOperators::DoThrowCheckClone(err.GetAndClear(), scriptContext);
        }
    }

    // Clear the value property of the resume yield object so that we don't
    // extend the lifetime of the value
    SetResumeYieldProperties(library->GetUndefined(), ResumeYieldKind::Normal);

    if (IsCompleted())
        return library->CreateIteratorResultObject(result, library->GetTrue());

    return result;
}

void JavascriptGenerator::SetResumeYieldProperties(Var value, ResumeYieldKind kind)
{
    auto* library = GetScriptContext()->GetLibrary();
    DynamicType* type = library->GetResumeYieldObjectType();

    if (!this->resumeYieldObject)
        this->resumeYieldObject = DynamicObject::New(GetScriptContext()->GetRecycler(), type);
    else
        AssertOrFailFast(this->resumeYieldObject->GetDynamicType() == type);

    Var kindVar = TaggedInt::ToVarUnchecked((int)kind);
    this->resumeYieldObject->SetSlot(SetSlotArguments(Js::PropertyIds::value, 0, value));
    this->resumeYieldObject->SetSlot(SetSlotArguments(Js::PropertyIds::kind, 1, kindVar));
}

Var JavascriptGenerator::EntryNext(RecyclableObject* function, CallInfo callInfo, ...)
{
    PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
    ARGUMENTS(args, callInfo);

    auto* scriptContext = function->GetScriptContext();
    auto* library = scriptContext->GetLibrary();

    AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Generator.prototype.next");

    if (!VarIs<DynamicObject>(args[0]) || JavascriptOperators::GetTypeId(args[0]) != TypeIds_Generator)
    {
        JavascriptError::ThrowTypeErrorVar(
            scriptContext,
            JSERR_NeedObjectOfType,
            u"Generator.prototype.next",
            u"Generator");
    }

    Var undefinedVar = library->GetUndefined();
    Var input = args.Info.Count > 1 ? args[1] : undefinedVar;

    auto* generator = UnsafeVarTo<JavascriptGenerator>(args[0]);

    if (generator->IsCompleted())
        return library->CreateIteratorResultObject(undefinedVar, library->GetTrue());

    generator->ThrowIfExecuting(u"Generator.prototype.next");
    return generator->CallGenerator(input, ResumeYieldKind::Normal);
}

Var JavascriptGenerator::EntryReturn(RecyclableObject* function, CallInfo callInfo, ...)
{
    PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
    ARGUMENTS(args, callInfo);

    auto* scriptContext = function->GetScriptContext();
    auto* library = scriptContext->GetLibrary();

    AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Generator.prototype.return");

    if (!VarIs<DynamicObject>(args[0]) || JavascriptOperators::GetTypeId(args[0]) != TypeIds_Generator)
    {
        JavascriptError::ThrowTypeErrorVar(
            scriptContext,
            JSERR_NeedObjectOfType,
            u"Generator.prototype.return",
            u"Generator");
    }

    Var input = args.Info.Count > 1 ? args[1] : library->GetUndefined();

    auto* generator = UnsafeVarTo<JavascriptGenerator>(args[0]);

    if (generator->IsSuspendedStart())
        generator->SetCompleted();

    if (generator->IsCompleted())
        return library->CreateIteratorResultObject(input, library->GetTrue());

    generator->ThrowIfExecuting(u"Generator.prototype.return");
    return generator->CallGenerator(input, ResumeYieldKind::Return);
}

Var JavascriptGenerator::EntryThrow(RecyclableObject* function, CallInfo callInfo, ...)
{
    PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault);
    ARGUMENTS(args, callInfo);

    auto* scriptContext = function->GetScriptContext();
    auto* library = scriptContext->GetLibrary();

    AUTO_TAG_NATIVE_LIBRARY_ENTRY(function, callInfo, u"Generator.prototype.throw");

    if (!VarIs<DynamicObject>(args[0]) || JavascriptOperators::GetTypeId(args[0]) != TypeIds_Generator)
    {
        JavascriptError::ThrowTypeErrorVar(
            scriptContext,
            JSERR_NeedObjectOfType,
            u"Generator.prototype.throw",
            u"Generator");
    }

    Var input = args.Info.Count > 1 ? args[1] : library->GetUndefined();

    auto* generator = UnsafeVarTo<JavascriptGenerator>(args[0]);

    if (generator->IsSuspendedStart())
        generator->SetCompleted();

    if (generator->IsCompleted())
        JavascriptExceptionOperators::OP_Throw(input, scriptContext);

    generator->ThrowIfExecuting(u"Generator.prototype.throw");
    return generator->CallGenerator(input, ResumeYieldKind::Throw);
}

bool JavascriptGenerator::IsAsyncModule() const
{ 
    FunctionProxy* proxy = this->scriptFunction->GetFunctionProxy();
    return proxy->IsModule() && proxy->IsAsync();
}

void JavascriptGenerator::OutputBailInTrace(JavascriptGenerator* generator)
{
    char16_t debugStringBuffer[MAX_FUNCTION_BODY_DEBUG_STRING_SIZE];
    FunctionBody* fnBody = generator->scriptFunction->GetFunctionBody();

    Output::Print(
        u"BailIn: function: %s (%s) offset: #%04x\n",
        fnBody->GetDisplayName(),
        fnBody->GetDebugNumberSet(debugStringBuffer),
        generator->frame->m_reader.GetCurrentOffset());

    if (generator->bailInSymbolsTraceArrayCount == 0)
    {
        Output::Print(
            u"BailIn: No symbols reloaded\n",
            fnBody->GetDisplayName(),
            fnBody->GetDebugNumberSet(debugStringBuffer));
    }
    else
    {
        for (int i = 0; i < generator->bailInSymbolsTraceArrayCount; i++)
        {
            const BailInSymbol& symbol = generator->bailInSymbolsTraceArray[i];
            Output::Print(u"BailIn: Register #%4d, value: 0x%p\n", symbol.id, symbol.value);
        }
    }
}
