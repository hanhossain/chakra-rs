use std::process::ExitCode;

pub fn run(args: &Vec<String>) -> ExitCode {
    let res = chakracore_sys::chhelper::ffi::main_internal(args);
    ExitCode::from(res as u8)
}
