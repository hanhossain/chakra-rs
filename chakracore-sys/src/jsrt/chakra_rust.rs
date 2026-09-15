#[cxx::bridge]
pub(crate) mod bridge {
    extern "C++" {
        include!("ChakraRust.h");

        type JsErrorCode = crate::jsrt::JsErrorCode;
        type JsValueRef = crate::jsrt::JsValueRef;

        #[namespace = "chakra_rs"]
        type JsNativeFunctionArgs<'a> = crate::jsrt::JsNativeFunctionArgs<'a>;

        #[namespace = "chakracore::jsrt"]
        unsafe fn JsCreateNamedFunction(
            name: JsValueRef,
            native_function: fn(&JsNativeFunctionArgs) -> JsValueRef,
            function: *mut JsValueRef,
        ) -> JsErrorCode;
    }
}
