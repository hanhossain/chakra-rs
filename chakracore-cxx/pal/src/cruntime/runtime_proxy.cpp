//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

// do not use other PAL headers here.
// combining wctype.h with PAL headers creates type mismatches under latest NDK

#include <wctype.h>

int proxy_iswspace(char16_t c)
{
    return iswspace(c);
}
