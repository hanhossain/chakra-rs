use crate::Error;
use chakracore_sys::config::ConfigContext;
use chakracore_sys::helpers;
use chakracore_sys::jsrt::buffer::{JsArrayBuffer, JsExternalArrayBuffer};
use chakracore_sys::jsrt::run::{JsRun, JsRunSerialized};
use chakracore_sys::jsrt::runtime::JsRuntime;
use chakracore_sys::jsrt::{
    ChakraRt, JsError, JsParseScriptAttributes, JsRuntimeAttributes, JsSourceContext, JsValueRef,
};
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
    let runtime = JsRuntime::create(JsRuntimeAttributes::JsRuntimeAttributeNone)?;

    let _context = runtime.create_context()?.set_current()?;
    let wscript = WScript::new(config, script_cache);
    wscript.initialize(&runtime)?;

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
            None,
            &path,
            None,
            &wscript,
        )?;
    };

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
    let runtime = JsRuntime::create(jsrt_attributes)?;
    let _new_context = runtime.create_context()?.set_current()?;

    // initialize the WScript object on the new context
    wscript.initialize(&runtime)?;

    run_script(filename, contents, None, full_path, Some(buffer), &wscript)?;
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
    let runtime = JsRuntime::create(jsrt_attributes)?;
    let _new_context = runtime.create_context()?.set_current()?;

    // initialize the WScript object on the new context
    wscript.initialize(&runtime)?;

    run_script(
        filename,
        contents,
        Some(buffer_val),
        full_path,
        None,
        wscript,
    )?;

    Ok(())
}

#[tracing::instrument(skip(file_contents), err)]
fn get_serialized_buffer(file_contents: &String) -> Result<JsArrayBuffer, JsError> {
    let script_source = JsExternalArrayBuffer::create(file_contents)?;
    let byte_code_buffer =
        script_source.serialize(JsParseScriptAttributes::JsParseScriptAttributeNone)?;
    Ok(byte_code_buffer)
}

fn get_parser_state_buffer(file_contents: &str) -> Result<JsArrayBuffer, Error> {
    // We don't want this to free fileContents when it completes, so the finalizeCallback is nullptr
    let script_source = JsExternalArrayBuffer::create(file_contents)?;
    let buffer = script_source
        .serialize_parser_state(JsParseScriptAttributes::JsParseScriptAttributeNone)?;
    Ok(buffer)
}

#[tracing::instrument(skip_all)]
fn run_script(
    filename: &str,
    contents: &String,
    buffer_value: Option<JsArrayBuffer>,
    full_path: &String,
    parser_state_cache: Option<JsArrayBuffer>,
    wscript: &WScript,
) -> Result<(), Error> {
    let _callback = ChakraRt::set_promise_continuation_callback(|task| {
        let _ = wscript.promise_continuation_callback(task);
    })?;
    let fname = ChakraRt::create_string(full_path)?;

    let run_script_result = if let Some(buffer_value) = buffer_value {
        // Now we can run our script, with this serializedCallbackInfo as the sourcecontext
        let contents = CString::from_str(&contents)?;

        // Use source ptr as sourceContext
        let source_context = JsSourceContext(contents.into_raw() as usize);
        buffer_value
            .run_serialized(
                dummy_js_serialized_script_load_utf8_source,
                source_context,
                *fname,
            )
            .map(|_| ())
    } else if let Some(parser_state_cache) = parser_state_cache {
        let script_source = JsExternalArrayBuffer::create(contents)?;
        script_source
            .run_script_with_parser_state(
                JsSourceContext(WScript::get_next_source_context()),
                *fname,
                JsParseScriptAttributes::JsParseScriptAttributeNone,
                &parser_state_cache,
            )
            .map(|_| ())
    } else if wscript.config.host.module {
        wscript.module_entry_point(contents, full_path).as_result()
    } else {
        let script_source = JsExternalArrayBuffer::create(contents)?;
        script_source.run(
            JsSourceContext(WScript::get_next_source_context()),
            *fname,
            JsParseScriptAttributes::JsParseScriptAttributeNone,
        )
    };
    if let Err(err) = run_script_result {
        wscript.print_exception(filename, err.into(), JsValueRef::default());
    } else {
        // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
        // because setTimeout can add scripts to execute.
        while !wscript.message_queue.is_empty() {
            wscript.message_queue.process_all(filename, wscript);
        }
    }

    // clean up possible pinned exception object on exit to avoid potential leak
    if ChakraRt::has_exception().unwrap_or_default() {
        // don't care about the actual exception, just want to clear it
        let _ = ChakraRt::get_and_clear_exception()?;
    }

    Ok(())
}

#[tracing::instrument(skip_all)]
fn dummy_js_serialized_script_load_utf8_source(
    source_context: JsSourceContext,
    script_buffer: &mut JsValueRef,
    parse_attributes: &mut JsParseScriptAttributes,
) -> bool {
    let script_body = source_context.0 as *mut c_char;
    let script_body = unsafe { CStr::from_ptr(script_body) };

    // sourceContext is source ptr, see run_serialized below
    let Ok(buffer) = JsExternalArrayBuffer::create(script_body.to_str().unwrap()) else {
        return false;
    };
    *script_buffer = *buffer.as_ref();

    *parse_attributes = JsParseScriptAttributes::JsParseScriptAttributeNone;
    true
}
