pub use ffi::Helpers;

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("Helpers.h");

        type Helpers;
        #[Self = "Helpers"]
        fn LoadScriptFromFile(filename: &str) -> Result<String>;
    }
}
