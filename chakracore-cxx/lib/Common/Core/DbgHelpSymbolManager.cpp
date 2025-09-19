//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"

#ifdef DBGHELP_SYMBOL_MANAGER
#include "Core/DbgHelpSymbolManager.h"

// Initialization order
//  AB AutoSystemInfo
//  AD PerfCounter
//  AE PerfCounterSet
//  AM Output/Configuration
//  AN MemProtectHeap
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AR LeakReport
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAP")

DbgHelpSymbolManager DbgHelpSymbolManager::Instance;

void
DbgHelpSymbolManager::Initialize()
{
    char16_t *wszSearchPath = nullptr;
    char16_t *wszModuleDrive = nullptr;
    char16_t *wszModuleDir = nullptr;
    char16_t *wszOldSearchPath = nullptr;
    char16_t *wszNewSearchPath = nullptr;
    char16_t *wszModuleName = nullptr;
    char16_t const *wszModule = nullptr;

    const size_t ceModuleName = _MAX_PATH;
    const size_t ceOldSearchPath = 32767;
    const size_t ceNewSearchPath = ceOldSearchPath + _MAX_PATH + 1;

    if (isInitialized)
    {
        return;
    }

    AutoCriticalSection autocs(&cs);
    if (isInitialized)
    {
        goto end;
    }
    isInitialized = true;
    hProcess = GetCurrentProcess();

    // Let's make sure the directory where chakra.dll is, is on the symbol path.

    wszModule = AutoSystemInfo::GetJscriptDllFileName();
    wszModuleName = NoCheckHeapNewArray(char16_t, ceModuleName);
    if (wszModuleName == nullptr)
    {
        goto end;
    }

    if (wcscmp(wszModule, u"") == 0)
    {
        if (PlatformAgnostic::SystemInfo::GetBinaryLocation(wszModuleName, static_cast<uint32_t>(ceModuleName)))
        {
            wszModule = wszModuleName;
        }
        else
        {
            wszModule = nullptr;
        }
    }

    if (wszModule != nullptr)
    {
        wszModuleDrive = NoCheckHeapNewArray(char16_t, _MAX_DRIVE);
        if (wszModuleDrive == nullptr)
        {
            goto end;
        }

        wszModuleDir = NoCheckHeapNewArray(char16_t, _MAX_DIR);
        if (wszModuleDir == nullptr)
        {
            goto end;
        }

        _wsplitpath_s(wszModule, wszModuleDrive, _MAX_DRIVE, wszModuleDir, _MAX_DIR, NULL, 0, NULL, 0);
        _wmakepath_s(wszModuleName, ceModuleName, wszModuleDrive, wszModuleDir, NULL, NULL);

        wszOldSearchPath = NoCheckHeapNewArray(char16_t, ceOldSearchPath);
        if (wszOldSearchPath == nullptr)
        {
            goto end;
        }

        wszNewSearchPath = NoCheckHeapNewArray(char16_t, ceNewSearchPath);
        if (wszNewSearchPath == nullptr)
        {
            goto end;
        }

        if (GetEnvironmentVariable(u"_NT_SYMBOL_PATH", wszOldSearchPath, static_cast<uint32_t>(ceOldSearchPath)) != 0)
        {
            swprintf_s(wszNewSearchPath, ceNewSearchPath, u"%s;%s", wszOldSearchPath, wszModuleName);
            wszSearchPath = wszNewSearchPath;
        }
        else
        {
            wszSearchPath = wszModuleName;
        }
    }

    hDbgHelpModule = LoadLibraryEx(u"dbghelp.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hDbgHelpModule == nullptr)
    {
        goto end;
    }

    {
        typedef BOOL(*PfnSymInitialize)(HANDLE, const char16_t *, BOOL);
        PfnSymInitialize pfnSymInitialize = (PfnSymInitialize)GetProcAddress(hDbgHelpModule, "SymInitializeW");
        if (pfnSymInitialize)
        {
            pfnSymInitialize(hProcess, wszSearchPath, TRUE);
            pfnSymFromAddrW = (PfnSymFromAddrW)GetProcAddress(hDbgHelpModule, "SymFromAddrW");
            pfnSymGetLineFromAddr64W = (PfnSymGetLineFromAddr64W)GetProcAddress(hDbgHelpModule, "SymGetLineFromAddrW64");

            // load line information
            typedef uint32_t(*PfnSymGetOptions)();
            typedef void(*PfnSymSetOptions)(uint32_t);
            PfnSymGetOptions pfnSymGetOptions = (PfnSymGetOptions)GetProcAddress(hDbgHelpModule, "SymGetOptions");
            PfnSymSetOptions pfnSymSetOptions = (PfnSymSetOptions)GetProcAddress(hDbgHelpModule, "SymSetOptions");

            uint32_t options = pfnSymGetOptions();
            options |= SYMOPT_LOAD_LINES;
            pfnSymSetOptions(options);
        }
    }

end:
    if (wszModuleName != nullptr)
    {
        NoCheckHeapDeleteArray(ceModuleName, wszModuleName);
        wszModuleName = nullptr;
    }

    if (wszModuleDrive != nullptr)
    {
        NoCheckHeapDeleteArray(_MAX_DRIVE, wszModuleDrive);
        wszModuleDrive = nullptr;
    }

    if (wszModuleDir != nullptr)
    {
        NoCheckHeapDeleteArray(_MAX_DIR, wszModuleDir);
        wszModuleDir = nullptr;
    }

    if (wszOldSearchPath != nullptr)
    {
        NoCheckHeapDeleteArray(ceOldSearchPath, wszOldSearchPath);
        wszOldSearchPath = nullptr;
    }

    if (wszNewSearchPath != nullptr)
    {
        NoCheckHeapDeleteArray(ceNewSearchPath, wszNewSearchPath);
        wszNewSearchPath = nullptr;
    }
}

DbgHelpSymbolManager::~DbgHelpSymbolManager()
{
    if (hDbgHelpModule)
    {
        typedef BOOL(*PfnSymCleanup)(HANDLE);
        PfnSymCleanup pfnSymCleanup = (PfnSymCleanup)GetProcAddress(hDbgHelpModule, "SymCleanup");
        if (pfnSymCleanup)
        {
            pfnSymCleanup(hProcess);
        }

        FreeLibrary(hDbgHelpModule);
    }
}

BOOL
DbgHelpSymbolManager::SymFromAddr(void * address, unsigned long * dwDisplacement, PSYMBOL_INFO pSymbol)
{
    if (Instance.pfnSymFromAddrW)
    {
        return Instance.pfnSymFromAddrW(Instance.hProcess, (unsigned long)address, dwDisplacement, pSymbol);
    }
    return FALSE;
}

BOOL
DbgHelpSymbolManager::SymGetLineFromAddr64(_In_ void * address, _Out_ uint32_t * pdwDisplacement, _Out_ PIMAGEHLP_LINEW64 pLine)
{
    if (pdwDisplacement != nullptr)
    {
        *pdwDisplacement = 0;
    }

    if (pLine != nullptr)
    {
        ZeroMemory(pLine, sizeof(IMAGEHLP_LINEW64));
        pLine->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    }

    if (Instance.pfnSymGetLineFromAddr64W)
    {
        return Instance.pfnSymGetLineFromAddr64W(Instance.hProcess, (unsigned long)address, pdwDisplacement, pLine);
    }
    return FALSE;
}

size_t DbgHelpSymbolManager::PrintSymbol(void * address)
{
    size_t retValue = 0;
    unsigned long  dwDisplacement = 0;
    char buffer[sizeof(SYMBOL_INFO)+MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    IMAGEHLP_LINE64 lineInfo;
    lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    if (DbgHelpSymbolManager::SymFromAddr(address, &dwDisplacement, pSymbol))
    {
        uint32_t dwDisplacementDWord = static_cast<uint32_t>(dwDisplacement);
        if (DbgHelpSymbolManager::SymGetLineFromAddr64(address, &dwDisplacementDWord, &lineInfo))
        {
            retValue += Output::Print(u"0x%p %s+0x%llx (%s:%d)", address, pSymbol->Name, dwDisplacement, lineInfo.FileName, lineInfo.LineNumber);
        }
        else
        {
            // SymGetLineFromAddr64 failed
            retValue += Output::Print(u"0x%p %s+0x%llx", address, pSymbol->Name, dwDisplacement);
        }
    }
    else
    {
        // SymFromAddr failed
        retValue += Output::Print(u"0x%p", address);
    }
    return retValue;
}
#endif
