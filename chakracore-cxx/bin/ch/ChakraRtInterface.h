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
public:
    typedef void(* HostPrintUsageFuncPtr)();

    struct ArgInfo
    {
        std::vector<std::u16string> vargs_;
        HostPrintUsageFuncPtr hostPrintUsage;
        std::string filename_;

        ArgInfo() :
            hostPrintUsage(nullptr)
        {
        }

        ArgInfo(const std::vector<std::u16string> &vargs, HostPrintUsageFuncPtr hostPrintUsage, std::string filename) :
            vargs_(vargs),
            hostPrintUsage(hostPrintUsage),
            filename_(std::move(filename))
        {
        }
    };

#define CHECKED_CALL_RETURN(func, retVal, ...) (m_testHooksSetup && m_testHooks.pf##func? m_testHooks.pf##func(__VA_ARGS__) : retVal)
#define CHECKED_CALL(func, ...) (m_testHooksSetup && m_testHooks.pf##func? m_testHooks.pf##func(__VA_ARGS__) : E_NOTIMPL)

private:
    static bool m_testHooksSetup;
    static bool m_testHooksInitialized;
    static bool m_usageStringPrinted;
    static ArgInfo* m_argInfo;
    static TestHooks m_testHooks;

private:
    static int32_t ParseConfigFlags();

public:
    static int32_t InitializeTestHooks(TestHooks& testHooks);

    static bool LoadChakraDll(ArgInfo* argInfo);

    static int32_t SetAssertToConsoleFlag(bool flag) { return CHECKED_CALL(SetAssertToConsoleFlag, flag); }
    static int32_t SetConfigFlags(const std::vector<std::u16string> &vargs, ICustomConfigFlags* customConfigFlags) { return CHECKED_CALL(SetConfigFlags, vargs, customConfigFlags); }
    static int32_t SetConfigFile(char16_t * strConfigFileName) { return CHECKED_CALL(SetConfigFile, strConfigFileName); }
    static int32_t GetFileNameFlag(BSTR * filename) { return CHECKED_CALL(GetFilenameFlag, filename); }
    static int32_t PrintConfigFlagsUsageString() { m_usageStringPrinted = true;  return CHECKED_CALL(PrintConfigFlagsUsageString); }

#ifdef CHECK_MEMORY_LEAK
    static bool IsEnabledCheckMemoryFlag() { return CHECKED_CALL_RETURN(IsEnabledCheckMemoryLeakFlag, FALSE); }
    static int32_t SetCheckMemoryLeakFlag(bool flag) { return CHECKED_CALL(SetCheckMemoryLeakFlag, flag); }
    static int32_t SetEnableCheckMemoryLeakOutput(bool flag) { return CHECKED_CALL(SetEnableCheckMemoryLeakOutput, flag); }
#endif
#ifdef DEBUG
    static int32_t SetCheckOpHelpersFlag(bool flag) { return CHECKED_CALL(SetCheckOpHelpersFlag, flag); }
#endif
    static int32_t SetOOPCFGRegistrationFlag(bool flag) { return CHECKED_CALL(SetOOPCFGRegistrationFlag, flag); }

    static JsErrorCode WINAPI JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime) { return chakracore::jsrt::JsCreateRuntime(attributes, threadService, runtime); }
    static JsErrorCode WINAPI JsCreateContext(JsRuntimeHandle runtime, JsContextRef *newContext) { return chakracore::jsrt::JsCreateContext(runtime, newContext); }
    static JsErrorCode WINAPI JsSetObjectBeforeCollectCallback(JsRef ref, void* callbackState, JsObjectBeforeCollectCallback objectBeforeCollectCallback) { return chakracore::jsrt::JsSetObjectBeforeCollectCallback(ref, callbackState, objectBeforeCollectCallback); }
    static JsErrorCode WINAPI JsSetRuntimeDomWrapperTracingCallbacks(JsRuntimeHandle runtime, JsRef wrapperTracingState, JsDOMWrapperTracingCallback wrapperTracingCallback, JsDOMWrapperTracingDoneCallback wrapperTracingDoneCallback, JsDOMWrapperTracingEnterFinalPauseCallback enterFinalPauseCallback) { return chakracore::jsrt::JsSetRuntimeDomWrapperTracingCallbacks(runtime, wrapperTracingState, wrapperTracingCallback, wrapperTracingDoneCallback, enterFinalPauseCallback); }
    static JsErrorCode WINAPI JsSetRuntimeMemoryLimit(JsRuntimeHandle runtime, size_t memory) { return chakracore::jsrt::JsSetRuntimeMemoryLimit(runtime, memory); }
    static JsErrorCode WINAPI JsSetCurrentContext(JsContextRef context) { return chakracore::jsrt::JsSetCurrentContext(context); }
    static JsErrorCode WINAPI JsGetCurrentContext(JsContextRef* context) { return chakracore::jsrt::JsGetCurrentContext(context); }
    static JsErrorCode WINAPI JsDisposeRuntime(JsRuntimeHandle runtime) { return chakracore::jsrt::JsDisposeRuntime(runtime); }
    static JsErrorCode WINAPI JsCreateObject(JsValueRef *object) { return chakracore::jsrt::JsCreateObject(object); }
    static JsErrorCode WINAPI JsCreateExternalObject(void *data, JsFinalizeCallback callback, JsValueRef *object) { return chakracore::jsrt::JsCreateExternalObject(data, callback, object); }
    static JsErrorCode WINAPI JsGetArrayForEachFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayForEachFunction(result); }
    static JsErrorCode WINAPI JsGetArrayKeysFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayKeysFunction(result); }
    static JsErrorCode WINAPI JsGetArrayValuesFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayValuesFunction(result); }
    static JsErrorCode WINAPI JsGetArrayEntriesFunction(JsValueRef * result) { return chakracore::jsrt::JsGetArrayEntriesFunction(result); }
    static JsErrorCode WINAPI JsGetPropertyIdSymbolIterator(JsPropertyIdRef * propertyId) { return chakracore::jsrt::JsGetPropertyIdSymbolIterator(propertyId); }
    static JsErrorCode WINAPI JsGetErrorPrototype(JsValueRef * result) { return chakracore::jsrt::JsGetErrorPrototype(result); }
    static JsErrorCode WINAPI JsGetIteratorPrototype(JsValueRef * result) { return chakracore::jsrt::JsGetIteratorPrototype(result); }
    static JsErrorCode WINAPI JsCreateFunction(JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateFunction(nativeFunction, callbackState, function); }
    static JsErrorCode WINAPI JsCreateEnhancedFunction(JsEnhancedNativeFunction nativeFunction, JsValueRef metadata, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateEnhancedFunction(nativeFunction, metadata, callbackState, function); }
    static JsErrorCode WINAPI JsCreateNamedFunction(JsValueRef name, JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) { return chakracore::jsrt::JsCreateNamedFunction(name, nativeFunction, callbackState, function); }
    static JsErrorCode WINAPI JsSetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef value, bool useStrictRules) { return chakracore::jsrt::JsSetProperty(object, property, value, useStrictRules); }
    static JsErrorCode WINAPI JsGetGlobalObject(JsValueRef *globalObject) { return chakracore::jsrt::JsGetGlobalObject(globalObject); }
    static JsErrorCode WINAPI JsGetUndefinedValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetUndefinedValue(globalObject); }
    static JsErrorCode WINAPI JsGetNullValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetNullValue(globalObject); }
    static JsErrorCode WINAPI JsGetTrueValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetTrueValue(globalObject); }
    static JsErrorCode WINAPI JsGetFalseValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetFalseValue(globalObject); }
    static JsErrorCode WINAPI JsConvertValueToString(JsValueRef value, JsValueRef *stringValue) { return chakracore::jsrt::JsConvertValueToString(value, stringValue); }
    static JsErrorCode WINAPI JsConvertValueToNumber(JsValueRef value, JsValueRef *numberValue) { return chakracore::jsrt::JsConvertValueToNumber(value, numberValue); }
    static JsErrorCode WINAPI JsConvertValueToBoolean(JsValueRef value, JsValueRef *booleanValue) { return chakracore::jsrt::JsConvertValueToBoolean(value, booleanValue); }
    static JsErrorCode WINAPI JsBooleanToBool(JsValueRef value, bool* boolValue) { return chakracore::jsrt::JsBooleanToBool(value, boolValue); }
    static JsErrorCode WINAPI JsGetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef* value) { return chakracore::jsrt::JsGetProperty(object, property, value); }
    static JsErrorCode WINAPI JsHasProperty(JsValueRef object, JsPropertyIdRef property, bool *hasProperty) { return chakracore::jsrt::JsHasProperty(object, property, hasProperty); }
    static JsErrorCode WINAPI JsCallFunction(JsValueRef function, JsValueRef* arguments, unsigned short argumentCount, JsValueRef *result) { return chakracore::jsrt::JsCallFunction(function, arguments, argumentCount, result); }
    static JsErrorCode WINAPI JsNumberToDouble(JsValueRef value, double* doubleValue) { return chakracore::jsrt::JsNumberToDouble(value, doubleValue); }
    static JsErrorCode WINAPI JsNumberToInt(JsValueRef value, int* intValue) { return chakracore::jsrt::JsNumberToInt(value, intValue); }
    static JsErrorCode WINAPI JsIntToNumber(int intValue, JsValueRef *value) { return chakracore::jsrt::JsIntToNumber(intValue, value); }
    static JsErrorCode WINAPI JsDoubleToNumber(double doubleValue, JsValueRef* value) { return chakracore::jsrt::JsDoubleToNumber(doubleValue, value); }
    static JsErrorCode WINAPI JsGetExternalData(JsValueRef object, void **data) { return chakracore::jsrt::JsGetExternalData(object, data); }
    static JsErrorCode WINAPI JsSetExternalData(JsValueRef object, void *data)  { return chakracore::jsrt::JsSetExternalData(object, data); }
    static JsErrorCode WINAPI JsCloneObject(JsValueRef object, JsValueRef *data) { return chakracore::jsrt::JsCloneObject(object, data); }
    static JsErrorCode WINAPI JsCreateArray(unsigned int length, JsValueRef *result) { return chakracore::jsrt::JsCreateArray(length, result); }
    static JsErrorCode WINAPI JsCreateArrayBuffer(unsigned int byteLength, JsValueRef *result) { return chakracore::jsrt::JsCreateArrayBuffer(byteLength, result); }
    static JsErrorCode WINAPI JsCreateSharedArrayBufferWithSharedContent(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result) { return chakracore::jsrt::JsCreateSharedArrayBufferWithSharedContent(sharedContent, result); }
    static JsErrorCode WINAPI JsGetSharedArrayBufferContent(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents) { return chakracore::jsrt::JsGetSharedArrayBufferContent(sharedArrayBuffer, sharedContents); }
    static JsErrorCode WINAPI JsReleaseSharedArrayBufferContentHandle(JsSharedArrayBufferContentHandle sharedContent) { return chakracore::jsrt::JsReleaseSharedArrayBufferContentHandle(sharedContent); }
    static JsErrorCode WINAPI JsGetArrayBufferStorage(JsValueRef instance, uint8_t **buffer, unsigned int *bufferLength) { return chakracore::jsrt::JsGetArrayBufferStorage(instance, buffer, bufferLength); }
    static JsErrorCode WINAPI JsCreateError(JsValueRef message, JsValueRef *error) { return chakracore::jsrt::JsCreateError(message, error); }
    static JsErrorCode WINAPI JsHasException(bool *hasException) { return chakracore::jsrt::JsHasException(hasException); }
    static JsErrorCode WINAPI JsSetException(JsValueRef exception) { return chakracore::jsrt::JsSetException(exception); }
    static JsErrorCode WINAPI JsGetAndClearException(JsValueRef *exception) { return chakracore::jsrt::JsGetAndClearException(exception); }
    static JsErrorCode WINAPI JsGetAndClearExceptionWithMetadata(JsValueRef * metadata) { return chakracore::jsrt::JsGetAndClearExceptionWithMetadata(metadata); }
    static JsErrorCode WINAPI JsGetRuntime(JsContextRef context, JsRuntimeHandle *runtime) { return chakracore::jsrt::JsGetRuntime(context, runtime); }
    static JsErrorCode WINAPI JsRelease(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsRelease(ref, count); }
    static JsErrorCode WINAPI JsAddRef(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsAddRef(ref, count); }
    static JsErrorCode WINAPI JsGetValueType(JsValueRef value, JsValueType *type) { return chakracore::jsrt::JsGetValueType(value, type); }
    static JsErrorCode WINAPI JsGetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef *value) { return chakracore::jsrt::JsGetIndexedProperty(object, index, value); }
    static JsErrorCode WINAPI JsSetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef value) { return chakracore::jsrt::JsSetIndexedProperty(object, index, value); }
    static JsErrorCode WINAPI JsSetPromiseContinuationCallback(JsPromiseContinuationCallback callback, void *callbackState) { return chakracore::jsrt::JsSetPromiseContinuationCallback(callback, callbackState); }
    static JsErrorCode WINAPI JsSetHostPromiseRejectionTracker(JsHostPromiseRejectionTrackerCallback callback, void *callbackState) { return chakracore::jsrt::JsSetHostPromiseRejectionTracker(callback, callbackState); }
    static JsErrorCode WINAPI JsGetContextOfObject(JsValueRef object, JsContextRef* context) { return chakracore::jsrt::JsGetContextOfObject(object, context); }
    static JsErrorCode WINAPI JsDiagStartDebugging(JsRuntimeHandle runtimeHandle, JsDiagDebugEventCallback debugEventCallback, void* callbackState) { return chakracore::jsrt::JsDiagStartDebugging(runtimeHandle, debugEventCallback, callbackState); }
    static JsErrorCode WINAPI JsDiagStopDebugging(JsRuntimeHandle runtimeHandle, void** callbackState) { return chakracore::jsrt::JsDiagStopDebugging(runtimeHandle, callbackState); }
    static JsErrorCode WINAPI JsDiagGetSource(unsigned int scriptId, JsValueRef *source) { return chakracore::jsrt::JsDiagGetSource(scriptId, source); }
    static JsErrorCode WINAPI JsDiagSetBreakpoint(unsigned int scriptId, unsigned int lineNumber, unsigned int columnNumber, JsValueRef *breakpoint) { return chakracore::jsrt::JsDiagSetBreakpoint(scriptId, lineNumber, columnNumber, breakpoint); }
    static JsErrorCode WINAPI JsDiagGetStackTrace(JsValueRef *stackTrace) { return chakracore::jsrt::JsDiagGetStackTrace(stackTrace); }
    static JsErrorCode WINAPI JsDiagRequestAsyncBreak(JsRuntimeHandle runtimeHandle) { return chakracore::jsrt::JsDiagRequestAsyncBreak(runtimeHandle); }
    static JsErrorCode WINAPI JsDiagGetBreakpoints(JsValueRef * breakpoints) { return chakracore::jsrt::JsDiagGetBreakpoints(breakpoints); }
    static JsErrorCode WINAPI JsDiagRemoveBreakpoint(unsigned int breakpointId) { return chakracore::jsrt::JsDiagRemoveBreakpoint(breakpointId); }
    static JsErrorCode WINAPI JsDiagSetBreakOnException(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes exceptionAttributes) { return chakracore::jsrt::JsDiagSetBreakOnException(runtimeHandle, exceptionAttributes); }
    static JsErrorCode WINAPI JsDiagGetBreakOnException(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes * exceptionAttributes) { return chakracore::jsrt::JsDiagGetBreakOnException(runtimeHandle, exceptionAttributes); }
    static JsErrorCode WINAPI JsDiagSetStepType(JsDiagStepType stepType) { return chakracore::jsrt::JsDiagSetStepType(stepType); }
    static JsErrorCode WINAPI JsDiagGetScripts(JsValueRef * scriptsArray) { return chakracore::jsrt::JsDiagGetScripts(scriptsArray); }
    static JsErrorCode WINAPI JsDiagGetFunctionPosition(JsValueRef value, JsValueRef * functionPosition) { return chakracore::jsrt::JsDiagGetFunctionPosition(value, functionPosition); }
    static JsErrorCode WINAPI JsDiagGetStackProperties(unsigned int stackFrameIndex, JsValueRef * properties) { return chakracore::jsrt::JsDiagGetStackProperties(stackFrameIndex, properties); }
    static JsErrorCode WINAPI JsDiagGetProperties(unsigned int objectHandle, unsigned int fromCount, unsigned int totalCount, JsValueRef * propertiesObject) { return chakracore::jsrt::JsDiagGetProperties(objectHandle, fromCount, totalCount, propertiesObject); }
    static JsErrorCode WINAPI JsDiagGetObjectFromHandle(unsigned int handle, JsValueRef * handleObject) { return chakracore::jsrt::JsDiagGetObjectFromHandle(handle, handleObject); }
    static JsErrorCode WINAPI JsDiagEvaluate(JsValueRef expression, unsigned int stackFrameIndex, JsParseScriptAttributes parseAttributes, bool forceSetValueProp, JsValueRef * evalResult) { return chakracore::jsrt::JsDiagEvaluate(expression, stackFrameIndex, parseAttributes, forceSetValueProp, evalResult); }
    static JsErrorCode WINAPI JsParseModuleSource(JsModuleRecord requestModule, JsSourceContext sourceContext, byte* sourceText, unsigned int sourceLength, JsParseModuleSourceFlags sourceFlag, JsValueRef* exceptionValueRef) {
        return chakracore::jsrt::JsParseModuleSource(requestModule, sourceContext, sourceText, sourceLength, sourceFlag, exceptionValueRef);
    }
    static JsErrorCode WINAPI JsModuleEvaluation(JsModuleRecord requestModule, JsValueRef* result) { return chakracore::jsrt::JsModuleEvaluation(requestModule, result); }
    static JsErrorCode WINAPI JsGetModuleNamespace(JsModuleRecord requestModule, JsValueRef *moduleNamespace) { return chakracore::jsrt::JsGetModuleNamespace(requestModule, moduleNamespace); }
    static JsErrorCode WINAPI JsInitializeModuleRecord(JsModuleRecord referencingModule, JsValueRef normalizedSpecifier, JsModuleRecord* moduleRecord) {
        return chakracore::jsrt::JsInitializeModuleRecord(referencingModule, normalizedSpecifier, moduleRecord);
    }
    static JsErrorCode WINAPI JsSetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void* hostInfo) { return chakracore::jsrt::JsSetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }
    static JsErrorCode WINAPI JsGetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void** hostInfo) { return chakracore::jsrt::JsGetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }

    static JsErrorCode WINAPI JsRun(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsRun(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode WINAPI JsParse(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsParse(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode WINAPI JsSerialize(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerialize(script, buffer, parseAttributes); }
    static JsErrorCode WINAPI JsRunSerialized(JsValueRef buffer, JsSerializedLoadScriptCallback scriptLoadCallback, JsSourceContext sourceContext, JsValueRef sourceUrl, JsValueRef * result) { return chakracore::jsrt::JsRunSerialized(buffer, scriptLoadCallback, sourceContext, sourceUrl, result); }
    static JsErrorCode WINAPI JsGetStringLength(JsValueRef value, int *stringLength) { return chakracore::jsrt::JsGetStringLength(value, stringLength); }
    static JsErrorCode WINAPI JsCopyString(JsValueRef value, char* buffer, size_t bufferSize, size_t* length) { return chakracore::jsrt::JsCopyString(value, buffer, bufferSize, length); }
    static JsErrorCode WINAPI JsCreateString(const char *content, size_t length, JsValueRef *value) { return chakracore::jsrt::JsCreateString(content, length, value); }
    static JsErrorCode WINAPI JsCreateString(const std::string &content, JsValueRef *value) { return chakracore::jsrt::JsCreateString(content.c_str(), content.length(), value); }
    static JsErrorCode WINAPI JsCreateString(const rust::String &content, JsValueRef *value) { return chakracore::jsrt::JsCreateString(static_cast<std::string>(content).c_str(), content.length(), value); }
    static JsErrorCode WINAPI JsCreateStringUtf16(const uint16_t *content, size_t length, JsValueRef *value) { return chakracore::jsrt::JsCreateStringUtf16(content, length, value); }
    static JsErrorCode WINAPI JsCreatePropertyId(const char *name, size_t length, JsPropertyIdRef *propertyId) { return chakracore::jsrt::JsCreatePropertyId(name, length, propertyId); }
    static JsErrorCode WINAPI JsCreateExternalArrayBuffer(void *data, unsigned int byteLength, JsFinalizeCallback finalizeCallback, void *callbackState, JsValueRef *result)  { return chakracore::jsrt::JsCreateExternalArrayBuffer(data, byteLength, finalizeCallback, callbackState, result); }
    static JsErrorCode WINAPI JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  { return chakracore::jsrt::JsGetProxyProperties(object, isProxy, target, handler); }

    static JsErrorCode WINAPI JsSerializeParserState(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerializeParserState(script, buffer, parseAttributes); }
    static JsErrorCode WINAPI JsRunScriptWithParserState(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef * result) { return chakracore::jsrt::JsRunScriptWithParserState(script, sourceContext, sourceUrl, parseAttributes, parserState, result); }

    static JsErrorCode WINAPI JsVarSerializer(ReallocateBufferMemoryFunc reallocateBufferMemory, WriteHostObjectFunc writeHostObject, void * callbackState, JsVarSerializerHandle *serializerHandle) { return chakracore::jsrt::JsVarSerializer(reallocateBufferMemory, writeHostObject, callbackState, serializerHandle); }
    static JsErrorCode WINAPI JsVarSerializerSetTransferableVars(JsVarSerializerHandle serializerHandle, JsValueRef *transferableVars, size_t transferableVarsCount) { return chakracore::jsrt::JsVarSerializerSetTransferableVars(serializerHandle, transferableVars, transferableVarsCount); }
    static JsErrorCode WINAPI JsVarSerializerWriteValue(JsVarSerializerHandle serializerHandle, JsValueRef rootObject) { return chakracore::jsrt::JsVarSerializerWriteValue(serializerHandle, rootObject); }
    static JsErrorCode WINAPI JsVarSerializerReleaseData(JsVarSerializerHandle serializerHandle, byte** data, size_t *dataLength) { return chakracore::jsrt::JsVarSerializerReleaseData(serializerHandle, data, dataLength); }
    static JsErrorCode WINAPI JsVarSerializerFree(JsVarSerializerHandle serializerHandle) { return chakracore::jsrt::JsVarSerializerFree(serializerHandle); }

    static JsErrorCode WINAPI JsVarDeserializer(void *data, size_t dataLength, ReadHostObjectFunc readHostObject, GetSharedArrayBufferFromIdFunc getSharedArrayBufferFromId, void* callbackState, JsVarDeserializerHandle *deserializerHandle) { return chakracore::jsrt::JsVarDeserializer(data, dataLength, readHostObject, getSharedArrayBufferFromId, callbackState, deserializerHandle); }
    static JsErrorCode WINAPI JsVarDeserializerSetTransferableVars(JsVarDeserializerHandle deserializerHandle, JsValueRef* transferableVars, size_t transferableVarsCount) { return chakracore::jsrt::JsVarDeserializerSetTransferableVars(deserializerHandle, transferableVars, transferableVarsCount); }
    static JsErrorCode WINAPI JsVarDeserializerReadValue(JsVarDeserializerHandle deserializerHandle, JsValueRef* value) { return chakracore::jsrt::JsVarDeserializerReadValue(deserializerHandle, value); }
    static JsErrorCode WINAPI JsVarDeserializerFree(JsVarDeserializerHandle deserializerHandle) { return chakracore::jsrt::JsVarDeserializerFree(deserializerHandle); }

    static JsErrorCode WINAPI JsDetachArrayBuffer(JsValueRef buffer) { return chakracore::jsrt::JsDetachArrayBuffer(buffer); }
    static JsErrorCode WINAPI JsQueueBackgroundParse_Experimental(JsScriptContents* contents, uint32_t* dwBgParseCookie) { return chakracore::jsrt::JsQueueBackgroundParse_Experimental(contents, dwBgParseCookie);  }
    static JsErrorCode WINAPI JsDiscardBackgroundParse_Experimental(uint32_t dwBgParseCookie, void* buffer, bool* callerOwnsBuffer) { return chakracore::jsrt::JsDiscardBackgroundParse_Experimental(dwBgParseCookie, buffer, callerOwnsBuffer); }
    static JsErrorCode WINAPI JsExecuteBackgroundParse_Experimental(uint32_t dwBgParseCookie, JsValueRef script, JsSourceContext sourceContext, char16_t *url, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef *result) { return chakracore::jsrt::JsExecuteBackgroundParse_Experimental(dwBgParseCookie, script, sourceContext, url, parseAttributes, parserState, result); }

    static JsErrorCode WINAPI JsGetArrayBufferFreeFunction(JsValueRef buffer, ArrayBufferFreeFn* freeFn) { return chakracore::jsrt::JsGetArrayBufferFreeFunction(buffer, freeFn); }
    static JsErrorCode WINAPI JsExternalizeArrayBuffer(JsValueRef buffer) { return chakracore::jsrt::JsExternalizeArrayBuffer(buffer); }
};

class AutoRestoreContext
{
public:
    AutoRestoreContext(JsContextRef newContext)
    {
        JsErrorCode errorCode = ChakraRTInterface::JsGetCurrentContext(&oldContext);
        Assert(errorCode == JsNoError);

        if (oldContext != newContext)
        {
            errorCode = ChakraRTInterface::JsSetCurrentContext(newContext);
            Assert(errorCode == JsNoError);
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
