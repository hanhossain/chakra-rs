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