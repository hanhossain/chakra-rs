use crate::helpers::ScriptCache;
use crate::jsrt::{
    ChakraRt, JsError, JsModuleRecord, JsParseScriptAttributes, JsSourceContext, JsValueRef,
    JsValueType,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::{MODULE_ERROR_MAP, MODULE_RECORD_MAP, ModuleState, WScript};
use std::sync::atomic::{AtomicU32, Ordering};

static MESSAGE_COUNT: AtomicU32 = AtomicU32::new(0);

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("ChakraCore.h");
        /// Get the number of milliseconds since the system has started.
        fn GetTickCount() -> u32;
    }
}

#[derive(Eq, PartialEq)]
enum MessageInner {
    Callback(CallbackMessage),
    Module(ModuleMessage),
}

#[derive(Eq, PartialEq)]
pub struct Message {
    msg: MessageInner,
    time: u32,
    id: u32,
}

impl Message {
    fn new_id() -> u32 {
        MESSAGE_COUNT.fetch_add(1, Ordering::Relaxed)
    }
    pub(crate) fn new_callback(time: u32, function: JsValueRef) -> Box<Self> {
        Box::new(Self {
            msg: MessageInner::Callback(CallbackMessage::new(function)),
            time,
            id: Self::new_id(),
        })
    }
    pub(crate) fn new_module(
        module_record: JsModuleRecord,
        specifier: JsValueRef,
        full_path: Option<&str>,
    ) -> Box<Self> {
        Box::new(Self {
            msg: MessageInner::Module(ModuleMessage::new(module_record, specifier, full_path)),
            time: 0,
            id: Self::new_id(),
        })
    }

    #[tracing::instrument(skip(self, wscript))]
    fn call(&self, filename: &str, wscript: &WScript) {
        match &self.msg {
            MessageInner::Callback(msg) => msg.call(filename, wscript),
            MessageInner::Module(msg) => msg.call(filename, wscript),
        }
    }
    fn get_time(&self) -> u32 {
        self.time
    }
    fn begin_timer(&mut self) {
        self.time += ffi::GetTickCount();
    }
    pub(crate) fn get_id(&self) -> u32 {
        self.id
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
                let mut module_res = JsValueRef::default();
                unsafe {
                    ChakraRTInterface::JsModuleEvaluation(self.module_record, &raw mut module_res)
                        .as_result()?;
                }
            }
            return Ok(());
        }

        let specifier = self.specifier.to_string()?;

        let file_content = match &self.full_path {
            Some(path) => ScriptCache::get_script_with_full_path(&specifier, path),
            None => ScriptCache::get_script(&specifier),
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
