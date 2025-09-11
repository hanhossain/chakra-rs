//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    pal/malloc.hpp

Abstract:
    Declarations for suspension safe memory allocation functions



--*/

#ifndef _MALLOC_HPP
#define _MALLOC_HPP

#include "pal/corunix.hpp"
#include "pal/thread.hpp"

#include <stdarg.h>

#if HAVE_ALLOCA_H
#include <alloca.h>
#endif  // HAVE_ALLOCA_H  

inline void* operator new(size_t, void* p) throw () { return p; }
inline void* operator new[](size_t, void* p) throw () { return p; }

#endif // _MALLOC_HPP
