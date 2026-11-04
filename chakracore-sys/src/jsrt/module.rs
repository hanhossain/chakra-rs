use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsModuleRecord, JsValueRef};

#[derive(Eq, PartialEq, Hash, Copy, Clone)]
pub struct JsModule {
    pub(crate) record: JsModuleRecord,
}

impl JsModule {
    pub fn initialize(specifier: JsValueRef) -> Result<Self, JsError> {
        let mut record = JsModuleRecord::default();
        unsafe {
            bridge::JsInitializeModuleRecord(specifier, &raw mut record).as_result()?;
        }
        Ok(JsModule { record })
    }
}
