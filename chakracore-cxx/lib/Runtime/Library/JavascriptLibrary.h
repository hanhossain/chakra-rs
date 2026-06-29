//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DataStructures/Cache.h"
#include "Memory/RecyclerFastAllocator.h"

#define InlineSlotCountIncrement (HeapConstants::ObjectGranularity / sizeof(Var))

#define MaxPreInitializedObjectTypeInlineSlotCount 16
#define MaxPreInitializedObjectHeaderInlinedTypeInlineSlotCount \
    (Js::DynamicTypeHandler::GetObjectHeaderInlinableSlotCapacity() + MaxPreInitializedObjectTypeInlineSlotCount)
#define PreInitializedObjectTypeCount ((MaxPreInitializedObjectTypeInlineSlotCount / InlineSlotCountIncrement) + 1)
static_assert(MaxPreInitializedObjectTypeInlineSlotCount <= USHRT_MAX);

#include "StringCache.h"
#include "Library/JavascriptGenerator.h"
#include "Library/JavascriptAsyncGenerator.h"

class ScriptSite;
class ActiveScriptExternalLibrary;
class EditAndContinue;
class ChakraHostScriptContext;
class JsrtExternalType;

namespace Js
{
    class RefCountedBuffer;

    static const unsigned int EvalMRUSize = 15;
    typedef JsUtil::BaseDictionary<unsigned long, SourceContextInfo *, Recycler, PowerOf2SizePolicy> SourceContextInfoMap;
    typedef JsUtil::BaseDictionary<uint, SourceContextInfo *, Recycler, PowerOf2SizePolicy> DynamicSourceContextInfoMap;

    typedef JsUtil::BaseDictionary<EvalMapString, ScriptFunction*, RecyclerNonLeafAllocator, PrimeSizePolicy> SecondLevelEvalCache;
    typedef TwoLevelHashRecord<FastEvalMapString, ScriptFunction*, SecondLevelEvalCache, EvalMapString> EvalMapRecord;
    typedef JsUtil::Cache<FastEvalMapString, EvalMapRecord*, RecyclerNonLeafAllocator, PrimeSizePolicy, JsUtil::MRURetentionPolicy<FastEvalMapString, EvalMRUSize>, FastEvalMapStringComparer> EvalCacheTopLevelDictionary;
    typedef JsUtil::Cache<EvalMapString, FunctionInfo*, RecyclerNonLeafAllocator, PrimeSizePolicy, JsUtil::MRURetentionPolicy<EvalMapString, EvalMRUSize>> NewFunctionCache;
    typedef JsUtil::BaseDictionary<ParseableFunctionInfo*, ParseableFunctionInfo*, Recycler, PrimeSizePolicy, RecyclerPointerComparer> ParseableFunctionInfoMap;
    // This is the dictionary used by script context to cache the eval.
    typedef TwoLevelHashDictionary<FastEvalMapString, ScriptFunction*, EvalMapRecord, EvalCacheTopLevelDictionary, EvalMapString> EvalCacheDictionary;

    typedef JsUtil::BaseDictionary<JavascriptMethod, JavascriptFunction*, Recycler, PrimeSizePolicy> BuiltInLibraryFunctionMap;
    typedef JsUtil::BaseDictionary<uint, JavascriptString *, Recycler> StringMap;

    // valid if object!= NULL
    struct EnumeratedObjectCache
    {
        static const int kMaxCachedPropStrings = 16;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type object;
        typename WriteBarrierFieldTypeTraits<DynamicType*>::Type type;
        typename WriteBarrierFieldTypeTraits<PropertyString*>::Type propertyStrings[kMaxCachedPropStrings];
        typename WriteBarrierFieldTypeTraits<int>::Type validPropStrings;
    };

    struct PropertyStringMap
    {
        typename WriteBarrierFieldTypeTraits<PropertyString*>::Type strLen2[80];

        inline static uint PStrMapIndex(char16_t ch)
        {
            Assert(ch >= '0' && ch <= 'z');
            return ch - '0';
        }
    };

    struct Cache
    {
        static const uint AssignCacheSize = 16;
        static const uint StringifyCacheSize = 16;
        static const uint CreateKeysCacheSize = 16;

        typename WriteBarrierFieldTypeTraits<PropertyStringMap*>::Type propertyStrings[80];
        typename WriteBarrierFieldTypeTraits<JavascriptString *>::Type lastNumberToStringRadix10String;
        typename WriteBarrierFieldTypeTraits<EnumeratedObjectCache>::Type enumObjCache;
        typename WriteBarrierFieldTypeTraits<JavascriptString *>::Type lastUtcTimeFromStrString;
        typename WriteBarrierFieldTypeTraits<EvalCacheDictionary*>::Type evalCacheDictionary;
        typename WriteBarrierFieldTypeTraits<EvalCacheDictionary*>::Type indirectEvalCacheDictionary;
        typename WriteBarrierFieldTypeTraits<NewFunctionCache*>::Type newFunctionCache;
        typename WriteBarrierFieldTypeTraits<StringMap *>::Type integerStringMap;
        typename WriteBarrierFieldTypeTraits<RegexPatternMruMap *>::Type dynamicRegexMap;
        typename WriteBarrierFieldTypeTraits<SourceContextInfoMap*>::Type sourceContextInfoMap;   // maps host provided context cookie to the URL of the script buffer passed.
        typename WriteBarrierFieldTypeTraits<DynamicSourceContextInfoMap*>::Type dynamicSourceContextInfoMap;
        typename WriteBarrierFieldTypeTraits<SourceContextInfo*>::Type noContextSourceContextInfo;
        typename WriteBarrierFieldTypeTraits<SRCINFO*>::Type noContextGlobalSourceInfo;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<SRCINFO const *>::Type*>::Type moduleSrcInfo;
        typename WriteBarrierFieldTypeTraits<BuiltInLibraryFunctionMap*>::Type builtInLibraryFunctions;
        typename WriteBarrierFieldTypeTraits<ScriptContextPolymorphicInlineCache*>::Type toStringTagCache;
        typename WriteBarrierFieldTypeTraits<ScriptContextPolymorphicInlineCache*>::Type toJSONCache;
        typename WriteBarrierFieldTypeTraits<EnumeratorCache*>::Type assignCache;
        typename WriteBarrierFieldTypeTraits<EnumeratorCache*>::Type stringifyCache;
        typename WriteBarrierFieldTypeTraits<EnumeratorCache*>::Type createKeysCache;
#if ENABLE_PROFILE_INFO
#if DBG_DUMP || defined(DYNAMIC_PROFILE_STORAGE) || defined(RUNTIME_DATA_COLLECTION)
        typename WriteBarrierFieldTypeTraits<DynamicProfileInfoList*>::Type profileInfoList;
#endif
#endif
        Cache() : toStringTagCache(nullptr), toJSONCache(nullptr), assignCache(nullptr), stringifyCache(nullptr) { }
    };

    class MissingPropertyTypeHandler;
    class SourceTextModuleRecord;
    class ArrayBufferBase;
    class SharedContents;
    typedef RecyclerFastAllocator<JavascriptNumber, LeafBit> RecyclerJavascriptNumberAllocator;
    typedef JsUtil::List<Var, Recycler> ListForListIterator;

    class UndeclaredBlockVariable : public RecyclableObject
    {
        friend class JavascriptLibrary;
        UndeclaredBlockVariable(Type* type) : RecyclableObject(type) { }
    };

    class SourceTextModuleRecord;
    typedef JsUtil::List<SourceTextModuleRecord*> ModuleRecordList;

#if ENABLE_COPYONACCESS_ARRAY
    struct CacheForCopyOnAccessArraySegments
    {
        static const uint32_t MAX_SIZE = 31;
        typename WriteBarrierFieldTypeTraits<SparseArraySegment<int32_t> *>::Type cache[MAX_SIZE];
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type count;

        uint32_t AddSegment(SparseArraySegment<int32_t> *segment)
        {
            cache[count++] = segment;
            return count;
        }

        SparseArraySegment<int32_t> *GetSegmentByIndex(byte index)
        {
            Assert(index <= MAX_SIZE);
            return cache[index - 1];
        }

        bool IsNotOverHardLimit()
        {
            return count < MAX_SIZE;
        }

        bool IsNotFull()
        {
            return count < (uint32_t) CONFIG_FLAG(CopyOnAccessArraySegmentCacheSize);
        }

        bool IsValidIndex(uint32_t index)
        {
            return count && index && index <= count;
        }

        uint32_t GetCount()
        {
            return count;
        }
    };
#endif

    class JavascriptLibrary : public JavascriptLibraryBase
    {
        friend class EditAndContinue;
        friend class ScriptSite;
        friend class GlobalObject;
        friend class ScriptContext;
        friend class EngineInterfaceObject;
        friend class ExternalLibraryBase;
        friend class ActiveScriptExternalLibrary;
        friend class IntlEngineInterfaceExtensionObject;
#ifdef ENABLE_JS_BUILTINS
        friend class JsBuiltInEngineInterfaceExtensionObject;
#endif
        friend class ChakraHostScriptContext;
        static const char16_t* domBuiltinPropertyNames[];

    public:
#if ENABLE_COPYONACCESS_ARRAY
        typename WriteBarrierFieldTypeTraits<CacheForCopyOnAccessArraySegments *>::Type cacheForCopyOnAccessArraySegments;
#endif

        static uint32_t GetScriptContextOffset() { return offsetof(JavascriptLibrary, scriptContext); }
        static uint32_t GetUndeclBlockVarOffset() { return offsetof(JavascriptLibrary, undeclBlockVarSentinel); }
        static uint32_t GetEmptyStringOffset() { return offsetof(JavascriptLibrary, emptyString); }
        static uint32_t GetUndefinedValueOffset() { return offsetof(JavascriptLibrary, undefinedValue); }
        static uint32_t GetNullValueOffset() { return offsetof(JavascriptLibrary, nullValue); }
        static uint32_t GetBooleanTrueOffset() { return offsetof(JavascriptLibrary, booleanTrue); }
        static uint32_t GetBooleanFalseOffset() { return offsetof(JavascriptLibrary, booleanFalse); }
        static uint32_t GetNegativeZeroOffset() { return offsetof(JavascriptLibrary, negativeZero); }
        static uint32_t GetNumberTypeStaticOffset() { return offsetof(JavascriptLibrary, numberTypeStatic); }
        static uint32_t GetBigIntTypeStaticOffset() { return offsetof(JavascriptLibrary, bigintTypeStatic); }
        static uint32_t GetObjectTypesOffset() { return offsetof(JavascriptLibrary, objectTypes); }
        static uint32_t GetObjectHeaderInlinedTypesOffset() { return offsetof(JavascriptLibrary, objectHeaderInlinedTypes); }
        static uint32_t GetRegexTypeOffset() { return offsetof(JavascriptLibrary, regexType); }
        static uint32_t GetArrayConstructorOffset() { return offsetof(JavascriptLibrary, arrayConstructor); }
        static uint32_t GetPositiveInfinityOffset() { return offsetof(JavascriptLibrary, positiveInfinite); }
        static uint32_t GetNaNOffset() { return offsetof(JavascriptLibrary, nan); }
        static uint32_t GetNativeIntArrayTypeOffset() { return offsetof(JavascriptLibrary, nativeIntArrayType); }
#if ENABLE_COPYONACCESS_ARRAY
        static uint32_t GetCopyOnAccessNativeIntArrayTypeOffset() { return offsetof(JavascriptLibrary, copyOnAccessNativeIntArrayType); }
#endif
        static uint32_t GetNativeFloatArrayTypeOffset() { return offsetof(JavascriptLibrary, nativeFloatArrayType); }
        static uint32_t GetVTableAddressesOffset() { return offsetof(JavascriptLibrary, vtableAddresses); }
        static uint32_t GetConstructorCacheDefaultInstanceOffset() { return offsetof(JavascriptLibrary, constructorCacheDefaultInstance); }
        static uint32_t GetAbsDoubleCstOffset() { return offsetof(JavascriptLibrary, absDoubleCst); }
        static uint32_t GetUintConvertConstOffset() { return offsetof(JavascriptLibrary, uintConvertConst); }
        static uint32_t GetBuiltinFunctionsOffset() { return offsetof(JavascriptLibrary, builtinFunctions); }
        static uint32_t GetCharStringCacheOffset() { return offsetof(JavascriptLibrary, charStringCache); }
        static uint32_t GetCharStringCacheAOffset() { return GetCharStringCacheOffset() + CharStringCache::GetCharStringCacheAOffset(); }
        PolymorphicInlineCache *GetToStringTagCache() const { return cache.toStringTagCache; }
        const  JavascriptLibraryBase* GetLibraryBase() const { return static_cast<const JavascriptLibraryBase*>(this); }
        void SetGlobalObject(GlobalObject* globalObject) {this->globalObject = globalObject; }
        static uint32_t GetRandSeed0Offset() { return offsetof(JavascriptLibrary, randSeed0); }
        static uint32_t GetRandSeed1Offset() { return offsetof(JavascriptLibrary, randSeed1); }
        static uint32_t GetTypeDisplayStringsOffset() { return offsetof(JavascriptLibrary, typeDisplayStrings); }
        typedef bool (CALLBACK *PromiseContinuationCallback)(Var task, void *callbackState);

        Var GetUndeclBlockVar() const { return undeclBlockVarSentinel; }
        bool IsUndeclBlockVar(Var var) const { return var == undeclBlockVarSentinel; }

        static bool IsTypedArrayConstructor(Var constructor, ScriptContext* scriptContext);

    private:
        typename WriteBarrierFieldTypeTraits<Recycler *, _no_write_barrier_policy, _no_write_barrier_policy>::Type recycler;
        typename WriteBarrierFieldTypeTraits<ExternalLibraryBase*>::Type externalLibraryList;

        typename WriteBarrierFieldTypeTraits<UndeclaredBlockVariable*>::Type undeclBlockVarSentinel;

        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type generatorConstructorPrototypeObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type asyncGeneratorConstructorPrototypeObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type constructorPrototypeObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type heapArgumentsType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type strictHeapArgumentsType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type activationObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type arrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type nativeIntArrayType;
#if ENABLE_COPYONACCESS_ARRAY
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type copyOnAccessNativeIntArrayType;
#endif
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type nativeFloatArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type arrayBufferType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type sharedArrayBufferType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type dataViewType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type int8ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uint8ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uint8ClampedArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type int16ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uint16ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type int32ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uint32ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type float32ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type float64ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type int64ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uint64ArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type boolArrayType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type charArrayType;
        typename WriteBarrierFieldTypeTraits<StaticType *>::Type booleanTypeStatic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type booleanTypeDynamic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type bigintTypeDynamic;
        typename WriteBarrierFieldTypeTraits<StaticType *>::Type bigintTypeStatic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type dateType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type symbolTypeDynamic;
        typename WriteBarrierFieldTypeTraits<StaticType *>::Type symbolTypeStatic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type iteratorResultType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type awaitObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type resumeYieldObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type arrayIteratorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type mapIteratorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type setIteratorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type stringIteratorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type promiseType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type listIteratorType;

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type builtinFunctions[BuiltinFunction::Count];

        typename WriteBarrierFieldTypeTraits<long>::Type vtableAddresses[VTableValue::Count];
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type typeDisplayStrings[TypeIds_Limit];
        typename WriteBarrierFieldTypeTraits<ConstructorCache *>::Type constructorCacheDefaultInstance;
        typename WriteBarrierFieldTypeTraits<const uint8_t *>::Type absDoubleCst;
        typename WriteBarrierFieldTypeTraits<double const *>::Type uintConvertConst;

        // Function Types
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type anonymousFunctionTypeHandler;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type anonymousFunctionWithPrototypeTypeHandler;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type functionTypeHandler;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type functionTypeHandlerWithLength;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type functionWithPrototypeAndLengthTypeHandler;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type functionWithPrototypeTypeHandler;
        typename WriteBarrierFieldTypeTraits<DynamicTypeHandler *>::Type classPrototypeTypeHandler;

        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type externalFunctionWithDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type externalFunctionWithLengthAndDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type wrappedFunctionWithDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type stdCallFunctionWithDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type idMappedFunctionWithPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type externalConstructorFunctionWithDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type defaultExternalConstructorFunctionWithDeferredPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type boundFunctionType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type regexConstructorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type crossSiteDeferredFunctionType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type crossSiteDeferredPrototypeFunctionType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type crossSiteIdMappedFunctionWithPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type crossSiteExternalConstructFunctionWithPrototypeType;

        typename WriteBarrierFieldTypeTraits<StaticType  *>::Type enumeratorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type errorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type evalErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type rangeErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type referenceErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type syntaxErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type typeErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type uriErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type aggregateErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyCompileErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyRuntimeErrorType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyLinkErrorType;
        typename WriteBarrierFieldTypeTraits<StaticType  *>::Type numberTypeStatic;
        typename WriteBarrierFieldTypeTraits<StaticType  *>::Type int64NumberTypeStatic;
        typename WriteBarrierFieldTypeTraits<StaticType  *>::Type uint64NumberTypeStatic;

        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyModuleType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyInstanceType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyMemoryType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type webAssemblyTableType;

        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type numberTypeDynamic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type objectTypes[PreInitializedObjectTypeCount];
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type objectHeaderInlinedTypes[PreInitializedObjectTypeCount];
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type nullPrototypeObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type regexPrototypeType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type regexType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type regexResultType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type stringTypeDynamic;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type mapType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type setType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type weakMapType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type weakSetType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type proxyType;
        typename WriteBarrierFieldTypeTraits<StaticType  *>::Type withType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type SpreadArgumentType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type moduleNamespaceType;
        typename WriteBarrierFieldTypeTraits<PropertyDescriptor>::Type defaultPropertyDescriptor;

        typename WriteBarrierFieldTypeTraits<StringCache>::Type stringCache; // cache string variables once they are used

        // no late caching is needed
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type nullString;
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type emptyString;

        // used implicitly by ChakraFull - don't late cache
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type symbolTypeDisplayString;
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type debuggerDeadZoneBlockVariableString;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type missingPropertyHolder;
        typename WriteBarrierFieldTypeTraits<StaticType*>::Type throwErrorObjectType;
        typename WriteBarrierFieldTypeTraits<PropertyStringCacheMap*>::Type propertyStringMap;
        typename WriteBarrierFieldTypeTraits<SymbolCacheMap*>::Type symbolMap;
        typename WriteBarrierFieldTypeTraits<ConstructorCache*>::Type builtInConstructorCache;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type chakraLibraryObject;

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugObjectFaultInjectionCookieGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugObjectFaultInjectionCookieSetterFunction;

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type evalFunctionObject;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeValuesFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type parseIntFunctionObject;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type parseFloatFunctionObject;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeToStringFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeToLocaleStringFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type identityFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type throwerFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type generatorReturnFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type generatorNextFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type generatorThrowFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncGeneratorNextFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncGeneratorReturnFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncGeneratorThrowFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncFromSyncIteratorNextFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncFromSyncIteratorReturnFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type asyncFromSyncIteratorThrowFunction;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type asyncFromSyncIteratorValueUnwrapFalseFunction;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type asyncFromSyncIteratorValueUnwrapTrueFunction;

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type objectValueOfFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type objectToStringFunction;

#ifdef ENABLE_JS_BUILTINS
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type isArrayFunction;
#endif

#ifdef ENABLE_WASM
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyObject;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type webAssemblyQueryResponseFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type webAssemblyCompileFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type webAssemblyInstantiateBoundFunction;
#endif

        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolMatch;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolReplace;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolSearch;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolSplit;

        typename WriteBarrierFieldTypeTraits<UnifiedRegex::RegexPattern *>::Type emptyRegexPattern;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexExecFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexFlagsGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexGlobalGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexStickyGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexDotAllGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type regexUnicodeGetterFunction;

        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type sharedArrayBufferConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type sharedArrayBufferPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type atomicsObject;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyCompileErrorPrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyCompileErrorConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyRuntimeErrorPrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyRuntimeErrorConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyLinkErrorPrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyLinkErrorConstructor;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyMemoryPrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyMemoryConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyModulePrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyModuleConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyInstancePrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyInstanceConstructor;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type webAssemblyTablePrototype;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type webAssemblyTableConstructor;

        typename WriteBarrierFieldTypeTraits<int>::Type regexConstructorSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexExecSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexFlagsGetterSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexGlobalGetterSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexStickyGetterSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexDotAllGetterSlotIndex;
        typename WriteBarrierFieldTypeTraits<int>::Type regexUnicodeGetterSlotIndex;

        mutable typename WriteBarrierFieldTypeTraits<CharStringCache>::Type charStringCache;

        typename WriteBarrierFieldTypeTraits<PromiseContinuationCallback, _no_write_barrier_policy, _no_write_barrier_policy>::Type nativeHostPromiseContinuationFunction;
        typename WriteBarrierFieldTypeTraits<void *>::Type nativeHostPromiseContinuationFunctionState;

        typedef SList<Js::FunctionProxy*, Recycler> FunctionReferenceList;
        struct JsrtExternalCallbacks
        {
            JsrtExternalCallbacks() : traceCallback(0), finalizeCallback(0), prototype(0) {}
            JsrtExternalCallbacks(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t prototype) : traceCallback(traceCallback), finalizeCallback(finalizeCallback), prototype(prototype) {}

            uintptr_t traceCallback;
            uintptr_t finalizeCallback;
            uintptr_t prototype;

            operator hash_t() const { return (hash_t)(traceCallback ^ finalizeCallback ^ prototype); }
        };
        typedef JsUtil::WeakReferenceDictionary<JsrtExternalCallbacks, DynamicType, DictionarySizePolicy<PowerOf2Policy, 1>> JsrtExternalTypesCache;

        typename WriteBarrierFieldTypeTraits<void *>::Type bindRefChunkBegin;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<void*>::Type*>::Type bindRefChunkCurrent;
        typename WriteBarrierFieldTypeTraits<typename WriteBarrierFieldTypeTraits<void*>::Type*>::Type bindRefChunkEnd;
        typename WriteBarrierFieldTypeTraits<TypePath*>::Type rootPath;         // this should be in library instead of ScriptContext::Cache
        typename WriteBarrierFieldTypeTraits<Js::Cache>::Type cache;
        typename WriteBarrierFieldTypeTraits<FunctionReferenceList*>::Type dynamicFunctionReference;
        typename WriteBarrierFieldTypeTraits<uint>::Type dynamicFunctionReferenceDepth;
        typename WriteBarrierFieldTypeTraits<FinalizableObject*>::Type jsrtContextObject;
        typename WriteBarrierFieldTypeTraits<JsrtExternalTypesCache*>::Type jsrtExternalTypesCache;
        typename WriteBarrierFieldTypeTraits<FunctionBody*>::Type fakeGlobalFuncForUndefer;

        struct CustomExternalWrapperCallbacks
        {
            CustomExternalWrapperCallbacks() : traceCallback(0), finalizeCallback(0), interceptors(0), prototype(0) {}
            CustomExternalWrapperCallbacks(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t interceptors, uintptr_t prototype) : traceCallback(traceCallback), finalizeCallback(finalizeCallback), interceptors(interceptors), prototype(prototype) {}
            uintptr_t traceCallback;
            uintptr_t finalizeCallback;
            uintptr_t interceptors;
            uintptr_t prototype;

            operator hash_t() const { return (hash_t)(traceCallback ^ finalizeCallback ^ interceptors ^ prototype); }
        };
        typedef JsUtil::WeakReferenceDictionary<CustomExternalWrapperCallbacks, DynamicType, DictionarySizePolicy<PowerOf2Policy, 1>> CustomExternalWrapperTypesCache;

        typename WriteBarrierFieldTypeTraits<CustomExternalWrapperTypesCache*>::Type customExternalWrapperTypesCache;

        typename WriteBarrierFieldTypeTraits<ModuleRecordList*>::Type moduleRecordList;

        typename WriteBarrierFieldTypeTraits<OnlyWritablePropertyProtoChainCache>::Type typesWithOnlyWritablePropertyProtoChain;
        typename WriteBarrierFieldTypeTraits<NoSpecialPropertyProtoChainCache>::Type typesWithNoSpecialPropertyProtoChain;

        typename WriteBarrierFieldTypeTraits<unsigned long>::Type randSeed0, randSeed1;
        typename WriteBarrierFieldTypeTraits<bool>::Type isPRNGSeeded;
        typename WriteBarrierFieldTypeTraits<bool>::Type inProfileMode;
        typename WriteBarrierFieldTypeTraits<bool>::Type inDispatchProfileMode;
        typename WriteBarrierFieldTypeTraits<bool>::Type arrayObjectHasUserDefinedSpecies;

        JavascriptFunction * AddFunctionToLibraryObjectWithPrototype(DynamicObject * object, PropertyId propertyId, FunctionInfo * functionInfo, int length, DynamicObject * prototype = nullptr, DynamicType * functionType = nullptr);
        JavascriptFunction * AddFunctionToLibraryObject(DynamicObject* object, PropertyId propertyId, FunctionInfo * functionInfo, int length, PropertyAttributes attributes = PropertyBuiltInMethodDefaults);

        JavascriptFunction * AddFunctionToLibraryObjectWithName(DynamicObject* object, PropertyId propertyId, PropertyId nameId, FunctionInfo * functionInfo, int length);
        RuntimeFunction* AddGetterToLibraryObject(DynamicObject* object, PropertyId propertyId, FunctionInfo* functionInfo);
        void AddAccessorsToLibraryObject(DynamicObject* object, PropertyId propertyId, FunctionInfo * getterFunctionInfo, FunctionInfo * setterFunctionInfo);
        void AddAccessorsToLibraryObject(DynamicObject* object, PropertyId propertyId, RecyclableObject * getterFunction, RecyclableObject * setterFunction);
        void AddAccessorsToLibraryObjectWithName(DynamicObject* object, PropertyId propertyId, PropertyId nameId, FunctionInfo * getterFunctionInfo, FunctionInfo * setterFunction);
        void AddSpeciesAccessorsToLibraryObject(DynamicObject* object, FunctionInfo * getterFunctionInfo);
        RuntimeFunction * CreateGetterFunction(PropertyId nameId, FunctionInfo* functionInfo);
        RuntimeFunction * CreateSetterFunction(PropertyId nameId, FunctionInfo* functionInfo);

        template <size_t N>
        JavascriptFunction * AddFunctionToLibraryObjectWithPropertyName(DynamicObject* object, const char16_t(&propertyName)[N], FunctionInfo * functionInfo, int length);

        static SimpleTypeHandler<1> SharedPrototypeTypeHandler;
        static SimpleTypeHandler<1> SharedFunctionWithoutPrototypeTypeHandler;
        static SimpleTypeHandler<1> SharedFunctionWithPrototypeTypeHandlerV11;
        static SimpleTypeHandler<2> SharedFunctionWithPrototypeTypeHandler;
        static SimpleTypeHandler<1> SharedFunctionWithConfigurableLengthTypeHandler;
        static SimpleTypeHandler<1> SharedFunctionWithLengthTypeHandler;
        static SimpleTypeHandler<2> SharedFunctionWithLengthAndNameTypeHandler;
        static SimpleTypeHandler<2> SharedIdMappedFunctionWithPrototypeTypeHandler;
        static SimpleTypeHandler<1> SharedNamespaceSymbolTypeHandler;
        static SimpleTypeHandler<3> SharedFunctionWithPrototypeLengthAndNameTypeHandler;
        static SimpleTypeHandler<2> SharedFunctionWithPrototypeAndLengthTypeHandler;
        static SimpleTypeHandler<2> SharedFunctionWithNonWritablePrototypeAndLengthTypeHandler;
        static SimpleTypeHandler<3> SharedFunctionWithNonWritablePrototypeLengthAndNameTypeHandler;
        static MissingPropertyTypeHandler MissingPropertyHolderTypeHandler;

        static SimplePropertyDescriptor const SharedFunctionPropertyDescriptors[2];
        static SimplePropertyDescriptor const SharedIdMappedFunctionPropertyDescriptors[2];
        static SimplePropertyDescriptor const HeapArgumentsPropertyDescriptors[3];
        static SimplePropertyDescriptor const FunctionWithLengthAndPrototypeTypeDescriptors[2];
        static SimplePropertyDescriptor const FunctionWithLengthAndNameTypeDescriptors[2];
        static SimplePropertyDescriptor const FunctionWithPrototypeLengthAndNameTypeDescriptors[3];
        static SimplePropertyDescriptor const FunctionWithPrototypeAndLengthTypeDescriptors[2];
        static SimplePropertyDescriptor const FunctionWithNonWritablePrototypeAndLengthTypeDescriptors[2];
        static SimplePropertyDescriptor const FunctionWithNonWritablePrototypeLengthAndNameTypeDescriptors[3];
        static SimplePropertyDescriptor const ModuleNamespaceTypeDescriptors[1];
        static SimplePropertyDescriptor const ClassPrototypePropertyDescriptors[1];

    public:


        static const ObjectInfoBits EnumFunctionClass = EnumClass_1_Bit;

        static void InitializeProperties(ThreadContext * threadContext);

        JavascriptLibrary(GlobalObject* globalObject, Recycler * recycler) :
            JavascriptLibraryBase(globalObject),
            inProfileMode(false),
            inDispatchProfileMode(false),
            propertyStringMap(nullptr),
            symbolMap(nullptr),
            parseIntFunctionObject(nullptr),
            evalFunctionObject(nullptr),
            parseFloatFunctionObject(nullptr),
            arrayPrototypeToLocaleStringFunction(nullptr),
            arrayPrototypeToStringFunction(nullptr),
            identityFunction(nullptr),
            throwerFunction(nullptr),
            jsrtContextObject(nullptr),
            jsrtExternalTypesCache(nullptr),
            customExternalWrapperTypesCache(nullptr),
            fakeGlobalFuncForUndefer(nullptr),
            externalLibraryList(nullptr),
#if ENABLE_COPYONACCESS_ARRAY
            cacheForCopyOnAccessArraySegments(nullptr),
#endif
            referencedPropertyRecords(nullptr),
            moduleRecordList(nullptr),
            rootPath(nullptr),
            bindRefChunkBegin(nullptr),
            bindRefChunkCurrent(nullptr),
            bindRefChunkEnd(nullptr),
            dynamicFunctionReference(nullptr),
            typesWithOnlyWritablePropertyProtoChain(recycler),
            typesWithNoSpecialPropertyProtoChain(recycler)
        {
            this->globalObject = globalObject;
        }

        void Initialize(ScriptContext* scriptContext, GlobalObject * globalObject);
        void Uninitialize();
        GlobalObject* GetGlobalObject() const { return globalObject; }
        ScriptContext* GetScriptContext() const { return scriptContext; }

        Recycler * GetRecycler() const { return recycler; }
        Var GetTrueOrFalse(BOOL value) { return value ? booleanTrue : booleanFalse; }
        JavascriptSymbol* GetSymbolMatch() { return symbolMatch; }
        JavascriptSymbol* GetSymbolReplace() { return symbolReplace; }
        JavascriptSymbol* GetSymbolSearch() { return symbolSearch; }
        JavascriptSymbol* GetSymbolSplit() { return symbolSplit; }
        JavascriptSymbol* GetSymbolSpecies() { return symbolSpecies; }
        JavascriptString* GetNullString() { return nullString; }
        JavascriptString* GetEmptyString() const;

#define STRING(name, str) JavascriptString* Get##name##String() { return stringCache.Get##name(); }
#define PROPERTY_STRING(name, str) STRING(name, str)
#include "StringCacheList.h"
#undef PROPERTY_STRING
#undef STRING

        JavascriptString* GetSymbolTypeDisplayString() const { return symbolTypeDisplayString; }
        JavascriptString* GetDebuggerDeadZoneBlockVariableString() { Assert(debuggerDeadZoneBlockVariableString); return debuggerDeadZoneBlockVariableString; }
        JavascriptRegExp* CreateEmptyRegExp();
        JavascriptFunction* GetEvalFunctionObject() { return evalFunctionObject; }
        JavascriptFunction* GetArrayIteratorPrototypeBuiltinNextFunction() { return arrayIteratorPrototypeBuiltinNextFunction; }
        DynamicObject* GetReflectObject() const { return reflectObject; }
        const PropertyDescriptor* GetDefaultPropertyDescriptor() const { return &defaultPropertyDescriptor; }
        DynamicObject* GetMissingPropertyHolder() const { return missingPropertyHolder; }

        JavascriptFunction* GetSharedArrayBufferConstructor() { return sharedArrayBufferConstructor; }
        DynamicObject* GetAtomicsObject() { return atomicsObject; }

        DynamicObject* GetWebAssemblyCompileErrorPrototype() const { return webAssemblyCompileErrorPrototype; }
        DynamicObject* GetWebAssemblyCompileErrorConstructor() const { return webAssemblyCompileErrorConstructor; }
        DynamicObject* GetWebAssemblyRuntimeErrorPrototype() const { return webAssemblyRuntimeErrorPrototype; }
        DynamicObject* GetWebAssemblyRuntimeErrorConstructor() const { return webAssemblyRuntimeErrorConstructor; }
        DynamicObject* GetWebAssemblyLinkErrorPrototype() const { return webAssemblyLinkErrorPrototype; }
        DynamicObject* GetWebAssemblyLinkErrorConstructor() const { return webAssemblyLinkErrorConstructor; }

        DynamicObject* GetAsyncGeneratorPrototype() const { return asyncGeneratorPrototype; }
        DynamicObject* GetGeneratorPrototype() const { return generatorPrototype; }

        DynamicObject* GetChakraLib() const { return chakraLibraryObject; }

#if ENABLE_TTD
        Js::PropertyId ExtractPrimitveSymbolId_TTD(Var value);
        Js::RecyclableObject* CreatePrimitveSymbol_TTD(Js::PropertyId pid);
        Js::RecyclableObject* CreatePrimitveSymbol_TTD(Js::JavascriptString* str);

        Js::RecyclableObject* CreateDefaultBoxedObject_TTD(Js::TypeId kind);
        void SetBoxedObjectValue_TTD(Js::RecyclableObject* obj, Js::Var value);

        Js::RecyclableObject* CreateDate_TTD(double value);
        Js::RecyclableObject* CreateRegex_TTD(const char16_t* patternSource, uint32_t patternLength, UnifiedRegex::RegexFlags flags, CharCount lastIndex, Js::Var lastVar);
        Js::RecyclableObject* CreateError_TTD();

        Js::RecyclableObject* CreateES5Array_TTD();
        static void SetLengthWritableES5Array_TTD(Js::RecyclableObject* es5Array, bool isLengthWritable);

        Js::RecyclableObject* CreateSet_TTD();
        Js::RecyclableObject* CreateWeakSet_TTD();
        static void AddSetElementInflate_TTD(Js::JavascriptSet* set, Var value);
        static void AddWeakSetElementInflate_TTD(Js::JavascriptWeakSet* set, Var value);

        Js::RecyclableObject* CreateMap_TTD();
        Js::RecyclableObject* CreateWeakMap_TTD();
        static void AddMapElementInflate_TTD(Js::JavascriptMap* map, Var key, Var value);
        static void AddWeakMapElementInflate_TTD(Js::JavascriptWeakMap* map, Var key, Var value);

        Js::RecyclableObject* CreateExternalFunction_TTD(Js::Var fname);
        Js::RecyclableObject* CreateBoundFunction_TTD(
                RecyclableObject* function, Var bThis, uint32_t ct, typename WriteBarrierFieldTypeTraits<Var>::Type* args);

        Js::RecyclableObject* CreateProxy_TTD(RecyclableObject* handler, RecyclableObject* target);
        Js::RecyclableObject* CreateRevokeFunction_TTD(RecyclableObject* proxy);

        Js::RecyclableObject* CreateHeapArguments_TTD(uint32_t numOfArguments, uint32_t formalCount, ActivationObject* frameObject, byte* deletedArray);
        Js::RecyclableObject* CreateES5HeapArguments_TTD(uint32_t numOfArguments, uint32_t formalCount, ActivationObject* frameObject, byte* deletedArray);

        Js::JavascriptPromiseCapability* CreatePromiseCapability_TTD(Var promise, Var resolve, Var reject);
        Js::JavascriptPromiseReaction* CreatePromiseReaction_TTD(RecyclableObject* handler, JavascriptPromiseCapability* capabilities);

        Js::RecyclableObject* CreatePromise_TTD(uint32_t status, bool isHandled, Var result, SList<Js::JavascriptPromiseReaction*, HeapAllocator>& resolveReactions, SList<Js::JavascriptPromiseReaction*, HeapAllocator>& rejectReactions);
        JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* CreateAlreadyDefinedWrapper_TTD(bool alreadyDefined);
        Js::RecyclableObject* CreatePromiseResolveOrRejectFunction_TTD(RecyclableObject* promise, bool isReject, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyResolved);
        Js::RecyclableObject* CreatePromiseReactionTaskFunction_TTD(JavascriptPromiseReaction* reaction, Var argument);

        Js::JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* CreateRemainingElementsWrapper_TTD(Js::ScriptContext* ctx, uint32_t value);
        Js::JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* CreateAlreadyCalledWrapper_TTD(Js::ScriptContext* ctx, bool value);
        Js::RecyclableObject* CreatePromiseAllResolveElementFunction_TTD(Js::JavascriptPromiseCapability* capabilities, uint32_t index, Js::JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* wrapper, Js::RecyclableObject* values, bool alreadyCalled);
        Js::RecyclableObject* CreatePromiseAllSettledResolveOrRejectElementFunction_TTD(Js::JavascriptPromiseCapability* capabilities, uint32_t index, Js::JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* wrapper, Js::RecyclableObject* values, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyCalledWrapper, bool isRejecting);
        Js::RecyclableObject* CreateJavascriptGenerator_TTD(Js::ScriptContext *ctx,
                                                            Js::RecyclableObject *prototype, Js::Arguments &arguments,
                                                            Js::JavascriptGenerator::GeneratorState generatorState);
#endif

        void ResetIntlObject();
        void EnsureIntlObjectReady();
        template <class Fn>
        void InitializeIntlForPrototypes(Fn fn);
        void InitializeIntlForStringPrototype();
        void InitializeIntlForDatePrototype();
        void InitializeIntlForNumberPrototype();

#ifdef ENABLE_JS_BUILTINS
        void EnsureArrayBuiltInsAreReady();
        void EnsureMathBuiltInsAreReady();
        static bool InitializeChakraLibraryObject(DynamicObject* chakraLibraryObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

#endif

        DynamicType * GetDebugDisposableObjectType() { return debugDisposableObjectType; }
        DynamicType * GetDebugFuncExecutorInDisposeObjectType() { return debugFuncExecutorInDisposeObjectType; }

        DynamicType* GetErrorType(ErrorTypeEnum typeToCreate) const;
        StaticType  * GetBooleanTypeStatic() const { return booleanTypeStatic; }
        DynamicType * GetBooleanTypeDynamic() const { return booleanTypeDynamic; }
        DynamicType * GetDateType() const { return dateType; }
        StaticType * GetBigIntTypeStatic() const { return bigintTypeStatic; }
        DynamicType * GetBigIntTypeDynamic() const { return bigintTypeDynamic; }
        DynamicType * GetBoundFunctionType() const { return boundFunctionType; }
        DynamicType * GetRegExpConstructorType() const { return regexConstructorType; }
        StaticType  * GetEnumeratorType() const { return enumeratorType; }
        DynamicType * GetSpreadArgumentType() const { return SpreadArgumentType; }
        StaticType  * GetWithType() const { return withType; }
        DynamicType * GetErrorType() const { return errorType; }
        DynamicType * GetEvalErrorType() const { return evalErrorType; }
        DynamicType * GetRangeErrorType() const { return rangeErrorType; }
        DynamicType * GetReferenceErrorType() const { return referenceErrorType; }
        DynamicType * GetSyntaxErrorType() const { return syntaxErrorType; }
        DynamicType * GetTypeErrorType() const { return typeErrorType; }
        DynamicType * GetURIErrorType() const { return uriErrorType; }
        DynamicType * GetAggregateErrorType() const { return aggregateErrorType; }
        DynamicType * GetWebAssemblyCompileErrorType() const { return webAssemblyCompileErrorType; }
        DynamicType * GetWebAssemblyRuntimeErrorType() const { return webAssemblyRuntimeErrorType; }
        DynamicType * GetWebAssemblyLinkErrorType() const { return webAssemblyLinkErrorType; }
        StaticType  * GetNumberTypeStatic() const { return numberTypeStatic; }
        StaticType  * GetInt64TypeStatic() const { return int64NumberTypeStatic; }
        StaticType  * GetUInt64TypeStatic() const { return uint64NumberTypeStatic; }

        DynamicType * GetNumberTypeDynamic() const { return numberTypeDynamic; }
        DynamicType * GetPromiseType() const { return promiseType; }

        DynamicType * GetWebAssemblyModuleType()  const { return webAssemblyModuleType; }
        DynamicType * GetWebAssemblyInstanceType()  const { return webAssemblyInstanceType; }
        DynamicType * GetWebAssemblyMemoryType() const { return webAssemblyMemoryType; }
        DynamicType * GetWebAssemblyTableType() const { return webAssemblyTableType; }
        DynamicType * GetGeneratorConstructorPrototypeObjectType() const { return generatorConstructorPrototypeObjectType; }
        DynamicType * GetAsyncGeneratorConstructorPrototypeObjectType() const { return asyncGeneratorConstructorPrototypeObjectType; }
        DynamicType * GetResumeYieldObjectType() const { return resumeYieldObjectType; }
        DynamicType * GetAwaitObjectType() const { return awaitObjectType; }

#ifdef ENABLE_WASM
        JavascriptFunction* GetWebAssemblyQueryResponseFunction() const { return webAssemblyQueryResponseFunction; }
        JavascriptFunction* GetWebAssemblyCompileFunction() const { return webAssemblyCompileFunction; }
        JavascriptFunction* GetWebAssemblyInstantiateBoundFunction() const { return webAssemblyInstantiateBoundFunction; }
#endif
        
        DynamicType * GetObjectLiteralType(uint16 requestedInlineSlotCapacity);
        DynamicType * GetObjectHeaderInlinedLiteralType(uint16 requestedInlineSlotCapacity);
        DynamicType * GetObjectType() const { return objectTypes[0]; }
        DynamicType * GetNullPrototypeObjectType() const { return nullPrototypeObjectType; }
        DynamicType * GetObjectHeaderInlinedType() const { return objectHeaderInlinedTypes[0]; }
        StaticType  * GetSymbolTypeStatic() const { return symbolTypeStatic; }
        DynamicType * GetSymbolTypeDynamic() const { return symbolTypeDynamic; }
        DynamicType * GetProxyType() const { return proxyType; }
        DynamicType * GetHeapArgumentsObjectType() const { return heapArgumentsType; }
        DynamicType * GetActivationObjectType() const { return activationObjectType; }
        DynamicType * GetModuleNamespaceType() const { return moduleNamespaceType; }
        DynamicType * GetArrayType() const { return arrayType; }
        DynamicType * GetNativeIntArrayType() const { return nativeIntArrayType; }
#if ENABLE_COPYONACCESS_ARRAY
        DynamicType * GetCopyOnAccessNativeIntArrayType() const { return copyOnAccessNativeIntArrayType; }
#endif
        DynamicType * GetNativeFloatArrayType() const { return nativeFloatArrayType; }
        DynamicType * GetRegexPrototypeType() const { return regexPrototypeType; }
        DynamicType * GetRegexType() const { return regexType; }
        DynamicType * GetRegexResultType() const { return regexResultType; }
        DynamicType * GetArrayBufferType() const { return arrayBufferType; }
        StaticType  * GetStringTypeStatic() const { return stringCache.GetStringTypeStatic(); }
        DynamicType * GetStringTypeDynamic() const { return stringTypeDynamic; }
        void EnsureDebugObject(DynamicObject* newDebugObject);
        DynamicObject* GetDebugObject() const { Assert(debugObject != nullptr); return debugObject; }
        DynamicType * GetMapType() const { return mapType; }
        DynamicType * GetSetType() const { return setType; }
        DynamicType * GetWeakMapType() const { return weakMapType; }
        DynamicType * GetWeakSetType() const { return weakSetType; }
        DynamicType * GetArrayIteratorType() const { return arrayIteratorType; }
        DynamicType * GetMapIteratorType() const { return mapIteratorType; }
        DynamicType * GetSetIteratorType() const { return setIteratorType; }
        DynamicType * GetStringIteratorType() const { return stringIteratorType; }
        DynamicType * GetListIteratorType() const { return listIteratorType; }
        JavascriptFunction* GetDefaultAccessorFunction() const { return defaultAccessorFunction; }
        JavascriptFunction* GetStackTraceAccessorFunction() const { return stackTraceAccessorFunction; }
        JavascriptFunction* GetThrowTypeErrorRestrictedPropertyAccessorFunction() const { return throwTypeErrorRestrictedPropertyAccessorFunction; }
        JavascriptFunction* Get__proto__getterFunction() const { return __proto__getterFunction; }
        JavascriptFunction* Get__proto__setterFunction() const { return __proto__setterFunction; }

        JavascriptFunction* GetObjectValueOfFunction() const { return objectValueOfFunction; }
        JavascriptFunction* GetObjectToStringFunction() const { return objectToStringFunction; }

        JavascriptFunction* GetDebugObjectNonUserGetterFunction() const { return debugObjectNonUserGetterFunction; }
        JavascriptFunction* GetDebugObjectNonUserSetterFunction() const { return debugObjectNonUserSetterFunction; }

        UnifiedRegex::RegexPattern * GetEmptyRegexPattern() const { return emptyRegexPattern; }
        JavascriptFunction* GetRegexExecFunction() const { return regexExecFunction; }
        JavascriptFunction* GetRegexFlagsGetterFunction() const { return regexFlagsGetterFunction; }
        JavascriptFunction* GetRegexGlobalGetterFunction() const { return regexGlobalGetterFunction; }
        JavascriptFunction* GetRegexStickyGetterFunction() const { return regexStickyGetterFunction; }
        JavascriptFunction* GetRegexDotAllGetterFunction() const { return regexDotAllGetterFunction; }
        JavascriptFunction* GetRegexUnicodeGetterFunction() const { return regexUnicodeGetterFunction; }

        int GetRegexConstructorSlotIndex() const { return regexConstructorSlotIndex;  }
        int GetRegexExecSlotIndex() const { return regexExecSlotIndex;  }
        int GetRegexFlagsGetterSlotIndex() const { return regexFlagsGetterSlotIndex;  }
        int GetRegexGlobalGetterSlotIndex() const { return regexGlobalGetterSlotIndex;  }
        int GetRegexStickyGetterSlotIndex() const { return regexStickyGetterSlotIndex;  }
        int GetRegexDotAllGetterSlotIndex() const { return regexDotAllGetterSlotIndex;  }
        int GetRegexUnicodeGetterSlotIndex() const { return regexUnicodeGetterSlotIndex;  }

        TypePath* GetRootPath() const { return rootPath; }
        void BindReference(void * addr);
        void CleanupForClose();
        void BeginDynamicFunctionReferences();
        void EndDynamicFunctionReferences();
        void RegisterDynamicFunctionReference(FunctionProxy* func);

        void SetDebugObjectNonUserAccessor(FunctionInfo *funcGetter, FunctionInfo *funcSetter);

        JavascriptFunction* GetDebugObjectDebugModeGetterFunction() const { return debugObjectDebugModeGetterFunction; }
        void SetDebugObjectDebugModeAccessor(FunctionInfo *funcGetter);

        JavascriptFunction* GetDebugObjectFaultInjectionCookieGetterFunction() const { return debugObjectFaultInjectionCookieGetterFunction; }
        JavascriptFunction* GetDebugObjectFaultInjectionCookieSetterFunction() const { return debugObjectFaultInjectionCookieSetterFunction; }
        void SetDebugObjectFaultInjectionCookieGetterAccessor(FunctionInfo *funcGetter, FunctionInfo *funcSetter);

        JavascriptFunction* GetArrayPrototypeToStringFunction() const { return arrayPrototypeToStringFunction; }
        JavascriptFunction* GetArrayPrototypeToLocaleStringFunction() const { return arrayPrototypeToLocaleStringFunction; }
        JavascriptFunction* GetIdentityFunction() const { return identityFunction; }
        JavascriptFunction* GetThrowerFunction() const { return throwerFunction; }

        void SetNativeHostPromiseContinuationFunction(PromiseContinuationCallback function, void *state);

        void CallNativeHostPromiseRejectionTracker(Var promise, Var reason, bool handled);

        void SetJsrtContext(FinalizableObject* jsrtContext);
        FinalizableObject* GetJsrtContext();
        void EnqueueTask(Var taskVar);

        HeapArgumentsObject* CreateHeapArguments(Var frameObj, uint formalCount, bool isStrictMode = false);
        JavascriptArray* CreateArray();
        JavascriptArray* CreateArray(uint32_t length);
        JavascriptArray *CreateArrayOnStack(void *const stackAllocationPointer);
        JavascriptNativeIntArray* CreateNativeIntArray();
        JavascriptNativeIntArray* CreateNativeIntArray(uint32_t length);
#if ENABLE_COPYONACCESS_ARRAY
        JavascriptCopyOnAccessNativeIntArray* CreateCopyOnAccessNativeIntArray();
        JavascriptCopyOnAccessNativeIntArray* CreateCopyOnAccessNativeIntArray(uint32_t length);
#endif
        JavascriptNativeFloatArray* CreateNativeFloatArray();
        JavascriptNativeFloatArray* CreateNativeFloatArray(uint32_t length);
        JavascriptArray* CreateArray(uint32_t length, uint32_t size);
        ArrayBuffer* CreateArrayBuffer(uint32_t length);
        ArrayBuffer* CreateArrayBuffer(byte* buffer, uint32_t length);
        ArrayBuffer* CreateArrayBuffer(RefCountedBuffer* buffer, uint32_t length);
#ifdef ENABLE_WASM
        class WebAssemblyArrayBuffer* CreateWebAssemblyArrayBuffer(uint32_t length);
        class WebAssemblyArrayBuffer* CreateWebAssemblyArrayBuffer(byte* buffer, uint32_t length);
#ifdef ENABLE_WASM_THREADS
        class WebAssemblySharedArrayBuffer* CreateWebAssemblySharedArrayBuffer(uint32_t length, uint32_t maxLength);
        class WebAssemblySharedArrayBuffer* CreateWebAssemblySharedArrayBuffer(SharedContents *contents);
#endif
#endif
        SharedArrayBuffer* CreateSharedArrayBuffer(uint32_t length);
        SharedArrayBuffer* CreateSharedArrayBuffer(SharedContents *contents);
        ArrayBuffer* CreateProjectionArraybuffer(uint32_t length);
        ArrayBuffer* CreateProjectionArraybuffer(byte* buffer, uint32_t length);
        ArrayBuffer* CreateProjectionArraybuffer(RefCountedBuffer* buffer, uint32_t length);
        ArrayBuffer* CreateExternalArrayBuffer(RefCountedBuffer* buffer, uint32_t length);
        DataView* CreateDataView(ArrayBufferBase* arrayBuffer, uint32_t offSet, uint32_t mappedLength);

        template <typename TypeName, bool clamped>
        inline DynamicType* GetTypedArrayType(TypeName);

        template<> inline DynamicType* GetTypedArrayType<int8_t,false>(int8_t) { return int8ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<uint8_t,false>(uint8_t) { return uint8ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<uint8_t,true>(uint8_t) { return uint8ClampedArrayType; };
        template<> inline DynamicType* GetTypedArrayType<int16,false>(int16) { return int16ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<uint16,false>(uint16) { return uint16ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<int32_t,false>(int32_t) { return int32ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<uint32_t,false>(uint32_t) { return uint32ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<float,false>(float) { return float32ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<double,false>(double) { return float64ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<long,false>(long) { return int64ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<unsigned long,false>(unsigned long) { return uint64ArrayType; };
        template<> inline DynamicType* GetTypedArrayType<bool,false>(bool) { return boolArrayType; };

        DynamicType* GetCharArrayType() { return charArrayType; };

        //
        // This method would be used for creating array literals, when we really need to create a huge array
        // Avoids checks at runtime.
        //
        JavascriptArray*            CreateArrayLiteral(uint32_t length);
        JavascriptNativeIntArray*   CreateNativeIntArrayLiteral(uint32_t length);

#if ENABLE_PROFILE_INFO
        JavascriptNativeIntArray*   CreateCopyOnAccessNativeIntArrayLiteral(ArrayCallSiteInfo *arrayInfo, FunctionBody *functionBody, const Js::AuxArray<int32_t> *ints);
#endif

        JavascriptNativeFloatArray* CreateNativeFloatArrayLiteral(uint32_t length);

        JavascriptBoolean* CreateBoolean(BOOL value);
        JavascriptDate* CreateDate();
        JavascriptDate* CreateDate(double value);
        JavascriptDate* CreateDate(SYSTEMTIME* pst);
        JavascriptMap* CreateMap();
        JavascriptSet* CreateSet();
        JavascriptWeakMap* CreateWeakMap();
        JavascriptWeakSet* CreateWeakSet();
        JavascriptError* CreateError();
        JavascriptError* CreateError(DynamicType* errorType, BOOL isExternal = FALSE);
        JavascriptError* CreateExternalError(ErrorTypeEnum errorTypeEnum);
        JavascriptError* CreateEvalError();
        JavascriptError* CreateRangeError();
        JavascriptError* CreateReferenceError();
        JavascriptError* CreateSyntaxError();
        JavascriptError* CreateTypeError();
        JavascriptError* CreateURIError();
        JavascriptError* CreateAggregateError();
        JavascriptError* CreateStackOverflowError();
        JavascriptError* CreateOutOfMemoryError();
        JavascriptError* CreateWebAssemblyCompileError();
        JavascriptError* CreateWebAssemblyRuntimeError();
        JavascriptError* CreateWebAssemblyLinkError();
        JavascriptSymbol* CreateSymbol(JavascriptString* description);
        JavascriptSymbol* CreateSymbol(const char16_t* description, int descriptionLength);
        JavascriptSymbol* CreateSymbol(const PropertyRecord* propertyRecord);
        JavascriptPromise* CreatePromise();
        JavascriptGenerator* CreateGenerator(Arguments& args, ScriptFunction* scriptFunction, RecyclableObject* prototype);
        JavascriptAsyncGenerator* CreateAsyncGenerator(Arguments& args, ScriptFunction* scriptFunction, RecyclableObject* prototype);
        JavascriptAsyncFromSyncIterator* CreateAsyncFromSyncIterator(RecyclableObject* syncIterator);
        JavascriptFunction* CreateNonProfiledFunction(FunctionInfo * functionInfo);
        template <class MethodType>
        JavascriptExternalFunction* CreateIdMappedExternalFunction(MethodType entryPoint, DynamicType *pPrototypeType);
        JavascriptExternalFunction* CreateExternalConstructor(Js::ExternalMethod entryPoint, PropertyId nameId, RecyclableObject * prototype);
        JavascriptExternalFunction* CreateExternalConstructor(Js::ExternalMethod entryPoint, PropertyId nameId, InitializeMethod method, unsigned short deferredTypeSlots, bool hasAccessors);
        DynamicType* GetCachedCustomExternalWrapperType(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t interceptors, uintptr_t prototype);
        void CacheCustomExternalWrapperType(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t interceptors, uintptr_t prototype, DynamicType* dynamicType);

        JsrtExternalType* GetCachedJsrtExternalType(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t prototype);
        void CacheJsrtExternalType(uintptr_t traceCallback, uintptr_t finalizeCallback, uintptr_t prototype, JsrtExternalType* dynamicType);
        static DynamicTypeHandler * GetDeferredPrototypeGeneratorFunctionTypeHandler(ScriptContext* scriptContext);
        static DynamicTypeHandler * GetDeferredPrototypeAsyncFunctionTypeHandler(ScriptContext* scriptContext);
        DynamicType * CreateDeferredPrototypeAsyncGeneratorFunctionType(JavascriptMethod entrypoint, bool isAnonymousFunction, bool isShared = false);
        DynamicType * CreateDeferredPrototypeGeneratorFunctionType(JavascriptMethod entrypoint, bool isAnonymousFunction, bool isShared = false);
        DynamicType * CreateDeferredPrototypeAsyncFunctionType(JavascriptMethod entrypoint, bool isAnonymousFunction, bool isShared = false);

        static DynamicTypeHandler * GetDeferredPrototypeFunctionTypeHandler(ScriptContext* scriptContext);
        static DynamicTypeHandler * GetDeferredPrototypeFunctionWithLengthTypeHandler(ScriptContext* scriptContext);
        static DynamicTypeHandler * GetDeferredPrototypeAsyncGeneratorFunctionTypeHandler();
        static DynamicTypeHandler * GetDeferredAnonymousPrototypeFunctionWithLengthTypeHandler();
        static DynamicTypeHandler * GetDeferredAnonymousPrototypeGeneratorFunctionTypeHandler();
        static DynamicTypeHandler * GetDeferredAnonymousPrototypeAsyncFunctionTypeHandler();
        static DynamicTypeHandler * GetDeferredAnonymousPrototypeAsyncGeneratorFunctionTypeHandler();

        DynamicTypeHandler * GetDeferredFunctionTypeHandler();
        DynamicTypeHandler * GetDeferredFunctionWithLengthTypeHandler();
        DynamicTypeHandler* GetDeferredFunctionWithLengthUnsetTypeHandler();
        DynamicTypeHandler * GetDeferredPrototypeFunctionWithNameAndLengthTypeHandler();
        DynamicTypeHandler * ScriptFunctionTypeHandler(bool noPrototypeProperty, bool isAnonymousFunction);
        DynamicTypeHandler * ClassConstructorTypeHandler();
        DynamicTypeHandler * AnonymousClassConstructorTypeHandler();
        DynamicTypeHandler * GetDeferredAnonymousFunctionWithLengthTypeHandler();
        DynamicTypeHandler * GetDeferredAnonymousFunctionTypeHandler();
        template<bool isNameAvailable, bool isPrototypeAvailable = true, bool isLengthAvailable = false, bool addLength = isLengthAvailable>
        static DynamicTypeHandler * GetDeferredFunctionTypeHandlerBase();
        template<bool isNameAvailable, bool isPrototypeAvailable = true>
        static DynamicTypeHandler * GetDeferredGeneratorFunctionTypeHandlerBase();
        template<bool isNameAvailable, bool isPrototypeAvailable = true>
        static DynamicTypeHandler * GetDeferredAsyncGeneratorFunctionTypeHandlerBase();
        template<bool isNameAvailable>
        static DynamicTypeHandler * GetDeferredAsyncFunctionTypeHandlerBase();

        DynamicType * CreateDeferredFunctionType(JavascriptMethod entrypoint);
        DynamicType * CreateDeferredPrototypeFunctionType(JavascriptMethod entrypoint);
        DynamicType * CreateDeferredPrototypeFunctionTypeNoProfileThunk(JavascriptMethod entrypoint, bool isShared = false);
        DynamicType * CreateDeferredFunctionTypeNoProfileThunk(JavascriptMethod entrypoint, bool isShared = false);
        DynamicType * CreateDeferredLengthPrototypeFunctionTypeNoProfileThunk(JavascriptMethod entrypoint, bool isShared = false);
        DynamicType * CreateDeferredLengthFunctionTypeNoProfileThunk(JavascriptMethod entrypoint, bool isShared = false);
        template<bool isLengthAvailable, bool isPrototypeAvailable> DynamicType * CreateDeferredFunctionTypeNoProfileThunk_Internal(JavascriptMethod entrypoint, bool isShared);
        DynamicType * CreateFunctionType(JavascriptMethod entrypoint, RecyclableObject* prototype = nullptr);
        DynamicType * CreateFunctionWithConfigurableLengthType(FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthType(FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthAndNameType(FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthAndPrototypeType(FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithConfigurableLengthType(DynamicObject * prototype, FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthType(DynamicObject * prototype, FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthAndNameType(DynamicObject * prototype, FunctionInfo * functionInfo);
        DynamicType * CreateFunctionWithLengthAndPrototypeType(DynamicObject * prototype, FunctionInfo * functionInfo);
        ScriptFunction * CreateScriptFunction(FunctionProxy* proxy);
        AsmJsScriptFunction * CreateAsmJsScriptFunction(FunctionProxy* proxy);
#ifdef ENABLE_WASM
        WasmScriptFunction * CreateWasmScriptFunction(FunctionProxy* proxy);
#endif
        ScriptFunctionWithInlineCache * CreateScriptFunctionWithInlineCache(FunctionProxy* proxy);
        GeneratorVirtualScriptFunction * CreateGeneratorVirtualScriptFunction(FunctionProxy* proxy);

        DynamicType* CreateGeneratorType(RecyclableObject* prototype);
        DynamicType* CreateAsyncGeneratorType(RecyclableObject* prototype);
        DynamicType* CreateAsyncFromSyncIteratorType();

#if 0
        JavascriptNumber* CreateNumber(double value);
#endif
        JavascriptNumber* CreateNumber(double value, RecyclerJavascriptNumberAllocator * numberAllocator);
        JavascriptGeneratorFunction* CreateGeneratorFunction(JavascriptMethod entryPoint, GeneratorVirtualScriptFunction* scriptFunction);
        JavascriptGeneratorFunction* CreateGeneratorFunction(JavascriptMethod entryPoint, bool isAnonymousFunction);
        JavascriptAsyncGeneratorFunction* CreateAsyncGeneratorFunction(JavascriptMethod entryPoint, GeneratorVirtualScriptFunction* scriptFunction);
        AsyncGeneratorCallbackFunction* CreateAsyncGeneratorCallbackFunction(JavascriptMethod entryPoint, JavascriptAsyncGenerator* generator);
        RuntimeFunction* CreateAsyncModuleCallbackFunction(JavascriptMethod entryPoint, SourceTextModuleRecord* module);
        JavascriptAsyncFunction* CreateAsyncFunction(JavascriptMethod entryPoint, GeneratorVirtualScriptFunction* scriptFunction);
        JavascriptAsyncFunction* CreateAsyncFunction(JavascriptMethod entryPoint, bool isAnonymousFunction);
        JavascriptAsyncSpawnStepFunction* CreateAsyncSpawnStepFunction(JavascriptMethod entryPoint, JavascriptGenerator* generator, Var argument, Var resolve = nullptr, Var reject = nullptr, bool isReject = false);
        JavascriptExternalFunction* CreateExternalFunction(ExternalMethod entryPointer, PropertyId nameId, Var signature, unsigned long flags, bool isLengthAvailable = false);
        JavascriptExternalFunction* CreateExternalFunction(ExternalMethod entryPointer, Var nameId, Var signature, unsigned long flags, bool isLengthAvailable = false);
        JavascriptExternalFunction* CreateStdCallExternalFunction(StdCallJavascriptMethod entryPointer, Var name, void *callbackState);
        JavascriptPromiseCapabilitiesExecutorFunction* CreatePromiseCapabilitiesExecutorFunction(JavascriptMethod entryPoint, JavascriptPromiseCapability* capability);
        JavascriptPromiseResolveOrRejectFunction* CreatePromiseResolveOrRejectFunction(JavascriptMethod entryPoint, JavascriptPromise* promise, bool isReject, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyResolvedRecord);
        JavascriptPromiseReactionTaskFunction* CreatePromiseReactionTaskFunction(JavascriptMethod entryPoint, JavascriptPromiseReaction* reaction, Var argument);
        JavascriptPromiseResolveThenableTaskFunction* CreatePromiseResolveThenableTaskFunction(JavascriptMethod entryPoint, JavascriptPromise* promise, RecyclableObject* thenable, RecyclableObject* thenFunction);
        JavascriptPromiseAllResolveElementFunction* CreatePromiseAllResolveElementFunction(JavascriptMethod entryPoint, uint32_t index, JavascriptArray* values, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElements);
        JavascriptPromiseAnyRejectElementFunction* CreatePromiseAnyRejectElementFunction(JavascriptMethod entryPoint, uint32_t index, JavascriptArray* errors, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElements, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyCalledWrapper);
        JavascriptPromiseAllSettledResolveOrRejectElementFunction* CreatePromiseAllSettledResolveOrRejectElementFunction(JavascriptMethod entryPoint, uint32_t index, JavascriptArray* values, JavascriptPromiseCapability* capabilities, JavascriptPromiseAllResolveElementFunctionRemainingElementsWrapper* remainingElements, JavascriptPromiseResolveOrRejectFunctionAlreadyResolvedWrapper* alreadyCalledWrapper, bool isRejecting);
        JavascriptPromiseThenFinallyFunction* CreatePromiseThenFinallyFunction(JavascriptMethod entryPoint, RecyclableObject* OnFinally, RecyclableObject* Constructor, bool shouldThrow);
        JavascriptPromiseThunkFinallyFunction* CreatePromiseThunkFinallyFunction(JavascriptMethod entryPoint, Var value, bool shouldThrow);
        JavascriptExternalFunction* CreateWrappedExternalFunction(JavascriptExternalFunction* wrappedFunction);

        DynamicObject* CreateGeneratorConstructorPrototypeObject();
        DynamicObject* CreateAsyncGeneratorConstructorPrototypeObject();
        DynamicObject* CreateConstructorPrototypeObject(JavascriptFunction * constructor);
        DynamicObject* CreateClassPrototypeObject(RecyclableObject * protoParent);
        DynamicObject* CreateObject(const bool allowObjectHeaderInlining = false, const PropertyIndex requestedInlineSlotCapacity = 0);
        DynamicObject* CreateObject(DynamicTypeHandler * typeHandler);
        DynamicObject* CreateActivationObject();
        DynamicObject* CreatePseudoActivationObject();
        DynamicObject* CreateBlockActivationObject();
        DynamicObject* CreateConsoleScopeActivationObject();
        DynamicType* CreateObjectType(RecyclableObject* prototype, Js::TypeId typeId, uint16 requestedInlineSlotCapacity);
        DynamicType* CreateObjectTypeNoCache(RecyclableObject* prototype, Js::TypeId typeId);
        DynamicType* CreateObjectType(RecyclableObject* prototype, uint16 requestedInlineSlotCapacity);
        DynamicObject* CreateObject(RecyclableObject* prototype, uint16 requestedInlineSlotCapacity = 0);

        typedef JavascriptString* LibStringType; // used by diagnostics template
        template< size_t N > JavascriptString* CreateStringFromCppLiteral(const char16_t (&value)[N]) const;
        template<> JavascriptString* CreateStringFromCppLiteral(const char16_t (&value)[1]) const; // Specialization for empty string
        template<> JavascriptString* CreateStringFromCppLiteral(const char16_t (&value)[2]) const; // Specialization for single-char strings
        PropertyString* CreatePropertyString(const Js::PropertyRecord* propertyRecord);

        JavascriptBooleanObject* CreateBooleanObject(BOOL value);
        JavascriptBooleanObject* CreateBooleanObject();
        JavascriptNumberObject* CreateNumberObjectWithCheck(double value);
        JavascriptNumberObject* CreateNumberObject(Var number);
        JavascriptStringObject* CreateStringObject(JavascriptString* value);
        JavascriptStringObject* CreateStringObject(const char16_t* value, charcount_t length);
        JavascriptSymbolObject* CreateSymbolObject(JavascriptSymbol* value);
        JavascriptArrayIterator* CreateArrayIterator(Var iterable, JavascriptArrayIteratorKind kind);
        JavascriptMapIterator* CreateMapIterator(JavascriptMap* map, JavascriptMapIteratorKind kind);
        JavascriptSetIterator* CreateSetIterator(JavascriptSet* set, JavascriptSetIteratorKind kind);
        JavascriptStringIterator* CreateStringIterator(JavascriptString* string);
        JavascriptListIterator* CreateListIterator(ListForListIterator* list);

        JavascriptRegExp* CreateRegExp(UnifiedRegex::RegexPattern* pattern);

        DynamicObject* CreateIteratorResultObject(Var value, Var done);
        DynamicObject* CreateIteratorResultObject(Var value, bool done = false);
        DynamicObject* CreateIteratorResultObjectDone();

        RecyclableObject* CreateThrowErrorObject(JavascriptError* error);

        JavascriptFunction* EnsurePromiseResolveFunction();
        JavascriptFunction* EnsurePromiseThenFunction();
        JavascriptFunction* EnsureGeneratorReturnFunction();
        JavascriptFunction* EnsureGeneratorNextFunction();
        JavascriptFunction* EnsureGeneratorThrowFunction();
        JavascriptFunction* EnsureAsyncGeneratorNextFunction();
        JavascriptFunction* EnsureAsyncGeneratorReturnFunction();
        JavascriptFunction* EnsureAsyncGeneratorThrowFunction();
        JavascriptFunction* EnsureAsyncFromSyncIteratorNextFunction();
        JavascriptFunction* EnsureAsyncFromSyncIteratorThrowFunction();
        JavascriptFunction* EnsureAsyncFromSyncIteratorReturnFunction();
        RuntimeFunction*    EnsureAsyncFromSyncIteratorValueUnwrapTrueFunction();
        RuntimeFunction*    EnsureAsyncFromSyncIteratorValueUnwrapFalseFunction();
        JavascriptFunction* EnsureArrayPrototypeForEachFunction();
        JavascriptFunction* EnsureArrayPrototypeKeysFunction();
        JavascriptFunction* EnsureArrayPrototypeEntriesFunction();
        JavascriptFunction* EnsureArrayPrototypeValuesFunction();
        JavascriptFunction* EnsureJSONStringifyFunction();
        JavascriptFunction* EnsureObjectFreezeFunction();

        void SetCrossSiteForLockedFunctionType(JavascriptFunction * function);
        void SetCrossSiteForLockedNonBuiltInFunctionType(JavascriptFunction * function);

        bool IsPRNGSeeded() { return isPRNGSeeded; }
        unsigned long GetRandSeed0() { return randSeed0; }
        unsigned long GetRandSeed1() { return randSeed1; }
        void SetIsPRNGSeeded(bool val);
        void SetRandSeed0(unsigned long rs) { randSeed0 = rs;}
        void SetRandSeed1(unsigned long rs) { randSeed1 = rs; }

        void SetProfileMode(bool fSet);
        void SetDispatchProfile(bool fSet, JavascriptMethod dispatchInvoke);
        int32_t ProfilerRegisterBuiltIns();

#if ENABLE_COPYONACCESS_ARRAY
        static bool IsCopyOnAccessArrayCallSite(JavascriptLibrary *lib, ArrayCallSiteInfo *arrayInfo, uint32_t length);
        static bool IsCachedCopyOnAccessArrayCallSite(const JavascriptLibrary *lib, ArrayCallSiteInfo *arrayInfo);
        template <typename T>
        static void CheckAndConvertCopyOnAccessNativeIntArray(const T instance);
#endif    

        static void CheckAndInvalidateIsConcatSpreadableCache(PropertyId propertyId, ScriptContext *scriptContext);

#if DBG_DUMP
        static const char16_t* GetStringTemplateCallsiteObjectKey(Var callsite);
#endif

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type* GetBuiltinFunctions();
        long* GetVTableAddresses();
        static BuiltinFunction GetBuiltinFunctionForPropId(PropertyId id);
        static BuiltinFunction GetBuiltInForFuncInfo(LocalFunctionId localFuncId);
#if DBG
        static void CheckRegisteredBuiltIns(typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type* builtInFuncs, ScriptContext *scriptContext);
#endif
        static BOOL CanFloatPreferenceFunc(BuiltinFunction index);
        static BOOL IsFltFunc(BuiltinFunction index);
        static bool IsFloatFunctionCallsite(BuiltinFunction index, size_t argc);
        static bool IsFltBuiltInConst(PropertyId id);
        static size_t GetArgCForBuiltIn(BuiltinFunction index)
        {
            Assert(index < _countof(JavascriptLibrary::LibraryFunctionArgC));
            return JavascriptLibrary::LibraryFunctionArgC[index];
        }
        static BuiltInFlags GetFlagsForBuiltIn(BuiltinFunction index)
        {
            Assert(index < _countof(JavascriptLibrary::LibraryFunctionFlags));
            return (BuiltInFlags)JavascriptLibrary::LibraryFunctionFlags[index];
        }
        static BuiltinFunction GetBuiltInInlineCandidateId(Js::OpCode opCode);
        static BuiltInArgSpecializationType GetBuiltInArgType(BuiltInFlags flags, BuiltInArgShift argGroup);
        static bool IsTypeSpecRequired(BuiltInFlags flags)
        {
            return GetBuiltInArgType(flags, BuiltInArgShift::BIAS_Src1) || GetBuiltInArgType(flags, BuiltInArgShift::BIAS_Src2) || GetBuiltInArgType(flags, BuiltInArgShift::BIAS_Dst);
        }
        static char16_t const * GetNameForBuiltIn(BuiltinFunction index)
        {
            Assert(index < _countof(JavascriptLibrary::LibraryFunctionName));
            return JavascriptLibrary::LibraryFunctionName[index];
        }

        PropertyStringCacheMap* EnsurePropertyStringMap();
        SymbolCacheMap* EnsureSymbolMap();

        template <typename TProperty> WeakPropertyIdMap<TProperty>* GetPropertyMap();
        template <> PropertyStringCacheMap* GetPropertyMap<PropertyString>() { return this->propertyStringMap; }
        template <> SymbolCacheMap* GetPropertyMap<JavascriptSymbol>() { return this->symbolMap; }

        typename WriteBarrierFieldTypeTraits<OnlyWritablePropertyProtoChainCache*>::Type GetTypesWithOnlyWritablePropertyProtoChainCache() { return &this->typesWithOnlyWritablePropertyProtoChain; }
        typename WriteBarrierFieldTypeTraits<NoSpecialPropertyProtoChainCache*>::Type GetTypesWithNoSpecialPropertyProtoChainCache() { return &this->typesWithNoSpecialPropertyProtoChain; }

        static bool IsDefaultArrayValuesFunction(RecyclableObject * function, ScriptContext *scriptContext);
        static bool ArrayIteratorPrototypeHasUserDefinedNext(ScriptContext *scriptContext);

        CharStringCache& GetCharStringCache() { return charStringCache;  }
        static JavascriptLibrary * FromCharStringCache(CharStringCache * cache)
        {
            return (JavascriptLibrary *)((uintptr_t)cache - offsetof(JavascriptLibrary, charStringCache));
        }

        EnumeratorCache* GetObjectAssignCache(Type* type);
        EnumeratorCache* GetCreateKeysCache(Type* type);
        EnumeratorCache* GetStringifyCache(Type* type);

        bool GetArrayObjectHasUserDefinedSpecies() const { return arrayObjectHasUserDefinedSpecies; }
        void SetArrayObjectHasUserDefinedSpecies(bool val) { arrayObjectHasUserDefinedSpecies = val; }

        FunctionBody* GetFakeGlobalFuncForUndefer()const { return this->fakeGlobalFuncForUndefer; }
        void SetFakeGlobalFuncForUndefer(FunctionBody* functionBody) { this->fakeGlobalFuncForUndefer = functionBody; }

        ModuleRecordList* EnsureModuleRecordList();
        ModuleRecordList* GetModuleRecordList() const { return this->moduleRecordList; }
        SourceTextModuleRecord* GetModuleRecord(uint moduleId);

    private:
        // Declare fretest/debug properties here since otherwise it can cause
        // a mismatch between fre mshtml and fretest jscript9 causing undefined behavior

        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type debugDisposableObjectType;
        typename WriteBarrierFieldTypeTraits<DynamicType *>::Type debugFuncExecutorInDisposeObjectType;

        void InitializePrototypes();
        void InitializeTypes();
        void InitializeGlobal(GlobalObject * globalObject);
        static void PrecalculateArrayAllocationBuckets();

#define STANDARD_INIT(name) \
        static bool Initialize##name##Constructor(DynamicObject* arrayConstructor, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode); \
        static bool Initialize##name##Prototype(DynamicObject* arrayPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        STANDARD_INIT(Array);
        STANDARD_INIT(SharedArrayBuffer);
        STANDARD_INIT(ArrayBuffer);
        STANDARD_INIT(DataView);
        STANDARD_INIT(Error);
        STANDARD_INIT(EvalError);
        STANDARD_INIT(RangeError);
        STANDARD_INIT(ReferenceError);
        STANDARD_INIT(SyntaxError);
        STANDARD_INIT(TypeError);
        STANDARD_INIT(URIError);
        STANDARD_INIT(AggregateError);
        STANDARD_INIT(RuntimeError);
        STANDARD_INIT(TypedArray);
        STANDARD_INIT(Int8Array);
        STANDARD_INIT(Uint8Array);
        STANDARD_INIT(Uint8ClampedArray);
        STANDARD_INIT(Int16Array);
        STANDARD_INIT(Uint16Array);
        STANDARD_INIT(Int32Array);
        STANDARD_INIT(Uint32Array);
        STANDARD_INIT(Float32Array);
        STANDARD_INIT(Float64Array);
        STANDARD_INIT(Boolean);
        STANDARD_INIT(Symbol);
        STANDARD_INIT(Date);
        STANDARD_INIT(Proxy);
        STANDARD_INIT(Function);
        STANDARD_INIT(Number);
        STANDARD_INIT(BigInt);
        STANDARD_INIT(Object);
        STANDARD_INIT(Regex);
        STANDARD_INIT(String);
        STANDARD_INIT(Map);
        STANDARD_INIT(Set);
        STANDARD_INIT(WeakMap);
        STANDARD_INIT(WeakSet);
        STANDARD_INIT(Promise);
        STANDARD_INIT(GeneratorFunction);
        STANDARD_INIT(AsyncFunction);
        STANDARD_INIT(AsyncGeneratorFunction);
        STANDARD_INIT(WebAssemblyCompileError);
        STANDARD_INIT(WebAssemblyRuntimeError);
        STANDARD_INIT(WebAssemblyLinkError);
        STANDARD_INIT(WebAssemblyMemory);
        STANDARD_INIT(WebAssemblyModule);
        STANDARD_INIT(WebAssemblyInstance);
        STANDARD_INIT(WebAssemblyTable);

#undef STANDARD_INIT

        static bool InitializeAtomicsObject(DynamicObject* atomicsObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        static bool InitializeInt64ArrayPrototype(DynamicObject* prototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeUint64ArrayPrototype(DynamicObject* prototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeBoolArrayPrototype(DynamicObject* prototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeCharArrayPrototype(DynamicObject* prototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        void InitializeComplexThings();
        void InitializeStaticValues();
        static bool InitializeMathObject(DynamicObject* mathObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
#ifdef ENABLE_WASM
        static bool InitializeWebAssemblyObject(DynamicObject* WasmObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
#endif

        static bool InitializeJSONObject(DynamicObject* JSONObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeEngineInterfaceObject(DynamicObject* engineInterface, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeReflectObject(DynamicObject* reflectObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeIntlObject(DynamicObject* IntlEngineObject, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeAsyncIteratorPrototype(DynamicObject* asyncIteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeIteratorPrototype(DynamicObject* iteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeArrayIteratorPrototype(DynamicObject* arrayIteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeMapIteratorPrototype(DynamicObject* mapIteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeSetIteratorPrototype(DynamicObject* setIteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeStringIteratorPrototype(DynamicObject* stringIteratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        static bool InitializeGeneratorPrototype(DynamicObject* generatorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeAsyncGeneratorPrototype(DynamicObject* asyncGeneratorPrototype, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeAsyncFromSyncIteratorPrototype(DynamicObject* asyncFromSyncIteratorProtototype, DeferredTypeHandlerBase* typeHandler, DeferredInitializeMode mode);

        static bool InitializeAsyncFunction(DynamicObject *function, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        RuntimeFunction* CreateBuiltinConstructor(FunctionInfo * functionInfo, DynamicTypeHandler * typeHandler, DynamicObject* prototype = nullptr);
        void DefaultCreateFunction(ParseableFunctionInfo * functionInfo, int length, DynamicObject * prototype, PropertyId nameId);
        RuntimeFunction* DefaultCreateFunction(FunctionInfo * functionInfo, int length, DynamicObject * prototype, DynamicType * functionType, PropertyId nameId);
        RuntimeFunction* DefaultCreateFunction(FunctionInfo * functionInfo, int length, DynamicObject * prototype, DynamicType * functionType, Var nameId);
        JavascriptFunction* AddFunction(DynamicObject* object, PropertyId propertyId, RuntimeFunction* function);
        void AddMember(DynamicObject* object, PropertyId propertyId, Var value);
        void AddMember(DynamicObject* object, PropertyId propertyId, Var value, PropertyAttributes attributes);
        JavascriptString* CreateEmptyString();

        template<uint cacheSlotCount> EnumeratorCache* GetEnumeratorCache(Type* type, typename WriteBarrierFieldTypeTraits<EnumeratorCache*>::Type* cacheSlots);

        static bool InitializeAsyncGeneratorFunction(DynamicObject* function, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        static bool InitializeGeneratorFunction(DynamicObject* function, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);

        static size_t const LibraryFunctionArgC[BuiltinFunction::Count + 1];
        static int const LibraryFunctionFlags[BuiltinFunction::Count + 1];   // returns enum BuiltInFlags.
        static char16_t const * const LibraryFunctionName[BuiltinFunction::Count + 1];

    public:
        template<bool addPrototype, bool addName, bool useLengthType, bool addLength>
        static bool InitializeFunction(DynamicObject* function, DeferredTypeHandlerBase * typeHandler, DeferredInitializeMode mode);
        virtual void Finalize(bool isShutdown) override;

#if DBG
        void DumpLibraryByteCode();
#endif
    private:
        typedef JsUtil::BaseHashSet<Js::PropertyRecord const *, Recycler, PowerOf2SizePolicy> ReferencedPropertyRecordHashSet;
        typename WriteBarrierFieldTypeTraits<ReferencedPropertyRecordHashSet*>::Type referencedPropertyRecords;

        ReferencedPropertyRecordHashSet * EnsureReferencedPropertyRecordList()
        {
            ReferencedPropertyRecordHashSet* pidList = this->referencedPropertyRecords;
            if (pidList == nullptr)
            {
                pidList = RecyclerNew(this->recycler, ReferencedPropertyRecordHashSet, this->recycler, 173);
                this->referencedPropertyRecords = pidList;
            }
            return pidList;
        }

        ReferencedPropertyRecordHashSet * GetReferencedPropertyRecordList() const
        {
            return this->referencedPropertyRecords;
        }

        int32_t ProfilerRegisterObject();
        int32_t ProfilerRegisterArray();
        int32_t ProfilerRegisterBoolean();
        int32_t ProfilerRegisterDate();
        int32_t ProfilerRegisterFunction();
        int32_t ProfilerRegisterMath();
        int32_t ProfilerRegisterNumber();
        int32_t ProfilerRegisterBigInt();
        int32_t ProfilerRegisterString();
        int32_t ProfilerRegisterRegExp();
        int32_t ProfilerRegisterJSON();
        int32_t ProfilerRegisterMap();
        int32_t ProfilerRegisterSet();
        int32_t ProfilerRegisterWeakMap();
        int32_t ProfilerRegisterWeakSet();
        int32_t ProfilerRegisterSymbol();
        int32_t ProfilerRegisterIterator();
        int32_t ProfilerRegisterArrayIterator();
        int32_t ProfilerRegisterMapIterator();
        int32_t ProfilerRegisterSetIterator();
        int32_t ProfilerRegisterStringIterator();
        int32_t ProfilerRegisterTypedArray();
        int32_t ProfilerRegisterPromise();
        int32_t ProfilerRegisterProxy();
        int32_t ProfilerRegisterReflect();
        int32_t ProfilerRegisterGenerator();
        int32_t ProfilerRegisterAtomics();

#ifdef IR_VIEWER
        int32_t ProfilerRegisterIRViewer();
#endif /* IR_VIEWER */
    };
}
