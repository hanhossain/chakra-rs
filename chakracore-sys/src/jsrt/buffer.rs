use crate::jsrt::ffi::bridge;
use crate::jsrt::run::{JsRun, JsRunSerialized};
use crate::jsrt::{IntoResponse, JsError, JsSharedArrayBufferContentHandle, JsValueRef};

pub struct JsArrayBuffer(JsValueRef);

impl JsArrayBuffer {
    pub(super) fn new(value: JsValueRef) -> Self {
        Self(value)
    }

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

impl JsRunSerialized for JsArrayBuffer {}

#[derive(Eq, PartialEq)]
pub struct JsSharedArrayBufferContent {
    handle: JsSharedArrayBufferContentHandle,
}

impl JsSharedArrayBufferContent {
    pub fn create_shared_array_buffer(&self) -> Result<JsValueRef, JsError> {
        let mut shared_buffer = JsValueRef::default();
        unsafe {
            bridge::JsCreateSharedArrayBufferWithSharedContent(
                self.handle.clone(),
                &raw mut shared_buffer,
            )
            .as_result()?;
        }
        Ok(shared_buffer)
    }
}

impl TryFrom<JsValueRef> for JsSharedArrayBufferContent {
    type Error = JsError;

    fn try_from(value: JsValueRef) -> Result<Self, Self::Error> {
        let mut handle = JsSharedArrayBufferContentHandle::default();
        unsafe {
            bridge::JsGetSharedArrayBufferContent(value, &raw mut handle).as_result()?;
        }
        Ok(Self { handle })
    }
}

impl Drop for JsSharedArrayBufferContent {
    fn drop(&mut self) {
        bridge::JsReleaseSharedArrayBufferContentHandle(self.handle.clone())
            .as_result()
            .unwrap();
    }
}

pub struct JsExternalArrayBuffer(JsValueRef);

impl JsExternalArrayBuffer {
    pub fn create(content: &str) -> Result<Self, JsError> {
        let mut res = JsValueRef::default();
        unsafe {
            bridge::JsCreateExternalArrayBuffer(content, &raw mut res).as_result()?;
        }
        Ok(Self(res))
    }
}

impl AsRef<JsValueRef> for JsExternalArrayBuffer {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

impl JsRun for JsExternalArrayBuffer {}
impl JsRunSerialized for JsExternalArrayBuffer {}
