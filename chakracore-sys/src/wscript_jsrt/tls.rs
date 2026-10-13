use crate::jsrt::{
    ChakraRt, JsContextRef, JsError, JsParseScriptAttributes, JsRuntimeAttributes, JsRuntimeHandle,
    JsSourceContext, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::WScript;
use crate::wscript_jsrt::ffi::RuntimeThreadData;
use cxx::SharedPtr;
use tokio::sync::oneshot;

pub(super) struct ThreadData {
    pub(super) inner: SharedPtr<RuntimeThreadData>,
    initial_source: String,
}

impl ThreadData {
    pub(super) fn new() -> Self {
        let inner = RuntimeThreadData::New();
        Self {
            inner,
            initial_source: String::new(),
        }
    }

    pub(super) fn new_with_initial_source(source: &str) -> Self {
        let initial_source = source.to_owned();
        let inner = RuntimeThreadData::New();
        Self {
            inner,
            initial_source,
        }
    }

    #[tracing::instrument(skip(self, wscript), err)]
    pub(super) fn thread_proc(
        &self,
        wscript: &WScript,
        tx_initial_script_completed: oneshot::Sender<()>,
    ) -> Result<u32, ThreadProcError> {
        let mut runtime = JsRuntimeHandle::default();
        unsafe {
            ChakraRTInterface::JsCreateRuntime(
                JsRuntimeAttributes::JsRuntimeAttributeNone,
                &raw mut runtime,
            )
            .as_result()?;
        }

        let mut context = JsContextRef::default();
        unsafe {
            ChakraRTInterface::JsCreateContext(runtime, &raw mut context).as_result()?;
            ChakraRTInterface::JsSetCurrentContext(context).as_result()?;
        }
        wscript.initialize()?;

        let mut script_source = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateExternalArrayBuffer(
                &self.initial_source,
                &raw mut script_source,
            )
            .as_result()?;
        }
        let full_path = ChakraRt::create_string("agent source")?;

        unsafe {
            ChakraRTInterface::JsRun(
                script_source,
                JsSourceContext(WScript::get_next_source_context()),
                *full_path,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                std::ptr::null_mut(),
            )
            .as_result()?;
        }

        tx_initial_script_completed
            .send(())
            .map_err(|_| ThreadProcError::NotificationFailed)?;

        let res = self.inner.ThreadProc(runtime);
        Ok(res)
    }
}

#[derive(thiserror::Error, Debug)]
pub(super) enum ThreadProcError {
    #[error(transparent)]
    JsError(#[from] JsError),
    #[error("Failed to notify parent of initial script completion")]
    NotificationFailed,
}
