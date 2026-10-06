use crate::jsrt::{ChakraRt, JsErrorCode, JsValueRef};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::WScript;

#[cxx::bridge]
mod ffi {
    extern "C++" {
        include!("MessageQueue.h");
        type JsValueRef = crate::jsrt::JsValueRef;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type CallbackMessage;
        #[Self = "CallbackMessage"]
        fn boxed_new(function: JsValueRef) -> Box<CallbackMessage>;
        fn get_function(&self) -> JsValueRef;
    }
}

struct CallbackMessage {
    function: JsValueRef,
}

impl CallbackMessage {
    fn new(function: JsValueRef) -> Self {
        unsafe {
            ChakraRTInterface::JsAddRef(function.as_js_ref(), std::ptr::null_mut())
                .as_result()
                .unwrap();
        }
        Self { function }
    }

    fn boxed_new(function: JsValueRef) -> Box<Self> {
        Box::new(Self::new(function))
    }

    fn get_function(&self) -> JsValueRef {
        self.function
    }
}

impl Drop for CallbackMessage {
    fn drop(&mut self) {
        if ChakraRt::has_exception().unwrap_or_default() {
            WScript::print_exception(
                "",
                JsErrorCode::JsErrorScriptException,
                JsValueRef::default(),
            );
        }

        unsafe {
            ChakraRTInterface::JsRelease(self.function.as_js_ref(), std::ptr::null_mut())
                .as_result()
                .unwrap();
        }
    }
}
