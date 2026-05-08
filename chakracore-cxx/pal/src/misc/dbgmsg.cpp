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
#else
static const char FOPEN_FLAGS[] = "wt";
#endif

/* size of output buffer (arbitrary) */
#define DBG_BUFFER_SIZE 20000

/* global and static variables */

const char16_t* W16_NULLSTRING = (const char16_t*) "N\0U\0L\0L\0\0";

/* we must use stdio functions directly rather that rely on PAL functions for
  output, because those functions do tracing and we need to avoid recursion */
FILE *output_file = NULL;


/* Function definitions */


#ifdef FEATURE_PAL_SXS
#define MODULE_FORMAT "-%p"
#else
#define MODULE_ID
#define MODULE_FORMAT
#endif // FEATURE_PAL_SXS