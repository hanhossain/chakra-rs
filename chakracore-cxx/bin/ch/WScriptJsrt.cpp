//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "WScriptJsrt.h"

#include <utility>
#include <print>

#include <filesystem>
#include <chakracore-sys/src/filesystem.rs.h>

#include "ChakraRtInterface.h"
#include "Codex/Utf8Codex.h"
#include "HostConfigFlags.h"
#include "RuntimeThreadData.h"
#include "chakra/Logger.h"

#include <chakracore-sys/src/helpers.rs.h>
#include <chakracore-sys/src/wscript_jsrt.rs.h>

namespace fs = std::filesystem;

#pragma prefast(disable:26444, "This warning unfortunately raises false positives when auto is used for declaring the type of an iterator in a loop.")

MessageQueue* WScriptJsrt::messageQueue_ = nullptr;
std::size_t WScriptJsrt::sourceContext_ = 0;

std::size_t WScriptJsrt::GetNextSourceContext()
{
    return sourceContext_++;
}

std::size_t WScriptJsrt::GetSourceContext() {
    return sourceContext_;
}

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

void WScriptJsrt::AddMessageQueue(MessageQueue *_messageQueue)
{
    assert(messageQueue_ == nullptr);

    messageQueue_ = _messageQueue;
}

WScriptJsrt::CallbackMessage::CallbackMessage(unsigned int time, JsValueRef function)
    : callback_message_(chakra_rs::CallbackMessage::boxed_new(time, function)) {}

int32_t WScriptJsrt::CallbackMessage::Call(rust::Str fileName) {
    callback_message_->call(fileName);
    return S_OK;
}

void WScriptJsrt::CallbackMessage::BeginTimer() { callback_message_->begin_timer(); }
unsigned int WScriptJsrt::CallbackMessage::GetTime() const { return callback_message_->get_time(); }
unsigned int WScriptJsrt::CallbackMessage::GetId() const { return callback_message_->get_id(); }

WScriptJsrt::ModuleMessage::ModuleMessage(JsModuleRecord module, JsValueRef specifier, chakra_rs::OptionalStr fullpath)
    : module_message_(chakra_rs::ModuleMessage::boxed_new(module, specifier, fullpath))
{
}

int32_t WScriptJsrt::ModuleMessage::Call(rust::Str fileName)
{
    module_message_->call(fileName);
    return JsNoError;
}
void WScriptJsrt::ModuleMessage::BeginTimer() { module_message_->begin_timer(); }
unsigned int WScriptJsrt::ModuleMessage::GetTime() const { return module_message_->get_time(); }
unsigned int WScriptJsrt::ModuleMessage::GetId() const { return module_message_->get_id(); }

WScriptJsrt::CustomMessage::CustomMessage(rust::Box<chakra_rs::Message> message) : message_(std::move(message)) {}
void WScriptJsrt::CustomMessage::BeginTimer() { message_->begin_timer(); }
unsigned int WScriptJsrt::CustomMessage::GetTime() const { return message_->get_time(); }
unsigned int WScriptJsrt::CustomMessage::GetId() const { return message_->get_id(); }
int32_t WScriptJsrt::CustomMessage::Call(rust::Str fileName) {
    message_->call(fileName);
    return JsNoError;
}
