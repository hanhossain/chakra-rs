use chakracore_sys::config::CoreConfig;
use std::process::ExitCode;

pub fn run(config: CoreConfig) -> ExitCode {
    let res = chakracore_sys::chhelper::ffi::main_internal(config);
    ExitCode::from(res as u8)
}
