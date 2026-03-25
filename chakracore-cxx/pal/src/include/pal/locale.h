//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    locale.h

Abstract:

    Prototypes for codepage initialization, and control of the readwrite locks
    for systems that use them.

Revision History:



--*/

#ifndef _PAL_LOCALE_H_
#define _PAL_LOCALE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define ISO_NAME(region, encoding, part)  region ".ISO" encoding "-" part

#if defined(__APPLE__)
#define CF_EXCLUDE_CSTD_HEADERS
#include <CoreFoundation/CoreFoundation.h>
#endif  // defined(__APPLE__)

#if defined(__APPLE__)
BOOL LocaleInitialize( void );
void LocaleCleanup( void );

typedef
struct _CP_MAPPING
{
    uint32_t                nCodePage;      /* Code page identifier. */
    CFStringEncoding    nCFEncoding;    /* The equivalent CFString encoding. */
    uint32_t                nMaxByteSize;   /* The max byte size of any character. */
    uint8_t                LeadByte[ MAX_LEADBYTES ];  /* The lead byte array. */
} CP_MAPPING;
#else
typedef 
struct _CP_MAPPING
{
    uint32_t    nCodePage;                  // Code page identifier.
    const char *  lpBSDEquivalent;            // The equivalent BSD locale identifier.
    uint32_t    nMaxByteSize;               // The max byte size of any character.
    uint8_t    LeadByte[ MAX_LEADBYTES ];  // The lead byte array.
} CP_MAPPING;
#endif
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_LOCALE_H_ */
