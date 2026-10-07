//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "WScriptJsrt.h"

#include <utility>
#include <print>

#include "chakracore-sys/src/filesystem.rs.h"

#include "ChakraRtInterface.h"
#include "Codex/Utf8Codex.h"
#include "RuntimeThreadData.h"
#include "chakracore-sys/src/wscript_jsrt.rs.h"

#pragma prefast(disable:26444, "This warning unfortunately raises false positives when auto is used for declaring the type of an iterator in a loop.")

JsValueRef WScriptJsrt::LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule, rust::String fileContent, rust::Str scriptInjectType, rust::String fileName, bool isFile)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    std::string errorMessage;
    JsValueRef returnValue = JS_INVALID_REFERENCE;

    {
        // HACK: call to c_str() somehow sets the underlying rust::Str up to be null-terminated. This prevents a segfault
        //  down the line when chakra clones the utf8 but attempts to copy the null terminator even if it's not
        //  null-terminated.
        auto *fileContentPtr = new rust::String{std::move(fileContent)};
        fileContentPtr->c_str();

        // TODO: This is CESU-8. How to tell the engine?
        // TODO: How to handle this source (script) life time?
        returnValue = chakra_rs::WScript::load_script(args.callee, fileName, chakra_rs::OptionalStr{.has_value = true, .value = *fileContentPtr}, scriptInjectType, isSourceModule, isFile);
    }
    return returnValue;
}
