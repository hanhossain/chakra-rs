pub use ffi::Abstractions;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("Util/Abstractions.h");

        type Abstractions;
        #[Self = "Abstractions"]
        fn IsDebuggerPresent() -> bool;
    }
}
