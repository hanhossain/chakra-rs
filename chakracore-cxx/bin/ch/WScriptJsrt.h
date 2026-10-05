//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <list>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <rust/cxx.h>

#include "ChakraCore.h"
#include "MessageQueue.h"
#include "RuntimeThreadData.h"
#include <chakracore-sys/src/jsrt/ffi.rs.h>
#include <chakracore-sys/src/str_helper.rs.h>

enum ModuleState
{
    RootModule,
    ImportedModule,
    ErroredModule
};

class WScriptJsrt
{
public:
    static bool Uninitialize();

    class CallbackMessage : public MessageBase
    {
        JsValueRef m_function;

        CallbackMessage(CallbackMessage const&);

    public:
        CallbackMessage(unsigned int time, JsValueRef function);
        ~CallbackMessage();

        int32_t Call(rust::Str fileName);
        int32_t CallFunction(rust::Str fileName);
        static std::unique_ptr<CallbackMessage> New(unsigned int time, JsValueRef function)
        {
            return std::make_unique<CallbackMessage>(time, function);
        }
        static std::unique_ptr<MessageBase> Upcast(std::unique_ptr<CallbackMessage> msg)
        {
            return msg;
        }
    };

    class ModuleMessage : public MessageBase
    {
    private:
        JsModuleRecord moduleRecord;
        JsValueRef specifier;
        std::optional<std::filesystem::path> fullPath_;

    public:
        ModuleMessage(JsModuleRecord module, JsValueRef specifier, const std::optional<std::filesystem::path> &fullpath);
        ~ModuleMessage();

        int32_t Call(rust::Str fileName) override;

        static ModuleMessage* Create(JsModuleRecord module, JsValueRef specifier, const std::optional<std::filesystem::path> &fullPath)
        {
            return new ModuleMessage(module, specifier, fullPath);
        }

        static std::unique_ptr<ModuleMessage> New(JsModuleRecord module, JsValueRef specifier)
        {
            return std::make_unique<ModuleMessage>(module, specifier, std::nullopt);
        }
        static std::unique_ptr<ModuleMessage> NewWithPath(JsModuleRecord module, JsValueRef specifier, const rust::Str fullPath)
        {
            return std::make_unique<ModuleMessage>(module, specifier, static_cast<std::string_view>(fullPath));
        }
        static std::unique_ptr<MessageBase> Upcast(std::unique_ptr<ModuleMessage> msg)
        {
            return msg;
        }
    };

    static void AddMessageQueue(MessageQueue *messageQueue);
    static void PushMessage(MessageBase *message) { messageQueue_->InsertSorted(message); }
    static MessageQueue *GetMessageQueue() { return messageQueue_; }

    static bool PrintException(rust::Str fileName, JsValueRef exception, rust::Str errorMessage);
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
