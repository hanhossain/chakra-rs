//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

//
// WriteBarrier Macros are weird because of "compat"
// In general, write barriers are used only on non-Win32 builds
// However, for perf, some classes are also allocated in Write-Barrier memory, even on Windows
// Those classes can force WriteBarrier types to always be generated for their definitions
//
// Usage:
//   In general, don't need to use anything, just use the macros to annotate your fields
//
//

#define NO_WRITE_BARRIER_TAG_TYPE(arg) arg, _no_write_barrier_tag
#define NO_WRITE_BARRIER_TAG(arg) arg, _no_write_barrier_tag()

// Unsafely cast a typical "typename WriteBarrierFieldTypeTraits<>::Type *" type. Only use in rare cases where we
// understand the underlying memory usage.
template <class T, class U>
inline T unsafe_write_barrier_cast(U p)
{
    return (T)p;
}
