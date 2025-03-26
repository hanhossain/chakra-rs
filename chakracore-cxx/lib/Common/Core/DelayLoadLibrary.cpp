//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"
#include "Core/DelayLoadLibrary.h"

DelayLoadLibrary::DelayLoadLibrary()
{
    m_hModule = nullptr;
    m_isInit = false;
}

DelayLoadLibrary::~DelayLoadLibrary()
{
    if (m_hModule)
    {
        FreeLibrary(m_hModule);
        m_hModule = nullptr;
    }
}

void DelayLoadLibrary::Ensure(DWORD dwFlags)
{
    if (!m_isInit)
    {
        m_hModule = LoadLibraryEx(GetLibraryName(), nullptr, dwFlags);
        m_isInit = true;
    }
}

void DelayLoadLibrary::EnsureFromSystemDirOnly()
{
    Ensure(LOAD_LIBRARY_SEARCH_SYSTEM32);
}


FARPROC DelayLoadLibrary::GetFunction(__in LPCSTR lpFunctionName)
{
    if (m_hModule)
    {
        return GetProcAddress(m_hModule, lpFunctionName);
    }

    return nullptr;
}

bool DelayLoadLibrary::IsAvailable()
{
    return m_hModule != nullptr;
}
