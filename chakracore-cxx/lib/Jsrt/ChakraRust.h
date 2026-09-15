#pragma once

#include "ChakraCommon.h"
#include "chakracore-sys/src/jsrt/ffi.rs.h"

namespace chakracore::jsrt
{
    /// <summary>
    ///     Creates a new JavaScript function with name.
    /// </summary>
    /// <remarks>
    ///     Requires an active script context.
    /// </remarks>
    /// <param name="name">
    ///     The name of this function that will be used for diagnostics and stringification purposes.
    /// </param>
    /// <param name="nativeFunction">The method to call when the function is invoked.</param>
    /// <param name="function">The new function object.</param>
    /// <returns>
    ///     The code <c>JsNoError</c> if the operation succeeded, a failure code otherwise.
    /// </returns>
    JsErrorCode JsCreateNamedFunction(JsValueRef name, rust::Fn<JsValueRef(const chakra_rs::JsNativeFunctionArgs &)> nativeFunction, JsValueRef *function);
}