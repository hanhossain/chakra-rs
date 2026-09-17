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

    pub fn create_string(name: &str) -> Result<JsString, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateString(name, &raw mut value).as_result()?;
        }
        Ok(JsString(value))
    }

    pub fn create_array(length: u32) -> Result<JsArray, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateArray(length, &raw mut value).as_result()?;
        }
        Ok(JsArray(value))
    }

    pub fn create_error(message: JsString) -> Result<JsErrorObject, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateError(message.as_ref().clone(), &raw mut value).as_result()?;
        }
        Ok(JsErrorObject(value))
    }

    pub fn create_named_function<T, R>(
        name: &str,
        native_function: T,
    ) -> Result<JsFunction, JsError>
    where
        T: Fn(&JsNativeFunctionArgs) -> R,
        R: IntoResponse,
    {
        let boxed_func = Box::new(native_function);
        let name_obj = ChakraRt::create_string(name)?;
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsCreateNamedFunction(
                name_obj.as_ref(),
                |callee, is_construct_call, args, arg_count, state| {
                    let arguments = std::slice::from_raw_parts(args, arg_count as usize);

                    let function_args = JsNativeFunctionArgs {
                        arguments: &arguments,
                        callee,
                        is_construct_call,
                    };
                    let func: &mut T = &mut *std::mem::transmute::<*mut CVoid, *mut T>(state);
                    func(&function_args).into_response()
                },
                Box::into_raw(boxed_func) as *mut CVoid,
                &raw mut value,
            )
            .as_result()?;
        }
        Ok(JsFunction(value))
    }

    pub fn int_to_number(value: i32) -> Result<JsValueRef, JsError> {
        let mut value_ref = JsValueRef::default();
        unsafe {
            bridge::JsIntToNumber(value, &raw mut value_ref).as_result()?;
        }
        Ok(value_ref)
    }

    pub fn double_to_number(value: f64) -> Result<JsValueRef, JsError> {
        let mut value_ref = JsValueRef::default();
        unsafe {
            bridge::JsDoubleToNumber(value, &raw mut value_ref).as_result()?;
        }
        Ok(value_ref)
    }

    pub fn get_undefined_value() -> Result<JsValueRef, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsGetUndefinedValue(&raw mut value).as_result()?;
        }
        Ok(value)
    }

    pub fn number_to_int(number: &JsValueRef) -> Result<i32, JsError> {
        let mut value = 0;
        unsafe {
            bridge::JsNumberToInt(number.clone(), &raw mut value).as_result()?;
        }
        Ok(value)
    }

    pub fn number_to_double(number: &JsValueRef) -> Result<f64, JsError> {
        let mut value = 0.;
        unsafe {
            bridge::JsNumberToDouble(number.clone(), &raw mut value).as_result()?;
        }
        Ok(value)
    }

    pub fn set_exception(error: JsErrorObject) -> Result<(), JsError> {
        bridge::JsSetException(error.as_ref().clone()).as_result()
    }

    pub fn has_exception() -> Result<bool, JsError> {
        let mut value = false;
        unsafe {
            bridge::JsHasException(&raw mut value).as_result()?;
        }
        Ok(value)
    }

    pub fn get_module_namespace(request_module: &JsModuleRecord) -> Result<JsValueRef, JsError> {
        let mut value = JsValueRef::default();
        unsafe {
            bridge::JsGetModuleNamespace(request_module.clone(), &raw mut value).as_result()?;
        }
        Ok(value)
    }
}

pub struct JsObject(JsValueRef);

impl JsObject {
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

pub struct JsString(JsValueRef);

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

pub struct JsArray(JsValueRef);

impl JsArray {
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

pub struct JsErrorObject(JsValueRef);

impl AsRef<JsValueRef> for JsErrorObject {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

pub struct JsFunction(JsValueRef);

impl AsRef<JsValueRef> for JsFunction {
    fn as_ref(&self) -> &JsValueRef {
        &self.0
    }
}

/// Defines the return value for a JavaScript function callback.
pub trait IntoResponse {
    fn into_response(self) -> JsValueRef;
}

impl IntoResponse for JsValueRef {
    fn into_response(self) -> JsValueRef {
        self
    }
}

impl IntoResponse for () {
    fn into_response(self) -> JsValueRef {
        ChakraRt::get_undefined_value().unwrap_or_default()
    }
}

impl<T> IntoResponse for Option<T>
where
    T: IntoResponse,
{
    fn into_response(self) -> JsValueRef {
        self.map(|x| x.into_response())
            .unwrap_or_else(|| ChakraRt::get_undefined_value().unwrap_or_default())
    }
}

impl<T, E> IntoResponse for Result<T, E>
where
    T: IntoResponse,
    E: IntoResponse,
{
    fn into_response(self) -> JsValueRef {
        match self {
            Ok(x) => x.into_response(),
            Err(err) => err.into_response(),
        }
    }
}

impl IntoResponse for JsError {
    fn into_response(self) -> JsValueRef {
        tracing::error!(?self, "The callback returned an error");

        // If the exception is already is set - no need to create a new exception.
        let has_exception = ChakraRt::has_exception();
        if has_exception.is_err() || !has_exception.unwrap() {
            if let Err(err) = ChakraRt::create_string(&self.to_string())
                .and_then(|msg| ChakraRt::create_error(msg))
                .and_then(|error| ChakraRt::set_exception(error))
            {
                tracing::error!(?err, "Failed to set an exception");
            }
        }

        ChakraRt::get_undefined_value().unwrap_or_default()
    }
}
