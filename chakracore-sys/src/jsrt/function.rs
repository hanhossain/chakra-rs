use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsValueRef};

#[derive(Copy, Clone)]
pub struct JsFunction(pub(super) JsValueRef);

impl JsFunction {
    pub fn call_function(
        &self,
        arguments: &mut [JsValueRef],
    ) -> Result<Option<JsValueRef>, JsError> {
        let mut result = JsValueRef::default();
        unsafe {
            bridge::JsCallFunction(self.0, arguments, &raw mut result).as_result()?;
        }

        if result.is_null() {
            Ok(None)
        } else {
            Ok(Some(result))
        }
    }
}

impl From<JsValueRef> for JsFunction {
    fn from(value: JsValueRef) -> Self {
        Self(value)
    }
}

impl AsRef<JsValueRef> for JsFunction {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}
