use crate::helpers::ScriptCache;
use crate::host_config::HostConfigFlags;
use crate::jsrt::{
    ChakraRt, JsError, JsErrorCode, JsModuleRecord, JsParseScriptAttributes, JsSourceContext,
    JsValueRef, JsValueType,
};
use crate::rt_interface::ChakraRTInterface;
use crate::str_helper::OptionalStr;
use crate::wscript_jsrt::{MODULE_ERROR_MAP, MODULE_RECORD_MAP, ModuleState, WScript};
use std::sync::atomic::{AtomicU32, Ordering};

static MESSAGE_COUNT: AtomicU32 = AtomicU32::new(0);

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("MessageQueue.h");
        include!("ChakraCore.h");
        type JsValueRef = crate::jsrt::JsValueRef;
        type JsModuleRecord = crate::jsrt::JsModuleRecord;
        /// Get the number of milliseconds since the system has started.
        fn GetTickCount() -> u32;
    }

    #[namespace = "chakra_rs"]
    extern "C++" {
        include!("chakracore-sys/src/str_helper.rs.h");
        type OptionalStr<'a> = crate::str_helper::OptionalStr<'a>;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        #[derive(ExternType)]
        type Message;
        #[Self = "Message"]
        fn new_callback(msg: Box<CallbackMessage>) -> Box<Message>;
        #[Self = "Message"]
        fn new_module(msg: Box<ModuleMessage>) -> Box<Message>;
        fn call(&self, filename: &str);
        fn get_time(&self) -> u32;
        fn begin_timer(&mut self);
        fn get_id(&self) -> u32;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type CallbackMessage;
        #[Self = "CallbackMessage"]
        fn boxed_new(time: u32, function: JsValueRef) -> Box<CallbackMessage>;
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
    }
}

enum MessageInner {
    Callback(Box<CallbackMessage>),
    Module(Box<ModuleMessage>),
}

pub(crate) struct Message {
    msg: MessageInner,
}

impl Message {
    pub(crate) fn new_callback(msg: Box<CallbackMessage>) -> Box<Self> {
        Box::new(Self {
            msg: MessageInner::Callback(msg),
        })
    }
    pub(crate) fn new_module(msg: Box<ModuleMessage>) -> Box<Self> {
        Box::new(Self {
            msg: MessageInner::Module(msg),
        })
    }
    fn call(&self, filename: &str) {
        match &self.msg {
            MessageInner::Callback(msg) => msg.call(filename),
            MessageInner::Module(msg) => msg.call(filename),
        }
    }
    fn get_time(&self) -> u32 {
        match &self.msg {
            MessageInner::Callback(msg) => msg.get_time(),
            MessageInner::Module(msg) => msg.get_time(),
        }
    }
    fn begin_timer(&mut self) {
        match &mut self.msg {
            MessageInner::Callback(msg) => msg.begin_timer(),
            MessageInner::Module(msg) => msg.begin_timer(),
        }
    }
    fn get_id(&self) -> u32 {
        match &self.msg {
            MessageInner::Callback(msg) => msg.get_id(),
            MessageInner::Module(msg) => msg.get_id(),
        }
    }
}

pub(crate) struct CallbackMessage {
    function: JsValueRef,
    time: u32,
    id: u32,
}

impl CallbackMessage {
    fn new(time: u32, function: JsValueRef) -> Self {
        let id = MESSAGE_COUNT.fetch_add(1, Ordering::Relaxed);
        unsafe {
            ChakraRTInterface::JsAddRef(function.as_js_ref(), std::ptr::null_mut())
                .as_result()
                .unwrap();
        }
        Self { time, function, id }
    }

    pub(crate) fn boxed_new(time: u32, function: JsValueRef) -> Box<Self> {
        Box::new(Self::new(time, function))
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

    fn get_time(&self) -> u32 {
        self.time
    }

    fn begin_timer(&mut self) {
        self.time += ffi::GetTickCount();
    }

    fn get_id(&self) -> u32 {
        self.id
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

pub(crate) struct ModuleMessage {
    module_record: JsModuleRecord,
    specifier: JsValueRef,
    full_path: Option<String>,
    time: u32,
    id: u32,
}

impl ModuleMessage {
    fn new(module_record: JsModuleRecord, specifier: JsValueRef, full_path: Option<&str>) -> Self {
        let id = MESSAGE_COUNT.fetch_add(1, Ordering::Relaxed);
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
            time: 0,
            id,
        }
    }

    pub(crate) fn boxed_new(
        module_record: JsModuleRecord,
        specifier: JsValueRef,
        full_path: OptionalStr,
    ) -> Box<Self> {
        Box::new(Self::new(module_record, specifier, full_path.into()))
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

        let specifier = self.specifier.to_string()?;

        let file_content = match &self.full_path {
            Some(path) => ScriptCache::get_script_with_full_path(&specifier, path),
            None => ScriptCache::get_script(&specifier),
        };

        let Err(err) = file_content.map(|content| {
            let content = Some(content.as_str()).into();
            let path = self.full_path.as_ref().unwrap_or(&specifier);
            WScript::load_module_from_string(content, path, true).as_result()
        }) else {
            return Ok(());
        };

        if !HostConfigFlags::GetConfig().host.mute_host_error_msg {
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
        WScript::load_module_from_string(None.into(), path, false);

        Ok(())
    }

    #[tracing::instrument(skip(self))]
    fn call(&self, filename: &str) {
        let Err(err) = self.internal_call() else {
            return;
        };
        WScript::print_exception(filename, err.into(), JsValueRef::default());
    }

    fn get_time(&self) -> u32 {
        self.time
    }

    fn begin_timer(&mut self) {
        self.time += ffi::GetTickCount();
    }

    fn get_id(&self) -> u32 {
        self.id
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
