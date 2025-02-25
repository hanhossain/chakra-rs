//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

#ifdef _WIN32
LPCSTR chakraDllName = "chakracore.dll";
LPCWSTR chakraDllNameW = _u("chakracore.dll");
#else
#include <dlfcn.h>
#ifdef __APPLE__
LPCSTR chakraDllName = "libChakraCore.dylib";
#else
LPCSTR chakraDllName = "libChakraCore.so";
#endif
#endif

bool ChakraRTInterface::m_testHooksSetup = false;
bool ChakraRTInterface::m_testHooksInitialized = false;
bool ChakraRTInterface::m_usageStringPrinted = false;

ChakraRTInterface::ArgInfo* ChakraRTInterface::m_argInfo = nullptr;
TestHooks ChakraRTInterface::m_testHooks = { 0 };
JsAPIHooks ChakraRTInterface::m_jsApiHooks = { 0 };

#ifdef _WIN32
LPCWSTR GetChakraDllNameW()
{
    return chakraDllNameW;
}
#endif

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
#ifndef CHAKRA_STATIC_LIBRARY
    Assert(library != nullptr);
    FARPROC pDllCanUnloadNow = (FARPROC) GetChakraCoreSymbol(library, "DllCanUnloadNow");
    if (pDllCanUnloadNow != nullptr)
    {
        pDllCanUnloadNow();
    }
#ifdef _WIN32
    UnloadChakraCore(library);
#else  // !_WIN32
    // PAL thread shutdown needs more time after execution completion.
    // Do not FreeLibrary. Invoke DllMain(DLL_PROCESS_DETACH) directly.
    typedef BOOL (__stdcall *PDLLMAIN)(HINSTANCE, DWORD, LPVOID);
    PDLLMAIN pDllMain = (PDLLMAIN) GetChakraCoreSymbol(library, "DllMain");
    if (pDllMain)
    {
        pDllMain(library, DLL_PROCESS_DETACH, NULL);
    }
#endif
#endif
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

        char16* fileNameWide = nullptr;
        hr = GetFileNameFlag(&fileNameWide);

        if (hr != S_OK)
        {
            wprintf(_u("Error: no script file specified."));
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
                wprintf(_u("Error: Ran out of memory"));
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
