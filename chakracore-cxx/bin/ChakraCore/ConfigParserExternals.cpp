//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Common/Common.h"
#include "Core/ConfigParser.h"

bool ConfigParserAPI::FillConsoleTitle(__ecount(cchBufferSize) LPWSTR buffer, size_t cchBufferSize, LPWSTR moduleName)
{
    return false;
}

void ConfigParserAPI::DisplayInitialOutput(LPWSTR moduleName)
{
}

LPCWSTR JsUtil::ExternalApi::GetFeatureKeyName()
{
    return _u("");
}

extern "C"
{
    // For now, ChakraCore runs only platform that has MessageBoxW API
    bool IsMessageBoxWPresent()
    {
        return true;
    }
}
