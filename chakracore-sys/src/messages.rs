use crate::jsrt::{
    ChakraRt, JsError, JsErrorCode, JsModuleRecord, JsParseScriptAttributes, JsSourceContext,
    JsValueRef, JsValueType,
};
use crate::rt_interface::ChakraRTInterface;
use crate::str_helper::OptionalStr;
use crate::wscript_jsrt::{MODULE_ERROR_MAP, ModuleState, WScript};
use std::path::PathBuf;

#[cxx::bridge]
mod ffi {
    extern "C++" {
        include!("MessageQueue.h");
        include!("ChakraCore.h");
        type JsValueRef = crate::jsrt::JsValueRef;
        type JsModuleRecord = crate::jsrt::JsModuleRecord;
    }

    #[namespace = "chakra_rs"]
    extern "C++" {
        include!("chakracore-sys/src/str_helper.rs.h");
        type OptionalStr<'a> = crate::str_helper::OptionalStr<'a>;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type CallbackMessage;
        #[Self = "CallbackMessage"]
        fn boxed_new(function: JsValueRef) -> Box<CallbackMessage>;
        fn call(&self, filename: &str);
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type ModuleMessage;
        #[Self = "ModuleMessage"]
        fn boxed_new(
            module_record: JsModuleRecord,
            specifier: JsValueRef,
            full_path: OptionalStr,
        ) -> Box<ModuleMessage>;
        fn get_specifier(&self) -> JsValueRef;
        fn get_module_record(&self) -> JsModuleRecord;
        fn has_full_path(&self) -> bool;
        fn get_full_path(&self) -> String;
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

struct ModuleMessage {
    module_record: JsModuleRecord,
    specifier: JsValueRef,
    full_path: Option<PathBuf>,
}

impl ModuleMessage {
    fn new(module_record: JsModuleRecord, specifier: JsValueRef, full_path: Option<&str>) -> Self {
        let mut path: Option<PathBuf> = None;
        unsafe {
            ChakraRTInterface::JsAddRef(module_record.as_js_ref(), std::ptr::null_mut());
        }
        if !specifier.is_null() {
            path = full_path.map(|x| PathBuf::from(x));
            // nullptr specifier means a Promise to execute; non-nullptr means a "fetch" operation.
            unsafe {
                ChakraRTInterface::JsAddRef(specifier.as_js_ref(), std::ptr::null_mut());
            }
        }
        Self {
            module_record,
            specifier,
            full_path: path,
        }
    }

    fn boxed_new(
        module_record: JsModuleRecord,
        specifier: JsValueRef,
        full_path: OptionalStr,
    ) -> Box<Self> {
        Box::new(Self::new(module_record, specifier, full_path.into()))
    }

    fn get_specifier(&self) -> JsValueRef {
        self.specifier
    }

    fn get_module_record(&self) -> JsModuleRecord {
        self.module_record
    }

    fn has_full_path(&self) -> bool {
        self.full_path.is_some()
    }

    fn get_full_path(&self) -> String {
        self.full_path
            .as_ref()
            .map(|x| x.to_str().map(|x| x.to_owned()))
            .flatten()
            .unwrap()
    }

    #[tracing::instrument(skip(self), err)]
    fn internal_call(&self) -> Result<(), JsError> {
        if self.specifier.is_null() {
            let state = {
                let error_map = MODULE_ERROR_MAP.0.read().unwrap();
                error_map.get(&self.module_record).map(|x| *x)
            };
            let Some(state) = state else {
                return Ok(());
            };
            if state != ModuleState::ErroredModule {
                let mut module_res = JsValueRef::default();
                unsafe {
                    ChakraRTInterface::JsModuleEvaluation(self.module_record, &raw mut module_res)
                        .as_result()?;
                }
            }
            return Ok(());
        }

        Ok(())
    }

    #[tracing::instrument(skip(self))]
    fn call(&self, filename: &str) {
        let Err(err) = self.internal_call() else {
            return;
        };
        WScript::print_exception(filename, err.into(), JsValueRef::default());
    }
}

impl Drop for ModuleMessage {
    fn drop(&mut self) {
        unsafe {
            ChakraRTInterface::JsRelease(self.module_record.as_js_ref(), std::ptr::null_mut());
            if !self.specifier.is_null() {
                ChakraRTInterface::JsRelease(self.specifier.as_js_ref(), std::ptr::null_mut());
            }
        }
    }
}
