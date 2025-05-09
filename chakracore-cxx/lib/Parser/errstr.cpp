//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "ParserPch.h"
#include "errstr.h"

// scaffolding - get a g_hInstance from scrbgase.cpp
HANDLE g_hInstance;

// Used as a prefix to generate the resource dll name.
const char16 g_wszPrefix[] = _u("js");

static BOOL FGetStringFromLibrary(HMODULE hlib, int istring, __out_ecount(cchMax) WCHAR * psz, int cchMax)
{
    // NOTE - istring is expected to be HRESULT

    Assert(0 < cchMax);
    Assert(psz);

    HGLOBAL hgl = NULL;
    WCHAR * pchRes = NULL;
    HRSRC hrsrc;
    WCHAR * pchCur;
    int cch;
    int cstring;
    uint32_t cbRes;
    int itable = ((uint16_t)istring >> 4) + 1;
    istring &= 0x0F;
    BOOL fRet = FALSE;

#ifdef ENABLE_GLOBALIZATION
    psz[0] = '\0';

    if (NULL == hlib)
        goto LError;

    hrsrc = FindResourceEx((HMODULE)hlib, RT_STRING, MAKEINTRESOURCE(itable), 0);
    if (NULL == hrsrc)
        goto LError;

    hgl = LoadResource((HMODULE)hlib, hrsrc);
    if (NULL == hgl)
        goto LError;

    pchRes = (WCHAR *)LockResource(hgl);
    if (NULL == pchRes)
        goto LError;

    cbRes = SizeofResource((HMODULE)hlib, hrsrc);

    if (cbRes < sizeof(uint16_t))
        goto LError;

    pchCur = pchRes;
    for (cstring = istring; cstring-- > 0;)
    {
        if (cbRes - sizeof(uint16_t) < sizeof(WCHAR) * (pchCur - pchRes))
            goto LError;

        cch = (*(uint16_t *) pchCur) + 1;

        if (cch <= 0)
            goto LError;

        if (cbRes < sizeof(WCHAR) * cch)
            goto LError;

        if (cbRes - sizeof(WCHAR) * cch < sizeof(WCHAR) * (pchCur - pchRes))
            goto LError;

        pchCur += cch;
    }

    if (cbRes - sizeof(uint16_t) < sizeof(WCHAR) * (pchCur - pchRes))
        goto LError;
    cch = * (uint16_t *) pchCur;

    if (cch <= 0)
        goto LError;

    if (cbRes < sizeof(WCHAR) * (cch + 1))
        goto LError;

    if (cbRes - sizeof(WCHAR) * (cch + 1) < sizeof(WCHAR) * (pchCur - pchRes))
        goto LError;

    if (cch > cchMax - 1)
        cch = cchMax - 1;

    js_memcpy_s(psz, cchMax * sizeof(WCHAR), pchCur + 1, cch * sizeof(WCHAR));
    psz[cch] = '\0';
    fRet = TRUE;

LError:

    //
    // Unlock/FreeResource non-essential on win32/64.
    //
    if (NULL != hgl)
    {
        if (NULL != pchRes)
            UnlockResource(hgl);
        FreeResource(hgl);
    }

#endif // ENABLE_GLOBALIZATION
    return fRet;
}


BOOL FGetResourceString(int32 isz, __out_ecount(cchMax) OLECHAR *psz, int cchMax)
{
    return FGetStringFromLibrary((HINSTANCE)g_hInstance, isz, psz, cchMax);
}

// Get a bstr version of the error string
// Don't inline. This function needs 2KB stack.
BSTR BstrGetResourceString(int32 isz)
{
    // NOTE - isz is expected to be HRESULT

    const char16* LoadResourceStr(uint32_t id);

    uint32_t id = (uint16_t)isz;
    const char16* szT = LoadResourceStr(id);
    if (!szT || !szT[0])
    {
        return NULL;
    }

    return SysAllocString(szT);
}
