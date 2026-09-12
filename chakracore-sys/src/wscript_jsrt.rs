use crate::rt_interface::ffi::ChakraRTInterface;
use crate::rt_interface::{JsError, JsErrorExt, JsValueRef};

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        include!("PlatformAgnostic/ChakraICU.h");

        type WScriptJsrt;

        #[Self = "WScriptJsrt"]
        fn Initialize(icu_version: i32, wscript: JsValueRef) -> bool;

        #[Self = "WScriptJsrt"]
        fn Uninitialize() -> bool;

        type MessageQueue = crate::chhelper::ffi::MessageQueue;
        #[Self = "WScriptJsrt"]
        unsafe fn AddMessageQueue(messageQueue: *mut MessageQueue);

        type JsValueRef = crate::rt_interface::JsValueRef;
        type CVoid = crate::rt_interface::ffi::CVoid;
        #[Self = "WScriptJsrt"]
        unsafe fn PromiseContinuationCallback(task: JsValueRef, callbackState: *mut CVoid);

        #[Self = "WScriptJsrt"]
        fn GetNextSourceContext() -> usize;

        type JsErrorCode = crate::rt_interface::ffi::JsErrorCode;
        #[Self = "WScriptJsrt"]
        fn ModuleEntryPoint(fileContent: &str, fullName: &String) -> JsErrorCode;

        #[Self = "WScriptJsrt"]
        fn PrintException(filname: &str, jsErrorCode: JsErrorCode, exception: JsValueRef) -> bool;

        #[namespace = "PlatformAgnostic::ICUHelpers"]
        fn GetICUMajorVersion() -> i32;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type WScript;

        #[Self = "WScript"]
        fn initialize() -> Result<()>;
    }
}

pub struct WScript;

impl WScript {
    #[tracing::instrument(err)]
    pub fn initialize() -> Result<(), JsError> {
        let icu_version = ffi::GetICUMajorVersion();

        let mut wscript_object = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateObject(&raw mut wscript_object).as_result()?;
        }

        if !ffi::WScriptJsrt::Initialize(icu_version, wscript_object) {
            return Err(JsError::JsErrorFatal);
        }

        Ok(())
    }
}
