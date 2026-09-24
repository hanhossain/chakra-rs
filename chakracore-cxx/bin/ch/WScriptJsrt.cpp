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
#define IfJsrtErrorSetGoLabel(expr, label) do { errorCode = (expr); if(errorCode != JsNoError) { hr = E_FAIL; goto label; } } while(0)
#define IfFalseGo(expr) do { if(!(expr)) { hr = E_FAIL; goto Error; } } while(0)

#pragma prefast(disable:26444, "This warning unfortunately raises false positives when auto is used for declaring the type of an iterator in a loop.")

unsigned int MessageBase::s_messageCount = 0;
MessageQueue* WScriptJsrt::messageQueue_ = nullptr;
std::size_t WScriptJsrt::sourceContext_ = 0;

std::size_t WScriptJsrt::GetNextSourceContext()
{
    return sourceContext_++;
}

// TODO (hanhossain): do I need to free anything?
void WScriptJsrt::FinalizeFree(void* addr)
{
    // free(addr);
}

JsValueRef WScriptJsrt::LoadScriptFileHelper(JsValueRef callee, const rust::Slice<JsValueRef const> arguments, bool isSourceModule)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsNoError;
    std::string errorMessage;

    if (arguments.size() < 2 || arguments.size() > 4)
    {
        errorCode = JsErrorInvalidArgument;
        errorMessage = "Need more or fewer arguments for WScript.LoadScript";
    }
    else
    {
        rust::String fileName;
        IfJsrtErrorSetGo(chakracore::jsrt::JsToString(arguments[1], fileName));

        rust::String scriptInjectType;
        if (arguments.size() > 2)
        {
            IfJsrtErrorSetGo(chakracore::jsrt::JsToString(arguments[2], scriptInjectType));
        }

        // TODO (hanhossain): don't leak a string ptr
        rust::String *content;
        try
        {
            content = new rust::String{chakra_rs::helpers::ScriptCache::load_script_from_file(fileName)};
        }
        catch (const rust::Error &e)
        {
            chakra::Logger::error(std::format("Couldn't load file '{}' due to exception '{}'", fileName, e.what()));
            IfJsrtErrorSetGo(ChakraRTInterface::JsGetUndefinedValue(&returnValue));
            return returnValue;
        }

        returnValue = LoadScript(callee, fileName, *content, !scriptInjectType.empty() ? scriptInjectType : "self", isSourceModule, WScriptJsrt::FinalizeFree, true);
    }

Error:

    SetExceptionIf(errorCode, errorMessage);
    return returnValue;
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
        returnValue = LoadScript(args.callee, fileName, *fileContent, scriptInjectType ? scriptInjectType.value() : "self", isSourceModule, WScriptJsrt::FinalizeFree, isFile);
    }

Error:
    SetExceptionIf(errorCode, errorMessage);
    return returnValue;
}

JsErrorCode WScriptJsrt::ModuleEntryPoint(rust::Str fileContent, const rust::String &fullName)
{
    auto span = chakra::Span::create("WScriptJsrt::ModuleEntryPoint");
    return LoadModuleFromString(fileContent, static_cast<std::string>(fullName), true);
}

JsErrorCode WScriptJsrt::LoadModuleFromString(const std::optional<rust::Str> &fileContent, const std::string &fullName, bool isFile)
{
    auto span = chakra::Span::create("WScriptJsrt::LoadModuleFromString");
    unsigned long dwSourceCookie = WScriptJsrt::GetNextSourceContext();
    JsModuleRecord requestModule = JS_INVALID_REFERENCE;
    const std::string& moduleRecordKey = fullName;
    auto moduleRecordMapContent = chakra_rs::get_module_record_map()->get(moduleRecordKey);
    JsErrorCode errorCode = JsNoError;

    // we need to create a new moduleRecord if the specifier (fileName) is not found;
    // otherwise we'll use the old one.
    if (!moduleRecordMapContent.exists)
    {
        JsValueRef specifier = nullptr;
        if (isFile)
        {
            errorCode = ChakraRTInterface::JsCreateString(fullName, &specifier);
        }
        if (errorCode == JsNoError)
        {
            errorCode = ChakraRTInterface::JsInitializeModuleRecord(
                nullptr, specifier, &requestModule);
        }
        if (errorCode == JsNoError)
        {
            chakra_rs::get_module_directory_map()->insert(requestModule, fs::path(fullName).parent_path().native());
            chakra_rs::get_module_record_map()->insert(moduleRecordKey, chakra_rs::ModuleRecordEntry { requestModule });
            auto module_error_map = chakra_rs::get_module_error_map();
            module_error_map->insert(requestModule, RootModule);
        }
    }
    else
    {
        requestModule = moduleRecordMapContent.content.record;
    }
    IfJsrtErrorFailLogAndRetErrorCode(errorCode);
    JsValueRef errorObject = JS_INVALID_REFERENCE;

    // ParseModuleSource is sync, while additional fetch & evaluation are async.
    errorCode = ChakraRTInterface::JsParseModuleSource(requestModule, dwSourceCookie, (uint8_t *)(fileContent ? fileContent.value().data() : nullptr),
        fileContent ? fileContent.value().size() : 0, JsParseModuleSourceFlags_DataIsUTF8, &errorObject);
    if ((errorCode != JsNoError) && errorObject != JS_INVALID_REFERENCE && fileContent &&
        !HostConfigFlags::GetConfig().host.ignore_script_error_code)
    {
        if (auto [exists, state] = chakra_rs::get_module_error_map()->get(requestModule); exists && state == RootModule)
        {
            ChakraRTInterface::JsSetException(errorObject);
            auto module_error_map = chakra_rs::get_module_error_map();
            module_error_map->insert(requestModule, ErroredModule);
            return errorCode;
        }
    }
    return JsNoError;
}


JsValueRef WScriptJsrt::LoadScript(JsValueRef callee, rust::Str fileName,
    const std::optional<rust::Str> &content, rust::Str scriptInjectType, bool isSourceModule, JsFinalizeCallback finalizeCallback, bool isFile)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsErrorCode errorCode = JsNoError;
    std::string_view errorMessage = "Internal error.";
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsContextRef currentContext = JS_INVALID_REFERENCE;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    std::error_code ec;

    auto fullPath = fs::absolute(static_cast<std::string_view>(fileName), ec).lexically_normal();

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetCurrentContext(&currentContext));
    IfJsrtErrorSetGo(ChakraRTInterface::JsGetRuntime(currentContext, &runtime));

    // this is called with LoadModuleCallback method as well where caller pass in a string that should be
    // treated as a module source text instead of opening a new file.
    if (isSourceModule || scriptInjectType == "module")
    {
        errorCode = LoadModuleFromString(content, fullPath, isFile);
    }
    else if (scriptInjectType == "self")
    {
        JsContextRef calleeContext;
        IfJsrtErrorSetGo(ChakraRTInterface::JsGetContextOfObject(callee, &calleeContext));

        IfJsrtErrorSetGo(ChakraRTInterface::JsSetCurrentContext(calleeContext));

        JsValueRef scriptSource;
        IfJsrtErrorSetGo(ChakraRTInterface::JsCreateExternalArrayBuffer(*content, finalizeCallback, &scriptSource));
        JsValueRef fname;
        IfJsrtErrorSetGo(ChakraRTInterface::JsCreateString(fullPath, &fname));
        JsSourceContext sourceContext = GetNextSourceContext();

        if (HostConfigFlags::GetConfig().host.use_parser_state_cache)
        {
            JsValueRef parserState;
            IfJsrtErrorSetGo(ChakraRTInterface::JsSerializeParserState(scriptSource, &parserState, JsParseScriptAttributeNone));
            errorCode = ChakraRTInterface::JsRunScriptWithParserState(scriptSource, sourceContext, fname, JsParseScriptAttributeNone, parserState, &returnValue);
        }
        else
        {
            errorCode = ChakraRTInterface::JsRun(scriptSource, sourceContext, fname, JsParseScriptAttributeNone, &returnValue);
        }

        if(errorCode == JsNoError)
        {
            errorCode = ChakraRTInterface::JsGetGlobalObject(&returnValue);
        }

        IfJsrtErrorSetGo(ChakraRTInterface::JsSetCurrentContext(currentContext));
    }
    else if (scriptInjectType == "samethread")
    {
        JsValueRef newContext = JS_INVALID_REFERENCE;

        // Create a new context and set it as the current context
        IfJsrtErrorSetGo(ChakraRTInterface::JsCreateContext(runtime, &newContext));

        IfJsrtErrorSetGo(ChakraRTInterface::JsSetCurrentContext(newContext));

        IfJsErrorFailLog(ChakraRTInterface::JsSetPromiseContinuationCallback(chakra_rs::WScript::promise_continuation_callback, (void*)messageQueue_));

        // Initialize the host objects
        chakra_rs::WScript::initialize();

        JsValueRef scriptSource;
        IfJsrtErrorSetGo(ChakraRTInterface::JsCreateExternalArrayBuffer(*content, finalizeCallback, &scriptSource));
        JsValueRef fname;
        IfJsrtErrorSetGo(ChakraRTInterface::JsCreateString(fullPath, &fname));
        JsSourceContext sourceContext = GetNextSourceContext();

        if (HostConfigFlags::GetConfig().host.use_parser_state_cache)
        {
            JsValueRef parserState;
            IfJsrtErrorSetGo(ChakraRTInterface::JsSerializeParserState(scriptSource, &parserState, JsParseScriptAttributeNone));
            errorCode = ChakraRTInterface::JsRunScriptWithParserState(scriptSource, sourceContext, fname, JsParseScriptAttributeNone, parserState, &returnValue);
        }
        else
        {
            errorCode = ChakraRTInterface::JsRun(scriptSource, sourceContext, fname, JsParseScriptAttributeNone, &returnValue);
        }

        if (errorCode == JsNoError)
        {
            errorCode = ChakraRTInterface::JsGetGlobalObject(&returnValue);
        }

        // Set the context back to the old one
        ChakraRTInterface::JsSetCurrentContext(currentContext);
    }
    else if (scriptInjectType == "crossthread")
    {
        auto& threadData = GetRuntimeThreadLocalData().threadData;
        if (threadData == nullptr)
        {
            threadData = new RuntimeThreadData();
        }

        RuntimeThreadData* child = new RuntimeThreadData();
        child->initialSource = rust::String{content.value().data(), content.value().size()};
        threadData->children.push_back(child);
        child->parent = threadData;

        // TODO: need to add a switch in case we don't need to wait for
        // child initial script completion
        threadData->reset_initial_script_completed();

        child->hThread = ::CreateThread(NULL, [](void* param) -> uint32_t
        {
            return ((RuntimeThreadData*)param)->ThreadProc();
        }, (void*)child, NULL, NULL);

        threadData->wait_initial_script_completed();
    }
    else
    {
        errorCode = JsErrorInvalidArgument;
        errorMessage = "Unsupported argument type inject type.";
    }

Error:
    JsValueRef value = returnValue;
    if (errorCode != JsNoError)
    {
        SetExceptionIf(errorCode, errorMessage);
        ChakraRTInterface::JsDoubleToNumber(errorCode, &value);
    }

    fflush(NULL);

    return value;
}

JsValueRef WScriptJsrt::SetTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    constexpr std::string_view errorMessage = "invalid call to WScript.SetTimeout";
    JsErrorCode errorCode = JsNoError;
    [[maybe_unused]] int32_t hr = S_OK;

    JsValueRef function;
    JsValueRef timerId;
    unsigned int time;
    double tmp;
    CallbackMessage *msg = nullptr;

    if (args.arguments.size() != 3)
    {
        errorCode = JsErrorInvalidArgument;
        goto Error;
    }

    function = args.arguments[1];

    IfJsrtErrorSetGo(ChakraRTInterface::JsNumberToDouble(args.arguments[2], &tmp));

    time = static_cast<int>(tmp);
    msg = new CallbackMessage(time, function);
    messageQueue_->InsertSorted(msg);

    IfJsrtErrorSetGo(ChakraRTInterface::JsDoubleToNumber(static_cast<double>(msg->GetId()), &timerId));
    return timerId;

Error:
    SetExceptionIf(errorCode, errorMessage);
    return JS_INVALID_REFERENCE;
}

JsValueRef WScriptJsrt::ClearTimeoutCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    constexpr std::string_view errorMessage = "invalid call to WScript.ClearTimeout";
    JsErrorCode errorCode = JsNoError;
    [[maybe_unused]] int32_t hr = S_OK;

    if (args.arguments.size() != 2)
    {
        errorCode = JsErrorInvalidArgument;
        goto Error;
    }

    unsigned int timerId;
    double tmp;
    JsValueRef undef;

    if (ChakraRTInterface::JsNumberToDouble(args.arguments[1], &tmp) == JsNoError)
    {
        timerId = static_cast<int>(tmp);
        messageQueue_->RemoveById(timerId);
    }

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetUndefinedValue(&undef));
    return undef;

Error:
    SetExceptionIf(errorCode, errorMessage);
    return JS_INVALID_REFERENCE;
}

bool WScriptJsrt::Uninitialize()
{
    // moduleRecordMap is a global std::map, its destructor may access overridden
    // "operator delete" / global HeapAllocator::Instance. Clear it manually here
    // to avoid worrying about global destructor order.
    chakra_rs::get_module_record_map()->clear();
    chakra_rs::get_module_directory_map()->clear();
    chakra_rs::get_module_error_map()->clear();

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

JsValueRef WScriptJsrt::LoadBinaryFileCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    if (args.arguments.size() < 2)
    {
        JsValueRef returnValue;
        ChakraRTInterface::JsGetUndefinedValue(&returnValue);
        return returnValue;
    }

    rust::String fileName;
    if (ChakraRTInterface::JsToString(args.arguments[1], fileName) != JsNoError)
    {
        return JS_INVALID_REFERENCE;
    }

    auto fileContent = chakra_rs::fs::read_binary_file(fileName);

    JsValueRef arrayBuffer;
    if (ChakraRTInterface::JsCreateArrayBuffer(fileContent.size(), &arrayBuffer) != JsNoError)
    {
        return JS_INVALID_REFERENCE;
    }

    uint8_t *buffer;
    unsigned int bufferLength;
    if (ChakraRTInterface::JsGetArrayBufferStorage(arrayBuffer, &buffer, &bufferLength) != JsNoError)
    {
        return JS_INVALID_REFERENCE;
    }

    if (bufferLength < fileContent.size())
    {
        chakra::Logger::error("Array buffer size is insufficient to store the binary file.");
        return JS_INVALID_REFERENCE;
    }

    memcpy(buffer, fileContent.data(), fileContent.size());
    return arrayBuffer;
}

JsValueRef WScriptJsrt::BroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsNoError;

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetUndefinedValue(&returnValue));

    if (args.arguments.size() > 1)
    {
        auto& threadData = GetRuntimeThreadLocalData().threadData;
        if (threadData)
        {
            ChakraRTInterface::JsGetSharedArrayBufferContent(args.arguments[1], &threadData->sharedContent);

            std::size_t count = threadData->children.size();
            threadData->semaphore.emplace(count);

            for (const auto child : threadData->children)
            {
                SetEvent(child->hevntReceivedBroadcast);
            }

            threadData->semaphore->acquire();
            threadData->semaphore.reset();

            ChakraRTInterface::JsReleaseSharedArrayBufferContentHandle(threadData->sharedContent);
        }
    }

Error:
    return returnValue;
}

JsValueRef WScriptJsrt::ReceiveBroadcastCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsNoError;

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetUndefinedValue(&returnValue));

    if (args.arguments.size() > 1)
    {
        auto& threadData = GetRuntimeThreadLocalData().threadData;
        if (threadData)
        {
            if (threadData->receiveBroadcastCallbackFunc)
            {
                ChakraRTInterface::JsRelease(threadData->receiveBroadcastCallbackFunc, nullptr);
            }
            threadData->receiveBroadcastCallbackFunc = args.arguments[1];
            ChakraRTInterface::JsAddRef(threadData->receiveBroadcastCallbackFunc, nullptr);
        }
    }

Error:
    return returnValue;
}

JsValueRef WScriptJsrt::ReportCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsNoError;

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetUndefinedValue(&returnValue));

    if (args.arguments.size() > 1)
    {
        JsValueRef stringRef;
        ChakraRTInterface::JsConvertValueToString(args.arguments[1], &stringRef);

        rust::String autoStr;
        if (ChakraRTInterface::JsToString(stringRef,autoStr) == JsNoError)
        {
            auto& threadData = GetRuntimeThreadLocalData().threadData;

            if (threadData && threadData->parent)
            {
                std::unique_lock lease{threadData->parent->csReportQ};
                threadData->parent->reportQ.push_back(static_cast<std::string>(autoStr));
            }
        }
    }

Error:
    return returnValue;
}

JsValueRef WScriptJsrt::GetReportCallback(const chakra_rs::JsNativeFunctionArgs &args)
{
    [[maybe_unused]] int32_t hr = E_FAIL;
    JsValueRef returnValue = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsNoError;

    IfJsrtErrorSetGo(ChakraRTInterface::JsGetNullValue(&returnValue));

    if (args.arguments.size() > 0)
    {
        auto& threadData = GetRuntimeThreadLocalData().threadData;
        if (threadData)
        {
            std::unique_lock lease{threadData->csReportQ};
            if (!threadData->reportQ.empty())
            {
                const auto str = threadData->reportQ.front();
                threadData->reportQ.pop_front();
                ChakraRTInterface::JsCreateString(str, &returnValue);
            }
        }
    }

Error:
    return returnValue;
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
            LoadScript(nullptr, fullPath_ ? fullPath_.value().string() : specifierStr, fileContent, "module", true,
                       WScriptJsrt::FinalizeFree, true);
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
            LoadScript(nullptr, fullPath_ ? fullPath_.value().string() : specifierStr, std::nullopt, "module", true, WScriptJsrt::FinalizeFree, false);
        }
    }
    return errorCode;
}
