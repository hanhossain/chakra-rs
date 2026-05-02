use std::ffi::{CString, c_char};
use std::process::ExitCode;
use std::ptr;

fn print_version() {
    println!(
        "{} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
}

pub fn main_internal(args: &[String]) -> ExitCode {
    let Some(chakra_args) = chakra::ChakraArgs::new(&args) else {
        chakra::print_usage();
        return ExitCode::FAILURE;
    };

    if chakra_args.version {
        print_version();
        return ExitCode::SUCCESS;
    }

    if chakra_args.help {
        chakra::print_usage();
        return ExitCode::SUCCESS;
    }

    if chakra_args.do_tt_record && chakra_args.do_tt_replay {
        eprintln!("Cannot run in record and replay at same time!!!");
        return ExitCode::SUCCESS;
    }

    let args: Vec<CString> = args
        .into_iter()
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
        let res = chakracore_sys::chhelper::ffi::main_internal(
            argc as i32,
            argv.as_mut_ptr(),
            chakra_args.tt_snap_interval,
            chakra_args.tt_snap_history_length,
            chakra_args.ttd_start_event_count,
            chakra_args.do_tt_record,
            chakra_args.do_tt_replay,
            chakra_args.tt_uri,
        );
        ExitCode::from(res as u8)
    }
}
