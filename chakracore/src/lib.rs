use chakracore_sys::chhelper::ffi::ExecuteTestWithMemoryCheck;
use chakracore_sys::config::CoreConfig;
use chakracore_sys::host_config::ffi::HostConfigFlags;
use chakracore_sys::rt_interface::ffi::ChakraRTInterface;
use std::process::ExitCode;

pub fn run(config: CoreConfig) -> ExitCode {
    HostConfigFlags::SetHostArgs(&config.host_args);

    // handle command line flags
    ChakraRTInterface::InitializeTestHooks(&config.args);

    let res = ExecuteTestWithMemoryCheck(&config.filename);
    ExitCode::from(res as u8)
}
