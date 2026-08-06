//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Types/ScriptFunctionType.h"

namespace Js
{
    class ScriptFunctionBase : public JavascriptFunction
    {
    protected:
        ScriptFunctionBase(DynamicType * type);
        ScriptFunctionBase(DynamicType * type, FunctionInfo * functionInfo);

        DEFINE_VTABLE_CTOR(ScriptFunctionBase, JavascriptFunction);

    public:
        virtual Var  GetHomeObj() const = 0;
        virtual void SetHomeObj(Var homeObj) = 0;
        virtual void SetComputedNameVar(Var computedNameVar) = 0;
        virtual Var GetComputedNameVar() const = 0;
        virtual bool IsAnonymousFunction() const = 0;
    };

    template <> bool VarIsImpl<ScriptFunctionBase>(RecyclableObject* obj);

    template <class BaseClass>
    class FunctionWithComputedName : public BaseClass
    {
    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type computedNameVar;

    protected:
        DEFINE_VTABLE_CTOR(FunctionWithComputedName<BaseClass>, BaseClass);
        friend class Js::CrossSiteObject<FunctionWithComputedName<BaseClass>>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<FunctionWithComputedName<BaseClass>>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<FunctionWithComputedName<BaseClass>>>::SetVirtualTable(this);
        };
    public:
        FunctionWithComputedName(FunctionProxy * proxy, ScriptFunctionType* deferredPrototypeType)
            : BaseClass(proxy, deferredPrototypeType), computedNameVar(nullptr)
        {
            Assert(proxy->GetFunctionInfo()->HasComputedName());
        }
        virtual Var GetComputedNameVar() const override { return this->computedNameVar; }
        virtual void SetComputedNameVar(Var computedNameVar) override;
        VTableValue DummyVirtualFunctionToHinderLinkerICF() const override;
    };

    template <class BaseClass>
    class FunctionWithHomeObj : public BaseClass
    {
    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type homeObj;
    protected:
        DEFINE_VTABLE_CTOR(FunctionWithHomeObj<BaseClass>, BaseClass);
        friend class Js::CrossSiteObject<FunctionWithHomeObj<BaseClass>>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<FunctionWithHomeObj<BaseClass>>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<FunctionWithHomeObj<BaseClass>>>::SetVirtualTable(this);
        };
    public:
        FunctionWithHomeObj(FunctionProxy* proxy, ScriptFunctionType* deferredPrototypeType)
            : BaseClass(proxy, deferredPrototypeType), homeObj(nullptr)
        {
            Assert(proxy->GetFunctionInfo()->HasHomeObj());
        }
        virtual Var GetHomeObj() const override { return homeObj; }
        virtual void SetHomeObj(Var homeObj) override { this->homeObj = homeObj; }
        static uint32_t GetOffsetOfHomeObj() { return  offsetof(FunctionWithHomeObj<BaseClass>, homeObj); }
        virtual VTableValue DummyVirtualFunctionToHinderLinkerICF() const override;
    };

    class ScriptFunction : public ScriptFunctionBase
    {
    private:
        typename WriteBarrierFieldTypeTraits<FrameDisplay*>::Type environment;  // Optional environment, for closures
        typename WriteBarrierFieldTypeTraits<ActivationObjectEx *>::Type cachedScopeObj;
        typename WriteBarrierFieldTypeTraits<bool>::Type hasInlineCaches;

        static JavascriptString* GetComputedName(Var computedNameVar, ScriptContext * scriptContext);
        static bool GetSymbolName(Var computedNameVar, const char16_t** symbolName, charcount_t *length);
    protected:
        DEFINE_VTABLE_CTOR(ScriptFunction, ScriptFunctionBase);
        friend class Js::CrossSiteObject<ScriptFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ScriptFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ScriptFunction>>::SetVirtualTable(this);
        };
    public:
        ScriptFunction(FunctionProxy * proxy, ScriptFunctionType* deferredPrototypeType);
        inline static BOOL Test(JavascriptFunction *func) { return func->IsScriptFunction(); }
        static ScriptFunction * OP_NewScFunc(FrameDisplay *environment, FunctionInfoPtrPtr infoRef);
        static ScriptFunction * OP_NewScFuncHomeObj(FrameDisplay *environment, FunctionInfoPtrPtr infoRef, Var homeObj);
        static ScriptFunction * OP_NewClassConstructor(FrameDisplay *environment, FunctionInfoPtrPtr infoRef, Var homeObject, RecyclableObject * constructorParent);
        static void CopyEntryPointInfoToThreadContextIfNecessary(ProxyEntryPointInfo* oldEntryPointInfo, ProxyEntryPointInfo* newEntryPointInfo);

        ProxyEntryPointInfo* GetEntryPointInfo() const;
        FunctionEntryPointInfo* GetFunctionEntryPointInfo() const
        {
            Assert(this->GetFunctionProxy()->IsDeferred() == FALSE);
            ProxyEntryPointInfo* result = this->GetEntryPointInfo();
            Assert(result->IsFunctionEntryPointInfo());
            return static_cast<FunctionEntryPointInfo*>(result);
        }

        FunctionProxy * GetFunctionProxy() const;
        ScriptFunctionType * GetScriptFunctionType() const;

        FrameDisplay* GetEnvironment() const { return environment; }
        void SetEnvironment(FrameDisplay * environment);
        ActivationObjectEx *GetCachedScope() const { return cachedScopeObj; }
        void SetCachedScope(ActivationObjectEx *obj) { cachedScopeObj = obj; }
        void InvalidateCachedScopeChain();

        static uint32_t GetOffsetOfEnvironment() { return offsetof(ScriptFunction, environment); }
        static uint32_t GetOffsetOfCachedScopeObj() { return offsetof(ScriptFunction, cachedScopeObj); };
        static uint32_t GetOffsetOfHasInlineCaches() { return offsetof(ScriptFunction, hasInlineCaches); };

        void ChangeEntryPoint(ProxyEntryPointInfo* entryPointInfo, JavascriptMethod entryPoint);
        JavascriptMethod UpdateThunkEntryPoint(FunctionEntryPointInfo* entryPointInfo, JavascriptMethod entryPoint);
        bool IsNewEntryPointAvailable();
        JavascriptMethod UpdateUndeferredBody(FunctionBody* newFunctionInfo);

        ScriptFunctionType * DuplicateType() override;
        void PrepareForConversionToNonPathType() override;
        void ReplaceTypeWithPredecessorType(DynamicType * previousType) override;

        Var GetSourceString() const override;
        JavascriptString * EnsureSourceString() override;

        bool GetHasInlineCaches() { return hasInlineCaches; }
        void SetHasInlineCaches(bool has) { hasInlineCaches = has; }

        bool HasSuperReference();

        Var GetHomeObj() const override { return nullptr; }
        void SetHomeObj(Var homeObj) override { AssertMsg(false, "Should have created FunctionWithHomeObj variant"); }

        Var GetComputedNameVar() const override { return nullptr; }
        void SetComputedNameVar(Var computedNameVar) override { AssertMsg(false, "Should have created the FunctionWithComputedName variant"); }
        JavascriptString * GetDisplayNameImpl() const override;
        bool IsAnonymousFunction() const override;
        virtual bool IsAsmJsFunction() const { return false; }
        virtual bool IsWasmFunction() const { return false; }

        virtual JavascriptFunction* GetRealFunctionObject() { return this; }

        bool HasFunctionBody();

    public:
        VTableValue DummyVirtualFunctionToHinderLinkerICF() const override {
            return VTableValue::VtableScriptFunction;
        }
    };

    template <> inline bool VarIsImpl<ScriptFunction>(RecyclableObject* obj)
    {
        return VarIs<JavascriptFunction>(obj) && UnsafeVarTo<JavascriptFunction>(obj)->IsScriptFunction();
    }

    typedef FunctionWithComputedName<ScriptFunction> ScriptFunctionWithComputedName;
    typedef FunctionWithHomeObj<ScriptFunction> ScriptFunctionWithHomeObj;

    class AsmJsScriptFunction : public ScriptFunction
    {
    public:
        AsmJsScriptFunction(FunctionProxy * proxy, ScriptFunctionType* deferredPrototypeType);

        static AsmJsScriptFunction * OP_NewAsmJsFunc(FrameDisplay *environment, FunctionInfoPtrPtr infoRef);

        bool IsAsmJsFunction() const override { return true; }

        void SetModuleEnvironment(typename WriteBarrierFieldTypeTraits<Var>::Type* mem) { m_moduleEnvironment = mem; }
        typename WriteBarrierFieldTypeTraits<Var>::Type* GetModuleEnvironment() const { return m_moduleEnvironment; }
        static uint32_t GetOffsetOfModuleMemory() { return offsetof(AsmJsScriptFunction, m_moduleEnvironment); }

        class JavascriptArrayBuffer* GetAsmJsArrayBuffer() const;
    protected:
        DEFINE_VTABLE_CTOR(AsmJsScriptFunction, ScriptFunction);
        friend class Js::CrossSiteObject<AsmJsScriptFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<AsmJsScriptFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<AsmJsScriptFunction>>::SetVirtualTable(this);
        };

    private:
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<Var>::Type*>::Type m_moduleEnvironment;
    };

    template <> inline bool VarIsImpl<AsmJsScriptFunction>(RecyclableObject* obj)
    {
        return VarIs<ScriptFunction>(obj) && UnsafeVarTo<ScriptFunction>(obj)->IsAsmJsFunction();
    }

    typedef FunctionWithComputedName<AsmJsScriptFunction> AsmJsScriptFunctionWithComputedName;

#ifdef ENABLE_WASM
    class WasmScriptFunction : public AsmJsScriptFunction
    {
    public:
        WasmScriptFunction(FunctionProxy * proxy, ScriptFunctionType* deferredPrototypeType);

        void SetSignature(Wasm::WasmSignature * sig) { m_signature = sig; }
        Wasm::WasmSignature * GetSignature() const { return m_signature; }
        static uint32_t GetOffsetOfSignature() { return offsetof(WasmScriptFunction, m_signature); }

        WebAssemblyMemory* GetWebAssemblyMemory() const;

        bool IsWasmFunction() const override { return true; }
    protected:
        DEFINE_VTABLE_CTOR(WasmScriptFunction, AsmJsScriptFunction);
        friend class Js::CrossSiteObject<WasmScriptFunction>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<WasmScriptFunction>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<WasmScriptFunction>>::SetVirtualTable(this);
        };
    private:
        typename WriteBarrierFieldTypeTraits<Wasm::WasmSignature *>::Type m_signature;
    };

    template <> inline bool VarIsImpl<WasmScriptFunction>(RecyclableObject* obj)
    {
        return VarIs<ScriptFunction>(obj) && UnsafeVarTo<ScriptFunction>(obj)->IsWasmFunction();
    }
#else
    class WasmScriptFunction : public AsmJsScriptFunction
    {
    };
    template <> inline bool VarIsImpl<WasmScriptFunction>(RecyclableObject* obj) { return false; }
#endif

    class ScriptFunctionWithInlineCache : public ScriptFunction
    {
    private:
        typename WriteBarrierFieldTypeTraits<void**>::Type m_inlineCaches;

#if DBG
#define InlineCacheTypeNone         0x00
#define InlineCacheTypeInlineCache  0x01
#define InlineCacheTypeIsInst       0x02
        typename WriteBarrierFieldTypeTraits<byte *>::Type m_inlineCacheTypes;
#endif
        typename WriteBarrierFieldTypeTraits<uint>::Type inlineCacheCount;
        typename WriteBarrierFieldTypeTraits<uint>::Type rootObjectLoadInlineCacheStart;
        typename WriteBarrierFieldTypeTraits<uint>::Type rootObjectLoadMethodInlineCacheStart;
        typename WriteBarrierFieldTypeTraits<uint>::Type rootObjectStoreInlineCacheStart;
        typename WriteBarrierFieldTypeTraits<uint>::Type isInstInlineCacheCount;

    protected:
        ScriptFunctionWithInlineCache(DynamicType * type);

        DEFINE_VTABLE_CTOR(ScriptFunctionWithInlineCache, ScriptFunction);
        friend class Js::CrossSiteObject<ScriptFunctionWithInlineCache>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ScriptFunctionWithInlineCache>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ScriptFunctionWithInlineCache>>::SetVirtualTable(this);
        };

    public:
        ScriptFunctionWithInlineCache(FunctionProxy * proxy, ScriptFunctionType* deferredPrototypeType);
        void CreateInlineCache();
        void AllocateInlineCache();
        void ClearInlineCacheOnFunctionObject();
        InlineCache * GetInlineCache(uint index);
        uint GetInlineCacheCount() { return inlineCacheCount; }
        typename WriteBarrierFieldTypeTraits<void**>::Type GetInlineCaches() const { return m_inlineCaches; }
        static uint32_t GetOffsetOfInlineCaches() { return offsetof(ScriptFunctionWithInlineCache, m_inlineCaches); };
        template<bool isShutdown>
        void FreeOwnInlineCaches();
        void Finalize(bool isShutdown) override;
    };

    template <> inline bool VarIsImpl<ScriptFunctionWithInlineCache>(RecyclableObject* obj)
    {
        return VarIs<ScriptFunction>(obj) && UnsafeVarTo<ScriptFunction>(obj)->GetHasInlineCaches();
    }

    typedef FunctionWithComputedName<ScriptFunctionWithInlineCache> ScriptFunctionWithInlineCacheAndComputedName;
} // namespace Js
