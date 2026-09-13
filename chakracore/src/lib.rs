mod chhelper;

use crate::chhelper::execute_test;
use chakracore_sys::config::ConfigContext;
use chakracore_sys::jsrt::JsError;
use cxx::Exception;

#[tracing::instrument(skip(config))]
pub fn run(config: ConfigContext) -> Result<(), Error> {
    execute_test(&config)?;
    Ok(())
}

fn hresult_to_result(res: i32) -> Result<(), Error> {
    if res < 0 {
        return Err(Error::NegativeHResult(res));
    }
    if res > 0 {
        return Err(Error::ExitCode(res as u8));
    }

    Ok(())
}

#[derive(thiserror::Error, Debug)]
pub enum Error {
    #[error("exit code")]
    ExitCode(u8),
    #[error("hresult was negative")]
    NegativeHResult(i32),
    #[error("Exception propagated from c++")]
    Exception(#[from] Exception),
    #[error(transparent)]
    JsError(#[from] JsError),
    #[error(transparent)]
    Io(#[from] std::io::Error),
    #[error(transparent)]
    InteriorNull(#[from] std::ffi::NulError),
}
