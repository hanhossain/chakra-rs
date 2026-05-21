//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    path.c

Abstract:

    Implementation of all functions related to path support

Revision History:



--*/

#include <string>
#include "pal/thread.hpp"
#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include <new>
#include "pal/stackstring.hpp"

#include <errno.h>

#include <unistd.h>
#include <alloca.h>

SET_DEFAULT_DEBUG_CHANNEL(FILE);


// In safemath.h, Template SafeInt uses macro _ASSERTE, which need to use variable
// defdbgchan defined by SET_DEFAULT_DEBUG_CHANNEL. Therefore, the include statement
// should be placed after the SET_DEFAULT_DEBUG_CHANNEL(FILE)
#include <safemath.h>

/*++
Function:
  FileDosToUnixPathA

Abstract:
  Change a DOS path to a Unix path.

  Replaces '\' by '/', removes any trailing dots on directory/filenames,
  and changes '*.*' to be equal to '*'

Parameter:
  IN/OUT lpPath: path to be modified
--*/
void
FILEDosToUnixPathA(
       char* lpPath)
{
    char* p;
    char* pPointAtDot=NULL;
    char charBeforeFirstDot='\0';

    TRACE("Original DOS path = [%s]\n", lpPath);

    if (!lpPath)
    {
        return;
    }

    for (p = lpPath; *p; p++)
    {
        /* Make the \\ to / switch first */
        if (*p == '\\')
        {
            /* Replace \ with / */
            *p = '/';
        }

        if (pPointAtDot)
        {
            /* If pPointAtDot is not NULL, it is pointing at the first encountered
               dot.  If we encountered a \, that means it could be a trailing dot */
            if (*p == '/')
            {
                /* If char before the first dot is a '\' or '.' (special case if the
                   dot is the first char in the path) , then we leave it alone,
                   because it is either . or .., otherwise it is a trailing dot
                   pattern and will be truncated */
                if (charBeforeFirstDot != '.' && charBeforeFirstDot != '/')
                {
                    memmove(pPointAtDot,p,(strlen(p)*sizeof(char))+1);
                    p = pPointAtDot;
                }
                pPointAtDot = NULL; /* Need to reset this */
            }
            else if (*p == '*')
            {
                /* Check our size before doing anything with our pointers */
                if ((p - lpPath) >= 3)
                {
                    /* At this point, we know that there is 1 or more dots and
                       then a star.  AND we know the size of our string at this
                       point is at least 3 (so we can go backwards from our pointer
                       safely AND there could possilby be two characters back)
                       So lets check if there is a '*' and a '.' before, if there
                       is, replace just a '*'.  Otherwise, reset pPointAtDot to NULL
                       and do nothing */
                    if (p[-2] == '*' &&
                        p[-1] == '.' &&
                        p[0] == '*')
                    {
                        memmove(&(p[-2]),p,(strlen(p)*sizeof(char))+1);
                    }

                    pPointAtDot  = NULL;
                }
            }
            else if (*p != '.')
            {
                /* If we are here, that means that this is NOT a trailing dot,
                   some other character is here, so forget our pointer */
                pPointAtDot = NULL;
            }
        }
        else
        {
            if (*p == '.')
            {
                /* If pPointAtDot is NULL, and we encounter a dot, save the pointer */
                pPointAtDot = p;
                if (pPointAtDot != lpPath)
                {
                    charBeforeFirstDot = p[-1];
                }
                else
                {
                    charBeforeFirstDot = lpPath[0];
                }
            }
        }
    }

    /* If pPointAtDot still points at anything, then we still have trailing dots.
       Truncate at pPointAtDot, unless the dots are path specifiers (. or ..) */
    if (pPointAtDot)
    {
        /* make sure the trailing dots don't follow a '/', and that they aren't
           the only thing in the name */
        if(pPointAtDot != lpPath && *(pPointAtDot-1) != '/')
        {
            *pPointAtDot = '\0';
        }
    }

    TRACE("Resulting Unix path = [%s]\n", lpPath);
}