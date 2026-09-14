mod error;
mod ffi;

pub use error::*;
pub use ffi::*;

pub struct ChakraRt;

impl ChakraRt {
    pub fn create_object() -> Result<JsValueRef, JsError> {
        let mut object = JsValueRef::default();
        unsafe {
            bridge::JsCreateObject(&raw mut object).as_result()?;
        }
        Ok(object)
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

    pub fn set_property(
        object: &mut JsValueRef,
        property_id: JsPropertyIdRef,
        value: JsValueRef,
        use_strict_rules: bool,
    ) -> Result<(), JsError> {
        bridge::JsSetProperty(object.clone(), property_id, value, use_strict_rules).as_result()
    }
}
