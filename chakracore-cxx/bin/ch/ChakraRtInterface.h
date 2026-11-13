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
    static JsErrorCode JsRelease(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsRelease(ref, count); }
    static JsErrorCode JsAddRef(JsRef ref, unsigned int* count) { return chakracore::jsrt::JsAddRef(ref, count); }
    static JsErrorCode JsGetProxyProperties(JsValueRef object, bool* isProxy, JsValueRef* target, JsValueRef* handler)  { return chakracore::jsrt::JsGetProxyProperties(object, isProxy, target, handler); }
};
