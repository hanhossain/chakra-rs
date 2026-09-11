use crate::{Error, hresult_to_result};
use chakracore_sys::chhelper::ffi::MessageQueue;
use chakracore_sys::config::ConfigContext;
use chakracore_sys::helpers::ffi::Helpers;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use chakracore_sys::rt_interface::ffi::{
    ChakraRTInterface, JsErrorCode, JsParseScriptAttributes, JsRuntimeAttributes,
};
use chakracore_sys::rt_interface::{
    JsContextRef, JsError, JsErrorExt, JsRuntimeHandle, JsSourceContext, JsValueRef,
};
use chakracore_sys::wscript_jsrt::ffi::WScriptJsrt;
use std::ffi::{CStr, CString, c_char};
use std::str::FromStr;

#[tracing::instrument(skip(config))]
pub fn execute_test(config: &ConfigContext) -> Result<(), Error> {
    HostConfigFlags::SetHostArgs(&config.core.host_args, &config);

    // handle command line flags
    hresult_to_result(ChakraRTInterface::InitializeTestHooks(&config.core.args))?;

    let file_contents = Helpers::LoadScriptFromFile(&config.core.filename)?;
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

    let path = std::fs::canonicalize(&config.core.filename)?;
    let path = path.to_str().unwrap().to_owned();
    let jsrt_attributes = JsRuntimeAttributes::JsRuntimeAttributeNone;
    if config.host.serialized {
        create_and_run_serialized_script(
            &config.core.filename,
            &file_contents,
            &path,
            jsrt_attributes,
        )?;
    } else if config.host.use_parser_state_cache {
        create_parser_state_and_run_script(
            &config.core.filename,
            &file_contents,
            &path,
            jsrt_attributes,
        )?;
    } else {
        run_script(
            &config.core.filename,
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
    let mut message_queue = MessageQueue::New();
    let fname = unsafe {
        WScriptJsrt::AddMessageQueue(message_queue.as_mut_ptr());
        ChakraRTInterface::JsSetPromiseContinuationCallback(
            |task, callback_state| {
                WScriptJsrt::PromiseContinuationCallback(task, callback_state);
            },
            message_queue.as_mut_ptr() as *mut _,
        )
        .as_result()?;
        let mut fname = JsValueRef::default();
        ChakraRTInterface::JsCreateString(full_path, &raw mut fname).as_result()?;
        fname
    };

    let run_script_result = if !buffer_value.is_null() {
        // Now we can run our script, with this serializedCallbackInfo as the sourcecontext
        let contents = CString::from_str(&contents)?;

        // Use source ptr as sourceContext
        let source_context = JsSourceContext(contents.into_raw() as usize);
        unsafe {
            ChakraRTInterface::JsRunSerialized(
                buffer_value,
                dummy_js_serialized_script_load_utf8_source,
                source_context,
                fname,
                std::ptr::null_mut(),
            )
        }
    } else if !parser_state_cache.is_null() {
        let mut script_source = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateExternalArrayBuffer(contents, &raw mut script_source)
                .as_result()?;
            ChakraRTInterface::JsRunScriptWithParserState(
                script_source,
                JsSourceContext(WScriptJsrt::GetNextSourceContext()),
                fname,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                parser_state_cache,
                std::ptr::null_mut(),
            )
        }
    } else if HostConfigFlags::GetConfig().host.module {
        WScriptJsrt::ModuleEntryPoint(contents, full_path)
    } else {
        let mut script_source = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateExternalArrayBuffer(contents, &raw mut script_source)
                .as_result()?;
            ChakraRTInterface::JsRun(
                script_source,
                JsSourceContext(WScriptJsrt::GetNextSourceContext()),
                fname,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                std::ptr::null_mut(),
            )
        }
    };

    if run_script_result != JsErrorCode::JsNoError {
        WScriptJsrt::PrintException(filename, run_script_result, JsValueRef::default());
    } else {
        // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
        // because setTimeout can add scripts to execute.
        while !message_queue.pin_mut().IsEmpty() {
            message_queue.pin_mut().ProcessAll(filename);
        }
    }

    message_queue.pin_mut().RemoveAll();

    // clean up possible pinned exception object on exit to avoid potential leak
    let mut has_exception = false;
    unsafe {
        if ChakraRTInterface::JsHasException(&raw mut has_exception) == JsErrorCode::JsNoError
            && has_exception
        {
            let mut exception = JsValueRef::default();
            ChakraRTInterface::JsGetAndClearException(&raw mut exception);
        }
    }

    // We only call RunScript() once, safe to Uninitialize()
    WScriptJsrt::Uninitialize();
    Ok(())
}

#[tracing::instrument(skip_all)]
fn dummy_js_serialized_script_load_utf8_source(
    source_context: JsSourceContext,
    script_buffer: *mut JsValueRef,
    parse_attributes: *mut JsParseScriptAttributes,
) -> bool {
    let script_body = source_context.0 as *mut c_char;
    unsafe {
        let script_body = CStr::from_ptr(script_body);

        // sourceContext is source ptr, see run_serialized below
        if ChakraRTInterface::JsCreateExternalArrayBuffer(
            script_body.to_str().unwrap(),
            script_buffer,
        ) != JsErrorCode::JsNoError
        {
            return false;
        }

        *parse_attributes = JsParseScriptAttributes::JsParseScriptAttributeNone;
        true
    }
}
