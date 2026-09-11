use crate::rt_interface::ffi::{ChakraRTInterface, JsErrorCode, JsParseScriptAttributes};
use crate::rt_interface::{JsSourceContext, JsValueRef};
use std::ffi::{CStr, CString, c_char};
use std::str::FromStr;

#[cxx::bridge]
pub mod ffi {
    #[namespace = "chakra_rs::chhelper"]
    extern "Rust" {
        fn print_usage();

        fn run_serialized(
            buffer_value: JsValueRef,
            contents: &String,
            fname: JsValueRef,
        ) -> JsErrorCode;
    }

    unsafe extern "C++" {
        include!("chhelper.h");
        include!("Util/Abstractions.h");

        fn RunScript(
            fileName: &str,
            contents: &String,
            bufferValue: JsValueRef,
            fullPath: &String,
            parserStateCache: JsValueRef,
            messageQueue: &UniquePtr<MessageQueue>,
            fname: JsValueRef,
        ) -> i32;

        type Abstractions;
        #[Self = "Abstractions"]
        fn IsDebuggerPresent() -> bool;

        type JsRuntimeHandle = crate::rt_interface::JsRuntimeHandle;
        type JsRuntimeAttributes = crate::rt_interface::ffi::JsRuntimeAttributes;
        type JsValueRef = crate::rt_interface::JsValueRef;

        type MessageQueue;
        #[Self = "MessageQueue"]
        fn New() -> UniquePtr<MessageQueue>;

        fn RemoveAll(self: Pin<&mut MessageQueue>);
        type JsErrorCode = crate::rt_interface::ffi::JsErrorCode;
    }
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

// TODO: this method's temporarily here while I port the rest of RunScript
#[tracing::instrument(skip_all)]
fn run_serialized(buffer_value: JsValueRef, contents: &String, fname: JsValueRef) -> JsErrorCode {
    let Ok(contents) = CString::from_str(&contents) else {
        return JsErrorCode::JsErrorFatal;
    };

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
}
