use crate::jsrt::JsValueRef;

pub struct JsErrorObject(JsValueRef);

impl AsRef<JsValueRef> for JsErrorObject {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

impl From<JsValueRef> for JsErrorObject {
    fn from(value: JsValueRef) -> Self {
        JsErrorObject(value)
    }
}
