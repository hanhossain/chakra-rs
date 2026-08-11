//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    thread/procprivate.hpp

Abstract:

    Private process structures and routines
    
Revision History:



--*/

#ifndef _PAL_PROCPRIVATE_HPP_
#define _PAL_PROCPRIVATE_HPP_

#include "pal/thread.hpp"

namespace CorUnix
{

    /*++
    Function:
      PROCAddThread

    Abstract
      Add a thread to the thread list of the current process
    --*/
    void PROCAddThread(CPalThread *pCurrentThread, CPalThread *pTargetThread);

    extern CPalThread *pGThreadList;

    /*++
    Function:
      PROCRemoveThread

    Abstract
      Remove a thread form the thread list of the current process
    --*/
    void PROCRemoveThread(CPalThread *pCurrentThread, CPalThread *pTargetThread);

} // namespace CorUnix

#endif //_PAL_PROCPRIVATE_HPP_


