use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsValueRef};

pub struct JsArray(JsValueRef);

impl JsArray {
    pub(super) fn new(value: JsValueRef) -> Self {
        Self(value)
    }

    pub fn set_indexed_property<T: AsRef<JsValueRef>>(
        &mut self,
        index: &JsValueRef,
        value: T,
    ) -> Result<(), JsError> {
        bridge::JsSetIndexedProperty(self.0.clone(), index.clone(), value.as_ref().clone())
            .as_result()
    }
}

impl AsRef<JsValueRef> for JsArray {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}
