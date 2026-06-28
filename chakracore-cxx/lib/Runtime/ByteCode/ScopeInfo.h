//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js {
    class ByteCodeBufferBuilder;
    class ByteCodeBufferReader;

    //
    // ScopeInfo is used to persist Scope info of outer functions. When reparsing deferred nested
    // functions, use persisted ScopeInfo to restore outer closures.
    //
    class ScopeInfo
    {
        friend class ByteCodeBufferBuilder;
        friend class ByteCodeBufferReader;

        DECLARE_RECYCLER_VERIFY_MARK_FRIEND()

        struct MapSymbolData
        {
            ByteCodeGenerator *byteCodeGenerator;
            FuncInfo* func;
            int nonScopeSymbolCount;
        };

        struct SymbolInfo
        {
            union
            {
                typename WriteBarrierFieldTypeTraits<PropertyId>::Type propertyId;
                typename WriteBarrierFieldTypeTraits<PropertyRecord const*>::Type name;
            };
            typename WriteBarrierFieldTypeTraits<SymbolType>::Type symbolType;
            typename WriteBarrierFieldTypeTraits<bool>::Type hasFuncAssignment : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type isBlockVariable : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type isConst : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type isFuncExpr : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type isModuleExportStorage : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type isModuleImport : 1;
            typename WriteBarrierFieldTypeTraits<bool>::Type needDeclaration : 1;
        };

    private:
        typename WriteBarrierFieldTypeTraits<ScopeInfo *>::Type parent;               // link to parent scope info (if any)
        typename WriteBarrierFieldTypeTraits<FunctionInfo * const>::Type functionInfo;// link to function owning this scope

        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isDynamic : 1;             // isDynamic bit affects how deferredChild access global ref
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isObject : 1;              // isObject bit affects how deferredChild access closure symbols
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type mustInstantiate : 1;       // the scope must be instantiated as an object/array
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isCached : 1;              // indicates that local vars and functions are cached across invocations
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type areNamesCached : 1;
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type hasLocalInClosure : 1;
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isGeneratorFunctionBody : 1;
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isAsyncFunctionBody : 1;
        typename WriteBarrierFieldTypeTraits<uint8_t>::Type isClassConstructor : 1;

        FieldNoBarrier(Scope *) scope;
        typename WriteBarrierFieldTypeTraits<::ScopeType>::Type scopeType;
        typename WriteBarrierFieldTypeTraits<int>::Type scopeId;
        typename WriteBarrierFieldTypeTraits<int>::Type symbolCount;                // symbol count in this scope
        typename WriteBarrierFieldTypeTraits<SymbolInfo>::Type symbols[];           // symbol PropertyIDs, index == sym.scopeSlot

    private:
        ScopeInfo(FunctionInfo * function, int symbolCount)
            : functionInfo(function), /*funcExprScopeInfo(nullptr), paramScopeInfo(nullptr),*/ symbolCount(symbolCount), parent(nullptr), scope(nullptr), areNamesCached(false), hasLocalInClosure(false), isGeneratorFunctionBody(false), isAsyncFunctionBody(false), isClassConstructor(false)/*, parentOnly(false)*/
        {
        }

        void SetSymbolId(int i, PropertyId propertyId)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].propertyId = propertyId;
        }

        void SetSymbolType(int i, SymbolType symbolType)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].symbolType = symbolType;
        }

        void SetHasFuncAssignment(int i, bool has)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].hasFuncAssignment = has;
        }

        void SetIsBlockVariable(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].isBlockVariable = is;
        }

        void SetIsConst(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].isConst = is;
        }

        void SetIsFuncExpr(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].isFuncExpr = is;
        }

        void SetIsModuleExportStorage(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].isModuleExportStorage = is;
        }

        void SetIsModuleImport(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].isModuleImport = is;
        }

        void SetNeedDeclaration(int i, bool is)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].needDeclaration = is;
        }

        void SetPropertyName(int i, PropertyRecord const* name)
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            symbols[i].name = name;
        }

        PropertyId GetSymbolId(int i) const
        {
            Assert(!areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].propertyId;
        }

        SymbolType GetSymbolType(int i) const
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].symbolType;
        }

        bool GetHasFuncAssignment(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].hasFuncAssignment;
        }

        bool GetIsModuleExportStorage(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].isModuleExportStorage;
        }

        bool GetIsModuleImport(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].isModuleImport;
        }

        bool GetNeedDeclaration(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].needDeclaration;
        }

        bool GetIsBlockVariable(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].isBlockVariable;
        }

        bool GetIsConst(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].isConst;
        }

        bool GetIsFuncExpr(int i)
        {
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].isFuncExpr;
        }

        PropertyRecord const* GetPropertyName(int i)
        {
            Assert(areNamesCached);
            Assert(i >= 0 && i < symbolCount);
            return symbols[i].name;
        }

        void SaveSymbolInfo(Symbol* sym, MapSymbolData* mapSymbolData);

        static ScopeInfo* SaveScopeInfo(ByteCodeGenerator * byteCodeGenerator, Scope * scope, ScriptContext * scriptContext);
        static ScopeInfo* SaveOneScopeInfo(ByteCodeGenerator * byteCodeGenerator, Scope * scope, ScriptContext * scriptContext);

    public:
        FunctionInfo * GetFunctionInfo() const
        {
            return functionInfo;
        }

        ParseableFunctionInfo * GetParseableFunctionInfo() const
        {
            return functionInfo ? functionInfo->GetParseableFunctionInfo() : nullptr;
        }

        ScopeInfo* GetParentScopeInfo() const
        {
            return parent;//? parent->GetParseableFunctionInfo()->GetScopeInfo() : nullptr;
        }

        void SetParentScopeInfo(ScopeInfo * parent)
        {
            Assert(this->parent == nullptr);
            this->parent = parent;
        }

        Scope * GetScope() const
        {
            return scope;
        }

        void SetScope(Scope * scope)
        {
            this->scope = scope;
        }

        ::ScopeType GetScopeType() const
        {
            return scopeType;
        }

        void SetScopeType(::ScopeType type)
        {
            this->scopeType = type;
        }

        void SetScopeId(int id)
        {
            this->scopeId = id;
        }

        int GetScopeId() const
        {
            return scopeId;
        }

        int GetSymbolCount() const
        {
            return symbolCount;
        }

        bool IsObject() const
        {
            return isObject;
        }

        bool IsCached() const
        {
            return isCached;
        }

        void SetHasLocalInClosure(bool has)
        {
            hasLocalInClosure = has;
        }

        bool GetHasOwnLocalInClosure() const
        {
            return hasLocalInClosure;
        }

        bool IsGeneratorFunctionBody() const
        {
            return this->isGeneratorFunctionBody;
        }

        bool IsAsyncFunctionBody() const
        {
            return this->isAsyncFunctionBody;
        }

        bool IsClassConstructor() const
        {
            return this->isClassConstructor;
        }

        static void SaveEnclosingScopeInfo(ByteCodeGenerator* byteCodeGenerator, /*FuncInfo* parentFunc,*/ FuncInfo* func);

        void EnsurePidTracking(ScriptContext* scriptContext);

        void ExtractScopeInfo(Parser *parser, /*ByteCodeGenerator* byteCodeGenerator, FuncInfo* funcInfo,*/ Scope* scope);

        //
        // Turn on capturesAll for a Scope temporarily. Restore old capturesAll when this object
        // goes out of scope.
        //
        class AutoCapturesAllScope
        {
        private:
            Scope* scope;
            bool oldCapturesAll;

        public:
            AutoCapturesAllScope(Scope* scope, bool turnOn);
            ~AutoCapturesAllScope();
            bool OldCapturesAll() const
            {
                return oldCapturesAll;
            }
        };
    };
}
