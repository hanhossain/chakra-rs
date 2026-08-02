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

// In safemath.h, Template SafeInt uses macro assert, which need to use variable
// defdbgchan defined by SET_DEFAULT_DEBUG_CHANNEL. Therefore, the include statement
// should be placed after the SET_DEFAULT_DEBUG_CHANNEL(MISC)
#include <safemath.h>

/*++
Function:
  UTIL_inverse_wcspbrk

  Opposite of wcspbrk : searches a string for the first character NOT in the 
  given set

Parameters :
    char16_t* lpwstr :   string to search
    const char16_t* charset : list of characters to search for
                                      
Return value :
    pointer to first character of lpwstr that isn't in the set
    NULL if all characters are in the set                                                                 
--*/
char16_t* UTIL_inverse_wcspbrk(char16_t* lpwstr, const char16_t* charset)
{
    while(*lpwstr)
    {
        if(NULL == PAL_wcschr(charset,*lpwstr))
        {
            return lpwstr;
        }
        lpwstr++;
    }                     
    return NULL;
}


/*++
Function : 
    UTIL_IsReadOnlyBitsSet
    
    Takes a struct stat *
    Returns true if the file is read only,
--*/
BOOL UTIL_IsReadOnlyBitsSet( struct stat * stat_data )
{
    BOOL bRetVal = FALSE;

    /* Check for read permissions. */
    if ( stat_data->st_uid == geteuid() )
    {
        /* The process owner is the file owner as well. */
        if ( ( stat_data->st_mode & S_IRUSR ) && !( stat_data->st_mode & S_IWUSR ) )
        {
            bRetVal = TRUE;
        }
    }
    else if ( stat_data->st_gid == getegid() )
    {
        /* The process's owner is in the same group as the file's owner. */
        if ( ( stat_data->st_mode & S_IRGRP ) && !( stat_data->st_mode & S_IWGRP ) )
        {
            bRetVal = TRUE;
        }
    }
    else
    {
        /* Check the other bits to see who can access the file. */
        if ( ( stat_data->st_mode & S_IROTH ) && !( stat_data->st_mode & S_IWOTH ) )
        {
            bRetVal = TRUE;
        }
    }

    return bRetVal;
}

/*++
Function : 
    UTIL_IsExecuteBitsSet
    
    Takes a struct stat *
    Returns true if the file is executable,
--*/
BOOL UTIL_IsExecuteBitsSet( struct stat * stat_data )
{
    BOOL bRetVal = FALSE;

    if ( (stat_data->st_mode & S_IFMT) == S_IFDIR )
    {
        return FALSE;
    }
    
    /* Check for read permissions. */
    if ( stat_data->st_uid == geteuid() )
    {
        /* The process owner is the file owner as well. */
        if ( ( stat_data->st_mode & S_IXUSR ) )
        {
            bRetVal = TRUE;
        }
    }
    else if ( stat_data->st_gid == getegid() )
    {
        /* The process's owner is in the same group as the file's owner. */
        if ( ( stat_data->st_mode & S_IXGRP ) )
        {
            bRetVal = TRUE;
        }
    }
    else
    {
        /* Check the other bits to see who can access the file. */
        if ( ( stat_data->st_mode & S_IXOTH ) )
        {
            bRetVal = TRUE;
        }
    }

    return bRetVal;
}

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
