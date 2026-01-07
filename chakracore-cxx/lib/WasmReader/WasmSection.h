//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

#ifdef ENABLE_WASM
namespace Wasm
{

    enum SectionFlag
    {
        fSectNone,
        fSectIgnore,
    };

#define WASM_SECTION(intName, id, flag, precendent) bSect ## intName,
    enum SectionCode : uint8_t
    {
#include "WasmSections.h"
        bSectLimit,
        bsectLastKnownSection = bSectData
    };

    struct SectionInfo
    {
        SectionInfo(SectionFlag, SectionCode, const char16_t*, const uint32_t);
        SectionFlag flag;
        SectionCode precedent;
        const char16_t* name;
        const uint32_t nameLength;
        static SectionInfo All[bSectLimit];
    };
}
#endif
