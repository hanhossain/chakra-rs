//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <filesystem>

#if DBG
#else // DBG
#pragma warning(disable: 4189)  // initialized but unused variable (e.g. variable that may only used by assert)
#endif

#define IfFailedReturn(EXPR) do { hr = (EXPR); if (FAILED(hr)) { return hr; }} while(FALSE)
#define IfFailedGoLabel(expr, label) do { hr = (expr); if (FAILED(hr)) { goto label; } } while (FALSE)
#define IfFailGo(expr) IfFailedGoLabel(hr = (expr), Error)

#define IfJsrtErrorFail(expr, ret) do { if ((expr) != JsNoError) return ret; } while (0)
#define IfJsrtErrorHR(expr) do { if((expr) != JsNoError) { hr = E_FAIL; goto Error; } } while(0)
#define IfJsrtErrorHRLabel(expr, label) do { if((expr) != JsNoError) { hr = E_FAIL; goto label; } } while(0)
#define IfJsrtError(expr) do { if((expr) != JsNoError) { goto Error; } } while(0)
#define IfJsrtErrorSetGo(expr) do { errorCode = (expr); if(errorCode != JsNoError) { hr = E_FAIL; goto Error; } } while(0)
#define IfJsrtErrorSetGoLabel(expr, label) do { errorCode = (expr); if(errorCode != JsNoError) { hr = E_FAIL; goto label; } } while(0)
#define IfFalseGo(expr) do { if(!(expr)) { hr = E_FAIL; goto Error; } } while(0)
#define IfFalseGoLabel(expr, label) do { if(!(expr)) { hr = E_FAIL; goto label; } } while(0)

#include "Interface/CommonDefines.h"
#include <map>
#include <string>
#include <print>

#include "PlatformAgnostic/CommonPal.h"

#include <stdarg.h>
#include <format>
#include "chakra/Logger.h"

#if defined(_DEBUG)
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#endif

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif

#ifdef Assert
#undef Assert
#endif

#ifdef AssertMsg
#undef AssertMsg
#endif

#if defined(DBG)

#if !defined(CHAKRACORE_STRINGIZE)
#define CHAKRACORE_STRINGIZE_IMPL(x) #x
#define CHAKRACORE_STRINGIZE(x) CHAKRACORE_STRINGIZE_IMPL(x)
#endif

#define AssertMsg(exp, comment)   \
do { \
if (!(exp)) \
{ \
    chakra::Logger::error(std::format("ASSERTION ({}, line {}) {} {}", __FILE__, __LINE__, CHAKRACORE_STRINGIZE(exp), comment)); \
    DebugBreak(); \
} \
} while (0)
#else
#define AssertMsg(exp, comment) ((void)0)
#endif //defined(DBG)

#define Assert(exp)             AssertMsg(exp, #exp)
