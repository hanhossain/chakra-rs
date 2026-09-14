use crate::jsrt::{ChakraRt, JsError, JsErrorExt, JsValueRef};
pub use ffi::WScriptJsrt;

#[cfg(target_arch = "aarch64")]
const CPU_ARCH_TEXT: &str = "ARM64";
#[cfg(target_arch = "x86_64")]
const CPU_ARCH_TEXT: &str = "x86_64";

#[cfg(debug_assertions)]
const BUILD_TYPE_STRING: &str = "Debug";
#[cfg(not(debug_assertions))]
const BUILD_TYPE_STRING: &str = "Test";

#[cfg(target_os = "macos")]
const DEST_PLATFORM_TEXT: &str = "darwin";
#[cfg(target_os = "linux")]
const DEST_PLATFORM_TEXT: &str = "posix";

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        include!("PlatformAgnostic/ChakraICU.h");

        type WScriptJsrt;

        type JsPropertyIdRef = crate::jsrt::JsPropertyIdRef;
        #[Self = "WScriptJsrt"]
        fn Initialize(wscript: &mut JsValueRef) -> bool;

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
        #[Self = "WScriptJsrt"]
        fn InstallObjectsOnObject(
            object: &mut JsValueRef,
            name: &str,
            native_functions: fn(&JsNativeFunctionArgs) -> JsValueRef,
        ) -> JsErrorCode;

        #[namespace = "PlatformAgnostic::ICUHelpers"]
        fn GetICUMajorVersion() -> i32;

        #[Self = "WScriptJsrt"]
        fn MonotonicNowCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn EchoCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn QuitCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadScriptFileCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadScriptCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadModuleCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn SetTimeoutCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ClearTimeoutCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn AttachCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn DetachCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadBinaryFileCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn LoadTextFileCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn FlagCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn RegisterModuleSourceCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn GetModuleNamespace(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn GetProxyPropertiesCallback(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn SerializeObject(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn Deserialize(args: &JsNativeFunctionArgs) -> JsValueRef;
        #[Self = "WScriptJsrt"]
        fn ReadLineStdinCallback(args: &JsNativeFunctionArgs) -> JsValueRef;

        #[Self = "WScriptJsrt"]
        unsafe fn CreateArgumentsObject(argsObject: *mut JsValueRef) -> bool;
        #[Self = "WScriptJsrt"]
        fn SetModuleHostInfoCallbacks() -> bool;
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
    #[tracing::instrument(err)]
    pub fn initialize() -> Result<(), JsError> {
        let icu_version = ffi::GetICUMajorVersion();

        let mut wscript_object = ChakraRt::create_object()?;

        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "monotonicNow",
            WScriptJsrt::MonotonicNowCallback,
        )
        .as_result()?;

        WScriptJsrt::InstallObjectsOnObject(&mut wscript_object, "Echo", WScriptJsrt::EchoCallback)
            .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(&mut wscript_object, "Quit", WScriptJsrt::QuitCallback)
            .as_result()?;

        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "LoadScriptFile",
            WScriptJsrt::LoadScriptFileCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "LoadScript",
            WScriptJsrt::LoadScriptCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "LoadModule",
            WScriptJsrt::LoadModuleCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "SetTimeout",
            WScriptJsrt::SetTimeoutCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "ClearTimeout",
            WScriptJsrt::ClearTimeoutCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "Attach",
            WScriptJsrt::AttachCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "Detach",
            WScriptJsrt::DetachCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "LoadBinaryFile",
            WScriptJsrt::LoadBinaryFileCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "LoadTextFile",
            WScriptJsrt::LoadTextFileCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(&mut wscript_object, "Flag", WScriptJsrt::FlagCallback)
            .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "RegisterModuleSource",
            WScriptJsrt::RegisterModuleSourceCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "GetModuleNamespace",
            WScriptJsrt::GetModuleNamespace,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "GetProxyProperties",
            WScriptJsrt::GetProxyPropertiesCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "SerializeObject",
            WScriptJsrt::SerializeObject,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut wscript_object,
            "Deserialize",
            WScriptJsrt::Deserialize,
        )
        .as_result()?;

        // Platform
        let mut platform_object = ChakraRt::create_object()?;
        let platform_property = ChakraRt::create_property_id("Platform")?;

        // Set CPU arch
        platform_object.set_property(
            ChakraRt::create_property_id("ARCH")?,
            &ChakraRt::create_string(CPU_ARCH_TEXT)?,
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
            &ChakraRt::create_string(DEST_PLATFORM_TEXT)?,
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

        let mut args_array = JsValueRef::default();
        unsafe {
            if !WScriptJsrt::CreateArgumentsObject(&raw mut args_array) {
                return Err(JsError::JsErrorFatal);
            }
        }

        wscript_object.set_property(
            ChakraRt::create_property_id("Arguments")?,
            &args_array,
            true,
        )?;

        let mut global_object = ChakraRt::get_global_object()?;
        global_object.set_property(
            ChakraRt::create_property_id("WScript")?,
            &wscript_object,
            true,
        )?;

        WScriptJsrt::InstallObjectsOnObject(&mut global_object, "print", WScriptJsrt::EchoCallback)
            .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut global_object,
            "read",
            WScriptJsrt::LoadTextFileCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut global_object,
            "readbuffer",
            WScriptJsrt::LoadBinaryFileCallback,
        )
        .as_result()?;
        WScriptJsrt::InstallObjectsOnObject(
            &mut global_object,
            "readline",
            WScriptJsrt::ReadLineStdinCallback,
        )
        .as_result()?;

        let mut console_object = ChakraRt::create_object()?;
        WScriptJsrt::InstallObjectsOnObject(&mut console_object, "log", WScriptJsrt::EchoCallback)
            .as_result()?;

        global_object.set_property(
            ChakraRt::create_property_id("console")?,
            &console_object,
            true,
        )?;

        if !WScriptJsrt::SetModuleHostInfoCallbacks() {
            return Err(JsError::JsErrorFatal);
        }

        if !WScriptJsrt::Initialize(&mut wscript_object) {
            return Err(JsError::JsErrorFatal);
        }

        Ok(())
    }
}
