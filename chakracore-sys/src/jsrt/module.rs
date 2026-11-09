use crate::jsrt::ffi::bridge;
use crate::jsrt::{
    JsError, JsErrorObject, JsModuleHostInfoKind, JsModuleRecord, JsSourceContext, JsValueRef,
};

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

    pub fn parse_source(
        &self,
        source_context: &JsSourceContext,
        source_text: Option<&str>,
    ) -> Result<(), ParseScriptError> {
        let mut error = JsValueRef::default();
        let res = unsafe {
            bridge::JsParseModuleSource(
                self.record,
                *source_context,
                source_text.into(),
                &raw mut error,
            )
            .as_result()
        };
        let Err(err) = res else {
            return Ok(());
        };
        if err == JsError::JsErrorScriptCompile && !error.is_null() && source_text.is_some() {
            return Err(ParseScriptError::ScriptCompile(error.into()));
        }
        Err(ParseScriptError::ParseFailed(err))
    }
}

#[derive(thiserror::Error, Debug)]
pub enum ParseScriptError {
    #[error(transparent)]
    ParseFailed(#[from] JsError),
    #[error("Script failed to compile")]
    ScriptCompile(JsErrorObject),
}
