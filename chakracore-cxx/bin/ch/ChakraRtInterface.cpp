//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#include <dlfcn.h>
#ifdef __APPLE__
LPCSTR chakraDllName = "libChakraCore.dylib";
#else
LPCSTR chakraDllName = "libChakraCore.so";
#endif

bool ChakraRTInterface::m_testHooksSetup = false;
bool ChakraRTInterface::m_testHooksInitialized = false;
bool ChakraRTInterface::m_usageStringPrinted = false;

ChakraRTInterface::ArgInfo* ChakraRTInterface::m_argInfo = nullptr;
TestHooks ChakraRTInterface::m_testHooks = { 0 };
JsAPIHooks ChakraRTInterface::m_jsApiHooks = { 0 };

// Wrapper functions to abstract out loading ChakraCore
// and resolving its symbols
// Currently, these functions resolve to the PAL on Linux
// but in the future, we can easily switch to a different mechanism
HINSTANCE LoadChakraCore(LPCSTR libPath)
{
    return LoadLibraryExA(libPath, nullptr, 0);
}

void UnloadChakraCore(HINSTANCE module)
{
    FreeLibrary(module);
}

void* GetChakraCoreSymbol(HINSTANCE module, const char* symbol)
{
    return reinterpret_cast<void*>(GetProcAddress(module, symbol));
}

/*static*/
bool ChakraRTInterface::LoadChakraDll(ArgInfo* argInfo, HINSTANCE *outLibrary)
{
    m_argInfo = argInfo;
    return true;
}

/*static*/
void ChakraRTInterface::UnloadChakraDll(HINSTANCE library)
{
}

/*static*/
HRESULT ChakraRTInterface::ParseConfigFlags()
{
    HRESULT hr = S_OK;

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
        m_argInfo->filename = nullptr;
        Assert(m_testHooks.pfGetFilenameFlag != nullptr);

        char16_t* fileNameWide = nullptr;
        hr = GetFileNameFlag(&fileNameWide);

        if (hr != S_OK)
        {
            wprintf(u"Error: no script file specified.");
            m_argInfo->hostPrintUsage();
            m_usageStringPrinted = true;
        }
        else
        {
            hr = WideStringToNarrowDynamic(fileNameWide, &m_argInfo->filename);
            SysFreeString(fileNameWide);
            if (FAILED(hr))
            {
                Assert(hr == E_OUTOFMEMORY);
                wprintf(u"Error: Ran out of memory");
                return hr;
            }
        }
    }

    return S_OK;
}

/*static*/
HRESULT ChakraRTInterface::OnChakraCoreLoaded(TestHooks& testHooks)
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
