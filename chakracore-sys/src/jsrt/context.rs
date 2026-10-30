use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsContextRef, JsError};

pub struct JsContext(pub(super) JsContextRef);

impl JsContext {
    pub fn set_current(self) -> Result<JsContextGuard, JsError> {
        let mut previous = JsContextRef::default();
        unsafe {
            bridge::JsGetCurrentContext(&raw mut previous).as_result()?;
        }
        bridge::JsSetCurrentContext(self.0).as_result()?;

        let previous = if previous.is_null() {
            None
        } else {
            Some(previous)
        };

        Ok(JsContextGuard { previous })
    }
}

pub struct JsContextGuard {
    previous: Option<JsContextRef>,
}

impl Drop for JsContextGuard {
    fn drop(&mut self) {
        let previous = self.previous.unwrap_or_default();
        if let Err(err) = bridge::JsSetCurrentContext(previous).as_result() {
            tracing::warn!(?err, "Failed to reset context");
        }
    }
}
