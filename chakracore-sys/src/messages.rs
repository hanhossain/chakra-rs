use crate::jsrt::run::JsRun;
use crate::jsrt::{
    ChakraRt, JsError, JsFunction, JsModule, JsParseScriptAttributes, JsRef, JsRefGuard,
    JsSourceContext, JsValueRef, JsValueType,
};
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
    pub(crate) fn new_callback(function: JsValueRef) -> Result<Self, JsError> {
        Ok(Self {
            msg: MessageInner::Callback(CallbackMessage::new(function)?),
        })
    }
    pub(crate) fn new_module(
        module_record: Option<JsModule>,
        specifier: JsValueRef,
        full_path: Option<&str>,
    ) -> Result<Self, JsError> {
        Ok(Self {
            msg: MessageInner::Module(ModuleMessage::new(module_record, specifier, full_path)?),
        })
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
    function_guard: JsRefGuard,
}

impl CallbackMessage {
    fn new(function: JsValueRef) -> Result<Self, JsError> {
        let function_guard = JsRef::add_ref(&function)?;
        Ok(Self {
            function,
            function_guard,
        })
    }

    #[tracing::instrument(skip(self), err)]
    fn internal_call(&self) -> Result<(), JsError> {
        if self.function.get_value_type()? == JsValueType::JsString {
            let string_value = self.function.to_string_value()?;
            let fname = ChakraRt::create_string("")?;
            string_value.run(
                JsSourceContext(usize::MAX),
                *fname,
                JsParseScriptAttributes::JsParseScriptAttributeArrayBufferIsUtf16Encoded,
            )?;
        } else {
            let global = ChakraRt::get_global_object()?;
            let func = JsFunction::from(self.function);
            func.call_function(&mut [*global])?;
        }
        Ok(())
    }

    #[tracing::instrument(skip(self, wscript))]
    fn call(&self, filename: &str, wscript: &WScript) {
        if let Err(err) = self.internal_call() {
            wscript.print_exception(filename, err.into(), JsValueRef::default());
        }
    }
}

#[derive(Eq, PartialEq)]
struct ModuleMessage {
    module_record: Option<JsModule>,
    specifier: JsValueRef,
    full_path: Option<String>,
    specifier_guard: Option<JsRefGuard>,
}

impl ModuleMessage {
    fn new(
        module_record: Option<JsModule>,
        specifier: JsValueRef,
        full_path: Option<&str>,
    ) -> Result<Self, JsError> {
        let mut path: Option<String> = None;
        let mut specifier_guard: Option<JsRefGuard> = None;
        if !specifier.is_null() {
            path = full_path.map(|x| x.to_owned());
            // nullptr specifier means a Promise to execute; non-nullptr means a "fetch" operation.
            specifier_guard = Some(JsRef::add_ref(&specifier)?);
        }
        Ok(Self {
            module_record,
            specifier,
            full_path: path,
            specifier_guard,
        })
    }

    #[tracing::instrument(skip(self, wscript), err)]
    fn internal_call(&self, wscript: &WScript) -> Result<(), JsError> {
        if self.specifier.is_null() {
            let state = {
                let error_map = MODULE_ERROR_MAP.0.read().unwrap();
                self.module_record
                    .as_ref()
                    .and_then(|record| error_map.get(record))
                    .map(|x| *x)
            };
            let Some(state) = state else {
                return Ok(());
            };
            if state != ModuleState::ErroredModule {
                self.module_record.as_ref().unwrap().evaluate()?;
            }
            return Ok(());
        }

        let specifier = self.specifier.to_string()?;

        let file_content = match &self.full_path {
            Some(path) => wscript.cache.get_script_with_full_path(&specifier, path),
            None => wscript.cache.get_script(&specifier),
        };

        let Err(err) = file_content.map(|content| {
            let path = self.full_path.as_ref().unwrap_or(&specifier);
            wscript.load_module_from_string(Some(&content), path, true)
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
        wscript.load_module_from_string(None, path, false)?;

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
