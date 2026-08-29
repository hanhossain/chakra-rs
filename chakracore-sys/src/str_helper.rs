use widestring::U16CString;
use widestring::error::ContainsNul;

#[cxx::bridge(namespace = "chakra_rs::str_helper")]
mod ffi {
    extern "Rust" {
        fn to_lowercase(s: &String) -> String;
        fn to_raw_u16_str(s: &str) -> Result<*mut u16>;
        unsafe fn free_raw_str(p: *mut u16);
    }
}

fn to_lowercase(s: &String) -> String {
    s.to_lowercase()
}

#[tracing::instrument(level = "trace")]
fn to_raw_u16_str(s: &str) -> Result<*mut u16, ContainsNul<u16>> {
    Ok(U16CString::from_str(s)?.into_raw())
}

unsafe fn free_raw_str(p: *mut u16) {
    unsafe {
        let _ = U16CString::from_raw(p);
    }
}
