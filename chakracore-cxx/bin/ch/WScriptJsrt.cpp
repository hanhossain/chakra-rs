//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "WScriptJsrt.h"

#include <utility>

#include "ChakraRtInterface.h"
#include "chakra/Logger.h"
#include "chakracore-sys/src/filesystem.rs.h"
#include "chakracore-sys/src/wscript_jsrt.rs.h"

#pragma prefast(disable:26444, "This warning unfortunately raises false positives when auto is used for declaring the type of an iterator in a loop.")

JsValueRef WScriptJsrt::LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule, rust::String fileContent, rust::Str scriptInjectType, rust::String fileName, bool isFile, const chakra_rs::WScript &wscript)
{
    auto span = chakra::Span::create("WScriptJsrt::LoadScriptHelper");
    auto *fileContentPtr = new rust::String{std::move(fileContent)};

    // TODO: This is CESU-8. How to tell the engine?
    // TODO: How to handle this source (script) life time?
    return wscript.load_script(args.callee, fileName, chakra_rs::OptionalStr{.has_value = true, .value = *fileContentPtr}, scriptInjectType, isSourceModule, isFile);
}
