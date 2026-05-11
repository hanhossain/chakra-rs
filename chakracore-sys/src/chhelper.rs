#[cxx::bridge]
pub mod ffi {
    extern "C++" {
        include!("chhelper.h");

        type c_char16_t;

        unsafe fn main_internal(
            argc: i32,
            argv: *mut *mut c_char,
            snap_interval: u32,
            snap_history_length: u32,
            start_event_count: u32,
            do_tt_record: bool,
            do_tt_replay: bool,
            tt_uri: String,
        ) -> i32;

        unsafe fn get_hello_world() -> *const c_char16_t;
    }
}
