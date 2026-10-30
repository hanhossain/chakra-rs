use crate::jsrt::ffi::bridge;
use crate::jsrt::{
    ChakraRt, IntoResponse, JsError, JsNativeFunctionArgs, JsPropertyIdRef, JsValueRef,
};
use std::ops::{Deref, DerefMut};

pub struct JsObject(JsValueRef);

impl JsObject {
    pub fn new(value: JsValueRef) -> Self {
        JsObject(value)
    }

    pub fn set_property<T: AsRef<JsValueRef>>(
        &mut self,
        property_id: JsPropertyIdRef,
        value: T,
        use_strict_rules: bool,
    ) -> Result<(), JsError> {
        bridge::JsSetProperty(
            self.0.clone(),
            property_id,
            value.as_ref().clone(),
            use_strict_rules,
        )
        .as_result()
    }

    pub fn set_named_function<T, R>(&mut self, name: &str, func: T) -> Result<(), JsError>
    where
        T: Fn(&JsNativeFunctionArgs) -> R,
        R: IntoResponse,
    {
        self.set_property(
            ChakraRt::create_property_id(name)?,
            ChakraRt::create_named_function(name, func)?,
            true,
        )
    }

    pub fn get_property(&self, property_id: JsPropertyIdRef) -> Result<JsValueRef, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsGetProperty(self.0, property_id, &raw mut value).as_result()?;
        }
        Ok(value)
    }
}

// TODO: this is temporary while other functions are ported. Don't actually want to deref to
//  JsValueRef.
impl Deref for JsObject {
    type Target = JsValueRef;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl DerefMut for JsObject {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

impl AsRef<JsValueRef> for JsObject {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

impl IntoResponse for JsObject {
    fn into_response(self) -> JsValueRef {
        self.0
    }
}
