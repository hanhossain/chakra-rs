//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++





--*/

#include "pal/dbgmsg.h"
#include "pal/thread.hpp"
#include "../thread/procprivate.hpp"
#include "pal/module.h"

using namespace CorUnix;

#ifdef FEATURE_PAL_SXS

SET_DEFAULT_DEBUG_CHANNEL(SXS);

PAL_ERROR AllocatePalThread(CPalThread **ppThread);

/************************* Enter *************************/

/*++
Function:
  CreateCurrentThreadData

Abstract:
  This function is called by the InternalGetOrCreateCurrentThread inlined
  function to create the thread data when it is null meaning the thread has
  never been in this PAL.

Warning:
  If the allocation fails, this function asserts and exits the process.
--*/
extern "C" CPalThread *
CreateCurrentThreadData()
{
    CPalThread *pThread = NULL;

    if (PALIsThreadDataInitialized()) {
        PAL_ERROR palError = AllocatePalThread(&pThread);
        if (NO_ERROR != palError)
        {
            ASSERT("Unable to allocate pal thread: error %d - aborting\n", palError);
            abort();
        }
    }

    return pThread;
}

PAL_ERROR
AllocatePalThread(CPalThread **ppThread)
{
    CPalThread *pThread = NULL;

    PAL_ERROR palError = CreateThreadData(&pThread);
    if (NO_ERROR != palError)
    {
        goto exit;
    }

    HANDLE hThread;
    palError = CreateThreadObject(pThread, pThread, &hThread);
    if (NO_ERROR != palError)
    {
        pthread_setspecific(thObjKey, NULL);
        pThread->ReleaseThreadReference();
        goto exit;
    }

    // Like CreateInitialProcessAndThreadObjects, we do not need this
    // thread handle, since we're not returning it to anyone who will
    // possibly release it.
    (void)g_pObjectManager->RevokeHandle(pThread, hThread);

    PROCAddThread(pThread, pThread);

exit:
    *ppThread = pThread;
    return palError;
}

PAL_ERROR CPalThread::Enter(PAL_Boundary /* boundary */)
{
    if (m_fInPal)
    {
        WARN("Enter called on a thread that already runs in this PAL\n");
        return NO_ERROR;
    }
    m_fInPal = TRUE;

    return ERROR_SUCCESS;
}


/************************* Leave *************************/

PAL_ERROR CPalThread::Leave(PAL_Boundary /* boundary */)
{
    if (!m_fInPal)
    {
        WARN("Leave called on a thread that is not running in this PAL\n");
        return ERROR_NOT_SUPPORTED;
    }

    PAL_ERROR palError = ERROR_SUCCESS;

    m_fInPal = FALSE;

    return palError;
}

#endif // FEATURE_PAL_SXS
