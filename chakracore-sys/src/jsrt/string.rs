use crate::jsrt::{IntoResponse, JsValueRef};
use std::ops::{Deref, DerefMut};

#[derive(Default)]
pub struct JsString(JsValueRef);

impl JsString {
    pub(super) fn new(value: JsValueRef) -> Self {
        Self(value)
    }
}

impl AsRef<JsValueRef> for JsString {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

// TODO: this is temporary while other functions are ported. Don't actually want to deref to
//  JsValueRef.
impl Deref for JsString {
    type Target = JsValueRef;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl DerefMut for JsString {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

impl IntoResponse for JsString {
    fn into_response(self) -> JsValueRef {
        self.0
    }
}
