use crate::jsrt::ffi::bridge;
use crate::jsrt::{
    JsError, JsErrorObject, JsModuleHostInfoKind, JsModuleRecord, JsRef, JsRefGuard,
    JsSourceContext, JsValueRef,
};

#[derive(Eq, PartialEq)]
pub struct JsModule {
    pub(crate) record: JsModuleRecord,
    guard: JsRefGuard,
}

impl JsModule {
    pub fn initialize(specifier: JsValueRef) -> Result<Self, JsError> {
        let mut record = JsModuleRecord::default();
        unsafe {
            bridge::JsInitializeModuleRecord(specifier, &raw mut record).as_result()?;
        }
        Self::new(record)
    }

    pub(crate) fn new(module: JsModuleRecord) -> Result<Self, JsError> {
        let guard = JsRef::add_ref(&module)?;
        Ok(Self {
            record: module,
            guard,
        })
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

    #[tracing::instrument(skip_all, err)]
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
        if err == JsError::JsErrorModuleParsed {
            return Err(ParseScriptError::ModuleAlreadyParsed);
        }
        if err == JsError::JsErrorScriptCompile && !error.is_null() && source_text.is_some() {
            return Err(ParseScriptError::ScriptCompile(error.into()));
        }
        Err(ParseScriptError::ParseFailed(err))
    }

    pub fn evaluate(&self) -> Result<JsValueRef, JsError> {
        let mut module_res = JsValueRef::default();
        unsafe {
            bridge::JsModuleEvaluation(self.record, &raw mut module_res).as_result()?;
        }
        Ok(module_res)
    }
}

impl std::ops::Deref for JsModule {
    type Target = JsModuleRecord;

    fn deref(&self) -> &Self::Target {
        &self.record
    }
}

#[derive(thiserror::Error, Debug)]
pub enum ParseScriptError {
    #[error(transparent)]
    ParseFailed(#[from] JsError),
    #[error("Module was already parsed")]
    ModuleAlreadyParsed,
    #[error("Script failed to compile")]
    ScriptCompile(JsErrorObject),
}
