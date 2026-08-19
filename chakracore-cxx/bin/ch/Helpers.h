//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "ChakraCommon.h"
#include <filesystem>
#include <optional>

#define IfJsErrorFailLog(expr) \
do { \
    JsErrorCode jsErrorCode = expr; \
    if ((jsErrorCode) != JsNoError) { \
        chakra::Logger::error(std::format("ERROR: {} failed. JsErrorCode=0x{:x} ({})", #expr, static_cast<int>(jsErrorCode), Helpers::JsErrorCodeToString(jsErrorCode))); \
        goto Error; \
    } \
} while (0)

#define IfJsErrorFailLogLabel(expr, label) \
do { \
    JsErrorCode jsErrorCode = expr; \
    if ((jsErrorCode) != JsNoError) { \
        chakra::Logger::error(std::format("ERROR: {} failed. JsErrorCode=0x{:x} ({})", #expr, static_cast<int>(jsErrorCode), Helpers::JsErrorCodeToString(jsErrorCode))); \
        goto label; \
    } \
} while (0)

#define IfJsErrorFailLogAndRet(expr) \
do { \
    JsErrorCode jsErrorCode = expr; \
    if ((jsErrorCode) != JsNoError) { \
        chakra::Logger::error(std::format("ERROR: {} failed. JsErrorCode=0x{:x} ({})", #expr, static_cast<int>(jsErrorCode), Helpers::JsErrorCodeToString(jsErrorCode))); \
        return JS_INVALID_REFERENCE; \
    } \
} while (0)

#define IfJsrtErrorFailLogAndRetFalse(expr) \
do { \
    JsErrorCode jsErrorCode = expr; \
    if ((jsErrorCode) != JsNoError) { \
        chakra::Logger::error(std::format("ERROR: {} failed. JsErrorCode=0x{:x} ({})", #expr, static_cast<int>(jsErrorCode), Helpers::JsErrorCodeToString(jsErrorCode))); \
        return false; \
    } \
} while (0)

#define IfJsrtErrorFailLogAndRetErrorCode(expr) \
do { \
    JsErrorCode jsErrorCode = expr; \
    if ((jsErrorCode) != JsNoError) { \
        chakra::Logger::error(std::format("ERROR: {} failed. JsErrorCode=0x{:x} ({})", #expr, static_cast<int>(jsErrorCode), Helpers::JsErrorCodeToString(jsErrorCode))); \
        return (jsErrorCode); \
    } \
} while (0)

class Helpers
{
public:
    struct Result
    {
        int32_t hr;
        const char* content;
        size_t length;

        Result() = default;
        explicit Result(int32_t hr) : hr(hr), content(nullptr), length(0) {}
        Result(const char* content, size_t length) : hr(S_OK), content(content), length(length) {}
    };

    static Result LoadScriptFromFile(const char *filename, const std::optional<std::filesystem::path> &fullPath = std::nullopt);
    static const char *JsErrorCodeToString(JsErrorCode jsErrorCode);
    static int32_t LoadBinaryFile(const char *filename, const char *&contents, uint32_t &lengthBytes,
                                  bool printFileOpenError = true);
};
