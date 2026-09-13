use crate::jsrt::{JsError, JsErrorExt, JsValueRef};
use crate::rt_interface::ChakraRTInterface;
pub use ffi::WScriptJsrt;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        include!("PlatformAgnostic/ChakraICU.h");

        type WScriptJsrt;

        #[Self = "WScriptJsrt"]
        fn Initialize(icu_version: i32, wscript: JsValueRef) -> bool;

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

        let mut wscript_object = JsValueRef::default();
        unsafe {
            ChakraRTInterface::JsCreateObject(&raw mut wscript_object).as_result()?;
        }

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

        if !WScriptJsrt::Initialize(icu_version, wscript_object) {
            return Err(JsError::JsErrorFatal);
        }

        Ok(())
    }
}
