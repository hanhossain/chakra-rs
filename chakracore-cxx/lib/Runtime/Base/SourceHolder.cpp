//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

namespace Js
{
    LPCUTF8 const ISourceHolder::emptyString = reinterpret_cast<LPCUTF8>("\0");
    SimpleSourceHolder const ISourceHolder::emptySourceHolder(emptyString, _no_write_barrier_tag(), 0, true);

    void HeapSourceHolder::Dispose(bool fShutdown)
    {
        Unload();
    }

    void HeapSourceHolder::Unload()
    {
        if (this->shouldFreeSource)
        {
            // REVIEW: Where is the inc for this guy?
            //PERF_COUNTER_DEC(Basic, ScriptCodeBufferCount);
            if (this->originalSourceBuffer) { free(this->originalSourceBuffer); }

            this->source = nullptr;
            this->originalSourceBuffer = nullptr;
            this->isEmpty = true;
            this->byteLength = 0;

            this->shouldFreeSource = false;
        }
    }
}
