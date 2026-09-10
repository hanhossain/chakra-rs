use chakracore_sys::chhelper::ffi::{
    CreateAndRunSerializedScript, CreateParserStateAndRunScript, ExecuteTest,
};
use chakracore_sys::config::CoreConfig;
use chakracore_sys::helpers::ffi::Helpers;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use chakracore_sys::rt_interface::ffi::{ChakraRTInterface, JsRuntimeAttributes};
use chakracore_sys::rt_interface::{JsContextRef, JsError, JsErrorExt, JsRuntimeHandle};
use chakracore_sys::wscript_jsrt::ffi::WScriptJsrt;
use cxx::Exception;

#[tracing::instrument(skip(config))]
pub fn run(config: CoreConfig) -> Result<(), Error> {
    HostConfigFlags::SetHostArgs(&config.host_args);

    // handle command line flags
    ChakraRTInterface::InitializeTestHooks(&config.args);

    execute_test(&config)?;
    Ok(())
}

#[tracing::instrument(skip(config))]
fn execute_test(config: &CoreConfig) -> Result<(), Error> {
    let file_contents = Helpers::LoadScriptFromFile(&config.filename)?;
    let mut runtime = JsRuntimeHandle::default();
    unsafe {
        ChakraRTInterface::JsCreateRuntime(
            JsRuntimeAttributes::JsRuntimeAttributeNone,
            &raw mut runtime,
        )
        .as_result()?;
    }

    let mut context = JsContextRef::default();
    unsafe {
        ChakraRTInterface::JsCreateContext(runtime, &raw mut context).as_result()?;
    }
    ChakraRTInterface::JsSetCurrentContext(context).as_result()?;
    if !WScriptJsrt::Initialize() {
        let fail = 0x80004005u32 as i32;
        return Err(Error::NegativeHResult(fail));
    }

    let path = std::fs::canonicalize(&config.filename)?;
    let path = path.to_str().unwrap().to_owned();
    let mut ch_runtime = runtime;
    let jsrt_attributes = JsRuntimeAttributes::JsRuntimeAttributeNone;
    let res = if config.serialized {
        CreateAndRunSerializedScript(
            &config.filename,
            &file_contents,
            &path,
            &mut ch_runtime,
            jsrt_attributes,
        )
    } else if config.use_parser_state_cache {
        CreateParserStateAndRunScript(
            &config.filename,
            &file_contents,
            &path,
            &mut ch_runtime,
            jsrt_attributes,
        )
    } else {
        ExecuteTest(&config.filename, &file_contents)?
    };

    if res < 0 {
        tracing::error!(hresult = res, "hresult was negative. exiting.");
        return Err(Error::NegativeHResult(res));
    }
    if res > 0 {
        return Err(Error::ExitCode(res as u8));
    }

    ChakraRTInterface::JsSetCurrentContext(JsContextRef::default()).as_result()?;

    if !runtime.is_invalid() {
        ChakraRTInterface::JsDisposeRuntime(runtime).as_result()?;
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
}
