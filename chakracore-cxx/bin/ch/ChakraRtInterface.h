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

class ChakraRTInterface
{
public:
    static JsErrorCode JsCreateSharedArrayBufferWithSharedContent(JsSharedArrayBufferContentHandle sharedContent, JsValueRef *result) { return chakracore::jsrt::JsCreateSharedArrayBufferWithSharedContent(sharedContent, result); }
    static JsErrorCode JsGetSharedArrayBufferContent(JsValueRef sharedArrayBuffer, JsSharedArrayBufferContentHandle *sharedContents) { return chakracore::jsrt::JsGetSharedArrayBufferContent(sharedArrayBuffer, sharedContents); }
    static JsErrorCode JsReleaseSharedArrayBufferContentHandle(JsSharedArrayBufferContentHandle sharedContent) { return chakracore::jsrt::JsReleaseSharedArrayBufferContentHandle(sharedContent); }
    static JsErrorCode JsRelease(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsRelease(ref, count); }
    static JsErrorCode JsAddRef(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsAddRef(ref, count); }
    static JsErrorCode JsRun(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef *result) { return chakracore::jsrt::JsRun(script, sourceContext, sourceUrl, parseAttributes, result); }
    static JsErrorCode JsSerialize(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerialize(script, buffer, parseAttributes); }
    static JsErrorCode JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  { return chakracore::jsrt::JsGetProxyProperties(object, isProxy, target, handler); }

    static JsErrorCode JsSerializeParserState(JsValueRef script, JsValueRef *buffer, JsParseScriptAttributes parseAttributes) { return chakracore::jsrt::JsSerializeParserState(script, buffer, parseAttributes); }
    static JsErrorCode JsRunScriptWithParserState(JsValueRef script, JsSourceContext sourceContext, JsValueRef sourceUrl, JsParseScriptAttributes parseAttributes, JsValueRef parserState, JsValueRef * result) { return chakracore::jsrt::JsRunScriptWithParserState(script, sourceContext, sourceUrl, parseAttributes, parserState, result); }
};
