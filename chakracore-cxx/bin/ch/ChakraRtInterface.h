//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <rust/cxx.h>

#include "ChakraCommon.h"
#include "ChakraCore.h"

class ChakraRTInterface
{
private:
    static bool m_testHooksSetup;
    static bool m_testHooksInitialized;
    static bool m_usageStringPrinted;

private:
    static int32_t ParseConfigFlags(const rust::Vec<rust::String> &vargs);

public:
    static int32_t InitializeTestHooks(const rust::Vec<rust::String> &vargs);

    static JsErrorCode JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime) { return chakracore::jsrt::JsCreateRuntime(attributes, threadService, runtime); }
    static JsErrorCode JsCreateContext(JsRuntimeHandle runtime, JsContextRef *newContext) { return chakracore::jsrt::JsCreateContext(runtime, newContext); }
    static JsErrorCode JsSetObjectBeforeCollectCallback(JsRef ref, void* callbackState, JsObjectBeforeCollectCallback objectBeforeCollectCallback) { return chakracore::jsrt::JsSetObjectBeforeCollectCallback(ref, callbackState, objectBeforeCollectCallback); }
    static JsErrorCode JsSetRuntimeDomWrapperTracingCallbacks(JsRuntimeHandle runtime, JsRef wrapperTracingState, JsDOMWrapperTracingCallback wrapperTracingCallback, JsDOMWrapperTracingDoneCallback wrapperTracingDoneCallback, JsDOMWrapperTracingEnterFinalPauseCallback enterFinalPauseCallback) { return chakracore::jsrt::JsSetRuntimeDomWrapperTracingCallbacks(runtime, wrapperTracingState, wrapperTracingCallback, wrapperTracingDoneCallback, enterFinalPauseCallback); }
    static JsErrorCode JsSetRuntimeMemoryLimit(JsRuntimeHandle runtime, size_t memory) { return chakracore::jsrt::JsSetRuntimeMemoryLimit(runtime, memory); }
    static JsErrorCode JsSetCurrentContext(JsContextRef context) { return chakracore::jsrt::JsSetCurrentContext(context); }
    static JsErrorCode JsGetCurrentContext(JsContextRef* context) { return chakracore::jsrt::JsGetCurrentContext(context); }
    static JsErrorCode JsDisposeRuntime(JsRuntimeHandle runtime) { return chakracore::jsrt::JsDisposeRuntime(runtime); }
    static JsErrorCode JsCreateObject(JsValueRef *object) { return chakracore::jsrt::JsCreateObject(object); }
    static JsErrorCode JsCreateExternalObject(void *data, JsFinalizeCallback callback, JsValueRef *object) { return chakracore::jsrt::JsCreateExternalObject(data, callback, object); }
    static JsErrorCode JsGetArrayForEachFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayForEachFunction(result); }
    static JsErrorCode JsGetArrayKeysFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayKeysFunction(result); }
    static JsErrorCode JsGetArrayValuesFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayValuesFunction(result); }
    static JsErrorCode JsGetArrayEntriesFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayEntriesFunction(result); }
    static JsErrorCode JsGetPropertyIdSymbolIterator(JsPropertyIdRef * propertyId) { return chakracore::jsrt::JsGetPropertyIdSymbolIterator(propertyId); }
    static JsErrorCode JsGetErrorPrototype(JsValueRef * result) { return chakracore::jsrt::JsGetErrorPrototype(result); }
    static JsErrorCode JsGetIteratorPrototype(JsValueRef * result) { return chakracore::jsrt::JsGetIteratorPrototype(result); }
    static JsErrorCode JsCreateFunction(JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateFunction(nativeFunction, callbackState, function); }
    static JsErrorCode JsCreateEnhancedFunction(JsEnhancedNativeFunction nativeFunction, JsValueRef metadata, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateEnhancedFunction(nativeFunction, metadata, callbackState, function); }
    static JsErrorCode JsCreateNamedFunction(JsValueRef name, JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateNamedFunction(name, nativeFunction, callbackState, function); }
    static JsErrorCode JsSetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef value, bool useStrictRules) { return chakracore::jsrt::JsSetProperty(object, property, value, useStrictRules); }
    static JsErrorCode JsGetGlobalObject(JsValueRef *globalObject) { return chakracore::jsrt::JsGetGlobalObject(globalObject); }
    static JsErrorCode JsGetUndefinedValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetUndefinedValue(globalObject); }
    static JsErrorCode JsGetNullValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetNullValue(globalObject); }
    static JsErrorCode JsGetTrueValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetTrueValue(globalObject); }
    static JsErrorCode JsGetFalseValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetFalseValue(globalObject); }
    static JsErrorCode JsConvertValueToString(JsValueRef value, JsValueRef *stringValue) { return chakracore::jsrt::JsConvertValueToString(value, stringValue); }
    static JsErrorCode JsConvertValueToNumber(JsValueRef value, JsValueRef *numberValue) { return chakracore::jsrt::JsConvertValueToNumber(value, numberValue); }
    static JsErrorCode JsConvertValueToBoolean(JsValueRef value, JsValueRef *booleanValue) { return chakracore::jsrt::JsConvertValueToBoolean(value, booleanValue); }
    static JsErrorCode JsBooleanToBool(JsValueRef value, bool* boolValue) { return chakracore::jsrt::JsBooleanToBool(value, boolValue); }
    static JsErrorCode JsGetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef* value) { return chakracore::jsrt::JsGetProperty(object, property, value); }
    static JsErrorCode JsHasProperty(JsValueRef object, JsPropertyIdRef property, bool *hasProperty) { return chakracore::jsrt::JsHasProperty(object, property, hasProperty); }
    static JsErrorCode JsCallFunction(JsValueRef function, JsValueRef* arguments, unsigned short argumentCount, JsValueRef *result) { return chakracore::jsrt::JsCallFunction(function, arguments, argumentCount, result); }
    static JsErrorCode JsNumberToDouble(JsValueRef value, double* doubleValue) { return chakracore::jsrt::JsNumberToDouble(value, doubleValue); }
    static JsErrorCode JsNumberToInt(JsValueRef value, int* intValue) { return chakracore::jsrt::JsNumberToInt(value, intValue); }
    static JsErrorCode JsIntToNumber(int intValue, JsValueRef *value) { return chakracore::jsrt::JsIntToNumber(intValue, value); }
    static JsErrorCode JsDoubleToNumber(double doubleValue, JsValueRef* value) { return chakracore::jsrt::JsDoubleToNumber(doubleValue, value); }
    static JsErrorCode JsGetExternalData(JsValueRef object, void **data) { return chakracore::jsrt::JsGetExternalData(object, data); }
    static JsErrorCode JsSetExternalData(JsValueRef object, void *data)  { return chakracore::jsrt::JsSetExternalData(object, data); }
    static JsErrorCode JsCloneObject(JsValueRef object, JsValueRef *data) { return chakracore::jsrt::JsCloneObject(object, data); }
    static JsErrorCode JsCreateArray(unsigned int length, JsValueRef *result) { return chakracore::jsrt::JsCreateArray(length, result); }
    static JsErrorCode JsCreateArrayBuffer(unsigned int byteLength, JsValueRef *result) { return chakracore::jsrt::JsCreateArrayBuffer(byteLength, result); }
    static JsErrorCode JsCreateSharedArrayBufferWithSharedContent(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result) { return chakracore::jsrt::JsCreateSharedArrayBufferWithSharedContent(sharedContent, result); }
    static JsErrorCode JsGetSharedArrayBufferContent(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents) { return chakracore::jsrt::JsGetSharedArrayBufferContent(sharedArrayBuffer, sharedContents); }
    static JsErrorCode JsReleaseSharedArrayBufferContentHandle(JsSharedArrayBufferContentHandle sharedContent) { return chakracore::jsrt::JsReleaseSharedArrayBufferContentHandle(sharedContent); }
    static JsErrorCode JsGetArrayBufferStorage(JsValueRef instance, uint8_t **buffer, unsigned int *bufferLength) { return chakracore::jsrt::JsGetArrayBufferStorage(instance, buffer, bufferLength); }
    static JsErrorCode JsCreateError(JsValueRef message, JsValueRef *error) { return chakracore::jsrt::JsCreateError(message, error); }
    static JsErrorCode JsHasException(bool *hasException) { return chakracore::jsrt::JsHasException(hasException); }
    static JsErrorCode JsSetException(JsValueRef exception) { return chakracore::jsrt::JsSetException(exception); }
    static JsErrorCode JsGetAndClearException(JsValueRef *exception) { return chakracore::jsrt::JsGetAndClearException(exception); }
    static JsErrorCode JsGetAndClearExceptionWithMetadata(JsValueRef * metadata) { return chakracore::jsrt::JsGetAndClearExceptionWithMetadata(metadata); }
    static JsErrorCode JsGetRuntime(JsContextRef context, JsRuntimeHandle *runtime) { return chakracore::jsrt::JsGetRuntime(context, runtime); }
    static JsErrorCode JsRelease(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsRelease(ref, count); }
    static JsErrorCode JsAddRef(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsAddRef(ref, count); }
    static JsErrorCode JsGetValueType(JsValueRef value, JsValueType *type) { return chakracore::jsrt::JsGetValueType(value, type); }
    static JsErrorCode JsGetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef *value) { return chakracore::jsrt::JsGetIndexedProperty(object, index, value); }
    static JsErrorCode JsSetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef value) { return chakracore::jsrt::JsSetIndexedProperty(object, index, value); }
    static JsErrorCode JsSetPromiseContinuationCallback(JsPromiseContinuationCallback callback, void *callbackState) { return chakracore::jsrt::JsSetPromiseContinuationCallback(callback, callbackState); }
    static JsErrorCode JsGetContextOfObject(JsValueRef object, JsContextRef* context) { return chakracore::jsrt::JsGetContextOfObject(object, context); }
    static JsErrorCode JsParseModuleSource(JsModuleRecord requestModule, JsSourceContext sourceContext, byte* sourceText, unsigned int sourceLength, JsParseModuleSourceFlags sourceFlag, JsValueRef* exceptionValueRef) {
        return chakracore::jsrt::JsParseModuleSource(requestModule, sourceContext, sourceText, sourceLength, sourceFlag, exceptionValueRef);
    }
    static JsErrorCode JsModuleEvaluation(JsModuleRecord requestModule, JsValueRef* result) { return chakracore::jsrt::JsModuleEvaluation(requestModule, result); }
    static JsErrorCode JsGetModuleNamespace(JsModuleRecord requestModule, JsValueRef *moduleNamespace) { return chakracore::jsrt::JsGetModuleNamespace(requestModule, moduleNamespace); }
    static JsErrorCode JsInitializeModuleRecord(JsModuleRecord referencingModule, JsValueRef normalizedSpecifier, JsModuleRecord* moduleRecord) {
        return chakracore::jsrt::JsInitializeModuleRecord(referencingModule, normalizedSpecifier, moduleRecord);
    }
    static JsErrorCode JsSetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void* hostInfo) { return chakracore::jsrt::JsSetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }
    static JsErrorCode JsGetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void** hostInfo) { return chakracore::jsrt::JsGetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }

    static JsErrorCode JsRun(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsRun(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode JsParse(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsParse(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode JsSerialize(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerialize(script, buffer, parseAttributes); }
    static JsErrorCode JsRunSerialized(JsValueRef buffer, JsSerializedLoadScriptCallback scriptLoadCallback, JsSourceContext sourceContext, JsValueRef sourceUrl, JsValueRef * result) { return chakracore::jsrt::JsRunSerialized(buffer, scriptLoadCallback, sourceContext, sourceUrl, result); }
    static JsErrorCode JsGetStringLength(JsValueRef value, int *stringLength) { return chakracore::jsrt::JsGetStringLength(value, stringLength); }
    static JsErrorCode JsToString(JsValueRef value, rust::String &string) { return chakracore::jsrt::JsToString(value, string); }
    static JsErrorCode JsCreateString(const char *content, size_t length, JsValueRef *value) { return chakracore::jsrt::JsCreateString(content, length, value); }
    static JsErrorCode JsCreateString(const std::string &content, JsValueRef *value) { return chakracore::jsrt::JsCreateString(content, value); }
    static JsErrorCode JsCreateString(const rust::String &content, JsValueRef *value) { return chakracore::jsrt::JsCreateString(content, value); }
    static JsErrorCode JsCreatePropertyId(const std::string &name, JsPropertyIdRef *propertyId) { return chakracore::jsrt::JsCreatePropertyId(name, propertyId); }
    static JsErrorCode JsCreatePropertyId(const char *name, size_t length, JsPropertyIdRef *propertyId) { return chakracore::jsrt::JsCreatePropertyId(name, length, propertyId); }
    static JsErrorCode JsCreateExternalArrayBuffer(void *data, unsigned int byteLength, JsFinalizeCallback finalizeCallback, void *callbackState, JsValueRef *result)  { return chakracore::jsrt::JsCreateExternalArrayBuffer(data, byteLength, finalizeCallback, callbackState, result); }
    static JsErrorCode JsCreateExternalArrayBuffer(const rust::Str content, JsFinalizeCallback finalizeCallback, JsValueRef *result)  { return chakracore::jsrt::JsCreateExternalArrayBuffer(const_cast<char *>(content.data()), content.length(), finalizeCallback, const_cast<char *>(content.data()), result); }
    static JsErrorCode JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  { return chakracore::jsrt::JsGetProxyProperties(object, isProxy, target, handler); }

    static JsErrorCode JsSerializeParserState(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerializeParserState(script, buffer, parseAttributes); }
    static JsErrorCode JsRunScriptWithParserState(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef * result) { return chakracore::jsrt::JsRunScriptWithParserState(script, sourceContext, sourceUrl, parseAttributes, parserState, result); }

    static JsErrorCode JsVarSerializer(ReallocateBufferMemoryFunc reallocateBufferMemory, WriteHostObjectFunc writeHostObject, void * callbackState, JsVarSerializerHandle *serializerHandle) { return chakracore::jsrt::JsVarSerializer(reallocateBufferMemory, writeHostObject, callbackState, serializerHandle); }
    static JsErrorCode JsVarSerializerSetTransferableVars(JsVarSerializerHandle serializerHandle, JsValueRef *transferableVars, size_t transferableVarsCount) { return chakracore::jsrt::JsVarSerializerSetTransferableVars(serializerHandle, transferableVars, transferableVarsCount); }
    static JsErrorCode JsVarSerializerWriteValue(JsVarSerializerHandle serializerHandle, JsValueRef rootObject) { return chakracore::jsrt::JsVarSerializerWriteValue(serializerHandle, rootObject); }
    static JsErrorCode JsVarSerializerReleaseData(JsVarSerializerHandle serializerHandle, byte** data, size_t *dataLength) { return chakracore::jsrt::JsVarSerializerReleaseData(serializerHandle, data, dataLength); }
    static JsErrorCode JsVarSerializerFree(JsVarSerializerHandle serializerHandle) { return chakracore::jsrt::JsVarSerializerFree(serializerHandle); }

    static JsErrorCode JsVarDeserializer(void *data, size_t dataLength, ReadHostObjectFunc readHostObject, GetSharedArrayBufferFromIdFunc getSharedArrayBufferFromId, void* callbackState, JsVarDeserializerHandle *deserializerHandle) { return chakracore::jsrt::JsVarDeserializer(data, dataLength, readHostObject, getSharedArrayBufferFromId, callbackState, deserializerHandle); }
    static JsErrorCode JsVarDeserializerSetTransferableVars(JsVarDeserializerHandle deserializerHandle, JsValueRef* transferableVars, size_t transferableVarsCount) { return chakracore::jsrt::JsVarDeserializerSetTransferableVars(deserializerHandle, transferableVars, transferableVarsCount); }
    static JsErrorCode JsVarDeserializerReadValue(JsVarDeserializerHandle deserializerHandle, JsValueRef* value) { return chakracore::jsrt::JsVarDeserializerReadValue(deserializerHandle, value); }
    static JsErrorCode JsVarDeserializerFree(JsVarDeserializerHandle deserializerHandle) { return chakracore::jsrt::JsVarDeserializerFree(deserializerHandle); }

    static JsErrorCode JsDetachArrayBuffer(JsValueRef buffer) { return chakracore::jsrt::JsDetachArrayBuffer(buffer); }
    static JsErrorCode JsDiscardBackgroundParse_Experimental(uint32_t dwBgParseCookie, void* buffer, bool* callerOwnsBuffer) { return chakracore::jsrt::JsDiscardBackgroundParse_Experimental(dwBgParseCookie, buffer, callerOwnsBuffer); }
    static JsErrorCode JsExecuteBackgroundParse_Experimental(uint32_t dwBgParseCookie, JsValueRef script, JsSourceContext sourceContext, char16_t *url, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef *result) { return chakracore::jsrt::JsExecuteBackgroundParse_Experimental(dwBgParseCookie, script, sourceContext, url, parseAttributes, parserState, result); }

    static JsErrorCode JsGetArrayBufferFreeFunction(JsValueRef buffer, ArrayBufferFreeFn* freeFn) { return chakracore::jsrt::JsGetArrayBufferFreeFunction(buffer, freeFn); }
    static JsErrorCode JsExternalizeArrayBuffer(JsValueRef buffer) { return chakracore::jsrt::JsExternalizeArrayBuffer(buffer); }
};

class AutoRestoreContext
{
public:
    AutoRestoreContext(JsContextRef newContext)
    {
        JsErrorCode errorCode = ChakraRTInterface::JsGetCurrentContext(&oldContext);
        assert(errorCode == JsNoError);

        if (oldContext != newContext)
        {
            errorCode = ChakraRTInterface::JsSetCurrentContext(newContext);
            assert(errorCode == JsNoError);
            contextChanged = true;
        }
        else
        {
            contextChanged = false;
        }
    }

    ~AutoRestoreContext()
    {
        if (contextChanged && oldContext != JS_INVALID_REFERENCE)
        {
            ChakraRTInterface::JsSetCurrentContext(oldContext);
        }
    }
private:
    JsContextRef oldContext;
    bool contextChanged;
};
