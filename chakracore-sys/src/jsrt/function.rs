use crate::jsrt::JsValueRef;

pub struct JsFunction(pub(super) JsValueRef);

impl AsRef<JsValueRef> for JsFunction {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}
