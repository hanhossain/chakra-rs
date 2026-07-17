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
        let res = chakracore_sys::chhelper::ffi::main_internal(argc as i32, argv.as_mut_ptr());
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

#[derive(Debug, Clone, Default)]
pub struct ChakraArgs {
    pub version: bool,
    pub help: bool,
}

impl ChakraArgs {
    pub fn new(args: &mut Vec<String>) -> Option<Self> {
        if args.len() < 2 {
            return None;
        }

        let chakra_args = ChakraArgs::default();

        for arg in args {
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
        }

        Some(chakra_args)
    }
}
