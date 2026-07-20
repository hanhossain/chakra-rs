use std::process::ExitCode;

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

    chakracore::run(args)
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
