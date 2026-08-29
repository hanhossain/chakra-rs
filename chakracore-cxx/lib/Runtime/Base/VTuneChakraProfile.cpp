//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifdef VTUNE_PROFILING

#include "VTuneChakraProfile.h"
#include "jitprofiling.h"

const utf8char_t VTuneChakraProfile::DynamicCode[] = "Dynamic code";

//
// Get URL from source context, called by LogMethodNativeLoadEvent and LogLoopBodyLoadEvent
//
utf8char_t* VTuneChakraProfile::GetUrl(Js::FunctionBody* body, size_t* urlBufferLength )
{
    utf8char_t* utf8Url = NULL;
    if (!body->GetSourceContextInfo()->IsDynamic())
    {
        const char16_t* url = body->GetSourceContextInfo()->url;
        if (url)
        {
            size_t urlCharLength = std::u16string(url).length();
            charcount_t ccUrlCharLength = static_cast<charcount_t>(urlCharLength);
            ccUrlCharLength = min(ccUrlCharLength, UINT_MAX);       // Just truncate if it is too big

            *urlBufferLength = UInt32Math::MulAdd<3, 1>(ccUrlCharLength);
            utf8Url = HeapNewNoThrowArray(utf8char_t, *urlBufferLength);
            if (utf8Url)
            {
                utf8::EncodeIntoAndNullTerminate<utf8::Utf8EncodingKind::Cesu8>(utf8Url, *urlBufferLength, url, ccUrlCharLength);
            }
        }
    }
    else
    {
        utf8Url = (utf8char_t*)VTuneChakraProfile::DynamicCode;
    }
    return utf8Url;
}

#endif /* VTUNE_PROFILING */
