//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/unicode_data.h

Abstract:

    Data, data retrieval function declarations.



--*/

#ifndef _UNICODE_DATA_H_
#define _UNICODE_DATA_H_

#include "pal/palinternal.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if !HAVE_COREFOUNDATION

typedef struct
{
  WCHAR nUnicodeValue;
  uint16_t  C1_TYPE_FLAGS;
  WCHAR nOpposingCase;             /* 0 if no opposing case. */
  uint16_t  rangeValue;
} UnicodeDataRec;

/* Global variables. */
extern const UnicodeDataRec UnicodeData[];
extern const uint32_t UNICODE_DATA_SIZE;
extern const uint32_t UNICODE_DATA_DIRECT_ACCESS;

/*++
Function:
  GetUnicodeData
  This function is used to get information about a Unicode character.

Parameters:
nUnicodeValue
  The numeric value of the Unicode character to get information about.
pDataRec
  The UnicodeDataRec to fill in with the data for the Unicode character.

Return value:
  TRUE if the Unicode character was found.

--*/
BOOL GetUnicodeData(int32_t nUnicodeValue, UnicodeDataRec *pDataRec);

#endif  /* !HAVE_COREFOUNDATION */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* _UNICODE_DATA_H_ */
