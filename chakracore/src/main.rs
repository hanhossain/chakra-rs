use std::ffi::{CString, c_char};
use std::os::unix::ffi::OsStrExt;
use std::ptr;

fn main() {
    let args: Vec<CString> = std::env::args_os()
        .map(|os_str| {
            let bytes = os_str.as_bytes();
            CString::new(bytes).unwrap_or_else(|nul_error| {
                let nul_position = nul_error.nul_position();
                let mut bytes = nul_error.into_vec();
                bytes.truncate(nul_position);
                CString::new(bytes).unwrap()
            })
        })
        .collect();

    let argc = args.len();
    let mut argv: Vec<*mut c_char> = Vec::with_capacity(argc + 1);
    for arg in &args {
        argv.push(arg.as_ptr() as *mut c_char);
    }
    argv.push(ptr::null_mut());

    unsafe {
        chakracore::ffi::main_internal(argc as i32, argv.as_mut_ptr());
    }
}
