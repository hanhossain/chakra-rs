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

enum ModuleState
{
    RootModule,
    ImportedModule,
    ErroredModule
};

class WScriptJsrt
{
public:
    class CustomMessage : public MessageBase {
        rust::Box<chakra_rs::Message> message_;
    public:
        explicit CustomMessage(rust::Box<chakra_rs::Message> message);
        void BeginTimer() override;
        unsigned int GetTime() const override;
        unsigned int GetId() const override;
        int32_t Call(rust::Str fileName) override;
    };

    class CallbackMessage final : public CustomMessage
    {
    public:
        CallbackMessage(unsigned int time, JsValueRef function);
        CallbackMessage(CallbackMessage const&) = delete;

        static std::unique_ptr<CallbackMessage> New(unsigned int time, JsValueRef function)
        {
            return std::make_unique<CallbackMessage>(time, function);
        }
        static std::unique_ptr<MessageBase> Upcast(std::unique_ptr<CallbackMessage> msg)
        {
            return msg;
        }
    };

    class ModuleMessage final : public CustomMessage
    {
    public:
        ModuleMessage(JsModuleRecord module, JsValueRef specifier, chakra_rs::OptionalStr fullpath);
        ModuleMessage(ModuleMessage const&) = delete;

        static std::unique_ptr<ModuleMessage> New(JsModuleRecord module, JsValueRef specifier)
        {
            return std::make_unique<ModuleMessage>(module, specifier, chakra_rs::OptionalStr{});
        }
        static std::unique_ptr<ModuleMessage> NewWithPath(JsModuleRecord module, JsValueRef specifier, const rust::Str fullPath)
        {
            return std::make_unique<ModuleMessage>(module, specifier, chakra_rs::OptionalStr{.has_value = true, .value = fullPath});
        }
        static std::unique_ptr<MessageBase> Upcast(std::unique_ptr<ModuleMessage> msg)
        {
            return msg;
        }
    };

    static void AddMessageQueue(MessageQueue *messageQueue);
    static void PushMessage(MessageBase *message) { messageQueue_->InsertSorted(message); }
    static MessageQueue *GetMessageQueue() { return messageQueue_; }

    static std::size_t GetNextSourceContext();
    static std::size_t GetSourceContext();
    static JsValueRef LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule, rust::String fileContent, rust::Str scriptInjectType, rust::String fileName, bool isFile);
private:
    static MessageQueue *messageQueue_;
    static std::size_t sourceContext_;
};

// type aliases for rust ffi
using WScriptJsrt_CallbackMessage = WScriptJsrt::CallbackMessage;
using WScriptJsrt_ModuleMessage = WScriptJsrt::ModuleMessage;
