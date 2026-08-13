use chakracore_sys::config::CoreConfig;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use std::process::ExitCode;

pub fn run(config: CoreConfig) -> ExitCode {
    HostConfigFlags::SetHostArgs(&config.host_args);
    let res = chakracore_sys::chhelper::ffi::main_internal(config);
    ExitCode::from(res as u8)
}
