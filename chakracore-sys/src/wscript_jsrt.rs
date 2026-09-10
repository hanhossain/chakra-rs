#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("WScriptJsrt.h");
        type WScriptJsrt;

        #[Self = "WScriptJsrt"]
        fn Initialize() -> bool;

        #[Self = "WScriptJsrt"]
        fn Uninitialize() -> bool;
    }
}
