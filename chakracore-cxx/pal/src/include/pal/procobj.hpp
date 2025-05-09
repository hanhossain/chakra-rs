//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    include/pal/procobj.hpp

Abstract:
    Header file for process structures



--*/

#ifndef _PAL_PROCOBJ_HPP_
#define _PAL_PROCOBJ_HPP_

#include "corunix.hpp"

namespace CorUnix
{
    extern CObjectType otProcess;

    typedef enum
    {
        PS_IDLE,
        PS_STARTING,
        PS_RUNNING,
        PS_DONE
    } PROCESS_STATE;

    //
    // Struct for process module list (EnumProcessModules)
    //
    struct ProcessModules
    {
        ProcessModules *Next;
        void * BaseAddress;
        char Name[0];
    };

    //
    // Ideally dwProcessId would be part of the process object's immutable
    // data. Doing so, though, creates complications in CreateProcess. The
    // contents of the immutable data for a new object must be set before
    // that object is registered with the object manager (as the object
    // manager may make a copy of the immutable data). The PID for a new
    // process, though, is not known until after creation. Registering the
    // process object after process creation creates an undesirable error path
    // -- if we are not able to register the process object (say, because of
    // a low resource condition) we would be forced to return an error to
    // the caller of CreateProcess, even though the new process was actually
    // created...
    //
    // Note: we could work around this by effectively always going down
    // the create suspended path. That is, the new process would not exec until
    // the parent process released it. It's unclear how much benefit this would
    // provide us.
    //

    class CProcProcessLocalData
    {
    public:
        CProcProcessLocalData() 
            : 
            dwProcessId(0), 
            ps(PS_IDLE),
            dwExitCode(0), 
            lAttachCount(0),
            pProcessModules(NULL)
        {
        };

        ~CProcProcessLocalData();
        
        uint32_t dwProcessId;
        PROCESS_STATE ps;
        uint32_t dwExitCode;
        int32_t lAttachCount;
        ProcessModules *pProcessModules;
    };

    class CProcSharedData
    {
    public:
        uint32_t dwProcessId;
    };

    PAL_ERROR
    InternalCreateProcess(
        CPalThread *pThread,
        LPCWSTR lpApplicationName,
        LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        BOOL bInheritHandles,
        uint32_t dwCreationFlags,
        void * lpEnvironment,
        LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation
        );

    PAL_ERROR
    InitializeProcessData(
        void
        );

    PAL_ERROR
    InitializeProcessCommandLine(
        LPWSTR lpwstrCmdLine,
        LPWSTR lpwstrFullPath
        );

    PAL_ERROR
    CreateInitialProcessAndThreadObjects(
        CPalThread *pThread
        );

    extern IPalObject *g_pobjProcess;
}

#endif // _PAL_PROCOBJ_HPP_

