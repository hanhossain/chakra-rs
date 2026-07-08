//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++





--*/

#ifndef __PAL_ASSERT_H__
#define __PAL_ASSERT_H__

extern "C" {

#if defined(_DEBUG)
#include "assert_only.h"
#define _ASSERTE(e) Assert(e)
#else // !DEBUG
#define _ASSERTE(e) ((void)0)
#endif

#ifndef assert
#define assert(e) _ASSERTE(e)
#endif  // assert

}

#endif // __PAL_ASSERT_H__
