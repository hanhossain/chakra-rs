#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        type WScriptJsrt;

        #[Self = "WScriptJsrt"]
        fn Initialize() -> bool;

        #[Self = "WScriptJsrt"]
        fn Uninitialize() -> bool;

        type MessageQueue = crate::chhelper::ffi::MessageQueue;
        #[Self = "WScriptJsrt"]
        unsafe fn AddMessageQueue(messageQueue: *mut MessageQueue);

        type JsValueRef = crate::rt_interface::JsValueRef;
        type CVoid = crate::rt_interface::ffi::CVoid;
        #[Self = "WScriptJsrt"]
        unsafe fn PromiseContinuationCallback(task: JsValueRef, callbackState: *mut CVoid);
    }
}
