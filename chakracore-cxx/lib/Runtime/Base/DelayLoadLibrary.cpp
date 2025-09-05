//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

namespace Js
{
    int32_t DelayLoadWinRtString::WindowsCreateString(_In_reads_opt_(length) const char16_t * sourceString, uint32_t length, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING * string)
    {
        if (m_hModule)
        {
            if (m_pfnWindowsCreateString == nullptr)
            {
                m_pfnWindowsCreateString = (PFNCWindowsCreateString)GetFunction("WindowsCreateString");
                if (m_pfnWindowsCreateString == nullptr)
                {
                    *string = nullptr;
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnWindowsCreateString != nullptr);
            return m_pfnWindowsCreateString(sourceString, length, string);
        }

        *string = nullptr;
        return E_NOTIMPL;
    }

    int32_t DelayLoadWinRtString::WindowsCreateStringReference(_In_reads_opt_(length + 1) const char16_t *sourceString, uint32_t length, _Out_ HSTRING_HEADER *hstringHeader, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING * string)
    {
        if (m_hModule)
        {
            if (m_pfnWindowsCreateStringReference == nullptr)
            {
                m_pfnWindowsCreateStringReference = (PFNCWindowsCreateStringReference)GetFunction("WindowsCreateStringReference");
                if (m_pfnWindowsCreateStringReference == nullptr)
                {
                    *string = nullptr;
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnWindowsCreateStringReference != nullptr);
            return m_pfnWindowsCreateStringReference(sourceString, length, hstringHeader, string);
        }

        *string = nullptr;
        return E_NOTIMPL;
    }

    int32_t DelayLoadWinRtString::WindowsDeleteString(_In_opt_ HSTRING string)
    {
        if (m_hModule)
        {
            if (m_pfnWindowsDeleteString == nullptr)
            {
                m_pfnWindowsDeleteString = (PFNCWindowsDeleteString)GetFunction("WindowsDeleteString");
                if (m_pfnWindowsDeleteString == nullptr)
                {
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnWindowsDeleteString != nullptr);
            int32_t hr = m_pfnWindowsDeleteString(string);
            Assert(SUCCEEDED(hr));
            return hr;
        }

        return E_NOTIMPL;
    }

    PCWSTR DelayLoadWinRtString::WindowsGetStringRawBuffer(_In_opt_ HSTRING string, _Out_opt_ uint32_t * length)
    {
        if (m_hModule)
        {
            if (m_pfWindowsGetStringRawBuffer == nullptr)
            {
                m_pfWindowsGetStringRawBuffer = (PFNCWindowsGetStringRawBuffer)GetFunction("WindowsGetStringRawBuffer");
                if (m_pfWindowsGetStringRawBuffer == nullptr)
                {
                    if (length)
                    {
                        *length = 0;
                    }
                    return u"\0";
                }
            }

            Assert(m_pfWindowsGetStringRawBuffer != nullptr);
            return m_pfWindowsGetStringRawBuffer(string, length);
        }

        if (length)
        {
            *length = 0;
        }
        return u"\0";
    }

    int32_t DelayLoadWinRtString::WindowsCompareStringOrdinal(_In_opt_ HSTRING string1, _In_opt_ HSTRING string2, _Out_ int32_t * result)
    {
        if (m_hModule)
        {
            if (m_pfnWindowsCompareStringOrdinal == nullptr)
            {
                m_pfnWindowsCompareStringOrdinal = (PFNCWindowsCompareStringOrdinal)GetFunction("WindowsCompareStringOrdinal");
                if (m_pfnWindowsCompareStringOrdinal == nullptr)
                {
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnWindowsCompareStringOrdinal != nullptr);
            return m_pfnWindowsCompareStringOrdinal(string1,string2,result);
        }

        return E_NOTIMPL;
    }
    int32_t DelayLoadWinRtString::WindowsDuplicateString(_In_opt_ HSTRING original, _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING *newString)
    {
        if(m_hModule)
        {
            if(m_pfnWindowsDuplicateString == nullptr)
            {
                m_pfnWindowsDuplicateString = (PFNCWindowsDuplicateString)GetFunction("WindowsDuplicateString");
                if(m_pfnWindowsDuplicateString == nullptr)
                {
                    *newString = nullptr;
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnWindowsDuplicateString != nullptr);
            return m_pfnWindowsDuplicateString(original, newString);
        }
        *newString = nullptr;
        return E_NOTIMPL;
    }

    int32_t DelayLoadWinRtFoundation::RoGetActivationFactory(
        HSTRING activatableClassId,
        REFIID iid,
        IActivationFactory** factory)
    {
        if (m_hModule)
        {
            if (m_pfnFNCWRoGetActivationFactory == nullptr)
            {
                m_pfnFNCWRoGetActivationFactory = (PFNCWRoGetActivationFactory)GetFunction("RoGetActivationFactory");
                if (m_pfnFNCWRoGetActivationFactory == nullptr)
                {
                    return E_UNEXPECTED;
                }
            }

            Assert(m_pfnFNCWRoGetActivationFactory != nullptr);
            return m_pfnFNCWRoGetActivationFactory(activatableClassId, iid, factory);
        }

        return E_NOTIMPL;
    }

    BOOL DelayLoadWinCoreProcessThreads::GetProcessInformation(
        HANDLE hProcess,
        PROCESS_INFORMATION_CLASS ProcessInformationClass,
        __out_bcount(nLength) void * lpBuffer,
        SIZE_T nLength
    )
    {
#if defined(DELAYLOAD_SET_CFG_TARGET) || defined(_M_ARM)
        if (m_hModule)
        {
            if (m_pfnGetProcessInformation == nullptr)
            {
                m_pfnGetProcessInformation = (PFNCGetProcessInformation)GetFunction("GetProcessInformation");
                if (m_pfnGetProcessInformation == nullptr)
                {
                    return FALSE;
                }
            }

            Assert(m_pfnGetProcessInformation != nullptr);
            return m_pfnGetProcessInformation(hProcess, ProcessInformationClass, lpBuffer, nLength);
        }
#endif
        return FALSE;
    }
}
