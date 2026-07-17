//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <filesystem>
#include <optional>

class Helpers
{
public :
    static int32_t LoadScriptFromFile(const char * filename, const char *& contents, uint32_t* lengthBytesOut = nullptr, const std::optional<std::filesystem::path> &fullPath = std::nullopt, bool shouldMute = false);
    static const char* JsErrorCodeToString(JsErrorCode jsErrorCode);
    static void LogError(__nullterminated const char16_t *msg, ...);
    static int32_t LoadBinaryFile(const char * filename, const char *& contents, uint32_t& lengthBytes, bool printFileOpenError = true);
};
