use crate::{Error, hresult_to_result};
use chakracore_sys::chhelper::ffi::{MessageQueue, RunScript};
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

#[tracing::instrument(skip(config))]
pub fn execute_test(config: &CoreConfig) -> Result<(), Error> {
    HostConfigFlags::SetHostArgs(&config.host_args);

    // handle command line flags
    hresult_to_result(ChakraRTInterface::InitializeTestHooks(&config.args))?;

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
    let jsrt_attributes = JsRuntimeAttributes::JsRuntimeAttributeNone;
    if config.serialized {
        create_and_run_serialized_script(&config.filename, &file_contents, &path, jsrt_attributes)?;
    } else if config.use_parser_state_cache {
        create_parser_state_and_run_script(
            &config.filename,
            &file_contents,
            &path,
            jsrt_attributes,
        )?;
    } else {
        run_script(
            &config.filename,
            &file_contents,
            JsValueRef::default(),
            &path,
            JsValueRef::default(),
        )?;
    };

    ChakraRTInterface::JsSetCurrentContext(JsContextRef::default()).as_result()?;

    if !runtime.is_invalid() {
        ChakraRTInterface::JsDisposeRuntime(runtime).as_result()?;
    }

    Ok(())
}

#[tracing::instrument(skip(contents))]
fn create_parser_state_and_run_script(
    filename: &str,
    contents: &String,
    full_path: &String,
    jsrt_attributes: JsRuntimeAttributes,
) -> Result<(), Error> {
    let buffer = get_parser_state_buffer(contents)?;

    // Bytecode buffer is created in one runtime and will be executed on different runtime.
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

    run_script(filename, contents, JsValueRef::default(), full_path, buffer)?;

    ChakraRTInterface::JsSetCurrentContext(old_context).as_result()?;
    ChakraRTInterface::JsDisposeRuntime(runtime).as_result()?;
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

    run_script(
        filename,
        contents,
        buffer_val,
        full_path,
        JsValueRef::default(),
    )?;

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

fn get_parser_state_buffer(file_contents: &str) -> Result<JsValueRef, Error> {
    let mut script_source = JsValueRef::default();
    unsafe {
        // We don't want this to free fileContents when it completes, so the finalizeCallback is nullptr
        ChakraRTInterface::JsCreateExternalArrayBuffer(file_contents, &raw mut script_source)
            .as_result()?;
        let mut buffer = JsValueRef::default();
        ChakraRTInterface::JsSerializeParserState(
            script_source,
            &raw mut buffer,
            JsParseScriptAttributes::JsParseScriptAttributeNone,
        )
        .as_result()?;
        Ok(buffer)
    }
}

#[tracing::instrument(skip_all)]
fn run_script(
    filename: &str,
    contents: &String,
    buffer_value: JsValueRef,
    full_path: &String,
    parser_state_cache: JsValueRef,
) -> Result<(), Error> {
    let message_queue = MessageQueue::New();
    hresult_to_result(RunScript(
        filename,
        contents,
        buffer_value,
        full_path,
        parser_state_cache,
        message_queue,
    ))?;

    // We only call RunScript() once, safe to Uninitialize()
    WScriptJsrt::Uninitialize();
    Ok(())
}
