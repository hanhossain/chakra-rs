//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace PlatformAgnostic
{
    class SystemInfo
    {
    public:
        static bool GetMaxVirtualMemory(size_t *totalAS);

#define SET_BINARY_PATH_ERROR_MESSAGE(path, msg) \
    str_len = (int) strlen(msg);                 \
    memcpy(path, msg, (size_t)str_len);          \
    path[str_len] = char(0)

        static bool GetBinaryLocation(char *path, const unsigned size);
        static bool GetBinaryLocation(char16 *path, const unsigned size);
    };
} // namespace PlatformAgnostic
