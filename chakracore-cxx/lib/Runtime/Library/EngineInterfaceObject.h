//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if defined(ENABLE_INTL_OBJECT) || defined(ENABLE_JS_BUILTINS)


#pragma warning(push)
#pragma warning(disable:4309) // truncation of constant value
#pragma warning(disable:4838) // conversion from 'int' to 'const char' requires a narrowing conversion

#if DISABLE_JIT
#if TARGET_64
#include "InJavascript/JsBuiltIn.nojit.bc.64b.h"
#else
#include "InJavascript/JsBuiltIn.nojit.bc.32b.h"
#endif // TARGET_64
#else
#if TARGET_64
#include "InJavascript/JsBuiltIn.bc.64b.h"
#else
#include "InJavascript/JsBuiltIn.bc.32b.h"
#endif // TARGET_64
#endif // DISABLE_JIT

#pragma warning(pop)


namespace Js
{
    class WindowsGlobalizationAdapter;

    enum EngineInterfaceExtensionKind
    {
        EngineInterfaceExtensionKind_JsBuiltIn = 0,
        EngineInterfaceExtensionKind_Intl = 1,
        EngineInterfaceExtensionKind_WinRTPromise = 2,
        MaxEngineInterfaceExtensionKind = EngineInterfaceExtensionKind_WinRTPromise
    };

    class EngineExtensionObjectBase
    {
    public:
        EngineExtensionObjectBase(EngineInterfaceExtensionKind kind, Js::ScriptContext * context) :
            extensionKind(kind),
            scriptContext(context)
        {
            hasBytecode = false;
        }

        EngineInterfaceExtensionKind GetExtensionKind() const { return extensionKind; }
        ScriptContext* GetScriptContext() const { return scriptContext; }
        bool GetHasByteCode() const { return hasBytecode; }
        void SetHasBytecode() { hasBytecode = true; }
        virtual void Initialize() = 0;
#if DBG
        virtual void DumpByteCode() = 0;
#endif

    protected:
        Field(EngineInterfaceExtensionKind) extensionKind;
        Field(ScriptContext*) scriptContext;
        Field(bool) hasBytecode;
    };

#define EngineInterfaceObject_CommonFunctionProlog(function, callInfo) \
        PROBE_STACK(function->GetScriptContext(), Js::Constants::MinStackDefault); \
        RUNTIME_ARGUMENTS(args, callInfo); \
        Assert(!(callInfo.Flags & CallFlags_New)); \
        unsigned argCount = args.Info.Count; \
        ScriptContext* scriptContext = function->GetScriptContext(); \
        AssertMsg(argCount > 0, "Should always have implicit 'this'"); \

    class EngineInterfaceObject : public DynamicObject
    {
    private:
        DEFINE_VTABLE_CTOR(EngineInterfaceObject, DynamicObject);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(EngineInterfaceObject);

        Field(DynamicObject*) commonNativeInterfaces;

        Field(EngineExtensionObjectBase*) engineExtensions[MaxEngineInterfaceExtensionKind + 1];

    public:
        EngineInterfaceObject(DynamicType * type)
            : DynamicObject(type), commonNativeInterfaces(nullptr), engineExtensions()
        {}

        DynamicObject* GetCommonNativeInterfaces() const { return commonNativeInterfaces; }
        EngineExtensionObjectBase* GetEngineExtension(EngineInterfaceExtensionKind extensionKind) const;
        void SetEngineExtension(EngineInterfaceExtensionKind extensionKind, EngineExtensionObjectBase* extensionObject);

        static EngineInterfaceObject* New(Recycler * recycler, DynamicType * type);

#if ENABLE_TTD
        virtual void MarkVisitKindSpecificPtrs(TTD::SnapshotExtractor* extractor) override;
        virtual void ProcessCorePaths() override;

        virtual TTD::NSSnapObjects::SnapObjectType GetSnapTag_TTD() const override;
        virtual void ExtractSnapObjectDataInto(TTD::NSSnapObjects::SnapObject* objData, TTD::SlabAllocator& alloc) override;
#endif

        void Initialize();
        bool IsInitialized() const { return commonNativeInterfaces != nullptr; }

        static bool InitializeCommonNativeInterfaces(DynamicObject* engineInterface, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        static ScriptFunction *CreateLibraryCodeScriptFunction(ScriptFunction *scriptFunction, JavascriptString *displayName, bool isConstructor, bool isJsBuiltIn, bool isPublic);

        class EntryInfo
        {
        public:
            // CallInstanceFunction is still handled specially because it gets special inline treatment from the JIT
            static FunctionInfo CallInstanceFunction;

#define BuiltInRaiseException(exceptionType, exceptionID) static NoProfileFunctionInfo BuiltIn_raise##exceptionID;
#define EngineInterfaceBuiltIn2(propId, nativeMethod) static NoProfileFunctionInfo nativeMethod;
#include "EngineInterfaceObjectBuiltIns.h"
        };

        static Var Entry_CallInstanceFunction(RecyclableObject *function, CallInfo callInfo, ...);

#define BuiltInRaiseException(exceptionType, exceptionID) static Var Entry_BuiltIn_raise##exceptionID(RecyclableObject *function, CallInfo callInfo, ...);
#define EngineInterfaceBuiltIn2(propId, nativeMethod) static Var Entry_##nativeMethod(RecyclableObject *function, CallInfo callInfo, ...);
#include "EngineInterfaceObjectBuiltIns.h"
    };

    template <> inline bool VarIsImpl<EngineInterfaceObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_EngineInterfaceObject;
    }
}

#endif // ENABLE_INTL_OBJECT || ENABLE_JS_BUILTINS
