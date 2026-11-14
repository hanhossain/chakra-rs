pub use ffi::ChakraRTInterface;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ChakraRtInterface.h");

        type ChakraRTInterface;

        type JsErrorCode = crate::jsrt::JsErrorCode;
        type JsRuntimeAttributes = crate::jsrt::JsRuntimeAttributes;
        type JsParseScriptAttributes = crate::jsrt::JsParseScriptAttributes;
        type JsRuntimeHandle = crate::jsrt::JsRuntimeHandle;
        type JsContextRef = crate::jsrt::JsContextRef;
        type JsValueRef = crate::jsrt::JsValueRef;
        type JsPropertyIdRef = crate::jsrt::JsPropertyIdRef;
        type JsSourceContext = crate::jsrt::JsSourceContext;
        type CULong = crate::jsrt::CULong;
        type CVoid = crate::jsrt::CVoid;
        type JsRef = crate::jsrt::JsRef;

        type JsModuleRecord = crate::jsrt::JsModuleRecord;
        type JsModuleHostInfoKind = crate::jsrt::JsModuleHostInfoKind;
        type JsSharedArrayBufferContentHandle = crate::jsrt::JsSharedArrayBufferContentHandle;
    }
}
