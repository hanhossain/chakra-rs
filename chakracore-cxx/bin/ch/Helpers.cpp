//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Helpers.h"

#include <filesystem>
#include <iostream>
#include <sys/stat.h>

#include "AutoString.h"
#include "ChakraRtInterface.h"
#include "SourceMap.h"
#include "WScriptJsrt.h"
#include "chakra/Logger.h"

namespace fs = std::filesystem;

#define IfFailedGoLabel(expr, label) do { hr = (expr); if (FAILED(hr)) { goto label; } } while (FALSE)
#define IfFailGo(expr) IfFailedGoLabel(hr = (expr), Error)

Helpers::Result Helpers::LoadScriptFromFile(const char *filenameToLoad, const std::optional<std::filesystem::path> &fullPath)
{
    static fs::path sHostApplicationPath;

    fs::path filenamePath = fullPath.value_or(filenameToLoad);

    // TODO (hanhossain): this just caches the current_dir and converts filenamePath to an absolute path relative to the host
    if (sHostApplicationPath.empty())
    {
        sHostApplicationPath = filenamePath.parent_path();
    }
    else if (filenamePath.is_relative() && !fullPath) // make sure it's not a full path
    {
        filenamePath = sHostApplicationPath / filenamePath;
    }

    // check if have it registered
    const auto cached = SourceMap::Find(filenameToLoad).or_else([&filenamePath]
    {
        return SourceMap::Find(filenamePath.native());
    });

    const char *pRawBytesFromMap = nullptr;
    size_t lengthBytes = 0;
    FILE *file = nullptr;

    if (cached)
    {
        pRawBytesFromMap = cached.value()->c_str();
        lengthBytes = cached.value()->length();
    }
    else
    {
        // Open the file as a binary file to prevent CRT from handling encoding, line-break conversions,
        // etc.
        if (fopen_s(&file, filenamePath.c_str(), "rb") != 0)
        {
            return Result(E_FAIL);
        }

        // TODO (hanhossain): read file with std::ifstream to std::string
        // Determine the file length, in bytes.
        fseek(file, 0, SEEK_END);
        lengthBytes = ftell(file);
        fseek(file, 0, SEEK_SET);
    }

    const size_t bufferLength = lengthBytes != 0 ? lengthBytes + sizeof(uint8_t) : 1;
    const auto pRawBytes = static_cast<uint8_t *>(malloc(bufferLength));
    if (pRawBytes == nullptr)
    {
        chakra::Logger::error("out of memory");
        if (file != nullptr)
        {
            fclose(file);
        }
        return Result(E_OUTOFMEMORY);
    }

    if (lengthBytes != 0)
    {
        if (file != nullptr)
        {
            //
            // Read the entire content as a binary block.
            //
            size_t readBytes = std::fread(pRawBytes, sizeof(uint8_t), lengthBytes, file);
            fclose(file);
            if (readBytes < lengthBytes * sizeof(uint8_t))
            {
                free(pRawBytes);
                return Result(E_FAIL);
            }
        }
        else // from module source register
        {
            // Q: module source is on persistent memory. Why do we use the copy instead?
            // A: if we use the same memory twice, ch doesn't know that during FinalizeCallback free.
            // the copy memory will be freed by the finalizer
            assert(pRawBytesFromMap);
            memcpy(pRawBytes, pRawBytesFromMap, lengthBytes);
        }
    }

    pRawBytes[lengthBytes] = 0; // Null terminate it. Could be UTF16

    auto contents = reinterpret_cast<const char *>(pRawBytes);
    auto result = cached ? cached.value() : std::make_shared<std::string>(contents, lengthBytes);

    return {contents, lengthBytes};
}

const char* Helpers::JsErrorCodeToString(JsErrorCode jsErrorCode)
{
    bool hasException = false;
    ChakraRTInterface::JsHasException(&hasException);
    if (hasException)
    {
        WScriptJsrt::PrintException("", JsErrorScriptException);
    }

    switch (jsErrorCode)
    {
    case JsNoError:                            return "JsNoError";
    // JsErrorCategoryUsage
    case JsErrorCategoryUsage:                 return "JsErrorCategoryUsage";
    case JsErrorInvalidArgument:               return "JsErrorInvalidArgument";
    case JsErrorNullArgument:                  return "JsErrorNullArgument";
    case JsErrorNoCurrentContext:              return "JsErrorNoCurrentContext";
    case JsErrorInExceptionState:              return "JsErrorInExceptionState";
    case JsErrorNotImplemented:                return "JsErrorNotImplemented";
    case JsErrorWrongThread:                   return "JsErrorWrongThread";
    case JsErrorRuntimeInUse:                  return "JsErrorRuntimeInUse";
    case JsErrorBadSerializedScript:           return "JsErrorBadSerializedScript";
    case JsErrorInDisabledState:               return "JsErrorInDisabledState";
    case JsErrorCannotDisableExecution:        return "JsErrorCannotDisableExecution";
    case JsErrorHeapEnumInProgress:            return "JsErrorHeapEnumInProgress";
    case JsErrorArgumentNotObject:             return "JsErrorArgumentNotObject";
    case JsErrorInProfileCallback:             return "JsErrorInProfileCallback";
    case JsErrorInThreadServiceCallback:       return "JsErrorInThreadServiceCallback";
    case JsErrorCannotSerializeDebugScript:    return "JsErrorCannotSerializeDebugScript";
    case JsErrorAlreadyDebuggingContext:       return "JsErrorAlreadyDebuggingContext";
    case JsErrorAlreadyProfilingContext:       return "JsErrorAlreadyProfilingContext";
    case JsErrorIdleNotEnabled:                return "JsErrorIdleNotEnabled";
    case JsErrorInObjectBeforeCollectCallback: return "JsErrorInObjectBeforeCollectCallback";
    case JsErrorObjectNotInspectable:          return "JsErrorObjectNotInspectable";
    case JsErrorPropertyNotSymbol:             return "JsErrorPropertyNotSymbol";
    case JsErrorPropertyNotString:             return "JsErrorPropertyNotString";
    case JsErrorInvalidContext:                return "JsErrorInvalidContext";
    case JsInvalidModuleHostInfoKind:          return "JsInvalidModuleHostInfoKind";
    case JsErrorModuleParsed:                  return "JsErrorModuleParsed";
    // JsErrorCategoryEngine
    case JsErrorCategoryEngine:                return "JsErrorCategoryEngine";
    case JsErrorOutOfMemory:                   return "JsErrorOutOfMemory";
    case JsErrorBadFPUState:                   return "JsErrorBadFPUState";
    // JsErrorCategoryScript
    case JsErrorCategoryScript:                return "JsErrorCategoryScript";
    case JsErrorScriptException:               return "JsErrorScriptException";
    case JsErrorScriptCompile:                 return "JsErrorScriptCompile";
    case JsErrorScriptTerminated:              return "JsErrorScriptTerminated";
    case JsErrorScriptEvalDisabled:            return "JsErrorScriptEvalDisabled";
    // JsErrorCategoryFatal
    case JsErrorCategoryFatal:                 return "JsErrorCategoryFatal";
    case JsErrorFatal:                         return "JsErrorFatal";
    case JsErrorWrongRuntime:                  return "JsErrorWrongRuntime";
    // JsErrorCategoryDiagError
    case JsErrorCategoryDiagError:             return "JsErrorCategoryDiagError";
    case JsErrorDiagAlreadyInDebugMode:        return "JsErrorDiagAlreadyInDebugMode";
    case JsErrorDiagNotInDebugMode:            return "JsErrorDiagNotInDebugMode";
    case JsErrorDiagNotAtBreak:                return "JsErrorDiagNotAtBreak";
    case JsErrorDiagInvalidHandle:             return "JsErrorDiagInvalidHandle";
    case JsErrorDiagObjectNotFound:            return "JsErrorDiagObjectNotFound";
    case JsErrorDiagUnableToPerformAction:     return "JsErrorDiagUnableToPerformAction";
    default:
        return "<unknown>";
        break;
    }
}

int32_t Helpers::LoadBinaryFile(const char * filename, const char *& contents, uint32_t& lengthBytes, bool printFileOpenError)
{
    int32_t hr = S_OK;
    contents = nullptr;
    lengthBytes = 0;
    size_t result;
    FILE * file;

    //
    // Open the file as a binary file to prevent CRT from handling encoding, line-break conversions,
    // etc.
    //
    if (fopen_s(&file, filename, "rb") != 0)
    {
        if (printFileOpenError)
        {
            chakra::Logger::error(std::format("Error in opening file '{}'", filename));
        }
        return E_FAIL;
    }
    // file will not be nullptr if _wfopen_s succeeds

    //
    // Determine the file length, in bytes.
    //
    fseek(file, 0, SEEK_END);
    lengthBytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    contents = (const char *)malloc(lengthBytes);
    if (contents != nullptr)
    {
        memset((void *)contents, 0, lengthBytes);
    }
    else
    {
        chakra::Logger::error("out of memory");
        IfFailGo(E_OUTOFMEMORY);
    }
    //
    // Read the entire content as a binary block.
    //
    result = std::fread((void*)contents, sizeof(char), lengthBytes, file);
    if (result != lengthBytes)
    {
        chakra::Logger::error("Read error");
        IfFailGo(E_FAIL);
    }

Error:
    fclose(file);
    if (contents && FAILED(hr))
    {
        free((void*)contents);
        contents = nullptr;
    }

    return hr;
}