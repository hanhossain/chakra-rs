//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include <chakracore-sys/src/chhelper.rs.h>
#include <dlfcn.h>
#include <print>

bool ChakraRTInterface::m_testHooksSetup = false;
bool ChakraRTInterface::m_testHooksInitialized = false;
bool ChakraRTInterface::m_usageStringPrinted = false;

TestHooks ChakraRTInterface::m_testHooks = { 0 };

/*static*/
int32_t ChakraRTInterface::ParseConfigFlags(const std::vector<std::u16string> &vargs)
{
    int32_t hr = S_OK;

    if (m_testHooks.pfSetAssertToConsoleFlag)
    {
        SetAssertToConsoleFlag(true);
    }

    if (m_testHooks.pfSetConfigFlags)
    {
        hr = SetConfigFlags(vargs, &HostConfigFlags::flags);
        if (hr != S_OK && !m_usageStringPrinted)
        {
            chakra_rs::chhelper::print_usage();
            m_usageStringPrinted = true;
        }
    }

    return S_OK;
}

/*static*/
int32_t ChakraRTInterface::InitializeTestHooks(const TestHooks& testHooks, const std::vector<std::u16string> &vargs)
{
    if (!m_testHooksInitialized)
    {
        m_testHooks = testHooks;
        m_testHooksSetup = true;
        m_testHooksInitialized = true;
        return ParseConfigFlags(vargs);
    }

    return S_OK;
}
