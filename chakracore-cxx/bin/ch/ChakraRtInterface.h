//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <rust/cxx.h>

#include <utility>

#include "ChakraCommon.h"
#include "ChakraCore.h"

#include <chakracore-sys/src/jsrt/ffi.rs.h>
#include <chakracore-sys/src/str_helper.rs.h>

class ChakraRTInterface
{
public:
    static JsErrorCode JsSetCurrentContext(JsContextRef context) { return chakracore::jsrt::JsSetCurrentContext(context); }
    static JsErrorCode JsGetCurrentContext(JsContextRef* context) { return chakracore::jsrt::JsGetCurrentContext(context); }
    static JsErrorCode JsDisposeRuntime(JsRuntimeHandle runtime) { return chakracore::jsrt::JsDisposeRuntime(runtime); }
    static JsErrorCode JsGetNullValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetNullValue(globalObject); }
    static JsErrorCode JsGetTrueValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetTrueValue(globalObject); }
    static JsErrorCode JsGetFalseValue(JsValueRef *globalObject) { return chakracore::jsrt::JsGetFalseValue(globalObject); }
    static JsErrorCode JsConvertValueToString(JsValueRef value, JsValueRef *stringValue) { return chakracore::jsrt::JsConvertValueToString(value, stringValue); }
    static JsErrorCode JsCallFunction(JsValueRef function, JsValueRef* arguments, unsigned short argumentCount, JsValueRef *result) { return chakracore::jsrt::JsCallFunction(function, arguments, argumentCount, result); }
    static JsErrorCode JsCreateArrayBuffer(unsigned int byteLength, JsValueRef *result) { return chakracore::jsrt::JsCreateArrayBuffer(byteLength, result); }
    static JsErrorCode JsCreateSharedArrayBufferWithSharedContent(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result) { return chakracore::jsrt::JsCreateSharedArrayBufferWithSharedContent(sharedContent, result); }
    static JsErrorCode JsGetSharedArrayBufferContent(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents) { return chakracore::jsrt::JsGetSharedArrayBufferContent(sharedArrayBuffer, sharedContents); }
    static JsErrorCode JsReleaseSharedArrayBufferContentHandle(JsSharedArrayBufferContentHandle sharedContent) { return chakracore::jsrt::JsReleaseSharedArrayBufferContentHandle(sharedContent); }
    static JsErrorCode JsGetArrayBufferStorage(JsValueRef instance, uint8_t **buffer, unsigned int *bufferLength) { return chakracore::jsrt::JsGetArrayBufferStorage(instance, buffer, bufferLength); }
    static JsErrorCode JsHasException(bool *hasException) { return chakracore::jsrt::JsHasException(hasException); }
    static JsErrorCode JsGetAndClearException(JsValueRef *exception) { return chakracore::jsrt::JsGetAndClearException(exception); }
    static JsErrorCode JsGetAndClearExceptionWithMetadata(JsValueRef * metadata) { return chakracore::jsrt::JsGetAndClearExceptionWithMetadata(metadata); }
    static JsErrorCode JsGetRuntime(JsContextRef context, JsRuntimeHandle *runtime) { return chakracore::jsrt::JsGetRuntime(context, runtime); }
    static JsErrorCode JsRelease(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsRelease(ref, count); }
    static JsErrorCode JsAddRef(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsAddRef(ref, count); }
    static JsErrorCode JsGetContextOfObject(JsValueRef object, JsContextRef* context) { return chakracore::jsrt::JsGetContextOfObject(object, context); }
    static JsErrorCode JsParseModuleSource(const JsModuleRecord &requestModule, const JsSourceContext &sourceContext,
                                           chakra_rs::OptionalStr sourceText, JsValueRef *exceptionValueRef) {
        return chakracore::jsrt::JsParseModuleSource(
            requestModule, sourceContext,
            sourceText.has_value ? reinterpret_cast<const uint8_t *>(sourceText.value.data()) : nullptr,
            sourceText.value.size(), JsParseModuleSourceFlags::JsParseModuleSourceFlags_DataIsUTF8, exceptionValueRef);
    }
    static JsErrorCode JsInitializeModuleRecord(JsModuleRecord referencingModule, JsValueRef normalizedSpecifier, JsModuleRecord* moduleRecord) {
        return chakracore::jsrt::JsInitializeModuleRecord(referencingModule, normalizedSpecifier, moduleRecord);
    }
    static JsErrorCode JsSetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void* hostInfo) { return chakracore::jsrt::JsSetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }
    static JsErrorCode JsGetModuleHostInfo(JsModuleRecord requestModule, JsModuleHostInfoKind moduleHostInfo, void** hostInfo) { return chakracore::jsrt::JsGetModuleHostInfo(requestModule, moduleHostInfo, hostInfo); }

    static JsErrorCode JsRun(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsRun(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode JsSerialize(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerialize(script, buffer, parseAttributes); }
    static JsErrorCode JsRunSerialized(
        JsValueRef buffer,
        rust::Fn<bool(JsSourceContext sourceContext, JsValueRef *value, JsParseScriptAttributes *parseAttributes)>
            scriptLoadCallback,
        JsSourceContext sourceContext, JsValueRef sourceUrl, JsValueRef *result)
    {
        return chakracore::jsrt::JsRunSerialized(buffer, scriptLoadCallback, sourceContext, sourceUrl, result);
    }
    static JsErrorCode JsCreateExternalArrayBuffer(const rust::Str content, JsValueRef *result)  { return chakracore::jsrt::JsCreateExternalArrayBuffer(const_cast<char *>(content.data()), content.length(), nullptr, const_cast<char *>(content.data()), result); }
    static JsErrorCode JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  { return chakracore::jsrt::JsGetProxyProperties(object, isProxy, target, handler); }

    static JsErrorCode JsSerializeParserState(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerializeParserState(script, buffer, parseAttributes); }
    static JsErrorCode JsRunScriptWithParserState(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef * result) { return chakracore::jsrt::JsRunScriptWithParserState(script, sourceContext, sourceUrl, parseAttributes, parserState, result); }
};
