#pragma once

enum VTableValue {
//#if !_M_X64 TODO: OOP JIT, can we make this arch specific again?
    VtableJavascriptNumber,
//#endif
    VtableDynamicObject,
    VtableInvalid,
    VtablePropertyString,
    VtableLazyJSONString,
    VtableLiteralStringWithPropertyStringPtr,
    VtableJavascriptBoolean,
    VtableJavascriptArray,
    VtableInt8Array,
    VtableUint8Array,
    VtableUint8ClampedArray,
    VtableInt16Array,
    VtableUint16Array,
    VtableInt32Array,
    VtableUint32Array,
    VtableFloat32Array,
    VtableFloat64Array,
    VtableJavascriptPixelArray,
    VtableInt64Array,
    VtableUint64Array,
    VtableBoolArray,
    VtableCharArray,
    VtableInt8VirtualArray,
    VtableUint8VirtualArray,
    VtableUint8ClampedVirtualArray,
    VtableInt16VirtualArray,
    VtableUint16VirtualArray,
    VtableInt32VirtualArray,
    VtableUint32VirtualArray,
    VtableFloat32VirtualArray,
    VtableFloat64VirtualArray,
    VtableNativeIntArray,
#if ENABLE_COPYONACCESS_ARRAY
    VtableCopyOnAccessNativeIntArray,
#endif
    VtableNativeFloatArray,
    VtableJavascriptNativeIntArray,
    VtableJavascriptRegExp,
    VtableScriptFunction,

    VtableScriptFunctionWithHomeObj,
    VtableScriptFunctionWithComputedName,
    VtableScriptFunctionWithHomeObjAndComputedName,

    // Generator/Async functions
    VtableJavascriptGeneratorFunction,
    VtableVirtualJavascriptGeneratorFunctionWithHomeObj,
    VtableVirtualJavascriptGeneratorFunctionWithComputedName,
    VtableVirtualJavascriptGeneratorFunctionWithHomeObjAndComputedName,
    VtableJavascriptAsyncFunction,
    VtableJavascriptAsyncGeneratorFunction,

    VtableStackScriptFunction,
    VtableAsmJsScriptFunctionWithComputedName,
    VtableScriptFunctionWithInlineCacheAndHomeObj,
    VtableScriptFunctionWithInlineCacheAndComputedName,
    VtableScriptFunctionWithInlineCacheHomeObjAndComputedName,
    VtableConcatStringMulti,
    VtableCompoundString,
    // SIMD_JS
    VtableSimd128F4,
    VtableSimd128I4,
    VtableSimd128I8,
    VtableSimd128I16,
    VtableSimd128U4,
    VtableSimd128U8,
    VtableSimd128U16,
    Count
};