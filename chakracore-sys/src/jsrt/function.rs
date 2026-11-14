use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsRef, JsRefGuard, JsValueRef};
use std::fmt::Formatter;
use std::ops::Deref;

#[derive(Eq, PartialEq)]
pub struct JsFunction {
    value: JsValueRef,
    guard: Option<JsRefGuard>,
}

impl JsFunction {
    pub(super) fn new(value: JsValueRef) -> Self {
        Self { value, guard: None }
    }

    pub fn new_ref(value: JsValueRef) -> Result<Self, JsError> {
        let guard = Some(JsRef::add_ref(&value)?);
        Ok(Self { value, guard })
    }
    pub fn call_function(
        &self,
        arguments: &mut [JsValueRef],
    ) -> Result<Option<JsValueRef>, JsError> {
        let mut result = JsValueRef::default();
        unsafe {
            bridge::JsCallFunction(self.value, arguments, &raw mut result).as_result()?;
        }

        if result.is_null() {
            Ok(None)
        } else {
            Ok(Some(result))
        }
    }
}

impl<T> AsRef<T> for JsFunction
where
    T: ?Sized,
    <JsFunction as Deref>::Target: AsRef<T>,
{
    fn as_ref(&self) -> &T {
        self.deref().as_ref()
    }
}

impl Deref for JsFunction {
    type Target = JsValueRef;

    fn deref(&self) -> &Self::Target {
        &self.value
    }
}

impl std::fmt::Debug for JsFunction {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("JsFunction")
            .field("value", &self.value)
            .finish()
    }
}
