use chakra as _;

#[cxx::bridge]
pub mod ffi {
    extern "C++" {
        include!("chhelper.h");

        unsafe fn main_internal(
            argc: i32,
            argv: *mut *mut c_char,
            bin_name: &str,
            version: &str,
        ) -> i32;
    }
}
