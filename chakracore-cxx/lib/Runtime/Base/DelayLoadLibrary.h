//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

//#include <roapi.h>
#include "activation.h"
#include <winstring.h>

#include "RoParameterizedIID.h"

namespace Js
{
    class DelayLoadWinRtString : public DelayLoadLibrary
    {
    private:
        // WinRTString specific functions
        typedef HRESULT FNCWindowsCreateString(const char16_t *, uint32_t, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING *);
        typedef FNCWindowsCreateString* PFNCWindowsCreateString;
        PFNCWindowsCreateString m_pfnWindowsCreateString;

        typedef HRESULT FNCWindowsCreateStringReference(const char16_t *, uint32_t, HSTRING_HEADER *, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING *);
        typedef FNCWindowsCreateStringReference* PFNCWindowsCreateStringReference;
        PFNCWindowsCreateStringReference m_pfnWindowsCreateStringReference;

        typedef PCWSTR FNCWindowsGetStringRawBuffer(HSTRING, uint32_t*);
        typedef FNCWindowsGetStringRawBuffer* PFNCWindowsGetStringRawBuffer;
        PFNCWindowsGetStringRawBuffer m_pfWindowsGetStringRawBuffer;

        typedef HRESULT FNCWindowsDeleteString(HSTRING);
        typedef FNCWindowsDeleteString* PFNCWindowsDeleteString;
        PFNCWindowsDeleteString m_pfnWindowsDeleteString;

        typedef HRESULT FNCWindowsCompareStringOrdinal(HSTRING,HSTRING,int32_t*);
        typedef FNCWindowsCompareStringOrdinal* PFNCWindowsCompareStringOrdinal;
        PFNCWindowsCompareStringOrdinal m_pfnWindowsCompareStringOrdinal;

        typedef HRESULT FNCWindowsDuplicateString(HSTRING, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING*);
        typedef FNCWindowsDuplicateString* PFNCWindowsDuplicateString;
        PFNCWindowsDuplicateString m_pfnWindowsDuplicateString;

    public:
        DelayLoadWinRtString() : DelayLoadLibrary(),
            m_pfnWindowsCreateString(NULL),
            m_pfWindowsGetStringRawBuffer(NULL),
            m_pfnWindowsDeleteString(NULL),
            m_pfnWindowsCreateStringReference(NULL),
            m_pfnWindowsDuplicateString(NULL),
            m_pfnWindowsCompareStringOrdinal(NULL) { }

        virtual ~DelayLoadWinRtString() { }

        LPCTSTR GetLibraryName() const { return u"api-ms-win-core-winrt-string-l1-1-0.dll"; }

        virtual HRESULT WindowsCreateString(_In_reads_opt_(length) const char16_t * sourceString, uint32_t length, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING * string);
        virtual HRESULT WindowsCreateStringReference(_In_reads_opt_(length + 1) const char16_t * sourceString, uint32_t length, _Out_ HSTRING_HEADER * header, _Outptr_result_maybenull_ _Result_nullonfailure_  HSTRING * string);
        virtual HRESULT WindowsDeleteString(_In_opt_ HSTRING string);
        virtual PCWSTR WindowsGetStringRawBuffer(_In_opt_ HSTRING string, _Out_opt_ uint32_t * length);
        virtual HRESULT WindowsCompareStringOrdinal(_In_opt_ HSTRING string1, _In_opt_ HSTRING string2, _Out_ int32_t * result);
        virtual HRESULT WindowsDuplicateString(_In_opt_ HSTRING original, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING * newString);
    };

    class DelayLoadWinRtFoundation sealed : public DelayLoadLibrary
    {
    private:

        // DelayLoadWindowsFoundation specific functions
        typedef HRESULT FNCWRoGetActivationFactory(HSTRING clsid, REFIID iid, IActivationFactory** factory);

        typedef FNCWRoGetActivationFactory* PFNCWRoGetActivationFactory;
        PFNCWRoGetActivationFactory m_pfnFNCWRoGetActivationFactory;

    public:
        DelayLoadWinRtFoundation() : DelayLoadLibrary(),
            m_pfnFNCWRoGetActivationFactory(nullptr) { }

        virtual ~DelayLoadWinRtFoundation() { }

        LPCTSTR GetLibraryName() const { return u"api-ms-win-core-winrt-l1-1-0.dll"; }

        HRESULT RoGetActivationFactory(
            HSTRING activatibleClassId,
            REFIID iid,
            IActivationFactory** factory);
    };

    class DelayLoadWinCoreProcessThreads sealed : public DelayLoadLibrary
    {
    private:
        // LoadWinCoreMemory specific functions

        typedef BOOL FNCGetProcessInformation(HANDLE, PROCESS_INFORMATION_CLASS, void *, SIZE_T);
        typedef FNCGetProcessInformation* PFNCGetProcessInformation;
        PFNCGetProcessInformation m_pfnGetProcessInformation;

    public:
        DelayLoadWinCoreProcessThreads() :
            DelayLoadLibrary(),
            m_pfnGetProcessInformation(nullptr)
        {
        }

        LPCTSTR GetLibraryName() const { return u"api-ms-win-core-processthreads-l1-1-3.dll"; }

        BOOL GetProcessInformation(
            HANDLE hProcess,
            PROCESS_INFORMATION_CLASS ProcessInformationClass,
            __out_bcount(nLength) void * lpBuffer,
            SIZE_T nLength
        );
    };
 }
