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

    pub fn storage_mut(&self) -> Result<&mut [u8], JsError> {
        let buffer = unsafe {
            let mut buffer: *mut u8 = std::ptr::null_mut();
            let mut buffer_length = 0;
            bridge::JsGetArrayBufferStorage(self.0, &raw mut buffer, &raw mut buffer_length)
                .as_result()?;
            std::slice::from_raw_parts_mut(buffer, buffer_length as usize)
        };
        Ok(buffer)
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
