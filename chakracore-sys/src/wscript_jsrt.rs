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
    }
}
