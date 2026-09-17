use crate::helpers::{ScriptCache, TestHooks};
use crate::host_config::HostConfigFlags;
use crate::jsrt::{
    ChakraRt, IntoResponse, JsArray, JsError, JsNativeFunctionArgs, JsParseScriptAttributes,
    JsSourceContext, JsString, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
pub use ffi::WScriptJsrt;
use std::time::{Duration, SystemTime, UNIX_EPOCH};

#[cfg(debug_assertions)]
const BUILD_TYPE_STRING: &str = "Debug";
#[cfg(not(debug_assertions))]
const BUILD_TYPE_STRING: &str = "Test";

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        include!("PlatformAgnostic/ChakraICU.h");

        type WScriptJsrt;

        type JsPropertyIdRef = crate::jsrt::JsPropertyIdRef;

        #[Self = "WScriptJsrt"]
        fn Uninitialize() -> bool;

        type MessageQueue = crate::chhelper::MessageQueue;
        #[Self = "WScriptJsrt"]
        unsafe fn AddMessageQueue(messageQueue: *mut MessageQueue);

        type JsValueRef = crate::jsrt::JsValueRef;
        type CVoid = crate::jsrt::CVoid;
        #[Self = "WScriptJsrt"]
        unsafe fn PromiseContinuationCallback(task: JsValueRef, callbackState: *mut CVoid);

        #[Self = "WScriptJsrt"]
        fn GetNextSourceContext() -> usize;

        type JsErrorCode = crate::jsrt::JsErrorCode;
        #[Self = "WScriptJsrt"]
        fn ModuleEntryPoint(fileContent: &str, fullName: &String) -> JsErrorCode;

        #[Self = "WScriptJsrt"]
        fn PrintException(filname: &str, jsErrorCode: JsErrorCode, exception: JsValueRef) -> bool;

        #[namespace = "chakra_rs"]
        type JsNativeFunctionArgs<'a> = crate::jsrt::JsNativeFunctionArgs<'a>;

        #[namespace = "PlatformAgnostic::ICUHelpers"]
        fn GetICUMajorVersion() -> i32;

        #[Self = "WScriptJsrt"]
        fn SetTimeoutCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ClearTimeoutCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadBinaryFileCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn GetModuleNamespace(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn GetProxyPropertiesCallback(args: &JsNativeFunctionArgs) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        fn BroadcastCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ReceiveBroadcastCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ReportCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn GetReportCallback(args: &JsNativeFunctionArgs) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        fn SetModuleHostInfoCallbacks() -> bool;

        #[Self = "WScriptJsrt"]
        fn LoadScriptFileHelper(
            callee: JsValueRef,
            arguments: &[JsValueRef],
            is_source_module: bool,
        ) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        fn LoadScriptHelper(args: &JsNativeFunctionArgs, is_source_module: bool) -> JsValueRef;
    }

    unsafe extern "C++" {
        include!("RuntimeThreadData.h");

        type RuntimeThreadData;
        fn GetCurrentRuntimeThreadData(dummy: &mut i32) -> Pin<&mut RuntimeThreadData>;

        fn set_leaving(self: Pin<&mut RuntimeThreadData>, mLeaving: bool);
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type WScript;

        #[Self = "WScript"]
        fn initialize() -> Result<()>;
    }
}

pub struct WScript;

impl WScript {
    fn create_arguments_array() -> Result<JsArray, JsError> {
        let host_args = &HostConfigFlags::GetConfig().host_args;

        let mut args_array = ChakraRt::create_array(host_args.len() as u32)?;

        for (i, arg) in host_args.iter().enumerate() {
            let value = ChakraRt::create_string(arg)?;
            let index = ChakraRt::int_to_number(i as i32)?;
            args_array.set_indexed_property(&index, &value)?;
        }

        Ok(args_array)
    }

    #[tracing::instrument(err)]
    pub fn initialize() -> Result<(), JsError> {
        let icu_version = ffi::GetICUMajorVersion();

        let mut wscript_object = ChakraRt::create_object()?;

        wscript_object.set_named_function("monotonicNow", WScript::monotonic_now_callback)?;

        wscript_object.set_named_function("Echo", WScript::echo_callback)?;
        wscript_object.set_named_function("Quit", WScript::quit_callback)?;

        wscript_object.set_named_function("LoadScriptFile", WScript::load_script_file_callback)?;
        wscript_object.set_named_function("LoadScript", WScript::load_script_callback)?;
        wscript_object.set_named_function("LoadModule", WScript::load_module_callback)?;
        wscript_object.set_named_function("SetTimeout", WScriptJsrt::SetTimeoutCallback)?;
        wscript_object.set_named_function("ClearTimeout", WScriptJsrt::ClearTimeoutCallback)?;
        wscript_object.set_named_function("Flag", WScript::flag_callback)?;
        wscript_object.set_named_function(
            "RegisterModuleSource",
            WScript::register_module_source_callback,
        )?;
        wscript_object.set_named_function("GetModuleNamespace", WScriptJsrt::GetModuleNamespace)?;
        wscript_object.set_named_function(
            "GetProxyProperties",
            WScriptJsrt::GetProxyPropertiesCallback,
        )?;

        // Platform
        let mut platform_object = ChakraRt::create_object()?;
        let platform_property = ChakraRt::create_property_id("Platform")?;

        // Set CPU arch
        platform_object.set_property(
            ChakraRt::create_property_id("ARCH")?,
            &ChakraRt::create_string(std::env::consts::ARCH)?,
            true,
        )?;

        // Set Build Type
        platform_object.set_property(
            ChakraRt::create_property_id("BUILD_TYPE")?,
            &ChakraRt::create_string(BUILD_TYPE_STRING)?,
            true,
        )?;

        // Set Link Type [static / shared]
        platform_object.set_property(
            ChakraRt::create_property_id("LINK_TYPE")?,
            &ChakraRt::create_string("static")?,
            true,
        )?;

        // Set destination OS
        platform_object.set_property(
            ChakraRt::create_property_id("OS")?,
            &ChakraRt::create_string(std::env::consts::OS)?,
            true,
        )?;

        // set Internationalization library
        platform_object.set_property(
            ChakraRt::create_property_id("INTL_LIBRARY")?,
            &ChakraRt::create_string("icu")?,
            true,
        )?;
        platform_object.set_property(
            ChakraRt::create_property_id("ICU_VERSION")?,
            &ChakraRt::int_to_number(icu_version)?,
            false,
        )?;

        wscript_object.set_property(platform_property, &platform_object, true)?;

        wscript_object.set_property(
            ChakraRt::create_property_id("Arguments")?,
            &WScript::create_arguments_array()?,
            true,
        )?;

        let mut global_object = ChakraRt::get_global_object()?;
        global_object.set_property(
            ChakraRt::create_property_id("WScript")?,
            &wscript_object,
            true,
        )?;

        global_object.set_named_function("print", WScript::echo_callback)?;
        global_object.set_named_function("read", WScript::load_text_file_callback)?;
        global_object.set_named_function("readbuffer", WScriptJsrt::LoadBinaryFileCallback)?;

        let mut console_object = ChakraRt::create_object()?;
        console_object.set_named_function("log", WScript::echo_callback)?;

        global_object.set_property(
            ChakraRt::create_property_id("console")?,
            &console_object,
            true,
        )?;

        if !WScriptJsrt::SetModuleHostInfoCallbacks() {
            return Err(JsError::JsErrorFatal);
        }

        // When the host config `Test262` is set,
        // WScript will have the extra support API below and $262 will be
        // added to global scope
        if HostConfigFlags::GetConfig().host.test262 {
            wscript_object.set_named_function("Broadcast", WScriptJsrt::BroadcastCallback)?;

            wscript_object
                .set_named_function("ReceiveBroadcast", WScriptJsrt::ReceiveBroadcastCallback)?;
            wscript_object.set_named_function("Report", WScriptJsrt::ReportCallback)?;
            wscript_object.set_named_function("GetReport", WScriptJsrt::GetReportCallback)?;
            wscript_object.set_named_function("Leaving", WScript::leaving_callback)?;
            wscript_object.set_named_function("Sleep", WScript::sleep_callback)?;

            // $262
            let test262 = include_str!("ch/262.js");

            let test262_script_ref = ChakraRt::create_string(test262)?;
            let fname = ChakraRt::create_string("262")?;

            unsafe {
                ChakraRTInterface::JsRun(
                    test262_script_ref.clone(),
                    JsSourceContext(WScriptJsrt::GetNextSourceContext()),
                    fname.clone(),
                    JsParseScriptAttributes::JsParseScriptAttributeNone,
                    std::ptr::null_mut(),
                )
                .as_result()?;
            }
        }

        Ok(())
    }

    fn echo_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        for (i, arg) in args.arguments.iter().skip(1).enumerate() {
            let string = arg.to_string()?;
            if i > 0 {
                print!(" ");
            }
            print!("{string}");
        }

        println!();
        Ok(())
    }

    fn quit_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        let exit_code = if args.arguments.len() > 1 {
            ChakraRt::number_to_int(&args.arguments[1])?
        } else {
            0
        };
        std::process::exit(exit_code)
    }

    fn monotonic_now_callback(_: &JsNativeFunctionArgs) -> anyhow::Result<JsValueRef> {
        let ms = SystemTime::now().duration_since(UNIX_EPOCH)?.as_millis();
        let res = ChakraRt::double_to_number(ms as f64)?;
        Ok(res)
    }

    fn register_module_source_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        if args.arguments.len() < 3 {
            return Ok(());
        }

        let filename = args.arguments[1].to_string()?;
        let data = args.arguments[2].to_string()?;
        ScriptCache::add_script(filename, data);
        Ok(())
    }

    fn load_text_file_callback(args: &JsNativeFunctionArgs) -> anyhow::Result<JsString> {
        if args.arguments.len() < 2 {
            anyhow::bail!("Incorrect number of arguments.");
        }

        let filename = args.arguments[1].to_string()?;
        let file_content = ScriptCache::load_script_from_file(&filename)?;
        let value = ChakraRt::create_string(&file_content)?;
        Ok(value)
    }

    fn load_script_file_callback(args: &JsNativeFunctionArgs) -> JsValueRef {
        WScriptJsrt::LoadScriptFileHelper(args.callee.clone(), args.arguments, false)
    }

    fn load_script_callback(args: &JsNativeFunctionArgs) -> JsValueRef {
        WScriptJsrt::LoadScriptHelper(args, false)
    }

    fn load_module_callback(args: &JsNativeFunctionArgs) -> JsValueRef {
        WScriptJsrt::LoadScriptHelper(args, true)
    }

    fn flag_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        if args.arguments.len() <= 1 {
            return Ok(());
        }

        let cmd = args.arguments[1].to_string()?;
        let argv = vec![String::new(), cmd];

        TestHooks::SetConfigFlags(&argv);
        Ok(())
    }

    fn sleep_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        if args.arguments.len() > 1 {
            let timeout = ChakraRt::number_to_double(&args.arguments[1])?;
            std::thread::sleep(Duration::from_millis(timeout as u64));
        }

        Ok(())
    }

    fn leaving_callback(args: &JsNativeFunctionArgs) -> Result<(), JsError> {
        if !args.arguments.is_empty() {
            let mut v = 42;
            let runtime_thread_data = ffi::GetCurrentRuntimeThreadData(&mut v);
            runtime_thread_data.set_leaving(true);
        }

        Ok(())
    }
}

impl IntoResponse for anyhow::Error {
    fn into_response(self) -> JsValueRef {
        tracing::error!(?self);
        ChakraRt::get_undefined_value().unwrap_or_default()
    }
}
