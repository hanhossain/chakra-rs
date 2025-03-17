add_library (Chakra.Runtime.Types OBJECT
    ActivationObject.cpp
    ArrayObject.cpp
    DeferredTypeHandler.cpp
    DictionaryTypeHandler.cpp
    DynamicObject.cpp
    DynamicObjectPropertyEnumerator.cpp
    DynamicType.cpp
    ES5ArrayTypeHandler.cpp
    JavascriptEnumerator.cpp
    JavascriptStaticEnumerator.cpp
    MissingPropertyTypeHandler.cpp
    NullTypeHandler.cpp
    PathTypeHandler.cpp
    PropertyDescriptor.cpp
    RecyclableObject.cpp
    RuntimeTypePch.cpp
    ScriptFunctionType.cpp
    SimpleDictionaryTypeHandler.cpp
    SimpleDictionaryUnorderedTypeHandler.cpp
    SimpleTypeHandler.cpp
    SpreadArgument.cpp
    StaticType.cpp
    Type.cpp
    TypeHandler.cpp
    TypePath.cpp
    TypePropertyCache.cpp
    UnscopablesWrapperObject.cpp
    )

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Runtime.Types PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
)

target_precompile_headers(Chakra.Runtime.Types PUBLIC RuntimeTypePch.h)
