//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <rust/cxx.h>

#include "ChakraCore.h"
#include "MessageQueue.h"
#include <chakracore-sys/src/jsrt/ffi.rs.h>
#include "chakracore-sys/src/messages.rs.h"

class WScriptJsrt
{
public:
    static void AddMessageQueue(MessageQueue *messageQueue);
    static void PushMessage(rust::Box<chakra_rs::Message> message) { messageQueue_->InsertSorted(std::move(message)); }
    static MessageQueue *GetMessageQueue() { return messageQueue_; }

    static std::size_t GetNextSourceContext();
    static std::size_t GetSourceContext();
    static JsValueRef LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule, rust::String fileContent, rust::Str scriptInjectType, rust::String fileName, bool isFile);
private:
    static MessageQueue *messageQueue_;
    static std::size_t sourceContext_;
};
