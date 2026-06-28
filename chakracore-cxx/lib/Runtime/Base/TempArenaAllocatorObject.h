//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    template <bool isGuestArena>
    class TempArenaAllocatorWrapper : public FinalizableObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<ArenaAllocator, _no_write_barrier_policy, _no_write_barrier_policy>::Type allocator;
        typename WriteBarrierFieldTypeTraits<ArenaData **, _no_write_barrier_policy, _no_write_barrier_policy>::Type externalGuestArenaRef;
        typename WriteBarrierFieldTypeTraits<Recycler *, _no_write_barrier_policy, _no_write_barrier_policy>::Type recycler;

        TempArenaAllocatorWrapper(const char16_t* name, PageAllocator * pageAllocator, void (*outOfMemoryFunc)());

    public:

        void AdviseInUse();
        void AdviseNotInUse();

        static TempArenaAllocatorWrapper* Create(ThreadContext * threadContext);

        virtual void Finalize(bool isShutdown) override
        {
        }

        virtual void Dispose(bool isShutdown) override;
        virtual void Mark(Recycler *recycler) override { AssertMsg(false, "Mark called on object that isn't TrackableObject"); }

        ArenaAllocator *GetAllocator()
        {
            return &allocator;
        }

    };

    typedef TempArenaAllocatorWrapper<true> TempGuestArenaAllocatorObject;
    typedef TempArenaAllocatorWrapper<false> TempArenaAllocatorObject;
 }
