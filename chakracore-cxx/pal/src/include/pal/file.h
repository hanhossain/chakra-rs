//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    include/pal/file.h

Abstract:
    Header file for file utility functions.

Revision History:



--*/

#ifndef _PAL_FILE_H_
#define _PAL_FILE_H_

#include "pal/shmemory.h"
#include <sys/types.h>
#include <dirent.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _find_handle
{
    struct _find_handle *self_addr; /* for pointer verification */

    char   dir[_MAX_DIR];
    char   fname[MAX_PATH_FNAME]; /* includes extension */
    char   **next;
} find_obj;

/*++
Function:
  FILEGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t FILEGetLastErrorFromErrno( void );

/*++
Function:
  DIRGetLastErrorFromErrno

Convert errno into the appropriate win32 error and return it.
--*/
uint32_t DIRGetLastErrorFromErrno( void );

/*++
FILEInitStdHandles

Create handle objects for stdin, stdout and stderr

(no parameters)

Return value:
    TRUE on success, FALSE on failure
--*/
BOOL FILEInitStdHandles(void);

/*++
FILECleanupStdHandles

Close promary handles for stdin, stdout and stderr

(no parameters, no return value)
--*/
void FILECleanupStdHandles(void);

/*++

Function :
    FILEGetProperNotFoundError

Returns the proper error code, based on the
Windows behavoir.

    IN char* lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError( char* lpPath, uint32_t * lpErrorCode );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_FILE_H_ */
