//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/common.h

Abstract:
    Header file for common helper functions in the map module.



--*/

#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****
 *
 * W32toUnixAccessControl( uint32_t ) - Maps Win32 to Unix memory access controls .
 *
 */
int32_t W32toUnixAccessControl(  uint32_t flProtect );

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H_ */




