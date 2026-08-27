use chakracore_sys::chhelper::ffi::ExecuteTest;
use chakracore_sys::config::CoreConfig;
use chakracore_sys::helpers::ffi::Helpers;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use chakracore_sys::rt_interface::ffi::ChakraRTInterface;
use cxx::Exception;

#[tracing::instrument(skip(config))]
pub fn run(config: CoreConfig) -> Result<(), Error> {
    HostConfigFlags::SetHostArgs(&config.host_args);

    // handle command line flags
    ChakraRTInterface::InitializeTestHooks(&config.args);

    let res = execute_test(&config.filename)?;
    if res < 0 {
        tracing::error!(hresult = res, "hresult was negative. exiting.");
        return Err(Error::NegativeHResult(res));
    }
    if res > 0 {
        return Err(Error::ExitCode(res as u8));
    }
    Ok(())
}

fn execute_test(filename: &String) -> Result<i32, Exception> {
    let file_contents = Helpers::LoadScriptFromFile(filename)?;
    ExecuteTest(filename, &file_contents)
}

#[derive(thiserror::Error, Debug)]
pub enum Error {
    #[error("exit code")]
    ExitCode(u8),
    #[error("hresult was negative")]
    NegativeHResult(i32),
    #[error("Exception propagated from c++")]
    Exception(#[from] Exception),
}
