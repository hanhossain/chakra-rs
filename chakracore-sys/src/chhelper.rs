#[cxx::bridge]
pub mod ffi {
    extern "C++" {
        include!("chhelper.h");

        unsafe fn main_internal(
            argc: i32,
            argv: *mut *mut c_char,
            snap_interval: u32,
            snap_history_length: u32,
            start_event_count: u32,
        ) -> i32;
    }
}
