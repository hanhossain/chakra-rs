//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

using namespace Js;

FunctionInfo JavascriptAsyncFunction::functionInfo(
    JavascriptAsyncFunction::EntryAsyncFunctionImplementation, _no_write_barrier_tag(),
    (FunctionInfo::Attributes)(FunctionInfo::DoNotProfile | FunctionInfo::ErrorOnNew));

JavascriptAsyncFunction::JavascriptAsyncFunction(
    DynamicType* type,
    GeneratorVirtualScriptFunction* scriptFunction) :
        JavascriptGeneratorFunction(type, &functionInfo, scriptFunction)
{
    DebugOnly(VerifyEntryPoint());
}

JavascriptAsyncFunction* JavascriptAsyncFunction::New(
    ScriptContext* scriptContext,
    GeneratorVirtualScriptFunction* scriptFunction)
{
    return scriptContext->GetLibrary()->CreateAsyncFunction(
        functionInfo.GetOriginalEntryPoint(),
        scriptFunction);
}

template<>
bool Js::VarIsImpl<JavascriptAsyncFunction>(RecyclableObject* obj)
{
    return VarIs<JavascriptFunction>(obj) && (
        VirtualTableInfo<JavascriptAsyncFunction>::HasVirtualTable(obj) ||
        VirtualTableInfo<CrossSiteObject<JavascriptAsyncFunction>>::HasVirtualTable(obj)
    );
}

Var JavascriptAsyncFunction::EntryAsyncFunctionImplementation(
    RecyclableObject* function,
    CallInfo callInfo, ...)
{
    auto* scriptContext = function->GetScriptContext();
    PROBE_STACK(scriptContext, Js::Constants::MinStackDefault);
    ARGUMENTS(args, callInfo);

    auto* library = scriptContext->GetLibrary();
    auto* asyncFn = VarTo<JavascriptAsyncFunction>(function);
    auto* scriptFn = asyncFn->GetGeneratorVirtualScriptFunction();
    auto* generator = library->CreateGenerator(args, scriptFn, library->GetNull());

    return BeginAsyncFunctionExecution(generator);
}

JavascriptPromise* JavascriptAsyncFunction::BeginAsyncFunctionExecution(JavascriptGenerator* generator)
{
    auto* library = generator->GetLibrary();
    auto* scriptContext = generator->GetScriptContext();
    auto* promise = library->CreatePromise();

    auto* stepFn = library->CreateAsyncSpawnStepFunction(
        EntryAsyncSpawnStepNextFunction,
        generator,
        library->GetUndefined());

    JavascriptExceptionObject* exception = nullptr;
    JavascriptPromiseResolveOrRejectFunction* resolve;
    JavascriptPromiseResolveOrRejectFunction* reject;
    JavascriptPromise::InitializePromise(promise, &resolve, &reject, scriptContext);

    try
    {
        AsyncSpawnStep(stepFn, generator, resolve, reject);
    }
    catch (const JavascriptException& err)
    {
        exception = err.GetAndClear();
    }

    if (exception != nullptr)
        JavascriptPromise::TryRejectWithExceptionObject(exception, reject, scriptContext);

    return promise;
}

Var JavascriptAsyncFunction::EntryAsyncSpawnStepNextFunction(
    RecyclableObject* function,
    CallInfo callInfo, ...)
{
    auto* scriptContext = function->GetScriptContext();
    PROBE_STACK(scriptContext, Js::Constants::MinStackDefault);
    auto* stepFn = VarTo<JavascriptAsyncSpawnStepFunction>(function);
    return stepFn->generator->CallGenerator(stepFn->argument, ResumeYieldKind::Normal);
}

Var JavascriptAsyncFunction::EntryAsyncSpawnStepThrowFunction(
    RecyclableObject* function,
    CallInfo callInfo, ...)
{
    auto* scriptContext = function->GetScriptContext();
    PROBE_STACK(scriptContext, Js::Constants::MinStackDefault);

    auto* stepFn = VarTo<JavascriptAsyncSpawnStepFunction>(function);
    return stepFn->generator->CallGenerator(stepFn->argument, ResumeYieldKind::Throw);
}

Var JavascriptAsyncFunction::EntryAsyncSpawnCallStepFunction(
    RecyclableObject* function,
    CallInfo callInfo, ...)
{
    auto* scriptContext = function->GetScriptContext();
    PROBE_STACK(scriptContext, Js::Constants::MinStackDefault);
    ARGUMENTS(args, callInfo);

    auto* library = scriptContext->GetLibrary();
    Var undefinedVar = library->GetUndefined();
    Var resolvedValue = args.Info.Count > 1 ? args[1] : undefinedVar;

    auto* stepFn = VarTo<JavascriptAsyncSpawnStepFunction>(function);

    JavascriptMethod method = stepFn->isReject
        ? EntryAsyncSpawnStepThrowFunction
        : EntryAsyncSpawnStepNextFunction;
    
    auto* nextStepFn = library->CreateAsyncSpawnStepFunction(
        method,
        stepFn->generator,
        resolvedValue);

    AsyncSpawnStep(nextStepFn, stepFn->generator, stepFn->resolve, stepFn->reject);
    return undefinedVar;
}

void JavascriptAsyncFunction::AsyncSpawnStep(
    JavascriptAsyncSpawnStepFunction* stepFunction,
    JavascriptGenerator* generator,
    Var resolve,
    Var reject)
{
    ScriptContext* scriptContext = generator->GetScriptContext();
    BEGIN_SAFE_REENTRANT_REGION(scriptContext->GetThreadContext())

    JavascriptLibrary* library = scriptContext->GetLibrary();
    Var undefinedVar = library->GetUndefined();

    JavascriptExceptionObject* exception = nullptr;
    RecyclableObject* result = nullptr;

    try
    {
        Var resultVar = CALL_FUNCTION(
            scriptContext->GetThreadContext(),
            stepFunction,
            CallInfo(CallFlags_Value, 1),
            undefinedVar);

        result = VarTo<RecyclableObject>(resultVar);
    }
    catch (const JavascriptException& err)
    {
        exception = err.GetAndClear();
    }

    if (exception != nullptr)
    {
        // If the generator threw an exception, reject the promise
        JavascriptPromise::TryRejectWithExceptionObject(exception, reject, scriptContext);
        return;
    }

    Assert(result != nullptr);

    Var value = JavascriptOperators::GetProperty(result, PropertyIds::value, scriptContext);

    // If the generator is done, resolve the promise
    if (generator->IsCompleted())
    {
        if (!JavascriptConversion::IsCallable(resolve))
            JavascriptError::ThrowTypeError(scriptContext, JSERR_NeedFunction);

        CALL_FUNCTION(
            scriptContext->GetThreadContext(),
            VarTo<RecyclableObject>(resolve),
            CallInfo(CallFlags_Value, 2),
            undefinedVar,
            value);

        return;
    }
    else
    {
        Assert(JavascriptOperators::GetTypeId(result) == TypeIds_AwaitObject);
    }


    // Chain off the yielded promise and step again
    auto* successFunction = library->CreateAsyncSpawnStepFunction(
        EntryAsyncSpawnCallStepFunction,
        generator,
        undefinedVar,
        resolve,
        reject);

    auto* failFunction = library->CreateAsyncSpawnStepFunction(
        EntryAsyncSpawnCallStepFunction,
        generator,
        undefinedVar,
        resolve,
        reject,
        true);

    auto* promise = JavascriptPromise::InternalPromiseResolve(value, scriptContext);
    auto* unused = JavascriptPromise::UnusedPromiseCapability(scriptContext);
    
    JavascriptPromise::PerformPromiseThen(
        promise,
        unused,
        successFunction,
        failFunction,
        scriptContext);

    END_SAFE_REENTRANT_REGION
}

template<>
bool Js::VarIsImpl<JavascriptAsyncSpawnStepFunction>(RecyclableObject* obj)
{
    return VarIs<JavascriptFunction>(obj) && (
        VirtualTableInfo<JavascriptAsyncSpawnStepFunction>::HasVirtualTable(obj) ||
        VirtualTableInfo<CrossSiteObject<JavascriptAsyncSpawnStepFunction>>::HasVirtualTable(obj)
    );
}
