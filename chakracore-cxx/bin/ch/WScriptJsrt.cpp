//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "WScriptJsrt.h"

#include <vector>
#include <ctime>
#include <ratio>
#include <print>

#include <filesystem>
#include <iostream>
#include <chakracore-sys/src/filesystem.rs.h>

#include "ChakraRtInterface.h"
#include "Codex/Utf8Codex.h"
#include "Helpers.h"
#include "HostConfigFlags.h"
#include "RuntimeThreadData.h"
#include "TestHooks.h"
#include "chakra/Logger.h"

#include <chakracore-sys/src/helpers.rs.h>
#include <chakracore-sys/src/wscript_jsrt.rs.h>

namespace fs = std::filesystem;

#define IfJsrtErrorFail(expr, ret) do { if ((expr) != JsNoError) return ret; } while (0)
#define IfJsrtErrorHR(expr) do { if((expr) != JsNoError) { hr = E_FAIL; goto Error; } } while(0)
#define IfJsrtErrorSetGo(expr) do { errorCode = (expr); if(errorCode != JsNoError) { hr = E_FAIL; goto Error; } } while(0)

#pragma prefast(disable:26444, "This warning unfortunately raises false positives when auto is used for declaring the type of an iterator in a loop.")

unsigned int MessageBase::s_messageCount = 0;
MessageQueue* WScriptJsrt::messageQueue_ = nullptr;
std::size_t WScriptJsrt::sourceContext_ = 0;

std::size_t WScriptJsrt::GetNextSourceContext()
{
    return sourceContext_++;
}

void WScriptJsrt::SetExceptionIf(JsErrorCode errorCode, const std::string_view errorMessage)
{
    if (errorCode == JsNoError)
    {
        return;
    }

    // If the exception is already is set - no need to create a new exception.
    bool hasException = false;
    if (ChakraRTInterface::JsHasException(&hasException) != JsNoError || !hasException)
    {
        JsValueRef errorObject;
        JsValueRef errorMessageString;

        std::string errorMessageStr = errorMessage.empty() ? ConvertErrorCodeToMessage(errorCode) : std::string{errorMessage};
        errorCode = ChakraRTInterface::JsCreateString(errorMessageStr, &errorMessageString);

        ChakraRTInterface::JsCreateError(errorMessageString, &errorObject);
        ChakraRTInterface::JsSetException(errorObject);
    }
}

JsValueRef WScriptJsrt::LoadScriptHelper(const chakra_rs::JsNativeFunctionArgs &args, bool isSourceModule)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsErrorCode errorCode = JsNoError;
    std::string errorMessage;
    JsValueRef returnValue = JS_INVALID_REFERENCE;

    if (args.arguments.size() < 2 || args.arguments.size() > 4)
    {
        errorCode = JsErrorInvalidArgument;
        errorMessage = "Need more or fewer arguments for WScript.LoadScript";
    }
    else
    {
        auto *fileContent = new rust::String{};
        rust::String fileName;
        std::optional<rust::String> scriptInjectType;
        bool isFile = true;

        IfJsrtErrorSetGo(ChakraRTInterface::JsToString(args.arguments[1], *fileContent));
        // ExternalArrayBuffer Finalize will clean this up
        // but only if we actually register a finalizecallback for this

        if (args.arguments.size() > 2)
        {
            rust::String injectType;
            IfJsrtErrorSetGo(ChakraRTInterface::JsToString(args.arguments[2], injectType));
            scriptInjectType = injectType;

            if (args.arguments.size() > 3)
            {
                IfJsrtErrorSetGo(ChakraRTInterface::JsToString(args.arguments[3], fileName));
            }
        }

        if (fileName.empty())
        {
            isFile = false;
            if (isSourceModule)
            {
                fileName = std::format("moduleScript{}.js", static_cast<int>(sourceContext_));
            }
        }

        // HACK: call to c_str() somehow sets the underlying rust::Str up to be null-terminated. This prevents a segfault
        //  down the line when chakra clones the utf8 but attempts to copy the null terminator even if it's not
        //  null-terminated.
        fileContent->c_str();

        // TODO: This is CESU-8. How to tell the engine?
        // TODO: How to handle this source (script) life time?
        returnValue = chakra_rs::WScript::load_script(args.callee, fileName, chakra_rs::OptionalStr{.has_value = true, .value = *fileContent}, scriptInjectType ? scriptInjectType.value() : "self", isSourceModule, isFile);
    }

Error:
    SetExceptionIf(errorCode, errorMessage);
    return returnValue;
}

bool WScriptJsrt::Uninitialize()
{
    auto& threadData = GetRuntimeThreadLocalData().threadData;
    if (threadData && !threadData->children.empty())
    {
        const size_t count = threadData->children.size();
        std::vector<HANDLE> childrenHandles;

        for (const auto child : threadData->children)
        {
            childrenHandles.push_back(child->hThread);
            SetEvent(child->hevntShutdown);
        }

        [[maybe_unused]] uint32_t waitRet = WaitForMultipleObjects(count, &childrenHandles[0], TRUE, INFINITE);
        assert(waitRet == WAIT_OBJECT_0);

        for (const auto &i : threadData->children)
        {
            delete i;
        }

        threadData->children.clear();
    }

    return true;
}

bool WScriptJsrt::PrintException(rust::Str fileName, JsErrorCode jsErrorCode, JsValueRef exception)
{
    const char* errorTypeString = ConvertErrorCodeToMessage(jsErrorCode);
    JsValueRef metaData = JS_INVALID_REFERENCE;

    if (exception == nullptr)
    {
        if (ChakraRTInterface::JsGetAndClearExceptionWithMetadata(&metaData) == JsNoError)
        {
            JsPropertyIdRef exceptionId = JS_INVALID_REFERENCE;
            IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("exception", &exceptionId), false);
            IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(metaData, exceptionId, &exception), false);
        }
        else
        {
            IfJsrtErrorFail(ChakraRTInterface::JsGetAndClearException(&exception), false);
        }

    }

    if (HostConfigFlags::GetConfig().host.mute_host_error_msg)
    {
        return false;
    }

    if (exception != nullptr)
    {
        if (jsErrorCode == JsErrorCode::JsErrorScriptCompile || jsErrorCode == JsErrorCode::JsErrorScriptException)
        {
            rust::String errorMessage;
            const std::filesystem::path path{static_cast<std::string_view>(fileName)};

            if (ChakraRTInterface::JsToString(exception, errorMessage) != JsNoError)
            {
                std::println("ERROR attempting to coerce error to string, using alternate handler");
                bool hasException = false;
                ChakraRTInterface::JsHasException(&hasException);
                if (hasException)
                {
                    JsValueRef throwAway = JS_INVALID_REFERENCE;
                    ChakraRTInterface::JsGetAndClearException(&throwAway);
                }
                JsPropertyIdRef messagePropertyId = JS_INVALID_REFERENCE;
                IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("message", &messagePropertyId), false);
                JsValueRef message = JS_INVALID_REFERENCE;
                IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(exception, messagePropertyId, &message), false);
                IfJsrtErrorFail(ChakraRTInterface::JsToString(message, errorMessage), false);

                if (jsErrorCode != JsErrorCode::JsErrorScriptCompile)
                {
                    if (metaData != JS_INVALID_REFERENCE)
                    {
                        JsPropertyIdRef linePropertyId = JS_INVALID_REFERENCE;
                        JsValueRef lineProperty = JS_INVALID_REFERENCE;

                        JsPropertyIdRef columnPropertyId = JS_INVALID_REFERENCE;
                        JsValueRef columnProperty = JS_INVALID_REFERENCE;

                        int line;
                        int column;

                        IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("line", &linePropertyId), false);
                        IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(metaData, linePropertyId, &lineProperty), false);
                        IfJsrtErrorFail(ChakraRTInterface::JsNumberToInt(lineProperty, &line), false);

                        IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("column", &columnPropertyId), false);
                        IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(metaData, columnPropertyId, &columnProperty), false);
                        IfJsrtErrorFail(ChakraRTInterface::JsNumberToInt(columnProperty, &column), false);
                        std::println("{}\n        at code ({}:{}:{})",
                            errorMessage, path.filename().string(), line + 1, column + 1);
                    }
                    else
                    {
                        std::println("{}\n\tat code ({}:\?\?:\?\?)", errorMessage, path.filename().string());
                    }
                    return true;
                }
            }

            if (jsErrorCode == JsErrorCode::JsErrorScriptCompile)
            {
                JsPropertyIdRef linePropertyId = JS_INVALID_REFERENCE;
                JsValueRef lineProperty = JS_INVALID_REFERENCE;

                JsPropertyIdRef columnPropertyId = JS_INVALID_REFERENCE;
                JsValueRef columnProperty = JS_INVALID_REFERENCE;

                int line;
                int column;

                IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("line", &linePropertyId), false);
                IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(exception, linePropertyId, &lineProperty), false);
                IfJsrtErrorFail(ChakraRTInterface::JsNumberToInt(lineProperty, &line), false);

                IfJsrtErrorFail(ChakraRTInterface::JsCreatePropertyId("column", &columnPropertyId), false);
                IfJsrtErrorFail(ChakraRTInterface::JsGetProperty(exception, columnPropertyId, &columnProperty), false);
                IfJsrtErrorFail(ChakraRTInterface::JsNumberToInt(columnProperty, &column), false);

                std::println("{}\n\tat code ({}:{}:{})",
                    errorMessage, path.filename().string(), (int)line + 1,
                    (int)column + 1);
            }
            else
            {
                JsValueType propertyType = JsUndefined;
                JsPropertyIdRef stackPropertyId = JS_INVALID_REFERENCE;
                JsValueRef stackProperty = JS_INVALID_REFERENCE;

                JsErrorCode errorCode = ChakraRTInterface::JsCreatePropertyId("stack", &stackPropertyId);

                if (errorCode == JsErrorCode::JsNoError)
                {
                    errorCode = ChakraRTInterface::JsGetProperty(exception, stackPropertyId, &stackProperty);
                    if (errorCode == JsErrorCode::JsNoError)
                    {
                        errorCode = ChakraRTInterface::JsGetValueType(stackProperty, &propertyType);
                    }
                }

                if (errorCode != JsErrorCode::JsNoError || propertyType == JsUndefined)
                {
                    std::filesystem::path filepath{static_cast<std::string_view>(fileName)};

                    // do not mix char/wchar. print them separately
                    std::println("thrown at {}:\n^", filepath.filename().string());
                    std::println("{}", errorMessage);
                }
                else
                {
                    rust::String errorStack;
                    IfJsrtErrorFail(ChakraRTInterface::JsToString(stackProperty, errorStack), false);
                    std::println("{}", errorStack);
                }
            }
        }
        else
        {
            chakra::Logger::error(std::format("Error : {}", errorTypeString));
        }
        return true;
    }
    else
    {
        chakra::Logger::error(std::format("Error : {}", errorTypeString));
    }
    return false;
}

void WScriptJsrt::AddMessageQueue(MessageQueue *_messageQueue)
{
    assert(messageQueue_ == nullptr);

    messageQueue_ = _messageQueue;
}

WScriptJsrt::CallbackMessage::CallbackMessage(unsigned int time, JsValueRef function) : MessageBase(time), m_function(function)
{
    JsErrorCode error = ChakraRTInterface::JsAddRef(m_function, nullptr);
    if (error != JsNoError)
    {
        // Simply report a fatal error and exit because continuing from this point would result in inconsistent state
        // and FailFast telemetry would not be useful.
        std::println("FATAL ERROR: ChakraRTInterface::JsAddRef failed in WScriptJsrt::CallbackMessage::`ctor`. error=0x{:x}", static_cast<int>(error));
        exit(1);
    }
}

WScriptJsrt::CallbackMessage::~CallbackMessage()
{
    bool hasException = false;
    ChakraRTInterface::JsHasException(&hasException);
    if (hasException)
    {
        WScriptJsrt::PrintException("", JsErrorScriptException);
    }
    [[maybe_unused]] JsErrorCode errorCode = ChakraRTInterface::JsRelease(m_function, nullptr);
    assert(errorCode == JsNoError);
    m_function = JS_INVALID_REFERENCE;
}

int32_t WScriptJsrt::CallbackMessage::Call(rust::Str fileName)
{
    return CallFunction(fileName);
}

int32_t WScriptJsrt::CallbackMessage::CallFunction(rust::Str fileName)
{
    int32_t hr = S_OK;

    JsValueRef global;
    JsValueRef result;
    JsValueRef stringValue;
    JsValueType type;
    JsErrorCode errorCode = JsNoError;

    IfJsrtErrorHR(ChakraRTInterface::JsGetGlobalObject(&global));
    IfJsrtErrorHR(ChakraRTInterface::JsGetValueType(m_function, &type));

    if (type == JsString)
    {
        IfJsrtErrorHR(ChakraRTInterface::JsConvertValueToString(m_function, &stringValue));

        JsValueRef fname;
        ChakraRTInterface::JsCreateString("", strlen(""), &fname);
        // Run the code
        errorCode = ChakraRTInterface::JsRun(stringValue, JS_SOURCE_CONTEXT_NONE,
          fname, JsParseScriptAttributeArrayBufferIsUtf16Encoded,
          nullptr /*no result needed*/);
    }
    else
    {
        errorCode = ChakraRTInterface::JsCallFunction(m_function, &global, 1, &result);
    }

    if (errorCode != JsNoError)
    {
        hr = E_FAIL;
        PrintException(fileName, errorCode);
    }

Error:
    return hr;
}

WScriptJsrt::ModuleMessage::ModuleMessage(JsModuleRecord module, JsValueRef specifier, const std::optional<fs::path> &fullpath)
    : MessageBase(0), moduleRecord(module), specifier(specifier)
{
    fullPath_ = std::nullopt;
    ChakraRTInterface::JsAddRef(module, nullptr);
    if (specifier != nullptr)
    {
        fullPath_ = fullpath;
        // nullptr specifier means a Promise to execute; non-nullptr means a "fetch" operation.
        ChakraRTInterface::JsAddRef(specifier, nullptr);
    }
}

WScriptJsrt::ModuleMessage::~ModuleMessage()
{
    ChakraRTInterface::JsRelease(moduleRecord, nullptr);
    if (specifier != nullptr)
    {
        ChakraRTInterface::JsRelease(specifier, nullptr);
    }
}

int32_t WScriptJsrt::ModuleMessage::Call(rust::Str fileName)
{
    JsErrorCode errorCode = JsNoError;
    if (specifier == nullptr)
    {
        if (auto [exists, state] = chakra_rs::get_module_error_map()->get(moduleRecord);
            exists && state != ErroredModule)
        {
            JsValueRef result = JS_INVALID_REFERENCE;
            errorCode = ChakraRTInterface::JsModuleEvaluation(moduleRecord, &result);
            if (errorCode != JsNoError)
            {
                PrintException(fileName, errorCode); // this should not be called
            }
        }
    }
    else
    {
        rust::String specifierStr;
        errorCode = ChakraRTInterface::JsToString(specifier, specifierStr);
        if (errorCode != JsNoError)
        {
            return errorCode;
        }

        try
        {
            rust::String fileContent = fullPath_
                ? chakra_rs::helpers::ScriptCache::load_script_with_full_path(specifierStr, fullPath_->native())
                : chakra_rs::helpers::ScriptCache::load_script_from_file(specifierStr);
            chakra_rs::WScript::load_script(nullptr, fullPath_ ? fullPath_.value().string() : specifierStr, chakra_rs::OptionalStr{.has_value = true, .value = fileContent}, "module", true, true);
        }
        catch (const rust::Error &e)
        {
            chakra::Logger::error(std::format("Caught exception: {}", e.what()));
            if (!HostConfigFlags::GetConfig().host.mute_host_error_msg)
            {
                auto actualModuleRecord = chakra_rs::get_module_record_map()->get(fullPath_.value().native());
                auto error_map_content = chakra_rs::get_module_error_map()->get(actualModuleRecord.content.record);
                if (!actualModuleRecord.exists || (error_map_content.exists && error_map_content.content == RootModule))
                {
                    chakra::Logger::error(std::format("Couldn't load file '{}'", specifierStr));
                }
            }
            chakra_rs::WScript::load_script(nullptr, fullPath_ ? fullPath_.value().string() : specifierStr, chakra_rs::OptionalStr{}, "module", true, false);
        }
    }
    return errorCode;
}
