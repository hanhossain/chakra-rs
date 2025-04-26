//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Automatic system info getter at startup
//----------------------------------------------------------------------------
#pragma once

class AutoSystemInfo : public SYSTEM_INFO
{
    friend void ChakraBinaryAutoSystemInfoInit(AutoSystemInfo *);  // The hosting DLL provides the implementation of this function.
public:
    static AutoSystemInfo Data;
    uint GetAllocationGranularityPageCount() const;
    uint GetAllocationGranularityPageSize() const;

    bool DisableDebugScopeCapture() const { return this->disableDebugScopeCapture; }
    bool IsWin8OrLater();
    bool IsWin8Point1OrLater();
#if defined(_M_IX86) || defined(_M_X64)
    bool VirtualSseAvailable(const int sseLevel) const;
#endif
    BOOL SSE2Available() const;
#if defined(_M_IX86) || defined(_M_X64)
    BOOL SSE3Available() const;
    BOOL SSE4_1Available() const;
    BOOL SSE4_2Available() const;
    BOOL PopCntAvailable() const;
    BOOL LZCntAvailable() const;
    BOOL TZCntAvailable() const;
    bool IsAtomPlatform() const;
#endif
    bool IsLowMemoryProcess();
    BOOL GetAvailableCommit(ULONG64 *pCommit);
    void SetAvailableCommit(ULONG64 commit);
    uint32_t GetNumberOfLogicalProcessors() const { return this->dwNumberOfProcessors; }
    uint32_t GetNumberOfPhysicalProcessors() const { return this->dwNumberOfPhysicalProcessors; }

#if SYSINFO_IMAGE_BASE_AVAILABLE
    UINT_PTR GetChakraBaseAddr() const;
#endif

#if defined(_M_ARM32_OR_ARM64)
    bool ArmDivAvailable() const { return this->armDivAvailable; }
#endif
    static uint32_t SaveModuleFileName(HANDLE hMod);
    static LPCWSTR GetJscriptDllFileName();
    static HRESULT GetJscriptFileVersion(uint32_t* majorVersion, uint32_t* minorVersion, uint32_t *buildDateHash = nullptr, uint32_t *buildTimeHash = nullptr);
#if DBG
    static bool IsInitialized();
#endif
#if SYSINFO_IMAGE_BASE_AVAILABLE
    static bool IsJscriptModulePointer(void * ptr);
#endif
#if defined(__APPLE__) && defined(_M_ARM64)
    static uint32_t const PageSize = 16384;
#else
    static uint32_t const PageSize = 4096;
#endif

    static size_t const MaxPageCount = SIZE_MAX / PageSize;

#ifdef STACK_ALIGN
    static uint32_t const StackAlign = STACK_ALIGN;
#else
# if defined(TARGET_64)
    static uint32_t const StackAlign = 16;
# elif defined(_M_ARM)
    static uint32_t const StackAlign = 8;
# elif defined(_M_IX86)
    static uint32_t const StackAlign = 4;
# else
    # error missing_target
# endif
#endif

#if SYSINFO_IMAGE_BASE_AVAILABLE
    UINT_PTR dllLoadAddress;
    UINT_PTR dllHighAddress;
#endif

private:
    AutoSystemInfo() : majorVersion(0), minorVersion(0), buildDateHash(0), buildTimeHash(0), crtSize(0) { Initialize(); }
    void Initialize();
    bool isWindows8OrGreater;
    bool isWindows8Point1OrGreater;
    uint allocationGranularityPageCount;
    HANDLE processHandle;
    uint32_t crtSize;
#if defined(_M_IX86) || defined(_M_X64)
    int CPUInfo[4];
#endif
#if defined(_M_ARM32_OR_ARM64)
    bool armDivAvailable;
#endif
    uint32_t dwNumberOfPhysicalProcessors;

    bool disableDebugScopeCapture;
#if DBG
    bool initialized;
#endif

private:
#if defined(_M_IX86) || defined(_M_X64)
    bool isAtom;
    bool CheckForAtom() const;
#endif

    bool InitPhysicalProcessorCount();

    WCHAR binaryName[MAX_PATH + 1];
    uint32_t majorVersion;
    uint32_t minorVersion;
    uint32_t buildDateHash;
    uint32_t buildTimeHash;
    static HRESULT GetVersionInfo(LPCWSTR pszPath, uint32_t* majorVersion, uint32_t* minorVersion);

    static const uint32_t INVALID_VERSION = (uint32_t)-1;

    ULONG64 availableCommit;
    bool shouldQCMoreFrequently;
    bool supportsOnlyMultiThreadedCOM;
    bool isLowMemoryDevice;

public:
    static bool ShouldQCMoreFrequently();
    static bool SupportsOnlyMultiThreadedCOM();
    static bool IsLowMemoryDevice();
};


// For Prefast where it doesn't like symbolic constants
#if defined(__APPLE__) && defined(_M_ARM64)
CompileAssert(AutoSystemInfo::PageSize == 16384);
#define __in_ecount_pagesize __in_ecount(16384)
#define __in_ecount_twopagesize __in_ecount(32768)
#else
CompileAssert(AutoSystemInfo::PageSize == 4096);
#define __in_ecount_pagesize __in_ecount(4096)
#define __in_ecount_twopagesize __in_ecount(8192)
#endif
