//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

namespace Js
{
    LPCUTF8 const ISourceHolder::emptyString = reinterpret_cast<LPCUTF8>("\0");
    SimpleSourceHolder const ISourceHolder::emptySourceHolder(emptyString, _no_write_barrier_tag(), 0, true);
}
