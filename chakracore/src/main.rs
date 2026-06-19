use std::env::current_exe;
use std::ffi::{CString, c_char};
use std::process::ExitCode;
use std::ptr;

fn main() -> ExitCode {
    let mut args: Vec<_> = std::env::args().collect();
    let Some(chakra_args) = ChakraArgs::new(&mut args) else {
        chakracore_sys::chhelper::print_usage();
        return ExitCode::FAILURE;
    };

    if chakra_args.version {
        print_version();
        return ExitCode::SUCCESS;
    }

    if chakra_args.help {
        chakracore_sys::chhelper::print_usage();
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

fn print_version() {
    println!(
        "{} version {}",
        env!("CARGO_PKG_NAME"),
        env!("CARGO_PKG_VERSION")
    );
}

fn get_ttd_directory(record_uri: String) -> String {
    let exe = current_exe().unwrap();
    let exe_dir = exe.parent().unwrap().to_owned();

    let directory = exe_dir.join("_ttdlog").join(record_uri);
    std::fs::create_dir_all(&directory).unwrap();
    directory.to_string_lossy().into_owned()
}

#[derive(Debug, Clone, Default)]
pub struct ChakraArgs {
    pub version: bool,
    pub help: bool,
    pub tt_snap_interval: u32,
    pub tt_snap_history_length: u32,
    pub ttd_start_event_count: u32,
    pub do_tt_record: bool,
    pub do_tt_replay: bool,
    pub tt_uri: String,
}

impl ChakraArgs {
    pub fn new(args: &mut Vec<String>) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        let mut chakra_args = ChakraArgs::default();
        let mut tt_indices = Vec::new();

        for (index, arg) in args.iter().enumerate() {
            if arg == "-v" || arg == "--version" {
                return Some(ChakraArgs {
                    version: true,
                    ..ChakraArgs::default()
                });
            }

            if arg == "-h" || arg == "--help" {
                return Some(ChakraArgs {
                    help: true,
                    ..ChakraArgs::default()
                });
            }

            if arg.starts_with("-TTRecord=") {
                chakra_args.do_tt_record = true;
                let record_uri = arg.split('=').next().unwrap_or_default().to_owned();
                chakra_args.tt_uri = get_ttd_directory(record_uri);
                tt_indices.push(index);
            } else if arg.starts_with("-TTReplay=") {
                chakra_args.do_tt_replay = true;
                let record_uri = arg.split('=').next().unwrap_or_default().to_owned();
                chakra_args.tt_uri = get_ttd_directory(record_uri);
                tt_indices.push(index);
            } else if arg.starts_with("-TTSnapInterval=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_interval = raw_value.parse::<u32>().unwrap_or(u32::MAX);
                tt_indices.push(index);
            } else if arg.starts_with("-TTHistoryLength=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.tt_snap_history_length = raw_value.parse::<u32>().unwrap_or(u32::MAX);
                tt_indices.push(index);
            } else if arg.starts_with("-TTDStartEvent=") {
                let raw_value = arg.split('=').next().unwrap_or_default();
                chakra_args.ttd_start_event_count = raw_value.parse::<u32>().unwrap_or(1);
                tt_indices.push(index);
            }
        }

        for index in tt_indices.into_iter().rev() {
            args.remove(index);
        }

        Some(chakra_args)
    }
}
