//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Helpers.h"

#include "ChakraRtInterface.h"
#include "WScriptJsrt.h"

const char* Helpers::JsErrorCodeToString(JsErrorCode jsErrorCode)
{
    bool hasException = false;
    ChakraRTInterface::JsHasException(&hasException);
    if (hasException)
    {
        WScriptJsrt::PrintException("", JsErrorScriptException);
    }

    switch (jsErrorCode)
    {
    case JsNoError:                            return "JsNoError";
    // JsErrorCategoryUsage
    case JsErrorCategoryUsage:                 return "JsErrorCategoryUsage";
    case JsErrorInvalidArgument:               return "JsErrorInvalidArgument";
    case JsErrorNullArgument:                  return "JsErrorNullArgument";
    case JsErrorNoCurrentContext:              return "JsErrorNoCurrentContext";
    case JsErrorInExceptionState:              return "JsErrorInExceptionState";
    case JsErrorNotImplemented:                return "JsErrorNotImplemented";
    case JsErrorWrongThread:                   return "JsErrorWrongThread";
    case JsErrorRuntimeInUse:                  return "JsErrorRuntimeInUse";
    case JsErrorBadSerializedScript:           return "JsErrorBadSerializedScript";
    case JsErrorInDisabledState:               return "JsErrorInDisabledState";
    case JsErrorCannotDisableExecution:        return "JsErrorCannotDisableExecution";
    case JsErrorHeapEnumInProgress:            return "JsErrorHeapEnumInProgress";
    case JsErrorArgumentNotObject:             return "JsErrorArgumentNotObject";
    case JsErrorInProfileCallback:             return "JsErrorInProfileCallback";
    case JsErrorInThreadServiceCallback:       return "JsErrorInThreadServiceCallback";
    case JsErrorCannotSerializeDebugScript:    return "JsErrorCannotSerializeDebugScript";
    case JsErrorAlreadyDebuggingContext:       return "JsErrorAlreadyDebuggingContext";
    case JsErrorAlreadyProfilingContext:       return "JsErrorAlreadyProfilingContext";
    case JsErrorIdleNotEnabled:                return "JsErrorIdleNotEnabled";
    case JsErrorInObjectBeforeCollectCallback: return "JsErrorInObjectBeforeCollectCallback";
    case JsErrorObjectNotInspectable:          return "JsErrorObjectNotInspectable";
    case JsErrorPropertyNotSymbol:             return "JsErrorPropertyNotSymbol";
    case JsErrorPropertyNotString:             return "JsErrorPropertyNotString";
    case JsErrorInvalidContext:                return "JsErrorInvalidContext";
    case JsInvalidModuleHostInfoKind:          return "JsInvalidModuleHostInfoKind";
    case JsErrorModuleParsed:                  return "JsErrorModuleParsed";
    // JsErrorCategoryEngine
    case JsErrorCategoryEngine:                return "JsErrorCategoryEngine";
    case JsErrorOutOfMemory:                   return "JsErrorOutOfMemory";
    case JsErrorBadFPUState:                   return "JsErrorBadFPUState";
    // JsErrorCategoryScript
    case JsErrorCategoryScript:                return "JsErrorCategoryScript";
    case JsErrorScriptException:               return "JsErrorScriptException";
    case JsErrorScriptCompile:                 return "JsErrorScriptCompile";
    case JsErrorScriptTerminated:              return "JsErrorScriptTerminated";
    case JsErrorScriptEvalDisabled:            return "JsErrorScriptEvalDisabled";
    // JsErrorCategoryFatal
    case JsErrorCategoryFatal:                 return "JsErrorCategoryFatal";
    case JsErrorFatal:                         return "JsErrorFatal";
    case JsErrorWrongRuntime:                  return "JsErrorWrongRuntime";
    // JsErrorCategoryDiagError
    case JsErrorCategoryDiagError:             return "JsErrorCategoryDiagError";
    case JsErrorDiagAlreadyInDebugMode:        return "JsErrorDiagAlreadyInDebugMode";
    case JsErrorDiagNotInDebugMode:            return "JsErrorDiagNotInDebugMode";
    case JsErrorDiagNotAtBreak:                return "JsErrorDiagNotAtBreak";
    case JsErrorDiagInvalidHandle:             return "JsErrorDiagInvalidHandle";
    case JsErrorDiagObjectNotFound:            return "JsErrorDiagObjectNotFound";
    case JsErrorDiagUnableToPerformAction:     return "JsErrorDiagUnableToPerformAction";
    default:
        return "<unknown>";
        break;
    }
}