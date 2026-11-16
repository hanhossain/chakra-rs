//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class ThreadContext;
class JsrtContext;

class JsrtCallbackState
{
public:
    JsrtCallbackState(ThreadContext* currentThreadContext);
    ~JsrtCallbackState();

private:
    ThreadContext* originalThreadContext;
    JsrtContext* originalJsrtContext;
};

void VALIDATE_ENTER_CURRENT_THREAD();
