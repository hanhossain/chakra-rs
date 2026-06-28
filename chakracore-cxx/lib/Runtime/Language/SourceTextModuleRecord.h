//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Language/ModuleRecordBase.h"

namespace Js
{
    class SourceTextModuleRecord;
    typedef JsUtil::BaseDictionary<LPCOLESTR, SourceTextModuleRecord*, ArenaAllocator, PowerOf2SizePolicy> ChildModuleRecordSet;
    typedef JsUtil::BaseDictionary<SourceTextModuleRecord*, SourceTextModuleRecord*, ArenaAllocator, PowerOf2SizePolicy> ParentModuleRecordSet;
    typedef JsUtil::BaseDictionary<PropertyId, uint, ArenaAllocator, PowerOf2SizePolicy> LocalExportMap;
    typedef JsUtil::BaseDictionary<PropertyId, ModuleNameRecord, ArenaAllocator, PowerOf2SizePolicy> ResolvedExportMap;
    typedef JsUtil::List<PropertyId, ArenaAllocator> LocalExportIndexList;

    class SourceTextModuleRecord : public ModuleRecordBase
    {
    public:
        friend class ModuleNamespace;
        friend class JavascriptLibrary;

        SourceTextModuleRecord(ScriptContext* scriptContext);
        IdentPtrList* GetRequestedModuleList() const { return requestedModuleList; }
        ModuleImportOrExportEntryList* GetImportEntryList() const { return importRecordList; }
        ModuleImportOrExportEntryList* GetLocalExportEntryList() const { return localExportRecordList; }
        ModuleImportOrExportEntryList* GetIndirectExportEntryList() const { return indirectExportRecordList; }
        ModuleImportOrExportEntryList* GetStarExportRecordList() const { return starExportRecordList; }
        virtual ExportedNames* GetExportedNames(ExportModuleRecordList* exportStarSet) override;
        virtual bool IsSourceTextModuleRecord() override { return true; } // we don't really have other kind of modulerecord at this time.

        // return false when "ambiguous".
        // otherwise nullptr means "null" where we have circular reference/cannot resolve.
        bool ResolveExport(PropertyId exportName, ResolveSet* resolveSet, ModuleNameRecord** exportRecord) override;
        bool ResolveImport(PropertyId localName, ModuleNameRecord** importRecord);
        bool ModuleDeclarationInstantiation() override;
        void GenerateRootFunction();
        Var ModuleEvaluation() override;
        void FinishModuleEvaluation(bool shouldIncrementAwait);
        bool ModuleEvaluationPrepass();
        void DecrementAndEvaluateIfNothingAwaited();
        void PropogateRejection(Var reason);
        virtual ModuleNamespace* GetNamespace();
        virtual void SetNamespace(ModuleNamespace* moduleNamespace);

        void Finalize(bool isShutdown) override;
        void Dispose(bool isShutdown) override { return; }
        void Mark(Recycler * recycler) override { return; }

        int32_t ResolveExternalModuleDependencies();
        void EnsureChildModuleSet(ScriptContext *scriptContext);
        bool ConfirmChildrenParsed();

        void* GetHostDefined() const { return hostDefined; }
        void SetHostDefined(void* hostObj) { hostDefined = hostObj; }

        void SetSpecifier(Var specifier) { this->normalizedSpecifier = specifier; }
        Var GetSpecifier() const { return normalizedSpecifier; }
        const char16_t *GetSpecifierSz() const
        {
            return this->normalizedSpecifier != nullptr ? 
                VarTo<JavascriptString>(this->normalizedSpecifier)->GetSz() : u"module"; 
        }

        Var GetErrorObject() const { return errorObject; }

        bool WasParsed() const { return wasParsed; }
        void SetWasParsed() { wasParsed = true; }
        bool WasEvaluationPrepassed() const { return wasPrepassed; }
        void SetEvaluationPrepassed() { wasPrepassed = true; }
        bool IsEvaluating() const { return evaluating; }
        void SetEvaluating(bool status) { evaluating = status; }
        void IncrementAwaited() { ++awaitedModules; }
        bool DecrementAwaited() { return (--awaitedModules) == 0; }
        bool WasDeclarationInitialized() const { return wasDeclarationInitialized; }
        void SetWasDeclarationInitialized() { wasDeclarationInitialized = true; }
        void SetIsRootModule() { isRootModule = true; }
        bool GetIsRootModule() { return isRootModule; }
        JavascriptPromise *GetPromise() { return this->promise; }
        void SetPromise(JavascriptPromise *value) { this->promise = value; }

        Var GetImportMetaObject();

        void SetImportRecordList(ModuleImportOrExportEntryList* importList) { importRecordList = importList; }
        void SetLocalExportRecordList(ModuleImportOrExportEntryList* localExports) { localExportRecordList = localExports; }
        void SetIndirectExportRecordList(ModuleImportOrExportEntryList* indirectExports) { indirectExportRecordList = indirectExports; }
        void SetStarExportRecordList(ModuleImportOrExportEntryList* starExports) { starExportRecordList = starExports; }
        void SetRequestedModuleList(IdentPtrList* requestModules) { requestedModuleList = requestModules; }

        ScriptContext* GetScriptContext() const { return scriptContext; }
        int32_t ParseSource(__in_bcount(sourceLength) byte* sourceText, uint32_t sourceLength, SRCINFO * srcInfo, Var* exceptionVar, bool isUtf8);
        int32_t OnHostException(void* errorVar);

        static SourceTextModuleRecord* FromHost(void* hostModuleRecord)
        {
            SourceTextModuleRecord* moduleRecord = static_cast<SourceTextModuleRecord*>(hostModuleRecord);
            Assert((moduleRecord == nullptr) || (moduleRecord->magicNumber == moduleRecord->ModuleMagicNumber));
            return moduleRecord;
        }

        static bool Is(void* hostModuleRecord)
        {
            SourceTextModuleRecord* moduleRecord = static_cast<SourceTextModuleRecord*>(hostModuleRecord);
            if (moduleRecord != nullptr && (moduleRecord->magicNumber == moduleRecord->ModuleMagicNumber))
            {
                return true;
            }
            return false;
        }

        static SourceTextModuleRecord* Create(ScriptContext* scriptContext);

        static Var EntryAsyncModuleFulfilled(
            RecyclableObject* function,
            CallInfo callInfo, ...);

        static Var EntryAsyncModuleRejected(
            RecyclableObject* function,
            CallInfo callInfo, ...);

        uint GetLocalExportSlotIndexByExportName(PropertyId exportNameId);
        uint GetLocalExportSlotIndexByLocalName(PropertyId localNameId);
        typename WriteBarrierFieldTypeTraits<Var>::Type* GetLocalExportSlots() const { return localExportSlots; }
        uint GetLocalExportSlotCount() const { return localSlotCount; }
        uint GetModuleId() const { return moduleId; }
        uint GetLocalExportCount() const { return localExportCount; }

        ModuleNameRecord* GetNamespaceNameRecord() { return &namespaceRecord; }

        SourceTextModuleRecord* GetChildModuleRecord(LPCOLESTR specifier) const;

        void SetParent(SourceTextModuleRecord* parentRecord, LPCOLESTR moduleName);
        Utf8SourceInfo* GetSourceInfo() { return this->pSourceInfo; }
        static Var ResolveOrRejectDynamicImportPromise(bool isResolve, Var value, ScriptContext *scriptContext, SourceTextModuleRecord *mr = nullptr, bool useReturn = true);
        Var PostProcessDynamicModuleImport();

    private:
        const static uint InvalidModuleIndex = 0xffffffff;
        const static uint InvalidSlotCount = 0xffffffff;
        const static uint InvalidSlotIndex = 0xffffffff;
        // TODO: move non-GC fields out to avoid false reference?
        typename WriteBarrierFieldTypeTraits<bool>::Type confirmedReady = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type notifying = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type wasPrepassed = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type wasParsed = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type wasDeclarationInitialized = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type parentsNotified = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type isRootModule = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type hadNotifyHostReady = false;
        typename WriteBarrierFieldTypeTraits<bool>::Type evaluating = false;
        typename WriteBarrierFieldTypeTraits<JavascriptGenerator*>::Type generator;
        typename WriteBarrierFieldTypeTraits<ParseNodeProg *>::Type parseTree; // This is the parsed tree resulted from compilation.
        typename WriteBarrierFieldTypeTraits<Utf8SourceInfo*>::Type pSourceInfo;
        typename WriteBarrierFieldTypeTraits<uint>::Type sourceIndex;
        typename WriteBarrierFieldTypeTraits<Parser*, _no_write_barrier_policy, _no_write_barrier_policy>::Type parser;  // we'll need to keep the parser around till we are done with bytecode gen.
        typename WriteBarrierFieldTypeTraits<ScriptContext*>::Type scriptContext;
        typename WriteBarrierFieldTypeTraits<IdentPtrList*>::Type requestedModuleList;
        typename WriteBarrierFieldTypeTraits<ModuleImportOrExportEntryList*>::Type importRecordList;
        typename WriteBarrierFieldTypeTraits<ModuleImportOrExportEntryList*>::Type localExportRecordList;
        typename WriteBarrierFieldTypeTraits<ModuleImportOrExportEntryList*>::Type indirectExportRecordList;
        typename WriteBarrierFieldTypeTraits<ModuleImportOrExportEntryList*>::Type starExportRecordList;
        typename WriteBarrierFieldTypeTraits<ChildModuleRecordSet*>::Type childrenModuleSet;
        typename WriteBarrierFieldTypeTraits<ModuleRecordList*>::Type parentModuleList;
        typename WriteBarrierFieldTypeTraits<LocalExportMap*>::Type localExportMapByExportName;  // from propertyId to index map: for bytecode gen.
        typename WriteBarrierFieldTypeTraits<LocalExportMap*>::Type localExportMapByLocalName;  // from propertyId to index map: for bytecode gen.
        typename WriteBarrierFieldTypeTraits<LocalExportIndexList*>::Type localExportIndexList; // from index to propertyId: for typehandler.
        typename WriteBarrierFieldTypeTraits<ExportedNames*>::Type exportedNames;
        typename WriteBarrierFieldTypeTraits<ResolvedExportMap*>::Type resolvedExportMap;

        typename WriteBarrierFieldTypeTraits<Js::JavascriptFunction*>::Type rootFunction;
        typename WriteBarrierFieldTypeTraits<void*>::Type hostDefined;
        typename WriteBarrierFieldTypeTraits<Var>::Type normalizedSpecifier;
        typename WriteBarrierFieldTypeTraits<Var>::Type errorObject;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<Var>::Type*>::Type localExportSlots;
        typename WriteBarrierFieldTypeTraits<uint>::Type localSlotCount;

        // module export allows aliasing, like export {foo as foo1, foo2, foo3}.
        typename WriteBarrierFieldTypeTraits<uint>::Type localExportCount;
        typename WriteBarrierFieldTypeTraits<uint>::Type moduleId;

        // for Top level Await
        typename WriteBarrierFieldTypeTraits<uint>::Type awaitedModules;

        typename WriteBarrierFieldTypeTraits<ModuleNameRecord>::Type namespaceRecord;
        typename WriteBarrierFieldTypeTraits<JavascriptPromise*>::Type promise;

        typename WriteBarrierFieldTypeTraits<Var>::Type importMetaObject;

        int32_t PostParseProcess();
        int32_t PrepareForModuleDeclarationInitialization();
        void ReleaseParserResources();
        void ReleaseParserResourcesForHierarchy();
        void ImportModuleListsFromParser();
        int32_t OnChildModuleReady(SourceTextModuleRecord* childModule, Var errorObj);
        void NotifyParentsAsNeeded();
        void CleanupBeforeExecution();
        void InitializeLocalImports();
        void InitializeLocalExports();
        void InitializeIndirectExports();
        bool ParentsNotified() const { return parentsNotified; }
        void SetParentsNotified() { parentsNotified = true; }
        PropertyId EnsurePropertyIdForIdentifier(IdentPtr pid);
        LocalExportMap* GetLocalExportMap() const { return localExportMapByExportName; }
        LocalExportIndexList* GetLocalExportIndexList() const { return localExportIndexList; }
        ResolvedExportMap* GetExportedNamesMap() const { return resolvedExportMap; }
    };

    struct ServerSourceTextModuleRecord
    {
        uint moduleId;
        typename WriteBarrierFieldTypeTraits<Var>::Type* localExportSlotsAddr;
    };

    class AsyncModuleCallbackFunction : public RuntimeFunction
    {
    protected:
        DEFINE_VTABLE_CTOR(AsyncModuleCallbackFunction, RuntimeFunction);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(AsyncModuleCallbackFunction);

    public:
        AsyncModuleCallbackFunction(
            DynamicType* type,
            FunctionInfo* functionInfo,
            SourceTextModuleRecord* module) :
                RuntimeFunction(type, functionInfo),
                module(module) {}

        typename WriteBarrierFieldTypeTraits<SourceTextModuleRecord*>::Type module;
    };

    template<>
    bool VarIsImpl<AsyncModuleCallbackFunction>(RecyclableObject* obj);

}
