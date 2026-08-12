//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    misc/utils.c

Abstract:

    Miscellaneous helper functions for the PAL, which don't fit anywhere else



--*/

#include "pal/palinternal.h"
#if defined(__APPLE__)
#include <mach/message.h>
#endif //defined(__APPLE__)

#include "pal/utils.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "chakra/Logger.h"
#include <format>

#include <errno.h>
#include <string.h>


SET_DEFAULT_DEBUG_CHANNEL(MISC);


#if defined(__APPLE__)
/*++
Function:
  UTIL_MachErrorToPalError

    Maps a Mach kern_return_t to a Win32 error code.
--*/
uint32_t UTIL_MachErrorToPalError(kern_return_t MachReturn)
{
    switch (MachReturn)
    {
    case KERN_SUCCESS:
        return ERROR_SUCCESS;

    case KERN_NO_ACCESS:
    case KERN_INVALID_CAPABILITY:
        return ERROR_ACCESS_DENIED;

    case KERN_TERMINATED:
        return ERROR_INVALID_HANDLE;

    case KERN_INVALID_ADDRESS:
        return ERROR_INVALID_ADDRESS;

    case KERN_NO_SPACE:
        return ERROR_NOT_ENOUGH_MEMORY;

    case KERN_INVALID_ARGUMENT:
        return ERROR_INVALID_PARAMETER;

    default:
        chakra::Logger::error(std::format("Unknown kern_return_t value {} - reporting ERROR_INTERNAL_ERROR\n", MachReturn));
        return ERROR_INTERNAL_ERROR;
    }
}

/*++
Function:
  UTIL_SetLastErrorFromMach

    Sets Win32 LastError according to the argument Mach kern_return_t value,
    provided it indicates an error.  If the argument indicates success, does
    not modify LastError.
--*/
void UTIL_SetLastErrorFromMach(kern_return_t MachReturn)
{
    uint32_t palError = UTIL_MachErrorToPalError(MachReturn);
    if (palError != ERROR_SUCCESS)
    {
        SetLastError(palError);
    }
}
#endif //defined(__APPLE__)
