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

unsigned int MessageBase::s_messageCount = 0;
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
    : MessageBase(time), callback_message_(chakra_rs::CallbackMessage::boxed_new(function)) {}

int32_t WScriptJsrt::CallbackMessage::Call(rust::Str fileName) {
    callback_message_->call(fileName);
    return S_OK;
}

WScriptJsrt::ModuleMessage::ModuleMessage(JsModuleRecord module, JsValueRef specifier, chakra_rs::OptionalStr fullpath)
    : MessageBase(0), module_message_(chakra_rs::ModuleMessage::boxed_new(module, specifier, fullpath))
{
}

WScriptJsrt::ModuleMessage::~ModuleMessage()
{
    ChakraRTInterface::JsRelease(module_message_->get_module_record(), nullptr);
    if (module_message_->get_specifier() != nullptr)
    {
        ChakraRTInterface::JsRelease(module_message_->get_specifier(), nullptr);
    }
}

int32_t WScriptJsrt::ModuleMessage::Call(rust::Str fileName)
{
    JsErrorCode errorCode = JsNoError;
    if (module_message_->get_specifier() == nullptr)
    {
        if (auto [exists, state] = chakra_rs::get_module_error_map()->get(module_message_->get_module_record());
            exists && state != ErroredModule)
        {
            JsValueRef result = JS_INVALID_REFERENCE;
            errorCode = ChakraRTInterface::JsModuleEvaluation(module_message_->get_module_record(), &result);
            if (errorCode != JsNoError)
            {
                chakra_rs::WScript::print_exception(fileName, errorCode, nullptr); // this should not be called
            }
        }
    }
    else
    {
        rust::String specifierStr;
        errorCode = ChakraRTInterface::JsToString(module_message_->get_specifier(), specifierStr);
        if (errorCode != JsNoError)
        {
            return errorCode;
        }

        try
        {
            rust::String fileContent = module_message_->has_full_path()
                ? chakra_rs::helpers::ScriptCache::get_script_with_full_path(specifierStr, module_message_->get_full_path())
                : chakra_rs::helpers::ScriptCache::get_script(specifierStr);
            chakra_rs::WScript::load_module_from_string(chakra_rs::OptionalStr{.has_value = true, .value = fileContent}, module_message_->has_full_path() ? module_message_->get_full_path() : specifierStr, true);
        }
        catch (const rust::Error &e)
        {
            chakra::Logger::error(std::format("Caught exception: {}", e.what()));
            if (!HostConfigFlags::GetConfig().host.mute_host_error_msg)
            {
                auto actualModuleRecord = chakra_rs::get_module_record_map()->get(module_message_->get_full_path());
                auto error_map_content = chakra_rs::get_module_error_map()->get(actualModuleRecord.content.record);
                if (!actualModuleRecord.exists || (error_map_content.exists && error_map_content.content == RootModule))
                {
                    chakra::Logger::error(std::format("Couldn't load file '{}'", specifierStr));
                }
            }
            chakra_rs::WScript::load_module_from_string(chakra_rs::OptionalStr{}, module_message_->has_full_path() ? module_message_->get_full_path() : specifierStr, false);
        }
    }
    return errorCode;
}
