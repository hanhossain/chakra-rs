//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

//=============================
// Enabled Warnings
//=============================
#pragma warning(default:4242)   // conversion possible loss of data

//=============================
// Disabled Warnings
//=============================
// Warnings that we don't care about
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4127)  // constant expression for Trace/Assert
#pragma warning(disable: 4200)  // nonstandard extension used: zero-sized array in struct/union
#pragma warning(disable: 4201)  // nameless unions are part of C++
#pragma warning(disable: 4512)  // private operator= are good to have
#pragma warning(disable: 4481)  // allow use of abstract and override keywords

#pragma warning(disable: 4324)  // structure was padded due to alignment specifier

// warnings caused by normal optimizations
#if DBG
#else // DBG
#pragma warning(disable: 4702)  // unreachable code caused by optimizations
#pragma warning(disable: 4189)  // initialized but unused variable
#pragma warning(disable: 4390)  // empty controlled statement
#endif // DBG
