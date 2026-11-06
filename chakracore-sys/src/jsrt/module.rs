use crate::jsrt::ffi::bridge;
use crate::jsrt::{JsError, JsModuleHostInfoKind, JsModuleRecord, JsValueRef};

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

    pub(crate) fn new(module: JsModuleRecord) -> Self {
        Self { record: module }
    }

    pub fn get_host_info_url(&self) -> Result<JsValueRef, JsError> {
        let mut url = JsValueRef::default();
        unsafe {
            bridge::JsGetModuleHostInfo(
                self.record,
                JsModuleHostInfoKind::JsModuleHostInfo_Url,
                &raw mut url as *mut _,
            )
            .as_result()?;
        }
        Ok(url)
    }
}
