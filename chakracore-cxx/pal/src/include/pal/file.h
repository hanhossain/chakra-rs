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
FILECanonicalizePath
    Removes all instances of '/./', '/../' and '//' from an absolute path.

Parameters:
    LPSTR lpUnixPath : absolute path to modify, in Unix format

(no return value)

Notes :
-behavior is undefined if path is not absolute
-the order of steps *is* important: /one/./../two would give /one/two
 instead of /two if step 3 was done before step 2
-reason for this function is that GetFullPathName can't use realpath(), since
 realpath() requires the given path to be valid and GetFullPathName does not.
--*/
void FILECanonicalizePath(LPSTR lpUnixPath);

/*++
Function:
  FileDosToUnixPathA

Abstract:
  Change a DOS path to a Unix path. Replace '\' by '/'.

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEDosToUnixPathA(LPSTR lpPath);

/*++
Function:
  FileDosToUnixPathW

Abstract:
  Change a DOS path to a Unix path. Replace '\' by '/'.

Parameter:
  IN/OUT lpPath: path to be modified
  --*/
void
FILEDosToUnixPathW(LPWSTR lpPath);

/*++
Function:
  FileUnixToDosPathA

Abstract:
  Change a Unix path to a DOS path. Replace '/' by '\'.

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEUnixToDosPathA(LPSTR lpPath);

/*++
Function:
  FILEGetDirectoryFromFullPathA

Parse the given path. If it contains a directory part and a file part,
put the directory part into the supplied buffer, and return the number of
characters written to the buffer. If the buffer is not large enough,
return the required size of the buffer including the NULL character. If
there is no directory part in the path, return 0.
--*/
uint32_t FILEGetDirectoryFromFullPathA( LPCSTR lpFullPath,
                     uint32_t  nBufferLength,
                     LPSTR  lpBuffer );

/*++
Function:
  FILEGetFileNameFromFullPath

Given a full path, return a pointer to the first char of the filename part.
--*/
LPCSTR FILEGetFileNameFromFullPathA( LPCSTR lpFullPath );

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

    IN LPSTR lpPath - The path to check.
    uint32_t * lpErrorCode - The error to set.
*/
void FILEGetProperNotFoundError( LPSTR lpPath, uint32_t * lpErrorCode );

/*++
PAL_fflush

Calls fflush

Input parameters:

PAL_FILE *stream = stream to be flushed.

Return value:
    0 is returned on success, otherwise EOF is returned.
--*/
int PAL_fflush( PAL_FILE *stream );

/*++
PAL_mkstemp

Calls InternalMkstemp to call mkstemp

Input parameters:

char *szNameTemplate = the pattern to follow when creating a new file.

Return value:
    file descriptor of opened file on success, -1 on failure.
--*/
int PAL_mkstemp(char *szNameTemplate);

/*++
PAL_unlink

Calls unlink.

Input parameters:

szPath = a symbolic link or a hard link to a file

Return value:
    Returns 0 on success and -1 on failure
--*/
int PAL_unlink(const char *szPath);

/*++
PAL_rename

Calls rename

Input parameters:

szOldName = pointer to the pathname of the file to be renamed
szNewName = pointer to the new pathname of the file

Return value:
    Returns 0 on success and -1 on failure
--*/
int PAL_rename(const char *szOldName, const char *szNewName);

/*++
PAL_fgets

Wrapper function for InternalFgets.

Input parameters:

sz = stores characters read from the given file stream
nSize = number of characters to be read
pf = stream to read characters from

Return value:
    Returns a pointer to the string storing the characters on success
    and NULL on failure.
--*/
char * PAL_fgets(char *sz, int nSize, PAL_FILE *pf);

/*++
PAL_fwrite

Wrapper function for InternalFwrite

Input parameters:

pvBuffer = array of objects to write to the given file stream
nSize = size of a object in bytes
nCount = number of objects to write
pf = stream to write characters to

Return value:
    Returns the number of objects written.
--*/
size_t PAL_fwrite(const void *pvBuffer, size_t nSize, size_t nCount, PAL_FILE *pf);

/*++
PAL__open

Wrapper function for InternalOpen.

Input parameters:

szPath = pointer to a pathname of a file to be opened
nFlags = arguments that control how the file should be accessed
mode = file permission settings that are used only when a file is created

Return value:
    File descriptor on success, -1 on failure
--*/
int PAL__open(const char *szPath, int nFlags, ...);

/*++
PAL_fseek

Wrapper function for fseek

Input parameters:

pf = a given file stream
lOffset = distance from position to set file-position indicator
nWhence = method used to determine the file_position indicator location relative to lOffset

Return value:
    0 on success, -1 on failure.
--*/
int PAL_fseek(PAL_FILE *pf, int32_t lOffset, int nWhence);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_FILE_H_ */
