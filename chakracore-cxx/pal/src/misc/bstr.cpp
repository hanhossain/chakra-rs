//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    misc/bstr.cpp

Abstract:

    Implementation of bstr related functionality

--*/

#include "pal/palinternal.h"

// Redefining here to not have to pull in all of palrt.h in order to get intsafe
#define S_OK ((HRESULT)0x00000000L)
#define E_INVALIDARG ((HRESULT) 0x80070057L)
#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)
#define NULL    0

#include "pal_assert.h"
#include "rt/intsafe.h"

#define WIN32_ALLOC_ALIGN (16 - 1)

size_t PAL_wcslen(const WCHAR *);

typedef WCHAR OLECHAR;
typedef WCHAR *BSTR;

inline HRESULT CbSysStringSize(uint32_t cchSize, BOOL isByteLen, uint32_t *result)
{
    if (result == nullptr)
        return E_INVALIDARG;

    // +2 for the null terminator
    // + DWORD_PTR to store the byte length of the string
    int constant = sizeof(WCHAR) + sizeof(DWORD_PTR) + WIN32_ALLOC_ALIGN;

    if (isByteLen)
    {
        if (SUCCEEDED(ULongAdd(constant, cchSize, result)))
        {
            *result = *result & ~WIN32_ALLOC_ALIGN;
            return NOERROR;
        }
    }
    else
    {
        uint32_t temp = 0; // should not use in-place addition in ULongAdd
        if (SUCCEEDED(ULongMult(cchSize, sizeof(WCHAR), &temp)) &
            SUCCEEDED(ULongAdd(temp, constant, result)))
        {
            *result = *result & ~WIN32_ALLOC_ALIGN;
            return NOERROR;
        }
    }
    return INTSAFE_E_ARITHMETIC_OVERFLOW;
}



/***
 *BSTR SysAllocStringLen(char*, unsigned int)
 *Purpose:
 *  Allocation a bstr of the given length and initialize with
 *  the pasted in string
 *
 *Entry:
 *  [optional]
 *
 *Exit:
 *  return value = BSTR, NULL if the allocation failed.
 *
 ***********************************************************************/
extern "C" BSTR SysAllocStringLen(const OLECHAR *psz, uint32_t len)
{

    BSTR bstr;
    uint32_t cbTotal = 0;

    if (FAILED(CbSysStringSize(len, FALSE, &cbTotal)))
        return NULL;

    bstr = (OLECHAR *)HeapAlloc(GetProcessHeap(), 0, cbTotal);

    if(bstr != NULL) {

        // NOTE: There are some apps which peek back 4 bytes to look at
        // the size of the BSTR. So, in case of 64-bit code,
        // we need to ensure that the BSTR length can be found by
        // looking one uint32_t before the BSTR pointer.
        *(DWORD_PTR *)bstr = (DWORD_PTR) 0;
        bstr = (BSTR) ((char *) bstr + sizeof (uint32_t));
        *(uint32_t *)bstr = (uint32_t)len * sizeof(OLECHAR);

        bstr = (BSTR) ((char*) bstr + sizeof(uint32_t));

        if(psz != NULL){
            memcpy(bstr, psz, len * sizeof(OLECHAR));
        }

        bstr[len] = '\0'; // always 0 terminate
    }

    return bstr;
}

/***
 *void SysFreeString(BSTR)
 *Purpose:
 *  Free a bstr using the passed in string
 *
 *Entry:
 *  BSTR to free
 *
 *Exit:
 *  return value = void
 *
 ***********************************************************************/
extern "C" void SysFreeString(BSTR bstr)
{
    if (bstr != NULL)
    {
        bstr = (BSTR) ((char*) bstr - sizeof(uint32_t));
        bstr = (BSTR) ((char*) bstr - sizeof(uint32_t));
        HeapFree(GetProcessHeap(), 0, (void *) bstr);
    }
}

/***
 *uint32_t SysStringLen(BSTR)
 *Purpose:
 *  Return the length of the string in characters (not including null terminator)
 *
 *Entry:
 *  BSTR whose length to return
 *
 *Exit:
 *  return value = length of the string
 *
 ***********************************************************************/
extern "C" uint32_t SysStringLen(BSTR bstr)
{
    if (bstr == NULL)
    {
        return 0;
    }

    return (uint32_t)((((uint32_t *)bstr)[-1]) / sizeof(OLECHAR));
}

/***
 *BSTR SysAllocString(char*)
 *Purpose:
 *  Allocation a bstr using the passed in string
 *
 *Entry:
 *  String to create a bstr for
 *
 *Exit:
 *  return value = BSTR, NULL if allocation failed
 *
 ***********************************************************************/
extern "C" BSTR SysAllocString(const OLECHAR* psz)
{
    if(psz == NULL)
    {
        return NULL;
    }

    return SysAllocStringLen(psz, (uint32_t)PAL_wcslen(psz));
}
