//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Core/BasePtr.h"

template <typename T, typename HeapAllocatorT = HeapAllocator>
class AutoPtr : public BasePtr<T>
{
public:
    AutoPtr(T * ptr) : BasePtr<T>(ptr) {}
    ~AutoPtr()
    {
        Clear();
    }

    AutoPtr& operator=(T * ptr)
    {
        Clear();
        this->ptr = ptr;
        return *this;
    }

private:
    void Clear()
    {
        if (this->ptr != nullptr)
        {
            AllocatorDelete(HeapAllocatorT, &HeapAllocatorT::Instance, this->ptr);
            this->ptr = nullptr;
        }
    }
};

template <typename T>
class AutoReleasePtr : public BasePtr<T>
{
    using BasePtr<T>::ptr;
public:
    AutoReleasePtr(T * ptr = nullptr) : BasePtr<T>(ptr) {}
    ~AutoReleasePtr()
    {
        Release();
    }

    void Release()
    {
        if (ptr != nullptr)
        {
            ptr->Release();
            this->ptr = nullptr;
        }
    }
};

template <typename T>
class AutoDiscardPTR : public BasePtr<T>
{
public:
    AutoDiscardPTR(T * ptr) : BasePtr<T>(ptr) {}
    ~AutoDiscardPTR()
    {
        Clear();
    }

    AutoDiscardPTR& operator=(T * ptr)
    {
        Clear();
        this->ptr = ptr;
        return *this;
    }

private:
    void Clear()
    {
        if (this->ptr != nullptr)
        {
            this->ptr->Discard();
            this->ptr = nullptr;
        }
    }
};

template <typename T>
class AutoCoTaskMemFreePtr : public BasePtr<T>
{
public:
    AutoCoTaskMemFreePtr(T* ptr) : BasePtr<T>(ptr) {}
    ~AutoCoTaskMemFreePtr()
    {
        Clear();
    }

private:
    void Clear()
    {
        if (this->ptr != nullptr)
        {
            CoTaskMemFree(this->ptr);
            this->ptr = nullptr;
        }
    }
};
