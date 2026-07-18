//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <filesystem>
#include <iostream>
#include <print>
#include <sys/stat.h>

namespace fs = std::filesystem;

int32_t Helpers::LoadScriptFromFile(const char * filenameToLoad, const char *& contents, uint32_t* lengthBytesOut /*= nullptr*/, const std::optional<std::filesystem::path> &fullPath, bool shouldMute /*=false */)
{
    static fs::path sHostApplicationPath;

    int32_t hr = S_OK;
    uint8_t * pRawBytes = nullptr;
    uint8_t * pRawBytesFromMap = nullptr;
    uint32_t lengthBytes = 0;
    contents = nullptr;
    FILE * file = NULL;
    size_t bufferLength = 0;

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
    AutoString *data;
    if (SourceMap::Find(filenameToLoad, strlen(filenameToLoad), &data) ||
        SourceMap::Find(filenamePath, &data))
    {
        pRawBytesFromMap = (uint8_t*) data->GetString();
        lengthBytes = (uint32_t) data->GetLength();
    }
    else
    {
        // Open the file as a binary file to prevent CRT from handling encoding, line-break conversions,
        // etc.
        if (fopen_s(&file, filenamePath.c_str(), "rb") != 0)
        {
            IfFailGo(E_FAIL);
        }
    }

    // TODO (hanhossain): read file with std::ifstream to std::string
    if (file != NULL)
    {
        // Determine the file length, in bytes.
        fseek(file, 0, SEEK_END);
        lengthBytes = ftell(file);
        fseek(file, 0, SEEK_SET);
    }

    if (lengthBytes != 0)
    {
        bufferLength = lengthBytes + sizeof(uint8_t);
        pRawBytes = (uint8_t *)malloc(bufferLength);
    }
    else
    {
        bufferLength = 1;
        pRawBytes = (uint8_t *)malloc(bufferLength);
    }

    if (nullptr == pRawBytes)
    {
        std::print(stderr, "out of memory");
        IfFailGo(E_OUTOFMEMORY);
    }

    if (lengthBytes != 0)
    {
        if (file != NULL)
        {
            //
            // Read the entire content as a binary block.
            //
            size_t readBytes = std::fread(pRawBytes, sizeof(uint8_t), lengthBytes, file);
            if (readBytes < lengthBytes * sizeof(uint8_t))
            {
                IfFailGo(E_FAIL);
            }
        }
        else // from module source register
        {
            // Q: module source is on persistent memory. Why do we use the copy instead?
            // A: if we use the same memory twice, ch doesn't know that during FinalizeCallback free.
            // the copy memory will be freed by the finalizer
            Assert(pRawBytesFromMap);
            memcpy(pRawBytes, pRawBytesFromMap, lengthBytes);
        }
    }

    if (pRawBytes)
    {
        pRawBytes[lengthBytes] = 0; // Null terminate it. Could be UTF16
    }

    if (file != NULL)
    {
        //
        // Read encoding to make sure it's supported
        //
        // Warning: The UNICODE buffer for parsing is supposed to be provided by the host.
        // This is not a complete read of the encoding. Some encodings like UTF7, UTF1, EBCDIC, SCSU, BOCU could be
        // wrongly classified as ANSI
        //
#pragma warning(push)
        // suppressing prefast warning that "readable size is bufferLength
        // bytes but 2 may be read" as bufferLength is clearly > 2 in the code that follows
#pragma warning(disable:6385)
        static_assert(sizeof(char16_t) == 2);
        if (bufferLength > 2)
        {
#pragma prefast(push)
#pragma prefast(disable:6385, "PREfast incorrectly reports this as an out-of-bound access.");
            if ((pRawBytes[0] == 0xFE && pRawBytes[1] == 0xFF) ||
                (pRawBytes[0] == 0xFF && pRawBytes[1] == 0xFE) ||
                (bufferLength > 4 && pRawBytes[0] == 0x00 && pRawBytes[1] == 0x00 &&
                    ((pRawBytes[2] == 0xFE && pRawBytes[3] == 0xFF) ||
                    (pRawBytes[2] == 0xFF && pRawBytes[3] == 0xFE))))

            {
                // unicode unsupported
                std::print(stderr, "unsupported file encoding. Only ANSI and UTF8 supported");
                IfFailGo(E_UNEXPECTED);
            }
#pragma prefast(pop)
        }
#pragma warning(pop)
    }

    contents = reinterpret_cast<const char *>(pRawBytes);

Error:
    if (SUCCEEDED(hr))
    {
        if (lengthBytesOut)
        {
            *lengthBytesOut = lengthBytes;
        }
    }

    if (file != NULL)
    {
        fclose(file);
    }

    if (pRawBytes && reinterpret_cast<const char *>(pRawBytes) != contents)
    {
        free(pRawBytes);
    }

    return hr;
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

void Helpers::LogError(__nullterminated const char16_t *msg, ...)
{
    va_list args;
    va_start(args, msg);
    std::print("ERROR: ");
    PAL_vfwprintf(stderr, msg, args);
    std::println();
    fflush(stdout);
    va_end(args);
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
            fprintf(stderr, "Error in opening file '%s' ", filename);
            fprintf(stderr, "\n");
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
        std::print(stderr, "out of memory");
        IfFailGo(E_OUTOFMEMORY);
    }
    //
    // Read the entire content as a binary block.
    //
    result = std::fread((void*)contents, sizeof(char), lengthBytes, file);
    if (result != lengthBytes)
    {
        std::print(stderr, "Read error");
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