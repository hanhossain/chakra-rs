pub use ffi::ChakraRTInterface;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ChakraRtInterface.h");

        type ChakraRTInterface;

        #[Self = "ChakraRTInterface"]
        fn InitializeTestHooks(vargs: &Vec<String>) -> i32;

        type JsErrorCode = crate::jsrt::JsErrorCode;
        type JsRuntimeAttributes = crate::jsrt::JsRuntimeAttributes;
        type JsParseScriptAttributes = crate::jsrt::JsParseScriptAttributes;
        type JsRuntimeHandle = crate::jsrt::JsRuntimeHandle;
        type JsContextRef = crate::jsrt::JsContextRef;
        type JsValueRef = crate::jsrt::JsValueRef;
        type JsSourceContext = crate::jsrt::JsSourceContext;
        type CULong = crate::jsrt::CULong;
        type CVoid = crate::jsrt::CVoid;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateRuntime(
            attributes: JsRuntimeAttributes,
            runtime: *mut JsRuntimeHandle,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        fn JsDisposeRuntime(runtime: JsRuntimeHandle) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateContext(
            runtime: JsRuntimeHandle,
            context: *mut JsContextRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        fn JsSetCurrentContext(context: JsContextRef) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsGetCurrentContext(context: *mut JsContextRef) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateExternalArrayBuffer(
            content: &str,
            result: *mut JsValueRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsSerialize(
            script: JsValueRef,
            buffer: *mut JsValueRef,
            parseAttributes: JsParseScriptAttributes,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsSerializeParserState(
            script: JsValueRef,
            buffer: *mut JsValueRef,
            parse_attributes: JsParseScriptAttributes,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsRunScriptWithParserState(
            script: JsValueRef,
            sourceContext: JsSourceContext,
            sourceUrl: JsValueRef,
            parseAttributes: JsParseScriptAttributes,
            parserState: JsValueRef,
            result: *mut JsValueRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsHasException(hasException: *mut bool) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsGetAndClearException(exception: *mut JsValueRef) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsSetPromiseContinuationCallback(
            callback: unsafe fn(task: JsValueRef, callbackState: *mut CVoid),
            callbackState: *mut CVoid,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateString(content: &String, value: *mut JsValueRef) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsRunSerialized(
            buffer: JsValueRef,
            callback: unsafe fn(
                sourceContext: JsSourceContext,
                value: *mut JsValueRef,
                parseAttributes: *mut JsParseScriptAttributes,
            ) -> bool,
            sourceContext: JsSourceContext,
            sourceUrl: JsValueRef,
            result: *mut JsValueRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsRun(
            script: JsValueRef,
            sourceContext: JsSourceContext,
            sourceUrl: JsValueRef,
            parseAttributes: JsParseScriptAttributes,
            result: *mut JsValueRef,
        ) -> JsErrorCode;

        #[Self = "ChakraRTInterface"]
        unsafe fn JsCreateObject(object: *mut JsValueRef) -> JsErrorCode;
    }
}

#[cfg(test)]
mod tests {
    use crate::jsrt::{JsErrorCode, JsRuntimeAttributes, JsRuntimeHandle};
    use crate::rt_interface::ChakraRTInterface;

    #[test]
    fn create_and_dispose_runtime() {
        let mut runtime = JsRuntimeHandle::default();
        unsafe {
            let res = ChakraRTInterface::JsCreateRuntime(
                JsRuntimeAttributes::JsRuntimeAttributeNone,
                &raw mut runtime,
            );
            assert_eq!(res, JsErrorCode::JsNoError);

            assert!(!runtime.is_invalid());

            let res = ChakraRTInterface::JsDisposeRuntime(runtime);
            assert_eq!(res, JsErrorCode::JsNoError);
        }
    }
}
