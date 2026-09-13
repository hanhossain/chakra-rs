//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <list>
#include <filesystem>
#include <map>
#include <optional>
#include <rust/cxx.h>

#include "ChakraCore.h"
#include "MessageQueue.h"
#include "chakracore-sys/src/jsrt.rs.h"

enum ModuleState
{
    RootModule,
    ImportedModule,
    ErroredModule
};

class WScriptJsrt
{
public:
    static bool Initialize(int icuVersion, JsValueRef wscript);
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
        template <class Func>
        static CallbackMessage* Create(JsValueRef function, const Func& func, unsigned int time = 0)
        {
            return new CustomMessage<Func, CallbackMessage>(time, function, func);
        }
    };

    class ModuleMessage : public MessageBase
    {
    private:
        JsModuleRecord moduleRecord;
        JsValueRef specifier;
        std::optional<std::filesystem::path> fullPath_;

        ModuleMessage(JsModuleRecord module, JsValueRef specifier, const std::optional<std::filesystem::path> &fullpath);

    public:
        ~ModuleMessage();

        int32_t Call(rust::Str fileName) override;

        static ModuleMessage* Create(JsModuleRecord module, JsValueRef specifier, const std::optional<std::filesystem::path> &fullPath = std::nullopt)
        {
            return new ModuleMessage(module, specifier, fullPath);
        }

    };

    static void AddMessageQueue(MessageQueue *messageQueue);
    static void PushMessage(MessageBase *message) { messageQueue_->InsertSorted(message); }

    static JsErrorCode FetchImportedModule(_In_ JsModuleRecord referencingModule, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord);
    static JsErrorCode FetchImportedModuleFromScript(_In_ unsigned long dwReferencingSourceContext, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord);
    static JsErrorCode NotifyModuleReadyCallback(_In_opt_ JsModuleRecord referencingModule, _In_opt_ JsValueRef exceptionVar);
    static JsErrorCode ReportModuleCompletionCallback(JsModuleRecord module, JsValueRef exception);
    static JsErrorCode CALLBACK InitializeImportMetaCallback(_In_opt_ JsModuleRecord referencingModule, _In_opt_ JsValueRef importMetaVar);
    static void CALLBACK PromiseContinuationCallback(JsValueRef task, void *callbackState);

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
    static JsValueRef LoadScriptFileHelper(JsValueRef callee, const std::vector<JsValueRef> &arguments, bool isSourceModule);
    static JsValueRef LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule);
    static bool InstallObjectsOnObject(JsValueRef object, const char* name, std::function<JsValueRef(const chakra_rs::JsNativeFunctionArgs &)> nativeFunction);
    static void FinalizeFree(void * addr);
private:
    static void SetExceptionIf(JsErrorCode errorCode, std::string_view errorMessage);
    static bool CreateArgumentsObject(JsValueRef *argsObject);
    static bool CreateNamedFunction(const char*, std::function<JsValueRef(const chakra_rs::JsNativeFunctionArgs &)>callback, JsValueRef *functionVar);
    static std::string GetDir(std::string_view fullPathNarrow);
    static JsValueRef CALLBACK EchoCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK QuitCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK LoadScriptFileCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK LoadScriptCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK LoadModuleCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK GetModuleNamespace(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK MonotonicNowCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK SetTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ClearTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK AttachCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK DetachCallback(const chakra_rs::JsNativeFunctionArgs &args);

    static JsErrorCode CALLBACK LoadModuleFromString(const std::optional<rust::Str> &fileContent, const std::string &fullName, bool isFile = false);

    static JsValueRef CALLBACK LoadBinaryFileCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK LoadTextFileCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK RegisterModuleSourceCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK FlagCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ReadLineStdinCallback(const chakra_rs::JsNativeFunctionArgs &args);

    static JsValueRef CALLBACK BroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ReceiveBroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK ReportCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK GetReportCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK LeavingCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK SleepCallback(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK GetProxyPropertiesCallback(const chakra_rs::JsNativeFunctionArgs &args);

    static JsValueRef CALLBACK SerializeObject(const chakra_rs::JsNativeFunctionArgs &args);
    static JsValueRef CALLBACK Deserialize(const chakra_rs::JsNativeFunctionArgs &args);

    static JsErrorCode FetchImportedModuleHelper(JsModuleRecord referencingModule, JsValueRef specifier,
                                                 JsModuleRecord* dependentModuleRecord,
                                                 const std::optional<std::filesystem::path>& refdir = std::nullopt);

    static MessageQueue *messageQueue_;
    static std::size_t sourceContext_;
    static std::map<std::filesystem::path, JsModuleRecord> moduleRecordMap;
    static std::map<JsModuleRecord, std::filesystem::path> moduleDirMap;
    static std::map<JsModuleRecord, ModuleState> moduleErrMap;
};
