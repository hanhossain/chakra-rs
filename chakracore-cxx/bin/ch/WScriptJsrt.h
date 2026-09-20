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
#include "chakracore-sys/src/jsrt/ffi.rs.h"

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
    static JsErrorCode ModuleEntryPoint(rust::Str fileContent, const rust::String &fullName);

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
        static std::unique_ptr<MessageBase> Upcast(std::unique_ptr<ModuleMessage> msg)
        {
            return msg;
        }
    };

    static void AddMessageQueue(MessageQueue *messageQueue);
    static void PushMessage(MessageBase *message) { messageQueue_->InsertSorted(message); }

    static JsErrorCode FetchImportedModule(_In_ JsModuleRecord referencingModule, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord);
    static JsErrorCode FetchImportedModuleFromScript(_In_ JsSourceContext dwReferencingSourceContext, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord);

    static const char * ConvertErrorCodeToMessage(JsErrorCode errorCode)
    {
        switch (errorCode)
        {
        case (JsErrorCode::JsErrorInvalidArgument) :
            return "TypeError: InvalidArgument";
        case (JsErrorCode::JsErrorNullArgument) :
            return "TypeError: NullArgument";
        case (JsErrorCode::JsErrorArgumentNotObject) :
            return "TypeError: ArgumentNotAnObject";
        case (JsErrorCode::JsErrorOutOfMemory) :
            return "OutOfMemory";
        case (JsErrorCode::JsErrorScriptException) :
            return "ScriptError";
        case (JsErrorCode::JsErrorScriptCompile) :
            return "SyntaxError";
        case (JsErrorCode::JsErrorFatal) :
            return "FatalError";
        case (JsErrorCode::JsErrorInExceptionState) :
            return "ErrorInExceptionState";
        case (JsErrorCode::JsErrorBadSerializedScript):
            return "ErrorBadSerializedScript ";
        default:
            assert(false && "Unexpected JsErrorCode");
            return nullptr;
        }
    }

    static bool PrintException(rust::Str fileName, JsErrorCode jsErrorCode, JsValueRef exception = nullptr);
    static JsValueRef LoadScript(JsValueRef callee, rust::Str fileName, const std::optional<rust::Str> &content, rust::Str scriptInjectType, bool isSourceModule, JsFinalizeCallback finalizeCallback, bool isFile);
    static std::size_t GetNextSourceContext();
    static JsValueRef LoadScriptFileHelper(JsValueRef callee, rust::Slice<JsValueRef const> arguments, bool isSourceModule);
    static JsValueRef LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule);
    static void FinalizeFree(void * addr);
private:
    static void SetExceptionIf(JsErrorCode errorCode, std::string_view errorMessage);
public:
    static bool GetModuleRecord(rust::Str path, JsModuleRecord *record);
    static JsValueRef CALLBACK SetTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ClearTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args);

    static JsErrorCode CALLBACK LoadModuleFromString(const std::optional<rust::Str> &fileContent, const std::string &fullName, bool isFile = false);

    static JsValueRef CALLBACK LoadBinaryFileCallback(const chakra_rs::JsNativeFunctionArgs &args);

    static JsValueRef CALLBACK BroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ReceiveBroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ReportCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK GetReportCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK GetProxyPropertiesCallback(const chakra_rs::JsNativeFunctionArgs &args);

private:
    static JsErrorCode FetchImportedModuleHelper(JsModuleRecord referencingModule, JsValueRef specifier,
                                                 JsModuleRecord* dependentModuleRecord,
                                                 const std::optional<std::filesystem::path>& refdir = std::nullopt);

    static MessageQueue *messageQueue_;
    static std::size_t sourceContext_;
    static std::map<std::filesystem::path, JsModuleRecord> moduleRecordMap;
    static std::map<JsModuleRecord, std::filesystem::path> moduleDirMap;
};

// type aliases for rust ffi
using WScriptJsrt_CallbackMessage = WScriptJsrt::CallbackMessage;
using WScriptJsrt_ModuleMessage = WScriptJsrt::ModuleMessage;
