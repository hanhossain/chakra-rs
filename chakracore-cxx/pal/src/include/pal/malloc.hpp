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

// TODO (hanhossain): remove these
namespace CorUnix{

    // Define "new" style allocators (which allocate then call a constructor) for different numbers of
    // constructor arguments. Added based on usage.

    template<class T> T* InternalNewArray(size_t cElements)
    {
        size_t cbSize = (cElements * sizeof(T)) + sizeof(size_t);
        T *pMem;

        pMem = (T*)malloc(cbSize);

        if (pMem == NULL)
            return NULL;

        *(size_t*)pMem = cElements;
        pMem = (T*)((size_t*)pMem + 1);

        return new (pMem) T[cElements]();
    }
}

#endif // _MALLOC_HPP
