//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class CriticalSection
: public CCLock
{
public:
    CriticalSection(DWORD spincount = 0): CCLock(true) { }
};

//FakeCriticalSection mimics CriticalSection apis
class FakeCriticalSection
{
public:
    FakeCriticalSection(DWORD spincount = 0) { /*do nothing*/spincount++; }
    ~FakeCriticalSection() {}
#pragma prefast(suppress:__WARNING_FAILING_TO_ACQUIRE_MEDIUM_CONFIDENCE)
    _Success_(return) BOOL _Acquires_lock_(this->cs) TryEnter() { return true; }
#pragma prefast(suppress:__WARNING_FAILING_TO_ACQUIRE_MEDIUM_CONFIDENCE)
    _Acquires_lock_(this->cs) void Enter() {}
#pragma prefast(suppress:__WARNING_FAILING_TO_RELEASE_MEDIUM_CONFIDENCE)
    _Releases_lock_(this->cs) void Leave() {}
#if DBG
    bool IsLocked() const { return true; }
#endif
private:
    // only used for prefast analysis
    int cs;
};

class AutoCriticalSection
{
public:
    _Acquires_lock_(this->cs->cs) AutoCriticalSection(CriticalSection * cs) : cs(cs) { this->cs->Enter(); }
    _Releases_lock_(this->cs->cs) ~AutoCriticalSection() { cs->Leave(); }
private:
    CriticalSection * cs;
};

class AutoOptionalCriticalSection
{
public:
    _When_(this->cs != nullptr, _Acquires_lock_(this->cs->cs)) AutoOptionalCriticalSection(CriticalSection * cs) : cs(cs)
    {
        if (this->cs)
        {
            this->cs->Enter();
        }
    }

    _When_(this->cs != nullptr, _Releases_lock_(this->cs->cs)) ~AutoOptionalCriticalSection()
    {
        if (this->cs)
        {
            this->cs->Leave();
        }
    }

private:
    CriticalSection * cs;
};

template <class SyncObject = FakeCriticalSection >
class AutoRealOrFakeCriticalSection
{
public:
    _Acquires_lock_(this->cs->cs) AutoRealOrFakeCriticalSection(SyncObject * cs) : cs(cs) { this->cs->Enter(); }
    _Releases_lock_(this->cs->cs) ~AutoRealOrFakeCriticalSection() { this->cs->Leave(); }
private:
    SyncObject * cs;
};

template <class SyncObject = FakeCriticalSection >
class AutoOptionalRealOrFakeCriticalSection
{
public:
    _When_(this->cs != nullptr, _Acquires_lock_(this->cs->cs)) AutoOptionalRealOrFakeCriticalSection(SyncObject * cs) : cs(cs)
    {
        if (this->cs)
        {
            this->cs->Enter();
        }
    }

    _When_(this->cs != nullptr, _Releases_lock_(this->cs->cs)) ~AutoOptionalRealOrFakeCriticalSection()
    {
        if (this->cs)
        {
            this->cs->Leave();
        }
    }

private:
    SyncObject * cs;
};
