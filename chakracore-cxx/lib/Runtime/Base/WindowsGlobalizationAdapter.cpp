//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"
#include "WindowsGlobalizationAdapter.h"

#if defined(ENABLE_INTL_OBJECT) || defined(ENABLE_ES6_CHAR_CLASSIFIER)

#define IfFailThrowHr(op) \
    if (FAILED(hr=(op))) \
    { \
        JavascriptError::MapAndThrowError(scriptContext, hr);\
    } \

#define IfNullReturnError(EXPR, ERROR) do { if (!(EXPR)) { return (ERROR); } } while(FALSE)
#define IfFailedReturn(EXPR) do { hr = (EXPR); if (FAILED(hr)) { return hr; }} while(FALSE)
#define IfFailedSetErrorCodeAndReturn(EXPR, hrVariable) do { hr = (EXPR); if (FAILED(hr)) { hrVariable = hr; return hr; }} while(FALSE)
#define IfFailedGoLabel(expr, label) if (FAILED(expr)) { goto label; }
#define IfFailedGo(expr) IfFailedGoLabel(expr, LReturn)

#endif // defined(ENABLE_INTL_OBJECT) || defined(ENABLE_ES6_CHAR_CLASSIFIER)
