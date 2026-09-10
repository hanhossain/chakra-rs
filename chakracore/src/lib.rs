use chakracore_sys::chhelper::ffi::{CreateParserStateAndRunScript, RunScript};
use chakracore_sys::config::CoreConfig;
use chakracore_sys::helpers::ffi::Helpers;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use chakracore_sys::rt_interface::ffi::{
    ChakraRTInterface, JsParseScriptAttributes, JsRuntimeAttributes,
};
use chakracore_sys::rt_interface::{
    JsContextRef, JsError, JsErrorExt, JsRuntimeHandle, JsValueRef,
};
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
        return Err(Error::hresult_fail());
    }

    let path = std::fs::canonicalize(&config.filename)?;
    let path = path.to_str().unwrap().to_owned();
    let mut ch_runtime = runtime;
    let jsrt_attributes = JsRuntimeAttributes::JsRuntimeAttributeNone;
    if config.serialized {
        create_and_run_serialized_script(&config.filename, &file_contents, &path, jsrt_attributes)?;
    } else if config.use_parser_state_cache {
        hresult_to_result(CreateParserStateAndRunScript(
            &config.filename,
            &file_contents,
            &path,
            &mut ch_runtime,
            jsrt_attributes,
        ))?;
    } else {
        hresult_to_result(RunScript(
            &config.filename,
            &file_contents,
            JsValueRef::default(),
            &path,
            JsValueRef::default(),
        ))?;
    };

    ChakraRTInterface::JsSetCurrentContext(JsContextRef::default()).as_result()?;

    if !runtime.is_invalid() {
        ChakraRTInterface::JsDisposeRuntime(runtime).as_result()?;
    }

    Ok(())
}

#[tracing::instrument(err)]
fn hresult_to_result(res: i32) -> Result<(), Error> {
    if res < 0 {
        return Err(Error::NegativeHResult(res));
    }
    if res > 0 {
        return Err(Error::ExitCode(res as u8));
    }

    Ok(())
}

#[tracing::instrument(skip(contents))]
fn create_and_run_serialized_script(
    filename: &str,
    contents: &String,
    full_path: &String,
    jsrt_attributes: JsRuntimeAttributes,
) -> Result<(), Error> {
    let buffer_val = get_serialized_buffer(contents)?;

    // Bytecode buffer is created in one runtime and will be executed on a different runtime.
    let mut runtime = JsRuntimeHandle::default();
    let old_context = unsafe {
        ChakraRTInterface::JsCreateRuntime(jsrt_attributes, &raw mut runtime).as_result()?;

        let mut new_context = JsContextRef::default();
        ChakraRTInterface::JsCreateContext(runtime, &raw mut new_context).as_result()?;

        let mut old_context = JsContextRef::default();
        ChakraRTInterface::JsGetCurrentContext(&raw mut old_context).as_result()?;
        ChakraRTInterface::JsSetCurrentContext(new_context).as_result()?;
        old_context
    };

    // initialize the WScript object on the new context
    if !WScriptJsrt::Initialize() {
        return Err(Error::hresult_fail());
    }

    hresult_to_result(RunScript(
        filename,
        contents,
        buffer_val,
        full_path,
        JsValueRef::default(),
    ))?;

    ChakraRTInterface::JsSetCurrentContext(old_context).as_result()?;
    ChakraRTInterface::JsDisposeRuntime(runtime).as_result()?;

    Ok(())
}

#[tracing::instrument(skip(file_contents), err)]
fn get_serialized_buffer(file_contents: &String) -> Result<JsValueRef, JsError> {
    let mut script_source = JsValueRef::default();
    unsafe {
        ChakraRTInterface::JsCreateExternalArrayBuffer(file_contents, &raw mut script_source)
            .as_result()?;
        let mut byte_code_buffer = JsValueRef::default();
        ChakraRTInterface::JsSerialize(
            script_source,
            &raw mut byte_code_buffer,
            JsParseScriptAttributes::JsParseScriptAttributeNone,
        )
        .as_result()?;
        Ok(byte_code_buffer)
    }
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

impl Error {
    /// Returns an HRESULT E_FAIL
    fn hresult_fail() -> Self {
        let fail = 0x80004005u32 as i32;
        Error::NegativeHResult(fail)
    }
}
