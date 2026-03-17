//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "ParserPch.h"
#include "errstr.h"

// Used as a prefix to generate the resource dll name.
const char16_t g_wszPrefix[] = u"js";

static BOOL FGetStringFromLibrary(int istring, __out_ecount(cchMax) char16_t * psz, int cchMax)
{
    // NOTE - istring is expected to be int32_t

    Assert(0 < cchMax);
    Assert(psz);

    HGLOBAL hgl = NULL;
    char16_t * pchRes = NULL;
    HRSRC hrsrc;
    char16_t * pchCur;
    int cch;
    int cstring;
    uint32_t cbRes;
    int itable = ((uint16_t)istring >> 4) + 1;
    istring &= 0x0F;
    BOOL fRet = FALSE;

    return fRet;
}


BOOL FGetResourceString(int32_t isz, __out_ecount(cchMax) OLECHAR *psz, int cchMax)
{
    return FGetStringFromLibrary(isz, psz, cchMax);
}

// Get a bstr version of the error string
// Don't inline. This function needs 2KB stack.
BSTR BstrGetResourceString(int32_t isz)
{
    // NOTE - isz is expected to be int32_t

    const char16_t* LoadResourceStr(uint32_t id);

    uint32_t id = (uint16_t)isz;
    const char16_t* szT = LoadResourceStr(id);
    if (!szT || !szT[0])
    {
        return NULL;
    }

    return SysAllocString(szT);
}
