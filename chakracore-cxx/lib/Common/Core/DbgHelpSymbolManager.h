//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#ifdef DBGHELP_SYMBOL_MANAGER
#define DBGHELP_TRANSLATE_TCHAR

// dbghelp.h is not clean with warning 4091
#pragma warning(push)
#pragma warning(disable: 4091) /* warning C4091: 'typedef ': ignored on left of '' when no variable is declared */
#include <dbghelp.h>
#pragma warning(pop)

class DbgHelpSymbolManager
{
public:
    static void EnsureInitialized() { Instance.Initialize(); }
    static BOOL SymFromAddr(void * address, unsigned long * dwDisplacement, PSYMBOL_INFO pSymbol);
    static BOOL SymGetLineFromAddr64(_In_ void * address, _Out_ uint32_t * pdwDisplacement, _Out_ PIMAGEHLP_LINEW64 pLine);

    static size_t PrintSymbol(void * address);
private:
    DbgHelpSymbolManager() : isInitialized(false), hDbgHelpModule(nullptr), pfnSymFromAddrW(nullptr) {}
    ~DbgHelpSymbolManager();

    static DbgHelpSymbolManager Instance;
    void Initialize();

    bool isInitialized;
    CriticalSection cs;
    HANDLE hProcess;
    HMODULE hDbgHelpModule;

    typedef BOOL(*PfnSymFromAddrW)(HANDLE, unsigned long, unsigned long *, PSYMBOL_INFOW);
    PfnSymFromAddrW pfnSymFromAddrW;

    typedef BOOL(*PfnSymGetLineFromAddr64W)(HANDLE, unsigned long, uint32_t *, PIMAGEHLP_LINEW64);
    PfnSymGetLineFromAddr64W pfnSymGetLineFromAddr64W;
};
#endif
