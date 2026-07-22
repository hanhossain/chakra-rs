//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Memory/RecyclerWeakReference.h"

namespace Js
{
    class JavascriptLibrary;
}

namespace UnifiedRegex
{
    struct Program;
    class Matcher;
    struct TrigramInfo;

    static const uint TestCacheSize = 8;

    struct RegExpTestCache
    {
        typename WriteBarrierFieldTypeTraits<BVStatic<TestCacheSize>>::Type resultBV;
        typename WriteBarrierFieldTypeTraits<RecyclerWeakReference<Js::JavascriptString>*>::Type inputArray[];
    };

    struct RegexPattern : FinalizableObject
    {
        typename WriteBarrierFieldTypeTraits<RegExpTestCache*>::Type testCache;

        struct UnifiedRep
        {
            typename WriteBarrierFieldTypeTraits<Program*>::Type program;
            typename WriteBarrierFieldTypeTraits<Matcher*>::Type matcher;
            typename WriteBarrierFieldTypeTraits<TrigramInfo*>::Type trigramInfo;
        };

        typename WriteBarrierFieldTypeTraits<Js::JavascriptLibrary *>::Type const library;

        typename WriteBarrierFieldTypeTraits<bool>::Type isLiteral : 1;
        typename WriteBarrierFieldTypeTraits<bool>::Type isShallowClone : 1;

        union Rep
        {
            typename WriteBarrierFieldTypeTraits<UnifiedRep>::Type unified;

            Rep() : unified() {}
        };
        typename WriteBarrierFieldTypeTraits<Rep>::Type rep;

        RegexPattern(Js::JavascriptLibrary *const library, Program* program, bool isLiteral);

        static RegexPattern *New(Js::ScriptContext *scriptContext, Program* program, bool isLiteral);

        virtual void Finalize(bool isShutdown) override;
        virtual void Dispose(bool isShutdown) override;
        virtual void Mark(Recycler *recycler) override { AssertMsg(false, "Mark called on object that isn't TrackableObject"); }

        Js::ScriptContext *GetScriptContext() const;

        inline bool IsLiteral() const { return isLiteral; }
        uint16 NumGroups() const;
        bool IsIgnoreCase() const;
        bool IsGlobal() const;
        bool IsMultiline() const;
        bool IsDotAll() const;
        bool IsUnicode() const;
        bool IsSticky() const;
        bool WasLastMatchSuccessful() const;
        GroupInfo GetGroup(int groupId) const;

        Js::InternalString GetSource() const;
        RegexFlags GetFlags() const;

        typename WriteBarrierFieldTypeTraits<RegExpTestCache*>::Type EnsureTestCache();
        static uint GetTestCacheIndex(Js::JavascriptString* str);

#if ENABLE_REGEX_CONFIG_OPTIONS
        void Print(DebugWriter* w);
#endif
        RegexPattern *CopyToScriptContext(Js::ScriptContext *scriptContext);
    };
}
