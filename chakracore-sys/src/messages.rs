use crate::jsrt::{
    ChakraRt, JsError, JsErrorCode, JsParseScriptAttributes, JsSourceContext, JsValueRef,
    JsValueType,
};
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
        fn call(&self, filename: &str);
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

    #[tracing::instrument(skip(self), err)]
    fn internal_call(&self, filename: &str) -> Result<(), JsError> {
        let res = if self.function.get_value_type()? == JsValueType::JsString {
            unsafe {
                let mut string_value = JsValueRef::default();
                ChakraRTInterface::JsConvertValueToString(self.function, &raw mut string_value)
                    .as_result()?;
                let fname = ChakraRt::create_string("")?;
                ChakraRTInterface::JsRun(
                    string_value,
                    JsSourceContext(usize::MAX),
                    *fname,
                    JsParseScriptAttributes::JsParseScriptAttributeArrayBufferIsUtf16Encoded,
                    std::ptr::null_mut(),
                )
                .as_result()
            }
        } else {
            let mut global = ChakraRt::get_global_object()?;
            let mut result = JsValueRef::default();
            unsafe {
                ChakraRTInterface::JsCallFunction(
                    self.function,
                    &raw mut *global,
                    1,
                    &raw mut result,
                )
                .as_result()
            }
        };

        if let Err(err) = res {
            WScript::print_exception(filename, err.into(), JsValueRef::default());
        }

        Ok(())
    }

    fn call(&self, filename: &str) {
        let _ = self.internal_call(filename);
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
