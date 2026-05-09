//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include <mutex>

class CriticalSection
{
    std::recursive_mutex mutex_;
public:
    CriticalSection(uint32_t spincount = 0) { }

    void Enter()
    {
        mutex_.lock();
    }

    void Leave()
    {
        mutex_.unlock();
    }

    bool TryEnter()
    {
        return mutex_.try_lock();
    }

    std::recursive_mutex& GetMutex() { return mutex_; }
};

//FakeCriticalSection mimics CriticalSection apis
class FakeCriticalSection
{
public:
    FakeCriticalSection(uint32_t spincount = 0) { /*do nothing*/spincount++; }
    ~FakeCriticalSection() {}
     BOOL  TryEnter() { return true; }
     void Enter() {}
     void Leave() {}
#if DBG
    bool IsLocked() const { return true; }
#endif
private:
    // only used for prefast analysis
    int cs;
};

class AutoOptionalCriticalSection
{
public:
     AutoOptionalCriticalSection(CriticalSection * cs) : cs(cs)
    {
        if (this->cs)
        {
            this->cs->Enter();
        }
    }

     ~AutoOptionalCriticalSection()
    {
        if (this->cs)
        {
            this->cs->Leave();
        }
    }

private:
    CriticalSection * cs;
};

template <class SyncObject>
class AutoRealOrFakeCriticalSection
{
public:
     AutoRealOrFakeCriticalSection(SyncObject * cs) : cs(cs) { this->cs->Enter(); }
     ~AutoRealOrFakeCriticalSection() { this->cs->Leave(); }
private:
    SyncObject * cs;
};
