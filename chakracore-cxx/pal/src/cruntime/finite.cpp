//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    finite.cpp

Abstract:

    Implementation of _finite function (Windows specific runtime function).



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"

#ifdef min
#define TEMP_MIN min
#undef min
#endif
#ifdef max
#define TEMP_MAX max
#undef max
#endif

#include <math.h>

#ifdef TEMP_MIN
#define min TEMP_MIN
#undef TEMP_MIN
#endif
#ifdef TEMP_MAX
#define max TEMP_MAX
#undef TEMP_MAX
#endif

#if HAVE_IEEEFP_H
#include <ieeefp.h>
#endif  // HAVE_IEEEFP_H
#include <errno.h>

#define PAL_NAN sqrt(-1.0)
#define PAL_POSINF -log(0.0)
#define PAL_NEGINF log(0.0)

SET_DEFAULT_DEBUG_CHANNEL(CRT);


