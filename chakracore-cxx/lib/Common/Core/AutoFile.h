//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Core/BasePtr.h"

class AutoFILE : public BasePtr<PAL_FILE>
{
public:
    AutoFILE(PAL_FILE * file = nullptr) : BasePtr<PAL_FILE>(file) {};
    ~AutoFILE()
    {
        Close();
    }
    AutoFILE& operator=(PAL_FILE * file)
    {
        Close();
        this->ptr = file;
        return *this;
    }
    void Close()
    {
        if (ptr != nullptr)
        {
            PAL_fclose(ptr);
            ptr = nullptr;
        }
    }
};
