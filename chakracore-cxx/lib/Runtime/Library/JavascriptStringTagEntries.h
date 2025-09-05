//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// This file defines String.prototype tag functions and their parameters.

#ifndef TAGENTRY
#error "TAGENTRY macro isn't defined!"
#define TAGENTRY(...) // Stop editors from seeing this as one big syntax error
#endif

//       Name       Tag         Properties
TAGENTRY(Anchor,    u"a",       u"name" )
TAGENTRY(Big,       u"big"              )
TAGENTRY(Blink,     u"blink"            )
TAGENTRY(Bold,      u"b"                )
TAGENTRY(Fixed,     u"tt"               )
TAGENTRY(FontColor, u"font",    u"color")
TAGENTRY(FontSize,  u"font",    u"size" )
TAGENTRY(Italics,   u"i"                )
TAGENTRY(Link,      u"a",       u"href" )
TAGENTRY(Small,     u"small"            )
TAGENTRY(Strike,    u"strike"           )
TAGENTRY(Sub,       u"sub"              )
TAGENTRY(Sup,       u"sup"              )
