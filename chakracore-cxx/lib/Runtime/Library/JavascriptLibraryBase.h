//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

// JavascriptLibraryBase.h is used by static lib shared between Trident and Chakra. We need to keep
// the size consistent and try not to change its size. We need to have matching mshtml.dll
// if the size changed here.
#pragma once

class ChakraEngine;

namespace Js
{
    class EngineInterfaceObject;

    class JavascriptLibraryBase : public FinalizableObject
    {
        friend class JavascriptLibrary;
        friend class ScriptSite;

    public:
        JavascriptLibraryBase(GlobalObject* globalObject):
            globalObject(globalObject),
            chakraEngine(nullptr)
        {
        }
        Var GetPI() { return pi; }
        Var GetNaN() { return nan; }
        Var GetNegativeInfinite() { return negativeInfinite; }
        Var GetPositiveInfinite() { return positiveInfinite; }
        Var GetMaxValue() { return maxValue; }
        Var GetMinValue() { return minValue; }
        Var GetNegativeZero() { return negativeZero; }
        RecyclableObject* GetUndefined() { return undefinedValue; }
        RecyclableObject* GetNull() { return nullValue; }
        JavascriptBoolean* GetTrue() { return booleanTrue; }
        JavascriptBoolean* GetFalse() { return booleanFalse; }

        JavascriptSymbol* GetSymbolHasInstance() { return symbolHasInstance; }
        JavascriptSymbol* GetSymbolIsConcatSpreadable() { return symbolIsConcatSpreadable; }
        JavascriptSymbol* GetSymbolAsyncIterator() { return symbolAsyncIterator; }
        JavascriptSymbol* GetSymbolIterator() { return symbolIterator; }
        JavascriptSymbol* GetSymbolToPrimitive() { return symbolToPrimitive; }
        JavascriptSymbol* GetSymbolToStringTag() { return symbolToStringTag; }
        JavascriptSymbol* GetSymbolUnscopables() { return symbolUnscopables; }

        JavascriptFunction* GetObjectConstructor() { return objectConstructor; }
        JavascriptFunction* GetArrayConstructor() { return arrayConstructor; }
        JavascriptFunction* GetBooleanConstructor() { return booleanConstructor; }
        JavascriptFunction* GetDateConstructor() { return dateConstructor; }
        JavascriptFunction* GetFunctionConstructor() { return functionConstructor; }
        JavascriptFunction* GetNumberConstructor() { return numberConstructor; }
        JavascriptFunction* GetBigIntConstructor() { return bigIntConstructor; }
        JavascriptRegExpConstructor* GetRegExpConstructor() { return regexConstructor; }
        JavascriptFunction* GetStringConstructor() { return stringConstructor; }
        JavascriptFunction* GetArrayBufferConstructor() { return arrayBufferConstructor; }
        JavascriptFunction* GetPixelArrayConstructor() { return pixelArrayConstructor; }
        JavascriptFunction* GetTypedArrayConstructor() const { return typedArrayConstructor; }
        JavascriptFunction* GetInt8ArrayConstructor() { return Int8ArrayConstructor; }
        JavascriptFunction* GetUint8ArrayConstructor() { return Uint8ArrayConstructor; }
        JavascriptFunction* GetUint8ClampedArrayConstructor() { return Uint8ClampedArrayConstructor; }
        JavascriptFunction* GetInt16ArrayConstructor() { return Int16ArrayConstructor; }
        JavascriptFunction* GetUint16ArrayConstructor() { return Uint16ArrayConstructor; }
        JavascriptFunction* GetInt32ArrayConstructor() { return Int32ArrayConstructor; }
        JavascriptFunction* GetUint32ArrayConstructor() { return Uint32ArrayConstructor; }
        JavascriptFunction* GetFloat32ArrayConstructor() { return Float32ArrayConstructor; }
        JavascriptFunction* GetFloat64ArrayConstructor() { return Float64ArrayConstructor; }
        JavascriptFunction* GetMapConstructor() { return mapConstructor; }
        JavascriptFunction* GetSetConstructor() { return setConstructor; }
        JavascriptFunction* GetWeakMapConstructor() { return weakMapConstructor; }
        JavascriptFunction* GetWeakSetConstructor() { return weakSetConstructor; }
        JavascriptFunction* GetSymbolConstructor() { return symbolConstructor; }
        JavascriptFunction* GetProxyConstructor() const { return proxyConstructor; }
        JavascriptFunction* GetPromiseConstructor() const { return promiseConstructor; }
        JavascriptFunction* GetGeneratorFunctionConstructor() const { return generatorFunctionConstructor; }
        JavascriptFunction* GetAsyncFunctionConstructor() const { return asyncFunctionConstructor; }
        JavascriptFunction* GetAsyncGeneratorFunctionConstructor() const { return asyncGeneratorFunctionConstructor; }

        JavascriptFunction* GetErrorConstructor() const { return errorConstructor; }
        JavascriptFunction* GetEvalErrorConstructor() const { return evalErrorConstructor; }
        JavascriptFunction* GetRangeErrorConstructor() const { return rangeErrorConstructor; }
        JavascriptFunction* GetReferenceErrorConstructor() const { return referenceErrorConstructor; }
        JavascriptFunction* GetSyntaxErrorConstructor() const { return syntaxErrorConstructor; }
        JavascriptFunction* GetTypeErrorConstructor() const { return typeErrorConstructor; }
        JavascriptFunction* GetURIErrorConstructor() const { return uriErrorConstructor; }
        JavascriptFunction* GetAggregateErrorConstructor() const { return aggregateErrorConstructor;  }
        JavascriptFunction* GetPromiseResolve() const { return promiseResolveFunction; }
        JavascriptFunction* GetPromiseThen() const { return promiseThenFunction; }
        JavascriptFunction* GetJSONStringify() const { return jsonStringifyFunction; }
        JavascriptFunction* GetObjectFreeze() const { return objectFreezeFunction; }
        JavascriptFunction* GetDebugEval() const { return debugEval; }
        JavascriptFunction* GetStackTraceFunction() const { return getStackTrace; }
#ifdef EDIT_AND_CONTINUE
        JavascriptFunction* GetEditSource() const { return editSource; }
#endif

        JavascriptFunction* GetArrayPrototypeForEachFunction() const { return arrayPrototypeForEachFunction; }
        JavascriptFunction* GetArrayPrototypeKeysFunction() const { return arrayPrototypeKeysFunction; }
        JavascriptFunction* GetArrayPrototypeValuesFunction() const { return arrayPrototypeValuesFunction; }
        JavascriptFunction* GetArrayPrototypeEntriesFunction() const { return arrayPrototypeEntriesFunction; }

        DynamicObject* GetMathObject() { return mathObject; }
        DynamicObject* GetJSONObject() { return JSONObject; }
        DynamicObject* GetIntlObject() { return IntlObject; }
        EngineInterfaceObject* GetEngineInterfaceObject() { return engineInterfaceObject; }

        DynamicObject* GetArrayPrototype() { return arrayPrototype; }
        DynamicObject* GetBooleanPrototype() { return booleanPrototype; }
        DynamicObject* GetDatePrototype() { return datePrototype; }
        DynamicObject* GetFunctionPrototype() { return functionPrototype; }
        DynamicObject* GetNumberPrototype() { return numberPrototype; }
        DynamicObject* GetBigIntPrototype() { return bigintPrototype; }
        DynamicObject* GetSIMDBool8x16Prototype()  { return simdBool8x16Prototype;  }
        DynamicObject* GetSIMDBool16x8Prototype()  { return simdBool16x8Prototype;  }
        DynamicObject* GetSIMDBool32x4Prototype()  { return simdBool32x4Prototype;  }
        DynamicObject* GetSIMDInt8x16Prototype()   { return simdInt8x16Prototype;   }
        DynamicObject* GetSIMDInt16x8Prototype()   { return simdInt16x8Prototype;   }
        DynamicObject* GetSIMDInt32x4Prototype()   { return simdInt32x4Prototype;   }
        DynamicObject* GetSIMDUint8x16Prototype()  { return simdUint8x16Prototype;  }
        DynamicObject* GetSIMDUint16x8Prototype()  { return simdUint16x8Prototype;  }
        DynamicObject* GetSIMDUint32x4Prototype()  { return simdUint32x4Prototype;  }
        DynamicObject* GetSIMDFloat32x4Prototype() { return simdFloat32x4Prototype; }
        DynamicObject* GetSIMDFloat64x2Prototype() { return simdFloat64x2Prototype; }
        ObjectPrototypeObject* GetObjectPrototypeObject() { return objectPrototype; }
        DynamicObject* GetObjectPrototype();
        DynamicObject* GetRegExpPrototype() { return regexPrototype; }
        DynamicObject* GetStringPrototype() { return stringPrototype; }
        DynamicObject* GetMapPrototype() { return mapPrototype; }
        DynamicObject* GetSetPrototype() { return setPrototype; }
        DynamicObject* GetWeakMapPrototype() { return weakMapPrototype; }
        DynamicObject* GetWeakSetPrototype() { return weakSetPrototype; }
        DynamicObject* GetSymbolPrototype() { return symbolPrototype; }
        DynamicObject* GetAsyncIteratorPrototype() const { return asyncIteratorPrototype; }
        DynamicObject* GetArrayIteratorPrototype() const { return arrayIteratorPrototype; }
        DynamicObject* GetMapIteratorPrototype() const { return mapIteratorPrototype; }
        DynamicObject* GetSetIteratorPrototype() const { return setIteratorPrototype; }
        DynamicObject* GetStringIteratorPrototype() const { return stringIteratorPrototype; }
        DynamicObject* GetIteratorPrototype() const { return iteratorPrototype; }
        DynamicObject* GetPromisePrototype() const { return promisePrototype; }
        DynamicObject* GetGeneratorFunctionPrototype() const { return generatorFunctionPrototype; }
        DynamicObject* GetGeneratorPrototype() const { return generatorPrototype; }
        DynamicObject* GetAsyncFunctionPrototype() const { return asyncFunctionPrototype; }
        DynamicObject* GetAsyncGeneratorFunctionPrototype() const { return asyncGeneratorFunctionPrototype; }

        DynamicObject* GetErrorPrototype() const { return errorPrototype; }
        DynamicObject* GetEvalErrorPrototype() const { return evalErrorPrototype; }
        DynamicObject* GetRangeErrorPrototype() const { return rangeErrorPrototype; }
        DynamicObject* GetReferenceErrorPrototype() const { return referenceErrorPrototype; }
        DynamicObject* GetSyntaxErrorPrototype() const { return syntaxErrorPrototype; }
        DynamicObject* GetTypeErrorPrototype() const { return typeErrorPrototype; }
        DynamicObject* GetURIErrorPrototype() const { return uriErrorPrototype; }
        DynamicObject* GetAggregateErrorPrototype() const { return aggregateErrorPrototype;  }
        PropertyId GetPropertyIdSymbolIterator() { return PropertyIds::_symbolIterator; };
        PropertyId GetPropertyIdSymbolToStringTag() { return PropertyIds::_symbolToStringTag; };
        PropertyId GetPropertyIdSymbolUnscopables() { return PropertyIds::_symbolUnscopables; };

        bool IsChakraEngine() const { return chakraEngine != nullptr; }

    protected:
        typename WriteBarrierFieldTypeTraits<GlobalObject*>::Type globalObject;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type mapConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type setConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type weakMapConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type weakSetConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type arrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type typedArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Int8ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Uint8ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Uint8ClampedArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Int16ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Uint16ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Int32ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Uint32ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Float32ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type Float64ArrayConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type arrayBufferConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type dataViewConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type booleanConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type dateConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type functionConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type numberConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type bigIntConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type objectConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type symbolConstructor;
        typename WriteBarrierFieldTypeTraits<JavascriptRegExpConstructor*>::Type regexConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type stringConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type pixelArrayConstructor;

        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type errorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type evalErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type rangeErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type referenceErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type syntaxErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type typeErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type uriErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type aggregateErrorConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type proxyConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type promiseConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type generatorFunctionConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type asyncFunctionConstructor;
        typename WriteBarrierFieldTypeTraits<RuntimeFunction*>::Type asyncGeneratorFunctionConstructor;

        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type defaultAccessorFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type stackTraceAccessorFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type throwTypeErrorRestrictedPropertyAccessorFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugObjectNonUserGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugObjectNonUserSetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugObjectDebugModeGetterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type __proto__getterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type __proto__setterFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayIteratorPrototypeBuiltinNextFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type promiseResolveFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type promiseThenFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type jsonStringifyFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type objectFreezeFunction;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type mathObject;
        // SIMD_JS
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdObject;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type debugObject;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type JSONObject;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type IntlObject;
        typename WriteBarrierFieldTypeTraits<EngineInterfaceObject*>::Type engineInterfaceObject;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type reflectObject;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type arrayPrototype;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type typedArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Int8ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Uint8ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Uint8ClampedArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Int16ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Uint16ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Int32ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Uint32ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Float32ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Float64ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Int64ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type Uint64ArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type BoolArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type CharArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type arrayBufferPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type dataViewPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type pixelArrayPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type booleanPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type datePrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type functionPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type numberPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type bigintPrototype;
        typename WriteBarrierFieldTypeTraits<ObjectPrototypeObject*>::Type objectPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type regexPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type stringPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type mapPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type setPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type weakMapPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type weakSetPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type symbolPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type asyncIteratorPrototype;           // aka %AsyncIteratorPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type iteratorPrototype;           // aka %IteratorPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type arrayIteratorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type mapIteratorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type setIteratorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type stringIteratorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type promisePrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type generatorFunctionPrototype;      // aka %Generator%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type generatorPrototype;              // aka %GeneratorPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type asyncFunctionPrototype;          // aka %AsyncFunctionPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type asyncGeneratorPrototype;         // aka %AsyncGeneratorPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type asyncGeneratorFunctionPrototype; // aka %AsyncGeneratorFunctionPrototype%
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type asyncFromSyncIteratorProtototype;

        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type errorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type evalErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type rangeErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type referenceErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type syntaxErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type typeErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type uriErrorPrototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type aggregateErrorPrototype;

        //SIMD Prototypes
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdBool8x16Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdBool16x8Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdBool32x4Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdInt8x16Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdInt16x8Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdInt32x4Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdUint8x16Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdUint16x8Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdUint32x4Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdFloat32x4Prototype;
        typename WriteBarrierFieldTypeTraits<DynamicObject*>::Type simdFloat64x2Prototype;

        typename WriteBarrierFieldTypeTraits<JavascriptBoolean*>::Type booleanTrue;
        typename WriteBarrierFieldTypeTraits<JavascriptBoolean*>::Type booleanFalse;

        typename WriteBarrierFieldTypeTraits<Var>::Type nan;
        typename WriteBarrierFieldTypeTraits<Var>::Type negativeInfinite;
        typename WriteBarrierFieldTypeTraits<Var>::Type positiveInfinite;
        typename WriteBarrierFieldTypeTraits<Var>::Type pi;
        typename WriteBarrierFieldTypeTraits<Var>::Type minValue;
        typename WriteBarrierFieldTypeTraits<Var>::Type maxValue;
        typename WriteBarrierFieldTypeTraits<Var>::Type negativeZero;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type undefinedValue;
        typename WriteBarrierFieldTypeTraits<RecyclableObject*>::Type nullValue;

        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolAsyncIterator;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolHasInstance;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolIsConcatSpreadable;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolIterator;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolSpecies;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolToPrimitive;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolToStringTag;
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type symbolUnscopables;

    public:
        typedef void (CALLBACK *HostPromiseRejectionTrackerCallback)(Var promise, Var reason, bool handled, void *callbackState);

        void SetNativeHostPromiseRejectionTrackerCallback(HostPromiseRejectionTrackerCallback function, void *state)
        {
            this->nativeHostPromiseRejectionTracker = function;
            this->nativeHostPromiseRejectionTrackerState = state;
        }

    private:
        typename WriteBarrierFieldTypeTraits<HostPromiseRejectionTrackerCallback, _no_write_barrier_policy, _no_write_barrier_policy>::Type nativeHostPromiseRejectionTracker = nullptr;
        typename WriteBarrierFieldTypeTraits<void *>::Type nativeHostPromiseRejectionTrackerState = nullptr;

    public:
        typename WriteBarrierFieldTypeTraits<ScriptContext*>::Type scriptContext;
        typename WriteBarrierFieldTypeTraits<ChakraEngine*>::Type chakraEngine;

    private:
        virtual void Dispose(bool isShutdown) override;
        virtual void Finalize(bool isShutdown) override;
        virtual void Mark(Recycler *recycler) override { AssertMsg(false, "Mark called on object that isn't TrackableObject"); }

    protected:
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeForEachFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeKeysFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeValuesFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type arrayPrototypeEntriesFunction;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type debugEval;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type getStackTrace;
#ifdef EDIT_AND_CONTINUE
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type editSource;
#endif
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type mathMin;
        typename WriteBarrierFieldTypeTraits<JavascriptFunction*>::Type mathMax;

#ifdef ENABLE_JS_BUILTINS
    public:
        JavascriptFunction* GetMathMinFunction() const { return mathMin; }
        JavascriptFunction* GetMathMaxFunction() const { return mathMax; }
#endif
    };
}
