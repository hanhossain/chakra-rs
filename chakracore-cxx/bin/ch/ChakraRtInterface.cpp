//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include <dlfcn.h>
#include <print>
#ifdef __APPLE__
const char * chakraDllName = "libChakraCore.dylib";
#else
const char * chakraDllName = "libChakraCore.so";
#endif

bool ChakraRTInterface::m_testHooksSetup = false;
bool ChakraRTInterface::m_testHooksInitialized = false;
bool ChakraRTInterface::m_usageStringPrinted = false;

ChakraRTInterface::ArgInfo* ChakraRTInterface::m_argInfo = nullptr;
TestHooks ChakraRTInterface::m_testHooks = { 0 };
JsAPIHooks ChakraRTInterface::m_jsApiHooks = { 0 };

/*static*/
bool ChakraRTInterface::LoadChakraDll(ArgInfo* argInfo)
{
    m_argInfo = argInfo;
    return true;
}

/*static*/
int32_t ChakraRTInterface::ParseConfigFlags()
{
    int32_t hr = S_OK;

    if (m_testHooks.pfSetAssertToConsoleFlag)
    {
        SetAssertToConsoleFlag(true);
    }

    if (m_testHooks.pfSetConfigFlags)
    {
        hr = SetConfigFlags(m_argInfo->argc, m_argInfo->argv, &HostConfigFlags::flags);
        if (hr != S_OK && !m_usageStringPrinted)
        {
            m_argInfo->hostPrintUsage();
            m_usageStringPrinted = true;
        }
    }

    if (hr == S_OK)
    {
        Assert(m_testHooks.pfGetFilenameFlag != nullptr);

        char16_t* fileNameWide = nullptr;
        hr = GetFileNameFlag(&fileNameWide);

        if (hr != S_OK)
        {
            std::print("Error: no script file specified.");
            m_argInfo->hostPrintUsage();
            m_usageStringPrinted = true;
        }
        else
        {
            m_argInfo->filename = utf8::U16StringToString(fileNameWide);
            SysFreeString(fileNameWide);
        }
    }

    return S_OK;
}

/*static*/
int32_t ChakraRTInterface::InitializeTestHooks(TestHooks& testHooks)
{
    if (!m_testHooksInitialized)
    {
        m_testHooks = testHooks;
        m_testHooksSetup = true;
        m_testHooksInitialized = true;
        return ParseConfigFlags();
    }

    return S_OK;
}
