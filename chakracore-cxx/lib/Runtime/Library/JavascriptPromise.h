//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    struct JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper
    {
        typename WriteBarrierFieldTypeTraits<bool>::Type alreadyResolved;
    };

    class JavascriptPromiseResolveOrRejectFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseResolveOrRejectFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseResolveOrRejectFunction);

    public:
        JavascriptPromiseResolveOrRejectFunction(DynamicType* type);
        JavascriptPromiseResolveOrRejectFunction(DynamicType* type, FunctionInfo* functionInfo, JavascriptPromise* promise, bool isReject, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyResolvedRecord);

        JavascriptPromise* GetPromise();
        bool IsRejectFunction();
        bool IsAlreadyResolved();
        void SetAlreadyResolved(bool is);

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromise*>::Type promise;
        typename WriteBarrierFieldTypeTraits<bool>::Type isReject;
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper*>::Type alreadyResolvedWrapper;
    };

    template <> bool VarIsImpl<JavascriptPromiseResolveOrRejectFunction>(RecyclableObject* obj);

    class JavascriptPromiseCapabilitiesExecutorFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseCapabilitiesExecutorFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseCapabilitiesExecutorFunction);

    public:
        JavascriptPromiseCapabilitiesExecutorFunction(DynamicType* type, FunctionInfo* functionInfo, JavascriptPromiseCapability* capability);

        JavascriptPromiseCapability* GetCapability();

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseCapability*>::Type capability;
    };

    template <> bool VarIsImpl<JavascriptPromiseCapabilitiesExecutorFunction>(RecyclableObject* obj);

    class JavascriptPromiseResolveThenableTaskFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseResolveThenableTaskFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseResolveThenableTaskFunction);

    public:
        JavascriptPromiseResolveThenableTaskFunction(DynamicType* type, FunctionInfo* functionInfo, JavascriptPromise* promise, RecyclableObject* thenable, RecyclableObject* thenFunction)
            : RuntimeFunction(type, functionInfo), promise(promise), thenable(thenable), thenFunction(thenFunction)
        { }

        JavascriptPromise* GetPromise();
        RecyclableObject* GetThenable();
        RecyclableObject* GetThenFunction();


    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromise*>::Type promise;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type thenable;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type thenFunction;
    };

    template <> bool VarIsImpl<JavascriptPromiseResolveThenableTaskFunction>(RecyclableObject* obj);

    class JavascriptPromiseReactionTaskFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseReactionTaskFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseReactionTaskFunction);

    public:
        JavascriptPromiseReactionTaskFunction(DynamicType* type, FunctionInfo* functionInfo, JavascriptPromiseReaction* reaction, Var argument)
            : RuntimeFunction(type, functionInfo), reaction(reaction), argument(argument)
        { }

        JavascriptPromiseReaction* GetReaction();
        Var GetArgument();

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseReaction*>::Type reaction;
        typename WriteBarrierFieldTypeTraits<Var>::Type argument;
    };

    template <> bool VarIsImpl<JavascriptPromiseReactionTaskFunction>(RecyclableObject* obj);

    class JavascriptPromiseThenFinallyFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseThenFinallyFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseThenFinallyFunction);

    public:
        JavascriptPromiseThenFinallyFunction(DynamicType* type, FunctionInfo* functionInfo, RecyclableObject* OnFinally, RecyclableObject* Constructor, bool shouldThrow)
            : RuntimeFunction(type, functionInfo), OnFinally(OnFinally), Constructor(Constructor), shouldThrow(shouldThrow)
        { }

        inline bool GetShouldThrow() { return this->shouldThrow; }
        inline RecyclableObject* GetOnFinally() { return this->OnFinally; }
        inline RecyclableObject* GetConstructor() { return this->Constructor; }

    private:
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type OnFinally;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type Constructor;
        typename WriteBarrierFieldTypeTraits<bool>::Type shouldThrow;
    };

    template <> bool VarIsImpl<JavascriptPromiseThenFinallyFunction>(RecyclableObject* obj);

    class JavascriptPromiseThunkFinallyFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseThunkFinallyFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseThunkFinallyFunction);

    public:
        JavascriptPromiseThunkFinallyFunction(DynamicType* type, FunctionInfo* functionInfo, Var value, bool shouldThrow)
            : RuntimeFunction(type, functionInfo), value(value), shouldThrow(shouldThrow)
        { }

        inline bool GetShouldThrow() { return this->shouldThrow; }
        inline Var GetValue() { return this->value; }

    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type value;
        typename WriteBarrierFieldTypeTraits<bool>::Type shouldThrow;
    };

    template <> bool VarIsImpl<JavascriptPromiseThunkFinallyFunction>(RecyclableObject* obj);

    struct JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper
    {
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type remainingElements;
    };

    class JavascriptPromiseAllResolveElementFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseAllResolveElementFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseAllResolveElementFunction);

    public:
        JavascriptPromiseAllResolveElementFunction(DynamicType* type);
        JavascriptPromiseAllResolveElementFunction(DynamicType* type, FunctionInfo* functionInfo, uint32_t index, JavascriptArray* values, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElementsWrapper);

        JavascriptPromiseCapability* GetCapabilities();
        uint32_t GetIndex();
        uint32_t GetRemainingElements();
        JavascriptArray* GetValues();
        bool IsAlreadyCalled() const;
        void SetAlreadyCalled(const bool is);

        uint32_t DecrementRemainingElements();

    protected:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseCapability*>::Type capabilities;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type index;
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper*>::Type remainingElementsWrapper;
        typename WriteBarrierFieldTypeTraits<JavascriptArray*>::Type values;
        typename WriteBarrierFieldTypeTraits<bool>::Type alreadyCalled;
    };

    template <> bool VarIsImpl<JavascriptPromiseAllResolveElementFunction>(RecyclableObject* obj);

    class JavascriptPromiseAllSettledResolveOrRejectElementFunction : public JavascriptPromiseAllResolveElementFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseAllSettledResolveOrRejectElementFunction, JavascriptPromiseAllResolveElementFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseAllSettledResolveOrRejectElementFunction);

    public:
        JavascriptPromiseAllSettledResolveOrRejectElementFunction(DynamicType* type);
        JavascriptPromiseAllSettledResolveOrRejectElementFunction(DynamicType* type, FunctionInfo* functionInfo, uint32_t index, JavascriptArray* values, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElementsWrapper, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyCalledWrapper, bool isRejecting);

        bool IsAlreadyCalled() const;
        void SetAlreadyCalled(const bool is);

        bool IsRejectFunction();

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper*>::Type alreadyCalledWrapper;
        typename WriteBarrierFieldTypeTraits<bool>::Type isRejecting;
    };

    template <> bool VarIsImpl<JavascriptPromiseAllSettledResolveOrRejectElementFunction>(RecyclableObject* obj);

    class JavascriptPromiseCapability : FinalizableObject
    {
    private:
        JavascriptPromiseCapability(Var promise, Var resolve, Var reject)
            : promise(promise), resolve(resolve), reject(reject)
        { }

    public:
        static JavascriptPromiseCapability* New(Var promise, Var resolve, Var reject, ScriptContext* scriptContext);

        Var GetResolve();
        Var GetReject();
        Var GetPromise();
        void SetPromise(Var);

        void SetResolve(Var resolve);
        void SetReject(Var reject);

    public:
        // Finalizable support
        virtual void Finalize(bool isShutdown)
        {
        }

        virtual void Dispose(bool isShutdown)
        {
        }

        virtual void Mark(Recycler *recycler) override
        {
            AssertMsg(false, "Mark called on object that isnt TrackableObject");
        }

    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type promise;
        typename WriteBarrierFieldTypeTraits<Var>::Type resolve;
        typename WriteBarrierFieldTypeTraits<Var>::Type reject;
    };

    class JavascriptPromiseReaction : FinalizableObject
    {
    private:
        JavascriptPromiseReaction(JavascriptPromiseCapability* capabilities, RecyclableObject* handler)
            : capabilities(capabilities), handler(handler)
        { }

    public:
        static JavascriptPromiseReaction* New(JavascriptPromiseCapability* capabilities, RecyclableObject* handler, ScriptContext* scriptContext);

        JavascriptPromiseCapability* GetCapabilities();
        RecyclableObject* GetHandler();

    public:
        // Finalizable support
        virtual void Finalize(bool isShutdown)
        {
        }

        virtual void Dispose(bool isShutdown)
        {
        }

        virtual void Mark(Recycler *recycler) override
        {
            AssertMsg(false, "Mark called on object that isnt TrackableObject");
        }

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseCapability*>::Type capabilities;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type handler;
    };

    struct JavascriptPromiseReactionPair
    {
        JavascriptPromiseReaction* resolveReaction;
        JavascriptPromiseReaction* rejectReaction;
    };

    typedef SList<Js::JavascriptPromiseReactionPair, Recycler> JavascriptPromiseReactionList;

    class JavascriptPromise : public DynamicObject
    {
    private:
        DEFINE_VTABLE_CTOR(JavascriptPromise, DynamicObject);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromise);

    public:
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;

            static FunctionInfo All;
            static FunctionInfo Catch;
            static FunctionInfo Race;
            static FunctionInfo Reject;
            static FunctionInfo Resolve;
            static FunctionInfo Then;
            static FunctionInfo Finally;
            static FunctionInfo AllSettled;
            static FunctionInfo Any;

            static FunctionInfo Identity;
            static FunctionInfo Thrower;

            static FunctionInfo FinallyValueFunction;
            static FunctionInfo ThenFinallyFunction;
            static FunctionInfo ResolveOrRejectFunction;
            static FunctionInfo CapabilitiesExecutorFunction;
            static FunctionInfo AllResolveElementFunction;
            static FunctionInfo AllSettledResolveOrRejectElementFunction;
            static FunctionInfo AnyRejectElementFunction;

            static FunctionInfo GetterSymbolSpecies;
        };

        JavascriptPromise(DynamicType * type);

        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryAll(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryCatch(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryRace(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryReject(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryResolve(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryThen(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFinally(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAllSettled(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAny(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryThunkFinallyFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryThenFinallyFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryCapabilitiesExecutorFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryResolveOrRejectFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryReactionTaskFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryResolveThenableTaskFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryIdentityFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryThrowerFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAllResolveElementFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAllSettledResolveOrRejectElementFunction(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAnyRejectElementFunction(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryGetterSymbolSpecies(RecyclableObject* function, CallInfo callInfo, ...);

        static Var CreateRejectedPromise(Var resolution, ScriptContext* scriptContext, Var promiseConstructor = nullptr);
        static Var CreateResolvedPromise(Var resolution, ScriptContext* scriptContext, Var promiseConstructor = nullptr);
        static Var CreatePassThroughPromise(JavascriptPromise* sourcePromise, ScriptContext* scriptContext);
        static Var CreateThenPromise(JavascriptPromise* sourcePromise, RecyclableObject* fulfillmentHandler, RecyclableObject* rejectionHandler, ScriptContext* scriptContext);
        
        static JavascriptPromise* InternalPromiseResolve(Var value, ScriptContext* scriptContext);
        static Var PromiseResolve(Var constructor, Var value, ScriptContext* scriptContext);
        
        static void PerformPromiseThen(
            JavascriptPromise* sourcePromise,
            JavascriptPromiseCapability* capability,
            RecyclableObject* fulfillmentHandler,
            RecyclableObject* rejectionHandler,
            ScriptContext* scriptContext);

        virtual BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        virtual BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;

        JavascriptPromiseReactionList* GetReactions();


        static JavascriptPromiseCapability* NewPromiseCapability(Var constructor, ScriptContext* scriptContext);
        static JavascriptPromiseCapability* UnusedPromiseCapability(ScriptContext* scriptContext);
        static JavascriptPromiseCapability* CreatePromiseCapabilityRecord(RecyclableObject* constructor, ScriptContext* scriptContext);
        static Var TriggerPromiseReactions(JavascriptPromiseReactionList* reactions, bool isReject, Var resolution, ScriptContext* scriptContext);
        static void EnqueuePromiseReactionTask(JavascriptPromiseReaction* reaction, Var resolution, ScriptContext* scriptContext);

        static void InitializePromise(JavascriptPromise* promise, JavascriptPromiseResolveOrRejectFunction** resolve, JavascriptPromiseResolveOrRejectFunction** reject, ScriptContext* scriptContext);
        static Var TryCallResolveOrRejectHandler(Var handler, Var value, ScriptContext* scriptContext);
        static Var TryRejectWithExceptionObject(JavascriptExceptionObject* exceptionObject, Var handler, ScriptContext* scriptContext);

        static JavascriptPromise* CreateEnginePromise(ScriptContext *scriptContext);

        Var Resolve(Var resolution, ScriptContext* scriptContext);
        Var Reject(Var resolution, ScriptContext* scriptContext);

        enum PromiseStatus : unsigned char
        {
            PromiseStatusCode_Undefined     = 0x00,
            PromiseStatusCode_Unresolved    = 0x01,
            PromiseStatusCode_HasResolution = 0x02,
            PromiseStatusCode_HasRejection  = 0x03
        };


        bool GetIsHandled() const { return this->isHandled; }
        void SetIsHandled() { this->isHandled = true; }

        PromiseStatus GetStatus() const { return this->status; }
        void SetStatus(const PromiseStatus newStatus) { this->status = newStatus; }

        Var GetResult() const { return result; }

    protected:
        Var ResolveHelper(Var resolution, bool isRejecting, ScriptContext* scriptContext);

    protected:
        typename WriteBarrierFieldTypeTraits<Var>::Type result;
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseReactionList*>::Type reactions;

        // we could pack status & isHandled into a single byte, but the compiler is aligning this on address-size
        // boundaries, so we don't save anything.  Leaving these separate fields for clarity
        typename WriteBarrierFieldTypeTraits<PromiseStatus>::Type status;
        typename WriteBarrierFieldTypeTraits<bool>::Type isHandled;

    private :
        bool WillRejectionBeUnhandled();
    };

    template <> inline bool VarIsImpl<JavascriptPromise>(RecyclableObject* obj)
    {
        return Js::JavascriptOperators::GetTypeId(obj) == TypeIds_Promise;
    }

    class JavascriptPromiseAnyRejectElementFunction : public JavascriptPromiseAllResolveElementFunction {
    protected:
        DEFINE_VTABLE_CTOR(JavascriptPromiseAnyRejectElementFunction, JavascriptPromiseAllResolveElementFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptPromiseAnyRejectElementFunction);

    public:
        JavascriptPromiseAnyRejectElementFunction(DynamicType* type);
        JavascriptPromiseAnyRejectElementFunction(DynamicType* type, FunctionInfo* functionInfo, uint32_t index, JavascriptArray* values, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElementsWrapper, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyCalledWrapper);

    private:
        typename WriteBarrierFieldTypeTraits<JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper*>::Type alreadyCalledWrapper;
    };

    template <> bool VarIsImpl<JavascriptPromiseAnyRejectElementFunction>(RecyclableObject* obj);
}
