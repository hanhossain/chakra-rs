//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// ARM-specific macro definitions

#pragma once

#ifndef _M_ARM
#error Include arm.h in builds of ARM targets only.
#endif

extern "C" void * arm_GET_CURRENT_FRAME(void);
extern "C" void arm_SAVE_REGISTERS(void*);

/*
 * The relevant part of the frame looks like this (high addresses at the top, low ones at the bottom):
 *
 * ----------------------
 * r3     <=== Homed input parameters
 * r2     <
 * r1     <
 * r0     <===
 * lr     <=== return address
 * r11    <=== current r11 (frame pointer)
 * ...
 */

const uint32_t ReturnAddrOffsetFromFramePtr = 1;
const uint32_t ArgOffsetFromFramePtr = 2;
