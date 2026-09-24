use crate::helpers::{ScriptCache, TestHooks};
use crate::host_config::HostConfigFlags;
use crate::jsrt::{
    ChakraRt, IntoResponse, JsArray, JsError, JsErrorCode, JsModuleHostInfoKind, JsModuleRecord,
    JsNativeFunctionArgs, JsObject, JsParseScriptAttributes, JsSourceContext, JsString, JsValueRef,
};
use crate::rt_interface::ChakraRTInterface;
use crate::wscript_jsrt::ffi::{
    CVoid, ModuleState, WScriptJsrt_CallbackMessage, WScriptJsrt_ModuleMessage,
};
pub use ffi::{MessageQueue, WScriptJsrt};
use std::collections::HashMap;
use std::hash::Hash;
use std::path::PathBuf;
use std::pin::Pin;
use std::sync::{Arc, LazyLock, RwLock};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

#[cfg(debug_assertions)]
const BUILD_TYPE_STRING: &str = "Debug";
#[cfg(not(debug_assertions))]
const BUILD_TYPE_STRING: &str = "Test";

static MODULE_ERROR_MAP: LazyLock<ModuleErrorMap> = LazyLock::new(|| ModuleErrorMap::new());
static MODULE_RECORD_MAP: LazyLock<ModuleRecordMap> = LazyLock::new(|| ModuleRecordMap::new());
static MODULE_DIRECTORY_MAP: LazyLock<ModuleDirectoryMap> =
    LazyLock::new(|| ModuleDirectoryMap::new());

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        include!("PlatformAgnostic/ChakraICU.h");

        type WScriptJsrt;

        type JsPropertyIdRef = crate::jsrt::JsPropertyIdRef;
        type JsModuleRecord = crate::jsrt::JsModuleRecord;
        type JsSourceContext = crate::jsrt::JsSourceContext;

        #[Self = "WScriptJsrt"]
        fn Uninitialize() -> bool;

        type MessageQueue;
        type MessageBase;

        #[Self = "MessageQueue"]
        fn New() -> UniquePtr<MessageQueue>;

        fn RemoveAll(self: Pin<&mut MessageQueue>);
        fn IsEmpty(self: Pin<&mut MessageQueue>) -> bool;
        fn ProcessAll(self: Pin<&mut MessageQueue>, filename: &str) -> i32;
        unsafe fn InsertSorted(self: Pin<&mut MessageQueue>, message: *mut MessageBase);

        #[Self = "WScriptJsrt"]
        unsafe fn AddMessageQueue(messageQueue: *mut MessageQueue);

        type JsValueRef = crate::jsrt::JsValueRef;
        type CVoid = crate::jsrt::CVoid;

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
        fn BroadcastCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ReceiveBroadcastCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ReportCallback(args: &JsNativeFunctionArgs) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        fn LoadScriptFileHelper(
            callee: JsValueRef,
            arguments: &[JsValueRef],
            is_source_module: bool,
        ) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        fn LoadScriptHelper(args: &JsNativeFunctionArgs, is_source_module: bool) -> JsValueRef;

        #[cxx_name = "WScriptJsrt_CallbackMessage"]
        type WScriptJsrt_CallbackMessage;
        #[Self = "WScriptJsrt_CallbackMessage"]
        fn New(time: u32, function: JsValueRef) -> UniquePtr<WScriptJsrt_CallbackMessage>;

        #[Self = "WScriptJsrt_CallbackMessage"]
        fn Upcast(msg: UniquePtr<WScriptJsrt_CallbackMessage>) -> UniquePtr<MessageBase>;

        type ModuleState;

        type WScriptJsrt_ModuleMessage;
        #[Self = "WScriptJsrt_ModuleMessage"]
        fn New(
            module: JsModuleRecord,
            specifier: JsValueRef,
        ) -> UniquePtr<WScriptJsrt_ModuleMessage>;
        #[Self = "WScriptJsrt_ModuleMessage"]
        fn NewWithPath(
            module: JsModuleRecord,
            specifier: JsValueRef,
            full_path: &str,
        ) -> UniquePtr<WScriptJsrt_ModuleMessage>;
        #[Self = "WScriptJsrt_ModuleMessage"]
        fn Upcast(msg: UniquePtr<WScriptJsrt_ModuleMessage>) -> UniquePtr<MessageBase>;

        #[Self = "WScriptJsrt"]
        unsafe fn PushMessage(message: *mut MessageBase);
    }

    unsafe extern "C++" {
        include!("RuntimeThreadData.h");

        type RuntimeThreadData;
        fn GetCurrentRuntimeThreadData(dummy: &mut i32) -> Pin<&mut RuntimeThreadData>;

        fn set_leaving(self: Pin<&mut RuntimeThreadData>, mLeaving: bool);
        fn dequeue_report(self: Pin<&mut RuntimeThreadData>, report: &mut String) -> bool;
    }

    #[namespace = "chakra_rs"]
    extern "Rust" {
        type WScript;

        #[Self = "WScript"]
        fn initialize() -> Result<()>;

        #[Self = "WScript"]
        unsafe fn promise_continuation_callback(task: JsValueRef, callback_state: *mut CVoid);

        type ModuleErrorMap;
        fn get_module_error_map() -> Box<ModuleErrorMap>;
        fn insert(self: &ModuleErrorMap, key: JsModuleRecord, value: ModuleState);
        fn clear(self: &ModuleErrorMap);
        fn get(self: &ModuleErrorMap, key: &JsModuleRecord) -> ModuleErrorMapContent;

        type ModuleRecordMap;
        fn get_module_record_map() -> Box<ModuleRecordMap>;
        fn insert(self: &ModuleRecordMap, key: String, value: ModuleRecordEntry);
        fn clear(self: &ModuleRecordMap);
        fn get(self: &ModuleRecordMap, key: &str) -> ModuleRecordMapContent;

        type ModuleDirectoryMap;
        fn get_module_directory_map() -> Box<ModuleDirectoryMap>;
        fn insert(self: &ModuleDirectoryMap, key: JsModuleRecord, value: String);
        fn clear(self: &ModuleDirectoryMap);
    }

    #[repr(i32)]
    enum ModuleState {
        RootModule,
        ImportedModule,
        ErroredModule,
    }

    #[namespace = "chakra_rs"]
    struct ModuleErrorMapContent {
        exists: bool,
        content: ModuleState,
    }

    #[namespace = "chakra_rs"]
    #[derive(Clone, Default)]
    struct ModuleRecordEntry {
        record: JsModuleRecord,
    }

    #[namespace = "chakra_rs"]
    #[derive(Clone, Default)]
    struct ModuleRecordMapContent {
        exists: bool,
        content: ModuleRecordEntry,
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
        wscript_object.set_named_function("GetModuleNamespace", WScript::get_module_namespace)?;
        wscript_object
            .set_named_function("GetProxyProperties", WScript::get_proxy_properties_callback)?;

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

        WScript::set_module_host_info_callbacks()?;

        // When the host config `Test262` is set,
        // WScript will have the extra support API below and $262 will be
        // added to global scope
        if HostConfigFlags::GetConfig().host.test262 {
            wscript_object.set_named_function("Broadcast", WScriptJsrt::BroadcastCallback)?;

            wscript_object
                .set_named_function("ReceiveBroadcast", WScriptJsrt::ReceiveBroadcastCallback)?;
            wscript_object.set_named_function("Report", WScriptJsrt::ReportCallback)?;
            wscript_object.set_named_function("GetReport", WScript::get_report_callback)?;
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

    #[tracing::instrument(skip_all, err)]
    fn get_module_namespace(args: &JsNativeFunctionArgs) -> anyhow::Result<JsValueRef> {
        anyhow::ensure!(
            args.arguments.len() >= 2,
            "Need an argument for WScript.GetModuleNamespace"
        );
        let specifier_str = args.arguments[1].to_string()?;
        let full_path = std::path::absolute(specifier_str)?;
        let guard = MODULE_RECORD_MAP.0.read().unwrap();
        let Some(module_record_entry) = guard.get(full_path.to_str().unwrap()) else {
            anyhow::bail!(
                "Need to supply a path for an already loaded module for WScript.GetModuleNamespace"
            );
        };

        match ChakraRt::get_module_namespace(&module_record_entry.record) {
            Ok(module_namespace) => Ok(module_namespace),
            Err(JsError::JsErrorModuleNotEvaluated) => {
                anyhow::bail!("GetModuleNamespace called with un-evaluated module")
            }
            Err(x) => Err(anyhow::Error::new(x)),
        }
    }

    fn get_proxy_properties_callback(
        args: &JsNativeFunctionArgs,
    ) -> Result<Option<JsObject>, JsError> {
        if args.arguments.len() <= 1 {
            return Ok(None);
        }

        let mut is_proxy = false;
        let mut target = JsValueRef::default();
        let mut handler = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsGetProxyProperties(
                args.arguments[1].clone(),
                &raw mut is_proxy,
                &raw mut target,
                &raw mut handler,
            )
            .as_result()?;
        }

        if !is_proxy {
            return Ok(None);
        }

        let target_property = ChakraRt::create_property_id("target")?;
        let handler_property = ChakraRt::create_property_id("handler")?;
        let revoked_property = ChakraRt::create_property_id("revoked")?;
        let mut obj = ChakraRt::create_object()?;
        let mut revoked = JsValueRef::default();

        unsafe {
            if target.is_null() {
                ChakraRTInterface::JsGetTrueValue(&raw mut revoked).as_result()?;
                target = ChakraRt::get_undefined_value()?;
                handler = ChakraRt::get_undefined_value()?;
            } else {
                ChakraRTInterface::JsGetFalseValue(&raw mut revoked).as_result()?;
            }
        }
        obj.set_property(handler_property, handler, true)?;
        obj.set_property(target_property, target, true)?;
        obj.set_property(revoked_property, revoked, true)?;
        Ok(Some(obj))
    }

    pub unsafe fn promise_continuation_callback(task: JsValueRef, callback_state: *mut CVoid) {
        assert!(!task.is_null());
        assert!(!callback_state.is_null());

        unsafe {
            let message_queue =
                std::mem::transmute::<*mut CVoid, *mut MessageQueue>(callback_state);
            let msg = WScriptJsrt_CallbackMessage::New(0, task);
            let msg = WScriptJsrt_CallbackMessage::Upcast(msg);

            Pin::new_unchecked(&mut *message_queue).InsertSorted(msg.into_raw());
        }
    }

    fn set_module_host_info_callbacks() -> Result<(), JsError> {
        unsafe {
            ChakraRTInterface::JsSetModuleHostInfo(
                JsModuleRecord::default(),
                JsModuleHostInfoKind::JsModuleHostInfo_FetchImportedModuleCallback,
                WScript::fetch_imported_module as _,
            )
            .as_result()?;
            ChakraRTInterface::JsSetModuleHostInfo(
                JsModuleRecord::default(),
                JsModuleHostInfoKind::JsModuleHostInfo_FetchImportedModuleFromScriptCallback,
                WScript::fetch_imported_module_from_script as _,
            )
            .as_result()?;
            ChakraRTInterface::JsSetModuleHostInfo(
                JsModuleRecord::default(),
                JsModuleHostInfoKind::JsModuleHostInfo_NotifyModuleReadyCallback,
                WScript::notify_module_ready_callback as _,
            )
            .as_result()?;
            ChakraRTInterface::JsSetModuleHostInfo(
                JsModuleRecord::default(),
                JsModuleHostInfoKind::JsModuleHostInfo_InitializeImportMetaCallback,
                WScript::initialize_import_meta_callback as _,
            )
            .as_result()?;
            ChakraRTInterface::JsSetModuleHostInfo(
                JsModuleRecord::default(),
                JsModuleHostInfoKind::JsModuleHostInfo_ReportModuleCompletionCallback,
                WScript::report_module_completion_callback as _,
            )
            .as_result()?;
        }

        Ok(())
    }

    /// Callback from chakraCore when the module resolution is finished, either successfully or unsuccessfully.
    #[tracing::instrument(skip_all)]
    fn notify_module_ready_callback(
        referencing_module: JsModuleRecord,
        exception_var: JsValueRef,
    ) -> JsErrorCode {
        if !exception_var.is_null() && HostConfigFlags::GetConfig().host.trace_host_callback {
            let mut specifier = JsValueRef::default();
            unsafe {
                ChakraRTInterface::JsGetModuleHostInfo(
                    referencing_module.clone(),
                    JsModuleHostInfoKind::JsModuleHostInfo_Url,
                    &raw mut specifier as *mut _,
                );
                let mut filename = String::new();
                if !specifier.is_null() {
                    filename = specifier.to_string().unwrap_or_default();
                }
                println!("NotifyModuleReadyCallback(exception) {filename}");
            }
        }

        let guard = MODULE_ERROR_MAP.0.read().unwrap();
        if let Some(module_error) = guard.get(&referencing_module)
            && *module_error != ModuleState::ErroredModule
        {
            let module_message =
                WScriptJsrt_ModuleMessage::New(referencing_module, JsValueRef::default());
            let msg = WScriptJsrt_ModuleMessage::Upcast(module_message);
            unsafe {
                WScriptJsrt::PushMessage(msg.into_raw());
            }
        }
        JsErrorCode::JsNoError
    }

    fn initialize_import_meta_callback(
        referencing_module: JsModuleRecord,
        import_meta_var: JsValueRef,
    ) -> JsErrorCode {
        if !import_meta_var.is_null() {
            let mut specifier = JsValueRef::default();
            unsafe {
                ChakraRTInterface::JsGetModuleHostInfo(
                    referencing_module,
                    JsModuleHostInfoKind::JsModuleHostInfo_Url,
                    &raw mut specifier as _,
                );
                if let Ok(url_prop_id) = ChakraRt::create_property_id("url") {
                    let mut import_meta_var = JsObject::new(import_meta_var);
                    let _ = import_meta_var.set_property(url_prop_id, specifier, false);
                }
            }
        }

        JsErrorCode::JsNoError
    }

    fn report_module_completion_callback(
        module: JsModuleRecord,
        exception: JsValueRef,
    ) -> JsErrorCode {
        if !exception.is_null() {
            let mut specifier = JsValueRef::default();
            unsafe {
                ChakraRTInterface::JsGetModuleHostInfo(
                    module,
                    JsModuleHostInfoKind::JsModuleHostInfo_Url,
                    &raw mut specifier as _,
                );
                if let Ok(specifier) = specifier.to_string() {
                    WScriptJsrt::PrintException(
                        &specifier,
                        JsErrorCode::JsErrorScriptException,
                        exception,
                    );
                }
            }
        }

        JsErrorCode::JsNoError
    }

    /// Callback from chakracore to fetch module dynamically during runtime. In the test harness,
    /// we are not doing any translation, just treat the specifier as fileName.
    /// While this call will come back directly from runtime script or module code, the additional
    /// task can be scheduled asynchronously that executed later.
    unsafe fn fetch_imported_module_from_script(
        #[allow(unused_variables)] referencing_source_context: JsSourceContext,
        specifier: JsValueRef,
        dependent_module_record: *mut JsModuleRecord,
    ) -> JsErrorCode {
        unsafe {
            match fetch_imported_module_helper(
                JsModuleRecord::default(),
                specifier,
                dependent_module_record,
                "",
            ) {
                Ok(()) => JsErrorCode::JsNoError,
                Err(FetchImportedModuleHelperError::JsError(err)) => err.into(),
                Err(FetchImportedModuleHelperError::IoError(_)) => JsErrorCode::JsErrorFatal,
            }
        }
    }

    /// Callback from chakracore to fetch dependent module. In the test harness,
    /// we are not doing any translation, just treat the specifier as fileName.
    /// While this call will come back directly from ParseModuleSource, the additional
    /// task are treated as Promise that will be executed later.
    unsafe fn fetch_imported_module(
        referencing_module: JsModuleRecord,
        specifier: JsValueRef,
        dependent_module_record: *mut JsModuleRecord,
    ) -> JsErrorCode {
        let directory = {
            let guard = MODULE_DIRECTORY_MAP.0.read().unwrap();
            guard
                .get(&referencing_module)
                .map(|x| x.clone())
                .unwrap_or_default()
        };
        unsafe {
            match fetch_imported_module_helper(
                JsModuleRecord::default(),
                specifier,
                dependent_module_record,
                &directory,
            ) {
                Ok(()) => JsErrorCode::JsNoError,
                Err(FetchImportedModuleHelperError::JsError(err)) => err.into(),
                Err(FetchImportedModuleHelperError::IoError(_)) => JsErrorCode::JsErrorFatal,
            }
        }
    }

    fn get_report_callback(args: &JsNativeFunctionArgs) -> Result<JsValueRef, JsError> {
        let mut return_value = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsGetNullValue(&raw mut return_value).as_result()?;
        }

        if !args.arguments.is_empty() {
            let mut v = 42;
            let thread_data = ffi::GetCurrentRuntimeThreadData(&mut v);
            let mut report = String::new();
            if thread_data.dequeue_report(&mut report) {
                unsafe {
                    ChakraRTInterface::JsCreateString(&report, &raw mut return_value)
                        .as_result()?;
                }
            }
        }

        Ok(return_value)
    }
}

impl IntoResponse for anyhow::Error {
    fn into_response(self) -> JsValueRef {
        tracing::error!(?self);

        // If the exception is already is set - no need to create a new exception.
        let has_exception = ChakraRt::has_exception();
        if has_exception.is_err() || !has_exception.unwrap() {
            if let Err(err) = ChakraRt::create_string(&self.to_string())
                .and_then(|msg| ChakraRt::create_error(msg))
                .and_then(|error| ChakraRt::set_exception(error))
            {
                tracing::error!(?err, "Failed to set an exception");
            }
        }
        ChakraRt::get_undefined_value().unwrap_or_default()
    }
}

// TODO: error can be a field in ModuleRecordEntry instead of its own hashmap
type ModuleErrorMap = ConcurrentMap<JsModuleRecord, ModuleState>;
type ModuleRecordMap = ConcurrentMap<String, ffi::ModuleRecordEntry>;
type ModuleDirectoryMap = ConcurrentMap<JsModuleRecord, String>;

#[derive(Clone)]
struct ConcurrentMap<K, V>(Arc<RwLock<HashMap<K, V>>>);

impl<K, V> ConcurrentMap<K, V>
where
    K: Eq + Hash,
{
    fn new() -> Self {
        ConcurrentMap(Arc::new(RwLock::new(HashMap::new())))
    }

    fn insert(&self, key: K, value: V) {
        let mut guard = self.0.write().unwrap();
        guard.insert(key, value);
    }

    fn clear(&self) {
        let mut guard = self.0.write().unwrap();
        guard.clear();
    }
}

impl ModuleErrorMap {
    fn get(&self, key: &JsModuleRecord) -> ffi::ModuleErrorMapContent {
        let guard = self.0.read().unwrap();
        match guard.get(key) {
            Some(x) => ffi::ModuleErrorMapContent {
                exists: true,
                content: *x,
            },
            None => ffi::ModuleErrorMapContent {
                exists: false,
                content: ModuleState::RootModule,
            },
        }
    }
}

impl ModuleRecordMap {
    fn get(&self, key: &str) -> ffi::ModuleRecordMapContent {
        let guard = self.0.read().unwrap();
        guard
            .get(key)
            .map(|x| ffi::ModuleRecordMapContent {
                exists: true,
                content: x.clone(),
            })
            .unwrap_or_default()
    }
}

fn get_module_error_map() -> Box<ModuleErrorMap> {
    Box::new(MODULE_ERROR_MAP.clone())
}

fn get_module_record_map() -> Box<ModuleRecordMap> {
    Box::new(MODULE_RECORD_MAP.clone())
}

fn get_module_directory_map() -> Box<ModuleDirectoryMap> {
    Box::new(MODULE_DIRECTORY_MAP.clone())
}

#[tracing::instrument(skip_all, fields(ref_dir), err)]
unsafe fn fetch_imported_module_helper(
    referencing_module: JsModuleRecord,
    specifier: JsValueRef,
    dependent_module_record: *mut JsModuleRecord,
    ref_dir: &str,
) -> Result<(), FetchImportedModuleHelperError> {
    unsafe {
        *dependent_module_record = JsModuleRecord::default();
    }
    let specifier_str = specifier.to_string()?;
    let mut specifier_full_path = PathBuf::from(ref_dir);
    specifier_full_path.push(&specifier_str);
    let abs_path = std::fs::canonicalize(&specifier_full_path).or_else(|err| {
        tracing::warn!(?specifier_full_path, ?err, "Falling back to absolute path.");
        std::path::absolute(&specifier_full_path)
    })?;
    tracing::trace!(specifier_str, ?abs_path);
    let parent_path = abs_path
        .parent()
        .map(|x| x.to_str())
        .flatten()
        .unwrap_or_default()
        .to_owned();

    {
        let lease = MODULE_RECORD_MAP.0.read().unwrap();
        if let Some(entry) = lease.get(abs_path.to_str().unwrap_or_default()) {
            unsafe {
                *dependent_module_record = entry.record.clone();
                return Ok(());
            }
        }
    }

    let mut module_record = JsModuleRecord::default();
    unsafe {
        ChakraRTInterface::JsInitializeModuleRecord(
            referencing_module.clone(),
            specifier.clone(),
            &raw mut module_record,
        )
        .as_result()?;
    }

    MODULE_DIRECTORY_MAP
        .0
        .write()
        .unwrap()
        .insert(module_record.clone(), parent_path);

    MODULE_RECORD_MAP.0.write().unwrap().insert(
        abs_path.to_str().unwrap_or_default().to_owned(),
        ffi::ModuleRecordEntry {
            record: module_record.clone(),
        },
    );

    MODULE_ERROR_MAP
        .0
        .write()
        .unwrap()
        .insert(module_record.clone(), ModuleState::ImportedModule);

    let module_message = WScriptJsrt_ModuleMessage::NewWithPath(
        referencing_module,
        specifier,
        abs_path.to_str().unwrap_or_default(),
    );
    let module_message = WScriptJsrt_ModuleMessage::Upcast(module_message);
    unsafe {
        WScriptJsrt::PushMessage(module_message.into_raw());
        *dependent_module_record = module_record;
    }

    Ok(())
}

#[derive(thiserror::Error, Debug)]
enum FetchImportedModuleHelperError {
    #[error(transparent)]
    JsError(#[from] JsError),
    #[error(transparent)]
    IoError(#[from] std::io::Error),
}
