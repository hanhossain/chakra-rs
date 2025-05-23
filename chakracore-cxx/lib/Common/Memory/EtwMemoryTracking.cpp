//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"

// xplat-todo: Need to figure out equivalent method for allocation tracing
// on platforms other than Windows
#ifdef ETW_MEMORY_TRACKING
#include "microsoft-scripting-jscript9.internalevents.h"

enum ArenaType: unsigned short
{
    ArenaTypeRecycler = 1,
    ArenaTypeArena = 2
};

class EtwMemoryEvents
{
public:
    static void ReportAllocation(void *arena, void *address, size_t size)
    {
        allocCount++;
        if (allocCount == 7500)
        {
            allocCount = 0;
            ::Sleep(350);
        }

        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_ALLOC(arena, address, size);
    }

    static void ReportFree(void *arena, void *address, size_t size)
    {
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_FREE(arena, address, size);
    }

    static void ReportReallocation(void *arena, void *address, size_t existingSize, size_t newSize)
    {
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_FREE(arena, address, existingSize);
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_ALLOC(arena, address, newSize);
    }

    static void ReportArenaCreated(void *arena, ArenaType arenaType)
    {
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_CREATE(arena, arenaType);
    }

    static void ReportArenaDestroyed(void *arena)
    {
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_DESTROY(arena);
    }

    static void ReportFreeAll(void *arena, ArenaType arenaType)
    {
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_DESTROY(arena);
        EventWriteJSCRIPT_INTERNAL_ALLOCATOR_CREATE(arena, arenaType);
    }

private:
    static int allocCount;
};

int EtwMemoryEvents::allocCount = 0;

// Workaround to stop the linker from collapsing the Arena/recycler methods to
// a single implementation so that we have nicer stacks
ArenaType g_arena;
#define DISTINGUISH_FUNCTION(arenaType) g_arena = ArenaType##arenaType

void ArenaMemoryTracking::Activate()
{
}

void ArenaMemoryTracking::ArenaCreated(Allocator *arena, LPCWSTR name)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportArenaCreated(arena, ArenaTypeArena);
}

void ArenaMemoryTracking::ArenaDestroyed(Allocator *arena)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportArenaDestroyed(arena);
}

void ArenaMemoryTracking::ReportAllocation(Allocator *arena, void *address, size_t size)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportAllocation(arena, address, size);
}

void ArenaMemoryTracking::ReportReallocation(Allocator *arena, void *address, size_t existingSize, size_t newSize)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportReallocation(arena, address, existingSize, newSize);
}

void ArenaMemoryTracking::ReportFree(Allocator *arena, void *address, size_t size)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportFree(arena, address, size);
}

void ArenaMemoryTracking::ReportFreeAll(Allocator *arena)
{
    DISTINGUISH_FUNCTION(Arena);
    EtwMemoryEvents::ReportFreeAll(arena, ArenaTypeArena);
}

// Recycler tracking

void RecyclerMemoryTracking::Activate()
{
}

bool RecyclerMemoryTracking::IsActive()
{
    return true;
}

// The external reporting for the recycler uses the MemspectMemoryTracker
void RecyclerMemoryTracking::ReportRecyclerCreate(Recycler * recycler)
{
    DISTINGUISH_FUNCTION(Recycler);
    EtwMemoryEvents::ReportArenaCreated(recycler, ArenaTypeRecycler);
}

void RecyclerMemoryTracking::ReportRecyclerDestroy(Recycler * recycler)
{
    DISTINGUISH_FUNCTION(Recycler);
    EtwMemoryEvents::ReportArenaDestroyed(recycler);
}

void RecyclerMemoryTracking::ReportAllocation(Recycler * recycler, void *address, size_t size)
{
    DISTINGUISH_FUNCTION(Recycler);
    EtwMemoryEvents::ReportAllocation(recycler, address, size);
}

void RecyclerMemoryTracking::ReportFree(Recycler * recycler, void *address, size_t size)
{
    DISTINGUISH_FUNCTION(Recycler);
    EtwMemoryEvents::ReportFree(recycler, address, size);
}

void RecyclerMemoryTracking::ReportUnallocated(Recycler * recycler, void* address, void *endAddress, size_t sizeCat)
{
    byte * byteAddress = (byte *) address;

    while (byteAddress + sizeCat <= endAddress)
    {
        EtwMemoryEvents::ReportAllocation(recycler, byteAddress, sizeCat);
        byteAddress += sizeCat;
    }
}

#endif
