//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Interface/CommonDefines.h"

#ifdef __clang__
#include <typeinfo>
using std::type_info;
#endif

#include "PlatformAgnostic/CommonPal.h"
#include "PlatformAgnostic/CommonTypedefs.h"
