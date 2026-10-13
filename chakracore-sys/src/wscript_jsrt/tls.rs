use crate::jsrt::{JsContextRef, JsError, JsRuntimeAttributes, JsRuntimeHandle};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::WScript;
use crate::wscript_jsrt::ffi::RuntimeThreadData;
use cxx::SharedPtr;

pub(super) struct ThreadData {
    pub(super) inner: SharedPtr<RuntimeThreadData>,
}

impl ThreadData {
    pub(super) fn new() -> Self {
        let inner = RuntimeThreadData::New();
        Self { inner }
    }

    pub(super) fn new_with_initial_source(source: &str) -> Self {
        let inner = RuntimeThreadData::NewWithInitialSource(&source.to_owned());
        Self { inner }
    }

    pub(super) fn wait_initial_script_completed(&self) {
        let mut inner = self.inner.clone();
        unsafe {
            inner.pin_mut_unchecked().wait_initial_script_completed();
        }
    }

    #[tracing::instrument(skip(self, wscript), err)]
    pub(super) fn thread_proc(&self, wscript: &WScript) -> Result<u32, JsError> {
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
        let res = self.inner.ThreadProc(wscript, runtime);
        Ok(res)
    }
}
