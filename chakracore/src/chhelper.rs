use crate::Error;
use chakracore_sys::config::ConfigContext;
use chakracore_sys::helpers;
use chakracore_sys::jsrt::{
    ChakraRt, JsContextRef, JsError, JsErrorCode, JsParseScriptAttributes, JsRuntimeAttributes,
    JsSourceContext, JsValueRef,
};
use chakracore_sys::rt_interface::ChakraRTInterface;
use chakracore_sys::wscript_jsrt::WScript;
use chakracore_sys::wscript_jsrt::cache::ScriptCache;
use std::ffi::{CStr, CString, c_char};
use std::str::FromStr;

#[tracing::instrument(skip(config))]
pub fn execute_test(config: &ConfigContext) -> Result<(), Error> {
    // handle command line flags
    parse_config_flags(&config.core.args)?;

    let script_cache = ScriptCache::new();
    let file_contents = script_cache.get_script(&config.core.filename)?;
    let runtime = ChakraRt::create_runtime(JsRuntimeAttributes::JsRuntimeAttributeNone)?;

    let context = ChakraRt::create_context(runtime)?;
    ChakraRt::set_current_context(context)?;
    let wscript = WScript::new(config, script_cache);
    wscript.initialize()?;

    let path = std::fs::canonicalize(&config.core.filename)?;
    let path = path.to_str().unwrap().to_owned();
    let jsrt_attributes = JsRuntimeAttributes::JsRuntimeAttributeNone;
    if config.host.serialized {
        create_and_run_serialized_script(
            &config.core.filename,
            &file_contents,
            &path,
            jsrt_attributes,
            &wscript,
        )?;
    } else if config.host.use_parser_state_cache {
        create_parser_state_and_run_script(
            &config.core.filename,
            &file_contents,
            &path,
            jsrt_attributes,
            &wscript,
        )?;
    } else {
        run_script(
            &config.core.filename,
            &file_contents,
            JsValueRef::default(),
            &path,
            JsValueRef::default(),
            &wscript,
        )?;
    };

    ChakraRt::set_current_context(JsContextRef::default())?;

    if !runtime.is_invalid() {
        ChakraRt::dispose_runtime(runtime)?;
    }

    Ok(())
}

pub fn print_usage() {
    #[cfg(debug_assertions)]
    {
        println!("\nUsage: ch [-v|--version] [-h|--help] [-?] [flaglist] <source file>");
        println!("\t-v|--version\t\tDisplays version info");
        println!("\t-h|--help\t\tDisplays this help message");
        println!("\t-?\t\t\tDisplays this help message with complete [flaglist] info");
    }

    #[cfg(not(debug_assertions))]
    {
        println!("\nUsage: ch [-v|--version] [-h|--help|-?] <source file>");
        println!(
            "Note: [flaglist] is not supported in Release builds; try a Debug or Test build to enable these flags."
        );
        println!("\t-v|--version\t\tDisplays version info");
        println!("\t-h|--help|-?\t\tDisplays this help message");
    }
}

fn parse_config_flags(args: &Vec<String>) -> Result<(), helpers::Error> {
    let Err(err) = helpers::set_config_flags(args) else {
        return Ok(());
    };

    print_usage();
    Err(err)
}

#[tracing::instrument(skip(contents, wscript))]
fn create_parser_state_and_run_script(
    filename: &str,
    contents: &String,
    full_path: &String,
    jsrt_attributes: JsRuntimeAttributes,
    wscript: &WScript,
) -> Result<(), Error> {
    let buffer = get_parser_state_buffer(contents)?;

    // Bytecode buffer is created in one runtime and will be executed on different runtime.
    let runtime = ChakraRt::create_runtime(jsrt_attributes)?;
    let new_context = ChakraRt::create_context(runtime)?;
    let old_context = ChakraRt::get_current_context()?;
    ChakraRt::set_current_context(new_context)?;

    // initialize the WScript object on the new context
    wscript.initialize()?;

    run_script(
        filename,
        contents,
        JsValueRef::default(),
        full_path,
        buffer,
        &wscript,
    )?;

    ChakraRt::set_current_context(old_context)?;
    ChakraRt::dispose_runtime(runtime)?;
    Ok(())
}

#[tracing::instrument(skip(contents, wscript))]
fn create_and_run_serialized_script(
    filename: &str,
    contents: &String,
    full_path: &String,
    jsrt_attributes: JsRuntimeAttributes,
    wscript: &WScript,
) -> Result<(), Error> {
    let buffer_val = get_serialized_buffer(contents)?;

    // Bytecode buffer is created in one runtime and will be executed on a different runtime.
    let runtime = ChakraRt::create_runtime(jsrt_attributes)?;
    let new_context = ChakraRt::create_context(runtime)?;
    let old_context = ChakraRt::get_current_context()?;
    ChakraRt::set_current_context(new_context)?;

    // initialize the WScript object on the new context
    wscript.initialize()?;

    run_script(
        filename,
        contents,
        buffer_val,
        full_path,
        JsValueRef::default(),
        wscript,
    )?;

    ChakraRt::set_current_context(old_context)?;
    ChakraRt::dispose_runtime(runtime)?;

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
    wscript: &WScript,
) -> Result<(), Error> {
    let (fname, _callback) = unsafe {
        let callback = ChakraRt::set_promise_continuation_callback(|task| {
            wscript.promise_continuation_callback(task)
        })?;
        let fname = ChakraRt::create_string(full_path)?;
        (fname, callback)
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
                *fname,
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
                JsSourceContext(WScript::get_next_source_context()),
                *fname,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                parser_state_cache,
                std::ptr::null_mut(),
            )
        }
    } else if wscript.config.host.module {
        wscript.module_entry_point(contents, full_path)
    } else {
        let mut script_source = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateExternalArrayBuffer(contents, &raw mut script_source)
                .as_result()?;
            ChakraRTInterface::JsRun(
                script_source,
                JsSourceContext(WScript::get_next_source_context()),
                *fname,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                std::ptr::null_mut(),
            )
        }
    };

    if run_script_result != JsErrorCode::JsNoError {
        wscript.print_exception(filename, run_script_result, JsValueRef::default());
    } else {
        // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
        // because setTimeout can add scripts to execute.
        while !wscript.message_queue.is_empty() {
            wscript.message_queue.process_all(filename, wscript);
        }
    }

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
