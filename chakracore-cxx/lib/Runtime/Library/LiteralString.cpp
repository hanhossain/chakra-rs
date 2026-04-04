//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"

namespace Js
{
    DEFINE_RECYCLER_TRACKER_PERF_COUNTER(LiteralString);

    LiteralString::LiteralString(StaticType * type) : JavascriptString(type)
    {
    }

    LiteralString::LiteralString(StaticType * type, const char16_t* content, charcount_t charLength) :
        JavascriptString(type, charLength, content)
    {
#if defined(DBG) && defined(_M_IX86)
        // Make sure content isn't on the stack by comparing to stack bounds in TIB
        AssertMsg(!ThreadContext::IsOnStack((void*)content),
            "LiteralString object created using stack buffer...");
#endif

#ifdef PROFILE_STRINGS
        StringProfiler::RecordNewString( type->GetScriptContext(), content, charLength );
#endif
    }

    LiteralString* LiteralString::New(StaticType* type, const char16_t* content, charcount_t charLength, Recycler* recycler)
    {
        return RecyclerNew(recycler, LiteralString, type, content, charLength);
    }

    LiteralString* LiteralString::CreateEmptyString(StaticType* type)
    {
        return RecyclerNew(type->GetScriptContext()->GetRecycler(), LiteralString, type, u"", 0);
    }

} // namespace Js
