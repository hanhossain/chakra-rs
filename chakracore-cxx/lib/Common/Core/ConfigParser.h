//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Core/CmdParser.h"

// API for methods that the ConfigParser can call
// Implement these methods to customize what happens when the configuration is loaded
class ConfigParserAPI
{
public:
    // If the -Console flag is passed in, this method is called with a buffer
    // The implementor will fill in the buffer and return true if a custom Console window title is desired
    // To not change the console title, return false
    static bool FillConsoleTitle(__ecount(cchBufferSize) char16_t* buffer, size_t cchBufferSize, char16_t* moduleName);
};

class ConfigParser
{
private:
    static const int  MaxTokenSize  = 512;
    static const int MaxRegSize = 2048;
public:
    static ConfigParser s_moduleConfigParser;

    ConfigParser()
    {
    }

    static void ParseOnModuleLoad(CmdLineArgsParser& parser);

    void ProcessConfiguration();
};
