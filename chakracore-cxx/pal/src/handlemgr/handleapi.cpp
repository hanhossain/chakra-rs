//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    handleapi.cpp

Abstract:

    Implementation of the handle management APIs



--*/

#include "pal/handleapi.hpp"
#include "pal/handlemgr.hpp"
#include "pal/thread.hpp"
#include "pal/procobj.hpp"
#include "pal/dbgmsg.h"
#include "pal/process.h"
#include "chakra/Logger.h"
#include <format>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(HANDLE);

CAllowedObjectTypes aotDuplicateHandle __attribute__((init_priority(200)))(TRUE);

PAL_ERROR
CloseSpecialHandle(
    HANDLE hObject
    );

/*++
Function:
  CloseHandle

See MSDN doc.

Note : according to MSDN, FALSE is returned in case of error. But also
according to MSDN, closing an invalid handle raises an exception when running a
debugger [or, alternately, if a special registry key is set]. This behavior is
not required in the PAL, so we'll always return FALSE.
--*/
BOOL
CloseHandle(
          HANDLE hObject)
{

    CPalThread *pThread = InternalGetCurrentThread();
    PAL_ERROR palError = InternalCloseHandle(pThread, hObject);

    if (NO_ERROR != palError)
    {
        CorUnix::CPalThread::SetLastError(palError);
    }

    LOGEXIT("CloseHandle returns BOOL %d\n", (NO_ERROR == palError));
    return (NO_ERROR == palError);
}

PAL_ERROR
CorUnix::InternalCloseHandle(
    CPalThread * pThread,
    HANDLE hObject
    )
{
    PAL_ERROR palError = NO_ERROR;

    if (!HandleIsSpecial(hObject))
    {
        palError = g_pObjectManager->RevokeHandle(
            pThread,
            hObject
            );
    }
    else
    {
        palError = CloseSpecialHandle(hObject);
    }

    return palError;
}

PAL_ERROR
CloseSpecialHandle(
    HANDLE hObject
    )
{
    if (hObject == hPseudoCurrentThread)
    {
        return NO_ERROR;
    }

    return ERROR_INVALID_HANDLE;
}

