use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsContextRef, JsError, JsRuntimeAttributes, JsRuntimeHandle};

pub struct JsRuntime {
    handle: JsRuntimeHandle,
}

impl JsRuntime {
    pub fn create(attributes: JsRuntimeAttributes) -> Result<Self, JsError> {
        let mut handle = JsRuntimeHandle::default();
        unsafe {
            bridge::JsCreateRuntime(attributes, &raw mut handle).as_result()?;
        }
        Ok(Self { handle })
    }

    pub fn create_context(&self) -> Result<JsContextRef, JsError> {
        let mut context = JsContextRef::default();
        unsafe {
            bridge::JsCreateContext(self.handle, &raw mut context).as_result()?;
        }
        Ok(context)
    }
}

impl Drop for JsRuntime {
    fn drop(&mut self) {
        if let Err(err) = bridge::JsDisposeRuntime(self.handle).as_result() {
            tracing::warn!(?err, "Failed to dispose runtime");
        }
    }
}
