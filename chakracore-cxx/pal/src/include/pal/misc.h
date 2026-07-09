//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/misc.h

Abstract:
    Header file for the initialization and clean up functions
    for the misc Win32 functions



--*/

#ifndef __MISC_H_
#define __MISC_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*++
Function:
TIMEInitialize

Return value:
TRUE if initialize succeeded
FALSE otherwise

--*/
BOOL TIMEInitialize( void );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __MISC_H_ */

