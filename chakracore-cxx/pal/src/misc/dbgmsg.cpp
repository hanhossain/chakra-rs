//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    misc/dbgmsg.cpp

Abstract:
    Implementation of Debug Message utilies. Relay channel information,
    output functions, etc.



--*/

/* PAL headers */

#include "pal/thread.hpp"
#include <new>
#include "pal/file.hpp"

#include "pal/dbgmsg.h"
#include "pal/cruntime.h"
#include "pal/critsect.h"
#include "pal/file.h"
#include "pal/misc.h"

/* standard headers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /* for pthread_self */
#include <errno.h>
#include <dirent.h>
#include <dlfcn.h>

/* <stdarg.h> needs to be included after "palinternal.h" to avoid name
   collision for va_start and va_end */
#include <stdarg.h>

using namespace CorUnix;

/* append mode file I/O is safer */
#define _PAL_APPEND_DBG_OUTPUT_

#if defined(_PAL_APPEND_DBG_OUTPUT_)
static const char FOPEN_FLAGS[] = "at";
#else
static const char FOPEN_FLAGS[] = "wt";
#endif

/* number of ENTRY nesting levels to indicate with a '.' */
#define MAX_NESTING 50

/* size of output buffer (arbitrary) */
#define DBG_BUFFER_SIZE 20000

/* global and static variables */

const char16_t* W16_NULLSTRING = (const char16_t*) "N\0U\0L\0L\0\0";

uint32_t dbg_channel_flags[DCI_LAST];
BOOL g_Dbg_asserts_enabled;

/* we must use stdio functions directly rather that rely on PAL functions for
  output, because those functions do tracing and we need to avoid recursion */
FILE *output_file = NULL;

/* master switch for debug channel enablement, to be modified by debugger */
Volatile<BOOL> dbg_master_switch __attribute__((init_priority(200))) = TRUE;


static const char *dbg_channel_names[]=
{
    "PAL",
    "LOADER",
    "HANDLE",
    "SHMEM",
    "THREAD",
    "EXCEPT",
    "CRT",
    "UNICODE",
    "ARCH",
    "SYNC",
    "FILE",
    "VIRTUAL",
    "MEM",
    "SOCKET",
    "DEBUG",
    "LOCALE",
    "MISC",
    "MUTEX",
    "CRITSEC",
    "POLL",
    "CRYPT",
    "SHFOLDER"
#ifdef FEATURE_PAL_SXS
  , "SXS"
#endif // FEATURE_PAL_SXS
};

static const char *dbg_level_names[]=
{
    "ENTRY",
    "TRACE",
    "WARN",
    "ERROR",
    "ASSERT",
    "EXIT"
};

static const char ENV_FILE[] = "PAL_API_TRACING";
static const char ENV_CHANNELS[] = "PAL_DBG_CHANNELS";
static const char ENV_ASSERTS[] = "PAL_DISABLE_ASSERTS";
static const char ENV_ENTRY_LEVELS[] = "PAL_API_LEVELS";

/* per-thread storage for ENTRY tracing level */
static pthread_key_t entry_level_key;

/* entry level limitation */
static int max_entry_level;

static CRITICAL_SECTION fprintf_crit_section __attribute__((init_priority(200)));

/* Function definitions */

/*++
Function :
    DBG_init_channels

    Parse environment variables PAL_DBG_CHANNELS and PAL_API_TRACING for debug
    channel settings; initialize static variables.

    (no parameters, no return value)
--*/
BOOL DBG_init_channels(void)
{
    int32_t i;
    const char * env_string;
    char* env_workstring;
    char* env_pcache;
    char* entry_ptr;
    char* level_ptr;
    char plus_or_minus;
    uint32_t flag_mask = 0;
    int ret;

    InternalInitializeCriticalSection(&fprintf_crit_section);

    /* output only asserts by default [only affects no-vararg-support case; if
       we have varargs, these flags aren't even checked for ASSERTs] */
    for(i=0;i<DCI_LAST;i++)
        dbg_channel_flags[i]=1<<DLI_ASSERT;

    /* parse PAL_DBG_CHANNELS environment variable */

    if (!(env_string = MiscGetenv(ENV_CHANNELS)))
    {
        env_pcache = env_workstring = NULL;
    }
    else
    {
        env_pcache = env_workstring = strdup(env_string);

        if (env_workstring == NULL)
        {
            /* Not enough memory */
            DeleteCriticalSection(&fprintf_crit_section);
            return FALSE;
        }
    }
    while(env_workstring)
    {
        entry_ptr=env_workstring;

        /* find beginning of next entry */
        while((*entry_ptr != '\0') &&(*entry_ptr != '+') && (*entry_ptr != '-'))
        {
            entry_ptr++;
        }

        /* break if end of string is reached */
        if(*entry_ptr == '\0')
        {
           break;
        }

        plus_or_minus=*entry_ptr++;

        /* find end of entry; if strchr returns NULL, we have reached the end
           of the string and we will leave the loop at the end of this pass. */
        env_workstring=strchr(entry_ptr,':');

        /* NULL-terminate entry, make env_string point to rest of string */
        if(env_workstring)
        {
            *env_workstring++='\0';
        }

        /* find period that separates channel name from level name */
        level_ptr=strchr(entry_ptr,'.');

        /* an entry with no period is illegal : ignore it */
        if(!level_ptr)
        {
            continue;
        }
        /* NULL-terminate channel name, make level_ptr point to the level name */
        *level_ptr++='\0';

        /* build the flag mask based on requested level */

        /* if "all" level is specified, we want to open/close all levels at
           once, so mask is either all ones or all zeroes */
        if(!strcmp(level_ptr,"all"))
        {
            if(plus_or_minus=='+')
            {
                flag_mask=0xFFFF;  /* OR this to open all levels */
            }
            else
            {
                flag_mask=0;       /* AND this to close all levels*/
            }
        }
        else
        {
            for(i=0;i<DLI_LAST;i++)
            {
                if(!strcmp(level_ptr,dbg_level_names[i]))
                {
                    if(plus_or_minus=='+')
                    {
                        flag_mask=1<<i;     /* OR this to open the level */
                    }
                    else
                    {
                        flag_mask=~(1<<i);  /* AND this to close the level */
                    }
                    break;
                }
            }
            /* didn't find a matching level : skip it. */
            if(i==DLI_LAST)
            {
                continue;
            }
        }

        /* Set EXIT and ENTRY channels to be identical */
        if(!(flag_mask & (1<<DLI_ENTRY)))
        {
            flag_mask = flag_mask & (~(1<<DLI_EXIT));
        }
        else
        {
            flag_mask = flag_mask | (1<<DLI_EXIT);
        }

        /* apply the flag mask to the specified channel */

        /* if "all" channel is specified, apply mask to all channels */
        if(!strcmp(entry_ptr,"all"))
        {
            if(plus_or_minus=='+')
            {
                for(i=0;i<DCI_LAST;i++)
                {
                    dbg_channel_flags[i] |= flag_mask; /* OR to open levels*/
                }
            }
            else
            {
                for(i=0;i<DCI_LAST;i++)
                {
                    dbg_channel_flags[i] &= flag_mask; /* AND to close levels */
                }
            }
        }
        else
        {
            for(i=0;i<DCI_LAST;i++)
            {
                if(!strcmp(entry_ptr,dbg_channel_names[i]))
                {
                    if(plus_or_minus=='+')
                    {
                        dbg_channel_flags[i] |= flag_mask;
                    }
                    else
                    {
                        dbg_channel_flags[i] &= flag_mask;
                    }

                    break;
                }
            }
            /* ignore the entry if the channel name is unknown */
        }
        /* done processing this entry; on to the next. */
    }
    free(env_pcache);

    /* select output file */
    env_string=MiscGetenv(ENV_FILE);
    if(env_string && *env_string!='\0')
    {
        if(!strcmp(env_string, "stderr"))
        {
            output_file = stderr;
        }
        else if(!strcmp(env_string, "stdout"))
        {
            output_file = stdout;
        }
        else
        {
            output_file = fopen(env_string,FOPEN_FLAGS);

            /* if file can't be opened, default to stderr */
            if(!output_file)
            {
                output_file = stderr;
                fprintf(stderr, "Can't open %s for writing : debug messages "
                        "will go to stderr. Check your PAL_API_TRACING "
                        "variable!\n", env_string);
            }
        }
    }
    else
    {
        output_file = stderr; /* output to stderr by default */
    }

    /* see if we need to disable assertions */
    env_string = MiscGetenv(ENV_ASSERTS);
    if(env_string && 0 == strcmp(env_string,"1"))
    {
        g_Dbg_asserts_enabled = FALSE;
    }
    else
    {
        g_Dbg_asserts_enabled = TRUE;
    }

    /* select ENTRY level limitation */
    env_string = MiscGetenv(ENV_ENTRY_LEVELS);
    if(env_string)
    {
        max_entry_level = atoi(env_string);
    }
    else
    {
        max_entry_level = 1;
    }

    /* if necessary, allocate TLS key for entry nesting level */
    if(0 != max_entry_level)
    {
        if ((ret = pthread_key_create(&entry_level_key,NULL)) != 0)
        {
            fprintf(stderr, "ERROR : pthread_key_create() failed error:%d (%s)\n",
                   ret, strerror(ret));
            DeleteCriticalSection(&fprintf_crit_section);;
            return FALSE;
        }
    }

    return TRUE;
}

/*++
Function :
    DBG_close_channels

    Stop outputting debug messages by closing the associated file.

    (no parameters, no return value)
--*/
void DBG_close_channels()
{
    if(output_file && output_file != stderr && output_file != stdout)
    {
        if (fclose(output_file) != 0)
        {
            fprintf(stderr, "ERROR : fclose() failed errno:%d (%s)\n",
                   errno, strerror(errno));
        }
    }

    output_file = NULL;

    DeleteCriticalSection(&fprintf_crit_section);

    /* if necessary, release TLS key for entry nesting level */
    if(0 != max_entry_level)
    {
        int retval;

        retval = pthread_key_delete(entry_level_key);
        if(0 != retval)
        {
            fprintf(stderr, "ERROR : pthread_key_delete() returned %d! (%s)\n",
                    retval, strerror(retval));
        }
    }
}


#ifdef FEATURE_PAL_SXS
#define MODULE_FORMAT "-%p"
#else
#define MODULE_ID
#define MODULE_FORMAT
#endif // FEATURE_PAL_SXS

/*++
Function :
    DBG_change_entrylevel

    retrieve current ENTRY nesting level and [optionally] modify it

Parameters :
    int new_level : value to which the nesting level must be set, or -1

Return value :
    nesting level at the time the function was called

Notes:
if new_level is -1, the nesting level will not be modified
--*/
int DBG_change_entrylevel(int new_level)
{
    int ret;

    if(0 == max_entry_level)
    {
        return 0;
    }

    // cast to long first to prevent void* to int cast warning
    int old_level = (int)(long) pthread_getspecific(entry_level_key);
    if(-1 != new_level)
    {
        if ((ret = pthread_setspecific(entry_level_key, (void *)new_level)) != 0)
        {
            fprintf(stderr, "ERROR : pthread_setspecific() failed "
                    "error:%d (%s)\n", ret, strerror(ret));
        }
    }
    return old_level;
}
