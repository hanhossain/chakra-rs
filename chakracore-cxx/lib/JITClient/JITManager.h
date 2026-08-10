//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

// We need real JITManager code when on windows.
// Otherwise we use a dummy JITManager which disables OOP JIT to reduce code noise.

class JITManager
{
public:
    static int32_t DeserializeRPCData(
        _In_reads_(bufferSize) const byte* buffer,
        _In_ uint bufferSize,
        _Out_ CodeGenWorkItemIDL **workItemData
    ) { *workItemData = nullptr; return E_NOTIMPL; }

    static int32_t SerializeRPCData(
        _In_ CodeGenWorkItemIDL *workItemData,
        _Out_ size_t* bufferSize,
        _Outptr_result_buffer_(*bufferSize) const byte** outBuffer
    ) { *bufferSize = 0; *outBuffer = nullptr; return E_NOTIMPL; }

private:
    static JITManager s_jitManager;
};
