//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    include/pal/debug.h

Abstract:

    Debug API utility functions



--*/

#ifndef _PAL_DEBUG_H_
#define _PAL_DEBUG_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*++
Function :
    DBG_DebugBreak

    Processor-dependent implementation of DebugBreak

(no parameters, no return value)
--*/
extern "C" void
DBG_DebugBreak();

/*++
Function:
  IsInDebugBreak(addr)

  Returns true if the address is in DBG_DebugBreak.

--*/
BOOL
IsInDebugBreak(void *addr);

/*++
Function :
    DBG_FlushInstructionCache

    Processor-dependent implementation of FlushInstructionCache

Parameters :
    const void * lpBaseAddress: start of region to flush
    SIZE_T dwSize : length of region to flush

Return value :
    TRUE on success, FALSE on failure

--*/
BOOL
DBG_FlushInstructionCache(
                       const void * lpBaseAddress,
                       SIZE_T dwSize);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //PAL_DEBUG_H_
