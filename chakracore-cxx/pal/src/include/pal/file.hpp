//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/file.hpp

Abstract:
    Header file for file utility functions.

Revision History:



--*/

#ifndef _PAL_FILE_HPP_
#define _PAL_FILE_HPP_

#include "corunix.hpp"

#include <sys/types.h>
#include <sys/param.h>
#include <dirent.h>


namespace CorUnix
{
    extern CObjectType otFile;
    extern CAllowedObjectTypes aotFile;

    class CFileProcessLocalData
    {
    public:
        int  unix_fd;
        uint32_t dwDesiredAccess; /* Unix assumes files are always opened for reading.
                                  In Windows we can open a file for writing only */
        int  open_flags;       /* stores Unix file creation flags */
        BOOL open_flags_deviceaccessonly;
        char unix_filename[MAXPATHLEN];
        BOOL inheritable;
    };

    /*++
    InternalOpen
    Wraps open
    --*/
    int
    InternalOpen(
        const char *szFilename,
        int nFlags,
        ...
        );
}

#endif /* _PAL_FILE_HPP_ */

