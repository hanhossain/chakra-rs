pub use ffi::GetICUMajorVersion;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("PlatformAgnostic/ChakraICU.h");

        #[namespace = "PlatformAgnostic::ICUHelpers"]
        fn GetICUMajorVersion() -> i32;
    }
}
