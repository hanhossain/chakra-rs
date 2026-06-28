//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
class RemoteSourceContextInfo;
class SourceDynamicProfileManager;
};

//
// This object is created per script source file or dynamic script buffer.
//
class SourceContextInfo
{
public:
    typename WriteBarrierFieldTypeTraits<uint>::Type sourceContextId;
    typename WriteBarrierFieldTypeTraits<Js::LocalFunctionId>::Type nextLocalFunctionId;           // Count of functions seen so far

#if DBG
    typename WriteBarrierFieldTypeTraits<bool>::Type closed;
#endif

    typename WriteBarrierFieldTypeTraits<unsigned long>::Type dwHostSourceContext;      // Context passed in to ParseScriptText
    typename WriteBarrierFieldTypeTraits<bool>::Type isHostDynamicDocument;         // will be set to true when current doc is treated dynamic from the host side. (IActiveScriptContext::IsDynamicDocument)

    union
    {
        struct
        {
            typename WriteBarrierFieldTypeTraits<char16_t const *, _no_write_barrier_policy, _no_write_barrier_policy>::Type url;            // The url of the file
            typename WriteBarrierFieldTypeTraits<char16_t const *, _no_write_barrier_policy, _no_write_barrier_policy>::Type sourceMapUrl;   // The url of the source map, such as actual non-minified source of JS on the server.
        };
        typename WriteBarrierFieldTypeTraits<uint>::Type      hash;                 // hash for dynamic scripts
    };

#if ENABLE_PROFILE_INFO
    typename WriteBarrierFieldTypeTraits<Js::SourceDynamicProfileManager *>::Type sourceDynamicProfileManager;
#endif

    void EnsureInitialized();
    bool IsDynamic() const { return dwHostSourceContext == Js::Constants::NoHostSourceContext || isHostDynamicDocument; }
    bool IsSourceProfileLoaded() const;
    SourceContextInfo* Clone(Js::ScriptContext* scriptContext) const;
};
