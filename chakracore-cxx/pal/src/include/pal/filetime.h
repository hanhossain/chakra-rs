//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/filetime.h

Abstract:

    Header file for utility functions having to do with file times.

Revision History:



--*/

#ifndef _PAL_FILETIME_H_
#define _PAL_FILETIME_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/* Provide consistent access to nanosecond fields, if they exist. */

#if defined(__APPLE__)

#define ST_ATIME_NSEC(statstruct) ((statstruct)->st_atimespec.tv_nsec)
#define ST_MTIME_NSEC(statstruct) ((statstruct)->st_mtimespec.tv_nsec)
#define ST_CTIME_NSEC(statstruct) ((statstruct)->st_ctimespec.tv_nsec)

#else /* defined(__APPLE__) */

#define ST_ATIME_NSEC(statstruct) 0
#define ST_MTIME_NSEC(statstruct) 0
#define ST_CTIME_NSEC(statstruct) 0

#endif /* defined(__APPLE__) */

FILETIME FILEUnixTimeToFileTime( time_t sec, long nsec );
time_t FILEFileTimeToUnixTime( FILETIME FileTime, long *nsec );

#ifdef __APPLE__
#include <CoreFoundation/CFDate.h>

FILETIME FILECFAbsoluteTimeToFileTime( CFAbsoluteTime sec );
#endif // __APPLE__

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_FILE_H_ */











