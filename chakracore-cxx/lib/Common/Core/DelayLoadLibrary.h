//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class DelayLoadLibrary
{
protected:
    HMODULE m_hModule;
    bool m_isInit;

public:
    DelayLoadLibrary();
    virtual ~DelayLoadLibrary();

    virtual LPCTSTR GetLibraryName() const = 0;

    FARPROC GetFunction(const char * lpFunctionName);

    void EnsureFromSystemDirOnly();
    bool IsAvailable();
private:
    void Ensure(uint32_t dwFlags = 0);

};
