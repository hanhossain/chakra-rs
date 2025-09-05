//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

using namespace Js;

const uint Constants::InvalidSourceIndex;
const RegSlot Constants::NoRegister;

const char16_t Constants::AnonymousFunction[] = u"Anonymous function";
const char16_t Constants::Anonymous[] = u"anonymous";
const char16_t Constants::Empty[] = u"";
const char16_t Constants::FunctionCode[] = u"Function code";
const char16_t Constants::GlobalCode[] = u"Global code";
const char16_t Constants::ModuleCode[] = u"Module code";
const char16_t Constants::EvalCode[] = u"eval code";
const char16_t Constants::GlobalFunction[] = u"glo";
const char16_t Constants::UnknownScriptCode[] = u"Unknown script code";
const char16_t Constants::StringReplace[] = u"String.prototype.replace";
const char16_t Constants::StringMatch[] = u"String.prototype.match";

const unsigned long Constants::ExponentMask = 0x3FF0000000000000;
const unsigned long Constants::MantissaMask = 0x000FFFFFFFFFFFFF;

#ifdef TARGET_64
const size_t Constants::StackLimitForScriptInterrupt = 0x7fffffffffffffff;
#else
const size_t Constants::StackLimitForScriptInterrupt = 0x7fffffff;
#endif

#pragma warning(push)
#pragma warning(disable:4815) // Allow no storage for zero-sized array at end of NullFrameDisplay struct.
const Js::FrameDisplay Js::NullFrameDisplay = 0;
const Js::FrameDisplay Js::StrictNullFrameDisplay(0, true);
#pragma warning(pop)
