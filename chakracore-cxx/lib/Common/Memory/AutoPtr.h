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
