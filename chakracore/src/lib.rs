use std::process::ExitCode;

pub fn run(mut args: Vec<String>) -> ExitCode {
    let res = chakracore_sys::chhelper::ffi::main_internal(&mut args);
    ExitCode::from(res as u8)
}
