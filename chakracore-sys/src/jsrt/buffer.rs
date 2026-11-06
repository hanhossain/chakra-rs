use crate::jsrt::ffi::bridge;
use crate::jsrt::{IntoResponse, JsError, JsValueRef};

pub struct JsArrayBuffer(JsValueRef);

impl JsArrayBuffer {
    pub fn create(length: u32) -> Result<Self, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateArrayBuffer(length, &raw mut value).as_result()?;
        }
        Ok(Self(value))
    }
}

impl AsRef<JsValueRef> for JsArrayBuffer {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

impl IntoResponse for JsArrayBuffer {
    fn into_response(self) -> JsValueRef {
        self.0
    }
}
