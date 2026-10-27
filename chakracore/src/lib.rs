pub mod chhelper;

use crate::chhelper::execute_test;
use chakracore_sys::config::ConfigContext;
use chakracore_sys::helpers;
use chakracore_sys::jsrt::JsError;
use cxx::Exception;

#[tracing::instrument(skip(config))]
pub fn run(config: ConfigContext) -> Result<(), Error> {
    execute_test(&config)?;
    Ok(())
}

#[derive(thiserror::Error, Debug)]
pub enum Error {
    #[error("Exception propagated from c++")]
    Exception(#[from] Exception),
    #[error(transparent)]
    JsError(#[from] JsError),
    #[error(transparent)]
    Io(#[from] std::io::Error),
    #[error(transparent)]
    InteriorNull(#[from] std::ffi::NulError),
    #[error(transparent)]
    Helpers(#[from] helpers::Error),
}
