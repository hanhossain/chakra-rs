if(BuildJIT)
    add_definitions(-D_ENABLE_DYNAMIC_THUNKS=1)
endif()

if(CC_TARGETS_AMD64)
    set(Wasm_dep
    WasmLibrary.cpp
    WebAssembly.cpp
    WebAssemblyEnvironment.cpp
    WebAssemblyInstance.cpp
    WebAssemblyMemory.cpp
    WebAssemblyModule.cpp
    WebAssemblyTable.cpp
    WabtInterface.cpp
    )
endif()

set(CRLIB_SOURCE_CODES
    ArgumentsObject.cpp
    ArgumentsObjectEnumerator.cpp
    DelayFreeArrayBufferHelper.cpp
    ArrayBuffer.cpp
    AtomicsOperations.cpp
    AtomicsObject.cpp
    BoundFunction.cpp
    BufferStringBuilder.cpp
    CommonExternalApiImpl.cpp
    CompoundString.cpp
    ConcatString.cpp
    CustomExternalWrapperObject.cpp
    DataView.cpp
    DateImplementation.cpp
    ES5Array.cpp
    ES5ArrayIndexEnumerator.cpp
    EngineInterfaceObject.cpp
    ExternalLibraryBase.cpp
    ForInObjectEnumerator.cpp
    GlobalObject.cpp
    IntlEngineInterfaceExtensionObject.cpp
    JSON.cpp
    JSONParser.cpp
    JSONScanner.cpp
    JSONStack.cpp
    JSONStringBuilder.cpp
    JSONStringifier.cpp
    JavascriptArray.cpp
    JavascriptArrayIndexEnumerator.cpp
    JavascriptArrayIndexEnumeratorBase.cpp
    JavascriptArrayIndexSnapshotEnumerator.cpp
    JavascriptArrayIterator.cpp
    JavascriptAsyncFromSyncIterator.cpp
    JavascriptAsyncFunction.cpp
    JavascriptAsyncGenerator.cpp
    JavascriptAsyncGeneratorFunction.cpp
    JavascriptBoolean.cpp
    JavascriptBooleanObject.cpp
    JavascriptBuiltInFunctions.cpp
    JavascriptDate.cpp
    JavascriptError.cpp
    # JavascriptErrorDebug.cpp
    JavascriptExceptionMetadata.cpp
    JavascriptExternalFunction.cpp
    JavascriptFunction.cpp
    JavascriptGenerator.cpp
    JavascriptGeneratorFunction.cpp
    JavascriptIterator.cpp
    JavascriptLibrary.cpp
    JavascriptListIterator.cpp
    JavascriptMap.cpp
    JavascriptMapIterator.cpp
    JavascriptNumber.cpp
    JavascriptNumberObject.cpp
    JavascriptObject.cpp
    JavascriptPromise.cpp
    JavascriptProxy.cpp
    JavascriptReflect.cpp
    JavascriptRegExpConstructor.cpp
    JavascriptRegExpEnumerator.cpp
    JavascriptRegularExpression.cpp
    JavascriptRegularExpressionResult.cpp
    JavascriptSet.cpp
    JavascriptSetIterator.cpp
    JavascriptString.cpp
    JavascriptStringEnumerator.cpp
    JavascriptStringIterator.cpp
    JavascriptStringObject.cpp
    JavascriptBigInt.cpp
    JavascriptBigIntObject.cpp
    JavascriptSymbol.cpp
    JavascriptSymbolObject.cpp
    JavascriptTypedNumber.cpp
    JavascriptWeakMap.cpp
    JavascriptWeakSet.cpp
    JsBuiltInEngineInterfaceExtensionObject.cpp
    LazyJSONString.cpp
    LiteralString.cpp
    MathLibrary.cpp
    ModuleRoot.cpp
    ObjectPrototypeObject.cpp
    ProfileString.cpp
    PropertyRecordUsageCache.cpp
    PropertyString.cpp
    RegexHelper.cpp
    RootObjectBase.cpp
    RuntimeFunction.cpp
    RuntimeLibraryPch.cpp
    ScriptFunction.cpp
    SharedArrayBuffer.cpp
    SingleCharString.cpp
    SparseArraySegment.cpp
    StackScriptFunction.cpp
    StringCopyInfo.cpp
    SubString.cpp
    ThrowErrorObject.cpp
    TypedArray.cpp
    TypedArrayIndexEnumerator.cpp
    UriHelper.cpp
    VerifyMarkFalseReference.cpp
    ${Wasm_dep}
    )

if(CC_TARGETS_AMD64)
    set (CRLIB_SOURCE_CODES ${CRLIB_SOURCE_CODES}
        amd64/JavascriptFunctionA.S
    )
elseif(CC_TARGETS_ARM64)
    set (CRLIB_SOURCE_CODES ${CRLIB_SOURCE_CODES}
        arm64/arm64_CallFunction.S
        arm64/arm64_DeferredDeserializeThunk.S
        arm64/arm64_DeferredParsingThunk.S
    )
endif()

add_library (Chakra.Runtime.Library OBJECT ${CRLIB_SOURCE_CODES})

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Runtime.Library PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
)

if(CAN_BUILD_WABT)
   # TODO (hanhossain): cleanup includes
   target_include_directories(Chakra.Runtime.Library PUBLIC ${CHAKRACORE_SOURCE_DIR}/lib/wabt/chakra)
endif()

target_compile_definitions(Chakra.Runtime.Library PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

target_precompile_headers(Chakra.Runtime.Library PUBLIC RuntimeLibraryPch.h)
