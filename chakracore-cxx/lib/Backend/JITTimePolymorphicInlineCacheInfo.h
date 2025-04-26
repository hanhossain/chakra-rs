//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

class JITTimePolymorphicInlineCacheInfo
{
public:
    JITTimePolymorphicInlineCacheInfo();

    static void InitializeEntryPointPolymorphicInlineCacheInfo(
        Recycler * recycler,
        Js::EntryPointPolymorphicInlineCacheInfo * runtimeInfo,
        __out CodeGenWorkItemIDL * jitInfo);

    JITTimePolymorphicInlineCache * GetInlineCache(uint index) const;
    bool HasInlineCaches() const;
    byte GetUtil(uint index) const;

private:

    static void InitializePolymorphicInlineCacheInfo(
        Recycler * recycler,
        Js::PolymorphicInlineCacheInfo * runtimeInfo,
        __out PolymorphicInlineCacheInfoIDL * jitInfo);

    PolymorphicInlineCacheInfoIDL m_data;
};
