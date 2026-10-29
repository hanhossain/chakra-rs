use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsRuntimeAttributes, JsRuntimeHandle};

pub struct JsRuntime {
    // TODO: don't expose handle
    pub handle: JsRuntimeHandle,
}

impl JsRuntime {
    pub fn create(attributes: JsRuntimeAttributes) -> Result<Self, JsError> {
        let mut handle = JsRuntimeHandle::default();
        unsafe {
            bridge::JsCreateRuntime(attributes, &raw mut handle).as_result()?;
        }
        Ok(Self { handle })
    }
}

impl Drop for JsRuntime {
    fn drop(&mut self) {
        if let Err(err) = bridge::JsDisposeRuntime(self.handle).as_result() {
            tracing::warn!(?err, "Failed to dispose runtime");
        }
    }
}
