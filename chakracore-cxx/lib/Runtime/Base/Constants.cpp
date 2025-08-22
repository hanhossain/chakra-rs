//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

using namespace Js;

const uint Constants::InvalidSourceIndex;
const RegSlot Constants::NoRegister;

const char16_t Constants::AnonymousFunction[] = _u("Anonymous function");
const char16_t Constants::Anonymous[] = _u("anonymous");
const char16_t Constants::Empty[] = _u("");
const char16_t Constants::FunctionCode[] = _u("Function code");
const char16_t Constants::GlobalCode[] = _u("Global code");
const char16_t Constants::ModuleCode[] = _u("Module code");
const char16_t Constants::EvalCode[] = _u("eval code");
const char16_t Constants::GlobalFunction[] = _u("glo");
const char16_t Constants::UnknownScriptCode[] = _u("Unknown script code");
const char16_t Constants::StringReplace[] = _u("String.prototype.replace");
const char16_t Constants::StringMatch[] = _u("String.prototype.match");

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
