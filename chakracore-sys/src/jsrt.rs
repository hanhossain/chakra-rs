mod error;
mod ffi;

pub use error::*;
pub use ffi::*;
use std::ops::{Deref, DerefMut};

pub struct ChakraRt;

impl ChakraRt {
    pub fn create_object() -> Result<JsObject, JsError> {
        let mut object = JsValueRef::default();
        unsafe {
            bridge::JsCreateObject(&raw mut object).as_result()?;
        }
        Ok(JsObject(object))
    }

    pub fn get_global_object() -> Result<JsObject, JsError> {
        let mut object = JsValueRef::default();
        unsafe {
            bridge::JsGetGlobalObject(&raw mut object).as_result()?;
        }
        Ok(JsObject(object))
    }

    pub fn create_property_id(name: &str) -> Result<JsPropertyIdRef, JsError> {
        let mut property = JsPropertyIdRef::default();
        unsafe {
            bridge::JsCreatePropertyId(name, &raw mut property).as_result()?;
        }

        Ok(property)
    }

    pub fn create_string(name: &str) -> Result<JsValueRef, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateString(name, &raw mut value).as_result()?;
        }
        Ok(value)
    }

    pub fn int_to_number(value: i32) -> Result<JsValueRef, JsError> {
        let mut value_ref = JsValueRef::default();
        unsafe {
            bridge::JsIntToNumber(value, &raw mut value_ref).as_result()?;
        }
        Ok(value_ref)
    }
}

pub struct JsObject(JsValueRef);

impl JsObject {
    pub fn set_property(
        &mut self,
        property_id: JsPropertyIdRef,
        value: &JsValueRef,
        use_strict_rules: bool,
    ) -> Result<(), JsError> {
        bridge::JsSetProperty(self.0.clone(), property_id, value.clone(), use_strict_rules)
            .as_result()
    }
}

impl From<JsObject> for JsValueRef {
    fn from(value: JsObject) -> Self {
        value.0
    }
}

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
