use crate::jsrt::{
    ChakraRt, JsError, JsModuleRecord, JsParseScriptAttributes, JsSourceContext, JsValueRef,
    JsValueType,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::{MODULE_ERROR_MAP, MODULE_RECORD_MAP, ModuleState, WScript};

#[derive(Eq, PartialEq)]
enum MessageInner {
    Callback(CallbackMessage),
    Module(ModuleMessage),
}

#[derive(Eq, PartialEq)]
pub struct Message {
    msg: MessageInner,
}

impl Message {
    pub(crate) fn new_callback(function: JsValueRef) -> Self {
        Self {
            msg: MessageInner::Callback(CallbackMessage::new(function)),
        }
    }
    pub(crate) fn new_module(
        module_record: JsModuleRecord,
        specifier: JsValueRef,
        full_path: Option<&str>,
    ) -> Self {
        Self {
            msg: MessageInner::Module(ModuleMessage::new(module_record, specifier, full_path)),
        }
    }

    #[tracing::instrument(skip(self, wscript))]
    pub(crate) fn call(&self, filename: &str, wscript: &WScript) {
        match &self.msg {
            MessageInner::Callback(msg) => msg.call(filename, wscript),
            MessageInner::Module(msg) => msg.call(filename, wscript),
        }
    }
}

#[derive(Eq, PartialEq)]
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

    #[tracing::instrument(skip(self, wscript), err)]
    fn internal_call(&self, filename: &str, wscript: &WScript) -> Result<(), JsError> {
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
            wscript.print_exception(filename, err.into(), JsValueRef::default());
        }

        Ok(())
    }

    fn call(&self, filename: &str, wscript: &WScript) {
        let _ = self.internal_call(filename, wscript);
    }
}

impl Drop for CallbackMessage {
    fn drop(&mut self) {
        unsafe {
            ChakraRTInterface::JsRelease(self.function.as_js_ref(), std::ptr::null_mut())
                .as_result()
                .unwrap();
        }
    }
}

#[derive(Eq, PartialEq)]
struct ModuleMessage {
    module_record: JsModuleRecord,
    specifier: JsValueRef,
    full_path: Option<String>,
}

impl ModuleMessage {
    fn new(module_record: JsModuleRecord, specifier: JsValueRef, full_path: Option<&str>) -> Self {
        let mut path: Option<String> = None;
        unsafe {
            ChakraRTInterface::JsAddRef(module_record.as_js_ref(), std::ptr::null_mut());
        }
        if !specifier.is_null() {
            path = full_path.map(|x| x.to_owned());
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

    #[tracing::instrument(skip(self, wscript), err)]
    fn internal_call(&self, wscript: &WScript) -> Result<(), JsError> {
        if self.specifier.is_null() {
            let state = {
                let error_map = MODULE_ERROR_MAP.0.read().unwrap();
                error_map.get(&self.module_record).map(|x| *x)
            };
            let Some(state) = state else {
                return Ok(());
            };
            if state != ModuleState::ErroredModule {
                ChakraRt::module_evaluation(self.module_record)?;
            }
            return Ok(());
        }

        let specifier = self.specifier.to_string()?;

        let file_content = match &self.full_path {
            Some(path) => wscript.cache.get_script_with_full_path(&specifier, path),
            None => wscript.cache.get_script(&specifier),
        };

        let Err(err) = file_content.map(|content| {
            let content = Some(content.as_str()).into();
            let path = self.full_path.as_ref().unwrap_or(&specifier);
            wscript
                .load_module_from_string(content, path, true)
                .as_result()
        }) else {
            return Ok(());
        };

        if !wscript.config.host.mute_host_error_msg {
            let actual_record = MODULE_RECORD_MAP
                .0
                .read()
                .unwrap()
                .get(self.full_path.as_ref().unwrap())
                .cloned();
            let state = actual_record
                .map(|entry| {
                    MODULE_ERROR_MAP
                        .0
                        .read()
                        .unwrap()
                        .get(&entry.record)
                        .cloned()
                })
                .flatten();
            match state {
                Some(ModuleState::RootModule) | None => {
                    tracing::error!(?err, specifier, "Couldn't load file");
                }
                _ => {}
            };
        }
        let path = self.full_path.as_ref().unwrap_or(&specifier);
        wscript.load_module_from_string(None.into(), path, false);

        Ok(())
    }

    #[tracing::instrument(skip(self, wscript))]
    fn call(&self, filename: &str, wscript: &WScript) {
        let Err(err) = self.internal_call(wscript) else {
            return;
        };
        wscript.print_exception(filename, err.into(), JsValueRef::default());
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
