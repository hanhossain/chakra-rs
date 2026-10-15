//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "ChakraCore.h"

#include <condition_variable>
#include <memory>
#include <rust/cxx.h>

class RuntimeThreadData
{
public:
    RuntimeThreadData();
    ~RuntimeThreadData();
    static std::shared_ptr<RuntimeThreadData> New();

    JsSharedArrayBufferContentHandle sharedContent_;

    void set_shared_content(JsSharedArrayBufferContentHandle shared_content);
    JsSharedArrayBufferContentHandle get_shared_content() const;
};
