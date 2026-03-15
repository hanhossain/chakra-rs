//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

struct JsAPIHooks
{
    typedef JsErrorCode (*JsrtCreateRuntimePtr)(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime);
    typedef JsErrorCode (*JsrtCreateContextPtr)(JsRuntimeHandle runtime, JsContextRef *newContext);
    typedef JsErrorCode (*JsrtSetObjectBeforeCollectCallbackPtr)(JsRef ref, void* callbackState, JsObjectBeforeCollectCallback objectBeforeCollectCallback);
    typedef JsErrorCode(*JsrtSetRuntimeDomWrapperTracingCallbacksPtr)(JsRuntimeHandle runtime, JsRef wrapperTracingState, JsDOMWrapperTracingCallback wrapperTracingCallback, JsDOMWrapperTracingDoneCallback wrapperTracingDoneCallback, JsDOMWrapperTracingEnterFinalPauseCallback enterFinalPauseCallback);
    typedef JsErrorCode (*JsrtSetRuntimeMemoryLimitPtr)(JsRuntimeHandle runtime, size_t memoryLimit);
    typedef JsErrorCode (*JsrtSetCurrentContextPtr)(JsContextRef context);
    typedef JsErrorCode (*JsrtGetCurrentContextPtr)(JsContextRef* context);
    typedef JsErrorCode (*JsrtDisposeRuntimePtr)(JsRuntimeHandle runtime);
    typedef JsErrorCode (*JsrtCreateObjectPtr)(JsValueRef *object);
    typedef JsErrorCode (*JsrtCreateExternalObjectPtr)(void* data, JsFinalizeCallback callback, JsValueRef *object);
    typedef JsErrorCode (*JsrtGetArrayForEachFunctionPtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtGetArrayKeysFunctionPtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtGetArrayValuesFunctionPtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtGetArrayEntriesFunctionPtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtGetPropertyIdSymbolIteratorPtr)(JsPropertyIdRef *propertyId);
    typedef JsErrorCode (*JsrtGetErrorPrototypePtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtGetIteratorPrototypePtr)(JsValueRef *result);
    typedef JsErrorCode (*JsrtCreateFunctionPtr)(JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function);
    typedef JsErrorCode (*JsrtCreateEnhancedFunctionPtr)(JsEnhancedNativeFunction nativeFunction, JsValueRef metadata, void *callbackState, JsValueRef *function);
    typedef JsErrorCode (*JsCreateNamedFunctionPtr)(JsValueRef name, JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function);
    typedef JsErrorCode (*JsrtSetPropertyPtr)(JsValueRef object, JsPropertyIdRef property, JsValueRef value, bool useStrictRules);
    typedef JsErrorCode (*JsrtGetGlobalObjectPtr)(JsValueRef *globalObject);
    typedef JsErrorCode (*JsrtGetUndefinedValuePtr)(JsValueRef *globalObject);
    typedef JsErrorCode (*JsrtConvertValueToStringPtr)(JsValueRef value, JsValueRef *stringValue);
    typedef JsErrorCode (*JsrtConvertValueToNumberPtr)(JsValueRef value, JsValueRef *numberValue);
    typedef JsErrorCode (*JsrtConvertValueToBooleanPtr)(JsValueRef value, JsValueRef *booleanValue);
    typedef JsErrorCode (*JsrtBooleanToBoolPtr)(JsValueRef value, bool *boolValue);
    typedef JsErrorCode (*JsrtGetPropertyPtr)(JsValueRef object, JsPropertyIdRef property, JsValueRef* value);
    typedef JsErrorCode (*JsrtHasPropertyPtr)(JsValueRef object, JsPropertyIdRef property, bool *hasProperty);
    typedef JsErrorCode (*JsInitializeModuleRecordPtr)(JsModuleRecord referencingModule, JsValueRef normalizedSpecifier, JsModuleRecord* moduleRecord);
    typedef JsErrorCode (*JsParseModuleSourcePtr)(JsModuleRecord requestModule, JsSourceContext sourceContext, byte* sourceText, unsigned int sourceLength, JsParseModuleSourceFlags sourceFlag, JsValueRef* exceptionValueRef);
    typedef JsErrorCode (*JsModuleEvaluationPtr)(JsModuleRecord requestModule, JsValueRef* result);
    typedef JsErrorCode (*JsGetModuleNamespacePtr)(JsModuleRecord requestModule, JsValueRef *moduleNamespace);
    typedef JsErrorCode (*JsSetModuleHostInfoPtr)(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void* hostInfo);
    typedef JsErrorCode (*JsGetModuleHostInfoPtr)(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void** hostInfo);
    typedef JsErrorCode (*JsrtCallFunctionPtr)(JsValueRef function, JsValueRef* arguments, unsigned short argumentCount, JsValueRef *result);
    typedef JsErrorCode (*JsrtNumberToDoublePtr)(JsValueRef value, double *doubleValue);
    typedef JsErrorCode (*JsrtNumberToIntPtr)(JsValueRef value, int *intValue);
    typedef JsErrorCode (*JsrtIntToNumberPtr)(int intValue, JsValueRef *value);
    typedef JsErrorCode (*JsrtDoubleToNumberPtr)(double doubleValue, JsValueRef* value);
    typedef JsErrorCode (*JsrtGetExternalDataPtr)(JsValueRef object, void **data);
    typedef JsErrorCode (*JsrtSetExternalDataPtr)(JsValueRef object, void *data);
    typedef JsErrorCode(*JsrtCloneObjectPtr)(JsValueRef object, JsValueRef *newObject);
    typedef JsErrorCode (*JsrtCreateArrayPtr)(unsigned int length, JsValueRef *result);
    typedef JsErrorCode (*JsrtCreateArrayBufferPtr)(unsigned int byteLength, JsValueRef *result);
    typedef JsErrorCode (*JsrtCreateSharedArrayBufferWithSharedContentPtr)(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result);
    typedef JsErrorCode (*JsrtGetSharedArrayBufferContentPtr)(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents);
    typedef JsErrorCode (*JsrtReleaseSharedArrayBufferContentHandlePtr)(JsSharedArrayBufferContentHandle sharedContent);
    typedef JsErrorCode (*JsrtGetArrayBufferStoragePtr)(JsValueRef instance, uint8_t **buffer, unsigned int *bufferLength);
    typedef JsErrorCode (*JsrtCreateErrorPtr)(JsValueRef message, JsValueRef *error);
    typedef JsErrorCode (*JsrtHasExceptionPtr)(bool *hasException);
    typedef JsErrorCode (*JsrtSetExceptionPtr)(JsValueRef exception);
    typedef JsErrorCode (*JsrtGetAndClearExceptiopnWithMetadataPtr)(JsValueRef* metadata);
    typedef JsErrorCode (*JsrtGetAndClearExceptiopnPtr)(JsValueRef* exception);
    typedef JsErrorCode (*JsrtGetRuntimePtr)(JsContextRef context, JsRuntimeHandle *runtime);
    typedef JsErrorCode (*JsrtReleasePtr)(JsRef ref, unsigned int* count);
    typedef JsErrorCode (*JsrtAddRefPtr)(JsRef ref, unsigned int* count);
    typedef JsErrorCode (*JsrtGetValueType)(JsValueRef value, JsValueType *type);
    typedef JsErrorCode(*JsrtGetIndexedPropertyPtr)(JsValueRef object, JsValueRef index, JsValueRef *value);
    typedef JsErrorCode (*JsrtSetIndexedPropertyPtr)(JsValueRef object, JsValueRef index, JsValueRef value);
    typedef JsErrorCode (*JsrtSetPromiseContinuationCallbackPtr)(JsPromiseContinuationCallback callback, void *callbackState);
    typedef JsErrorCode (*JsrtSetHostPromiseRejectionTrackerPtr)(JsHostPromiseRejectionTrackerCallback callback, void *callbackState);
    typedef JsErrorCode (*JsrtGetContextOfObject)(JsValueRef object, JsContextRef *callbackState);

    typedef JsErrorCode(*JsrtDiagStartDebugging)(JsRuntimeHandle runtimeHandle, JsDiagDebugEventCallback debugEventCallback, void* callbackState);
    typedef JsErrorCode(*JsrtDiagStopDebugging)(JsRuntimeHandle runtimeHandle, void** callbackState);
    typedef JsErrorCode(*JsrtDiagGetSource)(unsigned int scriptId, JsValueRef *source);
    typedef JsErrorCode(*JsrtDiagSetBreakpoint)(unsigned int scriptId, unsigned int lineNumber, unsigned int columnNumber, JsValueRef *breakPoint);
    typedef JsErrorCode(*JsrtDiagGetStackTrace)(JsValueRef *stackTrace);
    typedef JsErrorCode(*JsrtDiagRequestAsyncBreak)(JsRuntimeHandle runtimeHandle);
    typedef JsErrorCode(*JsrtDiagGetBreakpoints)(JsValueRef * breakpoints);
    typedef JsErrorCode(*JsrtDiagRemoveBreakpoint)(unsigned int breakpointId);
    typedef JsErrorCode(*JsrtDiagSetBreakOnException)(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes exceptionAttributes);
    typedef JsErrorCode(*JsrtDiagGetBreakOnException)(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes * exceptionAttributes);
    typedef JsErrorCode(*JsrtDiagSetStepType)(JsDiagStepType stepType);
    typedef JsErrorCode(*JsrtDiagGetScripts)(JsValueRef * scriptsArray);
    typedef JsErrorCode(*JsrtDiagGetFunctionPosition)(JsValueRef value, JsValueRef * functionInfo);
    typedef JsErrorCode(*JsrtDiagGetStackProperties)(unsigned int stackFrameIndex, JsValueRef * properties);
    typedef JsErrorCode(*JsrtDiagGetProperties)(unsigned int objectHandle, unsigned int fromCount, unsigned int totalCount, JsValueRef * propertiesObject);
    typedef JsErrorCode(*JsrtDiagGetObjectFromHandle)(unsigned int handle, JsValueRef * handleObject);
    typedef JsErrorCode(*JsrtDiagEvaluate)(JsValueRef expression, unsigned int stackFrameIndex, JsParseScriptAttributes parseAttributes, bool forceSetValueProp, JsValueRef * evalResult);

    typedef JsErrorCode(*JsrtRun)(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result);
    typedef JsErrorCode(*JsrtParse)(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result);
    typedef JsErrorCode(*JsrtSerialize)(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes);
    typedef JsErrorCode(*JsrtRunSerialized)(JsValueRef buffer, JsSerializedLoadScriptCallback scriptLoadCallback, JsSourceContext sourceContext, JsValueRef sourceUrl, JsValueRef * result);
    typedef JsErrorCode(*JsrtGetStringLength)(JsValueRef value, int *stringLength);
    typedef JsErrorCode(*JsrtCopyString)(JsValueRef value, char* buffer, size_t bufferSize, size_t* length);
    typedef JsErrorCode(*JsrtCreateString)(const char *content, size_t length, JsValueRef *value);
    typedef JsErrorCode(*JsrtCreateStringUtf16)(const uint16_t *content, size_t length, JsValueRef *value);
    
    typedef JsErrorCode(*JsrtCreateExternalArrayBuffer)(void *data, unsigned int byteLength, JsFinalizeCallback finalizeCallback, void *callbackState, JsValueRef *result);
    typedef JsErrorCode(*JsrtCreatePropertyId)(const char *name, size_t length, JsPropertyIdRef *propertyId);
    typedef JsErrorCode(*JsrtGetProxyProperties)(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler);

    typedef JsErrorCode(*JsrtSerializeParserState)(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes);
    typedef JsErrorCode(*JsrtRunScriptWithParserState)(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef *result);
    
    typedef JsErrorCode(*JsrtQueueBackgroundParse_Experimental)(JsScriptContents* contents, uint32_t* dwBgParseCookie);
    typedef JsErrorCode(*JsrtDiscardBackgroundParse_Experimental)(uint32_t dwBgParseCookie, void* buffer, bool* callerOwnsBuffer);
    typedef JsErrorCode(*JsrtExecuteBackgroundParse_Experimental)(uint32_t dwBgParseCookie, JsValueRef script, JsSourceContext sourceContext, char16_t *url, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef *result);

    typedef JsErrorCode(*JsrtTTDCreateRecordRuntimePtr)(JsRuntimeAttributes attributes, bool enableDebugging, size_t snapInterval, size_t snapHistoryLength, TTDOpenResourceStreamCallback openResourceStream, JsTTDWriteBytesToStreamCallback writeBytesToStream, JsTTDFlushAndCloseStreamCallback flushAndCloseStream, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime);
    typedef JsErrorCode(*JsrtTTDCreateReplayRuntimePtr)(JsRuntimeAttributes attributes, const char* infoUri, size_t infoUriCount, bool enableDebugging, TTDOpenResourceStreamCallback openResourceStream, JsTTDReadBytesFromStreamCallback readBytesFromStream, JsTTDFlushAndCloseStreamCallback flushAndCloseStream, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime);
    typedef JsErrorCode(*JsrtTTDCreateContextPtr)(JsRuntimeHandle runtime, bool useRuntimeTTDMode, JsContextRef *newContext);
    typedef JsErrorCode(*JsrtTTDNotifyContextDestroyPtr)(JsContextRef context);

    typedef JsErrorCode(*JsrtTTDStartPtr)();
    typedef JsErrorCode(*JsrtTTDStopPtr)();

    typedef JsErrorCode(*JsrtTTDNotifyYieldPtr)();
    typedef JsErrorCode(*JsrtTTDHostExitPtr)(int statusCode);

    typedef JsErrorCode(*JsrtTTDGetSnapTimeTopLevelEventMovePtr)(JsRuntimeHandle runtimeHandle, JsTTDMoveMode moveMode, uint32_t kthEvent, int64_t* targetEventTime, int64_t* targetStartSnapTime, int64_t* targetEndSnapTime);
    typedef JsErrorCode(*JsrtTTDMoveToTopLevelEventPtr)(JsRuntimeHandle runtimeHandle, JsTTDMoveMode moveMode, int64_t snapshotStartTime, int64_t eventTime);
    typedef JsErrorCode(*JsrtTTDReplayExecutionPtr)(JsTTDMoveMode* moveMode, int64_t* rootEventTime);

    typedef JsErrorCode(*JsrtVarSerializerPtr)(ReallocateBufferMemoryFunc reallocateBufferMemory, WriteHostObjectFunc writeHostObject, void * callbackState, JsVarSerializerHandle *serializerHandle);
    typedef JsErrorCode(*JsrtVarSerializerSetTransferableVarsPtr)(JsVarSerializerHandle serializerHandle, JsValueRef *transferableVars, size_t transferableVarsCount);
    typedef JsErrorCode(*JsrtVarSerializerWriteValuePtr)(JsVarSerializerHandle serializerHandle, JsValueRef rootObject);
    typedef JsErrorCode(*JsrtVarSerializerReleaseDataPtr)(JsVarSerializerHandle serializerHandle, byte** data, size_t *dataLength);
    typedef JsErrorCode(*JsrtVarSerializerFreePtr)(JsVarSerializerHandle serializerHandle);

    typedef JsErrorCode(*JsrtVarDeserializerPtr)(void *data, size_t dataLength, ReadHostObjectFunc readHostObject, GetSharedArrayBufferFromIdFunc getSharedArrayBufferFromId, void* callbackState, JsVarDeserializerHandle *deserializerHandle);
    typedef JsErrorCode(*JsrtVarDeserializerSetTransferableVarsPtr)(JsVarDeserializerHandle deserializerHandle, JsValueRef *transferableVars, size_t transferableVarsCount);
    typedef JsErrorCode(*JsrtVarDeserializerReadValuePtr)(JsVarDeserializerHandle deserializerHandle, JsValueRef* value);
    typedef JsErrorCode(*JsrtVarDeserializerFreePtr)(JsVarDeserializerHandle deserializerHandle);

    typedef JsErrorCode(*JsrtDetachArrayBufferPtr)(JsValueRef buffer);
    typedef JsErrorCode(*JsrtGetArrayBufferFreeFunction)(JsValueRef buffer, ArrayBufferFreeFn* freeFn);
    typedef JsErrorCode(*JsrtExternalizeArrayBufferPtr)(JsValueRef buffer);

    JsrtCreateRuntimePtr pfJsrtCreateRuntime;
    JsrtCreateContextPtr pfJsrtCreateContext;
    JsrtSetObjectBeforeCollectCallbackPtr pfJsrtSetObjectBeforeCollectCallback;
    JsrtSetRuntimeDomWrapperTracingCallbacksPtr pfJsrtSetRuntimeDomWrapperTracingCallbacks;
    JsrtSetRuntimeMemoryLimitPtr pfJsrtSetRuntimeMemoryLimit;
    JsrtSetCurrentContextPtr pfJsrtSetCurrentContext;
    JsrtGetCurrentContextPtr pfJsrtGetCurrentContext;
    JsrtDisposeRuntimePtr pfJsrtDisposeRuntime;
    JsrtCreateObjectPtr pfJsrtCreateObject;
    JsrtCreateExternalObjectPtr pfJsrtCreateExternalObject;
    JsrtGetArrayForEachFunctionPtr pfJsrtGetArrayForEachFunction;
    JsrtGetArrayKeysFunctionPtr pfJsrtGetArrayKeysFunction;
    JsrtGetArrayValuesFunctionPtr pfJsrtGetArrayValuesFunction;
    JsrtGetArrayEntriesFunctionPtr pfJsrtGetArrayEntriesFunction;
    JsrtGetPropertyIdSymbolIteratorPtr pfJsrtGetPropertyIdSymbolIterator;
    JsrtGetErrorPrototypePtr pfJsrtGetErrorPrototype;
    JsrtGetIteratorPrototypePtr pfJsrtGetIteratorPrototype;

    JsrtCreateFunctionPtr pfJsrtCreateFunction;
    JsrtCreateEnhancedFunctionPtr pfJsrtCreateEnhancedFunction;
    JsCreateNamedFunctionPtr pfJsrtCreateNamedFunction;
    JsrtSetPropertyPtr pfJsrtSetProperty;
    JsrtGetGlobalObjectPtr pfJsrtGetGlobalObject;
    JsrtGetUndefinedValuePtr pfJsrtGetUndefinedValue;
    JsrtGetUndefinedValuePtr pfJsrtGetNullValue;
    JsrtGetUndefinedValuePtr pfJsrtGetTrueValue;
    JsrtGetUndefinedValuePtr pfJsrtGetFalseValue;
    JsrtConvertValueToStringPtr pfJsrtConvertValueToString;
    JsrtConvertValueToNumberPtr pfJsrtConvertValueToNumber;
    JsrtConvertValueToBooleanPtr pfJsrtConvertValueToBoolean;
    JsrtBooleanToBoolPtr pfJsrtBooleanToBool;
    JsrtGetPropertyPtr pfJsrtGetProperty;
    JsrtHasPropertyPtr pfJsrtHasProperty;
    JsParseModuleSourcePtr pfJsrtParseModuleSource;
    JsInitializeModuleRecordPtr pfJsrtInitializeModuleRecord;
    JsModuleEvaluationPtr pfJsrtModuleEvaluation;
    JsGetModuleNamespacePtr pfJsrtGetModuleNamespace;
    JsSetModuleHostInfoPtr pfJsrtSetModuleHostInfo;
    JsGetModuleHostInfoPtr pfJsrtGetModuleHostInfo;
    JsrtCallFunctionPtr pfJsrtCallFunction;
    JsrtNumberToDoublePtr pfJsrtNumberToDouble;
    JsrtNumberToIntPtr pfJsrtNumberToInt;
    JsrtIntToNumberPtr pfJsrtIntToNumber;
    JsrtDoubleToNumberPtr pfJsrtDoubleToNumber;
    JsrtGetExternalDataPtr pfJsrtGetExternalData;
    JsrtSetExternalDataPtr pfJsrtSetExternalData;
    JsrtCloneObjectPtr pfJsrtCloneObject;
    JsrtCreateArrayPtr pfJsrtCreateArray;
    JsrtCreateArrayBufferPtr pfJsrtCreateArrayBuffer;
    JsrtCreateSharedArrayBufferWithSharedContentPtr pfJsrtCreateSharedArrayBufferWithSharedContent;
    JsrtGetSharedArrayBufferContentPtr pfJsrtGetSharedArrayBufferContent;    
    JsrtReleaseSharedArrayBufferContentHandlePtr pfJsrtReleaseSharedArrayBufferContentHandle;
    JsrtGetArrayBufferStoragePtr pfJsrtGetArrayBufferStorage;
    JsrtCreateErrorPtr pfJsrtCreateError;
    JsrtHasExceptionPtr pfJsrtHasException;
    JsrtSetExceptionPtr pfJsrtSetException;
    JsrtGetAndClearExceptiopnWithMetadataPtr pfJsrtGetAndClearExceptionWithMetadata;
    JsrtGetAndClearExceptiopnPtr pfJsrtGetAndClearException;
    JsrtGetRuntimePtr pfJsrtGetRuntime;
    JsrtReleasePtr pfJsrtRelease;
    JsrtAddRefPtr pfJsrtAddRef;
    JsrtGetValueType pfJsrtGetValueType;
    JsrtGetIndexedPropertyPtr pfJsrtGetIndexedProperty;
    JsrtSetIndexedPropertyPtr pfJsrtSetIndexedProperty;
    JsrtSetPromiseContinuationCallbackPtr pfJsrtSetPromiseContinuationCallback;
    JsrtSetHostPromiseRejectionTrackerPtr pfJsrtSetHostPromiseRejectionTracker;
    JsrtGetContextOfObject pfJsrtGetContextOfObject;
    JsrtDiagStartDebugging pfJsrtDiagStartDebugging;
    JsrtDiagStopDebugging pfJsrtDiagStopDebugging;
    JsrtDiagGetSource pfJsrtDiagGetSource;
    JsrtDiagSetBreakpoint pfJsrtDiagSetBreakpoint;
    JsrtDiagGetStackTrace pfJsrtDiagGetStackTrace;
    JsrtDiagRequestAsyncBreak pfJsrtDiagRequestAsyncBreak;
    JsrtDiagGetBreakpoints pfJsrtDiagGetBreakpoints;
    JsrtDiagRemoveBreakpoint pfJsrtDiagRemoveBreakpoint;
    JsrtDiagSetBreakOnException pfJsrtDiagSetBreakOnException;
    JsrtDiagGetBreakOnException pfJsrtDiagGetBreakOnException;
    JsrtDiagSetStepType pfJsrtDiagSetStepType;
    JsrtDiagGetScripts pfJsrtDiagGetScripts;
    JsrtDiagGetFunctionPosition pfJsrtDiagGetFunctionPosition;
    JsrtDiagGetStackProperties pfJsrtDiagGetStackProperties;
    JsrtDiagGetProperties pfJsrtDiagGetProperties;
    JsrtDiagGetObjectFromHandle pfJsrtDiagGetObjectFromHandle;
    JsrtDiagEvaluate pfJsrtDiagEvaluate;

    JsrtRun pfJsrtRun;
    JsrtParse pfJsrtParse;
    JsrtSerialize pfJsrtSerialize;
    JsrtRunSerialized pfJsrtRunSerialized;
    JsrtGetStringLength pfJsrtGetStringLength;
    JsrtCreateString pfJsrtCreateString;
    JsrtCreateStringUtf16 pfJsrtCreateStringUtf16;
    JsrtCopyString pfJsrtCopyString;
    JsrtCreatePropertyId pfJsrtCreatePropertyId;
    JsrtCreateExternalArrayBuffer pfJsrtCreateExternalArrayBuffer;
    JsrtGetProxyProperties pfJsrtGetProxyProperties;
    JsrtSerializeParserState pfJsrtSerializeParserState;
    JsrtRunScriptWithParserState pfJsrtRunScriptWithParserState;

    JsrtQueueBackgroundParse_Experimental pfJsrtQueueBackgroundParse_Experimental;
    JsrtDiscardBackgroundParse_Experimental pfJsrtDiscardBackgroundParse_Experimental;
    JsrtExecuteBackgroundParse_Experimental pfJsrtExecuteBackgroundParse_Experimental;

    JsrtTTDCreateRecordRuntimePtr pfJsrtTTDCreateRecordRuntime;
    JsrtTTDCreateReplayRuntimePtr pfJsrtTTDCreateReplayRuntime;
    JsrtTTDCreateContextPtr pfJsrtTTDCreateContext;
    JsrtTTDNotifyContextDestroyPtr pfJsrtTTDNotifyContextDestroy;

    JsrtTTDStartPtr pfJsrtTTDStart;
    JsrtTTDStopPtr pfJsrtTTDStop;

    JsrtTTDNotifyYieldPtr pfJsrtTTDNotifyYield;
    JsrtTTDHostExitPtr pfJsrtTTDHostExit;

    JsrtTTDGetSnapTimeTopLevelEventMovePtr pfJsrtTTDGetSnapTimeTopLevelEventMove;
    JsrtTTDMoveToTopLevelEventPtr pfJsrtTTDMoveToTopLevelEvent;
    JsrtTTDReplayExecutionPtr pfJsrtTTDReplayExecution;

    JsrtVarSerializerPtr pfJsrtVarSerializer;
    JsrtVarSerializerSetTransferableVarsPtr pfJsrtVarSerializerSetTransferableVars;
    JsrtVarSerializerWriteValuePtr pfJsrtVarSerializerWriteValue;
    JsrtVarSerializerReleaseDataPtr pfJsrtVarSerializerReleaseData;
    JsrtVarSerializerFreePtr pfJsrtVarSerializerFree;

    JsrtVarDeserializerPtr pfJsrtVarDeserializer;
    JsrtVarDeserializerSetTransferableVarsPtr pfJsrtVarDeserializerSetTransferableVars;
    JsrtVarDeserializerReadValuePtr pfJsrtVarDeserializerReadValue;
    JsrtVarDeserializerFreePtr pfJsrtVarDeserializerFree;

    JsrtDetachArrayBufferPtr pfJsrtDetachArrayBuffer;
    JsrtGetArrayBufferFreeFunction pfJsrtGetArrayBufferFreeFunction;
    JsrtExternalizeArrayBufferPtr pfJsrtExternalizeArrayBuffer;
};

class ChakraRTInterface
{
public:
    typedef void(* HostPrintUsageFuncPtr)();

    struct ArgInfo
    {
        int argc;
        char16_t ** argv;
        HostPrintUsageFuncPtr hostPrintUsage;
        char* filename;

        ArgInfo() :
            argc(0),
            argv(nullptr),
            hostPrintUsage(nullptr),
            filename(nullptr)
        {
        }

        ArgInfo(int argc, char16_t ** argv, HostPrintUsageFuncPtr hostPrintUsage, char* filename) :
            argc(argc),
            argv(argv),
            hostPrintUsage(hostPrintUsage),
            filename(filename)
        {
        }

        ~ArgInfo()
        {
            if (filename != nullptr)
            {
                free(filename);
            }
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
    static JsAPIHooks m_jsApiHooks;

private:
    static int32_t ParseConfigFlags();

public:
    static int32_t OnChakraCoreLoaded(TestHooks& testHooks);

    static bool LoadChakraDll(ArgInfo* argInfo, HINSTANCE *library);

    static int32_t SetAssertToConsoleFlag(bool flag) { return CHECKED_CALL(SetAssertToConsoleFlag, flag); }
    static int32_t SetConfigFlags(int argc, __in_ecount(argc) char16_t * argv[], ICustomConfigFlags* customConfigFlags) { return CHECKED_CALL(SetConfigFlags, argc, argv, customConfigFlags); }
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
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    static int32_t SetOOPCFGRegistrationFlag(bool flag) { return CHECKED_CALL(SetOOPCFGRegistrationFlag, flag); }
#endif

    static int32_t GetCrashOnExceptionFlag(bool * flag)
    {
#ifdef SECURITY_TESTING
        return CHECKED_CALL(GetCrashOnExceptionFlag, flag);
#else
        return E_UNEXPECTED;
#endif
    }

    static void NotifyUnhandledException(PEXCEPTION_POINTERS exceptionInfo)
    {
        if (m_testHooksSetup && m_testHooks.pfnNotifyUnhandledException != NULL)
        {
            m_testHooks.pfnNotifyUnhandledException(exceptionInfo);
        }
    }

    static JsErrorCode JsCreateRuntime(JsRuntimeAttributes attributes, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime) {
      return ::JsCreateRuntime(attributes, threadService, runtime);
    }
    static JsErrorCode JsCreateContext(JsRuntimeHandle runtime, JsContextRef *newContext) {
      return ::JsCreateContext(runtime, newContext);
    }
    static JsErrorCode JsSetObjectBeforeCollectCallback(JsRef ref, void* callbackState, JsObjectBeforeCollectCallback objectBeforeCollectCallback) {
      return ::JsSetObjectBeforeCollectCallback(ref, callbackState,
                                                objectBeforeCollectCallback);
    }
    static JsErrorCode JsSetRuntimeDomWrapperTracingCallbacks(JsRuntimeHandle runtime, JsRef wrapperTracingState, JsDOMWrapperTracingCallback wrapperTracingCallback, JsDOMWrapperTracingDoneCallback wrapperTracingDoneCallback, JsDOMWrapperTracingEnterFinalPauseCallback enterFinalPauseCallback) {
      return ::JsSetRuntimeDomWrapperTracingCallbacks(
          runtime, wrapperTracingState, wrapperTracingCallback,
          wrapperTracingDoneCallback, enterFinalPauseCallback);
    }
    static JsErrorCode JsSetRuntimeMemoryLimit(JsRuntimeHandle runtime, size_t memory) {
      return ::JsSetRuntimeMemoryLimit(runtime, memory);
    }
    static JsErrorCode JsSetCurrentContext(JsContextRef context) {
      return ::JsSetCurrentContext(context);
    }
    static JsErrorCode JsGetCurrentContext(JsContextRef* context) {
      return ::JsGetCurrentContext(context);
    }
    static JsErrorCode JsDisposeRuntime(JsRuntimeHandle runtime) {
      return ::JsDisposeRuntime(runtime);
    }
    static JsErrorCode JsCreateObject(JsValueRef *object) {
      return ::JsCreateObject(object);
    }
    static JsErrorCode JsCreateExternalObject(void *data, JsFinalizeCallback callback, JsValueRef *object) {
      return ::JsCreateExternalObject(data, callback, object);
    }
    static JsErrorCode JsGetArrayForEachFunction(JsValueRef * result) {
      return ::JsGetArrayForEachFunction(result);
    }
    static JsErrorCode JsGetArrayKeysFunction(JsValueRef * result) {
      return ::JsGetArrayKeysFunction(result);
    }
    static JsErrorCode JsGetArrayValuesFunction(JsValueRef * result) {
      return ::JsGetArrayValuesFunction(result);
    }
    static JsErrorCode JsGetArrayEntriesFunction(JsValueRef * result) {
      return ::JsGetArrayEntriesFunction(result);
    }
    static JsErrorCode JsGetPropertyIdSymbolIterator(JsPropertyIdRef * propertyId) {
      return ::JsGetPropertyIdSymbolIterator(propertyId);
    }
    static JsErrorCode JsGetErrorPrototype(JsValueRef * result) {
      return ::JsGetErrorPrototype(result);
    }
    static JsErrorCode JsGetIteratorPrototype(JsValueRef * result) {
      return ::JsGetIteratorPrototype(result);
    }
    static JsErrorCode JsCreateFunction(JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) {
      return ::JsCreateFunction(nativeFunction, callbackState, function);
    }
    static JsErrorCode JsCreateEnhancedFunction(JsEnhancedNativeFunction nativeFunction, JsValueRef metadata, void *callbackState, JsValueRef *function) {
      return ::JsCreateEnhancedFunction(nativeFunction, metadata, callbackState,
                                        function);
    }
    static JsErrorCode JsCreateNamedFunction(JsValueRef name, JsNativeFunction nativeFunction, void *callbackState, JsValueRef *function) {
      return ::JsCreateNamedFunction(name, nativeFunction, callbackState,
                                     function);
    }
    static JsErrorCode JsSetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef value, bool useStrictRules) {
      return ::JsSetProperty(object, property, value, useStrictRules);
    }
    static JsErrorCode JsGetGlobalObject(JsValueRef *globalObject) {
      return ::JsGetGlobalObject(globalObject);
    }
    static JsErrorCode JsGetUndefinedValue(JsValueRef *globalObject) {
      return ::JsGetUndefinedValue(globalObject);
    }
    static JsErrorCode JsGetNullValue(JsValueRef *globalObject) {
      return ::JsGetNullValue(globalObject);
    }
    static JsErrorCode JsGetTrueValue(JsValueRef *globalObject) {
      return ::JsGetTrueValue(globalObject);
    }
    static JsErrorCode JsGetFalseValue(JsValueRef *globalObject) {
      return ::JsGetFalseValue(globalObject);
    }
    static JsErrorCode JsConvertValueToString(JsValueRef value, JsValueRef *stringValue) {
      return ::JsConvertValueToString(value, stringValue);
    }
    static JsErrorCode JsConvertValueToNumber(JsValueRef value, JsValueRef *numberValue) {
      return ::JsConvertValueToNumber(value, numberValue);
    }
    static JsErrorCode JsConvertValueToBoolean(JsValueRef value, JsValueRef *booleanValue) {
      return ::JsConvertValueToBoolean(value, booleanValue);
    }
    static JsErrorCode JsBooleanToBool(JsValueRef value, bool* boolValue) {
      return ::JsBooleanToBool(value, boolValue);
    }
    static JsErrorCode JsGetProperty(JsValueRef object, JsPropertyIdRef property, JsValueRef* value) {
      return ::JsGetProperty(object, property, value);
    }
    static JsErrorCode JsHasProperty(JsValueRef object, JsPropertyIdRef property, bool *hasProperty) {
      return ::JsHasProperty(object, property, hasProperty);
    }
    static JsErrorCode JsCallFunction(JsValueRef function, JsValueRef* arguments, unsigned short argumentCount, JsValueRef *result) {
      return ::JsCallFunction(function, arguments, argumentCount, result);
    }
    static JsErrorCode JsNumberToDouble(JsValueRef value, double* doubleValue) {
      return ::JsNumberToDouble(value, doubleValue);
    }
    static JsErrorCode JsNumberToInt(JsValueRef value, int* intValue) {
      return ::JsNumberToInt(value, intValue);
    }
    static JsErrorCode JsIntToNumber(int intValue, JsValueRef *value) {
      return ::JsIntToNumber(intValue, value);
    }
    static JsErrorCode JsDoubleToNumber(double doubleValue, JsValueRef* value) {
      return ::JsDoubleToNumber(doubleValue, value);
    }
    static JsErrorCode JsGetExternalData(JsValueRef object, void **data) {
      return ::JsGetExternalData(object, data);
    }
    static JsErrorCode JsSetExternalData(JsValueRef object, void *data)  {
      return ::JsSetExternalData(object, data);
    }
    static JsErrorCode JsCloneObject(JsValueRef object, JsValueRef *data) {
      return ::JsCloneObject(object, data);
    }
    static JsErrorCode JsCreateArray(unsigned int length, JsValueRef *result) {
      return ::JsCreateArray(length, result);
    }
    static JsErrorCode JsCreateArrayBuffer(unsigned int byteLength, JsValueRef *result) {
      return ::JsCreateArrayBuffer(byteLength, result);
    }
    static JsErrorCode JsCreateSharedArrayBufferWithSharedContent(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result) {
      return ::JsCreateSharedArrayBufferWithSharedContent(sharedContent,
                                                          result);
    }
    static JsErrorCode JsGetSharedArrayBufferContent(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents) {
      return ::JsGetSharedArrayBufferContent(sharedArrayBuffer, sharedContents);
    }
    static JsErrorCode JsReleaseSharedArrayBufferContentHandle(JsSharedArrayBufferContentHandle sharedContent) {
      return ::JsReleaseSharedArrayBufferContentHandle(sharedContent);
    }
    static JsErrorCode JsGetArrayBufferStorage(JsValueRef instance, uint8_t **buffer, unsigned int *bufferLength) {
      return ::JsGetArrayBufferStorage(instance, buffer, bufferLength);
    }
    static JsErrorCode JsCreateError(JsValueRef message, JsValueRef *error) {
      return ::JsCreateError(message, error);
    }
    static JsErrorCode JsHasException(bool *hasException) {
      return ::JsHasException(hasException);
    }
    static JsErrorCode JsSetException(JsValueRef exception) {
      return ::JsSetException(exception);
    }
    static JsErrorCode JsGetAndClearException(JsValueRef *exception) {
      return ::JsGetAndClearException(exception);
    }
    static JsErrorCode JsGetAndClearExceptionWithMetadata(JsValueRef * metadata) {
      return ::JsGetAndClearExceptionWithMetadata(metadata);
    }
    static JsErrorCode JsGetRuntime(JsContextRef context, JsRuntimeHandle *runtime) {
      return ::JsGetRuntime(context, runtime);
    }
    static JsErrorCode JsRelease(JsRef ref, unsigned int* count) {
      return ::JsRelease(ref, count);
    }
    static JsErrorCode JsAddRef(JsRef ref, unsigned int* count) {
      return ::JsAddRef(ref, count);
    }
    static JsErrorCode JsGetValueType(JsValueRef value, JsValueType *type) {
      return ::JsGetValueType(value, type);
    }
    static JsErrorCode JsGetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef *value) {
      return ::JsGetIndexedProperty(object, index, value);
    }
    static JsErrorCode JsSetIndexedProperty(JsValueRef object, JsValueRef index, JsValueRef value) {
      return ::JsSetIndexedProperty(object, index, value);
    }
    static JsErrorCode JsSetPromiseContinuationCallback(JsPromiseContinuationCallback callback, void *callbackState) {
      return ::JsSetPromiseContinuationCallback(callback, callbackState);
    }
    static JsErrorCode JsSetHostPromiseRejectionTracker(JsHostPromiseRejectionTrackerCallback callback, void *callbackState) {
      return ::JsSetHostPromiseRejectionTracker(callback, callbackState);
    }
    static JsErrorCode JsGetContextOfObject(JsValueRef object, JsContextRef* context) {
      return ::JsGetContextOfObject(object, context);
    }
    static JsErrorCode JsDiagStartDebugging(JsRuntimeHandle runtimeHandle, JsDiagDebugEventCallback debugEventCallback, void* callbackState) {
      return ::JsDiagStartDebugging(runtimeHandle, debugEventCallback,
                                    callbackState);
    }
    static JsErrorCode JsDiagStopDebugging(JsRuntimeHandle runtimeHandle, void** callbackState) {
      return ::JsDiagStopDebugging(runtimeHandle, callbackState);
    }
    static JsErrorCode JsDiagGetSource(unsigned int scriptId, JsValueRef *source) {
      return ::JsDiagGetSource(scriptId, source);
    }
    static JsErrorCode JsDiagSetBreakpoint(unsigned int scriptId, unsigned int lineNumber, unsigned int columnNumber, JsValueRef *breakpoint) {
      return ::JsDiagSetBreakpoint(scriptId, lineNumber, columnNumber,
                                   breakpoint);
    }
    static JsErrorCode JsDiagGetStackTrace(JsValueRef *stackTrace) {
      return ::JsDiagGetStackTrace(stackTrace);
    }
    static JsErrorCode JsDiagRequestAsyncBreak(JsRuntimeHandle runtimeHandle) {
      return ::JsDiagRequestAsyncBreak(runtimeHandle);
    }
    static JsErrorCode JsDiagGetBreakpoints(JsValueRef * breakpoints) {
      return ::JsDiagGetBreakpoints(breakpoints);
    }
    static JsErrorCode JsDiagRemoveBreakpoint(unsigned int breakpointId) {
      return ::JsDiagRemoveBreakpoint(breakpointId);
    }
    static JsErrorCode JsDiagSetBreakOnException(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes exceptionAttributes) {
      return ::JsDiagSetBreakOnException(runtimeHandle, exceptionAttributes);
    }
    static JsErrorCode JsDiagGetBreakOnException(JsRuntimeHandle runtimeHandle, JsDiagBreakOnExceptionAttributes * exceptionAttributes) {
      return ::JsDiagGetBreakOnException(runtimeHandle, exceptionAttributes);
    }
    static JsErrorCode JsDiagSetStepType(JsDiagStepType stepType) {
      return ::JsDiagSetStepType(stepType);
    }
    static JsErrorCode JsDiagGetScripts(JsValueRef * scriptsArray) {
      return ::JsDiagGetScripts(scriptsArray);
    }
    static JsErrorCode JsDiagGetFunctionPosition(JsValueRef value, JsValueRef * functionPosition) {
      return ::JsDiagGetFunctionPosition(value, functionPosition);
    }
    static JsErrorCode JsDiagGetStackProperties(unsigned int stackFrameIndex, JsValueRef * properties) {
      return ::JsDiagGetStackProperties(stackFrameIndex, properties);
    }
    static JsErrorCode JsDiagGetProperties(unsigned int objectHandle, unsigned int fromCount, unsigned int totalCount, JsValueRef * propertiesObject) {
      return ::JsDiagGetProperties(objectHandle, fromCount, totalCount,
                                   propertiesObject);
    }
    static JsErrorCode JsDiagGetObjectFromHandle(unsigned int handle, JsValueRef * handleObject) {
      return ::JsDiagGetObjectFromHandle(handle, handleObject);
    }
    static JsErrorCode JsDiagEvaluate(JsValueRef expression, unsigned int stackFrameIndex, JsParseScriptAttributes parseAttributes, bool forceSetValueProp, JsValueRef * evalResult) {
      return ::JsDiagEvaluate(expression, stackFrameIndex, parseAttributes,
                              forceSetValueProp, evalResult);
    }
    static JsErrorCode JsParseModuleSource(JsModuleRecord requestModule, JsSourceContext sourceContext, byte* sourceText, unsigned int sourceLength, JsParseModuleSourceFlags sourceFlag, JsValueRef* exceptionValueRef) {
      return ::JsParseModuleSource(requestModule, sourceContext, sourceText,
                                   sourceLength, sourceFlag, exceptionValueRef);
    }
    static JsErrorCode JsModuleEvaluation(JsModuleRecord requestModule, JsValueRef* result) {
      return ::JsModuleEvaluation(requestModule, result);
    }
    static JsErrorCode JsGetModuleNamespace(JsModuleRecord requestModule, JsValueRef *moduleNamespace) {
      return ::JsGetModuleNamespace(requestModule, moduleNamespace);
    }
    static JsErrorCode JsInitializeModuleRecord(JsModuleRecord referencingModule, JsValueRef normalizedSpecifier, JsModuleRecord* moduleRecord) {
      return ::JsInitializeModuleRecord(referencingModule, normalizedSpecifier,
                                        moduleRecord);
    }
    static JsErrorCode JsSetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void* hostInfo) {
      return ::JsSetModuleHostInfo(requestModule, moduleHostInfo, hostInfo);
    }
    static JsErrorCode JsGetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void** hostInfo) {
      return ::JsGetModuleHostInfo(requestModule, moduleHostInfo, hostInfo);
    }

    static JsErrorCode JsTTDCreateRecordRuntime(JsRuntimeAttributes attributes, size_t snapInterval, size_t snapHistoryLength, TTDOpenResourceStreamCallback openResourceStream, JsTTDWriteBytesToStreamCallback writeBytesToStream, JsTTDFlushAndCloseStreamCallback flushAndCloseStream, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime) {
      return ::JsTTDCreateRecordRuntime(attributes, false, snapInterval,
                                        snapHistoryLength, openResourceStream,
                                        writeBytesToStream, flushAndCloseStream,
                                        threadService, runtime);
    }
    static JsErrorCode JsTTDCreateReplayRuntime(JsRuntimeAttributes attributes, const char* infoUri, size_t infoUriCount, TTDOpenResourceStreamCallback openResourceStream, JsTTDReadBytesFromStreamCallback readBytesFromStream, JsTTDFlushAndCloseStreamCallback flushAndCloseStream, JsThreadServiceCallback threadService, JsRuntimeHandle *runtime) {
      return ::JsTTDCreateReplayRuntime(
          attributes, infoUri, infoUriCount, false, openResourceStream,
          readBytesFromStream, flushAndCloseStream, threadService, runtime);
    }
    static JsErrorCode JsTTDCreateContext(JsRuntimeHandle runtime, bool useRuntimeTTDMode, JsContextRef *newContext) {
      return ::JsTTDCreateContext(runtime, useRuntimeTTDMode, newContext);
    }
    static JsErrorCode JsTTDNotifyContextDestroy(JsContextRef context) {
      return ::JsTTDNotifyContextDestroy(context);
    }

    static JsErrorCode JsTTDStart() { return ::JsTTDStart(); }
    static JsErrorCode JsTTDStop() { return ::JsTTDStop(); }

    static JsErrorCode JsTTDNotifyYield() {
      return ::JsTTDNotifyYield();
    }
    static JsErrorCode JsTTDHostExit(int statusCode) {
      return ::JsTTDHostExit(statusCode);
    }

    static JsErrorCode JsTTDGetSnapTimeTopLevelEventMove(JsRuntimeHandle runtimeHandle, JsTTDMoveMode moveMode, uint32_t kthEvent, int64_t* targetEventTime, int64_t* targetStartSnapTime, int64_t* targetEndSnapTime) {
      return ::JsTTDGetSnapTimeTopLevelEventMove(
          runtimeHandle, moveMode, kthEvent, targetEventTime,
          targetStartSnapTime, targetEndSnapTime);
    }
    static JsErrorCode JsTTDMoveToTopLevelEvent(JsRuntimeHandle runtimeHandle, JsTTDMoveMode moveMode, int64_t snapshotStartTime, int64_t eventTime) {
      return ::JsTTDMoveToTopLevelEvent(runtimeHandle, moveMode,
                                        snapshotStartTime, eventTime);
    }
    static JsErrorCode JsTTDReplayExecution(JsTTDMoveMode* moveMode, int64_t* rootEventTime) {
      return ::JsTTDReplayExecution(moveMode, rootEventTime);
    }

    static JsErrorCode JsRun(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) {
      return ::JsRun(script, sourceContext, sourceUrl, parseAttributes, result);
    }
    static JsErrorCode JsParse(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) {
      return ::JsParse(script, sourceContext, sourceUrl, parseAttributes,
                       result);
    }
    static JsErrorCode JsSerialize(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) {
      return ::JsSerialize(script, buffer, parseAttributes);
    }
    static JsErrorCode JsRunSerialized(JsValueRef buffer, JsSerializedLoadScriptCallback scriptLoadCallback, JsSourceContext sourceContext, JsValueRef sourceUrl, JsValueRef * result) {
      return ::JsRunSerialized(buffer, scriptLoadCallback, sourceContext,
                               sourceUrl, result);
    }
    static JsErrorCode JsGetStringLength(JsValueRef value, int *stringLength) {
      return ::JsGetStringLength(value, stringLength);
    }
    static JsErrorCode JsCopyString(JsValueRef value, char* buffer, size_t bufferSize, size_t* length) {
      return ::JsCopyString(value, buffer, bufferSize, length);
    }
    static JsErrorCode JsCreateString(const char *content, size_t length, JsValueRef *value) {
      return ::JsCreateString(content, length, value);
    }
    static JsErrorCode JsCreateStringUtf16(const uint16_t *content, size_t length, JsValueRef *value) {
      return ::JsCreateStringUtf16(content, length, value);
    }
    static JsErrorCode JsCreatePropertyId(const char *name, size_t length, JsPropertyIdRef *propertyId) {
      return ::JsCreatePropertyId(name, length, propertyId);
    }
    static JsErrorCode JsCreateExternalArrayBuffer(void *data, unsigned int byteLength, JsFinalizeCallback finalizeCallback, void *callbackState, JsValueRef *result)  {
      return ::JsCreateExternalArrayBuffer(data, byteLength, finalizeCallback,
                                           callbackState, result);
    }
    static JsErrorCode JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  {
      return ::JsGetProxyProperties(object, isProxy, target, handler);
    }

    static JsErrorCode JsSerializeParserState(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) {
      return ::JsSerializeParserState(script, buffer, parseAttributes);
    }
    static JsErrorCode JsRunScriptWithParserState(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef * result) {
      return ::JsRunScriptWithParserState(script, sourceContext, sourceUrl,
                                          parseAttributes, parserState, result);
    }

    static JsErrorCode JsVarSerializer(ReallocateBufferMemoryFunc reallocateBufferMemory, WriteHostObjectFunc writeHostObject, void * callbackState, JsVarSerializerHandle *serializerHandle) {
      return ::JsVarSerializer(reallocateBufferMemory, writeHostObject,
                               callbackState, serializerHandle);
    }
    static JsErrorCode JsVarSerializerSetTransferableVars(JsVarSerializerHandle serializerHandle, JsValueRef *transferableVars, size_t transferableVarsCount) {
      return ::JsVarSerializerSetTransferableVars(
          serializerHandle, transferableVars, transferableVarsCount);
    }
    static JsErrorCode JsVarSerializerWriteValue(JsVarSerializerHandle serializerHandle, JsValueRef rootObject) {
      return ::JsVarSerializerWriteValue(serializerHandle, rootObject);
    }
    static JsErrorCode JsVarSerializerReleaseData(JsVarSerializerHandle serializerHandle, byte** data, size_t *dataLength) {
      return ::JsVarSerializerReleaseData(serializerHandle, data, dataLength);
    }
    static JsErrorCode JsVarSerializerFree(JsVarSerializerHandle serializerHandle) {
      return ::JsVarSerializerFree(serializerHandle);
    }

    static JsErrorCode JsVarDeserializer(void *data, size_t dataLength, ReadHostObjectFunc readHostObject, GetSharedArrayBufferFromIdFunc getSharedArrayBufferFromId, void* callbackState, JsVarDeserializerHandle *deserializerHandle) {
      return ::JsVarDeserializer(data, dataLength, readHostObject,
                                 getSharedArrayBufferFromId, callbackState,
                                 deserializerHandle);
    }
    static JsErrorCode JsVarDeserializerSetTransferableVars(JsVarDeserializerHandle deserializerHandle, JsValueRef* transferableVars, size_t transferableVarsCount) {
      return ::JsVarDeserializerSetTransferableVars(
          deserializerHandle, transferableVars, transferableVarsCount);
    }
    static JsErrorCode JsVarDeserializerReadValue(JsVarDeserializerHandle deserializerHandle, JsValueRef* value) {
      return ::JsVarDeserializerReadValue(deserializerHandle, value);
    }
    static JsErrorCode JsVarDeserializerFree(JsVarDeserializerHandle deserializerHandle) {
      return ::JsVarDeserializerFree(deserializerHandle);
    }

    static JsErrorCode JsDetachArrayBuffer(JsValueRef buffer) {
      return ::JsDetachArrayBuffer(buffer);
    }
    static JsErrorCode JsQueueBackgroundParse_Experimental(JsScriptContents* contents, uint32_t* dwBgParseCookie) {
      return ::JsQueueBackgroundParse_Experimental(contents, dwBgParseCookie);
    }
    static JsErrorCode JsDiscardBackgroundParse_Experimental(uint32_t dwBgParseCookie, void* buffer, bool* callerOwnsBuffer) {
      return ::JsDiscardBackgroundParse_Experimental(dwBgParseCookie, buffer,
                                                     callerOwnsBuffer);
    }
    static JsErrorCode JsExecuteBackgroundParse_Experimental(uint32_t dwBgParseCookie, JsValueRef script, JsSourceContext sourceContext, char16_t *url, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef *result) {
      return ::JsExecuteBackgroundParse_Experimental(
          dwBgParseCookie, script, sourceContext, url, parseAttributes,
          parserState, result);
    }

    static JsErrorCode JsGetArrayBufferFreeFunction(JsValueRef buffer, ArrayBufferFreeFn* freeFn) {
      return ::JsGetArrayBufferFreeFunction(buffer, freeFn);
    }
    static JsErrorCode JsExternalizeArrayBuffer(JsValueRef buffer) {
      return ::JsExternalizeArrayBuffer(buffer);
    }
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
